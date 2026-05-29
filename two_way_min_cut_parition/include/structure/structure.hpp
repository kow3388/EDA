// File: structure.hpp
// Author: YU-WEN WANG
// Created: 2026-05-12

#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

struct Net;

struct Cell
{
	// alias for cell smart pointer
	using ptr = std::unique_ptr<Cell>;

	std::vector<Net*> nets;
	std::string name;
	int size;
	int gain;
	bool lock;
	int group_id;
	Cell *next;	// for gain bucketlist (non-owning)
	Cell *prev;	// for gain bucketlist (non-owning)

	Cell();
	Cell(std::string name, int cell_size);
};

struct Net
{
	// alias for net smart pointer
	using ptr = std::unique_ptr<Net>;

	std::vector<Cell*> cells;
	std::string name;
	int group1_cnt;
	int group2_cnt;

	Net();
	Net(std::string name);
};

struct GainBucketList
{
	// alias for GainBucketList smart pointer
	using ptr = std::unique_ptr<GainBucketList>;

	int max_gain;
	// Dummy heads are owned here. The cells inserted are non-owning.
	std::unordered_map<int, Cell::ptr> gain2cells;

	GainBucketList();
	GainBucketList(int max_gain);
	void insert(Cell *cell);
	void remove(Cell *cell);
	Cell* getBestCell();
};

struct Group
{
	// alias for group smart pointer
	using ptr = std::unique_ptr<Group>;

	int size;

	Group();
};

struct Input
{
	// alias for input smart pointer
	using ptr = std::unique_ptr<Input>;

	std::vector<Cell::ptr> cells;
	std::vector<Net::ptr> nets;
	int max_degree;
	double diff_spec;

	Input();
};
