#include "test.h"
#include <badger/slist.h>

#include <malloc.h>

badger_slist_t *slist;

void test_search_head()
{
	int *i;
	
	if (!(i = badger_slist_search(slist,"a")))
	{
		test_fail("unable to find element %s in the slist","a");
	}

	if (*i != 1)
	{
		test_fail("element %s has %d as the value, expected %d","a",*i,1);
	}
}

void test_search()
{
	int *i;
	
	if (!(i = badger_slist_search(slist,"b")))
	{
		test_fail("unable to find element %s in the slist","b");
	}

	if (*i != 2)
	{
		test_fail("element %s has %d as the value, expected %d","b",*i,2);
	}
}

void test_search_not_found()
{
	if (badger_slist_search(slist,"c"))
	{
		test_fail("found element %s, that should not be in the slist","c");
	}
}

int main()
{
	if (!(slist = badger_slist_create(NULL)))
	{
		test_fail("malloc failed");
	}

	int i1 = 1;
	int i2 = 2;

	if (badger_slist_add(slist,"a",&i1) < 1)
	{
		test_fail("failed to add %s => %d into the sorted slist","a",i1);
	}

	if (badger_slist_add(slist,"b",&i2) < 1)
	{
		test_fail("failed to add %s => %d into the sorted slist","b",i2);
	}

	test_search_head();
	test_search();
	test_search_not_found();

	badger_slist_destroy(slist);
	return 0;
}
