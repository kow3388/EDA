// File: main.cpp
// Author: YU-WEN WANG
// Created: 2026-05-17

#include "../include/parser/parser.hpp"
#include "../include/structure/structure.hpp"
#include "../include/algo/algo.hpp"
#include "../include/writer/writer.hpp"
#include <filesystem>
#include <iostream>
#include <memory>
#include <chrono>

using time_clock = std::chrono::steady_clock;

int main(int argc, char** argv)
{
	// read filename from command
	std::filesystem::path input_dir = "testcase";
	std::filesystem::path cell_file_path = "";
	std::filesystem::path net_file_path = "";
	std::filesystem::path file_name = "";
	if(argc == 1)
		file_name = "p2-1";
	else if(argc == 2)
		file_name = argv[1];
	else
	{
		std::cout << "Usage: make run" << std::endl;
		std::cout << "Usage: ./main" << std::endl;
		std::cout << "Usage: ./main <file name>" << std::endl;
		return 1;
	}

	cell_file_path = input_dir / file_name;
	net_file_path = input_dir / file_name;

	cell_file_path += ".cells";
	net_file_path += ".nets";

	auto t0 = time_clock::now();

	Parser parser;
	Input::ptr input = parser.parseInput(cell_file_path, net_file_path);

	auto t1 = time_clock::now();

	FmAlgo fm(input.get());
	Writer::ptr writer = fm.solve();

	auto t2 = time_clock::now();

	writer->writeResult(file_name);

	auto t3 = time_clock::now();
	
	auto ms = [](auto d){
		return std::chrono::duration<double, std::milli>(d).count();
	};

	std::cout << "--------------------------------" << std::endl;
	std::cout << "Input time consume: " << ms(t1 - t0) << std::endl;
	std::cout << "Algo time consume: " << ms(t2 - t1) << std::endl;
	std::cout << "Output time consume: " << ms(t3 - t2) << std::endl;
	
	return 0;
};
