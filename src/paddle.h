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

#define PADDLE_SIZE 5

typedef enum dir_ { UP, DOWN } MoveDir;

typedef struct Paddle {
    int y_pos;      //Variable y position of the current paddle
    int x_pos;      //Fixed x position of the current paddle
    int points;     //Variable number of points the paddle has
} Paddle_t;

/**
 * @brief Creates a new paddle at the given x position
 * @param init_x, the x position at which the paddle will be fixed
 * @return a pointer to the newly created paddle
 */
Paddle_t* initPaddle(WINDOW* mainWin, int init_x);

/**
 * @brief moves the given paddle in the given direction
 * @param paddle, the paddle that we want to move
 * @param mainWin, the window in which the paddle is contained
 * @param dir, the direction in which the paddle will be moved
 */
void movePaddle(Paddle_t* paddle, WINDOW* mainWin, MoveDir dir);

/**
 * @brief Updates the given paddle's attributes
 * @param paddle, the paddle that will be updated
 * @param mainWin, the window in which the paddle is contained
 * @param player, the player ID (1 for left, 2 for right)
 * @param isAI, whether or not our right paddle is controlled by the computer
 */
void updatePaddle(Paddle_t* paddle, WINDOW* mainWin, int player);

/**
 * @brief Deallocated all memory linked to the given paddle
 * @param paddle, the given paddle that will be freed
 */
void freePaddle(Paddle_t* paddle);