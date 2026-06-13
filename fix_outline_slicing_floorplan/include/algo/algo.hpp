// File: algo.hpp
// Author: YU-WEN WANG
// Created: 2026-06-13

#pragma once
#include "../include/structure/structure.hpp"
#include <vector>

class WongLuiAlgo
{
private:
	Input *input;
	std::vector<Node::ptr> expr;
	int limit;	// floorplan width & hegiht limit
	int Width;	// current floorplan width
	int Height;	// current floorplan height
	int weight;	// for initial penalty weight

public:
	WongLuiAlgo(Input *input);
	void initial();
	bool isValid();		// is expr NPE
	void show();
};
