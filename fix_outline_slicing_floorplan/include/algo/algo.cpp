// File: algo.cpp
// Author: YU-WEN WANG
// Created: 2026-06-13

#include "algo.hpp"
#include <iostream>
#include <cmath>
#include <memory>
#include <utility>
#include <climits>

WongLuiAlgo::WongLuiAlgo(Input *input)
{
	this->input = input;
	limit = 0;
	Width = 0;
	Height = 0;
	weight = 1e3;	// change if you want
}

void WongLuiAlgo::initial()
{
	// calculate limit
	limit = sqrt(input->block_area * (1 + input->dead_space_ratio));

	// initial NPE
	Node::ptr node = std::make_unique<Node>();
	node->block = input->blocks[0].get();
	
	Width = node->block->width;
	Height = node->block->height;
	expr.push_back(std::move(node));

	for(int i = 1; i < input->blocks.size(); i++)
	{
		node = std::make_unique<Node>();

		Block *block = input->blocks[i].get();
		int w = block->width;
		int h = block->height;

		node->block = block;

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

		if(best == 3 || best == 4)
			node->rotate = true;

		expr.push_back(std::move(node));

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
		}

		expr.push_back(std::move(node));
	}
}

bool WongLuiAlgo::isValid()
{
	int diff = 0;
	for(Node::ptr &node : expr)
	{
		if(node->type == Type::BLOCK)
			diff++;
		else
			diff--;

		if(diff <= 0)
			return false;
	}

	return diff == 1;
}

void WongLuiAlgo::show()
{
	std::cout << "limit: " << limit << std::endl;
	std::cout << Width << ", " << Height << std::endl;

	for(Node::ptr &node : expr)
	{
		if(node->type == Type::BLOCK)
			std::cout << node->block->name << std::endl;
		else if(node->type == Type::H_CUT)
			std::cout << "H" << std::endl;
		else
			std::cout << "V" << std::endl;
	}
}
