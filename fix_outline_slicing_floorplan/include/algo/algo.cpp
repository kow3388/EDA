// File: algo.cpp
// Author: YU-WEN WANG
// Created: 2026-06-13

#include "algo.hpp"
#include <iostream>
#include <cmath>
#include <memory>
#include <climits>
#include <cstdlib>
#include <utility>
#include <string>
#include <tuple>

WongLuiAlgo::WongLuiAlgo(Input *input)
{
	this->input = input;
	limit = 0;
	weight = 1e3;	// change if you want
}

std::vector<int> WongLuiAlgo::dfsNPE(int depth, int l, int r)
{
	if(l == r)
	{
		Node::ptr node = std::make_unique<Node>();
		node->block = input->blocks[l].get();
		node->update();

		block_nodes.push_back(std::move(node));

		std::vector<int> expr;
		expr.push_back(l);

		return expr;
	}

	int mid = l + (r - l)/2;
	std::vector<int> left = dfsNPE(depth + 1, l, mid);
	std::vector<int> right = dfsNPE(depth + 1, mid + 1, r);

	left.insert(left.end(), right.begin(), right.end());

	Node::ptr node = std::make_unique<Node>();

	if(depth % 2)
	{
		left.push_back(-1);
		node->type = Type::V_CUT;
	}
	else
	{
		left.push_back(-2);
		node->type = Type::H_CUT;
	}

	cut_nodes.push_back(std::move(node));

	return left;
}

std::vector<int> WongLuiAlgo::initialNPE()
{
	/*
	 * generate initial NPE
	 *
	 * output: NPE
	 */

	// calculate limit
	limit = sqrt(input->block_area * (1 + input->dead_space_ratio));

	// initial NPE
	std::vector<int> expr = dfsNPE(0, 0, input->blocks.size() - 1);

	return expr;
}

bool WongLuiAlgo::isValid(std::vector<int> &expr)
{
	const int n = expr.size();

	int diff = 0;
	for(int i = 0; i < n; i++)
	{
		int cur_type = expr[i];
		if(cur_type >= static_cast<int>(Type::BLOCK))
			diff++;
		else
		{
			diff--;
			if(i > 0 && cur_type == static_cast<int>(Type::H_CUT) && expr[i - 1] == static_cast<int>(Type::H_CUT))
				return false;

			if(i > 0 && cur_type == static_cast<int>(Type::V_CUT) && expr[i - 1] == static_cast<int>(Type::V_CUT))
				return false;
		}

		// no enough operands
		if(diff <= 0)
			return false;
	}

	return diff == 1;
}

Node* WongLuiAlgo::buildTree(std::vector<int> &expr)
{
	/*
	 * bottom up build slicing tree
	 */

	int cut_idx = 0;
	std::vector<Node*> stk;
	for(int &e : expr)
	{
		if(e >= static_cast<int>(Type::BLOCK))
			stk.push_back(block_nodes[e].get());
		else
		{
			// is cut
			Node *cur = cut_nodes[cut_idx++].get();
			cur->type = e == -1 ? Type::V_CUT : Type::H_CUT;

			Node *r = stk.back();
			stk.pop_back();

			Node *l = stk.back();
			stk.pop_back();

			cur->left = l;
			cur->right = r;

			cur->update();

			stk.push_back(cur);
		}
	}

	return stk.back();
}

void WongLuiAlgo::setCoordinate(Node *root, int choice, int x, int y)
{
	/*
	 * top down set all block bottom left coordinate
	 *
	 * input: current node, choice, x and y
	 */

	Record record = root->records[choice];

	if(root->type == Type::BLOCK)
	{
		Block *block = root->block;

		int w = block->width;
		int h = block->height;
		int record_w = record.width;
		int record_h = record.height;

		if(w == record_h && h == record_w)
			block->rotate = true;

		block->bottom_left = {x, y};
	}
	else
	{
		int l_choice = record.l_choice;
		int r_choice = record.r_choice;

		setCoordinate(root->left, l_choice, x, y);
		
		int l_w = root->left->records[l_choice].width;
		int l_h = root->left->records[l_choice].height;

		if(root->type == Type::H_CUT)
			y += l_h;
		else
			x += l_w;

		setCoordinate(root->right, r_choice, x, y);
	}
}

