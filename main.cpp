#include <iostream>
#include <vector>

#include <GL/glut.h>
#include "NavGUI.h"
#include "MobileRobot.h"
#include "A_Star.h"
#include "ParticleFilter.h"
#include "Matrix2D.h"

#define N_PARTICLES 1000

using namespace std;

void map_display();
void sense_display();
void keyboard(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);

void setup_colors(Matrix2D& colors);
void setup_map(Matrix2D& map_mat);

int win_width = 800;
int win_height = 600;
NavGUI nav_gui = NavGUI(win_width, win_height);
NavGUI* NavGUI::this_inst = &nav_gui;

Matrix2D map_mat = Matrix2D(40, 60, 1);
Matrix2D colors = Matrix2D(6, 3, 0);

MobileRobot robot(4, 6, 0);
ParticleFilter pFilt(N_PARTICLES);

A_Star a_star_planner;
int mode = 0;

int main(int argc, char** argv)
{
    setup_colors(colors);
    setup_map(map_mat);

    // Robot initial position
    map_mat.data[35][4] = 4;
    // Add a target
    map_mat.data[2][52] = 5;
//    map_mat.print();

    // Initialize A* planner
    a_star_planner.set_map(&map_mat);
    a_star_planner.set_start(4, 4);
    a_star_planner.set_target(35, 48);

    // Provide robot with physical map
    robot.set_map(&map_mat);

    // Initialize particles
    pFilt.set_robot(&robot);
    pFilt.set_gt_map(&map_mat);
    pFilt.initialize();

    nav_gui.set_ext_display_fcn(map_display);
    nav_gui.set_ext_display_fcn2(sense_display);
    nav_gui.set_ext_keyboard_fcn(keyboard);
    nav_gui.set_ext_special_fcn(specialKeys);
    nav_gui.init(argc, argv);

    return 0;
}

void map_display() {
    nav_gui.drawMatrix(a_star_planner.a_map_mat, &colors);
    robot.render();
    pFilt.render();
}

void sense_display() {
    Matrix2D* sense_mat = robot.rect_sense();
    if (sense_mat) {
//        sense_mat->print();
        nav_gui.drawMatrix(sense_mat, &colors);
    }
}

/* ------------ Keyboard Function ---------- */
void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 'm':           // Move the robot with arrow keys
            mode = 0;
            break;
        case 'p':           // Start planning with A*
            mode = 1;
            break;
        case 's':           // Toggle particle filter render
            pFilt.render_all = !pFilt.render_all;
            break;
    }
}

/* Callback handler for special-key event */
void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_RIGHT:
            switch (mode) {
            case 0:
            	robot.actuate(ROT_RIGHT);
            	pFilt.actuate(ROT_RIGHT);
                break;
            case 1:
                a_star_planner.step_fwd();
                break;
            }
            break;
        case GLUT_KEY_LEFT:
            switch (mode) {
            case 0:
            	robot.actuate(ROT_LEFT);
            	pFilt.actuate(ROT_LEFT);
                break;
            case 1:
                a_star_planner.step_fwd();
                break;
            }
            break;
        case GLUT_KEY_UP:
            switch (mode) {
            case 0:
            	robot.actuate(MOVE_FWD);
            	pFilt.actuate(MOVE_FWD);
            	break;
            }
    }
}

