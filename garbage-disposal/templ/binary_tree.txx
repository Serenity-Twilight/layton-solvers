#ifndef BINARY_TREE_TXX
#define BINARY_TREE_TXX

template<typename key_t, typename data_t>
class binary_tree {
public:
	binary_tree() = default;

	bool insert(key_t&& key, data_t&& data) {
		if (!root) {
			root = new node(key, data, (node*)this);
			return true;
		}

		node* curr = root;
		for (;;) {
			if (key == curr->key)
				return false;
			else if (key < curr->key) {
				if (curr->left == nullptr) {
					curr->left = new node(key, data, curr);
					return true;
				} else
					curr = curr->left;
			} else { // key > curr->key
				if (curr->children[1] == nullptr) {
					curr->children[1] = new node(key, data, curr);
					return true;
				} else
					curr = curr->children[1];
			} // end evaluation of key to curr->key_v
		} // end iteration into the depths of the binary tree
	} // end insert()

private:
	struct node {
		node* left = nullptr;
		node* right = nullptr;
		node* parent;
		key_t key;
		data_t data;

		node(key_t& key, data_t& data, node* parent) {
			this->parent = parent;
			this->key = key;
			this->data = data;
		} // end constructor()
	}; // end struct node
	
	node* root = nullptr;
}; // end class binary_tree

#endif // BINARY_TREE_TXX
