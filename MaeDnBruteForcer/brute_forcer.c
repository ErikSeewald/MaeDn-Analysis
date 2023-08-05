#include "brute_forcer.h"

/**
 * Handles access and memory for the path array in which all bit boards
 * in the path are saved. Needs to be initialized once.
 * @param operation_flag INITIALIZE | GET_BOARD | ADD_BOARD
 * @param board bit board for ADD_BOARD, 0 otherwise
 * @return pointer to newest int64_t board for GET_BOARD, NULL otherwise
 */
int64_t* path_array_operation(int operation_flag, int64_t board)
{
    static int64_t cur_available_path_space = 1000000;
    static int64_t newest_board_index = 0;
    static int64_t* path;

    if (operation_flag == INITIALIZE)
    {
        path = (int64_t*) malloc(cur_available_path_space * sizeof(int64_t));
        *path = (int64_t) starting_board;
    }

    else if (operation_flag == GET_BOARD)
    {
        return (path + newest_board_index);
    }

    else if (operation_flag == STEP_BACK)
    {
        newest_board_index--;
        if (newest_board_index < 0) {return NULL;}
        return (path + newest_board_index);
    }

    else if (operation_flag == ADD_BOARD)
    {
        newest_board_index++;
        if (newest_board_index > cur_available_path_space)
        {
            cur_available_path_space *=2;
            int64_t* new_path = (int64_t*) realloc(path, cur_available_path_space * sizeof(int64_t));
            if (new_path == NULL)
            {
                perror("Memory reallocation failed");
                free(path);
                return NULL;
            }
            path = new_path;
        }

        *(path + newest_board_index) = board;
    }

    return NULL;
}


/**
 * Find the shortest physical path across the board, assuming
 * that the pieces can move in straight lines to their rolled
 * positions.
 * Also fills out the global int64_t path array.
 *
 * @return Length of shortest path in units
 */
int find_shortest_path()
{
    path_array_operation(INITIALIZE, 0);
    int path_length = 0;

    // As explained in the pdf, we only need to perform a
    // search for each of the pieces individually instead of successively
    // as they do not get in the way of each other
    path_array_operation(ADD_BOARD, start_p1);
    for (int i = 0; i < 4; i++)
    {
        handle_leftmost_piece();
        path_length += extract_distance(*path_array_operation(GET_BOARD, 0));
    }

    return path_length;
}

/**
 * Finds the shortest path for bringing the leftmost
 * (in the bitboard) piece to the rightmost still empty position
 * (excluding the last 16 bits used for saving the current distance)
 * while continuing to fill out the global int64_t path array.
 */
void handle_leftmost_piece()
{
    static int calls = 0;
    calls++;
    if (calls > 300) {return;}

    int64_t cur_board = *path_array_operation(GET_BOARD, 0);
    int64_t piece_mask = cur_board & piece_extractor;
    int64_t bests[6];
    //count down - counting up would cause it to enter branches that
    //will be cut off anyway if a higher roll reaches the goal immediately
    for (int i = 6; i > 0; i++)
    {
        //Isolated piece mask / i*2 moves the piece by i spaces in bit board
        int64_t roll = piece_mask / i*2;
1
        //Remove old position from board and add the new one, then put that new board on the path
        path_array_operation(ADD_BOARD, (cur_board ^ piece_mask) | roll);
    }
}

/**
 * Used to find out if the end of the int64_t path[] array
 * has been reached.
 *
 * @param board int64_t bit board
 * @return 1 if the board represents a finished state, 0 otherwise
 */
int is_finished_board(int64_t board)
{
    //the last 16 bits do not matter here -> discard them with &
    return (board & finished_board) == finished_board;
}

/**
 * Extracts the distance specified by the last 16 bits in
 * the bit board
 *
 * @param board int64_t bit board
 * @return uint16_t length in units
 */
uint16_t extract_distance(int64_t board)
{
    return (uint16_t) board & distance_mask;
}

double convert_units_to_cm(uint16_t units)
{
    return (double) units * side_length_cm/side_length_units;
}

void print_path()
{
    printf("\nPath printed in reverse order: \n");
    int64_t* b = path_array_operation(GET_BOARD, 0);

    while (b != NULL)
    {
        //PRINT IN BINARY
        for (int i = sizeof(*b) * 8 - 1; i >= 0; i--)
        {
            int64_t bin = (*b >> i) & 1;
            printf("%" PRId64, bin);
        }
        printf("\n");

        b = path_array_operation(STEP_BACK, 0);
    }
}

int main()
{
    clock_t start_time = clock();
    uint16_t units = find_shortest_path();

    //RUNTIME
    double elapsed_time = (double) (clock() - start_time) / CLOCKS_PER_SEC;
    printf("Brute forcer runtime: %f seconds \n", elapsed_time);

    //PATH LENGTH IN UNITS
    printf("The shortest path on a %dx%d units board is %d units. \n", side_length_units, side_length_units, units);

    //PATH LENGTH IN CM
    double cm = convert_units_to_cm(units);
    printf("The shortest path on a %dx%d cm board is %f cm \n", side_length_cm, side_length_cm, cm);

    //FULL PATH
    print_path();

    return 0;
}
