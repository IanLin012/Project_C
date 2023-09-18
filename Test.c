#include <stdio.h>
#include <stdbool.h>

#define EMPTY ' '
#define ROOK 'R'
#define KNIGHT 'N'
#define BISHOP 'B'
#define QUEEN 'Q'
#define KING 'K'
#define PAWN 'P'

char board[8][8]; // 8x8 chess board

// Function to initialize the chessboard with the starting position
void init_board() {
    // Initial chess position
    char initial_row[] = {ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK};
    for (int i = 0; i < 8; i++) {
        board[0][i] = initial_row[i];
        board[1][i] = PAWN;
        board[6][i] = PAWN;
        board[7][i] = initial_row[i];
    }

    // Fill the empty squares
    for (int i = 2; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = EMPTY;
        }
    }
}

// Function to display the chessboard
void display_board() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

int main() {
    // Initialize the chessboard
    init_board();

    // Display the initial position
    display_board();

    return 0;
}