std::vector<int> WongLuiAlgo::getCost(std::vector<int> &expr,
				      bool wl_optimize)
{
	/*
	 * get total cost
	 * if wire length optimize: cost = alpha * wl + beta * penalty
	 * else: cost = beta * penalty
	 * penalty = is W or H out of bound
	 * 
	 * input: NPE and is wire length optimize
	 * output: {cost, wire length, penalty}
	 */

	Node *root = buildTree(expr);

	int alpha = 0, beta = limit;

	// use area & penalty to decide which one left & right child
	int cost = INT_MAX;
	int penalty = 0, choice = 0;
	for(int i = 0; i < root->records.size(); i++)
	{
		Record cur_record = root->records[i];
		int w = cur_record.width, h = cur_record.height;

		int cur_penalty = std::max(0, w - limit) + std::max(0, h - limit);
		int cur_cost = beta * cur_penalty;

		if(cost > cur_cost)
		{
			cost = cur_cost;
			choice = i;
			penalty = cur_penalty;
		}
	}

	int wl = 0;
	if(wl_optimize)
	{
		alpha = 1;
		
		setCoordinate(root, choice, 0, 0);
		for(Net::ptr &net : input->nets)
			wl += net->getWL();

		cost = alpha * wl + beta * penalty;
	}

	return {cost, wl, penalty};
}

std::vector<int> WongLuiAlgo::perturb(std::vector<int> &expr, int move_type)
{
	/*
	 * 3 type of move of SA
	 * move_type == 0: swap 2 operands
	 * move_type == 1: invert operators
	 * move_type == 2: swap adjcent operand and operator
	 *
	 * input: NPE, move type
	 * output: new NPE
	 */

	std::vector<int> new_expr = expr;

	if(move_type == 0)
	{
		std::vector<int> operand_idxs;
		for(int i = 0; i < expr.size(); i++)
			if(expr[i] >= static_cast<int>(Type::BLOCK))
				operand_idxs.push_back(i);

		int n = operand_idxs.size();
		int idx1 = rand() % n;
		int idx2 = rand() % n;

		// idx2 is cut or idx2 == idx1
		while(idx2 == idx1)
			idx2 = rand() % n;

		std::swap(new_expr[operand_idxs[idx1]], new_expr[operand_idxs[idx2]]);
	}
	else if(move_type == 1)
	{
		// start index, chain length
		std::vector<std::pair<int, int>> chain;
		for(int i = 1; i < expr.size(); i++)
		{
			// start idx
			if(expr[i - 1] >= static_cast<int>(Type::BLOCK) && expr[i] < static_cast<int>(Type::BLOCK))
				chain.push_back({i, 1});
			else if(expr[i - 1] < static_cast<int>(Type::BLOCK) && expr[i] < static_cast<int>(Type::BLOCK))
				chain.back().second++;
		}

		int n = chain.size();

		int idx = rand() % n;
		int start_idx = chain[idx].first, len = chain[idx].second;

		for(int i = start_idx; i < start_idx + len; i++)
		{
			if(expr[i] == static_cast<int>(Type::H_CUT))
				new_expr[i] = static_cast<int>(Type::V_CUT);
			else
				new_expr[i] = static_cast<int>(Type::H_CUT);
		}
	}
	else
	{
		std::vector<int> valid_idxs;
		for(int i = 0; i < expr.size() - 1; i++)
			if((expr[i] >= static_cast<int>(Type::BLOCK) && expr[i + 1] < static_cast<int>(Type::BLOCK)) ||
			   (expr[i] < static_cast<int>(Type::BLOCK) && expr[i + 1] >= static_cast<int>(Type::BLOCK)))
				valid_idxs.push_back(i);

		while(!valid_idxs.empty())
		{
			int idx = rand() % valid_idxs.size();
			std::swap(new_expr[valid_idxs[idx]], new_expr[valid_idxs[idx] + 1]);

			if(isValid(new_expr))
				break;

			std::swap(new_expr[valid_idxs[idx]], new_expr[valid_idxs[idx] + 1]);
			std::swap(valid_idxs[idx], valid_idxs.back());
			valid_idxs.pop_back();
		}
	}

	return new_expr;
}

