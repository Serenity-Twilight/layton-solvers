#include <stdint.h>
#include "templ/binary_tree.txx"

int main() {
	binary_tree<uint64_t, uint64_t> tree;
	tree.insert(102, 114400);
	tree.insert(12, 1000);
	tree.insert(150, 1);
} // end main()

