#pragma once
#include "node.h"

class List
{
	Node *head, *tail;
	int size;
public:
	List();
	~List();
	void addHead(Coord data);
	void addTail(Coord data);
	void print();
	void printBack();
	bool removeHead();
	bool removeTail();
	int getSize();
	void push(Coord data);
	Coord pop();
	void clear();
};