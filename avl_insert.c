#include "avl.h"

#define AVL_MAX(a, b)          \
({                             \
	typeof(a) __a = a;     \
	typeof(b) __b = b;     \
	__a > __b ? __a : __b; \
})

int32_t avl_tree_height_node(avl_tree_node *node);
int32_t avl_tree_balance_node(avl_tree_node *node);
avl_tree_node * avl_tree_ror_node(avl_tree_node *node);
avl_tree_node * avl_tree_rol_node(avl_tree_node *node);

static avl_tree_node * avl_tree_insert_node(avl_tree *t,
					    void *item,
					    avl_tree_node *node,
					    int *inserted)
{
	int res;
	int32_t balance;

	if (!node) {
		*inserted = 1;
		return t->allocate_node(item);
	}

	res = t->compare_items(item, node->item);

	if (res < 0)
		node->left = avl_tree_insert_node(t, item, node->left, inserted);
	else if (res > 0)
		node->right = avl_tree_insert_node(t, item, node->right, inserted);
	else // item collision - new item not inserted
		return node;

	node->height = 1 + AVL_MAX(avl_tree_height_node(node->left),
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
