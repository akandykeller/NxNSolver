/* 
 *  N by N Snake Cube Puzzle Solver
 *  By Andy Keller
 *  December 2014
 *
 */

#include "NxNSolver.h"
 
// The segment lengths are recorded below for each puzzle. 
// Note that each length overlaps with the next segment by 1 block.
// 4x4
#define NUM_SEGS 46
int segment_lengths[] = {2,4,2,2,2,2,2,2,2,2,
                         2,3,2,4,2,2,2,4,3,2,
                         2,2,2,2,3,3,2,2,2,2,
                         2,2,2,2,3,2,3,2,3,2,
                         4,2,2,3,2,3};        

// 3x3
//#define NUM_SEGS 17
//int segment_lengths[] = {3, 2, 2, 3, 2, 3, 
//                         2, 2, 3, 3, 2, 2, 
//                         2, 3, 3, 3, 3};

// 2x2 
//#define NUM_SEGS 7
//int segment_lengths[] =  {2, 2, 2, 2, 2, 2, 2};

// Initalize list of unit direction vectors 
int possible_dir[6][3] = {{1, 0, 0},
                          {-1, 0, 0},
                          {0, 1, 0},
                          {0, -1, 0},
                          {0, 0, 1},
                          {0, 0, -1}};
// Count of moves attempted for progress
int num_moves_attempted = 0;
// Count of solutions found
int num_solutions = 0;

/*
 * Initializes a linked list of Segment objects, returning a pointer to the head.
 */
Segment* initialize_segment_chain() {
    // Set first segment
    Segment* prev_segment = new Segment(segment_lengths[0]);
    // Set its previous pointer to null
    prev_segment->last_segment = NULL;

    Segment* current_segment;

    // Go through all segments and set their next & last pointers
    for (int i = 1; i < NUM_SEGS; i++) {
        current_segment = new Segment(segment_lengths[i]);

        current_segment->last_segment = prev_segment;
        prev_segment->next_segment = current_segment;
        prev_segment = current_segment;
    }
    // Set the last segments next pointer to null
    current_segment->next_segment = NULL;
    // Search back through the list to the beginning
    while(current_segment->last_segment != NULL) {
        current_segment = current_segment->last_segment;
    }
    // return pointer to head.
    return current_segment;
}

/*
 * Recursive Depth First Search function.
 * 
 * DFS takes the current cube state and the current segment to be placed as 
 * arguments and recursively calls DFS on the next segment for all possible 
 * placements of the current segment. True is returned upwards if the cube
 * is filled with a valid configuration.
 * 
 * Args:
 *  Cube_Space cube: A copy of the current cube object.
 *  Segment* current_segment: The current segment to be placed.
 * Returns:
 *  bool: result of DFS search. true -> solved. 
 */
bool DFS(Cube_Space cube, Segment* current_segment) {
    // To display progress
    if (num_moves_attempted % 1000000 == 0) {
         printf("%d moves attempted.\n", num_moves_attempted);
    }

    // Iterate over possible positions for the current segment given the last segment.
    dir_list possible_dirs;

    // Fill possible_dirs
    orth_directions(current_segment->last_segment->direction,
                    &possible_dirs);

    for (int i = 0; i < 4; i++) {
        // If placing this segment in this position does not fall outside 
        // the bounds or self-intersect
        if (can_place(&cube, current_segment, possible_dirs[i])) {
            // printf("Placing length %d segment at (%d, %d, %d) w/ direction (%d, %d, %d).\n",
            //          current_segment->length, 
            //          current_segment->tail.x,
            //          current_segment->tail.y,
            //          current_segment->tail.z,
            //          possible_dirs[i][0],
            //          possible_dirs[i][1],
            //          possible_dirs[i][2]);

            // Place the segment in the cube in the given direction. Set current_segment's 
            // appropriate attributes.
            cube.place(current_segment, possible_dirs[i]);
            num_moves_attempted++;

            // Check end of search
            if (cube.is_full()) {
                return true;
            }

            // If no, recurse on next segment
            if(DFS(cube, current_segment->next_segment) == true){
                return true;
            }

            // undo
            cube.un_place(current_segment, possible_dirs[i]);
        }
    }

    return false;
}


int main(int argc, char **argv) {
    Cube_Space cube;
    Segment* segment_0 = initialize_segment_chain();

    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            for (int z = 0; z < N; z++) {
                Point pos(x, y, z);
                int dir[3];

                segment_0->tail = pos;
                cube.filled[pos.x][pos.y][pos.z] = true;

                for (int dir_idx = 0; dir_idx < 6; dir_idx++) {
                     dir[0] = possible_dir[dir_idx][0];
                     dir[1] = possible_dir[dir_idx][1];
                     dir[2] = possible_dir[dir_idx][2];                     

                    printf("Trying initial piece at (%d, %d, %d) w/ dir: (%d, %d, %d)\n", x, y, z, dir[0], dir[1], dir[2]);

                    if (can_place(&cube, segment_0, dir)) {
                       // printf("Piece works, recursing.\n");
                        cube.place(segment_0, dir);
                        bool res = DFS(cube, segment_0->next_segment);
                        if (res == true) {
                            printf("Done!\n");

                            Segment* current_segment = segment_0;

                            int i = 0;
                            while (i < NUM_SEGS) {
                                printf("Place segment %d, of length %d, at position (%d, %d, %d) in direction (%d, %d, %d)\n", i, current_segment->length,
                                        current_segment->tail.x, current_segment->tail.y, current_segment->tail.z,
                                        current_segment->direction[0], current_segment->direction[1], current_segment->direction[2]);
                                
                                current_segment = current_segment->next_segment;
                                i++;
                            }

                            printf("Total of %d moves attempted.\n", num_moves_attempted);
                            num_solutions++;
                        }
                        cube.un_place(segment_0, dir);
                    }
                }

                cube.filled[pos.x][pos.y][pos.z] = false;
            }
        }
    }

    printf("All possible positions searched. %d solutions found.\n", num_solutions);
    return 0;
}


