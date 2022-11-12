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

#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils.h"
#include "ball.h"

#define MAX_VELOCITY 5
#define DIR_MODULUS 3
#define A_LITTLE 1000000

Ball_t* initBall(int center_x, int center_y, WINDOW* mainWin) {
    //Initialize random number generator
    srand(time(NULL));

    //Allocate memory for the ball
    Ball_t* ball = calloc(1, sizeof(Ball_t));
    
    //Initialize a random direction
    int* dir = calloc(2, sizeof(int));
    randVec(dir, DIR_MODULUS);

    //Create the ball
    Ball_t ball_ = {
        center_x,
        center_y,
        1, //rand() % MAX_VELOCITY,
        dir
    };

    //Set pointer to be on ball
    *ball = ball_;

    return ball;
}

void resetBallPos(Ball_t* ball, WINDOW* mainWin) {
    //Reset the ball's position
    ball->x_pos = getmaxx(mainWin) / 2;
    ball->y_pos = getmaxy(mainWin) / 2;

    //Compute a random initial direction
    randVec(ball->direction, DIR_MODULUS);
}

void collideWithPaddle(Ball_t* ball, Paddle_t* paddle, int* new_x, int* new_y) {
    while(*new_x == paddle->x_pos) {
        //Update direction
        int x_dir = -(ball->direction[0]);
        randVec(ball->direction, DIR_MODULUS);
        ball->direction[0] = x_dir;

        //Compute new postitions
        *new_x = ball->x_pos + (ball->direction[0] * ball->velocity);
        *new_y = ball->y_pos + (ball->direction[1] * ball->velocity);
    }
}

void moveBall(Ball_t* ball, WINDOW* mainWin, Paddle_t* paddle_1, Paddle_t* paddle_2) {

    //Compute the new postition and set it
    int new_x = ball->x_pos + (ball->direction[0] * ball->velocity);
    int new_y = ball->y_pos + (ball->direction[1] * ball->velocity);

    //Check for horizontal wall collisions
    if(new_x <= 0) {
        //Increase right score
        paddle_2->points += 1;

        //Reset the ball's position
        ball->x_pos = getmaxx(mainWin) / 2;
        ball->y_pos = getmaxy(mainWin) / 2;

        //Compute a random initial direction
        randVec(ball->direction, DIR_MODULUS);

        //Wait a little
        usleep(A_LITTLE);

        return;
    } 
    
    if(new_x >= getmaxx(mainWin)) {
        //Increase right score
        paddle_1->points += 1;

        //Reset the ball's position
        ball->x_pos = getmaxx(mainWin) / 2;
        ball->y_pos = getmaxy(mainWin) / 2;

        //Compute a random initial direction
        randVec(ball->direction, DIR_MODULUS);

        //Wait a little
        usleep(A_LITTLE);

        return;
    } 
    //Check for vertical wall collisions
    if(new_y <= 0 || new_y >= getmaxy(mainWin)) {
        ball->direction[1] *= -1;
        new_y = ball->y_pos + (ball->direction[1] * ball->velocity);
    }

    if(new_x == paddle_1->x_pos) {
        //Check for left paddle collisions
        if(new_y >= (paddle_1->y_pos - (PADDLE_SIZE / 2)) &&
            new_y <= (paddle_1->y_pos + (PADDLE_SIZE / 2))) {
            
            //Collide with the paddle
            collideWithPaddle(ball, paddle_1, &new_x, &new_y);
        }
    }
    
    if(new_x == paddle_2->x_pos) {
        //Check for right paddle collisions
        if(new_y >= (paddle_2->y_pos - (PADDLE_SIZE / 2)) &&
            new_y <= (paddle_2->y_pos + (PADDLE_SIZE / 2))) {
            
            //Collide with the paddle
            collideWithPaddle(ball, paddle_2, &new_x, &new_y);
        }
    }

    ball->x_pos = new_x;
    ball->y_pos = new_y;
}

void freeBall(Ball_t* ball) {
    free(ball->direction);
    ball->direction = NULL;

    //Free the ball structure
    free(ball);
}