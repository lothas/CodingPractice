#ifndef MATRIX2D_H
#define MATRIX2D_H

#include <iostream>
#include <cstring>

class Matrix2D
{
    public:
        Matrix2D(void) {};
        Matrix2D(unsigned int m_rows, unsigned int m_cols, int init_val);
        Matrix2D(const Matrix2D& obj);
        virtual ~Matrix2D();

        bool is_valid_pos(unsigned int row, unsigned int col);
        void print();
        int* get_row(unsigned int row_pos);

        Matrix2D* convolute(Matrix2D* kernel); // TODO: Test that this thing actually works

        int** data = nullptr;
        unsigned int rows = 1;
        unsigned int cols = 1;

    protected:

    private:
};

#endif // MATRIX2D_H
