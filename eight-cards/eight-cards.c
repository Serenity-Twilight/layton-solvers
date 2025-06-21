#include <stdbool.h>
#include <stdio.h>

//=====================================================================
//---------------------------------------------------------------------
// INTERNAL FUNCTION DECLARATIONS
//---------------------------------------------------------------------
//=====================================================================
static inline bool is_solution(int product, bool cards_used[8]);
static inline void print_solution(int product, int multiplicand, int multiplier);

//=====================================================================
//---------------------------------------------------------------------
// MAIN FUNCTION
//---------------------------------------------------------------------
//=====================================================================
int main() {
	// Tracks cards used in a particular iteration.
	bool cards_used[8] = {false};

	for (int num1 = 0; num1 < 8; ++num1) {
		cards_used[num1] = true;
		for (int num2 = 0; num2 < 8; ++num2) {
			if (cards_used[num2])
				continue; // num2 must be unique
			cards_used[num2] = true;
			for (int num3 = 0; num3 < 8; ++num3) {
				if (cards_used[num3])
					continue; // num3 must be unique
				cards_used[num3] = true;
				for (int num4 = 0; num4 < 8; ++num4) {
					if (cards_used[num4])
						continue; // num4 must be unique
					cards_used[num4] = true;

					// Assemble numbers 1-3 into the hundreds, tens, and ones-places
					// of a 3-digit multiplicand, respectively.
					// Multiply the multiplicand by number 4, a 1-digit multiplier.
					// Test if the digits of the product are all unique from both each other
					// and the digits of the multiplicand and multiplier.
					int multiplicand = ((num1 + 1) * 100 + (num2 + 1) * 10 + (num3 + 1));
					int multiplier = num4 + 1;
					int product = multiplicand * multiplier;
					if (is_solution(product, cards_used))
						print_solution(product, multiplicand, multiplier);
					
					cards_used[num4] = false;
				} // end num4 iteration
				cards_used[num3] = false;
			} // end num3 iteration
			cards_used[num2] = false;
		} // end num2 iteration
		cards_used[num1] = false;
	} // end num1 iteration

	return 0;
} // end main()

//=====================================================================
//---------------------------------------------------------------------
// INTERNAL FUNCTION DEFINITIONS
//---------------------------------------------------------------------
//=====================================================================

//=====================================================================
static inline bool is_solution(int product, bool cards_used[8]) {
	// Is product below minimum solution or above maximum solution?
	if (product < 1234 || product > 8765)
		return false;

	// Check if each of the numbers in the 4-digit product is unique,
	// including the numbers of the multiplicand and multiplier:
	bool is_valid = false;

	// Decrease all digits of the product by 1 to make them zero-indexable:
	product -= 1111;

	// Is number 5 (thousands-place) unique?
	int num5 = (product / 1000);
	if (num5 < 0 || num5 > 7 || cards_used[num5])
		goto num5_not_unique;
	cards_used[num5] = true;

	// Is number 6 (hundreds-place) unique?
	int num6 = ((product / 100) % 10);
	if (num6 < 0 || num6 > 7 || cards_used[num6])
		goto num6_not_unique;
	cards_used[num6] = true;

	// Is number 7 (tens-place) unique?
	int num7 = ((product / 10) % 10);
	if (num7 < 0 || num7 > 7 || cards_used[num7])
		goto num7_not_unique;
	cards_used[num7] = true;

	// Is number 8 (ones-place) unique?
	int num8 = (product % 10);
	if (num8 < 0 || num8 > 7 || cards_used[num8])
		goto num8_not_unique;

	// All numbers are unique. This is a valid solution:
	is_valid = true;
	// Always unset any cards_used flags set by this function:
num8_not_unique:
	cards_used[num7] = false;
num7_not_unique:
	cards_used[num6] = false;
num6_not_unique:
	cards_used[num5] = false;
num5_not_unique:
	return is_valid;
} // end is_solution()

//=====================================================================
static inline void print_solution(int product, int multiplicand, int multiplier) {
	printf("%d * %d = %d\n", multiplicand, multiplier, product);
} // end print_solution()

