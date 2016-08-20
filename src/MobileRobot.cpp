#include "MobileRobot.h"

#define CIRCLE_RES  10.0

MobileRobot::MobileRobot()
{
    //ctor
    position[0] = 1;
    position[1] = 1;
    orientation = 0;
    for (unsigned int i = 0; i < (int)CIRCLE_RES; ++i) {
        verts[i][0] = 0.5f*cos((float)i/CIRCLE_RES*6.2832);
        verts[i][1] = 0.5f*sin((float)i/CIRCLE_RES*6.2832);
    }
}

MobileRobot::MobileRobot(unsigned int row, unsigned int col, unsigned int _orientation)
{
    //ctor
    position[0] = row;
    position[1] = col;
    orientation = _orientation;
    for (unsigned int i = 0; i < (int)CIRCLE_RES; ++i) {
        verts[i][0] = 0.5f*cos((float)i/CIRCLE_RES*6.2832);
        verts[i][1] = 0.5f*sin((float)i/CIRCLE_RES*6.2832);
    }
}

MobileRobot::~MobileRobot()
{
    //dtor
}

MobileRobot::MobileRobot(const MobileRobot& other)
{
    //copy ctor
    position[0] = other.position[0];
    position[1] = other.position[1];
    orientation = other.orientation;
    for (unsigned int i = 0; i < (int)CIRCLE_RES; ++i) {
        verts[i][0] = 0.5f*cos((float)i/CIRCLE_RES*6.2832);
        verts[i][1] = 0.5f*sin((float)i/CIRCLE_RES*6.2832);
    }
    set_map(other.gt_map);      // Ground-truth map
    color[0] = other.color[0];
    color[1] = other.color[1];
    color[2] = other.color[2];
//    std::cout << "Copy constructor called" << std::endl;
}

void MobileRobot::set_color(float r, float g, float b) {
	color[0] = r; color[1] = g; color[2] = b;
}

void MobileRobot::render() {
    if (!gt_map) return; // can't render without physical map
    glPushMatrix();
        glScalef(1.0f/(float)gt_map->cols, 1.0f/(float)gt_map->rows, 1.0f);
        glTranslatef((float)position[1]+0.5f,
                     float(gt_map->rows - 1 - position[0])+0.5f, 0);
        glRotatef(-90.0f+(float)orientation*90.0f, 0.0f, 0.0f, -1.0f);
        // Draw body
        glBegin(GL_TRIANGLE_FAN);
            glColor3f(color[0], color[1], color[2]);
            glVertex2f(0.0f, 0.0f);
            for (unsigned int i = 0; i < (int)CIRCLE_RES; ++i) {
                glVertex2f(verts[i][0], verts[i][1]);
            }
            glVertex2f(verts[0][0], verts[0][1]);
        glEnd();
        // Draw arrow
        glBegin(GL_TRIANGLES);
            glColor3f(0.0f, 0.0f, 0.0f);
            glVertex2f(0.35f, 0.0f);
            glVertex2f(-0.3f, 0.25f);
            glVertex2f(-0.3f, -0.25f);
        glEnd();
    glPopMatrix();
}

void MobileRobot::set_map(Matrix2D* ext_map) {
    // Save pointer to external map
    gt_map = ext_map;

    // Create new local map
//    local_map = Matrix2D(*ext_map);
//    if (local_map) delete local_map;
//    local_map = new Matrix2D(ext_map->rows, ext_map->cols, 1);
}

bool MobileRobot::actuate(act_type action) {
	switch (action) {
	case MOVE_FWD:
		return move_fwd();
	case ROT_LEFT:
		rotate_left();
		return true;
	case ROT_RIGHT:
		rotate_right();
		return true;
	default:
		return false;
	}
}

bool MobileRobot::move_fwd() {
    if (!gt_map) return false; // can't move without physical map

    // Calculate new position
    unsigned int new_row = position[0] + directions[orientation][0];
    unsigned int new_col = position[1] + directions[orientation][1];
    if (gt_map->is_valid_pos(new_row, new_col)) {
        if (gt_map->data[new_row][new_col] != 0) {
            position[0] = new_row;
            position[1] = new_col;
//            std::cout << "Robot position: (" << position[0] << ", " << position[1] << ")" << std::endl;
            return true;
        }
        else return false;
    }
    else return false;
}

