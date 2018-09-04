#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "avl.h"

#define mymin(a, b)            \
({                             \
	typeof(a) __a = a;     \
	typeof(b) __b = b;     \
	__a < __b ? __a : __b; \
})

#define mymax(a, b)            \
({                             \
	typeof(a) __a = a;     \
	typeof(b) __b = b;     \
	__a > __b ? __a : __b; \
})

avl_tree_node * my_allocate_avl_node(void *item)
{
	avl_tree_node *node = (avl_tree_node *)
				calloc(1, sizeof(avl_tree_node));
	if (node) {
		node->item = item;
		node->height = 1;
	}

	return node;
}

void my_free_avl_node(avl_tree_node *node)
{
	free(node);
}

int my_int_compare(void *a, void *b)
{
	int ia = (int) (int64_t) a;
	int ib = (int) (int64_t) b;
	return ia - ib;
}

void print_tree_item(avl_tree_node *node, void *context, int level)
{
	int *last_level = (int *) context;

	if (level > *last_level) {
		printf("\n");
		*last_level = level;
	}

	printf("%d ", (int) (int64_t) node->item);
}

void simple_insert_and_level_order(void)
{
	avl_tree t;
	int last_level;

	avl_tree_init(&t, 
		      my_allocate_avl_node,
		      my_free_avl_node,
		      my_int_compare);

	avl_tree_insert(&t, (void *) 10);
	assert(t.root->item == (void *) 10);

	avl_tree_insert(&t, (void *) 20);
	assert(t.root->item == (void *) 10);
	assert(t.root->right->item == (void *) 20);

	/*
	// Right Right Case
	// - rotate 10 left.
	//
	//   10          20
	//    \         /  \
	//     20     10    30
	//      \
	//       30
	*/
	avl_tree_insert(&t, (void *) 30);
	assert(t.root->item == (void *) 20);
	assert(t.root->left->item == (void *) 10);
	assert(t.root->right->item == (void *) 30);

	/*
	// (Still an AVL tree)
	//
	//    20
	//   /  \
	// 10    30
	//         \
	//          40
	*/
	avl_tree_insert(&t, (void *) 40);
	assert(t.root->item == (void *) 20);
	assert(t.root->left->item == (void *) 10);
	assert(t.root->right->item == (void *) 30);
	assert(t.root->right->right->item == (void *) 40);

	/*
	// Right Right case at 30
	// - rotate 30 left.
	//
	//    20              20
	//   /  \            /  \
	// 10    30        10    40
	//         \            /  \
	//          40        30    50
	//            \
	//             50
	*/
	avl_tree_insert(&t, (void *) 50);
	assert(t.root->item == (void *) 20);
	assert(t.root->left->item == (void *) 10);
	assert(t.root->right->item == (void *) 40);
	assert(t.root->right->left->item == (void *) 30);
	assert(t.root->right->right->item == (void *) 50);

	/*
	// Right Left Case at 20
	// - rotate 40 right.
	// - rotate 20 left.
	//
	//      20             20               30
	//     /  \           /  \             /  \
	//   10    40       10    30         20    40
	//        /  \           /  \       /  \    \
	//      30    50       25    40   10    25   50
	//     /                      \
	//   25                        50
	*/
	avl_tree_insert(&t, (void *) 25);
	assert(t.root->item == (void *) 30);
	assert(t.root->left->item == (void *) 20);
	assert(t.root->left->left->item == (void *) 10);
	assert(t.root->left->right->item == (void *) 25);
	assert(t.root->right->item == (void *) 40);
	assert(t.root->right->right->item == (void *) 50);


	last_level = 0;
	avl_tree_level_order(&t, print_tree_item, &last_level);
}

typedef struct _int_randomizer {
	int *array;
	int num_items;
	int num_to_consider;
} int_randomizer;

int * allocate_items(int num_items)
{
	int *arr = (int *) malloc(num_items * sizeof(int));
	int i;
	
	for (i = 0 ; i < num_items ; ++i)
		arr[i] = i;

	return arr;
}

int_randomizer * allocate_randomizer(int num_items)
{
	int_randomizer *r;

	r = (int_randomizer *) malloc(sizeof(int_randomizer));
	r->array = allocate_items(num_items);
	r->num_items = num_items;
	r->num_to_consider = num_items;

	return r;
}

void reset_randomizer(int_randomizer *r)
{
	r->num_to_consider = r->num_items;
}

int get_random(int_randomizer *r)
{
	int temp;
	int index = rand() % r->num_to_consider;
	--r->num_to_consider;

	temp = r->array[index];
	r->array[index] = r->array[r->num_to_consider];
	r->array[r->num_to_consider] = temp;

	return temp;
}

void free_randomizer(int_randomizer *r)
{
	free(r->array);
	free(r);
}

int brute_force_height(avl_tree_node *node)
{
	if (!node)
		return 0;
	return 1 + mymax(brute_force_height(node->left),
			 brute_force_height(node->right));
}

int is_avl_tree(avl_tree *t)
{
	int left_height;
	int right_height;

	int max_height;
	int min_height;

	if (!t->root)
		return 1;

	left_height = brute_force_height(t->root->left);
	right_height = brute_force_height(t->root->right);

	max_height = mymax(left_height, right_height);
	min_height = mymin(left_height, right_height);

	return (max_height - min_height) <= 1;
}

void insert_and_remove_stress(void)
{
	avl_tree t;
	int repetitions = 4096;
	int num_items;
	int max_items = 512;
	int j;
	int item;
	int i;

	avl_tree_init(&t, 
		      my_allocate_avl_node,
		      my_free_avl_node,
		      my_int_compare);

	for (j = 0 ; j < repetitions ; ++j) {
		for (num_items = 1 ; num_items < max_items ; ++num_items) {
			int_randomizer *r;

			r = allocate_randomizer(num_items);

			// add each item, randomly
			for (i = 0 ; i < num_items ; ++i) {
				item = get_random(r);
				avl_tree_insert(&t, (void *) (int64_t) item);
				assert(is_avl_tree(&t));
				assert(avl_tree_num_items(&t) == (uint32_t)i+1);
			}

			reset_randomizer(r);

			// remove each item, randomly
			for (i = 0 ; i < num_items ; ++i) {
				item = get_random(r);
				avl_tree_remove(&t, (void *) (int64_t) item);
				assert(is_avl_tree(&t));
				assert(avl_tree_num_items(&t) == ((uint32_t)num_items - ((uint32_t)i+1)));
			}

			free_randomizer(r);
		}
		printf(".");
		fflush(stdout);
	}

	avl_tree_destroy(&t);
}


int main(int argc, char *argv[])
{
	// simple_insert_and_level_order();
	insert_and_remove_stress();

	return 0;
}
