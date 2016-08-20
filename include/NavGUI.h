#ifndef NAVGUI_H
#define NAVGUI_H

#include <iostream>
#include <GL/glut.h>
#include <GL/glu.h>
#include "Matrix2D.h"

class NavGUI
{
    public:
        NavGUI(int width, int height);
        virtual ~NavGUI();

        void set_ext_display_fcn(void (*ptr)(void));
        void set_ext_display_fcn2(void (*ptr)(void));
        void set_ext_keyboard_fcn(void (*ptr)(unsigned char key, int x, int y));
        void set_ext_special_fcn(void (*ptr)(int key, int x, int y));
        void init(int argc, char** argv);

        void drawMatrix(Matrix2D* Mat, Matrix2D* colors);

        static NavGUI* this_inst;

    protected:

    private:
        static void display();
        static void display2();
        static void idle(void);
        static void reshape(GLsizei width, GLsizei height);
        static void keyboard(unsigned char key, int x, int y);
        static void specialKeys(int key, int x, int y);

        void drawSamples();

        void (*ext_display_fcn)(void) = nullptr;
        void (*ext_display_fcn2)(void) = nullptr;
        void (*ext_keyboard_fcn)(unsigned char key, int x, int y) = nullptr;
        void (*ext_special_fcn)(int key, int x, int y) = nullptr;

        GLsizei map_win;
        GLsizei sense_win;
        GLsizei win_height;
        GLsizei win_width;
};

#endif // NAVGUI_H
