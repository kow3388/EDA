// File: main.cpp
// Author: YU-WEN WANG
// Created: 2026-06-13

#include "../include/parser/parser.hpp"
#include "../include/structure/structure.hpp"
#include "../include/algo/algo.hpp"
#include <iostream>
#include <filesystem>
#include <vector>
#include <memory>
#include <chrono>

using time_clock = std::chrono::steady_clock;
using Path = std::filesystem::path;

int main(int argc, char** argv)
{
	// read filename from command
	Path inpu_dir = "testcase";
	Path block_file_path = "";
	Path pin_file_path = "";
	Path net_file_path = "";
	Path file_name = "";

	if(argc == 1)
		file_name = "n100";
	else if(argc == 2)
		file_name = argv[1];
	else
	{
		std::cout << "Usage: make run" << std::endl;
		std::cout << "Usage: ./main" << std::endl;
		std::cout << "Usage: ./main <file name>" << std::endl;

		return 1;
	}

	block_file_path = inpu_dir / file_name;
	pin_file_path = inpu_dir / file_name;
	net_file_path = inpu_dir / file_name;

	block_file_path += ".hardblocks";
	pin_file_path += ".pl";
	net_file_path += ".nets";

	auto t0 = time_clock::now();

	Parser parser;
	Input::ptr input = parser.parseInput(block_file_path, pin_file_path, net_file_path);

	auto t1 = time_clock::now();

	// sorted for better initial
	input->sortBlock();

	// dead space ratio need to run (can add more dead space ratio if needed)
	std::vector<double> dead_space_ratios = {0.2, 0.15};
	for(double &d : dead_space_ratios)
	{
		input->dead_space_ratio = d;

		WongLuiAlgo wl(input.get());
		wl.initial();

		std::cout << wl.isValid() << std::endl;
		std::cout << "=================================" << std::endl;
	}

	return 0;
}
