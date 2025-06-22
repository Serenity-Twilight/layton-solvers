#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "knights-tour-board.h"

//=====================================================================
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//=====================================================================
struct board {
	uint8_t knight_x;
	uint8_t knight_y;

	uint8_t width;
	uint8_t height;
	uint16_t move_count;
	uint16_t move_count_max;

	bool* visited;
	uint8_t* move_stack;
}; // end struct board

//=====================================================================
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//=====================================================================
enum { NUM_DISTINCT_MOVES = 8 };

static const char unsolvable_message[] = "Failed to solve %" PRIu8 "x%" PRIu8 " Knight's Tour.\n";
static const char logic_error_message[] = "A programming error has been encountered. The solver cannot continue.\n";

// Define deltas (destination - origin) for each move for x and y dimensions:
static const uint8_t x_deltas[NUM_DISTINCT_MOVES] = {2,1,-1,-2,-2,-1,1,2};
static const uint8_t y_deltas[NUM_DISTINCT_MOVES] = {-1,-2,-2,-1,1,2,2,1};

//=====================================================================
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//=====================================================================
static struct board* board_create(uint8_t width, uint8_t height);
static bool do_next_move(struct board* board, uint8_t minimum_move_id);
static uint8_t undo_last_move(struct board* board);
static bool try_move(struct board* board, uint8_t move_id);
static char* compose_solution_message(const struct board* board);

//=====================================================================
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//=====================================================================
char* solve_knights_tour(uint8_t board_width, uint8_t board_height) {
	struct board* board = board_create(board_width, board_height);
	if (board == NULL)
		return NULL;

	char* return_message = NULL;

	uint8_t next_move = 0;
	while (1) {
		if (do_next_move(board, next_move)) {
			// Move successful.
			if (board->move_count == board->move_count_max) {
				// Solution complete!
				// Convert solution into a human-readable message and terminate.
				return_message = compose_solution_message(board);
				goto cleanup;
			} else
				next_move = 0; // Start at move 0 for knight's new coordinates
		} else {
			//fprintf(stderr, "board->move_count = %" PRIu8 "\n", board->move_count);
			if (board->move_count == 0) {
				// Failed to solve the board.
				// Return an error message indicating this, assuming enough memory is available.
				int msglen = snprintf(NULL, 0, unsolvable_message, board_width, board_height);
//				fprintf(stderr, "return_message msglen = %d\n", msglen);
				return_message = malloc(msglen + 1);
//				fprintf(stderr, "return_message malloc = %p\n", return_message);
				if (return_message == NULL)
					goto cleanup;
				snprintf(return_message, msglen + 1, unsolvable_message, board_width, board_height);
//				fprintf(stderr, "return_message = \"%s\"\n", return_message);
//				fflush(stderr);
				goto cleanup;
			} // end failure-to-solve message composition

			// This path is a dead end, and will not solve the puzzle.
			// Undo the most recent move, and attempt a different move next time.
			next_move = undo_last_move(board) + 1;
			if (next_move == 0) {
				// This should never happen (because of prior `board->move_count == 0` check), but report if it does:
				return_message = malloc(sizeof(logic_error_message));
				if (return_message == NULL)
					goto cleanup;
				strncpy(return_message, logic_error_message, sizeof(logic_error_message));
				goto cleanup;
			}
		} // end ifelse move successful
	} // end brute force move iteration loop

cleanup:
	free(board);
	return return_message;
} // end solve_knights_tour()

//=====================================================================
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//=====================================================================

//=====================================================================
static struct board* board_create(uint8_t width, uint8_t height) {
	// The board tracks 2 elements of runtime-specified size:
	// - visited: Boolean flag for each space on the board, indicating
	//            whether or not it has been visited.
	//            sizeof(*visited) = width * height
	// - move_stack: The stack of previous moves the knight has taken.
	//          Since the knight begins on a space without taking any
	//          moves, and the knight can only visit each space once...
	//          sizeof(*move_stack) = width * height - 1
	
	size_t sizeof_visited = width * height;
	size_t sizeof_move_stack = sizeof_visited - 1;
	struct board* board = malloc(sizeof(struct board) + sizeof_visited + sizeof_move_stack);
	if (board == NULL)
		return NULL;

	// Memory layout of `board`:
	// [struct members] [`visited` array] [`move_stack` array]
	board->visited = (void*)board + sizeof(struct board);
	board->move_stack = (void*)(board->visited) + sizeof_visited;

	board->knight_x = 0;
	board->knight_y = 0;
	board->width = width;
	board->height = height;
	board->move_count = 0;
	board->move_count_max = sizeof_move_stack;

	// Clear all `visited` flags except for the knight's starting position at (0,0):
	board->visited[0] = true;
	for (size_t v = 1; v < sizeof_visited; ++v)
		board->visited[v] = false;

	return board;
} // end board_create()

