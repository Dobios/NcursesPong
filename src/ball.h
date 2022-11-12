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

#pragma once

#include <ncurses.h>
#include "paddle.h"

typedef struct Ball {
    int x_pos;          //Variable x position of the ball in the terrain
    int y_pos;          //Variable y position of the ball in the terrain
    int velocity;       //Current speed of the ball
    int* direction;     //Current direction of the ball
} Ball_t;

/**
 * @brief created a new ball with a centered initial position
 * and a random initial direction and velocity
 * @param center_x, the x position at the center of the window
 * @param center_y, the y position at the center of the window
 * @return a pointer to the newly created ball 
 */
Ball_t* initBall(int center_x, int center_y, WINDOW* mainWin);

/**
 * @brief Resets the position of the given ball
 * @param ball, the ball whose position will be reset
 * @param mainWin, the window in which the reset ball is contained
 */
void resetBallPos(Ball_t* ball, WINDOW* mainWin);

/**
 * @brief Collides the given ball with the given paddle.
 * @param ball, the ball whose direction will be updated
 * @param paddle, the paddle with whom the ball has collided
 * @param new_x, a pointer to the new x pos of the ball
 * @param new_y, a pointer to the new y pos of the ball
 */
void collideWithPaddle(Ball_t* ball, Paddle_t* paddle, int* new_x, int* new_y);

/**
 * @brief moves a ball through the terrain
 * @param ball, the ball that we want to move
 * @param mainWin, the window onto which we will draw
 * @param paddle_1, the left paddle
 * @param paddle_2, the right paddle
 */
void moveBall(Ball_t* ball, WINDOW* mainWin, Paddle_t* paddle_1, Paddle_t* paddle_2);
/**
 * @brief moves the given ball back to it's previous position
 * @param ball, the ball that we want to move back
 */
void moveBallBack(Ball_t* ball);

/**
 * @brief Draws the given ball onto the given window
 * @param ball, the ball we want to draw
 * @param mainWin, the window onto which we will draw
 */
void drawBall(Ball_t* ball, WINDOW* mainWin);
 
/**
 * @brief deallocated the given ball
 * @param ball, a pointer on the ball that we want to free
 */
void freeBall(Ball_t* ball);