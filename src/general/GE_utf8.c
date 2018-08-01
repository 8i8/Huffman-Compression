#define FOUR_BYTES	30		/* Test for UTF-8 width */
#define THREE_BYTES	14
#define TWO_BYTES	6

#define MULTIBYTE (c >> 7)

/*
 * utf8_bytes:	Return multi-char length in bytes, value is -1 as it is read as
 * the number of char remaining.
 */
static unsigned utf8_bytes(unsigned char c)
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
 * utf8_len: Return the length of a multibyte char, 0 if not multybyte.
 */
unsigned utf8_len(char c)
{
	if (MULTIBYTE)
		return utf8_bytes(c);
	else
		return 0;
}

