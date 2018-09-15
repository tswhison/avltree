/*
** main.c : test program for AVL Trees
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
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "avl.h"
#include "avl_util.h"

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
	}

	return node;
}

avl_queue_entry * my_allocate_avl_entry(avl_tree_node *node)
{
	avl_queue_entry *entry = (avl_queue_entry *)
			malloc(sizeof(avl_queue_entry));
	if (entry)
		entry->node = node;

	return entry;
}

void my_free_avl_node(avl_tree_node *node)
{
	free(node);
}

void my_free_avl_entry(avl_queue_entry *entry)
{
        free(entry);
}

int64_t my_int_compare(void *a, void *b)
{
	int64_t ia = (int64_t) a;
	int64_t ib = (int64_t) b;
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
		      my_int_compare,
		      my_allocate_avl_entry,
		      my_free_avl_entry);

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

	avl_tree_destroy(&t);
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
#ifdef MEMCHECK
	int repetitions = 1000;
	int max_items = 128;
#else
	int repetitions = 10000;
	int max_items = 128;
#endif // MEMCHECK
	int num_items;
	int j;
	int item;
	int i;

	avl_tree_init(&t, 
		      my_allocate_avl_node,
		      my_free_avl_node,
		      my_int_compare,
		      my_allocate_avl_entry,
		      my_free_avl_entry);

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

		if (!(j % 100)) {
			printf(".");
			fflush(stdout);
		}
	}

	avl_tree_destroy(&t);
}

typedef struct _visit_sequence {
	int i;
	int item_sequence[3];
} visit_sequence;

void sequence_visitor(avl_tree_node *node, void *context)
{
	visit_sequence *seq = (visit_sequence *) context;
	assert(seq->item_sequence[seq->i++] == (int) (int64_t) node->item);
}

void sequence_visitor_level(avl_tree_node *node, void *context, int level)
{
	visit_sequence *seq = (visit_sequence *) context;
	(void) level;
	assert(seq->item_sequence[seq->i++] == (int) (int64_t) node->item);
}

void other_coverage(void)
{
	avl_tree t;
	avl_tree_node *n;
	visit_sequence seq;
	int i;

	avl_tree_init(&t, 
		      my_allocate_avl_node,
		      my_free_avl_node,
		      my_int_compare,
		      my_allocate_avl_entry,
		      my_free_avl_entry);
	assert(!t.root);

	avl_tree_destroy(&t);
	assert(!t.root);
	assert(0 == avl_tree_num_items(&t));
	assert(!avl_tree_find(&t, (void *) 1));
	assert(0 == avl_tree_height(&t));

	seq.i = 0;
	avl_tree_pre_order(&t, sequence_visitor, &seq);
	avl_tree_in_order(&t, sequence_visitor, &seq);
	avl_tree_post_order(&t, sequence_visitor, &seq);
	avl_tree_level_order(&t, sequence_visitor_level, &seq);
	assert(0 == seq.i);

	assert(avl_tree_insert(&t, (void *) 1));
	assert(avl_tree_insert(&t, (void *) 2));
	assert(avl_tree_insert(&t, (void *) 0));

	// duplicates not allowed
	assert(!avl_tree_insert(&t, (void *) 0));
	assert(3 == avl_tree_num_items(&t));
	n = avl_tree_find(&t, (void *) 2);
	assert(n);
	assert(n->item == (void *) 2);
	assert(!avl_tree_find(&t, (void *) 3));

	n = avl_tree_find(&t, (void *) 0);
	assert(n);
	assert(n->item == (void *) 0);
	n = avl_tree_find(&t, (void *) 1);
	assert(n);
	assert(n->item == (void *) 1);
	n = avl_tree_find(&t, (void *) 2);
	assert(n);
	assert(n->item == (void *) 2);

	// test pre-order traversal
	seq.i = 0;
	seq.item_sequence[0] = 1;
	seq.item_sequence[1] = 0;
	seq.item_sequence[2] = 2;
	avl_tree_pre_order(&t, sequence_visitor, &seq);
	assert(3 == seq.i);

	// test in-order traversal
	seq.i = 0;
	seq.item_sequence[0] = 0;
	seq.item_sequence[1] = 1;
	seq.item_sequence[2] = 2;
	avl_tree_in_order(&t, sequence_visitor, &seq);
	assert(3 == seq.i);

	// test post-order traversal
	seq.i = 0;
	seq.item_sequence[0] = 0;
	seq.item_sequence[1] = 2;
	seq.item_sequence[2] = 1;
	avl_tree_post_order(&t, sequence_visitor, &seq);
	assert(3 == seq.i);

	// test level-order traversal
	seq.i = 0;
	seq.item_sequence[0] = 1;
	seq.item_sequence[1] = 0;
	seq.item_sequence[2] = 2;
	avl_tree_level_order(&t, sequence_visitor_level, &seq);
	assert(3 == seq.i);

	for (i = 3 ; i < 10 ; ++i)
		assert(avl_tree_insert(&t, (void *) (int64_t) i));

	assert(!avl_tree_remove(&t, (void *) (int64_t) 99));

	avl_tree_destroy(&t);
	assert(!t.root);

	avl_tree_balance_node(NULL);
}

int main(int argc, char *argv[])
{
	simple_insert_and_level_order();
	insert_and_remove_stress();
	other_coverage();
	return 0;
}
