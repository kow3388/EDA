// File: structure.cpp
// Author: YU-WEN WANG
// Created: 2026-07-30

#include "structure.hpp"

// Cell structure
Cell::Cell():
	name(""), x_global(0), y_global(0), x(0.0), y(0.0), weight(1.0), width(0), height(0)
{
}

Cell::Cell(std::string name, int x_global, int y_global, int width, int height):
	name(name), x_global(x_global), y_global(y_global), x(0.0), y(0.0), weight(1.0), width(width), height(height)
{
}

// cluster structure
Cluster::Cluster()
	x(0.0), width(0.0), weight(0.0), q(0.0)
{
}

Cluster::Cluster(double x)
	x(x), width(0.0), weight(0.0), q(0.0)
{
}

// Subrow structure
Subrow::Subrow()
	x_left(0), x_right(0), y(0), width(0), free_space(0)
{
}

Subrow::Subrow(int x_left, int x_right, int y, int width, int free_space)
	x_left(x_left), x_right(x_right), y(y), width(width), free_space(free_space)
{
}

// Row structure
Row::Row()
	x_left(0), x_right(0), y(0), width(0), height(0), x_start(x_left)
{
}

Row::Row(int x_left, int x_right, int y, int width, int height)
	x_left(x_left), x_right(x_right), y(y), width(width), height(height), x_start(x_left)
{
}

// Input structure
Input::Input():
	width(0), height(0), max_displace(0)
{
}

Input::Input(int width, int height):
	width(width), height(height), max_displace(0)
{
}
