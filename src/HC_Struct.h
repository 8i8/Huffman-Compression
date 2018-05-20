/*
 * TODO remove hard coded binary data size, write a function to deal with this
 * else integrate the String function to work with this also. Note that the
 * String and the Data structs are almost identical.
 */
#include <stdio.h>

#ifndef _data_
#define _data_
typedef struct Data {
	char str[3];
	char binary[256];
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
} HC_HuffmanTree;
#endif

#ifndef _string_
#define _string_
typedef struct _string {
	char *str;
	size_t len;
} String;
#endif

#ifndef _char_map_
#define _char_map_
typedef struct _char_map {
	char binary[256];
} HC_CharMap;
#endif

