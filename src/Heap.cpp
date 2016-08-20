#include "Heap.h"

HeapNode::HeapNode()
{
    //ctor
    value = 0;
    pos[0] = 0;
    pos[1] = 0;
}

HeapNode::HeapNode(float _value, unsigned int row, unsigned int col)
{
    //ctor
    value = _value;
    pos[0] = row;
    pos[1] = col;
}

HeapNode::~HeapNode()
{
    //dtor
}

void swap(HeapNode* n1, HeapNode* n2) {
    HeapNode* tmp = new HeapNode(n1->value, n1->pos[0], n1->pos[1]);
    n1->value = n2->value; n1->pos[0] = n2->pos[0]; n1->pos[1] = n2->pos[1];
    n2->value = tmp->value; n2->pos[0] = tmp->pos[0]; n2->pos[1] = tmp->pos[1];
    delete tmp;
}

Heap::Heap()
{
    //ctor
}

Heap::Heap(unsigned int _max_nodes)
{
    //ctor
    heap_arr = new HeapNode[_max_nodes];
    heap_n = 0;
    max_nodes = _max_nodes;
}

Heap::~Heap()
{
    //dtor
}

Heap::Heap(const Heap& other)
{
    //copy ctor
}

void Heap::set_max_nodes(unsigned int _max_nodes) {
    if (heap_arr) delete heap_arr;
    heap_arr = new HeapNode[_max_nodes];
    max_nodes = _max_nodes;
}

void Heap::print() {
    if (is_empty()) {
        std::cout << "Heap is empty" << std::endl;
        return;
    }

    std::cout << "[" << heap_arr[0].value;
    for (unsigned int i = 1; i<heap_n; ++i) {
        std::cout << ", " << heap_arr[i].value;
    }
    std::cout << "]" << std::endl;
}

void Heap::push(float _value, unsigned int row, unsigned int col) {
    if (heap_n >= max_nodes) {
        std::cout << "ERROR! Heap is full" << std::endl;
        return;
    }

//    std::cout << "Adding new node(" << _value << ", " << row << ", " << col << ") at pos.: " << heap_n << std::endl;
    heap_arr[heap_n].value = _value;
    heap_arr[heap_n].pos[0] = row;
    heap_arr[heap_n].pos[1] = col;

    bubble_up(heap_n);
    ++heap_n;
}

HeapNode* Heap::pop() {
    HeapNode* tmp = new HeapNode(heap_arr[0].value, heap_arr[0].pos[0], heap_arr[0].pos[1]);
    if (is_empty()) return nullptr;
    if (heap_n == 1) {
        --heap_n;
        return tmp;
    }
    else {
        swap(&heap_arr[0], &heap_arr[heap_n-1]);
        --heap_n;
        bubble_down();
        return tmp;
    }
}

bool Heap::is_empty() {
    return heap_n <= 0;
}

void Heap::bubble_up(unsigned int index) {
    if (index <= 0) return;

    unsigned int parent = (index-1)/2;
    if (heap_arr[index].value > heap_arr[parent].value) {
        swap(&heap_arr[index], &heap_arr[parent]);
        bubble_up(parent);
    }
}

void Heap::bubble_down() {
    unsigned int index = 0;
    while (true) {
        // Get children
        unsigned int left = 2*index + 1;
        unsigned int right = 2*index + 2;

        if (left >= heap_n) return;

        // Check if the heap property is maintained
        unsigned int max_index = index;
        if (heap_arr[left].value > heap_arr[index].value) max_index = left;

        if (right < heap_n) {
            if (heap_arr[right].value > heap_arr[max_index].value) max_index = right;
        }

        if (max_index != index) {
            // Swap with largest
            swap(&heap_arr[max_index], &heap_arr[index]);
            index = max_index;
        }
        else return;
    }
}
