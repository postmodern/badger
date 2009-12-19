#ifndef _RONIN_RAT_TREE_H_
#define _RONIN_RAT_TREE_H_

struct ronin_rat_tree_node
{
	const char *key;

	struct ronin_rat_tree_node *left;
	void *data;
	struct ronin_rat_tree_node *right;
};
typedef struct ronin_rat_tree_node ronin_rat_tree_node_t;

typedef void (*ronin_rat_tree_destroy_func)(void *data);

struct ronin_rat_tree
{
	ronin_rat_tree_node_t *root;

	ronin_rat_tree_destroy_func destroy_func;
};
typedef struct ronin_rat_tree ronin_rat_tree_t;

extern ronin_rat_tree_t * ronin_rat_tree_create(ronin_rat_tree_destroy_func destroy_func);
extern int ronin_rat_tree_add(ronin_rat_tree_t *tree,const char *key,void *data);
extern void * ronin_rat_tree_search(ronin_rat_tree_t *tree,const char *key);
extern void ronin_rat_tree_destroy(ronin_rat_tree_t *tree);

#endif
