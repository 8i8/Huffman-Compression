#include <stdio.h>

#ifndef _data_
#define _data_
typedef struct Data {
	char str[5];
	char binary[256];
	size_t len;
	size_t num;
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

