#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include "block.h"

//=====================================================================
//---------------------------------------------------------------------
// INTERNAL FUNCTION DECLARATIONS
//---------------------------------------------------------------------
//=====================================================================
static uint32_t block0_coords_to_posid(const struct vec2_s8* coords);
static uint32_t block1_coords_to_posid(const struct vec2_s8* coords);
static uint32_t block2_coords_to_posid(const struct vec2_s8* coords);
static uint32_t block3_coords_to_posid(const struct vec2_s8* coords);
static uint32_t block4_coords_to_posid(const struct vec2_s8* coords);
static uint32_t block5_coords_to_posid(const struct vec2_s8* coords);

//=====================================================================
//---------------------------------------------------------------------
// GLOBAL CONSTANT DEFINITIONS
//---------------------------------------------------------------------
//=====================================================================

const struct block blocks[NUM_BLOCKS] = {
	{ // Square block
		.segments={.count=4, .offsets={{.x=0,.y=0},{.x=1,.y=0},{.x=0,.y=1},{.x=1,.y=1}}},
		// 11000000
		// 11000000
		.collision={0xC0, 0xC0},
		.coords_to_posid = block0_coords_to_posid,
		.posid_to_coords = NULL
	},
	{ // Top-left L-block
		.segments={.count=3, .offsets={{.x=0,.y=0},{.x=1,.y=0},{.x=1,.y=1}}},
		// 11000000
		// 01000000
		.collision={0xC0, 0x40},
	},
	{ // Top-right L-block
		.segments={.count=3, .offsets={{.x=0,.y=0},{.x=1,.y=0},{.x=0,.y=1}}},
		// 11000000
		// 10000000
		.collision={0xC0, 0x80},
	},
	{ // Bottom-left L-block
		.segments={.count=3, .offsets={{.x=0,.y=0},{.x=0,.y=1},{.x=1,.y=1}}},
		// 10000000
		// 11000000
		.collision={0x80, 0xC0},
	},
	{ // Bottom-right L-block
		.segments={.count=3, .offsets={{.x=1,.y=0},{.x=0,.y=1},{.x=1,.y=1}}},
		// 01000000
		// 11000000
		.collision={0x40, 0xC0},
	},
	{ // Rectangle block
		.segments={.count=2, .offsets={{.x=0,.y=0},{.x=1,.y=0}}},
		// 11000000
		// 00000000
		.collision={0xC0, 0x00},
	}
}; // end blocks[] definition
// Block definitions assume exact indices specified by `enum block_id`:
static_assert(BLOCK_SQUARE == 0);
static_assert(BLOCK_TLL == 1);
static_assert(BLOCK_TRL == 2);
static_assert(BLOCK_BLL == 3);
static_assert(BLOCK_BRL == 4);
static_assert(BLOCK_RECT == 5);

//=====================================================================
//---------------------------------------------------------------------
// INTERNAL FUNCTION DEFINITIONS
//---------------------------------------------------------------------
//=====================================================================

#if !NDEBUG && !LSGD_SKIP_BLOCK_BOUNDS_CHECKING
#	define test_cond(args) assert(args)
#elif !LSGD_SKIP_BLOCK_BOUNDS_CHECKING
#	define test_cond(args) if (!(args)) return (uint32_t)-1
#else
#	define test_cond(args) (void(0))
#endif

//----------------------------------------------------------------------
// Square block
static uint32_t block0_coords_to_posid(const struct vec2_s8* coords) {
	switch (coords->y) {
		case 1: test_cond(coords->x == 3); return 0;
		case 2: test_cond(coords->x == 3); return 1;
		case 3: test_cond(coords->x >= 1 && coords->x <= 5); return coords->x + 1;
		case 4: test_cond(coords->x >= 2 && coords->x <= 4); return coords->x + 5;
		case 5: test_cond(coords->x >= 2 && coords->x <= 4); return coords->x + 8;
		case 6: test_cond(coords->x == 3); return 13;
		case 7: test_cond(coords->x == 3); return 14;
		default: test_cond(0); break;
	} // end switch (coords->y)
} // end block0_coords_to_posid()

