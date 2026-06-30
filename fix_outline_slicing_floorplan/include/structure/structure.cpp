// File: structure.cpp
// Author: YU-WEN WANG
// Created: 2026-06-10

#include "structure.hpp"
#include <algorithm>
#include <climits>

// block structure
Block::Block():
	name(""), bottom_left(0,0), width(0),
	height(0), score(0.0), rotate(false)
{
}

Block::Block(std::string name, std::pair<int, int> bottom_left, int width, int height):
	name(name), bottom_left(bottom_left), width(width),
	height(height), score(0.0), rotate(false)
{
}

// pin struct
Pin::Pin():
	name(""), point(0,0)
{
}

Pin::Pin(std::string name, std::pair<int, int> point):
	name(name), point(point)
{
}

// net structur
Net::Net() {}

int Net::getWL()
{
	int min_x = INT_MAX, min_y = INT_MAX;
	int max_x = 0, max_y = 0;
	int wl = 0;

	for(Pin *pin : pins)
	{
		int x = pin->point.first;
		int y = pin->point.second;

		min_x = std::min(min_x, x);
		min_y = std::min(min_y, y);

		max_x = std::max(max_x, x);
		max_y = std::max(max_y, y);
	}

	for(Block *block : blocks)
	{
		int x = block->bottom_left.first;
		int y = block->bottom_left.second;

		if(block->rotate)
		{
			x += block->height / 2;
			y += block->width / 2;
		}
		else
		{
			x += block->width / 2;
			y += block->height / 2;
		}

		min_x = std::min(min_x, x);
		min_y = std::min(min_y, y);

		max_x = std::max(max_x, x);
		max_y = std::max(max_y, y);
	}

	return (max_x - min_x) + (max_y - min_y);
}

// Input structure
Input::Input():
	block_area(0), dead_space_ratio(0.0)
{
}

void Input::sortBlock()
{
	std::sort(blocks.begin(),
		  blocks.end(),
		  [](Block::ptr &a, Block::ptr &b)
		  {
		  	if(std::max(a->width, a->height) > std::max(b->width, b->height))
				return true;
			else
				return a->score > b->score;
		  }
	);
}

// Record structure
Record::Record():
	width(0), height(0), l_choice(0), r_choice(0)
{
}

Record::Record(int width, int height):
	width(width), height(height), l_choice(0), r_choice(0)
{
}

Record::Record(int width, int height, int l_choice, int r_choice):
	width(width), height(height), l_choice(l_choice), r_choice(r_choice)
{
}

// Node structure
Node::Node():
	type(Type::BLOCK), block(nullptr), left(nullptr), right(nullptr)
{
}

void Node::update()
{
	records.clear();
	if(type == Type::BLOCK)
	{
		// ensure monotonic
		if(block->width >= block->height)
		{
			records.push_back(Record(block->width, block->height));
			records.push_back(Record(block->height, block->width));
		}
		else
		{
			records.push_back(Record(block->height, block->width));
			records.push_back(Record(block->width, block->height));
		}
	}
	else if(type == Type::H_CUT)
	{
		std::vector<Record> l_records = left->records;
		std::vector<Record> r_records = right->records;

		std::sort(l_records.begin(),
			  l_records.end(),
			  [](Record &a, Record &b) { return a.width > b.width; });
		std::sort(r_records.begin(),
			  r_records.end(),
			  [](Record &a, Record &b) { return a.width > b.width; });

		for(int l = 0, r = 0; l < l_records.size() && r < r_records.size();)
		{
			Record l_child = l_records[l];
			Record r_child = r_records[r];

			records.push_back(Record(std::max(l_child.width, r_child.width),
					  	 l_child.height + r_child.height,
					  	 l,
					  	 r));

			if(l_child.width >= r_child.width)
				l++;
			if(l_child.width <= r_child.width)
				r++;
		}
	}
	else
	{
		std::vector<Record> l_records = left->records;
		std::vector<Record> r_records = right->records;

		std::sort(l_records.begin(),
			  l_records.end(),
			  [](Record &a, Record &b) { return a.height > b.height; });
		std::sort(r_records.begin(),
			  r_records.end(),
			  [](Record &a, Record &b) { return a.height > b.height; });

		for(int l = 0, r = 0; l < l_records.size() && r < r_records.size();)
		{
			Record l_child = l_records[l];
			Record r_child = r_records[r];

			records.push_back(Record(l_child.width + r_child.width,
					  	 std::max(l_child.height, r_child.height),
					  	 l,
					  	 r));

			if(l_child.height >= r_child.height)
				l++;
			if(l_child.height <= r_child.height)
				r++;
		}
	}
}
