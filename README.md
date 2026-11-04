# README


## Authors :
- Student 2
- Project contributor
- Student 3


## Rules :
This game is a simple one-player puzzle. The game starts with a grid of squares,
some of which contain *trees* (🌲). The goal of the game is to place *tents* (⛺)
in the *empty* squares (⬜) in such a way that the following conditions are met:

- Rule 1) No two tents are adjacent, even diagonally.
- Rule 2) The number of tents in each row, and in each column, matches the
  expected numbers given around the sides of the grid.
- Rule 3) There are exactly as many tents as trees.
- Rule 4) Each tent must be orthogonally adjacent (horizontally or vertically,
  but not diagonally) to at least one tree and vice versa.

Besides, it is also possible to place *grass* (🟩) in a square, indicating that
you are sure there is no tent on this square. The *grass* square gives a clue,
but it is not necessary to use it to win the game.

In summary, the game uses 4 different squares:

- empty (⬜ or ' ')
- tree (🌲 or 'x')
- tent (⛺ or '*')
- grass (🟩 or '-')

By convention, the square in the *i*-th row and *j*-th column of the grid is
referred to as the coordinate (i,j), and the coordinate (0,0) corresponds to the
top left corner. The dimension of the default game is 8x8.

You can try the original game on the famous <a
href="https://www.chiark.greenend.org.uk/~sgtatham/puzzles/js/tents.html"> Simon
Tatham's Portable Puzzle Collection</a>


## Example: Default Game

As an example, let's see the instance of the game, considered thereafter as the
*default* game:

```
🌲⬜⬜⬜⬜🌲🌲⬜ 1
⬜⬜⬜⬜⬜⬜⬜⬜ 2
⬜⬜⬜⬜⬜⬜🌲⬜ 1
🌲⬜🌲⬜⬜⬜⬜⬜ 2
⬜⬜⬜⬜🌲⬜⬜⬜ 2
⬜⬜🌲⬜⬜⬜⬜⬜ 0
⬜⬜⬜⬜⬜⬜⬜⬜ 1
⬜🌲🌲⬜⬜🌲🌲⬜ 3
 3 0 2 1 2 1 2 1
```

Hereafter, its solution:

```
🌲🟩🟩🟩⛺🌲🌲🟩 1
⛺🟩🟩🟩🟩🟩⛺🟩 2
🟩🟩⛺🟩🟩🟩🌲🟩 1
🌲🟩🌲🟩⛺🟩⛺🟩 2
⛺🟩⛺🟩🌲🟩🟩🟩 2
🟩🟩🌲🟩🟩🟩🟩🟩 0
🟩🟩🟩🟩🟩⛺🟩🟩 1
⛺🌲🌲⛺🟩🌲🌲⛺ 3
 3 0 2 1 2 1 2 1
```

Try it: https://www.chiark.greenend.org.uk/~sgtatham/puzzles/js/tents.html#8x8de#911105660705777


## About the Rule 4

Note that the Rule 4) is not exactly formulated in the same way in the original
game. According to <a
href="https://www.chiark.greenend.org.uk/~sgtatham/puzzles/doc/tents.html">Simon
Tatham</a>, the rule 4) is defined by a *matching* condition:

> It is possible to match tents to trees so that each tree is orthogonally
> adjacent to its own tent (but may also be adjacent to other tents).

Another way of phrasing the tree/tent matching criterion is to say that the
bipartite adjacency graph between trees and tents has a perfect matching. And
there is no trivial algorithm to check this condition (see <a
href="https://en.wikipedia.org/wiki/Hopcroft%E2%80%93Karp_algorithm">
Hopcroft–Karp algorithm</a>).

Here is an example that satisfies the Rule 4) as we have stated it, but does not
satisfy the *matching criterion* formulated in the original rule.

```
🌲⬜⬜
⬜⬜⬜
⬜⬜🌲
```

The variant of the Rule 4) we have chosen is a local criterion, that is much easier to implement.


## Game Extension (Version 2)

We have implemented an extended version of the game with the following features:

> Arbitrary Dimensions & Wrapping
Rectangular Grids: The grid can now have any number of rows and columns.

Toric Topology (Wrapping): If enabled, the grid "wraps" around (the last row is adjacent to the first row, and the same for columns).

> New Piece: BIGTREE (#)
A new square type BIGTREE is introduced:

Rule update: A TREE (🌲) requires at least one tent. A BIGTREE (🌳) requires exactly two orthogonal tents.

> Move History (Undo/Redo)
You can now navigate through your move history:

Undo (game_undo): Revert the last move.

Redo (game_redo): Replay a move that was just undone.

Note: Playing a new move clears the Redo history.


## Programming Interface

The game interface is divided into diferent files:

- Basic Game Functions: @ref game.h
- Extension Game Functions: @ref game_ext.h
- Auxiliary Game Functions: @ref game_aux.h
- Auxiliary Game Functions: @ref game_aux2.h


It manipulates the *opaque* @ref game structure by means of pointers or const pointers (@ref cgame).

- https://en.wikipedia.org/wiki/Opaque_data_type

About Const Pointer:

- See also: http://www.geeksforgeeks.org/const-qualifier-in-c/
- See also this more technical discussion: http://stackoverflow.com/questions/8504411/typedef-pointer-const-weirdness

## Usage
To compile and run the project, we recommend using a dedicated build directory to keep the source tree clean.

# Compilation
From the project root, run the following commands:

$ mkdir build && cd build
$ cmake ..
$ make

# Running Tests
You can execute all tests or filter them by author using the following commands:

To run all tests:
$ make test
$ ctest

To run tests for a specific author: Use the -R flag followed by the author's username:
$ ctest -R <username>   # Example: ctest -R student2

## SDL2 : Artistic Direction & Graphic Credits
The visual identity of this project was entirely designed to provide an immersive experience based on a seasonal system (Spring, Autumn, Winter).

Original Creation (Handmade)
All graphic assets were custom-made by **Student 2**, including a complete set for each climate:

# Environmental Elements:

Dirt Paths: Straight lines, turns, intersections, and borders.
Aquatic Zones: Central water, borders, and corners.
Decoration: Wooden posts, tall grass, and background trees.

# Gameplay Elements:

Grids: Board backgrounds adapted to the color palette of each season.
Interactive Sprites: Tents (Red/Blue/Purple depending on the theme), Gameplay Trees, and "Big Trees".

Inspiration & Tribute
The artistic direction pays tribute to the iconic style of Ken Sugimori (historical Art Director of Pokémon) and Hironobu Yoshida.
The overall aesthetics are particularly inspired by Pokémon FireRed / LeafGreen (specifically Four Island), used here for contrast management and tile readability, while integrating brand-new winter palettes.

# Technical Specifications
Adaptation: All sprites have been edited and scaled for fluid integration under SDL2.
Modularity: The graphics engine automatically switches between asset sets (Spring, Autumn, Winter) based on the dimensions of the loaded game:

Spring: Perfectly square grid (number of columns equals number of rows).
Autumn: Landscape format (number of columns is strictly greater than number of rows).
Winter: Portrait format (number of columns is strictly less than number of rows).
## Compilation et lancement

### Pré-requis

- CMake 3.5 ou plus récent
- Un compilateur C99
- SDL2, SDL2_image, SDL2_ttf et SDL2_gfx

### Version texte et tests

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
ctest --output-on-failure
./game_text
```

### Version graphique SDL2

Depuis le répertoire `build/` :

```bash
./game_sdl
```
