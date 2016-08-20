#ifndef HEAP_H
#define HEAP_H

#include <iostream>

class HeapNode
{
    public:
        HeapNode();
        HeapNode(float _score, unsigned int row, unsigned int col);
        HeapNode(const HeapNode& obj);
        virtual ~HeapNode();

        void operator=(const HeapNode& obj);

        float score = 0;
        unsigned int pos[2] = {0,0};

//        HeapNode* parent = nullptr;
//        HeapNode* left = nullptr;
//        HeapNode* right = nullptr;

        void swap(HeapNode* n1, HeapNode* n2);

    protected:

    private:
};

template <typename T>
class Heap
{
    public:
        Heap();
        Heap(unsigned int max_nodes);
        virtual ~Heap();
        Heap(const Heap& other);

        void set_max_nodes(unsigned int _max_nodes);
        void print();

        void push(T const& new_node);
        T* at(unsigned int i);
        T* pop();
        bool is_empty();

        void swap(T& el1, T& el2);
        void bubble_up(unsigned int index);     // recursive
        void bubble_down(unsigned int index);   // while loop
        void heapify();

        T* heap_arr = nullptr;
        unsigned int heap_n = 0;
        unsigned int max_nodes = 0;

    protected:

    private:
};


template <typename T>
Heap<T>::Heap()
{
    //ctor
}

template <typename T>
Heap<T>::Heap(unsigned int _max_nodes)
{
    //ctor
    heap_arr = new T[_max_nodes];
    heap_n = 0;
    max_nodes = _max_nodes;
}

template <typename T>
Heap<T>::~Heap()
{
    //dtor
}

template <typename T>
Heap<T>::Heap(const Heap& other)
{
    //copy ctor
    max_nodes = other.max_nodes;
    heap_arr = new T[other.max_nodes];
    heap_n = other.heap_n;
}

template <typename T>
void Heap<T>::set_max_nodes(unsigned int _max_nodes) {
    if (heap_arr) delete heap_arr;
    heap_arr = new T[_max_nodes];
    max_nodes = _max_nodes;
}

template <typename T>
void Heap<T>::print() {
    if (is_empty()) {
        std::cout << "Heap is empty" << std::endl;
        return;
    }

    std::cout << "[" << heap_arr[0].score;
    for (unsigned int i = 1; i<heap_n; ++i) {
        std::cout << ", " << heap_arr[i].score;
    }
    std::cout << "]" << std::endl;
}

template <typename T>
void Heap<T>::push(T const& new_node) {
    if (heap_n >= max_nodes) {
        std::cout << "ERROR! Heap is full" << std::endl;
        return;
    }

//    std::cout << "Adding new node(" << _value << ", " << row << ", " << col << ") at pos.: " << heap_n << std::endl;
    heap_arr[heap_n] = new_node;

    bubble_up(heap_n);
    ++heap_n;
}

template <typename T>
T* Heap<T>::at(unsigned int i) {
	if (i<heap_n) return &heap_arr[i];
	else return nullptr;
}

template <typename T>
T* Heap<T>::pop() {
    if (is_empty()) return nullptr;
    T* tmp = new T(heap_arr[0]);
    if (heap_n == 1) {
        --heap_n;
        return tmp;
    }
    else {
        swap(heap_arr[0], heap_arr[heap_n-1]);
        --heap_n;
        bubble_down(0);
        return tmp;
    }
}

template <typename T>
bool Heap<T>::is_empty() {
    return heap_n <= 0;
}

template <typename T>
void Heap<T>::swap(T& el1, T& el2) {
	T temp(el2);
	el2 = el1;
	el1 = temp;
}

template <typename T>
void Heap<T>::bubble_up(unsigned int index) {
    if (index <= 0) return;

    unsigned int parent = (index-1)/2;
    if (heap_arr[index].score > heap_arr[parent].score) {
        swap(heap_arr[index], heap_arr[parent]);
        bubble_up(parent);
    }
}

template <typename T>
void Heap<T>::bubble_down(unsigned int index) {
    while (true) {
        // Get children
        unsigned int left = 2*index + 1;
        unsigned int right = 2*index + 2;

        if (left >= heap_n) return;

        // Check if the heap property is maintained
        unsigned int max_index = index;
        if (heap_arr[left].score > heap_arr[index].score) max_index = left;

        if (right < heap_n) {
            if (heap_arr[right].score > heap_arr[max_index].score) max_index = right;
        }

        if (max_index != index) {
            // Swap with largest
            swap(heap_arr[max_index], heap_arr[index]);
            index = max_index;
        }
        else return;
    }
}

template <typename T>
void Heap<T>::heapify() {
	for (unsigned int i = 0; i<heap_n; ++i) {
		bubble_down(i);
	}
}

#endif // HEAP_H
