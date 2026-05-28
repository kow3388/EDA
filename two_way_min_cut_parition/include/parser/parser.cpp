// File: parser.cpp
// Author: YU-WEN WANG
// Created: 2026-05-16

#include "parser.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

// <cell name, cell>
std::unordered_map<std::string, Cell*> mp;

Parser::Parser() {}

// read cell file
void Parser::readCell(std::filesystem::path &file_path, Input *input)
{
	std::ifstream file(file_path);

	if(!file.is_open()){
		std::cout << "Can't open file: " << file_path << std::endl;
		return;
	}

	// read each line & create cell
	std::string line;
	while(std::getline(file, line)){
		std::stringstream ss(line);

		std::string name;
		int size = 0;

		ss >> name >> size;
		Cell *cell = new Cell(name, size);
		input->cells.push_back(cell);

		// record in hashtable to quickly find cell for net
		mp[name] = cell;

		// calculate cell size diff spec
		input->diff_spec += static_cast<double>(size);
	}

	file.close();
}

void Parser::readNet(std::filesystem::path &file_path, Input *input)
{
	std::ifstream file(file_path);

	if(!file.is_open()){
		std::cout << "Can't open file: " << file_path << std::endl;
		return;
	}

	// Token-based parsing so a net can span multiple lines.
	std::string tok;
	while(file >> tok)
	{
		// get net name and _ get dummy token
		std::string net_name, _;
		file >> net_name >> _;

		Net *net = new Net(net_name);

		bool closed = false;
		while(file >> tok)
		{
			if(tok == "}")
			{
				closed = true;
				break;
			}

			Cell *cell = mp[tok];
			net->cells.push_back(cell);
			cell->nets.push_back(net);
		}

		input->nets.push_back(net);
	}

	file.close();
}

Input* Parser::parseInput(std::filesystem::path &cell_path, std::filesystem::path &net_path)
{
	Input *input = new Input();

	readCell(cell_path, input);
	readNet(net_path, input);

	// cacluate legal spec
	input->diff_spec /= 10;

	// calculate max degree of all cell
	for(Cell *cell : input->cells)
		input->max_degree = std::max(input->max_degree, static_cast<int>(cell->nets.size()));

	return input;
}
