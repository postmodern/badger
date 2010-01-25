#include <badger/errno.h>
#include <badger/slist.h>

#include <malloc.h>
#include <string.h>

badger_slist_node_t * badger_slist_node_create(const char *key,void *data)
{
	badger_slist_node_t *new_node;

	if (!(new_node = malloc(sizeof(badger_slist_node_t))))
	{
		// malloc failed
		return NULL;
	}

	new_node->key = key;
	new_node->data = data;
	new_node->next = NULL;
	return new_node;
}

void badger_slist_node_destroy(badger_slist_node_t *node,badger_slist_destroy_func destroy_func)
{
	if (node->data && destroy_func)
	{
		destroy_func(node->data);
	}

	free(node);
}

badger_slist_t * badger_slist_create(badger_slist_destroy_func destroy_func)
{
	badger_slist_t *new_slist;

	if (!(new_slist = malloc(sizeof(badger_slist_t))))
	{
		// malloc failed
		return NULL;
	}

	new_slist->head = NULL;
	new_slist->destroy_func = destroy_func;
	return new_slist;
}

int badger_slist_add(badger_slist_t *slist,const char *key,void *data)
{
	badger_slist_node_t *next_node = slist->head;
	badger_slist_node_t *last_node = NULL;
	badger_slist_node_t *new_node;

	while (next_node)
	{
		switch (strcmp(key,next_node->key))
		{
			case -1:
				if (!last_node)
				{
					if (!(new_node = badger_slist_node_create(key,data)))
					{
						return BADGER_ERRNO_MALLOC;
					}

					new_node->next = slist->head;
					slist->head = new_node;
					return 1;
				}
				else if (strcmp(key,last_node->key) == 1)
				{
					if (!(new_node = badger_slist_node_create(key,data)))
					{
						return BADGER_ERRNO_MALLOC;
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

	if (!(new_node = badger_slist_node_create(key,data)))
	{
		return BADGER_ERRNO_MALLOC;
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

void * badger_slist_search(badger_slist_t *slist,const char *key)
{
	badger_slist_node_t *next_node = slist->head;

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

void badger_slist_destroy(badger_slist_t *slist)
{
	badger_slist_node_t *next_node = slist->head;
	badger_slist_node_t *last_node;
	badger_slist_destroy_func destroy_func = slist->destroy_func;

	while (next_node)
	{
		last_node = next_node;
		next_node = next_node->next;

		badger_slist_node_destroy(last_node,destroy_func);
	}

	free(slist);
}
