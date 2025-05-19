# Match-3 Candy Game in C

A simple command-line **Match-3 Candy Game** written in C, inspired by classic puzzle games like Candy Crush.  
Swap adjacent candies to make matches of three or more, clear them, and let new candies fall with gravity!

---

## Features

- **6x6 Game Board**: Swap candies to match three or more in a row or column.
- **Gravity and Cascade**: Cleared candies trigger falling and cascades.
- **Refill from Stack**: New candies are filled from a stack defined in the input file.
- **File-based Initialization**: Game board and stacks are loaded from `board.txt`.
- **Game Over Detection**: The game ends when no valid moves or candies remain.

---

## Getting Started

## Example of each step

=====
New Round:
  | 0 | 1 | 2 | 3 | 4 | 5 |
 0| # | % | @ | * | # | * |
 1| @ | @ | * | # | # | % |
 ...
Enter the coordinate (row, column) of the candy: 2 3
Enter the direction to swap (U for Up, D for Down, L for Left, R for Right): D

