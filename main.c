#include "main.h"

#include <stdio.h>
#include <stdlib.h>
//#include <time.h>

#include "gba.h"
#include "images/coverPage.h"
#include "images/maze.h"
#include "images/cat.h"
#include "images/catOwner.h"
#include "images/lose.h"
#include "images/win.h"

/* TODO: */
// Add any additional states you need for your app. You are not requried to use
// these specific provided states.
enum gba_state {
  STARTBG,
  STARTTEXT,
  CLEAR,
  PLAY,
  WIN,
  WIN_TEXT,
  LOSE,
};

int main(void) {
  /* TODO: */
  // Manipulate REG_DISPCNT here to set Mode 3. //
  REG_DISPCNT = MODE3 | BG2_ENABLE;

  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  // Load initial application state
  enum gba_state state = STARTBG;

  Player user = {
    .oldRow = 130,
    .oldCol = 10,
    .newRow = 130,
    .newCol = 10,
    .width = 18,
    .height = 22,
    .image = catOwner,
  };

  int time = 0;
  int movingText = 0;



  while (1) {
    currentButtons = BUTTONS; // Load the current state of the buttons

    /* TODO: */
    // Manipulate the state machine below as needed //
    // NOTE: Call waitForVBlank() before you draw
    waitForVBlank();
    switch (state) {
      case STARTBG:
        // Start Screen
        drawFullScreenImageDMA(coverPage);
        state = STARTTEXT;

        user.oldRow = 132;
        user.oldCol = 13;
        user.newRow = 132;
        user.newCol = 13;
        time = 0;
        break;

      case STARTTEXT:
        // Adding text to start screen
        waitForVBlank();
        undrawImageDMA(130, movingText, 240, 30, coverPage);
        drawCenteredString(130, movingText, 80, 30, "meow meow meow meow meow ", WHITE);
        movingText++;
        if (movingText == 240) {
            movingText = 0;
        }
        drawString(40, 45, "Press Start to Continue!", WHITE);


        if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
            state = CLEAR;
        }

        break;

      case CLEAR:
        // blanking the screen
        drawFullScreenImageDMA(maze);
        drawCenteredString(0, 0, WIDTH, 20, "Time: ", GREEN);
        state = PLAY;
        break;

      case PLAY:
        waitForVBlank();
        undrawImageDMA(user.oldRow, user.oldCol,user.width, user.height, maze);
        // draw cat owner
        drawImageDMA(user.newRow, user.newCol, user.width, user.height, user.image);
        user.oldRow = user.newRow;
        user.oldCol = user.newCol;

        // detects if win by finding cat
        if (collideWin(user.newRow, user.height, user.newCol, user.width)) {
            state = WIN;
            continue;
        }

        // displays time and detects if lost
        if (time % 60 == 0) {
            undrawImageDMA(6, 138, 18, 8, maze);
            char arr[4];
            snprintf(arr, sizeof(arr), "%d", 15 - (time/60));
            drawString(6, 138, arr, GREEN);
        }
        if (time == 900) {
            state = LOSE;
            continue;
        }
        time++;


        // move cat owner
        if (KEY_DOWN(BUTTON_UP, currentButtons)) {
            user.newRow--;
            if (isColliding(user.newRow, user.height, user.newCol, user.width)) {
                user.newRow++;
            }
        }
        if (KEY_DOWN(BUTTON_DOWN, currentButtons)) {
            user.newRow++;
            if (isColliding(user.newRow, user.height, user.newCol, user.width)) {
                user.newRow--;
            }
        }
        if (KEY_DOWN(BUTTON_LEFT, currentButtons)) {
            user.newCol--;
            if (isColliding(user.newRow, user.height, user.newCol, user.width)) {
                user.newCol++;
            }
        }
        if (KEY_DOWN(BUTTON_RIGHT, currentButtons)) {
            user.newCol++;
            if (isColliding(user.newRow, user.height, user.newCol, user.width)) {
                user.newCol--;
            }
       }


        if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
            state = STARTBG;
        }
        break;

      case WIN:
        drawFullScreenImageDMA(win);
        state = WIN_TEXT;
        break;

      case WIN_TEXT:
        drawCenteredString(8, 0, WIDTH, 5, "You safely found your cat!", BLACK);
        drawCenteredString(18, 0, WIDTH, 5, "Now you can get cozy and warm in bed", BLACK);
        drawCenteredString(28, 0, WIDTH, 5, "Beware if you click SELECT ", BLACK);
        drawCenteredString(38, 0, WIDTH, 5, "your cat might run away again...", BLACK);

        if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
            state = STARTBG;
        }
        if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
            state = STARTBG;
        }
        break;

      case LOSE:
        drawFullScreenImageDMA(lose);
        if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
            state = STARTBG;
        }
        if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
            state = STARTBG;
        }
        break;
    }

    previousButtons = currentButtons; // Store the current state of the buttons
  }
  return 0;
}

// detects if any part of the sprite touches the black border of the cat
int isColliding(int row, int height, int col, int width) {
    int spriteTop = row;
    int spriteBottom = row + height;
    int spriteLeft = col;
    int spriteRight = col + width;

    for (int y = spriteTop; y < spriteBottom; y++) {
        for (int x = spriteLeft; x < spriteRight; x++) {
            if (maze[(y * WIDTH) + x] == BLACK) {
                return 1;
            }
        }
    }
    return 0;
}

// detects if any part of the sprite touches the white border of the cat
int collideWin(int row, int height, int col, int width) {
    int spriteTop = row;
    int spriteBottom = row + height;
    int spriteLeft = col;
    int spriteRight = col + width;

    for (int y = spriteTop; y < spriteBottom; y++) {
        for (int x = spriteLeft; x < spriteRight; x++) {
            if (maze[(y * WIDTH) + x] == WHITE) {
                return 1;
            }
        }
    }
    return 0;
}
