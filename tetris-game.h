#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H

// Function to draw the playing board
void drawPlayingBoard(int board[20][10]);

// Function to process user input for moving the block
void processInput(int* posX, int* posY);

// Function to generate a new block at a specific position
void generateNewBlock(int board[20][10], int *posX, int *posY);

// Main game loop
void gameLoop(int board[20][10], int *posX, int *posY, int fixed[20][10]);

#endif
