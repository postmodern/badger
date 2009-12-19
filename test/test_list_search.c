#include "test.h"
#include <ronin/rat/list.h>

#include <malloc.h>

ronin_rat_list_t *list;

void test_search_head()
{
	int *i;
	
	if (!(i = ronin_rat_list_search(list,"a")))
	{
		test_fail("unable to find element %s in the list","a");
	}

	if (*i != 1)
	{
		test_fail("element %s has %d as the value, expected %d","a",*i,1);
	}
}

void test_search()
{
	int *i;
	
	if (!(i = ronin_rat_list_search(list,"b")))
	{
		test_fail("unable to find element %s in the list","b");
	}

	if (*i != 2)
	{
		test_fail("element %s has %d as the value, expected %d","b",*i,2);
	}
}

void test_search_not_found()
{
	if (ronin_rat_list_search(list,"c"))
	{
		test_fail("found element %s, that should not be in the list","c");
	}
}

int main()
{
	if (!(list = ronin_rat_list_create(NULL)))
	{
		test_fail("malloc failed");
	}

	int i1 = 1;
	int i2 = 2;

	if (ronin_rat_list_add(list,"a",&i1) < 1)
	{
		test_fail("failed to add %s => %d into the sorted list","a",i1);
	}

	if (ronin_rat_list_add(list,"b",&i2) < 1)
	{
		test_fail("failed to add %s => %d into the sorted list","b",i2);
	}

	test_search_head();
	test_search();
	test_search_not_found();

	ronin_rat_list_destroy(list);
	return 0;
}
