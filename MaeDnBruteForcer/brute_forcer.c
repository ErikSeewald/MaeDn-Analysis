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
void get_coords(int index, int coords[])
{
    coords[0] = start_pos_X;
    coords[1] = start_pos_Y;

    //WALK ALONG GAME PATH
    int i = 0;
    while (i < index)
    {
        if (i < 4) {coords[1] -= tile_distance;}
        else if (i < 8) {coords[0] -= tile_distance;}
        else if (i < 10) {coords[1] -= tile_distance;}
        else if (i < 14) {coords[0] += tile_distance;}
        else if (i < 18) {coords[1] -= tile_distance;}
        else if (i < 20) {coords[0] += tile_distance;}
        else if (i < 24) {coords[1] += tile_distance;}
        else if (i < 28) {coords[0] += tile_distance;}
        else if (i < 30) {coords[1] += tile_distance;}
        else if (i < 34) {coords[0] -= tile_distance;}
        else if (i < 38) {coords[1] += tile_distance;}
        else if (i < 39) {coords[0] -= tile_distance;}
        else {coords[1] -= tile_distance;}
        i++;
    }
}


/**
 * Calculates the distances (in units) from one position to another and fills up the distances
 * array as specified in it's declaration
 */
void calculate_distances()
{
    int from[2], to[2], vec[2];
    for (int i = 0; i < 40; i++)
    {
        get_coords(i, from);
        for (int j = 0; j < 6; j++)
        {
            get_coords(i+j+1, to);
            vec[0] = (int16_t) (to[0] - from[0]);
            vec[1] = (int16_t) (to[1] - from[1]);
            distances[i*6 + j] = round(sqrt(vec[0] * vec[0] + vec[1] * vec[1]));
        }
    }
}


/**
 * Retrieves distance between two positions from the precalculated distances array.
 * Function fails if "from" has not been through the piece_extractor
 * ( = it's bit board can only have a single non-zero digit)
 * @param from the bit board of the starting position
 * @param roll_num the number rolled -> used to find the bit board to land on
 * @return distance in units
 */
int get_distance(int64_t from, int roll_num)
{
    int from_index = 0;
    while(!(from & starting_board))
    {
        from <<= 1;
        from_index++;
    }
    from_index--;

    int distance = distances[from_index * 6 + (roll_num-1)];
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

/**
 * Unwraps the path bitboards into single piece bitboards and
 * prints the path history step by step
 */
void print_path()
{
    static int64_t abcd_masks[4] =
            {0b00010000000000000000, 0b00100000000000000000, 0b01000000000000000000, 0b10000000000000000000};

    printf("\nPath: \n");

    int64_t cur_piece;
    int64_t cur_path;

    for (int i = 0; i < 4; i++)
    {
        cur_path = full_path[i];
        cur_piece = extract_piece(cur_path, EXTRACT_LEFTMOST);
        while(cur_piece != NULL_BOARD)
        {
            //Remove all pieces that are not on standby or already finished as declared by
            //abcd_masks[i] and only place cur_piece
            print_binary((cur_path&~piece_extractor)&~abcd_masks[i] | cur_piece);

            cur_path ^= cur_piece; //remove cur_piece from cur_path
            cur_piece = extract_piece(cur_path, EXTRACT_LEFTMOST);
        }
        print_binary(cur_path&~piece_extractor); //print finish step
    }
}


/*
 * Extracts the piece_mask for the rightmost non-finished piece
 * which, in the recursive call chain of handle_piece(), is the piece the function
 * currently needs to work with while all the others are just path history.
 */
int64_t extract_piece(int64_t path, int operation_flag)
{
    int64_t piece_mask = path & piece_extractor;
    if (piece_mask == NULL_BOARD) {return NULL_BOARD;}

    int64_t extracted;

    if (operation_flag == EXTRACT_RIGHTMOST)
    {
        extracted = tile_40;
        while (!(extracted&piece_mask)) {extracted <<= 1;}
    }

    else
    {
        extracted = tile_1;
        while (!(extracted&piece_mask)) {extracted >>= 1;}
    }

    return extracted;
}


/**
 * Finds the shortest path for bringing a single piece inside the piece_mask
 * (in the bitboard) to the rightmost still empty a,b,c,d tile by updating
 * the path history bit board in it's recursive call chain.
 * @param path bit board with all previously taken step digits turned on + current distance
 * @return Shortest path length in units
 */
void handle_piece(int64_t path)
{
    int64_t piece_mask = extract_piece(path, EXTRACT_RIGHTMOST);

    //CHECK IF PATH CAN EVEN STILL BE SHORTER THAN cur_shortest
    int tiles_left = 0;
    int64_t temp_piece_mask = piece_mask;
    while(!(temp_piece_mask&(path&abcd_mask|distance_mask)))
    {
        temp_piece_mask >>= 1;
        tiles_left++;
    }
    tiles_left--;

    if ((path & distance_mask) + tiles_left*min_units_per_tile  > cur_shortest)
    {return;}

    //count down - counting up would cause it to enter branches that
    //will be cut off anyway if a higher roll reaches the goal immediately
    for (int i = 6; i > 0; i--)
    {
        int64_t roll = piece_mask >> i;

        // skip if roll enters last 16 bits or lands on a finished piece
        if ((roll & (distance_mask|path))) {continue;}

        int dist = get_distance(piece_mask, i);

        if (roll & abcd_mask)
        {
            //Check if roll lands on the rightmost empty position
            //i.e piece shifted 1 to the right overlaps with rightmost finished piece or distance mask
            if ((roll >> 1) & (path|distance_mask))
            {
                int new_length = (int) (path & distance_mask) + dist;

                if (new_length < cur_shortest)
                {
                    full_path[full_path_index] = (path | roll) + dist;
                    cur_shortest = new_length;
                }
                return;
            }
        }
        else
        {
            handle_piece((path | roll) + dist); //Handle current rolls subtree
        }
    }
}


/**
 * Find the shortest *physical* path to win, assuming
 * that the pieces can move in straight lines to their rolled
 * positions.
 * Also fills out the the int64_t path bit board.
 *
 * @return Length of shortest path in units
 */
int find_shortest_path()
{
    int path_length = total_movement_to_A; // all piece movements to starting point A combined

    // As explained in the pdf, we only need to perform a
    // search for each of the pieces individually instead of successively
    // as they do not get in the way of each other

    int64_t start_board;
    while (full_path_index < 4)
    {
        switch (full_path_index)
        {
            case 0: start_board = start_p1;break;
            case 1: start_board = start_p2; break;
            case 2: start_board = start_p3; break;
            case 3: start_board = start_p4; break;
            default: break;
        }
        handle_piece(start_board);
        path_length += cur_shortest;
        printf("%d\n", cur_shortest);
        cur_shortest = distance_mask; //RESET
        full_path_index++;
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
    printf("The shortest path on a %dx%d units board is %d units long. \n", side_length_units, side_length_units, units);

    //PATH LENGTH IN CM
    double cm = convert_units_to_cm(units);
    printf("The shortest path on a %dx%d cm board is %f cm long. \n", side_length_cm, side_length_cm, cm);

    //FULL PATH
    print_path();

    return 0;
}