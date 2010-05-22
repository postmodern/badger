#ifndef _BADGER_SLIST_H_
#define _BADGER_SLIST_H_

#include <string.h>

typedef int (*slist_compare_func)(const void *key1,const void *key2);

static inline int slist_compare_strings(const void *key1,const void *key2)
{
	return strcmp(key1,key2);
}

static inline int slist_compare_ints(const void *key1,const void *key2)
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

typedef void (*slist_destroy_func)(void *data);

struct slist_node;
typedef struct slist_node slist_node_t;

struct slist_node
{
	const void *key;
	void *data;

	slist_node_t *next;
};

slist_node_t * slist_node_create(const void *key,void *data);
void slist_node_destroy(slist_node_t *node,slist_destroy_func destroy_func);

struct slist
{
	slist_node_t *head;

	slist_compare_func compare_func;
	slist_destroy_func destroy_func;
};
typedef struct slist slist_t;

extern slist_t * slist_create(slist_compare_func compare_func,slist_destroy_func destroy_func);
extern int slist_add(slist_t *slist,const void *key,void *data);
extern void * slist_search(slist_t *slist,const void *key);
extern void slist_destroy(slist_t *slist);

#endif
