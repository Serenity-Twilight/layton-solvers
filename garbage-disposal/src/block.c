#include <stdint.h>

struct block {
	uint8_t num_segments;
	int8_t segments[4][2];
} // end struct block

// TODO: Document origin segment of each block.
struct block blocks[6] = {
	{ .num_segments=4, .segments={{0,0},{1,0},{0,1},{1,1}}}, // Trash (square) block
	{ .num_segments=3, .segments={{0,0},{-1,0},{0,1}}},      // Top-left L-block
	{ .num_segments=3, .segments={{0,0},{1,0},{0,1}}},       // Top-right L-block
	{ .num_segments=3, .segments={{0,0},{1,0},{0,-1}}},      // Bottom-left L-block
	{ .num_segments=3, .segments={{0,0},{-1,0},{0,-1}}},     // Bottom-right L-block
	{ .num_segments=2, .segments={{0,0},{1,0}}}              // Rectangle block
};

