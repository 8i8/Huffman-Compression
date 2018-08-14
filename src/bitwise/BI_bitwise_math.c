
/*
 * ipow: Bitwise power function.
 */
unsigned ipow(unsigned base, unsigned exp)
{
	unsigned result = 1;

	while (exp)
	{
		if (exp & 1)
	    		result *= base;
		exp >>= 1;
		base *= base;
	}
	return result;
}
