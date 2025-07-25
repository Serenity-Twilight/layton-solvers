#include <stdint.h>
#include "block.h"

enum {
	// Collision is encoded as
	// - least-sigificant bit is equal to top-left corner
	// - most-sigificant bit (of 48 bits) is equal to bottom-right corner
	// - proceeds as row-major (iterate across each column before proceeding the next row)
	//
	EMPTY_COLLISION = 0xCF3861000CF3L, // 1100 1111 0011 1000 0110 0001 0000 0000 0000 1100 1111 0011
	
	SQUARE_START = 0,
	TLL_START = 8,
	TRL_START = 9,
	BLL_START = 
}; // end constants definition

// Collision is encoded as:
// - 1 byte = 1 row, 1 bit = 1 column within a row
// - y=0-9, top-to-bottom, index equals y-coordinate
// - x=0-7, left-to-right, 0 is most-significant bit, 7 is least-significant bit.

// def EMPTY_COLLISION
// Collision when there are no blocks:
// 11111111
// 11100111
// 11100111
// 10000001
// 10000001
// 11000011
// 11000011
// 11100111
// 11100111
// 11111111
const uint8_t EMPTY_COLLISION[10] = { 0xFF, 0xE7, 0xE7, 0x81, 0x81, 0xC3, 0xC3, 0xE7, 0xE7, 0xFF };

// Starting positions of blocks (a.k.a. after 0 moves)
// (see graphic 'compressed-position-id-map' for the meaning behind these numbers)
const uint8_t BLOCK_INIT_POSITIONS[6] = {
	0, // Square block
	8, // Top-left L-block
	9, // Top-right L-block
	11, // Bottom-left L-block
	13, // Bottom-right L-block
	18, // Rectangle block
}; // end BLOCK_INIT_POSITIONS[]

void playarea_init(struct playarea* playarea) {
	//memcpy(playarea->collision, 
} // end playarea_init()

