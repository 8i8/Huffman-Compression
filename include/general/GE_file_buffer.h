#include <stdio.h>

#ifndef _file_buffer_
#define _file_buffer_
typedef struct _file_buffer {
	FILE *fp;
	char name[255];
	char *buf;
	char *ptr;
	char *read;
	char *end;
	int tab_depth;
	int eof;
} F_Buf;
#endif

#define BUFFER_SIZE     20000
#define MAX_FILES       20

/*
 * GE_buffer_array_init: Initialise and return an array of pointers to file
 * buffers.
 */
F_Buf **GE_buffer_array_init(void);

/*
 * GE_buffer_init: Initialise a FILE buffer.
 */
F_Buf *GE_buffer_init(FILE *fp, char *name);

/*
 * open_file: Open a file and add it to the F_Buf array.
 */
unsigned GE_open_file(char *name, F_Buf **io, char *mode, const int state);

/*
 * GE_buffer_on: allocate memory for file buffer.
 */
F_Buf *GE_buffer_on(F_Buf *buf);

/*
 * GE_buffer_off: Free only the file buffers momory.
 */
F_Buf *GE_buffer_off(F_Buf *buf);

/*
 * GE_buffer_rewind: Rewind file pointer.
 */
F_Buf *GE_buffer_rewind(F_Buf *buf);

/*
 * GE_buffer_fill: Fills the buffer from the input stream, allowing a margin of
 * space to complete any utf char that may have been started.
 */
F_Buf *GE_buffer_fill(F_Buf *buf);

/*
 * GE_buffer_fwrite: Buffer the writing to a file pointer.
 */
F_Buf *GE_buffer_fwrite(char *str, size_t size, size_t num, F_Buf *buf);

/*
 * GE_buffer_fwrite: Write buffer content to file pointer.
 */
F_Buf *GE_buffer_fwrite_FILE(F_Buf *buf);

/*
 * GE_buffer_getc: Returns the next char from the buffer, refilling when
 * required.
 */
int GE_buffer_fgetc(F_Buf *buf);

/*
 * GE_buffer_skip: Skip over n cahr in the buffer.
 */
int GE_buffer_skip(F_Buf *buf, unsigned num);

/*
 * GE_buffer_ungetc: Push back unused character.
 */
int GE_buffer_ungetc(int c, F_Buf *buf);

/*
 * GE_buffer_free: Free all memory associated with a F_Buf struct.
 */
void GE_buffer_free(F_Buf *buf);

