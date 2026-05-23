// File: main.cpp
// Author: YU-WEN WANG
// Created: 2026-05-17

#include "../include/parser/parser.hpp"
#include "../include/structure/structure.hpp"
#include "../include/algo/algo.hpp"
#include <string>
#include <iostream>

int main(){
	std::string cell_file_path = "./testcase/p2-1.cells";
	std::string net_file_path = "./testcase/p2-1.nets";

	Parser parser;
	Input *input = parser.parseInput(cell_file_path, net_file_path);

	FmAlgo *fm = new FmAlgo(input);
	fm->solve();
	
	std::cout << "--------------------------------" << std::endl;
	fm->checkResult();

	return 0;
};
