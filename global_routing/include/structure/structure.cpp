// File: structure.cpp
// Author: YU-WEN WANG
// Created: 2026-08-15

#include "structure.hpp"

// Edge structure
Edge::Edge():
	capacity(0), demand(0)
{
}

// Net structure
Net::Net():
	name(""), id(0)
{
}

Net::Net(std::string name, int id):
	name(name), id(id)
{
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
