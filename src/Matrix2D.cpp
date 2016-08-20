#include "../include/Matrix2D.h"

Matrix2D::Matrix2D(unsigned int m_rows, unsigned int m_cols, int init_val)
{
    //ctor
    data = new int *[m_rows];
    for (unsigned int i = 0; i<m_rows; ++i) {
        data[i] = new int[m_cols];
        for (unsigned int j = 0; j<m_cols; ++j) {
            data[i][j] = init_val;
        }
    }
    rows = m_rows;
    cols = m_cols;
}

Matrix2D::Matrix2D(const Matrix2D& obj)
{
    //copy ctor
	rows = obj.rows;
	cols = obj.cols;

    data = new int *[rows];
    for (unsigned int i = 0; i<rows; ++i) {
        data[i] = new int[cols];
        for (unsigned int j = 0; j<cols; ++j) {
            data[i][j] = obj.data[i][j];
        }
    }
}

Matrix2D::~Matrix2D()
{
    //dtor
    delete data;
}

bool Matrix2D::is_valid_pos(unsigned int row, unsigned int col) {
    return (row<rows && col<cols);
}

void Matrix2D::print() {
	if (data == nullptr) return;

    for (unsigned int i = 0; i<rows; ++i) {
        std::cout << "[" << data[i][0];
        for (unsigned int j = 1; j<cols; ++j) {
            std::cout << ", " << data[i][j];
        }
        std::cout << "]" << std::endl;
    }
}

int* Matrix2D::get_row(unsigned int row_pos) {
    if (row_pos < rows) {
        return data[row_pos];
    }
    else {
        return nullptr;
    }
}


Matrix2D* Matrix2D::convolute(Matrix2D* kernel) {
	int drow = -kernel->rows/2;
	int dcol = -kernel->cols/2;

	// Initialize result matrix
	Matrix2D* res = new Matrix2D(rows, cols, 0);

    for (unsigned int i = 0; i<rows; ++i) {
        for (unsigned int j = 0; j<cols; ++j) {
        	// Set element (i,j) of the res matrix to the sum of this*kernel around (i,j)
        	int val = 0;

            for (unsigned int k = 0; k<kernel->rows; ++k) {
                for (unsigned int l = 0; l<kernel->cols; ++l) {
                	if (!is_valid_pos(i+drow+k, j+dcol+l)) continue;
                	// Multiply kernel[k][l] by this[i+drow+k][j+dcol+l]
                	val += kernel->data[k][l]*data[i+drow+k][j+dcol+l];
                }
            }
            res->data[i][j] = val;
        }
    }

	return res;
}
