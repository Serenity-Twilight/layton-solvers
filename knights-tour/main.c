#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "knights-tour-board.h"

//=====================================================================
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//=====================================================================
static bool parse_args(uint8_t out[2], int argc, char* argv[]);
static bool stoui(uint64_t* out, char* in);

//=====================================================================
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//=====================================================================
int main(int argc, char* argv[]) {
	uint8_t argi[2];
	if (!parse_args(argi, argc, argv))
		return 0;

	char* result = solve_knights_tour(argi[0], argi[1]);
	if (result == NULL)
		fputs("Insufficient memory error.\n", stderr);
	else {
		fputs(result, stdout);
		free(result);
	}
} // end main()

//=====================================================================
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//=====================================================================
static bool parse_args(uint8_t out[2], int argc, char* argv[]) {
	if (argc == 1) {
		// No arguments (beyond executable filepath).
		// Print usage and exit.
		fputs("Usage:\n"
				"\tknights-tour <cols> <rows>\n"
				"\n"
				"Description:\n"
				"\tUses brute force to solve the variation of Knight's Tour seen in Professor Layton and the Diabolical Box.\n"
				"\tSpecifically, this variation of Knight's Tour always begins the knight at the top-leftmost space (A1).\n"
				"\n"
				"\tThe solution is printed out as 1 move per line.\n"
				"\tMoves are formatted as \"<A-IW><1-255>\", described in further detail below:\n"
				"\n"
				"\tLetters represent the X-coordinate on the board.\n"
				"\tThe first 26 columns are labeled with A-Z.\n"
				"\tSubsequent rows are labeled with letters as if the alphabet were a base-26 numbering system.\n"
				"\tSo, for example, row 27 is AA, row 28 is AB, row 52 is AZ, row 53 is BA, and so on.\n"
				"\n"
				"\tNumbers represent the Y-coordinate on the board.\n"
				"\tRow numbering should be intuitive. Note the first row is labeled as row 1 (not 0).\n"
				"\n"
				"Arguments:\n"
				"\t- <cols>: The width, or number of columns on the board, in spaces. Min: 1; Max: 255.\n"
				"\t- <rows>: The height, or number of rows on the board, in spaces. Min: 1; Max: 255.\n"
				, stdout);
		return false;
	} // end if (argc == 1)
	
	if (argc == 2) {
		fputs("ERROR: This program requires at least 2 arguments to function.\n", stdout);
		goto input_failure;
	}

	// At least 3 arguments (including executable filepath) confirmed beyond this point.
	uint64_t val;
	// Parse <cols> argument
	if (argv[1][0] == '-') {
		fputs("ERROR: Number of columns cannot be less than 3.\n", stdout);
		goto input_failure;
	} else if (!stoui(&val, argv[1])) {
		printf("ERROR: '%s' is not recognized as a non-negative integer.\n", argv[1]);
		goto input_failure;
	} else if (val < 3) {
		fputs("ERROR: Number of columns cannot be less than 3.\n", stdout);
		goto input_failure;
	} else if (val > 255) {
		fputs("ERROR: Number of columns cannot be greater than 255.\n", stdout);
		goto input_failure;
	}
	out[0] = val & 0xFF;

	// Parse <rows> argument
	if (argv[2][0] == '-') {
		fputs("ERROR: Number of rows cannot be less than 3.\n", stdout);
		goto input_failure;
	} else if (!stoui(&val, argv[2])) {
		printf("ERROR: '%s' is not recognized as a non-negative integer.\n", argv[2]);
		goto input_failure;
	} else if (val < 3) {
		fputs("ERROR: Number of rows cannot be less than 3.\n", stdout);
		goto input_failure;
	} else if (val > 255) {
		fputs("ERROR: Number of rows cannot be greater than 255.\n", stdout);
		goto input_failure;
	}
	out[1] = val & 0xFF;

	return true;
input_failure:
	fputs("Run this program without arguments for a detailed explanation of usage.\n", stdout);
	return false;
} // end parse_args()

//=====================================================================
static bool stoui(uint64_t* out, char* in) {
	if (*in == 0) // strlen == 0
		return false;
	if (*in < '0' || *in > '9')
		return false; // Not string-encoded non-negative integer

	*out = *in - '0';
	for (++in; *in; ++in) {
		if (*in < '0' || *in > '9')
			return false; // Not string-encoded non-negative integer
		*out = *out * 10 + *in - '0';
	}

	return true;
} // end stoui

