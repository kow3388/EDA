// File: algo.cpp
// Author: YU-WEN WANG
// Created: 2026-05-18

#include "algo.hpp"
#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <memory>
#include <random>
#include <algorithm>
#include <unordered_map>
#include <climits>

FmAlgo::FmAlgo(Input *input)
{
	this->input = input;
	this->bucket_list = std::make_unique<GainBucketList>(input->max_degree);

	group1_size = 0;
	group2_size = 0;

	// run how many time FM algo
	epochs = 5;
}

void FmAlgo::initialGroup()
{
	// make 2 group size 0 first
	group1_size = group2_size = 0;

	/*
	 * for debug, fix random seed
	 * int seed = 0;
	 * std::mt19937 rng(seed);
	 */ 

	// get rng (shuffle for initial group)
	std::random_device rd;
	std::mt19937 rng(rd());
	std::shuffle(input->cells.begin(), input->cells.end(), rng);

	for(Cell::ptr &cell : input->cells)
	{
		// try make 2 groups cell size balance
		if(group1_size > group2_size)
		{
			cell->group_id = 2;
			group2_size += cell->size;
		}
		else
		{
			cell->group_id = 1;
			group1_size += cell->size;
		}
	}
}

void FmAlgo::updateAllNet()
{
	// update each net group cnt
	for(auto &net : input->nets)
	{
		net->group1_cnt = 0;
		net->group2_cnt = 0;
		for(Cell* cell : net->cells)
		{
			if(cell->group_id == 1)
				net->group1_cnt++;
			else
				net->group2_cnt++;
		}
	}
}

void FmAlgo::updateAllGain()
{
	for(auto &cell : input->cells)
	{
		cell->lock = false;

		bucket_list->remove(cell.get());
		cell->gain = 0;

		for(Net* net : cell->nets)
		{
			// situation gain will update
			if(cell->group_id == 1 && net->group1_cnt == 1)
				cell->gain++;

			if(cell->group_id == 1 && net->group2_cnt == 0)
				cell->gain--;

			if(cell->group_id == 2 && net->group2_cnt == 1)
				cell->gain++;

			if(cell->group_id == 2 && net->group1_cnt == 0)
				cell->gain--;
		}

		bucket_list->insert(cell.get());
	}
}

void FmAlgo::updateCellGain(Cell *input_cell)
{
	int ori_group = input_cell->group_id;
	int new_group = ori_group == 1 ? 2 : 1;
	input_cell->lock = true;
	input_cell->group_id = new_group;

	if(ori_group == 1)
	{
		group1_size -= input_cell->size;
		group2_size += input_cell->size;
	}
	else
	{
		group2_size -= input_cell->size;
		group1_size += input_cell->size;
	}

	bucket_list->remove(input_cell);

	// update the cell gain that the input cell nets conneted
	for(Net *net : input_cell->nets)
	{
		int from = ori_group == 1 ? net->group1_cnt : net->group2_cnt;
		int to = ori_group == 1 ? net->group2_cnt : net->group1_cnt;

		// before update
		for(Cell *cell : net->cells)
		{
			if(!cell->lock)
			{
				bucket_list->remove(cell);

				if(to == 0)
					cell->gain++;
				else if(to == 1 && cell->group_id == new_group)
					cell->gain--;
			}
		}

		// update input cell modify net cnt
		from--, to++;
		if(ori_group == 1)
		{
			net->group1_cnt--;
			net->group2_cnt++;
		}
		else
		{
			net->group2_cnt--;
			net->group1_cnt++;
		}

		// after update
		for(Cell *cell : net->cells)
		{
			if(!cell->lock)
			{
				if(from == 0)
					cell->gain--;
				else if(from == 1 && cell->group_id == ori_group)
					cell->gain++;

				bucket_list->insert(cell);
			}
		}
	}
}

int FmAlgo::iteration()
{
	// initial each iteration first
	updateAllNet();
	updateAllGain();

	int cur_gain = 0, cur_steps = 0, best_gain = 0, best_steps = 0;
	std::vector<Cell*> move_history;

	int cut_size = getCutSize();

	Cell *cur_cell;
	while(cur_cell = bucket_list->getBestCell())
	{
		// check can move or not
		int group_size_diff = 0;
		if(cur_cell->group_id == 1)
			group_size_diff = std::abs((group1_size - group2_size) - 2*cur_cell->size);
		else
			group_size_diff = std::abs((group2_size - group1_size) - 2*cur_cell->size);

		// move will be illegal, terminate current itteration
		if(group_size_diff >= input->diff_spec)
		{
			bucket_list->remove(cur_cell);
			continue;
		}

		updateCellGain(cur_cell);

		cur_gain += cur_cell->gain;
		cur_steps++;
		move_history.push_back(cur_cell);

		// update best
		if(cur_gain > best_gain)
		{
			best_gain = cur_gain;
			best_steps = cur_steps;
		}

	}

	// recover to best steps
	for(int i = move_history.size() - 1; i >= best_steps; i--)
	{
		cur_cell = move_history.back();
		move_history.pop_back();

		int ori_group = cur_cell->group_id;
		int new_group = ori_group == 1 ? 2 : 1;

		if(ori_group == 1)
			group1_size -= cur_cell->size;
		else
			group2_size -= cur_cell->size;

		if(new_group == 1)
			group1_size += cur_cell->size;
		else
			group2_size += cur_cell->size;

		cur_cell->group_id = new_group;
	}

	return best_gain;
}

int FmAlgo::getCutSize()
{
	updateAllNet();

	int cut = 0;
	for(auto &net : input->nets)
	{
		if(net->group1_cnt != 0 && net->group2_cnt != 0)
			cut++;
	}

	return cut;
}

Writer::ptr FmAlgo::solve()
{
	// record best result
	int best_cut_size = INT_MAX;
	std::unordered_map<std::string, int> best_mp;

	for(int i = 0; i < epochs; i++)
	{
		std::cout << "========== " << i << "'s epochs ==========" << std::endl;
		std::cout << "Start FM Algo" << std::endl;

		int it_time = 0, cut_size = 0;

		// initial
		initialGroup();
		updateAllNet();
		cut_size = getCutSize();
		std::cout << "Original cut size = " << cut_size << std::endl;

		while(true)
		{
			int best_gain = iteration();

			std::cout << it_time << " run: best_gain = " << best_gain << std::endl;
			it_time++;

			// do not improve stop it
			if(best_gain == 0)
			{
				std::cout << "Do not improve anymore, stop it!" << std::endl;

				cut_size = getCutSize();

				std::cout << i << "'s epoch final cut size = " << cut_size << std::endl;

				if(cut_size < best_cut_size)
				{
					std::cout << "Get better result, save" << std::endl;

					best_cut_size = cut_size;
					for(auto &cell : input->cells)
						best_mp[cell->name] = cell->group_id;

				}

				break;
			}
		}
	}

	std::cout << "================================" << std::endl;
	std::cout << "Best cut size = " << best_cut_size << std::endl;

	// create writer
	Writer::ptr writer = std::make_unique<Writer>();

	writer->setCutSize(best_cut_size);
	for(auto &cell : input->cells)
		cell->group_id = best_mp[cell->name];
	
	updateAllNet();

	for(auto &cell : input->cells)
		writer->addCell(cell.get());

	return writer;
}
