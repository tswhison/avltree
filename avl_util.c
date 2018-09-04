#include "avl.h"

#define AVL_MAX(a, b)          \
({                             \
	typeof(a) __a = a;     \
	typeof(b) __b = b;     \
	__a > __b ? __a : __b; \
})

int32_t avl_tree_height_node(avl_tree_node *node)
{
	if (!node)
		return 0;
	return node->height;
}

int32_t avl_tree_balance_node(avl_tree_node *node)
{
	if (!node)
		return 0;
	return avl_tree_height_node(node->left) -
	       avl_tree_height_node(node->right);
}

avl_tree_node * avl_tree_ror_node(avl_tree_node *node)
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

	node->height = AVL_MAX(avl_tree_height_node(node->left),
			       avl_tree_height_node(node->right)) + 1;
	nodes_left->height = AVL_MAX(avl_tree_height_node(nodes_left->left),
				     avl_tree_height_node(nodes_left->right)) + 1;
	return nodes_left;
}

avl_tree_node * avl_tree_rol_node(avl_tree_node *node)
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

	node->height = AVL_MAX(avl_tree_height_node(node->left),
			       avl_tree_height_node(node->right)) + 1;
	nodes_right->height = AVL_MAX(avl_tree_height_node(nodes_right->left),
				      avl_tree_height_node(nodes_right->right)) + 1;
	return nodes_right;
}
