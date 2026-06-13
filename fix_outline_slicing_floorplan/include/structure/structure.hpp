// File: structure.hpp
// Author: YU-WEN WANG
// Created: 2026-06-10

# pragma once
#include <string>
#include <utility>
#include <vector>
#include <memory>

struct Block
{
	// alias for block smart pointer
	using ptr = std::unique_ptr<Block>;

	std::string name;
	std::pair<int, int> bottom_left;
	int width;
	int height;
	double score;	// for initial sorting

	Block();
	Block(std::string name, std::pair<int, int> bottom_left, int width, int height);
};

struct Pin
{
	// alias for pin smart pointer
	using ptr = std::unique_ptr<Pin>;

	std::string name;
	std::pair<int, int> point;

	Pin();
	Pin(std::string name, std::pair<int, int> point);
};

struct Net
{
	// alias for net smart pointer
	using ptr = std::unique_ptr<Net>;

	std::vector<Block*> blocks;
	std::vector<Pin*> pins;

	Net();
};

struct Input
{
	// alias for input smart pointer
	using ptr = std::unique_ptr<Input>;

	std::vector<Block::ptr> blocks;
	std::vector<Pin::ptr> pins;
	std::vector<Net::ptr> nets;
	int block_area;
	double dead_space_ratio;

	Input();
	void sortBlock();
};

enum class Type {H_CUT, V_CUT, BLOCK};

struct Node
{
	// alias for node smart pointer
	using ptr = std::unique_ptr<Node>;

	Type type;
	Block *block;
	bool rotate;

	Node();
};
