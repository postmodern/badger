#include "test.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

void test_fail(const char *mesg,...)
{
	va_list args;

	va_start(args,mesg);
	vfprintf(stderr,mesg,args);
	va_end(args);

	putchar('\n');
	exit(-1);
}

void test_bytes(const unsigned char *bytes,const unsigned char *expected,size_t expected_length)
{
	unsigned int i;

	for (i=0;i<expected_length;i++)
	{
		if (bytes[i] != expected[i])
		{
			test_fail("byte[%u] is 0x%x, expected 0x%x",i,bytes[i],expected[i]);
		}
	}
}

void test_strings(const char *string,const char *expected,size_t expected_length)
{
	unsigned int i;

	for (i=0;i<expected_length;i++)
	{
		if (string[i] != expected[i])
		{
			test_fail("string[%u] is %c, expected %c",i,string[i],expected[i]);
		}
	}
}
