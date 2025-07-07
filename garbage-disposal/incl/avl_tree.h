#ifndef AVL_TREE_H
#define AVL_TREE_H

struct avl_node_head {
	struct avl_node_head* left;
	struct avl_node_head* right;
	struct avl_node_head* parent;
	signed int balance: 2;
}; // end struct avl_node_head()

struct avl_tree {
	struct avl_node_head* root;
	int (*cmp)(struct avl_node_head*, struct avl_node_head*);
	size_t num_entries;
}; // end struct avl_tree()

#endif // AVL_TREE_H

