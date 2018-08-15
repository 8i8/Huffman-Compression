#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "general/GE_file_buffer.h"
#include "general/GE_error.h"
#include "general/GE_state.h"
#include "general/GE_string.h"

// TODO NEXT Add a system of FILE type to the internal buffer state.

#define BUF   BUFFER_SIZE - 1      /* Buffer with a margin fo stop issues with
				      the end of buffer overflow  and the null
				      terminator */
#define BUF_BACK_AVAIL  (1 <<  0)
#define BUF_HOLD        (1 <<  1)
#define BUF_EXISTS      (1 <<  2)
#define BUF_UNGETC      (1 <<  3)

/*
 * GE_buffer_init_array: Initialise and return an array of pointers to file
 * buffers.
 */
F_Buf **GE_buffer_init_array(void)
{
	return calloc(MAX_FILES, sizeof(F_Buf*));
}

/*
 * GE_buffer_init_alloc: Initialise a FILE buffer.
 */
F_Buf *GE_buffer_init_alloc(FILE *fp, char *name)
{
	F_Buf *buf;
	buf = malloc(sizeof(F_Buf));
	memcpy(buf->name, name, strlen(name)+1);
	buf->fp = fp;
	buf->eof = 0;
	buf->buf = buf->read = buf->ptr = buf->end = NULL;
	buf->hold = NULL;
	buf->tab_depth = 0;
	buf->st_buf = state_init();
	return buf;
}

/*
 * GE_buffer_init: Initialise a FILE buffer.
 */
F_Buf *GE_buffer_init(F_Buf *buf, FILE *fp, char *name)
{
	memcpy(buf->name, name, strlen(name)+1);
	buf->fp = fp;
	buf->eof = 0;
	buf->buf = buf->read = buf->ptr = buf->end = NULL;
	buf->hold = NULL;
	buf->tab_depth = 0;
	buf->st_buf = state_init();
	return buf;
}

/*
 * GE_file_clear: empty the file, used berfor file open with append.
 */
unsigned GE_file_clear(char *name)
{
	if (fopen(name, "w") == NULL) {
		FAIL("file read failed");
		return 1;
	}
	return 0;
}

/*
 * GE_buffer_fopen: Open file for file buffer.
 */
unsigned GE_buffer_fopen(F_Buf *buf, char *name, char *mode, const int st_prg)
{
	FILE *fp;
	String *str = NULL;

	/* Ask first before overwriting */
	if (!is_set(st_prg, FORCE) 
						&& (!strcmp(mode, "w") 
						|| !strcmp(mode, "wb") 
						|| !strcmp(mode, "r+")))
	{
		str = GE_string_init(str);

		if ((fp = fopen(name, "r")) != NULL)
		{
			fprintf(stdout, "Overwrite %s ? [y/n] ", name);
			GE_string_getchar(str);
			if (str->str[0] != 'y') {
				fclose(fp);
				GE_string_free(str);
				return 1;
			}
		} else
			FAIL("File open failed");

		GE_string_free(str);
	}

	/* Open file */
	if ((fp = fopen(name, mode)) == NULL) {
		FAIL("file read failed");
		return 1;
	} else
		buf = GE_buffer_init(buf, fp, name);

	return 0;
}

/*
 * GE_buffer_fopen_array: Open a file.
 */
unsigned GE_buffer_fopen_array(F_Buf **io, char *name, char *mode, const int st_prg)
{
	int i;

	/* Next available slot */
	for (i = 0; i < MAX_FILES && io[i]; i++)
		;

	/* To many */
	if (i >= MAX_FILES) {
		FAIL("file limit exceeded");
		return 1;
	}

	/* allocate and try for a file */
	io[i] = malloc(sizeof(F_Buf));
	if (GE_buffer_fopen(io[i], name, mode, st_prg)) {
		free(io[i]);
		return 1;
	}

	return 0;
}

/*
 * GE_buffer_fclose: Clode a buffered file.
 */
F_Buf *GE_buffer_fclose(F_Buf *buf)
{
	if (buf->fp)
		fclose(buf->fp);
	return buf;
}

/*
 * GE_buffer_on: allocate memory for file buffer.
 */
F_Buf *GE_buffer_on(F_Buf *buf)
{
	buf->read = buf->ptr = buf->buf = malloc(BUFFER_SIZE);
	buf->end = buf->buf + BUF;
	state_set(buf->st_buf, BUF_EXISTS);
	return buf;
}

/*
 * GE_buffer_off: Free only the file buffers momory.
 */
F_Buf *GE_buffer_off(F_Buf *buf)
{
	free(buf->buf);
	buf->buf = buf->ptr = buf->end = NULL;
	state_unset(buf->st_buf, BUF_EXISTS);
	return buf;
}

