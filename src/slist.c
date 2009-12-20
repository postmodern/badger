#include <ronin/rat/errno.h>
#include <ronin/slist.h>

#include <malloc.h>
#include <string.h>

ronin_slist_node_t * ronin_slist_node_create(const char *key,void *data)
{
	ronin_slist_node_t *new_node;

	if (!(new_node = malloc(sizeof(ronin_slist_node_t))))
	{
		// malloc failed
		return NULL;
	}

	new_node->key = key;
	new_node->data = data;
	new_node->next = NULL;
	return new_node;
}

void ronin_slist_node_destroy(ronin_slist_node_t *node,ronin_slist_destroy_func destroy_func)
{
	if (node->data && destroy_func)
	{
		destroy_func(node->data);
	}

	free(node);
}

ronin_slist_t * ronin_slist_create(ronin_slist_destroy_func destroy_func)
{
	ronin_slist_t *new_slist;

	if (!(new_slist = malloc(sizeof(ronin_slist_t))))
	{
		// malloc failed
		return NULL;
	}

	new_slist->head = NULL;
	new_slist->destroy_func = destroy_func;
	return new_slist;
}

int ronin_slist_add(ronin_slist_t *slist,const char *key,void *data)
{
	ronin_slist_node_t *next_node = slist->head;
	ronin_slist_node_t *last_node = NULL;
	ronin_slist_node_t *new_node;

	while (next_node)
	{
		switch (strcmp(key,next_node->key))
		{
			case -1:
				if (!last_node)
				{
					if (!(new_node = ronin_slist_node_create(key,data)))
					{
						return RONIN_RAT_ERRNO_MALLOC;
					}

					new_node->next = slist->head;
					slist->head = new_node;
					return 1;
				}
				else if (strcmp(key,last_node->key) == 1)
				{
					if (!(new_node = ronin_slist_node_create(key,data)))
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

	if (!(new_node = ronin_slist_node_create(key,data)))
	{
		return RONIN_RAT_ERRNO_MALLOC;
	}

	if (last_node)
	{
		last_node->next = new_node;
	}
	else
	{
		slist->head = new_node;
	}
	return 1;
}

void * ronin_slist_search(ronin_slist_t *slist,const char *key)
{
	ronin_slist_node_t *next_node = slist->head;

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

void ronin_slist_destroy(ronin_slist_t *slist)
{
	ronin_slist_node_t *next_node = slist->head;
	ronin_slist_node_t *last_node;
	ronin_slist_destroy_func destroy_func = slist->destroy_func;

	while (next_node)
	{
		last_node = next_node;
		next_node = next_node->next;

		ronin_slist_node_destroy(last_node,destroy_func);
	}

	free(slist);
}
