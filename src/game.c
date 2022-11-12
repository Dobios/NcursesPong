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
#include <ncurses.h>
#include "game.h"

#define EDGE_OFFSET 2
#define MAX_SCORE 5

Game_t* initGame(WINDOW* mainWin, GameMode mode) {
    //Compute the paddle positions
    int start_x = 0;
    int end_x = getmaxx(mainWin);

    nodelay(mainWin, true);

    //Initialize both paddles
    Paddle_t* paddle1 = initPaddle(mainWin, start_x + EDGE_OFFSET);
    Paddle_t* paddle2 = initPaddle(mainWin, end_x - EDGE_OFFSET - 1);
    GameMode g_mode = mode; //mode is 0 if 1 PLAYER & 1 if 2 PLAYERS

    //Initialize the ball
    int mid_x = (end_x - start_x) / 2;
    int mid_y = (getmaxy(mainWin)) / 2;
    Ball_t* ball = initBall(mid_x, mid_y, mainWin);

    //Allocate and initialize the game
    Game_t* game = calloc(1, sizeof(Game_t));
    Game_t game_ = {paddle1, paddle2, g_mode, ball, 0, 0, MAX_SCORE};
    
    *game = game_;
    return game;
}

void updatePaddleAI(Game_t* game, WINDOW* mainWin, int player) {
    //Check the ball's next position
    int new_y = game->ball->y_pos + (game->ball->direction[1] * game->ball->velocity);

    int y_pos;
    Paddle_t* paddle;
    char moveCond;

    if(player == 1) {
        y_pos = game->paddle_1->y_pos;
        paddle = game->paddle_1;
        moveCond = game->ball->direction[0] < 0;
    } else {
        y_pos = game->paddle_2->y_pos;
        paddle = game->paddle_2;
        moveCond = game->ball->direction[0] > 0;
    }

    //Only move the paddle if the ball is coming in its direction
    if(moveCond) {
        //Move towards the ball
        if(new_y > y_pos) {
            movePaddle(paddle, mainWin, DOWN);
        } 

        if(new_y < y_pos) {
            movePaddle(paddle, mainWin, UP);
        }
    }
}

void updateGame(Game_t* game, WINDOW* mainWin) {
    //Clear the frame
    wclear(mainWin);
    box(mainWin, 0, 0);

    //Update both paddles
    if(game->mode == CPU_V_CPU) {
        updatePaddleAI(game, mainWin, 1);
    } else {
        updatePaddle(game->paddle_1, mainWin, 1);
    }
    
    if(game->mode != P1_V_P2) {
        updatePaddleAI(game, mainWin, 2);
    } else {
        updatePaddle(game->paddle_2, mainWin, 2);
    }
    
    //Udpate the ball
    moveBall(game->ball, mainWin, game->paddle_1, game->paddle_2);

    //Update game score
    game->scoreL = game->paddle_1->points;
    game->scoreR = game->paddle_2->points;
}

void drawGame(Game_t* game, WINDOW* mainWin) {
    //Draw both paddles
    for(int i = 0; i < PADDLE_SIZE; ++i) {
        mvwprintw(mainWin, game->paddle_1->y_pos - (PADDLE_SIZE / 2) + i, game->paddle_1->x_pos, "#");
        mvwprintw(mainWin, game->paddle_2->y_pos - (PADDLE_SIZE / 2) + i, game->paddle_2->x_pos, "#");
    } 

    mvwprintw(mainWin, game->ball->y_pos, game->ball->x_pos, "*");
}

void freeGame(Game_t* game) {
    //Free both paddles
    freePaddle(game->paddle_1);
    freePaddle(game->paddle_2);
    game->paddle_1 = NULL;
    game->paddle_2 = NULL;

    //Free the ball
    freeBall(game->ball);
    game->ball = NULL;

    //Free the game structure
    free(game);
}