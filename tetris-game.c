#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>

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

void processInput(int* posX, int* posY) {
    if (kbhit()) {
        char input = getch();

        switch (input) {
            // case 72: // Up arrow key
            //     // rotate
            //     break;
            case 80: // Down arrow key
                if (*posY < 19) {
                    (*posY)++;
                }
                break;
            case 75: // Left arrow key
                if (*posX > 0) {
                    (*posX)--;
                }
                break;
            case 77: // Right arrow key
                if (*posX < 9) {
                    (*posX)++;
            }
                break;
            case 'q':
                exit(0);
                break;
            default:
                break;
        }
    }
}


void generateNewBlock(int board[20][10], int *posX, int *posY) {
    // 1x1 block
    board[*posY][*posX] = 1;
}

int checkCollision(int board[20][10], int *posX, int *posY) {
    if (*posY < 19 && board[*posY + 1][*posX] == 0) {
        return 0; 
    }
    return 1; 
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

void gameLoop(int board[20][10], int *posX, int *posY, int fixed[20][10]) {
    if (*posY >= 19 || checkCollision(board, posX, posY)) {
        fixed[*posY][*posX] = 1;
        board[*posY][*posX] = 1;
        *posX = 4;
        *posY = 0;
    }
    else (*posY)++;
    checkLine(fixed, board);
    generateNewBlock(board, posX, posY);
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
    while (1) {
        processInput(&posX, &posY); 
        gameLoop(board, &posX, &posY, fixed); 
    }
    return 0;
}
