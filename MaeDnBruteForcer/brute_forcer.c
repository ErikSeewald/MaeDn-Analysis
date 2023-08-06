#include "brute_forcer.h"

//-------------DISTANCES-------------

double convert_units_to_cm(int units)
{
    return (double) units * side_length_cm/side_length_units;
}


/**
 * Calculates the coordinates of the position index
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
        if (i < 4) {coords[1] -= square_distance;}
        else if (i < 8) {coords[0] -= square_distance;}
        else if (i < 10) {coords[1] -= square_distance;}
        else if (i < 14) {coords[0] += square_distance;}
        else if (i < 18) {coords[1] -= square_distance;}
        else if (i < 20) {coords[0] += square_distance;}
        else if (i < 24) {coords[1] += square_distance;}
        else if (i < 28) {coords[0] += square_distance;}
        else if (i < 30) {coords[1] += square_distance;}
        else if (i < 34) {coords[0] -= square_distance;}
        else if (i < 38) {coords[1] += square_distance;}
        else if (i < 39) {coords[0] -= square_distance;}
        else {coords[1] -= square_distance;}
        i++;
    }
}


/**
 * Calculates the distances (in units) from one position to another and fills up the distances
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
 * Retrieves distance between two positions from the precalculated distances array.
 * Function fails if "from" has not been through the piece_extractor
 * ( = it's bit board can only have a single non-zero digit)
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

//-------------PATH-------------

void print_binary(int64_t board)
{
    //PRINT IN BINARY
    for (int j = sizeof(int64_t) * 8 - 1; j >= 0; j--)
    {
        int64_t bin = (board >> j) & 1;
        printf("%" PRId64, bin);
    }
    printf("\n");
}

void print_path()
{
    printf("\nPath: \n");

    int64_t* path = path_array_operation(RESET, 0);
    int64_t cur_piece;
    static int64_t abcd_masks[4] =
            {0b00010000000000000000, 0b00100000000000000000, 0b01000000000000000000, 0b10000000000000000000};
    int i = 0;

    while (path != NULL)
    {
        cur_piece = extract_piece(*path, EXTRACT_LEFTMOST);
        while(cur_piece != 0L)
        {
            print_binary((*path&~piece_extractor)&~abcd_masks[i] | cur_piece);

            *path ^= cur_piece;
            cur_piece = extract_piece(*path, EXTRACT_LEFTMOST);
        }
        print_binary(*path&~piece_extractor);
        path = path_array_operation(STEP_FORWARD, 0);
        i++;
    }
}


/**
 * Handles access and memory for the path array in which all bit boards
 * in the path are saved. Needs to be initialized once.
 * @param operation_flag INITIALIZE | GET_BOARD | ADD_BOARD
 * @param board bit board for ADD_BOARD, REPLACE_BOARD, 0 otherwise
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

    else if (operation_flag == ADD_BOARD)
    {
        newest_board_index++;
        *(path + newest_board_index) = board;
        *(path + newest_board_index + 1) = 0L; //MARK LAST INDEX
    }

    else if (operation_flag == RESET)
    {
        newest_board_index = 0;
        return path;
    }

    else if (operation_flag == REPLACE_BOARD)
    {
        *(path + newest_board_index) = board;
    }

    return NULL;
}

/*
 * Extracts the piece_mask for the rightmost non-finished piece
 * which, in the recursive call chain of handle_piece(), is the piece the function
 * currently needs to work with while all the others are the path history.
 */
int64_t extract_piece(int64_t path, int operation_flag)
{
    int64_t piece_mask = path & piece_extractor;
    int64_t extracted;

    if (operation_flag == EXTRACT_RIGHTMOST)
    {
        extracted = square_40;
        while (!(extracted&piece_mask))
        {
            extracted <<= 1;
            if (extracted == 0L) {return 0L;}
        }
    }

    else
    {
        extracted = square_1;
        while (!(extracted&piece_mask))
        {
            extracted >>= 1;
            if (extracted == 0L) {return 0L;}
        }
    }

    return extracted;
}


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
uint16_t handle_piece(int64_t path)
{
    int64_t piece_mask = extract_piece(path, EXTRACT_RIGHTMOST);
    uint16_t bests[6] = {distance_mask, distance_mask, distance_mask, distance_mask, distance_mask, distance_mask};

    //CHECK IF PATH CAN EVEN STILL BE SHORTER THAN cur_shortest
    int squares_left = 0;
    int64_t temp_piece_mask = piece_mask;
    while(!(temp_piece_mask&(path&abcd_mask|distance_mask)))
    {
        temp_piece_mask >>= 1;
        squares_left++;
    }
    squares_left--;

    if ((path & distance_mask) + squares_left*min_units_per_square  > cur_shortest)
    {return distance_mask;}

    //count down - counting up would cause it to enter branches that
    //will be cut off anyway if a higher roll reaches the goal immediately
    for (int i = 6; i > 0; i--)
    {
        int64_t roll = piece_mask >> i;

        // skip if roll enters last 16 bits || lands on a finished piece
        if ((roll & distance_mask) || (roll & path))
        {continue;}

        uint16_t dist = get_distance(piece_mask, i);

        //Check if roll lands on the rightmost empty position
        //i.e piece shifted 1 to the right overlaps with rightmost finished piece
        //or piece shifted 1 to the right overlaps with distance mask
        if ((roll & abcd_mask))
        {
            if (((roll >> 1) & path) || (roll >> 1) & distance_mask)
            {
                uint16_t new_length = (path & distance_mask) + dist;

                if (new_length < cur_shortest)
                {
                    //Save this paths final history
                    path_array_operation(REPLACE_BOARD, (path | roll) + dist);
                    cur_shortest = new_length;
                }
                return new_length;
            }
                //skip if we landed on the wrong [a,b,c,d] space as we would get in the way of
                //other pieces if we do not always finish at the very rightmost position
            else
            {continue;}
        }

        //Handle current rolls subtree
        bests[i - 1] = handle_piece((path | roll) + dist);
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
 * Find the shortest *physical* path to win, assuming
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

    int64_t start_board;
    for (int i = 0; i < 4; i++)
    {
        switch (i)
        {
            case 0: start_board = start_p1;break;
            case 1: start_board = start_p2; break;
            case 2: start_board = start_p3; break;
            case 3: start_board = start_p4; break;
            default: break;
        }
        handle_piece(start_board);
        path_length += cur_shortest;
        cur_shortest = distance_mask; //RESET
        path_array_operation(ADD_BOARD, 0); //PATH HISTORY FOR NEXT ROUND
    }

    return path_length;
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
