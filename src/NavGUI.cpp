#include "NavGUI.h"

NavGUI::NavGUI(int width, int height)
{
    //ctor
    win_width = width;
    win_height = height;
}

NavGUI::~NavGUI()
{
    //dtor
}

void NavGUI::set_ext_display_fcn(void (*ptr)(void)) {
    ext_display_fcn = ptr;
}

void NavGUI::set_ext_display_fcn2(void (*ptr)(void)) {
    ext_display_fcn2 = ptr;
}

void NavGUI::set_ext_keyboard_fcn(void (*ptr)(unsigned char key, int x, int y)) {
    ext_keyboard_fcn = ptr;
}

void NavGUI::set_ext_special_fcn(void (*ptr)(int key, int x, int y)) {
    ext_special_fcn = ptr;
}

/* --------- Initialize OpenGL Graphics --------- */
void NavGUI::init(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);  // Enable double buffered mode

    // Create sense window
    glutInitWindowSize(200, 200);   // Set the window's initial width & height
    glutInitWindowPosition(920, 450); // Position the window's initial top-left corner
    sense_win = glutCreateWindow("Robot sense");
    glutDisplayFunc(NavGUI::display2);
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f); // Gray and opaque

    // Create map window
    glutInitWindowSize(win_width, win_height);   // Set the window's initial width & height
    glutInitWindowPosition(120, 50); // Position the window's initial top-left corner
    map_win = glutCreateWindow("Mobile navigation");

    // Set-up function pointers
    glutKeyboardFunc(NavGUI::keyboard);
    glutSpecialFunc(NavGUI::specialKeys);   // Register callback handler for special-key event
    glutDisplayFunc(NavGUI::display);
    glutReshapeFunc(NavGUI::reshape);       // Register callback handler for window re-size event
    glutIdleFunc(NavGUI::idle);             // Register callback handler if no other event
//    glutIdleFunc(NavGUI::display);

    // Initialize OpenGL configuration
    // Set "clearing" or background color
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f); // Gray and opaque

    // Start running GLUT
    glutMainLoop();
}

/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void NavGUI::display() {
    glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color

    glMatrixMode(GL_MODELVIEW);      // To operate on Model-View matrix
    glLoadIdentity();                // Reset the model-view matrix

//    drawSamples();

    if (this_inst->ext_display_fcn) this_inst->ext_display_fcn();

//    glFlush();  // Render now
    glutSwapBuffers();   // Double buffered - swap the front and back buffers
}

/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void NavGUI::display2() {
    glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color

    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset the projection matrix
    gluOrtho2D(0, 1.0, 0, 1.0);

    glMatrixMode(GL_MODELVIEW);      // To operate on Model-View matrix
    glLoadIdentity();                // Reset the model-view matrix

//    drawSamples();

    if (this_inst->ext_display_fcn2) this_inst->ext_display_fcn2();

//    glFlush();  // Render now
    glutSwapBuffers();   // Double buffered - swap the front and back buffers
}

void NavGUI::idle(void) {
	glutSetWindow( this_inst->sense_win );
	glutPostRedisplay();

	glutSetWindow( this_inst->map_win );
	glutPostRedisplay();
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void NavGUI::reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent division by 0

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset the projection matrix
    gluOrtho2D(0, 1.0, 0, 1.0);
//   GLfloat aspect = (GLfloat)width / (GLfloat)height;
//   if (width >= height) {
//     // aspect >= 1, set the height from -1 to 1, with larger width
//      gluOrtho2D(0, 1.0 * aspect, 0, 1.0);
//   } else {
//      // aspect < 1, set the width to -1 to 1, with larger height
//     gluOrtho2D(0, 1.0, 0, 1.0 / aspect);
//   }

    // Store values
    this_inst->win_height = height;
    this_inst->win_width = width;
}

