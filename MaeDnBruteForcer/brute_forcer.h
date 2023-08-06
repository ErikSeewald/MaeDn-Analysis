#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <math.h>


//-------------MEMORY OPERATIONS-------------
#define ADD_BOARD 1
#define GET_BOARD 2
#define INITIALIZE 3
#define STEP_BACK 4
#define STEP_FORWARD 5
#define RESET 6
#define REPLACE_BOARD 7


//-------------BOARD-------------
#define side_length_units 10000
#define square_distance 850
#define min_units_per_square 200 //best possible skip
#define start_pos_X 4170
#define start_pos_Y 9170
#define side_length_cm 36


//-------------BIT BOARD-------------
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
#define square_40 0b0000000000000000000000000000000000000000000100000000000000000000
#define square_1 0b0000100000000000000000000000000000000000000000000000000000000000


//-------------DISTANCES-------------
/**
 * Array used to look up the distance from one position to another -> calculated once
 * at the start of the program.
 * Layout: from = index/6 | to = index/6 + index % 6 + 2
 * i.e 1-2, 1-3, 1-4, 1-5, 1-6, 1-7, 2-3, 2-4....
 */
static uint16_t distances[40*6];

double convert_units_to_cm(int units);

/**
 * Calculates the coordinates of the position index
 * and puts them into the given array
 * INDEX: 0-3: waiting | 4-43: board location | 44-47: a,b,c,d
 */
void get_coords(int index, uint16_t coords[]);

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
 * @return the uint16_t distance in units
 */
uint16_t get_distance(int64_t from, int roll_num);


//-------------PATH-------------
static uint16_t cur_shortest = 0b1111111111111111; // distance in units

void print_binary(int64_t board);

void print_path();

/**
 * Handles access and memory for the path array in which all bit boards
 * in the path are saved. Needs to be initialized once.
 * @param operation_flag INITIALIZE | GET_BOARD | ADD_BOARD
 * @param board bit board for ADD_BOARD, 0 otherwise
 * @return pointer to newest int64_t board for GET_BOARD, NULL otherwise
 */
int64_t* path_array_operation(int operation_flag, int64_t board);


#define EXTRACT_RIGHTMOST 1
#define EXTRACT_LEFTMOST 2
/*
 * Extracts the piece_mask for the rightmost non-finished piece
 * which, in the recursive call chain of handle_piece(), is the piece the function
 * currently needs to work with while all the others are the path history.
 */
int64_t extract_piece(int64_t path, int operation_flag);

/**
 * Finds the shortest path for bringing a single piece inside the piece_mask
 * (in the bitboard) to the rightmost still empty a,b,c,d tile.
 * It also updates the path history bit board in it's recursive call chain
 * When a new shortest path is found, the path history up to that point is saved.
 * At the end the path history with the shortest distance is unfolded back into
 * single steps to print out.
 * @param path bit board with all previously taken step digits turned on
 * @return Shortest path length in units
 */
uint16_t handle_piece(int64_t path);

/**
 * Find the shortest *physical* path to win, assuming
 * that the pieces can move in straight lines to their rolled
 * positions.
 * Also fills out the global int64_t path array.
 *
 * @return Length of shortest path in units
 */
int find_shortest_path();