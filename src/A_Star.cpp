#include "A_Star.h"

A_Star::A_Star(Matrix2D* map_in)
{
    //ctor
    set_map(map_in);
}

A_Star::~A_Star()
{
    //dtor
}

void A_Star::set_map(Matrix2D* map_in) {
    // Make a copy of input map
    if (a_map_mat == nullptr) delete a_map_mat;
    if (a_map_val == nullptr) delete a_map_val;

    a_map_mat = new Matrix2D(map_in->rows, map_in->cols, 0);
    a_map_val = new Matrix2D(map_in->rows, map_in->cols, 0);
    for (unsigned int i = 0; i<a_map_mat->rows; ++i) {
        for (unsigned int j = 0; j<a_map_mat->cols; ++j) {
            a_map_mat->data[i][j] = map_in->data[i][j];
            if (map_in->data[i][j] == 0) a_map_val->data[i][j] = 10000;
        }
    }

    // Set-up priority queue
    pqueue.set_max_nodes(a_map_mat->cols*a_map_mat->rows/5);
}

void A_Star::set_start(unsigned int row, unsigned int col) {
    if (is_valid_pos(row, col)) {
        start[0] = row; start[1] = col;
        a_map_mat->data[row][col] = 4;
        state = 0;
    }
    else std::cout << "Requested start position out of range" << std::endl;
}

void A_Star::set_target(unsigned int row, unsigned int col) {
    if (is_valid_pos(row, col)) {
        target[0] = row; target[1] = col;
        a_map_mat->data[row][col] = 5;
        state = 0;
    }
    else std::cout << "Requested target position out of range" << std::endl;
}

void A_Star::find_path() {
}

bool A_Star::is_valid_pos(unsigned int row, unsigned int col) {
    return a_map_mat->is_valid_pos(row, col);
}

void A_Star::visit_neighbors(unsigned int row, unsigned int col) {
    for (unsigned int i = 0; i<4; ++i) {
        unsigned int new_row = int(row)+directions[i][0];
        unsigned int new_col = int(col)+directions[i][1];
        if (is_valid_pos(new_row, new_col)) {
            if (a_map_mat->data[new_row][new_col] == 5) {
                // Target found
                a_map_val->data[new_row][new_col] = a_map_val->data[row][col] + 1;
                get_path();
                state = 2;
            }
            if (a_map_mat->data[new_row][new_col] == 1) {
                // We found an unvisited neighbor
                a_map_mat->data[new_row][new_col] = 2;
                a_map_val->data[new_row][new_col] = a_map_val->data[row][col] + 1;
                pqueue.push(HeapNode(1./(a_map_val->data[new_row][new_col] + heur(new_row, new_col)),
                            new_row, new_col));
            }
        }
    }
}

float A_Star::heur(unsigned int row, unsigned int col) {
    return abs((int)target[0]-(int)row)+abs((int)target[1]-(int)col);
}

void A_Star::get_path() {
    unsigned int row = target[0];
    unsigned int col = target[1];
    int counter = 0;
    while (++counter < 500) {
        float min_val = 2000;
        IntPoint2D min_dir;
        for (unsigned int i = 0; i<4; ++i) {
            unsigned int new_row = int(row)+directions[i][0];
            unsigned int new_col = int(col)+directions[i][1];
            if (a_map_val->data[new_row][new_col] != 0 &&
                a_map_val->data[new_row][new_col] < min_val) {
                    min_val = a_map_val->data[new_row][new_col];
                    min_dir.y = new_row;
                    min_dir.x = new_col;
                }
        }

        // Append direction to path
        path.push_back(min_dir);
        if (a_map_mat->data[min_dir.y][min_dir.x] == 4) break;
        a_map_mat->data[min_dir.y][min_dir.x] = 3;
        row = min_dir.y;
        col = min_dir.x;
    }
}

void A_Star::step_fwd() {
    if (state == 0) {
        // Start looking for a path
        a_map_val->data[start[0]][start[1]] = 0;
        visit_neighbors(start[0], start[1]);
        state = 1;
    }
    if (state == 1) {
        // Already looking for path,
        // Visit neighbors of next item on queue
        HeapNode* next = pqueue.pop();
        if (next) visit_neighbors(next->pos[0], next->pos[1]);
    }
}

void A_Star::step_bwd() {
    std::cout << "Going back is not implemented yet" << std::endl;
}
