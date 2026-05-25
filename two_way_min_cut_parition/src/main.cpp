// File: main.cpp
// Author: YU-WEN WANG
// Created: 2026-05-17

#include "../include/parser/parser.hpp"
#include "../include/structure/structure.hpp"
#include "../include/algo/algo.hpp"
#include <string>
#include <iostream>

int main(int argc, char** argv)
{
	// read filename from command
	std::string cell_file_path = "";
	std::string net_file_path = "";
	if(argc == 3)
	{
		cell_file_path = argv[1];
		net_file_path = argv[2];
	}
	else if(argc == 1)
	{
		cell_file_path = "./testcase/p2-1.cells";
		net_file_path = "./testcase/p2-1.nets";
	}
	else
	{
		std::cout << "Usage: ./main <file.cells> <file.nets>" << std::endl;
		return 1;
	}

	Parser parser;
	Input *input = parser.parseInput(cell_file_path, net_file_path);

	/*
	for(Net *net : input->nets)
		if(net->cells.size() == 1)
			std::cout << net->name << std::endl;
	*/

	// FmAlgo *fm = new FmAlgo(input);
	// fm->solve();
	
	return 0;
};
