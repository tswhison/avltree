/*
** avl_remove.c : implementation of AVL Tree remove
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

static avl_tree_node * avl_tree_remove_node(avl_tree *t,
					    void *item,
					    avl_tree_node *node,
					    int *removed)
{
	int64_t res;
	int32_t balance;

	if (!node)
		return node;

	res = t->compare_items(item, node->item);

	if (res < 0)
		node->left = avl_tree_remove_node(t, item, node->left, removed);
	else if (res > 0)
		node->right = avl_tree_remove_node(t, item, node->right, removed);
	else {

		if (!node->left || !node->right) {
			// one or both children empty.
			avl_tree_node *trash = node->left ? node->left : node->right;

			if (!trash) {
				// no children
				trash = node;
				node = NULL;
			} else // copy the contents of the non-empty child
				*node = *trash;

			*removed = 1;
			t->free_node(trash);

		} else {
			// both children present.
			// find the successor's value, place it in node,
			// and continue to delete the successor.
			avl_tree_node *successor = avl_tree_successor_node(node);

			node->item = successor->item;

			node->right = avl_tree_remove_node(t, successor->item, node->right, removed);
		}

	}

	if (!node)
		return node;

	// update the height of the current node
	node->height = avl_tree_max(avl_tree_height_node(node->left),
				    avl_tree_height_node(node->right)) + 1;

	balance = avl_tree_balance_node(node);

	if (balance > 1) {
		int32_t left_balance = avl_tree_balance_node(node->left);

		// Left Left Case
		if (left_balance >= 0)
			return avl_tree_ror_node(node);
		else { // Left Right Case
			node->left = avl_tree_rol_node(node->left);
			return avl_tree_ror_node(node);
		}
	}

	if (balance < -1) {
		int32_t right_balance = avl_tree_balance_node(node->right);

		// Right Right Case
		if (right_balance <= 0)
			return avl_tree_rol_node(node);
		else { // Right Left Case
			node->right = avl_tree_ror_node(node->right);
			return avl_tree_rol_node(node);
		}
	}

	return node;
}

int avl_tree_remove(avl_tree *t, void *item)
{
	int removed = 0;
	t->root = avl_tree_remove_node(t, item, t->root, &removed);
	return removed;
}
