#ifndef LSGD_PLAYAREA_H
#define LSGD_PLAYAREA_H
#include <stdint.h>
#include "block.h"

struct playarea {
	uint32_t state_id;
	uint8_t collision[10];
	struct vec2_s8 block_coords[NUM_BLOCKS];
}; // end struct playarea

#endif // LSGD_PLAYAREA_H

