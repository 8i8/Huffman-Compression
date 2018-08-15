#include <stdio.h>

#ifndef _file_buffer_
#define _file_buffer_
typedef struct _file_buffer {
	FILE *fp;
	char name[255];
	char *buf;
	char *ptr;
	char *read;
	char *hold;
	char *end;
	int tab_depth;
	int eof;
	int st_buf;
} F_Buf;
#endif

#define MAX_FILES       20
#define BUFFER_SIZE     4000  /* Should be relativly small, here at around the
				 size of the linux pagefile, absolutly should be
				 lower than the systems available ram and never
				 anywhere near the value of a 32 bit int. */

/* GE_buffer_init_array: Initialise and return an array of pointers to file
 * buffers */
F_Buf **GE_buffer_init_array(void);

/* GE_buffer_init_alloc: Initialise a FILE buffer */
F_Buf *GE_buffer_init_alloc(FILE *fp, char *name);

/* GE_buffer_stack_init: Initialise a FILE buffer for the stack */
F_Buf *GE_buffer_stack_init(F_Buf *buf, FILE *fp, char *name);

/* GE_file_clear: empty the file, used berfor file open with append */
unsigned GE_file_clear(char *name);

/* GE_file_open: Open file for file buffer */
unsigned GE_file_open(F_Buf *buf, char *name, char *mode, const int st_prg);

/* open_file: Open a file and add it to the F_Buf array */
unsigned GE_file_open_array(F_Buf **io, char *name, char *mode, const int state);

/* GE_buffer_on: allocate memory for file buffer */
F_Buf *GE_buffer_on(F_Buf *buf);

/* GE_buffer_off: Free only the file buffers momory */
F_Buf *GE_buffer_off(F_Buf *buf);

/* GE_buffer_rewind: Rewind file pointer */
F_Buf *GE_buffer_rewind(F_Buf *buf);

/* GE_buffer_fill: Fills the buffer from the input stream, allowing a margin of
 * space to complete any utf char that may have been started */
F_Buf *GE_buffer_fill(F_Buf *buf);

/* GE_buffer_fwrite: Buffer the writing to a file pointer */
F_Buf *GE_buffer_fwrite(char *str, size_t size, size_t num, F_Buf *buf);

/* GE_buffer_fwrite: Write buffer content to file pointer */
F_Buf *GE_buffer_fwrite_FILE(F_Buf *buf);

/* GE_buffer_getc: Returns the next char from the buffer, refilling when
 * required */
int GE_buffer_fgetc(F_Buf *buf);

/* GE_buffer_skip: Skip over n cahr in the buffer */
int GE_buffer_skip(F_Buf *buf, unsigned num);

/* GE_buffer_ungetc: Push back unused character */
int GE_buffer_ungetc(int c, F_Buf *buf);

/* GE_buffer_pushback_mark: Set mark for a potential pushback */
int GE_buffer_pushback_mark(F_Buf *buf);

/* GE_pushback_unmark: Remove previously placed pushback point */
int GE_buffer_pushback_unmark(F_Buf *buf);

/* GE_buffer_pushback_goto: Return to pushback marker */
int GE_buffer_pushback_goto(F_Buf *buf);

/* GE_buffer_free: Free all memory associated with a F_Buf struct */
void GE_buffer_free(F_Buf *buf);

