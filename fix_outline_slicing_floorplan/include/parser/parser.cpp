// File: parser.cpp
// Author: YU-WEN WANG
// Created: 2026-06-10

#include "parser.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <utility>

using Path = Parser::Path;

// <block/pin name, block/pin>
std::unordered_map<std::string, Block*> block_mp;
std::unordered_map<std::string, Pin*> pin_mp;

Parser::Parser() {}

void Parser::readBlock(const Path &file_path, Input *input)
{
	std::ifstream file(file_path);

	if(!file.is_open())
	{
		std::cout << "Can't open file: " << file_path << std::endl;
		return;
	}

	std::string line;

	// ignore first 3 line
	for(int i = 0; i < 3; i++)
		std::getline(file, line);

	std::string name, _;
	while(std::getline(file, line))
	{
		// block info end, break
		if(line == "" || line[0] == 'p' || line[0] == ' ')
			break;

		std::stringstream ss(line);

		ss >> name >> _ >> _;

		std::vector<int> X, Y;
		for(int i = 0; i < 4; i++)
		{
			// get '('
			while(ss && ss.peek() != '(')
				ss.get();
			ss.get();

			int x = 0, y = 0;
			ss >> x;

			// get ','
			while(ss && ss.peek() != ',')
				ss.get();
			ss.get();

			ss >> y;

			// get ')'
			while(ss && ss.peek() != ')')
				ss.get();
			ss.get();

			X.push_back(x);
			Y.push_back(y);
		}

		// calculate width & height
		int width = X[2] - X[1];
		int height = Y[1] - Y[0];
		std::pair<int, int> bottom_left = {X[0], Y[0]};
		Block::ptr block = std::make_unique<Block>(name, bottom_left, width, height);

		// add area to score
		block->score += width * height;

		// move pointer in block
		input->blocks.push_back(std::move(block));
		input->block_area += width * height;

		block_mp[name] = input->blocks.back().get();
	}

	file.close();
}

void Parser::readPin(const Path &file_path, Input *input)
{
	std::ifstream file(file_path);

	if(!file.is_open())
	{
		std::cout << "Can't open file: " << file_path << std::endl;
		return;
	}

	std::string line, name;
	int x = 0, y = 0;
	while(std::getline(file, line))
	{
		std::stringstream ss(line);

		ss >> name >> x >> y;

		std::pair<int, int> point = {x, y};
		Pin::ptr pin = std::make_unique<Pin>(name, point);

		input->pins.push_back(std::move(pin));

		pin_mp[name] = input->pins.back().get();
	}

	file.close();
}

void Parser::readNet(const Path &file_path, Input *input)
{
	std::ifstream file(file_path);

	if(!file.is_open())
	{
		std::cout << "Can't open file: " << file_path << std::endl;
		return;
	}

	std::string line, name, _;

	// ignore first 2 line
	for(int i = 0; i < 2; i++)
		std::getline(file, line);

	while(std::getline(file, line))
	{
		Net::ptr net = std::make_unique<Net>();

		std::stringstream ss(line);

		int n = 0;
		ss >> _ >> _ >> n;

		for(int i = 0; i < n; i++)
		{
			std::getline(file, line);

			ss.clear();
			ss.str(line);
			ss >> name;

			if(pin_mp.find(name) != pin_mp.end())
				net->pins.push_back(pin_mp[name]);

			if(block_mp.find(name) != block_mp.end())
				net->blocks.push_back(block_mp[name]);
		}

		// calculate weight score for each block
		double e = net->blocks.size() + net->pins.size() - 1;
		if(e != 0)
		{
			for(Block *block : net->blocks)
				block->score += 1/e;
		}

		// add weight score for each block if current net have pin
		if(net->pins.size() != 0)
		{
			for(Block *block : net->blocks)
				block->score += 1/e;
		}

		input->nets.push_back(std::move(net));
	}

	file.close();
}

Input::ptr Parser::parseInput(const Path &block_path, const Path &pin_path, const Path &net_path)
{
	Input::ptr input = std::make_unique<Input>();

	readBlock(block_path, input.get());
	readPin(pin_path, input.get());
	readNet(net_path, input.get());

	return input;
}
