#ifndef _BADGER_SLIST_H_
#define _BADGER_SLIST_H_

#include <string.h>

typedef int (*badger_slist_compare_func)(const void *key1,const void *key2);

static inline int badger_slist_compare_strings(const void *key1,const void *key2)
{
	return strcmp(key1,key2);
}

static inline int badger_slist_compare_ints(const void *key1,const void *key2)
{
	if (key1 < key2)
	{
		return -1;
	}
	else if (key1 > key2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

typedef void (*badger_slist_destroy_func)(void *data);

struct badger_slist_node;
typedef struct badger_slist_node badger_slist_node_t;

struct badger_slist_node
{
	const void *key;
	void *data;

	badger_slist_node_t *next;
};

badger_slist_node_t * badger_slist_node_create(const void *key,void *data);
void badger_slist_node_destroy(badger_slist_node_t *node,badger_slist_destroy_func destroy_func);

struct badger_slist
{
	badger_slist_node_t *head;

	badger_slist_compare_func compare_func;
	badger_slist_destroy_func destroy_func;
};
typedef struct badger_slist badger_slist_t;

extern badger_slist_t * badger_slist_create(badger_slist_compare_func compare_func,badger_slist_destroy_func destroy_func);
extern int badger_slist_add(badger_slist_t *slist,const void *key,void *data);
extern void * badger_slist_search(badger_slist_t *slist,const void *key);
extern void badger_slist_destroy(badger_slist_t *slist);

static inline badger_slist_t * badger_slist_dict(badger_slist_destroy_func destroy_func)
{
	return badger_slist_create(badger_slist_compare_strings,destroy_func);
}

#endif
