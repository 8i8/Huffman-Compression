#include "general/GE_file_buffer.h"

/*
 * LE_xml_element_open: Write an opening xml element.
 */
F_Buf *LE_xml_element_open(F_Buf *buf, char *token);

/*
 * LE_xml_element_close: Write an closing xml element.
 */
F_Buf *LE_xml_element_close(F_Buf *buf, char *token);

/*
 * LE_xml_add_word_pair: Add a word pair at the current tab depth.
 */
F_Buf *LE_xml_element_map(F_Buf *buf, char *st1, char *st2);

/*
 * LE_xml_element_item: Add an xml item at the current tab depth.
 */
F_Buf *LE_xml_element_item(F_Buf *buf, char *item, char *tag);

/*
 * LE_xml_read_token: Read and verify the token, set state acordingly.
 */
char LE_xml_read_token(F_Buf *buf, char c, int *st_lex);
