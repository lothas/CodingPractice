#ifndef SINGLYLLINT_H
#define SINGLYLLINT_H

#include <iostream>
#include "Node.h"

class SinglyLLint
{
    public:
        SinglyLLint();
        virtual ~SinglyLLint();

        Node* head = nullptr;
        Node* tail = nullptr;

        bool is_empty();
        void push_front(IntPoint2D value);
        void push_back(IntPoint2D value);
        IntPoint2D pop_front(bool* res);
        void print();


    protected:

    private:
};

#endif // SINGLYLLINT_H
