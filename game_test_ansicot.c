#define __USE_GNU
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"

/* ********** TEST ********** */

bool test_dummy() { return true; }

bool test_new() {
  square *squares = malloc((DEFAULT_SIZE * DEFAULT_SIZE) * sizeof(square));
  if (squares == NULL) {
    exit(EXIT_FAILURE);
  }
  uint nb_tents_row[DEFAULT_SIZE];
  uint nb_tents_col[DEFAULT_SIZE];
  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    nb_tents_row[i] = 0;
    nb_tents_col[i] = 0;
  }
  for (uint i = 0; i < DEFAULT_SIZE * DEFAULT_SIZE; i++) {
    squares[i] = EMPTY;
  }
  game g = game_new(squares, nb_tents_row, nb_tents_col);
  if (g == NULL) {
    free(squares);
    return false;
  }
  game g2 = game_new_empty();
  if (g2 == NULL) {
    game_delete(g);
    free(squares);
    return false;
  }
  if (!game_equal(g, g2)) {
    game_delete(g);
    game_delete(g2);
    free(squares);
    return false;
  }
  game_print(g);
  game_delete(g);
  game_delete(g2);
  free(squares);
  return true;
}

bool test_new_empty() {
  game g = game_new_empty();
  if (g == NULL) {
    return false;
  }
  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    for (uint j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(g, i, j) != EMPTY) {
        game_delete(g);
        return false;
      }
    }
  }
  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    if (game_get_expected_nb_tents_row(g, i) != 0) {
      game_delete(g);
      return false;
    }
  }
  for (uint j = 0; j < DEFAULT_SIZE; j++) {
    if (game_get_expected_nb_tents_col(g, j) != 0) {
      game_delete(g);
      return false;
    }
  }
  game_print(g);
  game_delete(g);
  return true;
}

