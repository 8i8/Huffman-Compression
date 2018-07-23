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

