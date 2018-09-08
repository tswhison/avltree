/*
** avl_insert.c : implementation of AVL Tree insert
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
#include "avl.h"
#include "avl_util.h"

static avl_tree_node * avl_tree_insert_node(avl_tree *t,
					    void *item,
					    avl_tree_node *node,
					    int *inserted)
{
	int64_t res;
	int32_t balance;

	if (!node) {
		node = t->allocate_node(item);
		if (node) {
			*inserted = 1;
			node->height = 1;
		}
		return node;
	}

	res = t->compare_items(item, node->item);

	if (res < 0)
		node->left = avl_tree_insert_node(t, item, node->left, inserted);
	else if (res > 0)
		node->right = avl_tree_insert_node(t, item, node->right, inserted);
	else // item collision - new item not inserted
		return node;

	node->height = 1 + avl_tree_max(avl_tree_height_node(node->left),
					avl_tree_height_node(node->right));

	balance = avl_tree_balance_node(node);

	/*
	// Left Left Case
	// - node == z.
	// - item was inserted under x.
	// - fix by rotating z right.
	//
	//        z                y
	//       / \             /   \
	//      y  T4           x     z
	//     / \             / \   / \
	//    x   T3          T1 T2 T3 T4
	//   / \
	// T1   T2
	*/
	if (balance > 1) {
		res = t->compare_items(item, node->left->item);
		if (res < 0)
			return avl_tree_ror_node(node);
	}

	/*
	// Right Right Case
	// - node == z.
	// - item was inserted under x.
	// - fix by rotating z left.
	//
	//    z                    y
	//   / \                 /   \
	// T1   y               z     x
	//     / \             / \   / \
	//   T2   x           T1 T2 T3 T4
	//       / \
	//     T3   T4
	*/
	if (balance < -1) {
		res = t->compare_items(item, node->right->item);
		if (res > 0)
			return avl_tree_rol_node(node);
	}

	/*
	// Left Right Case
	// - node == z.
	// - item was inserted under x.
	// - fix by rotating y left, then z right.
	//
	//     z            z            x
	//    / \          / \         /   \
	//   y  T4        x  T4       y     z
	//  / \          / \         / \   / \
	// T1  x        y  T3       T1 T2 T3 T4
	//    / \      / \
	//   T2 T3    T1 T2
	//
	*/
	if (balance > 1) {
		res = t->compare_items(item, node->left->item);
		if (res > 0) {
			node->left = avl_tree_rol_node(node->left);
			return avl_tree_ror_node(node);
		}
	}

	/*
	// Right Left Case
	// - node == z.
	// - item was inserted under x.
	// - fix by rotating y right, then z left.
	//
	//     z           z              x
	//    / \         / \           /   \
	//  T1   y      T1   x         z     y
	//      / \         / \       / \   / \
	//     x   T4     T2   y     T1 T2 T3 T4
	//    / \             / \
	//   T2 T3           T3 T4
	*/
	if (balance < -1) {
		res = t->compare_items(item, node->right->item);
		if (res < 0) {
			node->right = avl_tree_ror_node(node->right);
			return avl_tree_rol_node(node);
		}
	}

	return node; // no change
}

int avl_tree_insert(avl_tree *t, void *item)
{
	int inserted = 0;
	t->root = avl_tree_insert_node(t, item, t->root, &inserted);
	return inserted;
}
