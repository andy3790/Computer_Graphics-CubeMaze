#pragma once
#include "includer.h"


#define MAX_LOOF_COUNT 100000


extern int maze_size;

extern bool*** maze;
extern int*** maze_wilson;

void get_size_of_maze(int max_size, int min_size);
void print_maze();
void print_maze_wilson();
void init_maze();
void init_maze_wilson();
void make_maze_wilson();