// File: structure.cpp
// Author: YU-WEN WANG
// Created: 2026-06-10

#include "structure.hpp"
#include <algorithm>

// block structure
Block::Block():
	name(""), bottom_left(0,0), width(0), height(0), score(0.0)
{
}

Block::Block(std::string name, std::pair<int, int> bottom_left, int width, int height):
	name(name), bottom_left(bottom_left), width(width), height(height), score(0.0)
{
}

// pin struct
Pin::Pin():
	name(name), point(0,0)
{
}

Pin::Pin(std::string name, std::pair<int, int> point):
	name(name), point(point)
{
}

// net structur
Net::Net() {}

// Input structure
Input::Input():
	block_area(0), dead_space_ratio(0.0)
{
}

void Input::sortBlock()
{
	std::sort(blocks.begin(), blocks.end(), [](Block::ptr &a, Block::ptr &b)
			{ return a->score > b->score; });
}

// Node structure
Node::Node():
	type(Type::BLOCK), block(nullptr), rotate(false)
{
}
