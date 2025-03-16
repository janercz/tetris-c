#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

#define RESET "\033[0m"
#define WHTB "\e[47m"
#define BLUE "\033[34m"
#define WHTHB "\e[0;107m"

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
    {0, 1, 0, 0},
    {0, 1, 0, 0},
    {1, 1, 0, 0},
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
                printf(WHTHB "  " RESET);
            } else {
                printf(WHTB "# " RESET);
            }
        }
        printf("\n");
    }
}

void rotateBlock(Block *block) {
    int temp[4][4];

    for (int y = 0; y < block -> size; y++) {
        for (int x = 0; x < block -> size; x++) {
            if (block -> shape[y][x] == 1) {
                temp[x][block -> size - 1 - y] = 1;
            }
        }
    }

    for (int y = 0; y < block -> size; y++) {
        for (int x = 0; x < block -> size; x++) {
            block -> shape[y][x] = temp[y][x];
            
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

void processInput(int fixed[20][10], int* posX, int* posY, Block *block) {
    int ok = 1;

    if (GetAsyncKeyState('A') & 0x8000) {
        for (int y = 0; y < block -> size; y++) {
            for (int x = 0; x < block -> size; x++) {
                if (block -> shape[y][x] == 1) {
                    int boardX = *posX + x;
                    int boardY = *posY + y;
                    if (boardX - 1 < 0) {
                        ok = 0;
                    }
                    if (fixed[boardY][boardX - 1] == 1) {
                        ok = 0;
                    }
                }
            }
        }
        if (ok) (*posX)--;
    }

    if (GetAsyncKeyState('D') & 0x8000) {
        ok = 1;
        for (int y = 0; y < block -> size; y++) {
            for (int x = 0; x < block -> size; x++) {
                if (block -> shape[y][x] == 1) {
                    int boardX = *posX + x;
                    int boardY = *posY + y;
                    if (boardX + 1 > 9) {
                        ok = 0;
                    }
                    if (fixed[boardY][boardX + 1] == 1) {
                        ok = 0;
                    }
                }
            }
        }
        if (ok) (*posX)++;
    }

    if (GetAsyncKeyState('W') & 0x8000) {
        Block temp = *block;
        rotateBlock(&temp);

        ok = 1;
        for (int y = 0; y < temp.size; y++) {
            for (int x = 0; x < temp.size; x++) {
                if (temp.shape[y][x] == 1) {
                    int boardX = *posX + x;
                    int boardY = *posY + y;
                    if (boardX < 0 || boardX >= 10 || boardY >= 20 || fixed[boardY][boardX] == 1) {
                        ok = 0;
                    }
                }
            }
        }
        if (ok) *block = temp;
    }

    if (GetAsyncKeyState('S') & 0x8000) {
        if (!checkCollision(fixed, posX, posY, block)) {
            (*posY)++;
        }
    }

    if (GetAsyncKeyState('Q') & 0x8000) {
        exit(0);
    }
}

int generateNewBlock(int board[20][10], int *posX, int *posY, Block block) {
    for (int y = 0; y < block.size; y++) {
        for (int x = 0; x < block.size; x++) {
            if (block.shape[y][x] == 1) {
                int boardY = *posY + y;
                int boardX = *posX + x;

                if (boardY < 20 && boardX < 10 && boardY >= 0 && boardX >= 0) {
                    if (board[boardY][boardX] == 1) {
                        return 1;
                    }
                    else {
                        board[boardY][boardX] = 1;
                    }
                }
            }
        }
    }
    return 0;
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
    while (1) {
        processInput(fixed, posX, posY, block); 
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
        if (generateNewBlock(board, posX, posY, *block)) {
            break;
        }
        drawPlayingBoard(board);
    
        for (int y = 0; y < 20; y++) {
            for (int x = 0; x < 10; x++) {
                if (board[y][x] == 1 && fixed[y][x] != 1) {
                    board[y][x] = 0;
                }
            }
        }
        Sleep(120);
    }
}

int main() {
    srand((unsigned int)time(NULL));

    printf("PRESS ANY KEY TO PLAY...\n");
    while (1) {
        _getch();
        system("cls"); 

        int board[20][10] = {0};
        int fixed[20][10] = {0};
        int posX = 4, posY = 0;
        Block block = blocks[rand() % 7];

        gameLoop(board, &posX, &posY, fixed, &block);
        system("cls"); 
        Sleep(1000);
        printf("YOU LOST\n");
        printf("PRESS ANY KEY TO PLAY AGAIN OR 'Q' TO QUIT...\n");
        _getch();
    }
    
}
