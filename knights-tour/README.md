# Knight's Tour
A brute-force solver for the "Knight's Tour" puzzle series seen in Professor Layton and the Diabolical Box.

## Puzzle Rules
This series of puzzles follows the same rules as [the real life puzzle of the same name](https://en.wikipedia.org/wiki/Knight%27s_tour).

In Knight's Tour, the objective is to move a single knight chess piece onto every space of an arbitrarily-sized chess board exactly once. That is: the knight must go to every space, and cannot go to a space it has already been to before. The Layton version simplifies the puzzle by making the knight always start on A1 (the top-leftmost space).

Professor Layton and the Diabolical Box has 4 variations of this puzzle, with the only difference of each being the size of the board:
1. 3x4 (3 columns, 4 rows)
2. 5x6
3. 6x6
4. 8x8

## Building
Build the program using `make`. The Makefile assumes `gcc` is your compiler.
You can also build the program from the command line with `gcc knights-tour-board.c main.c -O3 -oknights-tour`.
You may substitute `gcc` any C compiler that has C Standard Library support.

I wrote, compiled, and ran this program on Arch Linux, but the only library it uses is the C Standard Library, so any environment which supports that should work.

## Usage
The program takes 2 arguments:\
`knights-tour <cols> <rows>`\
`<cols>`: The number of columns on the board, a.k.a. its width in spaces.\
`<rows>`: The number of rows on the board, a.k.a. its height in spaces.

If the program successfully finds a solution, it will print the solution in the form of a list of spaces in the order the knight should travel to them. Spaces are printed in [algebraic notation](https://en.wikipedia.org/wiki/Algebraic_notation_(chess)).\
Two notes on this:\
1. Columns 1-26 are labeled as A-Z. If the board has more then 26 columns, subsequent columns are labeled using letters as if they were digits of a base-26 numbering system. So column 27 is AA, 28 is AB, 51 is AY, 52 is AZ, 53 is BA, and so on.
2. The Y-coordinate starts at the top and increases going down because that's how computer graphics handle vertical coordinates and I'm super computer-brained and I did not know that algebraic notation counts up from the bottom until just now. :smiling_face_with_tear:

Running the program without any arguments gives an explanation on how to use the program, much like the one here.

PERFORMANCE NOTE:\
If the board dimensions given have a possible solution, or are trivially impossible (like a 8x1 board or something silly like that), then this program is very fast. Even an 8x8 only takes the program about 1 second to find a solution on my computer when compiled with `-O3` (highest optimization level in GCC, barring experimental features), and about 3 seconds without any compiler optimizations.

However, with certain dimensions with lots of possible moves but that are potentially unsolvable, the program will run for a very, very long time. In testing, I killed the program after waiting over an hour for a solution to a 10x10 board. So, if the program appears to stall on you, either your board's too big, or it most likely is impossible.

## Development notes
While I was able to solve the 3x4 Knight's Tour on my own, I hit a wall as soon as the 5x6 variation. Then I saw that there was an 8x8 variation in the post-game puzzles, and I decided I'd rather write this program than continue to guess the solution.

Writing this program took about 3 hours, which is significantly less time than it would have taken me to solve 3 more Knight's Tour puzzles.

I'm very proud of my resource-conscious implementation to this problem. The program is structured around [Depth-First Search](https://en.wikipedia.org/wiki/Depth-first_search) using compile-time constant IDs for the, at most, 8 possible moves the knight can take at any unique board state. Thus, each move in the stack of moves taken by the knight uses 1 byte. For an 8x8 board, with 64 flags to record visits (1 per space) and a 63-byte stack to remember moves (the knight begins at A1, so there are exactly 63 moves to solve the puzzle), the dynamic memory footprint of the solver is 151 bytes (including 24 bytes of static state variables and accounting for standard x86_64 memory alignment requirements).

As for the speed, I've considered micro-optimizations that may make the program slightly faster, but realistically the only thing that would have any notable impact would be a smarter algorithm that better understands the nuances of the Knight's Tour puzzle, such as if certain chains of moves are statistically more likely to work than others. The program, as it is now, just tries every possible combination of moves until it reaches a solution. And even on something seemingly as small as an 8x8 board, there are approximately more than 10^40 possible unique moves under unique board states. It's a very complex puzzle!

If I ever decide to revisit this program for whatever reason, I'd like to modify the program to be more transparent with how much work its doing by:
1. splitting the solver onto a different thread,
2. using an object shared between the solver thread and main thread to report runtime stats, such as how many moves have been attempted, and
3. having the now-free main thread print out live statistics at regular intervals.

This would give a better idea of how much work the program is doing when it fails to find a solution, and thus would make it easier to know if the program got stuck from some logic error, or if the puzzle really is just that complex that it takes too long to solve.
