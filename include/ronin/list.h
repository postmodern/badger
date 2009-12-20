#ifndef _RONIN_LIST_H_
#define _RONIN_LIST_H_

typedef void (*ronin_list_destroy_func)(void *data);

struct ronin_list_node;
typedef struct ronin_list_node ronin_list_node_t;

struct ronin_list_node
{
	const char *key;
	void *data;

	ronin_list_node_t *next;
};

ronin_list_node_t * ronin_list_node_create(const char *key,void *data);
void ronin_list_node_destroy(ronin_list_node_t *node,ronin_list_destroy_func destroy_func);

struct ronin_list
{
	ronin_list_node_t *head;

	ronin_list_destroy_func destroy_func;
};
typedef struct ronin_list ronin_list_t;

extern ronin_list_t * ronin_list_create(ronin_list_destroy_func destroy_func);
extern int ronin_list_add(ronin_list_t *list,const char *key,void *data);
extern void * ronin_list_search(ronin_list_t *list,const char *key);
extern void ronin_list_destroy(ronin_list_t *list);

#endif
