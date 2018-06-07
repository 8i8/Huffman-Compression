#define FOUR_BYTES	30		/* Test for UTF-8 width */
#define THREE_BYTES	14
#define TWO_BYTES	6

#define MULTIBYTE (c >> 7)

/*
 * _utf8_bytes:	Return multi-char length (-1) in bytes, read from the initial
 * UTF-8 char.
 */
static unsigned _utf8_bytes(unsigned char c)
{
	if	(c >> 3 == FOUR_BYTES)
		return 3;
	else if (c >> 4 == THREE_BYTES)
		return 2;
	else if (c >> 5 == TWO_BYTES)
		return 1;

	return 0;
}

/*
 * utf8_test: Test for utf-8 char. Return 1 if a normal ASCII char or the
 * length if it is a multi-byte char.
 */
unsigned utf8_test(unsigned char c)
{
	if (MULTIBYTE)
		return _utf8_bytes(c) + 1;
	else
		return 0;
}

/*
 * utf8_test_count: Test for utf-8 char. Return 1 if a normal ASCII char is
 * encountered; Return 0 whilst in the process of counting a multibyte char,
 * having first established its length.
 */
unsigned utf8_test_count(unsigned char c)
{
	static short unsigned count;

	if (count) {
		if(--count) 
			return count;
	}
	else if (MULTIBYTE) {
		count = _utf8_bytes(c);
		if (count)
			return count;
	}
	return 0;
}

