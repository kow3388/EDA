// File: structure.cpp
// Author: YU-WEN WANG
// Created: 2026-08-15

#include "structure.hpp"

// Net structur
Net::Net():
	name(""), id(0), overflow(0), wl(0)
{
}

Net::Net(std::string name, int id):
	name(name), id(id), overflow(0), wl(0)
{
}

bool Net::operator()(const Net *a, const Net *b)
{
	if(a->overflow < b->overflow)
		return true;
	else if(a->overflow == b->overflow)
		return a->wl > b->wl;
	else
		return false;
}

// Edge structure
Edge::Edge():
	capacity(0), demand(0), history(0)
{
}

Edge::Edge(int capacity):
	capacity(capacity), demand(0), history(0)
{
}

bool Edge::operator()(const Edge *a, const Edge *b)
{
	int overflow_a = std::max(0, a->demand - a->capacity);
	int overflow_b = std::max(0, b->demand - b->capacity);

	return overflow_a < overflow_b;
}

// Input structure
Input::Input():
	r_size(0), c_size(0), h_capacity(0), v_capacity(0)
{
}

Input::Input(int r_size, int c_size, int h_capacity, int v_capacity):
	r_size(r_size), c_size(c_size), h_capacity(h_capacity), v_capacity(v_capacity)
{
}
