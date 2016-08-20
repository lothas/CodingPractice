#ifndef NODE_H
#define NODE_H


class IntPoint2D
{
	public:
		IntPoint2D() { x = 0; y = 0; };
		IntPoint2D(int _x, int _y) { x = _x; y = _y; };
		virtual ~IntPoint2D() {};

		int x;
		int y;
};

class Node
{
    public:
        Node(IntPoint2D _value);
        virtual ~Node();

        IntPoint2D value;
        Node* next;

    protected:

    private:
};

#endif // NODE_H
