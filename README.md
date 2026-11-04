# README


## Authors :
- Sicot Anthony
- Guillaumin Samuel
- Massié Anthony


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
All graphic assets were custom-made by **Anthony Sicot**, including a complete set for each climate:

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
Autumn: Landscape format (number of columns is strictly greater than the number of rows).
Winter: Portrait format (number of columns is strictly less than the number of rows).

## Random Grid Generation

The `game_random` executable generates a random grid. Its syntax is:

```bash
./game_random <nb_rows> <nb_cols> <wrapping> <nb_trees> <nb_bigtrees> <solution> [filename]
```

The parameters are:

- `nb_rows`: number of rows.
- `nb_cols`: number of columns.
- `wrapping`: toric wrapping disabled (`0`) or enabled (`1`).
- `nb_trees`: number of regular trees.
- `nb_bigtrees`: number of BIGTREE pieces.
- `solution`: generate an unsolved grid (`0`) or include its solution (`1`).
- `filename`: optional output file.

The tree counts below are examples. Generation depends on whether a valid grid can be constructed with the requested dimensions and pieces.

For the three SDL2 themes, use for example:

**Spring (square grid):**
```bash
./game_random 8 8 0 12 0 1 spring.txt
./game_sdl spring.txt
```

**Autumn (horizontal grid):**
```bash
./game_random 8 10 0 12 0 1 autumn.txt
./game_sdl autumn.txt
```

**Winter (vertical grid):**
```bash
./game_random 10 8 0 12 0 1 winter.txt
./game_sdl winter.txt
```

The SDL2 theme is selected automatically from the grid dimensions; it is not a manually selectable mode. Square grids use Spring, wider grids use Autumn, and taller grids use Winter.

`game_text` can also load a saved grid file directly:

```bash
./game_text winter.txt
```

## Automatic Solver

The solver is implemented in `game_tools.c` and exposed through `game_solve` and `game_nb_solutions` in `game_tools.h`. The `game_solve` function updates a game with the first solution found and returns `true` when a solution exists; if no solution exists, the game is left unchanged. `game_nb_solutions` counts all solutions.

The implementation uses recursive depth-first backtracking. It visits the grid cell by cell, skips TREE and BIGTREE cells, and tries GRASS and TENT on every other cell. The `game_winable` checks prune branches that already violate row/column counts, tent adjacency, or the remaining possibilities for TREE and BIGTREE constraints. `game_solve` stops after the first solution, whereas `game_nb_solutions` explores all solutions.

The command-line executable accepts a saved game as input. It prints a solution with `-s` and can optionally save it; `-c` counts solutions and prints the count unless an output file is supplied:

```bash
./game_solve -s <input>
./game_solve -s <input> <output>
./game_solve -c <input>
./game_solve -c <input> <output>
```

In SDL2, press the `x` key to restart the current puzzle and display the first solution found. In the web interface, click the Solve button; it calls the same solver through the WebAssembly wrapper.

## Web Version

The `web/` directory contains an HTML/CSS/JavaScript interface backed by the C game library compiled to WebAssembly with Emscripten. It has no JavaScript package-manager dependencies. Building it requires the Emscripten tools `emcc` and `emar`; a local HTTP server is recommended so that the browser can load the generated WebAssembly files.

From the project root:

```bash
cd web
make
python3 -m http.server 8000
```

Then open `http://localhost:8000/game.html` in a browser. The page provides mouse interaction, Undo, Redo, Restart, Solve, and Random actions. Stop the server with `Ctrl+C` when finished.

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

## Avertissement

Ce dépôt contient un projet étudiant réalisé à des fins pédagogiques et académiques. Il peut contenir des limitations, des choix simplifiés ou des failles de sécurité ; il n est pas destiné à un usage professionnel, à la production ou au traitement de données réelles. Utilisez-le uniquement dans un environnement de test et adaptez-le avant toute réutilisation.
