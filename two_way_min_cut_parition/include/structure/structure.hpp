// File: structure.hpp
// Author: YU-WEN WANG
// Created: 2026-05-12

#pragma once
#include <string>
#include <vector>
#include <unordered_map>

struct Net;

struct Cell
{
	std::vector<Net*> nets;
	std::string name;
	int size;
	int gain;
	bool lock;
	int group_id;
	Cell *next;	// for gain bucketlist
	Cell *prev;	// for gain bucketlist

	Cell();
	Cell(std::string name, int cell_size);
};

struct Net
{
	std::vector<Cell*> cells;
	std::string name;
	int group1_cnt;
	int group2_cnt;

	Net();
	Net(std::string name);
};

struct GainBucketList
{
	int max_gain;
	std::unordered_map<int, Cell*> gain2cells;

	GainBucketList();
	GainBucketList(int max_gain);
	void insert(Cell *cell);
	void remove(Cell *cell);
	Cell* getBestCell();
};

struct Group
{
	int size;

	Group();
};

struct Input
{
	std::vector<Cell*> cells;
	std::vector<Net*> nets;
	int max_degree;
	double diff_spec;

	Input();
};
