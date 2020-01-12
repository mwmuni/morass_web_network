#pragma once
#include <iostream>
#include "node.h"

class Edge
{
private:
	Node start_node;
	Node end_node;
public:
	Edge();
	~Edge();
	int store_signal();
	int push_signal();
};