/* ------------ GLUT Keyboard Function ---------- */
void NavGUI::keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case '\x1B':  // Escape
            exit(EXIT_SUCCESS);
            break;
    }
    if (this_inst->ext_keyboard_fcn) this_inst->ext_keyboard_fcn(key, x, y);
}

/* Callback handler for special-key event */
void NavGUI::specialKeys(int key, int x, int y) {
    if (this_inst->ext_special_fcn) this_inst->ext_special_fcn(key, x, y);
}

void NavGUI::drawSamples() {
    // Define shapes enclosed within a pair of glBegin and glEnd
   glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
      glColor3f(1.0f, 0.0f, 0.0f); // Red
      glVertex2f(-0.8f, 0.1f);     // Define vertices in counter-clockwise (CCW) order
      glVertex2f(-0.2f, 0.1f);     //  so that the normal (front-face) is facing you
      glVertex2f(-0.2f, 0.7f);
      glVertex2f(-0.8f, 0.7f);

      glColor3f(0.0f, 1.0f, 0.0f); // Green
      glVertex2f(-0.7f, -0.6f);
      glVertex2f(-0.1f, -0.6f);
      glVertex2f(-0.1f,  0.0f);
      glVertex2f(-0.7f,  0.0f);

      glColor4f(0.2f, 0.2f, 0.2f, 0.5f); // Dark Gray
      glVertex2f(-0.9f, -0.7f);
      glVertex2f(-0.5f, -0.7f);
      glColor4f(1.0f, 1.0f, 1.0f, 0.5f); // White
      glVertex2f(-0.5f, -0.3f);
      glVertex2f(-0.9f, -0.3f);
   glEnd();

   glBegin(GL_TRIANGLES);          // Each set of 3 vertices form a triangle
      glColor3f(0.0f, 0.0f, 1.0f); // Blue
      glVertex2f(0.1f, -0.6f);
      glVertex2f(0.7f, -0.6f);
      glVertex2f(0.4f, -0.1f);

      glColor3f(1.0f, 0.0f, 0.0f); // Red
      glVertex2f(0.3f, -0.4f);
      glColor3f(0.0f, 1.0f, 0.0f); // Green
      glVertex2f(0.9f, -0.4f);
      glColor3f(0.0f, 0.0f, 1.0f); // Blue
      glVertex2f(0.6f, -0.9f);
   glEnd();

   glBegin(GL_POLYGON);            // These vertices form a closed polygon
      glColor3f(1.0f, 1.0f, 0.0f); // Yellow
      glVertex2f(0.4f, 0.2f);
      glVertex2f(0.6f, 0.2f);
      glVertex2f(0.7f, 0.4f);
      glVertex2f(0.6f, 0.6f);
      glVertex2f(0.4f, 0.6f);
      glVertex2f(0.3f, 0.4f);
   glEnd();
}

void NavGUI::drawMatrix(Matrix2D* Mat, Matrix2D* colors) {
    // Draws a 1x1 matrix with pixels colored by numbers
    float gap = 0.1f/float(std::max(Mat->rows, Mat->cols));
    float height = (1.-float(Mat->rows+1)*gap)/float(Mat->rows);
    float width = (1.-float(Mat->cols+1)*gap)/float(Mat->cols);
    float x = gap;
    float y = 1.0-gap;

    glBegin(GL_QUADS);
    for (unsigned int i = 0; i<Mat->rows; ++i) {
        for (unsigned int j = 0; j<Mat->cols; ++j) {
            // Draw a pixel colored by the number in Mat
            int* pix_color = colors->get_row(Mat->data[i][j]);
            glColor3f(float(pix_color[0])/255., float(pix_color[1])/255., float(pix_color[2])/255.);
            glVertex2f(x, y);
            glVertex2f(x, y-height);
            glVertex2f(x+width, y-height);
            glVertex2f(x+width, y);
            x += width + gap;
        }
        x = gap;
        y -= height + gap;
    }
    glEnd();
}

