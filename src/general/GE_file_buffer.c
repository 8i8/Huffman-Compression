#include <stdlib.h>
#include <string.h>
#include "general/GE_file_buffer.h"
#include "general/GE_state.h"
#include "general/GE_string.h"

#define BUF   BUFFER_SIZE - 1      /* Buffer with a margin */

int back = 1;                      /* Stop ungetc of nore than one c */

/*
 * GE_buffer_array_init: Initialise and return an array of pointers to file
 * buffers.
 */
F_Buf **GE_buffer_array_init(void)
{
	return calloc(MAX_FILES, sizeof(F_Buf*));
}

/*
 * GE_buffer_init: Initialise a FILE buffer.
 */
F_Buf *GE_buffer_init(FILE *fp, char *name)
{
	F_Buf *buf;
	buf = malloc(sizeof(F_Buf));
	memcpy(buf->name, name, strlen(name)+1);
	buf->fp = fp;
	buf->eof = 0;
	buf->buf = buf->read = buf->ptr = buf->end = NULL;
	back = 0;
	return buf;
}

/*
 * open_file: Open a file.
 */
unsigned GE_open_file(char *name, F_Buf **io, char *mode, const int state)
{
	FILE *fp;
	int i;
	char *pt;
	String *str = NULL;

	for (i = 0; i < MAX_FILES && io[i]; i++)
		;

	if (i >= MAX_FILES) {
		fprintf(stderr, "%s: file limit of %d files exceeded.", __func__, MAX_FILES);
		return 1;
	}

	/* Before opening for writing, check if the file already exists */
	if (!is_set(state, FORCE) && (pt = strchr(mode, 'w')) && pt[0] == 'w') {
		str = GE_string_init(str);
		if ((fp = fopen(name, mode)) != NULL) {
			fprintf(stdout, "Overwrite %s ? [y/n] ", name);
			GE_string_getchar(str);
			if (str->str[0] != 'y') {
				GE_string_free(str);
				return 1;
			}
		}
		GE_string_free(str);
	}

	/* Open file and store if successful */
	if ((fp = fopen(name, mode)) == NULL) {
		fprintf(stderr, "file read error: %s\n", name);
		return 1;
	} else
		io[i]= GE_buffer_init(fp, name);;

	return 0;
}


/*
 * GE_buffer_on: allocate memory for file buffer.
 */
F_Buf *GE_buffer_on(F_Buf *buf)
{
	buf->read = buf->ptr = buf->buf = malloc(BUFFER_SIZE);
	buf->end = buf->buf + BUF;
	return buf;
}

/*
 * GE_buffer_off: Free only the file buffers momory.
 */
F_Buf *GE_buffer_off(F_Buf *buf)
{
	free(buf->buf);
	buf->buf = buf->ptr = buf->end = NULL;
	return buf;
}

/*
 * GE_buffer_rewind: Rewind file pointer.
 */
F_Buf *GE_buffer_rewind(F_Buf *buf)
{
	buf->eof = 0;
	rewind(buf->fp);
	return buf;
}

/*
 * GE_buffer_fill: Fills the buffer from the input stream, allowing a margin of
 * space to complete any utf char that may have been started.
 */
F_Buf *GE_buffer_fill(F_Buf *buf)
{
	size_t len, written;
	len = written = 0;
	back = 0;
	/* If the buffer is at its end, back to the start */
	if (buf->ptr == buf->end)
		buf->ptr = buf->read = buf->buf;
	len = BUF - (buf->ptr - buf->buf);

	written = fread(buf->ptr, 1, len, buf->fp);
	buf->ptr += written;

	if (written != len)
		buf->eof = EOF, *buf->ptr = EOF;

	return buf;
}

/*
 * GE_buffer_fwrite: Buffer the writing to a file pointer.
 */
F_Buf *GE_buffer_fwrite(char *str, size_t size, size_t num, F_Buf *buf)
{
	size_t space, len;
	space = BUF - (buf->ptr - buf->buf);
	len = size * num;

	if (len < space) {
		memcpy(buf->ptr, str, len);
		buf->ptr += len;
	} else {
		memcpy(buf->ptr, str, space);
		fwrite(buf->buf, 1, buf->ptr + space - buf->buf, buf->fp);
		buf->ptr = buf->buf, str += space;
		len -= space;
		memcpy(buf->ptr, str, len);
		buf->ptr += len;
	}

	return buf;
}

/*
 * GE_buffer_fwrite: Write buffer content to file pointer.
 */
F_Buf *GE_buffer_fwrite(F_Buf *buf)
{
	fwrite(buf->buf, 1, buf->ptr - buf->buf, buf->fp);
	buf->ptr = buf->buf;
	back = 0;
	return buf;
}

/*
 * GE_buffer_getc: Returns the next char from the buffer, refilling when
 * required.
 */
int GE_buffer_fgetc(F_Buf *buf)
{
	if (buf->read < buf->ptr || *buf->read == EOF ) {
		back = 1;
		return *buf->read++;
	} else
		buf = GE_buffer_fill(buf);

	return GE_buffer_fgetc(buf);
}

/*
 * GE_buffer_skip: Skip over n cahr in the buffer.
 */
int GE_buffer_skip(F_Buf *buf, unsigned num)
{
	size_t i;
	for (i = 0; i < num && (buf->read < buf->ptr || *buf->read == EOF); i++)
		buf->read++;
	back = 1;

	if (i == num)
		return *buf->read++;
	else
		buf = GE_buffer_fill(buf);

	return GE_buffer_skip(buf, num - i);
}

/*
 * GE_buffer_ungetc: Push back unused character.
 */
int GE_buffer_ungetc(int c, F_Buf *buf)
{
	if (buf->read > buf->buf)
		buf->read--;
	else if (back)
		ungetc(c, buf->fp), back--;

	return 0;
}

/*
 * GE_buffer_free: Free all memory associated with a F_Buf struct.
 */
void GE_buffer_free(F_Buf *buf)
{
	free(buf->buf);
	free(buf);
}

