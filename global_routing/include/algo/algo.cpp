// File: algo.cpp
// Author: YU-WEN WANG
// Created: 2026-08-17

#include "algo.hpp"
#include <algorithm>
#include <cstdlib>

A_Star::A_Star(Input *input)
{
	this->input = input;
	this->r_size = input->r_size;
	this->c_size = input->c_size;
	vertical.resize(input->r_size - 1, std::vector<Edge>(input->c_size, input->v_capacity));
	horizontal.resize(input->r_size, std::vector<Edge>(input->c_size - 1, input->h_capacity));
}

void A_Star::routNet(Net *net)
{
	std::pair<int, int> source = net->pins[0];
	std::pair<int, int> target = net->pins[1];
}

Writer::ptr A_Star::solve()
{
	std::sort(input->nets.begin(), input->nets.end(), [](const Net::ptr &a, const Net::ptr &b){
		  int manhattan1 = std::abs(a->pins[0].first - a->pins[1].first) + std::abs(a->pins[0].second - a->pins[1].second);
		  int manhattan2 = std::abs(b->pins[0].first - b->pins[1].first) + std::abs(b->pins[0].second - b->pins[1].second);

		  return manhattan1 > manhattan2;
	});
}
