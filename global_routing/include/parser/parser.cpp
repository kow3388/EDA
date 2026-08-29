// File: parser.cpp
// Author: YU-WEN WANG
// Created: 2026-08-15

#include "parser.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

using Path = Parser::Path;

Parser::Parser() {}

void Parser::readTxt(const Path &file_path, Input *input)
{
	std::ifstream file(file_path);

	if(!file.is_open())
	{
		std::cout << "Can't open file: " << file_path << std::endl;
		return;
	}

	std::string line;
	std::string name, _;

	// get grid information
	for(int i = 0; i < 4; i++)
	{
		std::getline(file, line);
		std::stringstream ss(line);

		if(i == 0)
		{
			int r_size = 0, c_size = 0;

			ss >> name >> c_size >> r_size;

			input->r_size = r_size;
			input->c_size = c_size;
		}
		else if(i == 1)
		{
			int v_capacity = 0;

			ss >> name >> _ >> v_capacity;

			input->v_capacity = v_capacity;
		}
		else if(i == 2)
		{
			int h_capacity = 0;

			ss >> name >> _ >> h_capacity;

			input->h_capacity = h_capacity;
		}
		else
			break;
	}

	// get net information
	while(std::getline(file, line))
	{
		std::stringstream ss(line);
		int id = 0;

		ss >> name >> id >> _;

		Net::ptr net = std::make_unique<Net>(name, id);
		int x = 0, y = 0;

		for(int i = 0; i < 2; i++)
		{
			std::getline(file, line);

			ss.clear();
			ss.str(line);
			ss >> x >> y >> _;

			net->pins.push_back({x, y});
		}

		input->nets.push_back(std::move(net));
	}
}

Input::ptr Parser::parseInput(const Path &file_path)
{
	Input::ptr input = std::make_unique<Input>();

	readTxt(file_path, input.get());

	return input;
}
