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

#include <math.h>
#include <stdlib.h>
#include "utils.h"

void randVec(int* vec, int mod) {
    vec[0] = rand() % mod;
    vec[1] = rand() % mod;

    //Make sure that the direction is valid
    while(vec[0] == 0) {
        vec[0] = rand() % mod;
        vec[1] = rand() % mod;
    }
}