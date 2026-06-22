// File: algo.cpp
// Author: YU-WEN WANG
// Created: 2026-06-13

#include "algo.hpp"
#include <iostream>
#include <cmath>
#include <memory>
#include <climits>
#include <cstdlib>

WongLuiAlgo::WongLuiAlgo(Input *input)
{
	this->input = input;
	limit = 0;
	weight = 1e3;	// change if you want
}

std::vector<Node::ptr> WongLuiAlgo::initialNPE()
{
	std::vector<int> expr;

	// calculate limit
	limit = sqrt(input->block_area * (1 + input->dead_space_ratio));

	// initial NPE
	Node::ptr node = std::make_unique<Node>();
	node->block = input->blocks[0].get();
	node->update();
	
	expr.push_back(0);
	block_nodes->push_back(std::move(node));

	int Weight = 0, Height = 0;
	for(int i = 1; i < input->blocks.size(); i++)
	{
		node = std::make_unique<Node>();

		Block *block = input->blocks[i].get();
		node->block = block;
		node->update();

		expr.push_back(i);
		block_nodes.push_back(std::move(node));

		int best = -1;
		int p = INT_MAX, cur = INT_MAX;

		// H_cut and not rotate
		int h1 = Height + h;
		int w1 = std::max(Width, w);
		cur = (std::max(0, w1 - limit) + std::max(0, h1 - limit))*weight + std::abs(w1 - h1);
		if(p > cur)
		{
			p = cur;
			best = 1;
		}

		// V_cut and not rotate
		int h2 = std::max(Height, h);
		int w2 = Width + w;
		cur = (std::max(0, w2 - limit) + std::max(0, h2 - limit))*weight + std::abs(w2 - h2);
		if(p > cur)
		{
			p = cur;
			best = 2;
		}

		// H_cut and rorate
		int h3 = Height + w;
		int w3 = std::max(Width, h);
		cur = (std::max(0, w3 - limit) + std::max(0, h3 - limit))*weight + std::abs(w3 - h3);
		if(p > cur)
		{
			p = cur;
			best = 3;
		}

		// V_cut and rorate
		int h4 = std::max(Height, w);
		int w4 = Width + h;
		cur = (std::max(0, w4 - limit) + std::max(0, h4 - limit))*weight + std::abs(w4 - h4);
		if(p > cur)
		{
			p = cur;
			best = 4;
		}

		// decide slicing way
		if(best == 1 || best == 3)
		{
			node = std::make_unique<Node>();
			node->type = Type::H_CUT;

			if(best == 1)
			{
				Width = w1;
				Height = h1;
			}
			else
			{
				Width = w3;
				Height = h3;
			}

			expr.push_back(-2);
		}
		else
		{
			node = std::make_unique<Node>();
			node->type = Type::V_CUT;

			if(best == 2)
			{
				Width = w2;
				Height = h2;
			}
			else
			{
				Width = w4;
				Height = h4;
			}

			expr.push_back(-1);
		}

		cut_nodes.push_back(std::move(node));
	}

	return expr;
}

bool WongLuiAlgo::isValid(std::vector<int> expr)
{
	int diff = 0;
	for(int &node : expr)
	{
		if(node >= Type::BLOCK)
			diff++;
		else
			diff--;

		if(diff <= 0)
			return false;
	}

	return diff == 1;
}

Node* WongLuiAlgo::buildTree(std::vector<int> expr)
{
	/*
	 * bottom up build slicing tree
	 */

	int cut_idx = 0;
	std::vector<Node*> stk;
	for(int &e : expr)
	{
		if(e >= Type::BLOCK)
			stk.push_back(block_nodes[e].get());
		else
		{
			// is cut
			Node *cur = cut_nodes[cut_idx].get();

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

	Record::ptr &record = root->records[choice];

	if(root->type == Type::BLOCK)
	{
		Block *block = root->block;

		int w = block->width, h = block->height;
		int record_w = record.width, record_h = record.height;

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

std::vector<int> WongLuiAlgo::getCost(std::vector<int> expr,
				      bool wl_optimize)
{
	/*
	 * get total cost
	 * if wire length optimize: cost = alpha * wl + beta * area
	 * else: cost = beta * area + gamma * penalty
	 * penalty = is W or H out of bound
	 * 
	 * input: NPE and is wire length optimize
	 * output: {cost, wire length, penalty}
	 */

	Node *root = buildTree(expr);

	int alpha = 0, beta = 1, gamma = limit;

	// use area & penalty to decide which one left & right child
	int cost = INT_MAX;
	int area = 0, penalty = 0, choice = 0;
	for(int i = 0; i < root->records.size(); i++)
	{
		Record::ptr &cur_record = root->records[i];
		int w = cur_record.width, h = cur_record.height;

		int cur_area = w * h - input->block_area;
		int cur_penalty = std::max(0, w - limit) + std::max(0, h - limit);
		int cur_cost = beta * cur_area + gamma * cur_penalty;

		if(cost > cur_cost)
		{
			cost = cur_cost;
			choice = i;
			area = cur_area;
			penalty = penalty;
		}
	}

	int wl = 0;
	if(wl_optimize)
	{
		alpha = 1, gamma = 0;
		
		setCoordinate(root, choice, 0, 0);
		for(Net::ptr &net : input->nets)
			wl += net->getWL();

		int cur_cost = alpha * wl + beta * area;
		cost = std::min(cost, cur_cost);
	}

	return {cost, wl, penalty};
}

std::tuple<vector<int>, int, int> WongLuiAlgo::SA(std::vector<int> expr;
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
	std::vector<int> all_cost = getCost(expr, wl_optimize)[0];
	int best_cost = all_cost[0];
	int penalty = all_cost[2];

	// for a valid floorplan design first
	if(!wl_optimize && penalty == 0)
		return {best_expr, penalty};

	double temperature = init_temperature;
	int cur_cost = best_cost;

	while(temperature >= end_temperature)
	{
		for(int i = 0; i < max_try; i++)
		{
			// random select move
			int move_type = rand() % 3;

			std::vector<Node::ptr> new_expr = perturb(expr, move_type);

			all_cost = gestCost(new_expr, wl_optimize);
			cur_cost = all_cost[0];
			penalty = all_cost[2];

			// optimize wl but out of limit (not accept)
			if(wl_optimize && penalty > 0)
				continue;

			int diff_cost = all_cost[0] - cur_cost;
			if(diff_cost < 0 || 
			   static_cast<double>(rand())/RAND_MAX < exp(-diff_cost/temperature))
			{
				cur_cost = all_cost[0];
				expr = new_expr;

				if(cur_cost < best_cost)
				{
					best_expr = expr;
					best_cost = cur_cost;

					if(!wl_optimize && penalty == 0)
						return {best_expr, penalty};
				}
			}
		}

		temperature *= cool_factor;
	}

	return {best_expr, penalty};
}

void WongLuiAlgo::run()
{
	std::vector<int> expr = initialNPE();
}