//----------------------------------------------------------------------
// Top-left L-block
static uint32_t block1_coords_to_posid(const struct vec2_s8* coords) {
	switch (coords->y) {
		case 1: test_cond(coords->x == 3); return 0;
		case 2: test_cond(coords->x == 3); return 1;
		case 3: test_cond(coords->x >= 1 && coords->x <= 5); return coords->x + 1;
		case 4: test_cond(coords->x >= 1 && coords->x <= 4); return coords->x + 6;
		case 5: test_cond(coords->x >= 2 && coords->x <= 4); return coords->x + 9;
		case 6: test_cond(coords->x >= 2 && coords->x <= 3); return coords->x + 12;
		case 7: test_cond(coords->x == 3); return 16;
		default: test_cond(0); break;
	} // end switch (coords->y)
} // end block1_coords_to_posid()

//----------------------------------------------------------------------
// Top-right L-block
static uint32_t block2_coords_to_posid(const struct vec2_s8* coords) {
	switch (coords->y) {
		case 1: test_cond(coords->x == 3); return 0;
		case 2: test_cond(coords->x == 3); return 1;
		case 3: test_cond(coords->x >= 1 && coords->x <= 5); return coords->x + 1;
		case 4: test_cond(coords->x >= 2 && coords->x <= 5); return coords->x + 5;
		case 5: test_cond(coords->x >= 2 && coords->x <= 4); return coords->x + 9;
		case 6: test_cond(coords->x >= 3 && coords->x <= 4); return coords->x + 11;
		case 7: test_cond(coords->x == 3); return 16;
		default: test_cond(0); break;
	} // end switch (coords->y)
} // end block2_coords_to_posid()

//----------------------------------------------------------------------
// Bottom-left L-block
static uint32_t block3_coords_to_posid(const struct vec2_s8* coords) {
	switch (coords->y) {
		case 1: test_cond(coords->x == 3); return 0;
		case 2: test_cond(coords->x >= 3 && coords->x <= 4); return coords->x - 2;
		case 3: test_cond(coords->x >= 1 && coords->x <= 5); return coords->x + 2;
		case 4: test_cond(coords->x >= 2 && coords->x <= 4); return coords->x + 6;
		case 5: test_cond(coords->x >= 2 && coords->x <= 4); return coords->x + 9;
		case 6: test_cond(coords->x == 3); return 14;
		case 7: test_cond(coords->x == 3); return 15;
		default: test_cond(0); break;
	} // end switch (coords->y)
} // end block3_coords_to_posid()

//----------------------------------------------------------------------
// Bottom-right L-block
static int32_t block4_coords_to_posid(const struct vec2_s8* coords) {
	switch (coords->y) {
		case 1: test_cond(coords->x == 3); return 0;
		case 2: test_cond(coords->x >= 2 && coords->x <= 3); return coords->x - 1;
		case 3: test_cond(coords->x >= 1 && coords->x <= 5); return coords->x + 2;
		case 4: test_cond(coords->x >= 2 && coords->x <= 4); return coords->x + 6;
		case 5: test_cond(coords->x >= 2 && coords->x <= 4); return coords->x + 9;
		case 6: test_cond(coords->x == 3); return 14;
		case 7: test_cond(coords->x == 3); return 15;
		default: test_cond(0); break;
	} // end switch (coords->y)
} // end block4_coords_to_posid()

//----------------------------------------------------------------------
// Rectangle block
static uint32_t block5_coords_to_posid(const struct vec2_s8* coords) {
	switch (coords->y) {
		case 1: test_cond(coords->x == 3); return 0;
		case 2: test_cond(coords->x == 3); return 1;
		case 3: test_cond(coords->x >= 1 && coords->x <= 5); return coords->x + 1;
		case 4: test_cond(coords->x >= 1 && coords->x <= 5); return coords->x + 6;
		case 5: test_cond(coords->x >= 2 && coords->x <= 4); return coords->x + 10;
		case 6: test_cond(coords->x >= 2 && coords->x <= 4); return coords->x + 13;
		case 7: test_cond(coords->x == 3); return 18;
		case 8: test_cond(coords->x == 3); return 19;
		default: test_cond(0); break;
	} // end switch (coords->y)
} // end block5_coords_to_posid()


