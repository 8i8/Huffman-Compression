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

#ifndef _hc_huffman_node_
#define _hc_huffman_node_
typedef struct _hc_huffman_node {
	Data data;
	char bit;
	struct _hc_huffman_node *next;
	struct _hc_huffman_node *right;
	struct _hc_huffman_node *left;
} HC_HuffmanNode;
#endif

/* DS_huffman_data_init: Initalize an empty Data struct */
Data *DS_huffman_data_init(Data *map);

/* FN_data_strcmp: Compare Data one and two, the value should be a single char and
 * the result alphabetical order as per the ASCII char numbering system */
int FN_data_strcmp(void *v1, void *v2);

/* FN_data_frqcmp: Compare Data one and two, the value should be a number, sort
 * into numerical order, highest first. If d1 s greater than d2 return a
 * positive int, if equal return 0 and if smaller than d2 then return a negative
 * int */
int FN_data_frqcmp(void *v1, void *v2);

