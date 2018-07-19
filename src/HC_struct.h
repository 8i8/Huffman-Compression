#include <stdio.h>

#ifndef _data_
#define _data_
typedef struct _data {
	char utf8_char[5];
	char string[128];
	size_t len;
	size_t frq;
	struct _data *next;
} Data;
#endif

#ifndef _hc_dlinkedlist_
#define _hc_dlinkedlist_
typedef struct _hc_dlinkedlist {
	Data data;
	char bit;
	struct _hc_dlinkedlist *next;
	struct _hc_dlinkedlist *right;
	struct _hc_dlinkedlist *left;
} HC_HuffmanNode;
#endif

#ifndef _main_
#define _main_
typedef struct _main {
	FILE *in, *out;
} Files;
#endif

#ifndef _file_buffer_
#define _file_buffer_
typedef struct _file_buffer {
	FILE *fp;
	char *buf;
	char *ptr;
	char *end;
	int eof;
} F_Buf;
#endif
