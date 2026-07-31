// File: structure.cpp
// Author: YU-WEN WANG
// Created: 2026-07-30

#include "structure.hpp"

// Cell structure
Cell::Cell():
	name(""), x(0), y(0), width(0), height(0)
{
}

Cell::Cell(std::string name, int x, int y, int width, int height):
	name(name), x(x), y(y), width(width), height(height)
{
}

// Subrow structure
Subrow::Subrow()
	x_left(0), x_right(0), y(0), width(0), height(0)
{
}

Subrow::Subrow(int x_left, int x_right, int y, int width, int height)
	x_left(0), x_right(0), y(0), width(0), height(0)
{
}

// Input structure
Input::Input():
	width(0), height(0)
{
}

Input::Input(int width, int height):
	width(width), height(height)
{
}
