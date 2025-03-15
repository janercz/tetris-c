#ifndef TETRIS_H
#define TETRIS_H

#include <windows.h>

#define RESET   "\033[0m"
#define BLUE    "\033[34m"

typedef struct {
    int shape[4][4];
    int size;
} Block;

extern Block blocks[7];

void drawPlayingBoard(int board[20][10]);
void rotateBlock(Block *block);
void processInput(int* posX, int* posY, Block *block);
void generateNewBlock(int board[20][10], int *posX, int *posY, Block block);
int checkCollision(int board[20][10], int *posX, int *posY, Block *block);
void checkLine(int fixed[20][10], int board[20][10]);
void gameLoop(int board[20][10], int *posX, int *posY, int fixed[20][10], Block *block);

#endif
