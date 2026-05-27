// File: structure.cpp
// Author: YU-WEN WANG
// Created: 2026-05-16

#include "structure.hpp"

// cell structure
Cell::Cell():
	name(""), size(0), gain(0), lock(false),
	group_id(0), next(nullptr), prev(nullptr)
{
}

Cell::Cell(std::string name, int cell_size):
	name(name), size(cell_size), gain(0), lock(false),
	group_id(0), next(nullptr), prev(nullptr)
{
}

// Net structure
Net::Net():
	name(""), group1_cnt(0), group2_cnt(0)
{
}

Net::Net(std::string name):
	name(name), group1_cnt(0), group2_cnt(0)
{
}

// Gain bucket list structure
GainBucketList::GainBucketList():
	max_gain(0)
{
}

GainBucketList::GainBucketList(int max_gain):
	max_gain(max_gain)
{
}

void GainBucketList::insert(Cell *cell)
{
	/*
	 * insert cell to current cell gain bucket list
	 */

	// current gain first initial (create dummy cell for head)
	if(!gain2cells[cell->gain])
		gain2cells[cell->gain] = new Cell();

	Cell *head = gain2cells[cell->gain];

	cell->prev = head;
	cell->next = head->next;

	if(head->next)
		head->next->prev = cell;

	head->next = cell;
};

void GainBucketList::remove(Cell *cell)
{
	/*
	 * remove cell from origin bucket list
	 */

	Cell *prev = cell->prev;
	Cell *next = cell->next;

	if(prev)
		prev->next = next;

	if(next)
		next->prev = prev;

	cell->prev = cell->next = nullptr;
};

Cell* GainBucketList::getBestCell()
{
	/*
	 * get the current max gain cell
	 */

	for(int g = max_gain; g >= -max_gain; g--)
	{
		// no cell in current gain
		if(!gain2cells[g] || !gain2cells[g]->next)
			continue;

		return gain2cells[g]->next;
	}

	return nullptr;
};

// group structure
Group::Group():
	size(0)
{
}

// input structure
Input::Input():
	max_degree(0), diff_spec(0.0)
{
}
