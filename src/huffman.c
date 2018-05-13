#include <stdio.h>
#include "char_frequency.h"

int main(int argc, char *argv[])
{
	FILE *fp;
	DS_DLinkedList *list = NULL;

	while (--argc)
	{
		if ((fp = fopen(*++argv, "r")) == NULL)
			printf("Can not open '%s'.\n", argv[argc+1]);

		/* Make priority que of huffman nodes */
		build_frequency_table(&list, fp);
		build_huffman_tree(&list);

		print_frequency(&list);
		DS_DLinkedList_clear(&list);
	}

	return 0;
}

