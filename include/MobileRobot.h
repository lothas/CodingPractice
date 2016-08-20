#ifndef MOBILEROBOT_H
#define MOBILEROBOT_H

#include <iostream>
#include <stdlib.h>
#include <array>
#include <math.h>       /* cos */
#include <GL/glu.h>
#include "Matrix2D.h"

enum act_type { MOVE_FWD, ROT_LEFT, ROT_RIGHT };

class MobileRobot
{
    public:
        MobileRobot();
        MobileRobot(unsigned int row, unsigned int col, unsigned int _orientation);
        virtual ~MobileRobot();
        MobileRobot(const MobileRobot& other);

        void set_color(float r, float g, float b);
        void render();

        void set_map(Matrix2D* ext_map);

        bool actuate(act_type action);
        bool move_fwd();
        void rotate_left();
        void rotate_right();

        Matrix2D* sense();
        Matrix2D* rect_sense();

        unsigned int position[2];
        unsigned int orientation;
        Matrix2D* gt_map = nullptr;     // Ground-truth map
        Matrix2D local_map;    			// Locally generated map (not implemented yet)
        // TODO: Implement mapping

        const int sense_dist = 4;
    protected:

    private:
        int directions[4][2] = {{-1,0},{0,1},{1,0},{0,-1}};
//        int directions[8][2] = {{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};
        float verts[10][2];
        float color[3] = {1.0f, 0.0f, 0.0f};
};

#endif // MOBILEROBOT_H
