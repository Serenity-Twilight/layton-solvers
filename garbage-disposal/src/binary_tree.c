#include <stdalign.h>
#include <stddef.h>
#include "binary_tree.h"

#ifndef BINARY_TREE_ALIGNAS
#	define BINARY_TREE_ALIGNAS void*
#endif

//=====================================================================
//---------------------------------------------------------------------
// INTERNAL TYPE DEFINITIONS
//---------------------------------------------------------------------
//=====================================================================
struct bt_node {
	struct bt_node* left;
	struct bt_node* right;
	struct bt_node* prev;
	alignas(max_align_t) unsigned char data[];
}; // end struct bt_node

//=====================================================================
//---------------------------------------------------------------------
// EXTERNAL FUNCTION DEFINITIONS
//---------------------------------------------------------------------
//=====================================================================
size_t bt_node_size() {
	return sizeof(struct bt_node);
}

int bt_create_data(size_t size) {
	return -1;
}

int bt_update_position(void* data) {
	return -1;
}

