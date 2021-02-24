#pragma once
#include <iostream>
#include <iomanip>
#include "list.h"

using namespace std;

List::List()
{
	head = 0;
	tail = 0;
	size = 0;
}

List::~List()
{
	Node *p = head, *temp;
	while (p)
	{
		temp = p;
		p = p->next;
		delete temp;
	}
}

void List::addHead(Coord data)
{
	Node *newNode = new Node(data);
	if (head == 0)
		head = tail = newNode;
	else
	{
		newNode->next = head;
		head->prev = newNode;
		head = newNode;
	}
}

void List::addTail(Coord data)
{
	Node *newNode = new Node(data);
	if (head == 0)
		head = tail = newNode;
	else
	{
		newNode->prev = tail;
		tail->next = newNode;
		tail = newNode;
	}
}

void List::print()
{
	Node *p = head;
	while (p)
	{
		wcout << "row: " << setw(3) << p->data.row << ", col: " << setw(3) << p->data.col << endl;
		p = p->next;
	}
}

void List::printBack()
{
	Node *p = tail;
	while (p)
	{
		wcout << "row: " << setw(3) << p->data.row << ", col: " << setw(3) << p->data.col << endl;
		p = p->prev;
	}
}

bool List::removeHead()
{
	if (head)
	{
		Node *temp = head;
		head = head->next;
		if (head)
			head->prev = 0;
		delete temp;
		return true;
	}
	return false;
}

bool List::removeTail()
{
	if (tail)
	{
		Node *temp = tail;
		tail = tail->prev;
		tail->next = 0;
		delete temp;
		return true;
	}
	return false;
}

int List::getSize()
{
	return size;
}

void List::push(Coord data)
{
	addHead(data);
	size++;
}

Coord List::pop()
{
	Coord temp;
	temp.row = -1;
	temp.col = -1;
	if (size == 0)
		return temp;
	Coord data = head->data;
	if (removeHead())
	{
		size--;
		return data;
	}
	return temp;
}

void List::clear()
{
	Node *p = head, *temp;
	while (p)
	{
		temp = p;
		p = p->next;
		delete temp;
	}
	head = 0;
	tail = 0;
	size = 0;
}