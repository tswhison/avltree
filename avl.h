/*
** avl.h : definitions for AVL Trees
** Copyright (C) 2018  Tim Whisonant
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#ifndef __AVL_H__
#define __AVL_H__
#include <stdlib.h>
#include <stdint.h>

typedef struct _avl_tree_node {
	void *item;
	struct _avl_tree_node *left;
	struct _avl_tree_node *right;
	int32_t height;
} avl_tree_node;

typedef struct _avl_queue_entry {
	avl_tree_node *node;
	struct _avl_queue_entry *next;
} avl_queue_entry;

typedef struct _avl_tree {
	avl_tree_node *root;
	avl_tree_node * (*allocate_node)(void *item);
	void (*free_node)(avl_tree_node * );
	int64_t (*compare_items)(void * , void * );
	avl_queue_entry * (*allocate_entry)(avl_tree_node * );
	void (*free_entry)(avl_queue_entry * );
} avl_tree;

void avl_tree_init(avl_tree *t,
		avl_tree_node * (*allocate_node)(void *item),
		void (*free_node)(avl_tree_node * ),
		int64_t (*compare_items)(void * , void * ),
		avl_queue_entry * (*allocate_entry)(avl_tree_node * ),
		void (*free_entry)(avl_queue_entry * ));

void avl_tree_destroy(avl_tree *t);

// 0 if insertion failed
int avl_tree_insert(avl_tree *t, void *item);

// 0 if removal failed
int avl_tree_remove(avl_tree *t, void *item);

uint32_t avl_tree_num_items(avl_tree *t);

// 1 if found, 0 if not
int avl_tree_find(avl_tree *t,
		void *item,
		void (*visitor)(avl_tree_node *node, void *context),
		void *context);

void avl_tree_pre_order(avl_tree *t,
			void (*visitor)(avl_tree_node *node, void *context),
			void *context);

void avl_tree_in_order(avl_tree *t,
		       void (*visitor)(avl_tree_node *node, void *context),
		       void *context);

void avl_tree_post_order(avl_tree *t,
			 void (*visitor)(avl_tree_node *node, void *context),
			 void *context);

void avl_tree_level_order(avl_tree *t,
			  void (*visitor)(avl_tree_node *node, void *context, int level),
			  void *context);

int32_t avl_tree_height(avl_tree *t);

#endif // __AVL_H__
