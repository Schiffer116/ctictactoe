#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


void output_board(char** board, int size) {
    printf("\033c");
    printf("y\\x");
    for (int i = 0; i < size; i++) {
        printf("   %i", i);
    }
    printf("\n");

    for (int i = 0; i < size; i++) {
        printf("    ");
        for (int j = 0; j < size; j++) {
            printf("+ - ");
        }

        printf("+\n %i  ", i);

        for (int j = 0; j < size; j++) {
            printf("| %c ", (board[i][j] ? board[i][j] : ' '));
        }
        printf("|\n");
    }

    printf("    ");
    for (int i = 0; i < size; i++) {
        printf("+ - ");
    }
    printf("+\n");
}


int* place(char** board, int size, char symbol) {
    int x, y;

    bool valid_coord = false;
    while (!valid_coord) {
        printf("Enter y and x: ");
        scanf("%i %i", &y, &x);

        bool out_of_board = y < 0 || size <= y
                         || x < 0 || size <= x;
        bool taken = board[y][x];
        if (out_of_board) {
            printf("Invalid move! Coord is outside the board\n");
        } else if (taken) {
            printf("Invalid move! Spot is taken\n");
        } else {
            valid_coord = true;
        }
    }

    board[y][x] = symbol;
    int* coord = malloc(2 * sizeof(int));
    coord[0] = y;
    coord[1] = x;
    return coord;
}


int max(int a, int b) {
    return a > b ? a : b;
}

int check_line(char** board, int size, int y, int x, char symbol) {
    // horizontal
    int horizontal = 1;
    for (int i = x + 1; 0 <= i && i < size; i++) {
        if (board[y][i] == symbol) 
            horizontal++;
        else
            break;
    }
    for (int i = x - 1; 0 <= i && i < size; i--) {
        if (board[y][i] == symbol) 
            horizontal++;
        else
            break;
    }
    
    // vertical
    int vertical = 1;
    for (int i = y + 1; 0 <= i && i < size; i++) {
        if (board[i][x] == symbol)
            vertical++;
        else
            break;
    }
    for (int i = y - 1; 0 <= i && i < size; i--) {
        if (board[i][x] == symbol)
            vertical++;
        else
            break;
    }

    // forward diagnal
    int diag = 1;
    int pos_x = x + 1,
        pos_y = y + 1;
    while ( 0 <= pos_x && pos_x < size
         && 0 <= pos_y && pos_y < size
          ) {
        if (board[pos_y][pos_x] == symbol)
            diag++;
        else
            break;
        pos_x++;
        pos_y++;
    }
    pos_x = x - 1,
    pos_y = y - 1;
    while ( 0 <= pos_x && pos_x < size
         && 0 <= pos_y && pos_y < size
          ) {
        if (board[pos_y][pos_x] == symbol)
            diag++;
        else
            break;
        pos_x--;
        pos_y--;
    }

    // backward diagnal
    int back_diag = 1;
    pos_x = x - 1,
    pos_y = y + 1;
    while ( 0 <= pos_x && pos_x < size
         && 0 <= pos_y && pos_y < size
          ) {
        if (board[pos_y][pos_x] == symbol)
            back_diag++;
        else
            break;
        pos_x--;
        pos_y++;
    }

    pos_x = x + 1,
    pos_y = y - 1;
    while ( 0 <= pos_x && pos_x < size
         && 0 <= pos_y && pos_y < size
          ) {
        if (board[pos_y][pos_x] == symbol)
            back_diag++;
        else
            break;
        pos_x++;
        pos_y--;
    }

    return max(horizontal, max(vertical, max(diag, back_diag)));
}


void print_usage() {
    printf("Usage: ctictactoe [OPTION]...\n");
    printf("Command line tic tac toe.\n\n");
    printf("-p s[players' symbols]\tnumber of players (D=2).\n");
    printf("-s [1-]\t\t\tsize of board (D=3).\n");
    printf("-w [1-SIZE]\t\tnumber of symbols in a line before winning (D=3).\n");
}

int main(int argc, char* argv[]) {
    int size = 3,
        win_condition = 3;
    char* players = "xo";
    for (int i = 1; i < argc; i++) {
        if (argv[i + 1]) {
            if (!strcmp(argv[i], "-p")) {
                players = argv[i + 1];
            } else if (!strcmp(argv[i], "-s")) {
                size = atoi(argv[i + 1]);
            } else if (!strcmp(argv[i], "-w")) {
                win_condition = atoi(argv[i + 1]);
            }
        } else if (!strcmp(argv[i], "-h")) {
            print_usage();
            return 0;
        }
    }


    char** board = calloc(size, 8);
    for (int i = 0; i < size; i++) {
        board[i] = calloc(size, 1);
    }
    output_board(board, size);

    int move_rem = size * size;
    while (0 < move_rem) {
        for (int i = 0; i < strlen(players) && 0 < move_rem; i++, move_rem--) {
            printf("%c's turn:\n", players[i]);
            int* prev_move = place(board, size, players[i]);

            output_board(board, size);
            int max_line = check_line(board, size, prev_move[0], prev_move[1], players[i]);
            if (max_line == win_condition) {
                output_board(board, size);
                printf("%c WON!\n", players[i]);
                move_rem = 0;
            }
            free(prev_move);
        }
    }
    return 0;
}
