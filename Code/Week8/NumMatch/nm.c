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
    printf("All tests passed!\n");
}

// /* Check if two cells are adjacent or aligned with no numbers between them */
bool is_touching_or_aligned(board *b, pair z) {
    int x1=z.x1, y1=z.y1, x2=z.x2, y2=z.y2;

    /* 8-neighbour adjacency */
    if (abs(x1-x2)<=1 && abs(y1-y2)<=1)
        return true;

    /* same row → check between */
    if (x1==x2) {
        int a = y1<y2 ? y1 : y2;
        int b2 = y1>y2 ? y1 : y2;
        for (int y=a+1; y<b2; y++)
            if (b->graid[x1][y]!=0) return false;
        return true;
    }

    /* same column → check between */
    if (y1==y2) {
        int a = x1<x2 ? x1 : x2;
        int b2 = x1>x2 ? x1 : x2;
        for (int x=a+1; x<b2; x++)
            if (b->graid[x][y1]!=0) return false;
        return true;
    }

    return false;
}
bool is_valid_move(board *b, pair z) {
    /* bounds */
    if(z.x1<0||z.x1>=ROWS||z.y1<0||z.y1>=COLS||
       z.x2<0||z.x2>=ROWS||z.y2<0||z.y2>=COLS)
        return false;

    if(z.x1==z.x2 && z.y1==z.y2)
        return false;

    int a=b->graid[z.x1][z.y1];
    int c=b->graid[z.x2][z.y2];

    if(a==0||c==0) return false;

    bool same_row = (z.x1==z.x2);
    bool same_col = (z.y1==z.y2);
    bool adjacent = (abs(z.x1-z.x2)<=1 && abs(z.y1-z.y2)<=1);

    /* ----------------------------
       Rule A: line-match always allowed
       ---------------------------- */
    if((same_row || same_col) && is_touching_or_aligned(b,z)) {
        return true;
    }

    /* ----------------------------
       Rule B: sum=10 OR equal AND adjacent
       ---------------------------- */
    if ((a==c || a+c==10) && adjacent) {
        return true;
    }

    return false;
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

