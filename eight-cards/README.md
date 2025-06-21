# Eight Cards
A brute-force solver for the post-game puzzle in Professor Layton and the Diabolical Box by the same name.

## Puzzle rules
The player is presented with 8 cards, each numbered uniquely 1-8.
Using the cards, it is possible to create 2 correct multiplicative equations of the following format:\
**<3-digit multiplicand> * <1-digit multiplier> = <4-digit product>**

Each card (and thus each digit) must be used exactly once.

The player is given one of the two equations possible under these conditions:\
**453 * 6 = 2718**

With no further information, the player must find the other equation.

## Usage
Build the program using `make`. The Makefile assumes `gcc` is your compiler.
You can also build the program from the command line with `gcc eight-cards.c -O3 -oeight-cards`.
You may substitute `gcc` any C compiler that has C Standard Library support.

I wrote, compiled, and ran this program on Arch Linux, but the only library it uses is the C Standard Library, so any environment which supports that should work.

The program takes no arguments, as Professor Layton and the Diabolical Box does not have any alternative versions of this puzzle.

## Program development
This is the first puzzle that I made an automatic solver for, and what gave me the idea to do so.
With 8 unique digits, it is possible to create 40320 unique combinations (a.k.a. 8! (factorial)).
Of these, only 2 create correct equations under these circumstances.

So rather than solving this manually, I wrote this program to try every combination and output the 2 correct equations.

The program took about an hour to write, and on my computer solves the puzzle instantaneously.

