#include <stdio.h>

// TODO merge the structs data and string, there is no need for both in the
// code. 

#ifndef _data_
#define _data_
typedef struct _data {
	char utf8_char[5];
	char string[128];
	size_t len;
	size_t frq;
} Data;
#endif

#ifndef _hc_dlinkedlist_
#define _hc_dlinkedlist_
typedef struct _hc_dlinkedlist {
	Data data;
	char bit;
	struct _hc_dlinkedlist *prev;
	struct _hc_dlinkedlist *next;
	struct _hc_dlinkedlist *right;
	struct _hc_dlinkedlist *left;
} HC_HuffmanNode;
#endif

#ifndef _string_
#define _string_
typedef struct _string {
	char *str;
	size_t lim;
} String;
#endif

#ifndef _main_
#define _main_
typedef struct _main {
	FILE *in, *out;
	HC_HuffmanNode *tree;
	Data *map;
} Main;
#endif