/*
 * GE_buffer_rewind: Rewind file pointer.
 */
F_Buf *GE_buffer_rewind(F_Buf *buf)
{
	buf->eof = 0;
	rewind(buf->fp);
	state_unset(buf->st_buf, BUF_BACK_AVAIL);
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

	state_unset(buf->st_buf, BUF_UNGETC);
	state_unset(buf->st_buf, BUF_BACK_AVAIL);

	/* If the buffer is at its end, back to the start */
	if (buf->ptr == buf->end) {
		buf->ptr = buf->read = buf->buf;
		state_unset(buf->st_buf, BUF_BACK_AVAIL);
	}

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
	if (buf == NULL) {
		FAIL("F_Buf: NULL pointer recieved");
		return NULL;
	} else if (buf->ptr == NULL) {
		FAIL("F_Buf->ptr: NULL pointer recieved");
		return NULL;
	}

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
F_Buf *GE_buffer_fwrite_FILE(F_Buf *buf)
{
	fwrite(buf->buf, 1, buf->ptr - buf->buf, buf->fp);
	buf->ptr = buf->buf;
	state_unset(buf->st_buf, BUF_BACK_AVAIL);
	return buf;
}

/*
 * GE_buffer_getc: Returns the next char from the buffer, refilling when
 * required.
 * TODO NEXT is there any way around these state changes fgetc?
 */
int GE_buffer_fgetc(F_Buf *buf)
{
	if (buf->read < buf->ptr || *buf->read == EOF ) {
		state_unset(buf->st_buf, BUF_UNGETC);
		state_set(buf->st_buf, BUF_BACK_AVAIL);
		return *buf->read++;
	} else
		buf = GE_buffer_fill(buf);

	return GE_buffer_fgetc(buf);
}

/*
 * GE_buffer_skip: Skip over n char in the buffer.
 */
int GE_buffer_skip(F_Buf *buf, unsigned num)
{
	size_t i;
	for (i = 0; i < num && (buf->read < buf->ptr || *buf->read == EOF); i++)
		buf->read++;
	state_unset(buf->st_buf, BUF_UNGETC);
	state_set(buf->st_buf, BUF_BACK_AVAIL);

	if (i == num)
		return *buf->read++;
	else
		buf = GE_buffer_fill(buf);

	return GE_buffer_skip(buf, num - i);
}

/*
 * GE_buffer_ungetc: Push back unused character, can be used untill the buffer
 * reaches its entry point, fron where only one more push can be assured, the
 * distance from the read head to the buffer start is returned.
 * TODO NEXT ungetc added is it working here? This needs to be tested.
 */
int GE_buffer_ungetc(int c, F_Buf *buf)
{
	ptrdiff_t diff;
	if ((diff = buf->read - buf->buf) > 0) {
		if (--buf->read && --diff == 0)
			state_unset(buf->st_buf, BUF_BACK_AVAIL);
		return diff;
	}

	if (!is_set(buf->st_buf, BUF_UNGETC)) {
		state_set(buf->st_buf, BUF_UNGETC);
		return ungetc(c, buf->fp);
	} else {
		WARNING("ungetc: only one pushed back char is assured");
		return ungetc(c, buf->fp);
	}

	return 0;
}

/*
 * GE_buffer_pushback_mark: Set mark for a potential pushback.
 */
int GE_buffer_pushback_mark(F_Buf *buf)
{
	if (is_set(buf->st_buf, BUF_HOLD)) {
		FAIL("F_Buf already held");
		return 1;
	}

	buf->hold = buf->read;
	state_set(buf->st_buf, BUF_HOLD);

	return 0;
}

/*
 * GE_pushback_unmark: Remove previously placed pushback point.
 */
int GE_buffer_pushback_unmark(F_Buf *buf)
{
	if (!is_set(buf->st_buf, BUF_HOLD)) {
		FAIL("F_Buf not held");
		return 1;
	}

	buf->hold = NULL;
	state_unset(buf->st_buf, BUF_HOLD);

	return 0;
}

/*
 * GE_buffer_pushback_goto: Return to pushback marker.
 */
int GE_buffer_pushback_goto(F_Buf *buf)
{
	if (!is_set(buf->st_buf, BUF_HOLD)) {
		FAIL("F_Buf not held");
		return 1;
	}

	buf->read = buf->hold;
	buf->hold = NULL;
	state_unset(buf->st_buf, BUF_HOLD);

	return *buf->read;
}

/*
 * GE_buffer_free: Free all memory associated with a F_Buf struct.
 */
void GE_buffer_free(F_Buf *buf)
{
	if (buf->fp)
		fclose(buf->fp);
	free(buf->buf);
	free(buf);
}

