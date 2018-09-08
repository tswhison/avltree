/*
** avl_util.h : implementation of AVL Tree helpers
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
#ifndef __AVL_UTIL_H__
#define __AVL_UTIL_H__
#include <stdio.h>

#if 1
#define avl_tree_assert(__expr)
#else
#define avl_tree_assert(__expr)                                         \
do                                                                      \
{                                                                       \
	if (!(__expr)) {                                                \
		fprintf(stderr, "%s:%d avl_tree_assert( %s ) failed\n", \
			__FILE__, __LINE__, #__expr);                   \
	}                                                               \
}while(0)
#endif

#define avl_tree_max(__a, __b)     \
({                                 \
	typeof(__a) ___a = __a;    \
	typeof(__b) ___b = __b;    \
	___a > ___b ? ___a : ___b; \
})

static inline int32_t avl_tree_height_node(avl_tree_node *node)
{
	if (!node)
		return 0;
	return node->height;
}

static inline int32_t avl_tree_balance_node(avl_tree_node *node)
{
	if (!node)
		return 0;
	return avl_tree_height_node(node->left) -
	       avl_tree_height_node(node->right);
}

static inline avl_tree_node * avl_tree_ror_node(avl_tree_node *node)
{
/*
//             node
//           / 
// nodes_left
//           \
//           nodes_left_right
//
//       nodes_left
//                 \
//                  node
//                 /
// nodes_left_right
*/
	avl_tree_node *nodes_left = node->left;
	avl_tree_node *nodes_left_right = nodes_left->right;

	nodes_left->right = node;
	node->left = nodes_left_right;

	node->height = avl_tree_max(avl_tree_height_node(node->left),
				    avl_tree_height_node(node->right)) + 1;
	nodes_left->height = avl_tree_max(avl_tree_height_node(nodes_left->left),
					  avl_tree_height_node(nodes_left->right)) + 1;
	return nodes_left;
}

static inline avl_tree_node * avl_tree_rol_node(avl_tree_node *node)
{
/*
//             node
//                 \
//                  nodes_right
//                 /
// nodes_right_left
//
//           nodes_right
//          /
//      node
//          \
//           nodes_right_left
*/
	avl_tree_node *nodes_right = node->right;
	avl_tree_node *nodes_right_left = nodes_right->left;

	nodes_right->left = node;
	node->right = nodes_right_left;

	node->height = avl_tree_max(avl_tree_height_node(node->left),
				    avl_tree_height_node(node->right)) + 1;
	nodes_right->height = avl_tree_max(avl_tree_height_node(nodes_right->left),
					   avl_tree_height_node(nodes_right->right)) + 1;
	return nodes_right;
}

static inline avl_tree_node * avl_tree_successor_node(avl_tree_node *n)
{
	avl_tree_assert(n);
	avl_tree_assert(n->right);
	n = n->right;
	while (n->left)
		n = n->left;
	return n;
}

#endif // __AVL_UTIL_H__
