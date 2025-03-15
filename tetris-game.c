#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>

typedef struct {
    int shape[4][4];
    int size;
} Block;

Block blocks[7] = {{{
    {0, 0, 0, 0},
    {1, 1, 1, 1},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
}, 4}, {{
    {0, 1, 0, 0},
    {1, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
}, 3}, {{
    {1, 0, 0, 0},
    {1, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 0, 0, 0}
}, 3}, {{
    {0, 0, 1, 0},
    {0, 0, 1, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0}
}, 3}, {{
    {1, 1, 0, 0},
    {1, 1, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
}, 2}, {{
    {0, 1, 1, 0},
    {1, 1, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
}, 3}, {{
    {1, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
}, 3}};


void drawPlayingBoard(int board[20][10]) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord;

    for (int y = 0; y < 20; y++) {
        coord.X = 0;
        coord.Y = y;

        SetConsoleCursorPosition(hConsole, coord);

        for (int x = 0; x < 10; x++) {
            if (board[y][x] == 0) {
                printf(".");
            } else {
                printf("#");
            }
        }
        printf("\n");
    }
}

void processInput(int* posX, int* posY, Block block) {
    int ok = 1;
    if (kbhit()) {
        char input = getch();

        switch (input) {
            case 75: // Left arrow key
                for (int y = 0; y < block.size; y++) {
                    for (int x = 0; x < block.size; x++) {
                        if (block.shape[y][x] == 1) {
                        int boardX = *posX + x;
                        if (boardX - 1 < 0) {
                            ok = 0;
                        }
                        }
                    }
                }
                if (ok) {
                    (*posX)--;
                }
                break;
            case 77: // Right arrow key
                for (int y = 0; y < block.size; y++) {
                    for (int x = 0; x < block.size; x++) {
                        if (block.shape[y][x] == 1) {
                        int boardX = *posX + x;
                        if (boardX + 1 > 9) {
                            ok = 0;
                        }
                        }
                    }
                }
                if (ok) {
                    (*posX)++;
                }
                break;
            case 'q':
                exit(0);
                break;
            // case 72: // Up arrow key
            //     // rotate
            //     break;
            // case 80: // Down arrow key
            //     if (*posY < 19) {
            //         (*posY)++;
            //     }
            //     break;
            default:
                break;
        }
    }
}

void generateNewBlock(int board[20][10], int *posX, int *posY, Block block) {
    for (int y = 0; y < block.size; y++) {
        for (int x = 0; x < block.size; x++) {
            if (block.shape[y][x] == 1) {
                int boardY = *posY + y;
                int boardX = *posX + x;

                if (boardY < 20 && boardX < 10 && boardY >= 0 && boardX >= 0) {
                    board[boardY][boardX] = 1;
                }
            }
        }
    }
}

int checkCollision(int board[20][10], int *posX, int *posY, Block *block) {
    int ok = 1;
    for (int y = 0; y < block -> size; y++) {
        for (int x = 0; x < block -> size; x++) {
            if (block -> shape[y][x] == 1) {
                int boardY = *posY + y;
                int boardX = *posX + x;

                if (boardY >= 19 || board[boardY + 1][boardX] != 0) {
                    ok = 0;
                }
            }
        }
    }
    if (ok) return 0; 
    else return 1; 
}

void checkLine(int fixed[20][10], int board[20][10]) {
    for (int y = 0; y < 20; y++) {
        int check = 0;

        for (int x = 0; x < 10; x++) {
            if (fixed[y][x] == 1) check++;
        }

        if (check == 10) {
            for (int x = 0; x < 10; x++) {
                fixed[y][x] = 0;
                board[y][x] = 0;
            }
            for (int y2 = y; y2 > 0; y2--) {
                for (int x2 = 0; x2 < 10; x2++) {
                    fixed[y2][x2] = fixed[y2 - 1][x2];
                    board[y2][x2] = board[y2 - 1][x2]; 
                }
            }

            for (int x = 0; x < 10; x++) {
                fixed[0][x] = 0;
                board[0][x] = 0;
            }
        }
    }
}

void gameLoop(int board[20][10], int *posX, int *posY, int fixed[20][10], Block *block) {
    if (*posY >= 19 || checkCollision(board, posX, posY, block)) {
        for (int y = 0; y < block -> size; y++) {
            for (int x = 0; x < block -> size; x++) {
                if (block -> shape[y][x] == 1) {
                    int boardY = *posY + y;
                    int boardX = *posX + x;
    
                    if (boardY < 20 && boardX < 10 && boardY >= 0 && boardX >= 0) {
                        fixed[boardY][boardX] = 1;
                        board[boardY][boardX] = 1;
                    }
                }
            }
        }
        *block = blocks[rand() % 7];
        *posX = 4;
        *posY = 0;
    }
    else (*posY)++;
    checkLine(fixed, board);
    generateNewBlock(board, posX, posY, *block);
    drawPlayingBoard(board);

    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 10; x++) {
            if (board[y][x] == 1 && fixed[y][x] != 1) {
                board[y][x] = 0;
            }
        }
    }

    Sleep(100);
}

int main() {
    int board[20][10] = {0};
    int fixed[20][10] = {0};
    int posX = 4, posY = 0;
    Block block = blocks[rand() % 7];

    while (1) {
        processInput(&posX, &posY, block); 
        gameLoop(board, &posX, &posY, fixed, &block); 
    }
}