//=====================================================================
static bool do_next_move(struct board* board, uint8_t minimum_move_id) {
	// A knight can take one of, at most, eight possible moves.
	// Each move is represented by a unique ID.
	// Move IDs are mapped to moves demonstrated in the following
	// diagram showing a 7x7 example board:
	//
	// Legend:
	//   K - The knight piece
	//   * - An empty space that the knight cannot reach in a single move.
	// 1-8 - An empty space that is the destination of one of knight's 8 possible moves.
	// * * * * * * *
	// * * 3 * 2 * *
	// * 4 * * * 1 *
	// * * * K * * *
	// * 5 * * * 8 *
	// * * 6 * 7 * *
	// * * * * * * *
	//
	// Limitations, based on the knight's starting coordinates:
	// - if x == 0, moves [3,4,5,6] are not possible.
	// - if x == 1, moves [4,5] are not possible.
	// - if x == (width - 2), moves [1,8] are not possible.
	// - if x == (width - 1), moves [1,2,7,8] are not possible.
	// - if y == 0, moves [1,2,3,4] are not possible.
	// - if y == 1, moves [2,3] are not possible.
	// - if y == (height - 2), moves [6,7] are not possible.
	// - if y == (height - 1), moves [5,6,7,8] are not possible.

	// Try all untried moves in the current board state until a valid one
	// is found, or all possible moves are found to be invalid:
	uint8_t move_id = minimum_move_id;
	while (move_id < NUM_DISTINCT_MOVES && !try_move(board, move_id))
		++move_id; // Move impossible, try next one

	// If move_id < NUM_DISTINCT_MOVES, then a move was successfully made.
	// Else, no moves are possible after prev_move_id.
	return (move_id < NUM_DISTINCT_MOVES);
} // end do_next_move()

//=====================================================================
static uint8_t undo_last_move(struct board* board) {
	if (board->move_count == 0)
		return ((uint8_t)-1); // No moves to undo.
	
	// Unvisit current space:
	uint16_t unvisit_index = board->knight_y * board->width + board->knight_x;
	board->visited[unvisit_index] = false;

	// Reverse most recent move:
	// (subtracting a moves' deltas will undo that move)
	uint8_t move_id = board->move_stack[--(board->move_count)];
	board->knight_x -= x_deltas[move_id];
	board->knight_y -= y_deltas[move_id];

	return move_id;
} // end undo_last_move()

//=====================================================================
static bool try_move(struct board* board, uint8_t move_id) {
	if (move_id >= NUM_DISTINCT_MOVES)
		return false; // Fail on undefined move IDs

	// Calculate destination (x,y) coordinates:
	uint8_t x = board->knight_x + x_deltas[move_id];
	uint8_t y = board->knight_y + y_deltas[move_id];

	// Test if the destination is within bounds of the board:
	// REMEMBER: DESTINATION COORDINATES ARE UNSIGNED
	if (x >= board->width || y >= board->height)
		return false; // out-of-bounds
	
	// Check if the destination has already been visited:
	uint16_t index = ((uint16_t)y) * board->width + x;
	if (board->visited[index])
		return false; // space already visited, and cannot be visited again
	
	// Move is possible. Perform move:
	// - update the knight's position
	// - mark destination as visited
	// - add move to move stack
	board->knight_x = x;
	board->knight_y = y;
	board->visited[index] = true;
	board->move_stack[(board->move_count)++] = move_id;
	
	return true;
} // end try_move()

//=====================================================================
static char* compose_solution_message(const struct board* board) {
	static const char intro_text[] = "Knight's Tour solution, %" PRIu8 "x%" PRIu8 " board, beginning at A1:\n";

	// X-coordinates are represented by letters A-Z, as if the alphabet represents a base-26 numbering system.
	// Meaing that if the board's width exceeds 26, X of 27 is "AA", 28 is "AB", 29 is "AC", and so on.
	// Y-coordinates are represented by numbers 1-infinity
	//
	// Each space (except A1) is printed exactly once, so the exact required character count can be calculated up front.
	size_t numbers_per_column;
	if (board->height <= 9)
		numbers_per_column = board->height;
	else if (board->height <= 99)
		numbers_per_column = ((size_t)(board->height)) * 2 - 9; // 2 digits per number, except for first 9 rows
	else // board->height < 256
		numbers_per_column = ((size_t)(board->height)) * 3 - 108; // 3 digits per number >= 100, 2 digits 10-99, 1 digit 1-9
	
	size_t letters_per_row;
	if (board->width <= 26)
		letters_per_row = board->width;
	else // board->width < 256
		letters_per_row = ((size_t)(board->width)) * 2 - 26; // 2 letters per row, except first 26 columns
	
	//   Intro message length
	// + Numbers per column * num columns
	// + Letters per row * num rows
	// + 1 character for line feed ('\n') per space
	// + 1 for NUL terminator
	// - 3 for "A1\n"
	size_t msg_size = snprintf(NULL, 0, intro_text, board->width, board->height);
	msg_size += numbers_per_column * board->width + letters_per_row * board->height + ((size_t)(board->width)) * board->height - 2;
	char* msg = malloc(msg_size);
	if (msg == NULL)
		return NULL;

	size_t msg_pos = snprintf(msg, msg_size, intro_text, board->width, board->height);

	// Retrace the knight's full path, printing out each move in order.
	uint8_t x = 0, y = 0;
	for (uint16_t m = 0; m < board->move_count_max; ++m) {
		x += x_deltas[board->move_stack[m]];
		y += y_deltas[board->move_stack[m]];

		// Print x letter(s)
		if (x < 26)
			msg[msg_pos++] = 'A' + x;
		else {
			msg[msg_pos++] = 'A' + (x / 26);
			msg[msg_pos++] = 'A' + (x % 26);
		}

		// Print y number and newline
		msg_pos += snprintf(msg + msg_pos, msg_size - msg_pos, "%" PRIu8 "\n", y + 1);
//		fprintf(stderr, "DEBUG: move_stack[%" PRIu16 "] = %" PRIu8 "\n", m, board->move_stack[m]);
	}

//	fprintf(stderr, "DEBUG: msg_size = %zu, msg_pos = %zu\n", msg_size, msg_pos);
	return msg;
} // end compose_solution_message()

