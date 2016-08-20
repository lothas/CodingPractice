#ifndef HEAP_H
#define HEAP_H

#include <iostream>

class HeapNode
{
    public:
        HeapNode();
        HeapNode(float _value, unsigned int row, unsigned int col);
        virtual ~HeapNode();

        float value = 0;
        unsigned int pos[2] = {0,0};

//        HeapNode* parent = nullptr;
//        HeapNode* left = nullptr;
//        HeapNode* right = nullptr;

        void swap(HeapNode* n1, HeapNode* n2);

    protected:

    private:
};

class Heap
{
    public:
        Heap();
        Heap(unsigned int max_nodes);
        virtual ~Heap();
        Heap(const Heap& other);

        void set_max_nodes(unsigned int _max_nodes);
        void print();

        void push(float _value, unsigned int row, unsigned int col);
        HeapNode* pop();
        bool is_empty();

        void bubble_up(unsigned int index);     // recursive
        void bubble_down();                     // while loop

        HeapNode* heap_arr = nullptr;
        unsigned int heap_n = 0;
        unsigned int max_nodes = 0;

    protected:

    private:
};

#endif // HEAP_H