void setup_map(Matrix2D& map_mat) {
    // ################### Set-up map matrix ###################
    for (unsigned int i = 0; i<map_mat.cols; ++i) {
        // Add top and bottom map borders
        map_mat.data[0][i] = 0;
        map_mat.data[map_mat.rows-1][i] = 0;
    }
    for (unsigned int i = 0; i<map_mat.rows; ++i) {
        // Add left and right map borders
        map_mat.data[i][0] = 0;
        map_mat.data[i][map_mat.cols-1] = 0;
    }
    for (unsigned int i = 0; i<map_mat.cols; ++i) {
        // Add hallways
        map_mat.data[8][i] = 0;
        map_mat.data[12][i] = 0;
        map_mat.data[26][i] = 0;
        map_mat.data[30][i] = 0;
    }
    for (unsigned int i = 0; i<8; ++i) {
        // Add room dividers
        map_mat.data[i][6] = 0;
        map_mat.data[i][18] = 0;
        map_mat.data[i][26] = 0;
        map_mat.data[i][40] = 0;
    }
    for (unsigned int i = 12; i<26; ++i) {
        // Add room dividers
        map_mat.data[i][12] = 0;
        map_mat.data[i][32] = 0;
        map_mat.data[i][36] = 0;
    }
    for (unsigned int i = 30; i<map_mat.rows; ++i) {
        // Add room dividers
        map_mat.data[i][10] = 0;
        map_mat.data[i][20] = 0;
        map_mat.data[i][30] = 0;
        map_mat.data[i][45] = 0;
    }
    // Open doors
    map_mat.data[3][6] = 1; map_mat.data[4][6] = 1; map_mat.data[5][6] = 1;
    map_mat.data[8][14] = 1; map_mat.data[8][15] = 1; map_mat.data[8][16] = 1;
    map_mat.data[8][21] = 1; map_mat.data[8][22] = 1; map_mat.data[8][23] = 1;
    map_mat.data[8][21] = 1; map_mat.data[8][22] = 1; map_mat.data[8][23] = 1;
    map_mat.data[8][29] = 1; map_mat.data[8][30] = 1; map_mat.data[8][31] = 1;
    map_mat.data[8][48] = 1; map_mat.data[8][49] = 1; map_mat.data[8][50] = 1;
    map_mat.data[12][8] = 1; map_mat.data[12][9] = 1; map_mat.data[12][10] = 1;
    map_mat.data[12][33] = 1; map_mat.data[12][34] = 1; map_mat.data[12][35] = 1;
    map_mat.data[26][8] = 1; map_mat.data[26][9] = 1; map_mat.data[26][10] = 1;
    map_mat.data[26][28] = 1; map_mat.data[26][29] = 1; map_mat.data[26][30] = 1;
    map_mat.data[26][33] = 1; map_mat.data[26][34] = 1; map_mat.data[26][35] = 1;
    map_mat.data[26][38] = 1; map_mat.data[26][39] = 1; map_mat.data[26][40] = 1;
    map_mat.data[26][55] = 1; map_mat.data[26][56] = 1; map_mat.data[26][57] = 1;
    map_mat.data[30][14] = 1; map_mat.data[30][15] = 1; map_mat.data[30][16] = 1;
    map_mat.data[30][33] = 1; map_mat.data[30][34] = 1; map_mat.data[30][35] = 1;
    map_mat.data[30][55] = 1; map_mat.data[30][56] = 1; map_mat.data[30][57] = 1;
    map_mat.data[32][10] = 1; map_mat.data[33][10] = 1; map_mat.data[34][10] = 1;
    map_mat.data[32][20] = 1; map_mat.data[33][20] = 1; map_mat.data[34][20] = 1;
}

void setup_colors(Matrix2D& colors) {
    // ################### Set-up colors ###################
    // Obstacle color
    colors.data[0][0] = 0; colors.data[0][1] = 0; colors.data[0][2] = 0;
    // Empty color
    colors.data[1][0] = 255; colors.data[1][1] = 255; colors.data[1][2] = 255;
    // Algorithm "seen" color
    colors.data[2][0] = 0; colors.data[2][1] = 0; colors.data[2][2] = 255;
    // Robot path color
    colors.data[3][0] = 60; colors.data[3][1] = 200; colors.data[3][2] = 60;
    // Robot color
    colors.data[4][0] = 120; colors.data[4][1] = 120; colors.data[4][2] = 140;
    // Target color
    colors.data[5][0] = 200; colors.data[5][1] = 200; colors.data[5][2] = 60;
//    colors.print();
}
