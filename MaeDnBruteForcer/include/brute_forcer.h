#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <math.h>


//-------------DISTANCES-------------

#define side_length_units 10000
#define tile_distance 850
#define min_units_per_tile 193 //best possible skip
#define start_pos_X 4170
#define start_pos_Y 9170
#define side_length_cm 36

//3\cdot850\ +\ 4\cdot850\ +\ \sqrt{\left(3\cdot850\right)^{2}+850^{2}}+\ \sqrt{\left(4\cdot850\right)^{2}+850^{2}}
#define total_movement_to_A 12143

double convert_units_to_cm(int units);

/**
 * Calculates the coordinates of the position index
 * and puts them into the given array
 * INDEX: 0-3: waiting | 4-43: board location | 44-47: a,b,c,d
 */
void get_coords(int index, int coords[]);

/**
 * Array used to look up the distance from one position to another -> calculated once
 * at the start of the program.
 * Layout: from = index/6 | to = index/6 + index % 6 + 2
 * i.e 1-2, 1-3, 1-4, 1-5, 1-6, 1-7, 2-3, 2-4....
 */
static int distances[40*6];

/**
 * Calculates the distances (in units) from one position to another and fills up the distances
 * array as specified in it's declaration
 */
void calculate_distances();

/**
 * Retrieves distance between two positions from the precalculated distances array.
 * Function fails if "from" has not been through the piece_extractor
 * ( = it's bit board can only have a single non-zero digit)
 * @param from the bit board of the starting position
 * @param roll_num the number rolled -> used to find the bit board to land on
 * @return distance in units
 */
int get_distance(int64_t from, int roll_num);


//-------------PATH-------------
/**
 * For an intuitive explanation of the bit board
 * open MaeDnVisualizer or Visualizer.jar
 */
#define starting_board 0b1111000000000000000000000000000000000000000000000000000000000000
#define start_p1 0b1110100000000000000000000000000000000000000000000000000000000000
#define start_p2 0b1100100000000000000000000000000000000000000000010000000000000000
#define start_p3 0b1000100000000000000000000000000000000000000000110000000000000000
#define start_p4 0b0000100000000000000000000000000000000000000001110000000000000000
#define distance_mask 0b0000000000000000000000000000000000000000000000001111111111111111
#define abcd_mask 0b0000000000000000000000000000000000000000000011110000000000000000
#define piece_extractor 0b0000111111111111111111111111111111111111111100000000000000000000
#define tile_40 0b0000000000000000000000000000000000000000000100000000000000000000
#define tile_1 0b0000100000000000000000000000000000000000000000000000000000000000
#define NULL_BOARD 0L

static int64_t full_path[4] = {starting_board, NULL_BOARD, NULL_BOARD, NULL_BOARD};
static int full_path_index = 0;
static uint16_t cur_shortest = 0b1111111111111111; // distance in units

void print_binary(int64_t board);

/**
 * Unwraps the path bitboards into single piece bitboards and
 * prints the path history step by step
 */
void print_path();


#define EXTRACT_RIGHTMOST 1
#define EXTRACT_LEFTMOST 2
/*
 * Extracts the piece_mask for the rightmost non-finished piece
 * which, in the recursive call chain of handle_piece(), is the piece the function
 * currently needs to work with while all the others are just path history.
 */
int64_t extract_piece(int64_t path, int operation_flag);

/**
 * Finds the shortest path for bringing a single piece inside the piece_mask
 * (in the bitboard) to the rightmost still empty a,b,c,d tile by updating
 * the path history bit board in it's recursive call chain.
 * @param path bit board with all previously taken step digits turned on + current distance
 * @return Shortest path length in units
 */
void handle_piece(int64_t path);

/**
 * Find the shortest *physical* path to win, assuming
 * that the pieces can move in straight lines to their rolled
 * positions.
 * Also fills out the the int64_t path bit board.
 *
 * @return Length of shortest path in units
 */
int find_shortest_path();