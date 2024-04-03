#ifndef MAIN_H
#define MAIN_H

#include "gba.h"

// TODO: Create any necessary structs

/*
* For example, for a Snake game, one could be:
*
* struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* };
*
* Example of a struct to hold state machine data:
*
* struct state {
*   int currentState;
*   int nextState;
* };
*
*/

/**
* Cat Owner struct, represents the position of the player
*/
typedef struct player {
    int oldRow;
    int oldCol;
    int newRow;
    int newCol;
    int width;
    int height;
    const u16 *image;
} Player;

//typedef struct winOrLose {
//    int timer;
//    int startTime;
//} GameState;

int isColliding(int row, int height, int col, int width);
int collideWin(int row, int height, int col, int width);
//int timeElapsed(int time);


#endif
