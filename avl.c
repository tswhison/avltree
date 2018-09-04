#include "avl.h"

void avl_tree_init(avl_tree *t,
		avl_tree_node * (*allocate_node)(void *item),
		void (*free_node)(avl_tree_node * ),
		int (*compare_items)(void * , void * ))
{
	t->root = NULL;
	t->allocate_node = allocate_node;
	t->free_node = free_node;
	t->compare_items = compare_items;
}

static void avl_tree_destroy_node(avl_tree *t, avl_tree_node *node)
{
	if (!node)
		return;

	avl_tree_destroy_node(t, node->left);
	avl_tree_destroy_node(t, node->right);

	t->free_node(node);
}

void avl_tree_destroy(avl_tree *t)
{
	avl_tree_destroy_node(t, t->root);
}

static uint32_t avl_tree_num_items_node(avl_tree_node *node)
{
	if (!node)
		return 0;
	return 1 + avl_tree_num_items_node(node->left) +
		   avl_tree_num_items_node(node->right);
}

uint32_t avl_tree_num_items(avl_tree *t)
{
	return avl_tree_num_items_node(t->root);
}

int avl_tree_find(avl_tree *t,
		  void *item,
		  void (*visitor)(void *item, void *context),
		  void *context)
{
	avl_tree_node *node;
	int res;

	node = t->root;

	while (node) {

		res = t->compare_items(item, node->item);

		if (res < 0) {
			node = node->left;
		} else if (res > 0) {
			node = node->right;
		} else {
			visitor(node->item, context);
			return 1;
		}

	}

	return 0;
}

static void avl_tree_pre_order_node(avl_tree *t,
				    void (*visitor)(void *item, void *context),
				    void *context,
				    avl_tree_node *node)
{
	if (!node)
		return;

	visitor(node->item, context);

	avl_tree_pre_order_node(t, visitor, context, node->left);
	avl_tree_pre_order_node(t, visitor, context, node->right);
}

void avl_tree_pre_order(avl_tree *t,
			void (*visitor)(void *item, void *context),
			void *context)
{
	avl_tree_pre_order_node(t, visitor, context, t->root);
}

static void avl_tree_in_order_node(avl_tree *t,
				   void (*visitor)(void *item, void *context),
				   void *context,
				   avl_tree_node *node)
{
	if (!node)
		return;

	avl_tree_in_order_node(t, visitor, context, node->left);

	visitor(node->item, context);

	avl_tree_in_order_node(t, visitor, context, node->right);
}

void avl_tree_in_order(avl_tree *t,
		       void (*visitor)(void *item, void *context),
		       void *context)
{
	avl_tree_in_order_node(t, visitor, context, t->root);
}

static void avl_tree_post_order_node(avl_tree *t,
				     void (*visitor)(void *item, void *context),
				     void *context,
				     avl_tree_node *node)
{
	if (!node)
		return;

	avl_tree_post_order_node(t, visitor, context, node->left);

	avl_tree_post_order_node(t, visitor, context, node->right);

	visitor(node->item, context);
}

void avl_tree_post_order(avl_tree *t,
			 void (*visitor)(void *item, void *context),
			 void *context)
{
	avl_tree_post_order_node(t, visitor, context, t->root);
}

typedef struct _avl_queue_entry {
	avl_tree_node *node;
	struct _avl_queue_entry *next;
} avl_queue_entry;

static void avl_add_queue_entry(avl_tree_node *node, avl_queue_entry **queue_head)
{
	avl_queue_entry *entry;

	entry = (avl_queue_entry *) malloc(sizeof(avl_queue_entry));

	entry->node = node;
	entry->next = NULL;

	if (*queue_head == NULL) {
		*queue_head = entry;
	} else {
		avl_queue_entry *last = *queue_head;

		while (last->next)
			last = last->next;

		last->next = entry;
	}
}

static void avl_tree_level_order_node(avl_tree *t,
				      avl_tree_node *node,
				      avl_queue_entry **queue_array,
				      int level)
{
	if (!node)
		return;

	avl_add_queue_entry(node, &queue_array[level]);

	avl_tree_level_order_node(t, node->left, queue_array, level+1);
	avl_tree_level_order_node(t, node->right, queue_array, level+1);
}

void avl_tree_level_order(avl_tree *t,
			  void (*visitor)(avl_tree_node *node, void *context, int level),
			  void *context)
{
	avl_queue_entry **queue_array;
	int32_t height;
	int32_t i;

	// Allocate an array of queues, one for each level of the tree.
	height = avl_tree_height(t);

	if (!height)
		return;

	queue_array = (avl_queue_entry **)
			calloc(height, sizeof(avl_queue_entry *));

	// Place each tree item in one of the queues, based on the tree level.
	avl_tree_level_order_node(t, t->root, queue_array, 0);

	// Iterate over each queue.
	for (i = 0 ; i < height ; ++i) {
		avl_queue_entry *entry = queue_array[i];

		while (entry) {
			avl_queue_entry *trash;

			visitor(entry->node, context, i);

			trash = entry;
			entry = entry->next;
			free(trash);
		}
	}

	free(queue_array);
}

int32_t avl_tree_height_node(avl_tree_node *node);

int32_t avl_tree_height(avl_tree *t)
{
	return avl_tree_height_node(t->root);
}
