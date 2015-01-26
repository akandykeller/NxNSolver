#include <cstdio>

#define N 4

typedef int dir_list[4][3];

class Point {
    public:
        int x, y, z;
        Point(int, int, int);
        Point add(Point p2);
        Point mul(int len);

};

// Initializer, set coords 
Point::Point(int x_giv = 0, int y_giv = 0, int z_giv = 0) {
    x = x_giv;
    y = y_giv;
    z = z_giv;
}


// Return addition of coordinates of two points
Point Point::add(Point p2) {
    Point res;

    res.x = x + p2.x;
    res.y = y + p2.y;
    res.z = z + p2.z;

    return res;
}

// Return point equal to multiplication of current points position by 
// given length
Point Point::mul(int len) {
    Point res;

    res.x = x * len;
    res.y = y * len;
    res.z = z * len;

    return res;
}


class Segment {
    public:
        int length;
        Point head, tail;
        int direction[3];
        Segment* last_segment;
        Segment* next_segment;
        Segment(int);
        //Point* possible_head_pos();
        //int** possible_directions();
        //int* segment_direction();
};

// Initializer, set length 
Segment::Segment(int len) {
    length = len;
}


// return all possible directions orthogonal to the current direction
void orth_directions(int* direction, dir_list* orth_directions) {
    int j = 0;

    for (int axis = 0; axis < 3; axis++) {
        if (direction[axis] == 0) {
            int temp_dir[3] = {0, 0, 0};

            temp_dir[axis] = 1;
            (*orth_directions)[j][0] = temp_dir[0];
            (*orth_directions)[j][1] = temp_dir[1];
            (*orth_directions)[j][2] = temp_dir[2];
            j++;

            temp_dir[axis] = -1;
            (*orth_directions)[j][0] = temp_dir[0];
            (*orth_directions)[j][1] = temp_dir[1];
            (*orth_directions)[j][2] = temp_dir[2];

            j++;
        }        
    }
}


class Cube_Space {
    public:
        int size;
        bool filled[N][N][N];
        Cube_Space();
        bool is_filled(Point pos);
        void place(Segment* seg, int* direction);
        void un_place(Segment* seg, int* dir);
        bool is_full();
};

// Initialize the cube space as entirely empty
Cube_Space::Cube_Space() {
    for (int i=0; i < N; i++) {
        for (int j=0; j < N; j++) {
            for (int k=0; k < N; k++) {
                filled[i][j][k] = 0;
            }
        }
    }
}

bool can_place(Cube_Space *cube, Segment* current_segment, int* direction){
    int offset = 1;

    do {
        Point added_pos(offset * direction[0],
                        offset * direction[1],
                        offset * direction[2]);

        if(cube->is_filled(current_segment->tail.add(added_pos))) {
            //printf("Spot filled, can't play there, trying another direction.\n");
            return false;
        }

        offset++;
    } while (offset < current_segment->length);

    return true;
}

// Checks if the given position is empty and on the board
bool Cube_Space::is_filled(Point pos) {
    if (pos.x >= 0 && pos.x < N &&
        pos.y >= 0 && pos.y < N &&
        pos.z >= 0 && pos.z < N) {
        //printf("Space (%d, %d, %d) = %d\n", pos.x, pos.y, pos.z, filled[pos.x][pos.y][pos.z]);
        return filled[pos.x][pos.y][pos.z];
    }

    //printf("Space (%d, %d, %d) off board\n", pos.x, pos.y, pos.z);
    return true;
}

// Places the given segment with direction in the cube,
// and sets the appropriate attributes.
void Cube_Space::place(Segment* seg, int* dir) {
    int offset = 1;
    Point temp_pos; 

    do {
        Point added_pos(offset * dir[0],
                        offset * dir[1],
                        offset * dir[2]);

        temp_pos = seg->tail.add(added_pos);
    
        filled[temp_pos.x][temp_pos.y][temp_pos.z] = true;

        offset++;
    } while (offset < seg->length);

    seg->direction[0] = dir[0];
    seg->direction[1] = dir[1];
    seg->direction[2] = dir[2];

    seg->head = temp_pos;
    if (seg->next_segment != NULL) {
        seg->next_segment->tail = temp_pos;
    }
}

// Places the given segment with direction in the cube,
// and sets the appropriate attributes.
void Cube_Space::un_place(Segment* seg, int* dir) {
    int offset = 1;

    //printf("Undoing a move.\n");

    do {
        Point added_pos(offset * dir[0],
                        offset * dir[1],
                        offset * dir[2]);

        Point temp_pos = seg->tail.add(added_pos);
    
        filled[temp_pos.x][temp_pos.y][temp_pos.z] = false;

        offset++;
    } while (offset < seg->length);

    //seg->direction = NULL;
    //seg->head = NULL;
}


bool Cube_Space::is_full() {
    for (int i=0; i < N; i++) {
        for (int j=0; j < N; j++) {
            for (int k=0; k < N; k++) {
                if (filled[i][j][k] != true) {
                    return false;
                }
            }
        }
    }

    return true;
}


