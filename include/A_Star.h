#ifndef A_STAR_H
#define A_STAR_H

#include <iostream>
#include <stdlib.h>
#include "Matrix2D.h"
#include "Heap.h"
#include "Node.h"
#include "SinglyLLint.h"

class A_Star
{
    public:
        A_Star(void) {};
        A_Star(Matrix2D* map_in);
        virtual ~A_Star();

        void set_map(Matrix2D* map_in);
        void set_start(unsigned int row, unsigned int col);
        void set_target(unsigned int row, unsigned int col);

        void find_path();
        bool is_valid_pos(unsigned int row, unsigned int col);
        void visit_neighbors(unsigned int row, unsigned int col);
        float heur(unsigned int row, unsigned int col);
        void get_path();

        void step_fwd();
        void step_bwd();

        Matrix2D* a_map_mat = nullptr;
        Matrix2D* a_map_val = nullptr;
        Heap pqueue = Heap();
        SinglyLLint path = SinglyLLint();
        unsigned int state = 0;

    protected:

    private:
        int start[2];
        int target[2];

        int directions[4][2] = {{-1,0},{0,1},{1,0},{0,-1}};
//        int directions[8][2] = {{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};
};

#endif // A_STAR_H
