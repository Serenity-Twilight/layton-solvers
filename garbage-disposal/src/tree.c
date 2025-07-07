#ifndef key_t
#	error "`key_t` not specified before generating `binary_tree`."
#endif

#ifndef data_t
#	error "`data_t` not specified before generating `binary_tree`."
#endif

struct {
	struct node* left;
	struct node* right;
	struct node* parent;

	key_t key;
	data_t data;
} // end struct node

struct bitree {
	
} // end struct bitree

#undef BITREE_KEY_TYPE
#undef BITREE_DATA_TYPE

