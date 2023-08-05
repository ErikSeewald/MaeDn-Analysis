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
    static int newest_board_index = 0;
    static int64_t* path;

    if (operation_flag == INITIALIZE)
    {
        path = (int64_t*) malloc(1000 * sizeof(int64_t));
        *path = (int64_t) starting_board;
        *(path + 1) = 0L; //MARK LAST INDEX
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

    else if (operation_flag == STEP_FORWARD)
    {
        newest_board_index++;
        if (*(path + newest_board_index) == 0L) {return NULL;}
        return (path + newest_board_index);
    }

    else if (operation_flag == REPLACE_BOARD)
    {
        *(path + newest_board_index) = board;
    }

    else if (operation_flag == ADD_BOARD)
    {
        newest_board_index++;
        *(path + newest_board_index) = board;
        *(path + newest_board_index + 1) = 0L; //MARK LAST INDEX
    }

    else if (operation_flag == RESET)
    {
        newest_board_index = 0;
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

    for (int i = 0; i < 4; i++)
    {
        switch (i)
        {
            case 0: path_array_operation(ADD_BOARD, start_p1); break;
            case 1: path_array_operation(ADD_BOARD, start_p2); break;
            case 2: path_array_operation(ADD_BOARD, start_p3); break;
            case 3: path_array_operation(ADD_BOARD, start_p4); break;
            default: break;
        }
        handle_leftmost_piece(1);
        path_length += cur_shortest;
        cur_shortest = distance_mask;
        save_path(1); //update index;
        path_array_operation(RESET, 0);
    }

    return path_length;
}

void save_path(int update_index)
{
    static int i = 0;
    static int min_index = 0;

    if (update_index) {min_index = i; return;}
    i = min_index;

    int64_t* b = path_array_operation(GET_BOARD, 0);

    while (b != NULL)
    {b = path_array_operation(STEP_BACK, 0);}

    b = path_array_operation(STEP_FORWARD, 0);
    while (b != NULL)
    {
        shortest_path[i] = *b;
        i++;
        b = path_array_operation(STEP_FORWARD, 0);
    }

    //REMOVE FALSE ONES
    int j = min_index;
    while (shortest_path[j] != 0L)
    {
        //EHH
        int x = j;
        int skip = 0;
        while (j != 0 && shortest_path[x] != 0L)
        {
            if ((shortest_path[j]&piece_extractor) < (shortest_path[x]&piece_extractor))
            {
                shortest_path[j] = distance_mask;
                break;
            }
            x++;
        }
        j++;
    }

    shortest_path[i] = 0L; //END
}

/**
 * Finds the shortest path for bringing the leftmost
 * (in the bitboard) piece to the rightmost still empty position
 * (excluding the last 16 bits used for saving the current distance)
 * while continuing to fill out the global int64_t path array.
 */
uint16_t handle_leftmost_piece()
{
    int64_t cur_board = *path_array_operation(GET_BOARD, 0);
    int64_t piece_mask = cur_board & piece_extractor;
    int64_t bests[6] = {distance_mask, distance_mask, distance_mask, distance_mask, distance_mask, distance_mask};

    //CHECK IF PATH CAN EVEN STILL BE SHORTER THAN cur_shortest
    int squares_left = 0;
    int64_t temp_piece_mask = piece_mask;
    while(!(temp_piece_mask&distance_mask))
    {
        temp_piece_mask >>= 1;
        squares_left++;
    }
    squares_left--;

    if ((cur_board & distance_mask) + squares_left*min_units_per_square  > cur_shortest)
    {return distance_mask;}

    //count down - counting up would cause it to enter branches that
    //will be cut off anyway if a higher roll reaches the goal immediately
    for (int i = 6; i > 0; i--)
    {
        int64_t roll = piece_mask >> i;

        // skip if roll enters last 16 bits || lands on a finished piece
        if ((roll & distance_mask) || (roll & cur_board))
        {continue;}

        //Check if roll lands on the rightmost empty position
        //i.e piece shifted 1 to the right overlaps with rightmost finished piece
        //or piece shifted 1 to the right overlaps with distance mask
        if ((roll & abcd_mask))
        {
            if (((roll >> 1) & cur_board) || (roll >> 1) & distance_mask)
            {
                uint16_t dist = get_distance(piece_mask, i);
                uint16_t new_length = (cur_board & distance_mask) + dist;

                if (new_length < cur_shortest)
                {
                    //Save this pieces final position
                    path_array_operation(ADD_BOARD, ((cur_board ^ piece_mask) | roll) + dist);
                    cur_shortest = new_length;
                    save_path(0);
                }
                return new_length;
            }
            //skip if we landed on the wrong [a,b,c,d] space as we would get in the way of
            //other pieces if we do not always finish at the very rightmost position
            else
            {continue;}
        }


        //Remove old position from board and add the new one, then put that new board on the path
        path_array_operation(ADD_BOARD, ((cur_board ^ piece_mask) | roll) + get_distance(piece_mask, i));

        //Handle current rolls subtree
        bests[i - 1] = handle_leftmost_piece();

        //Step back into current branch
        path_array_operation(STEP_BACK, 0);
    }

    //Get shortest path length for this branch
    uint16_t shortest = (uint16_t) (bests[0] & distance_mask);
    for (int i = 1; i < 6; i++)
    {
        uint16_t cur = (uint16_t) (bests[i] & distance_mask);
        if (cur < shortest)
        {shortest = cur;}
    }

    return shortest;
}

/**
 * Retrieves distance between two positions from precalculated distances array.
 * Function fails if "from" has not been through the piece_extractor
 * (i.e it's binary representations can only have a single non-zero digit)
 * @param from the bit board of the starting position
 * @param roll_num the number rolled -> used to find the bit board to land on
 * @return the uint16_t distance in units
 */
uint16_t get_distance(int64_t from, int roll_num)
{
    int from_index = 0;
    while(!(from & starting_board))
    {
        from <<= 1;
        from_index++;
    }
    from_index--;

    uint16_t distance = distances[from_index * 6 + (roll_num-1)];
    return distance;
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

double convert_units_to_cm(int units)
{
    return (double) units * side_length_cm/side_length_units;
}

void print_path()
{
    printf("\nPath: \n");

    int i = 0;
    while (shortest_path[i] != 0L)
    {
        if (shortest_path[i] == distance_mask) {i++; continue;}
        //PRINT IN BINARY
        for (int j = sizeof(int64_t) * 8 - 1; j >= 0; j--)
        {
            int64_t bin = (shortest_path[i] >> j) & 1;
            printf("%" PRId64, bin);
        }
        printf("\n");
        i++;
    }
}

/**
 * Calculates the distances (in units) from one position to another and fill up the distances
 * array as specified in it's declaration
 */
void calculate_distances()
{
    uint16_t from[2];
    uint16_t to[2];

    //needs to be signed for negative vectors (fine since we can only move 6 spaces at
    //a time which -> vec distance is always way less than the int16_t limit)
    int16_t vec[2];
    for (int i = 0; i < 40; i++)
    {
        get_coords(i, from);
        for (int j = 0; j < 6; j++)
        {
            get_coords(i+j+1, to);
            vec[0] = to[0] - from[0];
            vec[1] = to[1] - from[1];
            distances[i*6 + j] = (uint16_t) round(sqrt(vec[0] * vec[0] + vec[1] * vec[1]));
        }
    }
}

/**
 * Calculates the coordinates of the given position index
 * and puts them into the given array
 * INDEX: 0-3: waiting | 4-43: board location | 44-47: a,b,c,d
 */
void get_coords(int index, uint16_t coords[])
{
    coords[0] = start_pos_X;
    coords[1] = start_pos_Y;

    //WALK ALONG GAME PATH
    int i = 0;
    while (i < index)
    {
        if (i < 4) {coords[1] -= 850;}
        else if (i < 8) {coords[0] -= 850;}
        else if (i < 10) {coords[1] -= 850;}
        else if (i < 14) {coords[0] += 850;}
        else if (i < 18) {coords[1] -= 850;}
        else if (i < 20) {coords[0] += 850;}
        else if (i < 24) {coords[1] += 850;}
        else if (i < 28) {coords[0] += 850;}
        else if (i < 30) {coords[1] += 850;}
        else if (i < 34) {coords[0] -= 850;}
        else if (i < 38) {coords[1] += 850;}
        else if (i < 39) {coords[0] -= 850;}
        else {coords[1] -= 850;}
        i++;
    }
}


int main()
{
    clock_t start_time = clock();
    calculate_distances();
    int units = find_shortest_path();

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
