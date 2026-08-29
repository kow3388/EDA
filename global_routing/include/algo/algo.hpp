// File: algo.hpp
// Author: YU-WEN WANG
// Created: 2026-08-17

#pragma once
#include "../structure/structure.hpp"
#include "../writer/writer.hpp"
#include <filesystem>
#include <vector>
#include <tuple>
#include <queue>

class A_Star
{
public:
	using Path = std::filesystem::path;

	A_Star(Input *input, Path file_name);
	Writer::ptr solve();

private:
	Path file_name;
	Input *input;
	int r_size, c_size;
	std::vector<int> dir;
	std::vector<std::vector<Edge>> vertical;		// vertical[y-1][x] direction up
	std::vector<std::vector<Edge>> horizontal;		// horizontal[y][x-1] direction left
	
	int getWL();
	void updateEdgeDemand(int y, int x, bool inc, Direction direction);
	double getEdgeCost(int y, int x, Direction &direction);
	double getCost(const std::tuple<double, int, int> &target,
		       const std::tuple<double, int, int> &cur,
		       Direction &direction);
	void wavePropagation(const std::tuple<double, int, int> &source,
			   const std::tuple<double, int, int> &target,
			   std::vector<std::vector<std::pair<Direction, double>>> &graph);
	void backTrack(Net *net, std::vector<std::vector<std::pair<Direction, double>>> &graph);
	void routNet(Net *net);
	int getOverflow();
	std::priority_queue<Edge*, std::vector<Edge*>, Edge> getRipupNet();
	void ripupRerout(std::unordered_set<Net*> nets);
};
