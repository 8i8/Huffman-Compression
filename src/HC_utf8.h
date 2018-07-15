/* utf8_test: Test for utf-8 char. Return 1 if a normal ASCII char or the
 * length if it is a multi-byte char. */
unsigned utf8_test(char c);

/* utf8_len: Return the length of a multibyte char */
unsigned utf8_len(char *c);

/* utf8_countdown: Test for utf-8 char. Return 1 if a normal ASCII char is
 * encountered; Return 0 whilst in the process of counting a multibyte char,
 * having first established its length. */
unsigned utf8_countdown(char c);

