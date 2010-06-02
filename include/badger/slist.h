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
extern void * slist_search(const slist_t *slist,const void *key);
extern int slist_remove(slist_t *slist,const void *key);
extern size_t slist_length(const slist_t *slist);
extern void slist_destroy(slist_t *slist);

#endif
