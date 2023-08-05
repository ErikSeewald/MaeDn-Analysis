#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>

//MEMORY OPERATIONS
#define ADD_BOARD 646868
#define GET_BOARD 716984
#define INITIALIZE 1111
#define STEP_BACK 8989

//BOARD
#define side_length_units 10000
#define side_length_cm 36

//BIT BOARD
/**
 * For an intuitive explanation of the bit board
 * open MaeDnVisualizer or Visualizer.jar
 */
#define finished_board 0b11110000000000000000
#define starting_board 0b1111000000000000000000000000000000000000000000000000000000000000
#define start_p1 0b1110100000000000000000000000000000000000000000000000000000000000
#define start_p2 0b1100100000000000000000000000000000000000000010000000000000000000
#define start_p3 0b1000100000000000000000000000000000000000000011000000000000000000
#define start_p4 0b0000100000000000000000000000000000000000000011100000000000000000
#define distance_mask 0b0000000000000000000000000000000000000000000000001111111111111111
#define piece_extractor 0b0000111111111111111111111111111111111111111100000000000000000000

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
void handle_leftmost_piece();

/**
 * Used to find out if the end of the int64_t path[] array
 * has been reached.
 *
 * @param board int64_t bit board
 * @return 1 if the board represents a finished state, 0 otherwise
 */
int is_finished_board(int64_t board);

/**
 * Extracts the distance specified by the last 16 bits in
 * the bit board
 *
 * @param board int64_t bit board
 * @return uint16_t length in units
 */
uint16_t extract_distance(int64_t board);

double convert_units_to_cm(uint16_t units);

void print_path();