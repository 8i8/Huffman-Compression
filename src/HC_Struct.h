/*
 * This Data struct is the default data type used in the data structure test
 * suit, the idea here is that it can be overridden when in standard uses and
 * any struct be passed through the code, but is required when running tests on
 * the suite.
 */
#include <stdio.h>

#ifndef _data_
#define _data_
typedef struct Data {
	char str[3];
	size_t num;
} Data;
#endif

