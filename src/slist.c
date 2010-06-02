/*
 * Badger - A fear-less, robust and portable systems management solution.
 *
 * Copyright (c) 2010 Hal Brodigan (postmodern.mod3 at gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

#include <badger/errno.h>
#include <badger/slist.h>

#include <stdlib.h>
#include <string.h>

slist_node_t * slist_node_create(const void *key,void *data)
{
	slist_node_t *new_node;

	if (!(new_node = malloc(sizeof(slist_node_t))))
	{
		// malloc failed
		return NULL;
	}

	new_node->key = key;
	new_node->data = data;
	new_node->next = NULL;
	return new_node;
}

void slist_node_destroy(slist_node_t *node,slist_destroy_func destroy_func)
{
	if (node->data && destroy_func)
	{
		destroy_func(node->data);
	}

	// zero the slist_node fields before freeing it
	node->key = NULL;
	node->data = NULL;
	node->next = NULL;

	free(node);
}

slist_t * slist_create(slist_compare_func compare_func,slist_destroy_func destroy_func)
{
	slist_t *new_slist;

	if (!(new_slist = malloc(sizeof(slist_t))))
	{
		// malloc failed
		return NULL;
	}

	new_slist->head = NULL;
	new_slist->compare_func = compare_func;
	new_slist->destroy_func = destroy_func;
	return new_slist;
}

int slist_add(slist_t *slist,const void *key,void *data)
{
	slist_node_t *next_node = slist->head;
	slist_node_t *last_node = NULL;
	slist_node_t *new_node;

	while (next_node)
	{
		switch (slist->compare_func(key,next_node->key))
		{
			case -1:
				if (!last_node)
				{
					if (!(new_node = slist_node_create(key,data)))
					{
						return BADGER_ERRNO_MALLOC;
					}

					new_node->next = slist->head;
					slist->head = new_node;
					return 1;
				}
				else if (strcmp(key,last_node->key) == 1)
				{
					if (!(new_node = slist_node_create(key,data)))
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

	if (!(new_node = slist_node_create(key,data)))
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

void * slist_search(const slist_t *slist,const void *key)
{
	slist_node_t *next_node = slist->head;

	while (next_node)
	{
		switch (slist->compare_func(key,next_node->key))
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

int slist_remove(slist_t *slist,const void *key)
{
	slist_node_t *last_node = NULL;
	slist_node_t *next_node = slist->head;

	while (next_node)
	{
		switch (slist->compare_func(key,next_node->key))
		{
			case -1:
				return 0;
			case 0:
				if (last_node)
				{
					last_node->next = next_node->next;
				}
				else
				{
					slist->head = next_node->next;
				}

				slist_node_destroy(next_node,slist->destroy_func);
				return 1;
			case 1:
				next_node = next_node->next;
				break;
		}
	}

	return 0;
}

size_t slist_length(const slist_t *slist)
{
	slist_node_t *next_node = slist->head;
	size_t length = 0;

	while (next_node)
	{
		++length;
		next_node = next_node->next;
	}

	return length;
}

void slist_destroy(slist_t *slist)
{
	slist_node_t *next_node = slist->head;
	slist_node_t *last_node;
	slist_destroy_func destroy_func = slist->destroy_func;

	while (next_node)
	{
		last_node = next_node;
		next_node = next_node->next;

		slist_node_destroy(last_node,destroy_func);
	}

	// zero the slist fields before freeing it
	slist->head = NULL;
	slist->compare_func = NULL;
	slist->destroy_func = NULL;

	free(slist);
}
