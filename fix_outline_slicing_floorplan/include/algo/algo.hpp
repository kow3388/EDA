// File: algo.hpp
// Author: YU-WEN WANG
// Created: 2026-06-13

#pragma once
#include "../include/structure/structure.hpp"
#include <tuple>
#include <vector>
#include <utility>

class WongLuiAlgo
{
private:
	Input *input;
	std::vector<Node::ptr> block_nodes;
	std::vector<Node::ptr> cut_nodes;
	int limit;	// floorplan width & hegiht limit
	int weight;	// for initial penalty weight
	
	std::vector<int> initialNPE();
	std::vector<int> perturb(std::vector<int> expr, int type);
	bool isValid(std::vector<int> expr);	// is expr NPE
	
	Node* buildTree(std::vector<int> expr);
	void setCoordinate(Node *root, int choice, int x, int y);

	std::vector<int> getCost(std::vector<int> expr, bool wl_optimize);
	std::tuple<std::vector<int>, int, int> SA(std::vector<int> expr,
					    	  double init_temperature,
						  double end_temperature,
						  double cool_factor,
						  bool wl_optimize);
public:
	WongLuiAlgo(Input *input);
	void run();
};
