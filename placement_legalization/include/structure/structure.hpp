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
	int x_global;
	int y_global;
	double x;
	double y;
	double weight;
	int width;
	int height;

	Cell();
	Cell(std::string name, int x_global, int y_global, int width, int height); 
};

struct Cluster
{
	using ptr = std::unique_ptr<Cluster>;

	double x;
	double width;
	double weight;
	double q;
	std::vector<Cell*> cells;

	Cluster();
	Cluster(double x);
};

struct Subrow
{
	using ptr = std::unique_ptr<Subrow>;

	int x_left;
	int x_right;
	int y;
	int width;
	int free_space;
	std::vector<Cluster::ptr> stk;	// cluster stack
	
	Subrow();
	Subrow(int x_left, int x_right, int y, int width, int free_space);
};

struct Row
{
	using ptr = std::unique_ptr<Row>;

	int x_left;
	int x_right;
	int y;
	int height;
	int width;
	int x_start;
	std::vector<Subrow> subrows;

	Row();
	Row(int x_left, int x_right, int y, int width, int height);
};

struct Input
{
	using ptr = std::unique_ptr<Input>;

	int height;
	int width;
	int max_displace;
	std::vector<Cell::ptr> cells;
	std::vector<Cell::ptr> blockages;
	std::vector<Row::ptr> rows;

	Input();
	Input(int width, int height);
};
