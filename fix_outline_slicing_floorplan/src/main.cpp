// File: main.cpp
// Author: YU-WEN WANG
// Created: 2026-06-13

#include "../include/parser/parser.hpp"
#include "../include/structure/structure.hpp"
#include "../include/algo/algo.hpp"
#include "../include/writer/writer.hpp"
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

	double dead_space_ratio = 0.0;

	if(argc == 1)
	{
		file_name = "n100";
		dead_space_ratio = 0.2;
	}
	else if(argc == 3)
	{
		file_name = argv[1];
		try
		{
			dead_space_ratio = std::stod(argv[2]);
		}
		catch (const std::exception &e)
		{
			std::cerr << "Invalid input" << argv[2] << std::endl;
			return 1;
		}
	}
	else
	{
		std::cout << "Usage: make run" << std::endl;
		std::cout << "Usage: make run <file name> <dead space ratio>" << std::endl;
		std::cout << "Usage: ./main" << std::endl;
		std::cout << "Usage: ./main <file name> <dead space ratio>" << std::endl;

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

	input->dead_space_ratio = dead_space_ratio;

	WongLuiAlgo wl_algo(input.get());
	Writer::ptr writer = wl_algo.solve();

	// no valid solution
	if(!writer)
	{
		std::cout << "No valid solution" << std::endl;
		return 1;
	}

	auto t2 = time_clock::now();

	writer->writeResult(file_name);

	auto t3 = time_clock::now();

	auto ms = [](auto d){
		return std::chrono::duration<double, std::milli>(d).count();
	};

	std::string line(32, '-');
	std::cout << line << std::endl;
	std::cout << "Input time consume: " << ms(t1 - t0) << std::endl;
	std::cout << "Algo time consume: " << ms(t2 - t1) << std::endl;
	std::cout << "Output time consume: " << ms(t3 - t2) << std::endl;

	return 0;
}
