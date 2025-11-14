#pragma once

/* Put other #includes here,
   your struct board, helper function
   prototypes etc.
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#define ROWS 4
#define COLS 5
#define MAX_BOARDS 100000

typedef struct board{
    int graid[ROWS][COLS];
}board;

#include "nm.h"

/* Helper function prototypes */
bool is_valid_move(board *b, pair z);
bool is_touching_or_aligned(board *b, pair z);
bool boards_equal(board *a, board *b);
bool is_final(board *b);
void print_board(board *b);


