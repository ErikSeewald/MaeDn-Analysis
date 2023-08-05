#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <math.h>

//MEMORY OPERATIONS
#define ADD_BOARD 646868
#define GET_BOARD 716984
#define INITIALIZE 1111
#define STEP_BACK 8989
#define STEP_FORWARD 9898
#define REPLACE_BOARD 3203
#define RESET 404

//BOARD
#define side_length_units 10000
#define position_distance_units 850
#define min_units_per_square 250 //(850*2)/6 rounded down (best possible skip across corner)
#define start_pos_X 4170
#define start_pos_Y 9170
#define side_length_cm 36
static uint16_t cur_shortest = 0b1111111111111111;

//BIT BOARD
/**
 * For an intuitive explanation of the bit board
 * open MaeDnVisualizer or Visualizer.jar
 */
#define finished_board 0b11110000000000000000
#define starting_board 0b1111000000000000000000000000000000000000000000000000000000000000
#define start_p1 0b1110100000000000000000000000000000000000000000000000000000000000
#define start_p2 0b1100100000000000000000000000000000000000000000010000000000000000
#define start_p3 0b1000100000000000000000000000000000000000000000110000000000000000
#define start_p4 0b0000100000000000000000000000000000000000000001110000000000000000
#define distance_mask 0b0000000000000000000000000000000000000000000000001111111111111111
#define abcd_mask 0b0000000000000000000000000000000000000000000011110000000000000000
#define piece_extractor 0b0000111111111111111111111111111111111111111100000000000000000000

//PRECALCULATED DISTANCES
/**
 * Array used to look up the distance from one position to another -> calculated once
 * at the start of the program.
 * Layout: from = index/6 | to = index/6 + index % 6 + 2
 * i.e 1-2, 1-3, 1-4, 1-5, 1-6, 1-7, 2-3, 2-4....
 */
static uint16_t distances[40*6];

//CURRENT SHORTEST PATH
static int64_t shortest_path[1000];

void save_path(int update_index);

/**
 * Calculates the distances (in units) from one position to another and fill up the distances
 * array as specified in it's declaration
 */
void calculate_distances();

/**
 * Retrieves distance between two positions from precalculated distances array.
 * Function fails if "from" has not been through the piece_extractor
 * (i.e it's binary representations can only have a single non-zero digit)
 * @param from the bit board of the starting position
 * @param roll_num the number rolled -> used to find the bit board to land on
 * @return the uint16_t distance in units
 */
uint16_t get_distance(int64_t from, int roll_num);

/**
 * Calculates the coordinates of the given position index
 * and puts them into the given array
 * INDEX: 0-3: waiting | 4-43: board location | 44-47: a,b,c,d
 */
void get_coords(int index, uint16_t coords[]);

/**
 * Handles access and memory for the path array in which all bit boards
 * in the path are saved. Needs to be initialized once.
 * @param operation_flag INITIALIZE | GET_BOARD | ADD_BOARD
 * @param board bit board for ADD_BOARD, 0 otherwise
 * @return pointer to newest int64_t board for GET_BOARD, NULL otherwise
 */
int64_t* path_array_operation(int operation_flag, int64_t board);

/**
 * Find the shortest physical path across the board, assuming
 * that the pieces can move in straight lines to their rolled
 * positions.
 * Also fills out the global int64_t path array.
 *
 * @return Length of shortest path in units
 */
int find_shortest_path();

/**
 * Finds the shortest path for bringing the leftmost
 * (in the bitboard) piece to the rightmost still empty position
 * (excluding the last 16 bits used for saving the current distance)
 * while continuing to fill out the global int64_t path array.
 */
uint16_t handle_leftmost_piece();

/**
 * Used to find out if the end of the int64_t path[] array
 * has been reached.
 *
 * @param board int64_t bit board
 * @return 1 if the board represents a finished state, 0 otherwise
 */
int is_finished_board(int64_t board);

double convert_units_to_cm(int units);

void print_path();