// File: algo.hpp
// Author: YU-WEN WANG
// Created: 2026-08-17

#pragma once
#include "../structure/structure.hpp"
#include <vector>

class A_Star
{
private:
	int r_size, c_size;
	std::vector<vector<Edge>> vertical;		// vertical[y-1][x] direction up
	std::vector<vector<Edge>> horizontal;		// horizontal[y][x-1] direction left
	
	void routNet(Net *net);
public:
	A_Star(Input *input);
	Writer::ptr solve();
};
