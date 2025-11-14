#include "nm.h"

bool solve(int seed)
{
    board boards[MAX_BOARDS];
    int board_count = 1;
    int index = 0;

    /* Initialize the first board */
    boards[0] = randfill(seed);

    while (index < board_count) {
        board current = boards[index];

        /* Check if current board is final */
        if (is_final(&current)) {
            return true;    
        }

        /* Generate all possible moves */
        for (int x1 = 0; x1 < ROWS; x1++) {
            for (int y1 = 0; y1 < COLS; y1++) {
                for (int x2 = 0; x2 < ROWS; x2++) {
                    for (int y2 = 0; y2 < COLS; y2++) {
                        pair move = {x1, y1, x2, y2};
                        board new_board = current;

                        if (take(&new_board, move)) {
                            /* Check if board is already in list */
                            bool found = false;
                            for (int k = 0; k < board_count; k++) {
                                if (boards_equal(&boards[k], &new_board)) {
                                    found = true;
                                    break;
                                }
                            }

                            /* If not found and within limit, add to list */
                            if (!found && board_count < MAX_BOARDS) {
                                boards[board_count++] = new_board;
                            }
                        }
                    }
                }
            }
        }
        index++;
    }
    return false;
}

bool take(board* p, pair z)
{
    /* Check if the move is valid */
    if (!is_valid_move(p, z)) {
        return false;   
    }

    /* Remove the numbers from the board */
    p->graid[z.x1][z.y1] = 0;
    p->graid[z.x2][z.y2] = 0;

    return true;
}


board randfill(int n)
{
    board b;
    memset(&b, 0, sizeof(board));
    srand(n);
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            b.graid[i][j] = rand() % 9 + 1;
        }
    }
    return b;
}

// Helper function implementations

void test(void)
{
    board b1 = randfill(42);
    board b2 = randfill(42);
    assert(boards_equal(&b1, &b2));

    board test_board = {{
        {1, 2, 3, 4, 5},
        {6, 7, 8, 9, 1},
        {2, 3, 4, 5, 6},
        {7, 8, 9, 1, 2}
    }};

    /* Test 1: Invalid match */
    pair move1 = {0, 0, 0, 1};
    assert(!take(&test_board, move1));
    
    /* Test 2: Valid match - sum to 10 and adjacent (2 at (0,1) with 8 at (1,2)) */
    pair move2 = {0, 1, 1, 2};
    assert(take(&test_board, move2));
    assert(test_board.graid[0][1] == 0);
    assert(test_board.graid[1][2] == 0);
    
    /* Test 3: Valid match - sum to 10 and adjacent */
    board test_board2 = {{
        {1, 4, 6, 2, 5},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
    }};
    pair move3 = {0, 1, 0, 2};
    assert(take(&test_board2, move3));
    assert(test_board2.graid[0][1] == 0 && test_board2.graid[0][2] == 0);
    
    /* Test 4: Invalid match - aligned but blocked */
    board test_board3 = {{
        {1, 4, 9, 6, 5},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
    }};
    pair move4 = {0, 1, 0, 3};
    assert(!take(&test_board3, move4));
    
    printf("All tests passed!\n");
}

/* Check if two cells are adjacent or aligned with no numbers between them */
bool is_touching_or_aligned(board *b, pair z) {
    int x1 = z.x1, y1 = z.y1, x2 = z.x2, y2 = z.y2;
    
    /* Check if adjacent (8-connected) */
    if (abs(x1 - x2) <= 1 && abs(y1 - y2) <= 1) {
        return true;
    }
    
    /* Check if on the same row (horizontal alignment) */
    if (x1 == x2) {
        int miny = (y1 < y2) ? y1 : y2;
        int maxy = (y1 > y2) ? y1 : y2;
        for (int j = miny + 1; j < maxy; j++) {
            if (b->graid[x1][j] != 0) {
                return false;
            }
        }
        return true;
    }
    
    /* Check if on the same column (vertical alignment) */
    if (y1 == y2) {
        int minx = (x1 < x2) ? x1 : x2;
        int maxx = (x1 > x2) ? x1 : x2;
        for (int i = minx + 1; i < maxx; i++) {
            if (b->graid[i][y1] != 0) {
                return false;
            }
        }
        return true;
    }
    
    /* Check if on the same diagonal */
    if (abs(x1 - x2) == abs(y1 - y2) && x1 != x2) {
        int dx = (x2 > x1) ? 1 : -1;
        int dy = (y2 > y1) ? 1 : -1;
        int x = x1 + dx, y = y1 + dy;
        
        while (x != x2 || y != y2) {
            if (b->graid[x][y] != 0) {
                return false;
            }
            x += dx;
            y += dy;
        }
        return true;
    }
    
    return false;
}

bool is_valid_move(board *b, pair z) {
    /* Check if the coordinates are within bounds */
    if(z.x1 < 0 || z.x1 >= ROWS || z.y1 < 0 || z.y1 >= COLS ||
       z.x2 < 0 || z.x2 >= ROWS || z.y2 < 0 || z.y2 >= COLS) {
        return false;
    }

    /* Check if it's the same cell */
    if(z.x1 == z.x2 && z.y1 == z.y2) {
        return false;
    }

    /* Check if cells are non-zero */
    int num1 = b->graid[z.x1][z.y1];
    int num2 = b->graid[z.x2][z.y2];
    if(num1 == 0 || num2 == 0) {
        return false;
    }

    /* Check if they are equal or sum to 10 */
    if(num1 != num2 && (num1 + num2) != 10) {
        return false;
    }

    /* Check if they are touching or aligned */
    if (!is_touching_or_aligned(b, z)) {
        return false;
    }

    return true;
}

bool is_final(board *b) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (b->graid[i][j] != 0) {
                return false;
            }
        }
    }
    return true;
}

void print_board(board *b) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%d ", b->graid[i][j]);
        }
        printf("\n");
    }
}

bool boards_equal(board *a, board *b) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (a->graid[i][j] != b->graid[i][j]) {
                return false;
            }
        }
    }
    return true;
}
