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
#include "ball.h"

typedef enum mode_ { P1_V_CPU, P1_V_P2, CPU_V_CPU } GameMode;

typedef struct Game {
    Paddle_t* paddle_1; //Left side paddle, controlled by the left joystick
    Paddle_t* paddle_2; //Right side paddle, controlled either by the computer or the right joystick
    GameMode mode;      //The current game mode
    Ball_t* ball;       //Ball used in the game
    int scoreL;         //Score-board contain the scores (left, right) of each paddle
    int scoreR;         //Score-board contain the scores (left, right) of each paddle
    int maxScore;       //Score needed to win
} Game_t;

/**
 * @brief Creates a new game with the given game mode
 * @param mode, the mode in which the game will be played
 * @return A newly created game in the given mode
 */
Game_t* initGame(WINDOW* mainWin, GameMode mode);

/**
 * @brief Updates all of the elements in the given game
 * @param game, the game we want to update
 * @param mainWin, the window that contains the game
 */
void updateGame(Game_t* game, WINDOW* mainWin);

/**
 * @brief Draws the entire game
 * @param game, the game that we want to draw
 * @param mainWin, the window onto which we want to draw the game
 */
void drawGame(Game_t* game, WINDOW* mainWin);

/**
 * @brief Deallocates all memory related to the given game
 * @param game, the game that we want free
 */
void freeGame(Game_t* game);