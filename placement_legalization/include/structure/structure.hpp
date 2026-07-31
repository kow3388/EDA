// File: structure.hpp
// Author: YU-WEN WANG
// Created: 2026-07-28

#pragma once
#include <string>
#include <vector>
#include <memory>

struct Cell
{
	using ptr = std::unique_ptr<Cell>;

	std::string name;
	int x;
	int y;
	int width;
	int height;
};

struct Subrow
{
	using ptr = std::unique_ptr<Subrow>;

	int x_left;
	int x_right;
	int y;
	int height;
	int width;
	std::vector<Cell*> cells;
};

struct Input
{
	using ptr = std::unique_ptr<Input>;

	int height;
	int width;
	std::vector<Cell::ptr> cells;
	std::vector<Blockage::ptr> blockages;
	std::vector<Subrow::ptr> rows;
};
