#ifndef _RONIN_SLIST_H_
#define _RONIN_SLIST_H_

typedef void (*ronin_slist_destroy_func)(void *data);

struct ronin_slist_node;
typedef struct ronin_slist_node ronin_slist_node_t;

struct ronin_slist_node
{
	const char *key;
	void *data;

	ronin_slist_node_t *next;
};

ronin_slist_node_t * ronin_slist_node_create(const char *key,void *data);
void ronin_slist_node_destroy(ronin_slist_node_t *node,ronin_slist_destroy_func destroy_func);

struct ronin_slist
{
	ronin_slist_node_t *head;

	ronin_slist_destroy_func destroy_func;
};
typedef struct ronin_slist ronin_slist_t;

extern ronin_slist_t * ronin_slist_create(ronin_slist_destroy_func destroy_func);
extern int ronin_slist_add(ronin_slist_t *slist,const char *key,void *data);
extern void * ronin_slist_search(ronin_slist_t *slist,const char *key);
extern void ronin_slist_destroy(ronin_slist_t *slist);

#endif
