// File: structure.hpp
// Author: YU-WEN WANG
// Created: 2026-08-15

#pragma once
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <unordered_set>

struct Net
{
	using ptr = std::unique_ptr<Net>;

	std::string name;
	int id;
	int overflow;
	int wl;
	std::vector<std::pair<int, int>> pins;
	std::vector<std::pair<int, int>> path;

	Net();
	Net(std::string name, int id);
	bool operator()(const Net *a, const Net *b);
};

struct Edge
{
	int capacity;
	int demand;
	int history;
	std::unordered_set<Net*> nets;

	Edge();
	Edge(int capacity);
	bool operator()(const Edge *a, const Edge *b);
};

enum class Direction
{
	empty = -1,
	stop = 0,
	up = 1,
	left = 2,
	down = 3,
	right = 4
};

struct Input
{
	using ptr = std::unique_ptr<Input>;

	int r_size;		// row size
	int c_size;		// column size
	int h_capacity;
	int v_capacity;
	std::vector<Net::ptr> nets;

	Input();
	Input(int r_size, int c_size, int h_capacity, int v_capacity);
};
