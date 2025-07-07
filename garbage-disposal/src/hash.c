#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

enum {
	UINT8_RANGE = 1 << 8,
	UINT20_RANGE = 1 << 20,
	UINT36_RANGE = 1L << 36,

	FIB8 = 413,
	FIB36 = 111188113369L,

	PUZZLE_STATES = 22195200,
	LOOKUPS = PUZZLE_STATES * 4,

	XY0_MASK = 0x3FL,
	XY1_MASK = 0xFC0L,
	XY2_MASK = 0x3F000L,
	XY3_MASK = 0xFC0000L,
	XY4_MASK = 0x3F000000L,
	XY5_MASK = 0xFC0000000L,

	XY0_INC = 0x1L,
	XY1_INC = 0x40L,
	XY2_INC = 0x1000L,
	XY3_INC = 0x40000L,
	XY4_INC = 0x1000000L,
	XY5_INC = 0x40000000L,

	XY0_END = 0x30L,
	XY1_END = 0xC00L, // 0b1100'0000'0000
	XY2_END = 0x30000L,
	XY3_END = 0xC00000L,
	XY4_END = 0x30000000L,
	XY5_END = 0xC00000000L,

	HITS_LENGTH = 1024,
};

int hash(int in) {
	return (((long long int)in) * FIB36) % 1024;
}

// Each block's dimensions can be X=0-5,Y=0-7
// This gives each block 48 possible coordinate combinations
// (including out-of-bounds areas for simplicity).
//
// Since each block's coordinates are stored as 6-bits,
// each block's coordinates has 16 (= 64 - 48) unused values.
//
// The following hash reduces the range of each block's coordinates
// in the final value from 64 to 48, improving linear distribution
// over a hash table.
size_t coord_hash(size_t in) {
	// Hash table unlikely to get big enough to use block 5 coordinates.
	// Commented out analysis of block 5 coordinates for now:
	//uint64_t xy5 = in & XY5_MASK;

	size_t xy4 = in & XY4_MASK;
	// 64^4 - 48^4 = 11468800 = 2^23 + 2^21 + 2^19 + 2^18 + 2^17 + 2^16
	xy4 -= (xy4 >> 1) + (xy4 >> 3) + (xy4 >> 5) + (xy4 >> 6) + (xy4 >> 7) + (xy4 >> 8);

	size_t xy3 = in & XY3_MASK;
	// 64^3 - 48^3 = 151552 = 2^17 + 2^14 + 2^12
	xy3 -= (xy3 >> 1) + (xy3 >> 4) + (xy3 >> 6);

	size_t xy2 = in & XY2_MASK;
	// 64^2 - 48^2 = 1792 = 2^11 + 2^8
	xy2 -= (xy2 >> 1) + (xy2 >> 4);

	size_t xy1 = in & XY1_MASK;
	// 64 - 48 = 16, xy1 - (xy1 / 4)
	xy1 -= xy1 >> 2;

	return (in & XY0_MASK) + xy1 + xy2 + xy3 + xy4;
} // end coord_hash()

int badhash(int in) {
	return in % HITS_LENGTH;
}

int main() {
	int hits[HITS_LENGTH] = {0};
	for (uint64_t xy5 = 0; xy5 < XY5_END; xy5 += XY5_INC) {
		for (uint64_t xy4 = 0; xy4 < XY4_END; xy4 += XY4_INC) {
			for (uint64_t xy3 = 0; xy3 < XY3_END; xy3 += XY3_INC) {
				for (uint64_t xy2 = 0; xy2 < XY2_END; xy2 += XY2_INC) {
					for (uint64_t xy1 = 0; xy1 < XY1_END; xy1 += XY1_INC) {
						for (uint64_t xy0 = 0; xy0 < XY0_END; xy0 += XY0_INC)
							hits[coord_hash(xy0 + xy1 + xy2 + xy3 + xy4 + xy5) % HITS_LENGTH] += 1;
					}
				}
			}
		}
	}

	FILE* file = fopen("out", "w");
	for (int i = 0; i < HITS_LENGTH; ++i)
		fprintf(file, "%d\n", hits[i]);
	fclose(file);
}

