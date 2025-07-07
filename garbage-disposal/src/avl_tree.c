#include <stdbool.h>
#include <stddef.h>
#include "avl_tree.h"

void avl_node_head_init(struct avl_node_head* self, struct avl_node_head* parent) {
	self->left = NULL;
	self->right = NULL;
	self->parent = parent;
	self->balance = 0;
} // end avl_node_head_init()

bool avl_insert(struct avl_tree* tree, struct avl_node_head* new_item) {
	// If tree is empty, then the first item always becomes the root:
	if (tree->root == NULL) {
		avl_node_head_init(new_item, (struct avi_node_head*)tree);
		tree->root = new_item;
		tree->num_entries += 1;
		return;
	}

	struct avl_node_head* curr_entry = tree->root;
	for (;;) {
		int diff = tree->cmp(new_entry, curr_entry);
		if (diff == 0) {
			// Tree cannot contain 2 entries with identical keys.
			// Insertion fails.
			return false;
		} else if (diff < 0) {
			// new_entry < curr_entry: Proceed left.
			if (curr_entry->left == NULL) {
				avl_node_head_init(new_item);
				
			}
		} else { // (diff > 0)
			// new_entry > curr_entry: Proceed right.
		}
	}
} // end avl_insert()