bool test_copy() {
  game g1 = game_default();
  if (g1 == NULL) {
    return false;
  }
  game g2 = game_copy(g1);
  if (g2 == NULL) {
    game_delete(g1);
    return false;
  }
  if (!game_equal(g1, g2)) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  game_set_square(g2, 1, 0, TENT);
  if (game_get_square(g1, 1, 0) == TENT) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  game_print(g1);
  game_print(g2);
  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_equal() {
  game g1 = game_default();
  if (g1 == NULL) {
    return false;
  }
  game g2 = game_copy(g1);
  if (g2 == NULL) {
    game_delete(g1);
    return false;
  }
  if (!game_equal(g1, g2)) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  game_set_square(g2, 1, 0, TENT);
  if (game_equal(g1, g2)) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  game_set_square(g2, 1, 0, EMPTY);
  game_print(g1);
  game_print(g2);
  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_set_square() {
  game g1 = game_default();
  if (g1 == NULL) {
    return false;
  }
  game g2 = game_copy(g1);
  if (g2 == NULL) {
    game_delete(g1);
    return false;
  }
  if (game_get_square(g1, 1, 0) != EMPTY) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  game_set_square(g1, 1, 0, TENT);
  if (game_get_square(g1, 1, 0) != TENT) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  game_set_square(g1, 1, 0, EMPTY);
  if (!game_equal(g1, g2)) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  game_print(g1);
  game_print(g2);
  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_get_square() {
  game g1 = game_default();
  if (g1 == NULL) {
    return false;
  }
  if (game_get_square(g1, 1, 0) != EMPTY) {
    game_delete(g1);
    return false;
  }
  game_set_square(g1, 1, 0, TENT);
  if (game_get_square(g1, 1, 0) != TENT) {
    game_delete(g1);
    return false;
  }
  game_set_square(g1, 1, 0, EMPTY);
  game_print(g1);
  game_delete(g1);
  return true;
}

bool test_set_expected_nb_tents_row() {
  game g1 = game_default();
  if (g1 == NULL) {
    return false;
  }
  game g2 = game_copy(g1);
  if (g2 == NULL) {
    game_delete(g1);
    return false;
  }
  uint row = 1;
  uint original = game_get_expected_nb_tents_row(g1, row);
  game_set_expected_nb_tents_row(g1, row, original + 1);
  if (game_get_expected_nb_tents_row(g1, row) != original + 1) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    if ((i != row) && (game_get_expected_nb_tents_row(g1, i) != game_get_expected_nb_tents_row(g2, i))) {
      game_delete(g1);
      game_delete(g2);
      return false;
    }
  }
  game_set_expected_nb_tents_row(g1, row, original);
  game_print(g1);
  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_set_expected_nb_tents_col() {
  game g1 = game_default();
  if (g1 == NULL) {
    return false;
  }
  game g2 = game_copy(g1);
  if (g2 == NULL) {
    game_delete(g1);
    return false;
  }
  uint col = 1;
  uint original = game_get_expected_nb_tents_col(g1, col);
  game_set_expected_nb_tents_col(g1, col, original + 1);
  if (game_get_expected_nb_tents_col(g1, col) != original + 1) {
    game_delete(g1);
    game_delete(g2);
    return false;
  }
  for (uint j = 0; j < DEFAULT_SIZE; j++) {
    if ((j != col) && (game_get_expected_nb_tents_col(g1, j) != game_get_expected_nb_tents_col(g2, j))) {
      game_delete(g1);
      game_delete(g2);
      return false;
    }
  }
  game_set_expected_nb_tents_col(g1, col, original);
  game_print(g1);
  game_delete(g1);
  game_delete(g2);
  return true;
}

bool test_new_ext() {
  uint rows = 4;
  uint cols = 4;
  bool wrapping = false;

  square squares[16] = {
    EMPTY, TREE, TENT, EMPTY,
    TREE, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, TREE, EMPTY,
    TENT, EMPTY, EMPTY, EMPTY
  };

  uint tents_row[4] = {1, 0, 0, 1};
  uint tents_col[4] = {1, 0, 1, 0};

  game g = game_new_ext(rows, cols, squares, tents_row, tents_col, wrapping);
  if (g == NULL) return false;

  if (game_nb_rows(g) != rows || game_nb_cols(g) != cols) {
    game_delete(g);
    return false;
  }

  for (uint i = 0; i < rows; i++) {
    for (uint j = 0; j < cols; j++) {
      if (game_get_square(g, i, j) != squares[i * cols + j]) {
        game_delete(g);
        return false;
      }
    }
  }

  for (uint i = 0; i < rows; i++) {
    if (game_get_expected_nb_tents_row(g, i) != tents_row[i]) {
      game_delete(g);
      return false;
    }
  }
  for (uint j = 0; j < cols; j++) {
    if (game_get_expected_nb_tents_col(g, j) != tents_col[j]) {
      game_delete(g);
      return false;
    }
  }

  if (game_is_wrapping(g) != wrapping) {
    game_delete(g);
    return false;
  }

  game_print(g);
  game_delete(g);
  return true;
}


bool test_new_empty_ext() {
  uint rows = 5;
  uint cols = 7;
  bool wrapping = true;

  game g = game_new_empty_ext(rows, cols, wrapping);
  if (g == NULL) return false;

  if (game_nb_rows(g) != rows || game_nb_cols(g) != cols) {
    game_delete(g);
    return false;
  }
  for (uint i = 0; i < rows; i++) {
    for (uint j = 0; j < cols; j++) {
      if (game_get_square(g, i, j) != EMPTY) {
        game_delete(g);
        return false;
      }
    }
  }
  for (uint i = 0; i < rows; i++) {
    if (game_get_expected_nb_tents_row(g, i) != 0) {
      game_delete(g);
      return false;
    }
  }
  for (uint j = 0; j < cols; j++) {
    if (game_get_expected_nb_tents_col(g, j) != 0) {
      game_delete(g);
      return false;
    }
  }
  if (game_is_wrapping(g) != wrapping) {
    game_delete(g);
    return false;
  }
  game_print(g);
  game_delete(g);
  return true;
}


/* ********** USAGE ********** */

void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

/* ********** MAIN ROUTINE ********** */

int main(int argc, char *argv[]) {
  if (argc == 1) usage(argc, argv);
  // start test
  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
  bool ok = false;
  if (strcmp("dummy", argv[1]) == 0)
    ok = test_dummy();
  else if (strcmp("new", argv[1]) == 0)
    ok = test_new();
  else if (strcmp("new_empty", argv[1]) == 0)
    ok = test_new_empty();
  else if (strcmp("copy", argv[1]) == 0)
    ok = test_copy();
  else if (strcmp("equal", argv[1]) == 0)
    ok = test_equal();
  else if (strcmp("set_square", argv[1]) == 0)
    ok = test_set_square();
  else if (strcmp("get_square", argv[1]) == 0)
    ok = test_get_square();
  else if (strcmp("set_expected_nb_tents_row", argv[1]) == 0)
    ok = test_set_expected_nb_tents_row();
  else if (strcmp("set_expected_nb_tents_col", argv[1]) == 0)
    ok = test_set_expected_nb_tents_col();
  else if (strcmp("new_ext", argv[1]) == 0)
    ok = test_new_ext();
  else if (strcmp("new_empty_ext", argv[1]) == 0)
    ok = test_new_empty_ext();

  else {
    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    exit(EXIT_FAILURE);
  }
  // print test result
  if (ok) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}
