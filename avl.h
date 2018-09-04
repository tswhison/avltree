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

typedef struct _avl_tree {
	avl_tree_node *root;
	avl_tree_node * (*allocate_node)(void *item);
	void (*free_node)(avl_tree_node * );
	int (*compare_items)(void * , void * );
} avl_tree;

void avl_tree_init(avl_tree *t,
		avl_tree_node * (*allocate_node)(void *item),
		void (*free_node)(avl_tree_node * ),
		int (*compare_items)(void * , void * ));

void avl_tree_destroy(avl_tree *t);

// 0 if insertion failed
int avl_tree_insert(avl_tree *t, void *item);

// 0 if removal failed
int avl_tree_remove(avl_tree *t, void *item);

uint32_t avl_tree_num_items(avl_tree *t);

// 1 if found, 0 if not
int avl_tree_find(avl_tree *t,
		void *item,
		void (*visitor)(void *item, void *context),
		void *context);

void avl_tree_pre_order(avl_tree *t,
			void (*visitor)(void *item, void *context),
			void *context);

void avl_tree_in_order(avl_tree *t,
		       void (*visitor)(void *item, void *context),
		       void *context);

void avl_tree_post_order(avl_tree *t,
			 void (*visitor)(void *item, void *context),
			 void *context);

void avl_tree_level_order(avl_tree *t,
			  void (*visitor)(avl_tree_node *node, void *context, int level),
			  void *context);

int32_t avl_tree_height(avl_tree *t);

#endif // __AVL_H__
