#ifndef _BADGER_SLIST_H_
#define _BADGER_SLIST_H_

typedef void (*badger_slist_destroy_func)(void *data);

struct badger_slist_node;
typedef struct badger_slist_node badger_slist_node_t;

struct badger_slist_node
{
	const char *key;
	void *data;

	badger_slist_node_t *next;
};

badger_slist_node_t * badger_slist_node_create(const char *key,void *data);
void badger_slist_node_destroy(badger_slist_node_t *node,badger_slist_destroy_func destroy_func);

struct badger_slist
{
	badger_slist_node_t *head;

	badger_slist_destroy_func destroy_func;
};
typedef struct badger_slist badger_slist_t;

extern badger_slist_t * badger_slist_create(badger_slist_destroy_func destroy_func);
extern int badger_slist_add(badger_slist_t *slist,const char *key,void *data);
extern void * badger_slist_search(badger_slist_t *slist,const char *key);
extern void badger_slist_destroy(badger_slist_t *slist);

#endif
