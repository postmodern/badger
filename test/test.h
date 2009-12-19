#ifndef _TEST_H_
#define _TEST_H_

#include <sys/types.h>

void test_fail(const char *mesg,...);

void test_bytes(const unsigned char *bytes,const unsigned char *expected,size_t expected_length);
void test_strings(const char *string,const char *expected,size_t expected_length);

#endif
