#ifndef _RONIN_RAT_LIST_H_
#define _RONIN_RAT_LIST_H_

typedef void (*ronin_rat_list_destroy_func)(void *data);

struct ronin_rat_list_node;
typedef struct ronin_rat_list_node ronin_rat_list_node_t;

struct ronin_rat_list_node
{
	const char *key;
	void *data;

	ronin_rat_list_node_t *next;
};

ronin_rat_list_node_t * ronin_rat_list_node_create(const char *key,void *data);
void ronin_rat_list_node_destroy(ronin_rat_list_node_t *node,ronin_rat_list_destroy_func destroy_func);

struct ronin_rat_list
{
	ronin_rat_list_node_t *head;

	ronin_rat_list_destroy_func destroy_func;
};
typedef struct ronin_rat_list ronin_rat_list_t;

extern ronin_rat_list_t * ronin_rat_list_create(ronin_rat_list_destroy_func destroy_func);
extern int ronin_rat_list_add(ronin_rat_list_t *list,const char *key,void *data);
extern void * ronin_rat_list_search(ronin_rat_list_t *list,const char *key);
extern void ronin_rat_list_destroy(ronin_rat_list_t *list);

#endif
