// File: structure.hpp
// Author: YU-WEN WANG
// Created: 2026-08-15

#pragma once
#include <string>
#include <utility>
#include <vector>
#include <memory>

struct Edge
{
	int capacity;
	int demand;

	Edge();
};

struct Net
{
	using ptr = std::unique_ptr<Net>;

	std::string name;
	int id;
	std::vector<std::pair<int, int>> pins;
	std::vector<std::pair<int, int>> path;

	Net();
	Net(std::string name, int id);
};

enum class Direction
{
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