void MobileRobot::rotate_left() {
//    if (orientation == 0) orientation = 7;
    if (orientation == 0) orientation = 3;
    else --orientation;
}

void MobileRobot::rotate_right() {
    ++orientation;
    if (orientation>3) orientation = 0;
//    if (orientation>7) orientation = 0;
}

Matrix2D* MobileRobot::sense() {
    // We'll use 2 ("seen") as a position not sensed
    unsigned int n_rows = 0;
    unsigned int n_cols = 0;
    switch (orientation) {
    case 0:     // {-1,0}
    case 2:     // {1,0}
        n_rows = sense_dist;
        n_cols = 2*sense_dist-1;
        break;
    case 1:     // {0,1}
    case 3:     // {0,-1}
        n_rows = 2*sense_dist-1;
        n_cols = sense_dist;
        break;
    }
    Matrix2D* sense_mat = new Matrix2D(n_rows, n_cols, 2);

    for (unsigned int i = 0; i<sense_mat->rows; ++i) {
        for (unsigned int j = 0; j<sense_mat->cols; ++j) {
            int x = 0;
            int y = 0;

            switch (orientation) {
            case 0:     // {-1,0}
                x += -sense_dist+1+j;
                y += -sense_dist+1+i;
                break;
            case 1:     // {0,1}
                x += j;
                y += -sense_dist+1+i;
                break;
            case 2:     // {1,0}
                x += -sense_dist+1+j;
                y += i;
                break;
            case 3:     // {0,-1}
                x += -sense_dist+1+j;
                y += -sense_dist+1+i;
                break;
            }
            if (abs(x)+abs(y)>sense_dist) continue;
            if (gt_map->is_valid_pos(position[0]+y,position[1]+x)) {
                sense_mat->data[i][j] = gt_map->data[position[0]+y][position[1]+x];
            }
        }
    }

    switch (orientation) {
    case 0:     // {-1,0}
        sense_mat->data[sense_dist-1][sense_dist-1] = 4; // robot position
        break;
    case 1:     // {0,1}
        sense_mat->data[sense_dist-1][0] = 4; // robot position
        break;
    case 2:     // {1,0}
        sense_mat->data[0][sense_dist-1] = 4; // robot position
        break;
    case 3:     // {0,-1}
        sense_mat->data[sense_dist-1][sense_dist-1] = 4; // robot position
        break;
    }
    return sense_mat;
}

Matrix2D* MobileRobot::rect_sense() {
	// Rectified sense
	// Returns the sense data unaffected by orientation

    // We'll use 2 ("seen") as a position not sensed
    Matrix2D* sense_mat = new Matrix2D(sense_dist, 2*sense_dist-1, 2);

    int row_delta = 0, col_delta = 0;
    std::array<int, 2> row_dir, col_dir;
    switch (orientation) {
    case 0:     // {-1,0}
    	row_dir = {1, 0};
    	col_dir = {0, 1};
    	row_delta = -(sense_dist-1);
    	col_delta = -(sense_dist-1);
        break;
    case 1:     // {0,1}
    	row_dir = {0,-1};
    	col_dir = {1,0};
    	row_delta = -(sense_dist-1);
    	col_delta = (sense_dist-1);
    	break;
    case 2:     // {1,0}
    	row_dir = {-1, 0};
    	col_dir = {0, -1};
    	row_delta = (sense_dist-1);
    	col_delta = (sense_dist-1);
        break;
    case 3:     // {0,-1}
    	row_dir = {0,1};
    	col_dir = {-1,0};
    	row_delta = (sense_dist-1);
    	col_delta = -(sense_dist-1);
        break;
    }

    int x = position[1]+col_delta, y = position[0]+row_delta;
    for (unsigned int i = 0; i<sense_mat->rows; ++i) {
        for (unsigned int j = 0; j<sense_mat->cols; ++j) {
            if (abs(x-position[1])+abs(y-position[0])<=sense_dist && gt_map->is_valid_pos(y, x)) {
                sense_mat->data[i][j] = gt_map->data[y][x];
            }
            x += col_dir[1];
            y += col_dir[0];
        }
        x -= col_dir[1]*sense_mat->cols;
        y -= col_dir[0]*sense_mat->cols;
        x += row_dir[1];
        y += row_dir[0];
    }

    sense_mat->data[sense_dist-1][sense_dist-1] = 4; // robot position
    return sense_mat;
}
