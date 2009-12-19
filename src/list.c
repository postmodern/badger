#include <ronin/rat/list.h>
#include <ronin/rat/errno.h>

#include <malloc.h>
#include <string.h>

ronin_rat_list_node_t * ronin_rat_list_node_create(const char *key,void *data)
{
	ronin_rat_list_node_t *new_node;

	if (!(new_node = malloc(sizeof(ronin_rat_list_node_t))))
	{
		// malloc failed
		return NULL;
	}

	new_node->key = key;
	new_node->data = data;
	new_node->next = NULL;
	return new_node;
}

void ronin_rat_list_node_destroy(ronin_rat_list_node_t *node,ronin_rat_list_destroy_func destroy_func)
{
	if (node->data && destroy_func)
	{
		destroy_func(node->data);
	}

	free(node);
}

ronin_rat_list_t * ronin_rat_list_create(ronin_rat_list_destroy_func destroy_func)
{
	ronin_rat_list_t *new_list;

	if (!(new_list = malloc(sizeof(ronin_rat_list_t))))
	{
		// malloc failed
		return NULL;
	}

	new_list->head = NULL;
	new_list->destroy_func = destroy_func;
	return new_list;
}

int ronin_rat_list_add(ronin_rat_list_t *list,const char *key,void *data)
{
	ronin_rat_list_node_t *next_node = list->head;
	ronin_rat_list_node_t *last_node = NULL;
	ronin_rat_list_node_t *new_node;

	while (next_node)
	{
		switch (strcmp(key,next_node->key))
		{
			case -1:
				if (!last_node)
				{
					if (!(new_node = ronin_rat_list_node_create(key,data)))
					{
						return RONIN_RAT_ERRNO_MALLOC;
					}

					new_node->next = list->head;
					list->head = new_node;
					return 1;
				}
				else if (strcmp(key,last_node->key) == 1)
				{
					if (!(new_node = ronin_rat_list_node_create(key,data)))
					{
						return RONIN_RAT_ERRNO_MALLOC;
					}

					new_node->next = next_node;
					last_node->next = new_node;
					return 1;
				}
			case 1:
				break;
			case 0:
				return 0;
		}

		last_node = next_node;
		next_node = next_node->next;
	}

	if (!(new_node = ronin_rat_list_node_create(key,data)))
	{
		return RONIN_RAT_ERRNO_MALLOC;
	}

	last_node->next = new_node;
	return 1;
}

void * ronin_rat_list_search(ronin_rat_list_t *list,const char *key)
{
	ronin_rat_list_node_t *next_node = list->head;

	while (next_node)
	{
		switch (strcmp(key,next_node->key))
		{
			case -1:
				return NULL;
			case 0:
				return next_node->data;
			case 1:
				next_node = next_node->next;
				break;
		}
	}

	return NULL;
}

void ronin_rat_list_destroy(ronin_rat_list_t *list)
{
	ronin_rat_list_node_t *next_node = list->head;
	ronin_rat_list_node_t *last_node;
	ronin_rat_list_destroy_func destroy_func = list->destroy_func;

	while (next_node)
	{
		last_node = next_node;
		next_node = next_node->next;

		ronin_rat_list_node_destroy(last_node,destroy_func);
	}

	free(list);
}
