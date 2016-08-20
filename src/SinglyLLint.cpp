#include "../include/SinglyLLint.h"

SinglyLLint::SinglyLLint()
{
    //ctor
}

SinglyLLint::~SinglyLLint()
{
    //dtor
}

bool SinglyLLint::is_empty()
{
    if (!head) return true;
    else return false;
}

void SinglyLLint::push_front(IntPoint2D value)
{
    if (is_empty())
    {
        head = new Node(value);
        tail = head;
    }
    else
    {
        Node* new_node = new Node(value);
        new_node->next = head;
        head = new_node;
    }
}

void SinglyLLint::push_back(IntPoint2D value)
{
    if (is_empty())
    {
        head = new Node(value);
        tail = head;
    }
    else
    {
        Node* new_node = new Node(value);
        tail->next = new_node;
        tail = new_node;
    }
}

IntPoint2D SinglyLLint::pop_front(bool* res)
{
    if (is_empty())
    {
        *res = false;
        return IntPoint2D(-1,-1);
    }
    if (head == tail)
    {
    	IntPoint2D value(head->value.x, head->value.y);
        delete(head);
        head = nullptr;
        tail = nullptr;
        *res = true;
        return value;
    }

    Node* tmp = head->next;
    IntPoint2D value(head->value.x, head->value.y);
    delete (head);
    head = tmp;
    *res = true;
    return value;
}

void SinglyLLint::print()
{
    Node* cur_node = head;
    std::cout << "[{" << cur_node->value.x << "," << cur_node->value.y << "}";
    while (cur_node->next)
    {
        cur_node = cur_node->next;
        std::cout << ", {" << cur_node->value.x << "," << cur_node->value.y << "}";
    }
    std::cout << "]" << std::endl;
}