std::tuple<std::vector<int>, int, int> WongLuiAlgo::SA(std::vector<int> expr,
					    	       double init_temperature,
						       double end_temperature,
						       double cool_factor,
						       int max_try,
						       bool wl_optimize)
{
	/*
	 * main simulated annealing
	 *
	 * input: NPE, initial temperature, end temperature, cool factor, try time, is wl optimize
	 * output: best NPE, penalty, wire lenght
	 */

	std::vector<int> best_expr = expr;
	std::vector<int> all_cost = getCost(expr, wl_optimize);
	int best_cost = all_cost[0];
	int best_wl = all_cost[1];
	int best_penalty = all_cost[2];

	// for a valid floorplan design first
	if(!wl_optimize && best_penalty == 0)
		return {best_expr, best_wl, best_penalty};

	double temperature = init_temperature;
	int cur_cost = best_cost;
	int cur_wl = best_wl;
	int cur_penalty = best_penalty;

	while(temperature >= end_temperature)
	{
		for(int i = 0; i < max_try; i++)
		{
			// random select move
			int move_type = wl_optimize ? rand() % 3 : 0;

			std::vector<int> new_expr = perturb(expr, move_type);

			// move type 3 and there is no valid move for move type 3
			if(new_expr == expr)
			{
				move_type = rand() % 2;
				new_expr = perturb(expr, move_type);
			}

			all_cost = getCost(new_expr, wl_optimize);

			// optimize wl but out of limit (not accept)
			if(wl_optimize && all_cost[2] > 0)
				continue;

			int diff_cost = all_cost[0] - cur_cost;
			if(diff_cost < 0 || 
			   static_cast<double>(rand())/RAND_MAX < exp(-diff_cost/temperature))
			{
				cur_cost = all_cost[0];
				cur_wl = all_cost[1];
				cur_penalty = all_cost[2];
				expr = new_expr;

				if(!wl_optimize && cur_penalty == 0)
					return {expr, cur_wl, cur_penalty};

				if(cur_cost < best_cost)
				{
					best_expr = expr;
					best_cost = cur_cost;
					best_wl = cur_wl;
					best_penalty = cur_penalty;
				}
			}
		}

		temperature *= cool_factor;
	}

	return {best_expr, best_wl, best_penalty};
}

Writer::ptr WongLuiAlgo::solve()
{
	// set random seed
	int seed = 0;
	srand(seed);

	std::string line(32, '-');

	std::cout << "Start initial slicing floorplan" << std::endl;
	std::vector<int> expr = initialNPE();

	std::vector<int> all_cost = getCost(expr, false);
	int cost = all_cost[0], wl = all_cost[1], penalty = all_cost[2];
	std::cout << line << std::endl;

	if(!penalty)
		std::cout << "Floorplan in spec, skip 1st Simulated Annealing" << std::endl;
	else
	{
		std::cout << "Start 1st Simulated Annealing" << std::endl;
		std::cout << "Find a valid floorplan first" << std::endl;

		std::tie(expr, wl, penalty) = SA(expr,
						 1000,
						 0.1,
						 0.9,
						 100,
						 false); 
	}

	if(penalty > 0)
	{
		std::cout << "Can't find valid slicing floorplan" << std::endl;
		return nullptr;
	}
	else
		std::cout << "Find valid Slicing floorplan" << std::endl;

	all_cost = getCost(expr, true);
	std::cout << "Current wire length: " << all_cost[1] << std::endl;
	std::cout << line << std::endl;
	std::cout << "Start 2nd Sumulated Annealing" << std::endl;

	std::tie(expr, wl, penalty) = SA(expr,
					 100,
					 0.1,
					 0.9,
					 50,
					 true);


	all_cost  = getCost(expr, true);
	cost = all_cost[0], wl = all_cost[1], penalty = all_cost[2];

	std::cout << "Best wire length: " << wl << std::endl;

	Writer::ptr writer = std::make_unique<Writer>();
	writer->setWL(wl);

	for(Block::ptr &block : input->blocks)
		writer->addBlock(block.get());

	return writer;
}
