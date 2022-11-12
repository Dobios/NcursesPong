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
#include <unistd.h>
#include <ncurses.h>
#include <stdio.h>

#include "game.h"

#define WINDOW_SPACING (1)
#define N_MENU_OPTIONS (3)
#define ENTER (10)
#define SLEEP_DURATION_US ((int)((1.0/15.0) * 1000000))

void initCurses() {
    //Init ncurses screen
    initscr();

    //Remove all char echo
    noecho();
    curs_set(0);
}

WINDOW** setupWindows() {
    //Create a new window
    int height, width, init_y, init_x;

    //Retrieve host terminal size
    getmaxyx(stdscr, height, width);
    getbegyx(stdscr, init_y, init_x);

    //Offset by a bit
    init_y += 2;
    init_x += 4;

    //Split the screen into two windows 1/4 -- 3/4
    int mainHeight = 3 * (height - init_y) / 4;
    int mainWidth = width - (init_x * 2);

    int secHeight = height - WINDOW_SPACING - mainHeight - init_y;
    int secWidth = mainWidth;

    //Create the main window 
    WINDOW* mainWin = newwin(mainHeight, mainWidth, init_y, init_x);

    //Move cursur down a bit
    init_y += WINDOW_SPACING + mainHeight;

    //Create secondary window
    WINDOW* secWin = newwin(secHeight, secWidth, init_y, init_x);

    //Refresh screen
    refresh();

    //Draw window borders
    box(mainWin, 0, 0);
    box(secWin, 0, 0);
    wrefresh(mainWin);
    wrefresh(secWin);

    WINDOW** result = calloc(2, sizeof(WINDOW*));
    result[0] = mainWin;
    result[1] = secWin;

    return result;
}

GameMode menuScreen(WINDOW* secWin) {
    //Clear the screen
    wclear(secWin);
    box(secWin, 0, 0);

    //Different menu options
    char* menuOptions[N_MENU_OPTIONS] = {"1 Player", "2 Players", "CPU vs. CPU"};
    int choice = -1;
    int highlight = 0;

    //Print out menu title
    mvwprintw(secWin, 1, 1, "Main menu: Select your game mode");

    //Menu loop
    while(1) {
        //Print out all menu options
        for(int i = 0; i < N_MENU_OPTIONS; ++i) {
            if(highlight == i) {
                //Highlight the current option
                wattron(secWin, A_REVERSE);
            }
            //Print the menu option on screen and unhighlight it
            if((i + 3) >= getmaxy(secWin)) {
                mvwprintw(secWin, i + 2, getmaxx(secWin) /2, menuOptions[i]);
            } else {
                mvwprintw(secWin, i + 3, 3, menuOptions[i]);
            }
            wattroff(secWin, A_REVERSE);
        }
        //Refresh the window
        wrefresh(secWin);
     
		//======================================================
		int c = wgetch(secWin);

		if(c == (int)'w') {
			highlight = highlight == 0 ? 0 : --highlight;
		} else if(c == (int)'s') {
			highlight = (highlight == N_MENU_OPTIONS - 1) ? highlight : ++highlight;
		}

		//Check for selection confirmation
		if(c == ENTER) {
			return highlight % N_MENU_OPTIONS;
		}
		//======================================================

    }
}

void drawScores(Game_t* game, WINDOW* secWin) {
    //Sanity Check
    if(game != NULL && secWin != NULL) {
        //Compute drawing coordinates
        int mid_y = (getmaxy(secWin)) / 2;
        int first_x = (getmaxx(secWin)) / 3;
        int second_x = 2 * first_x;

        //Convert the scores to character strings
        char score1[3];
        char score2[3];
        sprintf(score1, "%d", (game->scoreL));
        sprintf(score2, "%d", (game->scoreR));

        //Draw the scores
        mvwprintw(secWin, mid_y, first_x, score1);
        mvwprintw(secWin, mid_y, first_x + ((second_x - first_x) / 2), "-");  
        mvwprintw(secWin, mid_y, second_x, score2); 
    }
}

void playGame(Game_t* game, WINDOW* mainWin, WINDOW* secWin) {
    //Clear both windows
    wclear(mainWin);
    wclear(secWin);
    box(mainWin, 0, 0);
    box(secWin, 0, 0);

    //Disable input for the secondary window
    keypad(secWin, false);

    //Init Score Board 
    mvwprintw(secWin, 1, 1, "Score:");

    //Main game loop
    while(1) {
        if(game->scoreL >= game->maxScore ||
            game->scoreR >= game->maxScore) {
                break;
        }
        //Show the scores
        drawScores(game, secWin);
        wrefresh(secWin);

        //Update the game 
        updateGame(game, mainWin);

        //Draw the game
        drawGame(game, mainWin);

        //Refresh the window
        wrefresh(mainWin);

        //Sleep for a while to avoid an excessive refresh rate
        usleep(SLEEP_DURATION_US);
    } 
}

int main(int argc, char** argv) {
    //Initialize ncurses
    initCurses();

    //Setup main window
    WINDOW** wins = setupWindows();
    WINDOW* mainWin = wins[0];
    WINDOW* secWin = wins[1];

    //Free useless temp result array
    free(wins);
    wins = NULL;

    //==============UPDATE===================
    //Show the menu screen
    GameMode mode = menuScreen(secWin);
    
    //Create a new game
    Game_t* game = initGame(mainWin, mode);

    //Show the main screen
    playGame(game, mainWin, secWin);
    //=======================================
    char winner = game->scoreL > game->scoreR;

    //Free the game
    freeGame(game);
    game = NULL;

    //End ncurses
    endwin();

    //Show who won
    if(winner) {
        printf("Player 1 is the winner ! \n");
    } else {
        printf("Player 2 is the winner ! \n");
    }

    return EXIT_SUCCESS;
}

