/* utf8_test: Test for utf-8 char. Return 1 if a normal ASCII char or the
 * length if it is a multi-byte char. */
unsigned utf8_test(unsigned char c);

/* utf8_test_count: Test for utf-8 char. Return 1 if a normal ASCII char is
 * encountered; Return 0 whilst in the process of counting a multibyte char,
 * having first established its length. */
unsigned utf8_test_count(unsigned char c);

