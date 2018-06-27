#define FOUR_BYTES	30		/* Test for UTF-8 width */
#define THREE_BYTES	14
#define TWO_BYTES	6

#define MULTIBYTE (c >> 7)

/*
 * utf8_bytes:	Return multi-char length (-1) in bytes, read from the initial
 * UTF-8 char.
 */
static unsigned utf8_bytes(unsigned char c)
{
	if	(c >> 3 == FOUR_BYTES)
		return 4;
	else if (c >> 4 == THREE_BYTES)
		return 3;
	else if (c >> 5 == TWO_BYTES)
		return 2;

	return 0;
}

/*
 * utf8_test: Test for utf-8 char. Return 0 if a normal ASCII char or the
 * length if it is a multi-byte char.
 */
unsigned utf8_test(char c)
{
	if (MULTIBYTE)
		return utf8_bytes(c);
	else
		return 0;
}

/*
 * utf8_len: Return the length of a multibyte char.
 */
unsigned utf8_len(char *str)
{
	char c = str[0];

	if (MULTIBYTE)
		return utf8_bytes(c);
	else
		return 1;
}

/*
 * utf8_count: Test for utf-8 char. Return 0 if a normal ASCII char is
 * encountered; Return 0 whilst in the process of counting a multibyte char,
 * having first established its length.
 */
unsigned utf8_count(char c)
{
	static short unsigned count;

	if (count)
		return --count;

	else if (MULTIBYTE) {
		count = utf8_bytes(c);
		return count;
	}
	return 0;
}

