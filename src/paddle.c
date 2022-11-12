/*
ncurses-pong
Copyright (C) 2020  Andrew Dobis

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include "paddle.h"

#define MOVED 1
#define NO_MOVE 0

Paddle_t* initPaddle(WINDOW* mainWin, int init_x) {
    //Allocate memory for the paddle
    Paddle_t* paddle = calloc(1, sizeof(Paddle_t));

    int mid_y = (getmaxy(mainWin)) / 2;

    //Create the paddle
    Paddle_t paddle_ = {
        mid_y,
        init_x,
        0
    };

    *paddle = paddle_;

    return paddle;
}

void movePaddle(Paddle_t* paddle, WINDOW* mainWin, MoveDir dir) {
    switch(dir) {
        case UP:
            //Check that our paddle is still in the window bounds
            if(paddle->y_pos - 3 > 0) {
                paddle->y_pos -= 1;
            }
            break;
        case DOWN:
            //Check that our paddle is still in the window bounds
            if(paddle->y_pos + 2 < getmaxy(mainWin)) {
                paddle->y_pos += 1;
            }
            break;
        default:
            break;
    }
}

void updatePaddle(Paddle_t* paddle, WINDOW* mainWin, int player) {

    int up_key = player == 1 ? (int)'w' : (int)'i';
    int down_key = player == 1 ? (int)'s' : (int)'k';

    int c = wgetch(mainWin);

    if(c == up_key) {
        //Move the paddle up
        movePaddle(paddle, mainWin, UP);
    } else if(c == down_key) {
        //Move the paddle down
        movePaddle(paddle, mainWin, DOWN);
    }
}

void freePaddle(Paddle_t* paddle) {
    //Deallocate memory associated to the paddle
    free(paddle);
}