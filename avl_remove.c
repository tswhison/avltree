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

static avl_tree_node * avl_tree_successor_node(avl_tree_node *node)
{
	avl_tree_node *successor = node->right;
	while (successor->left)
		successor = successor->left;
	return successor;
}

static avl_tree_node * avl_tree_remove_node(avl_tree *t,
					    void *item,
					    avl_tree_node *node,
					    int *removed)
{
	int res;
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
	node->height = AVL_MAX(avl_tree_height_node(node->left),
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
