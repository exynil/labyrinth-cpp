#pragma once
struct Coord
{
	int row, col;
};

class Node
{
public:
	Coord data;
	Node *next;
	Node *prev;
	Node(Coord _data)
	{
		data.row = _data.row;
		data.col = _data.col;
		prev = 0;
		next = 0;
	}
};