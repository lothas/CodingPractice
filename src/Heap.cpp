#include "Heap.h"

HeapNode::HeapNode()
{
    //ctor
    score = 0;
    pos[0] = 0;
    pos[1] = 0;
}

HeapNode::HeapNode(float _score, unsigned int row, unsigned int col)
{
    //ctor
	score = _score;
    pos[0] = row;
    pos[1] = col;
}

HeapNode::HeapNode(const HeapNode& obj) {
	score = obj.score;
	pos[0] = obj.pos[0];
	pos[1] = obj.pos[1];
}

HeapNode::~HeapNode()
{
    //dtor
}

void HeapNode::operator=(const HeapNode& obj) {
	score = obj.score;
	pos[0] = obj.pos[0];
	pos[1] = obj.pos[1];
}

void HeapNode::swap(HeapNode* n1, HeapNode* n2) {
    HeapNode* tmp = new HeapNode(n1->score, n1->pos[0], n1->pos[1]);
    n1->score = n2->score; n1->pos[0] = n2->pos[0]; n1->pos[1] = n2->pos[1];
    n2->score = tmp->score; n2->pos[0] = tmp->pos[0]; n2->pos[1] = tmp->pos[1];
    delete tmp;
}

