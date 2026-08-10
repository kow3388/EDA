// File: main.cpp
// Author: YU-WEN WANG
// Created: 2026-08-09

#include "../include/parser/parser.hpp"
#include "../include/structure/structure.hpp"
#include "../include/algo/algo.hpp"
#include "../include/writer/writer.hpp"
#include <iostream>
#include <chrono>

using time_clock = std::chrono::steady_clock;
using Path = std::filesystem::path;

int main(int argc, char** argv)
{
	// read case name from command
	Path case_name = "";

	if(argc == 1)
		case_name = "ibm01";
	else if(argc == 2)
		case_name = argv[1];
	else
	{
		std::cout << "Usage: make run" << std::endl;
		std::cout << "Usage: ./main" << std::endl;
		std::cout << "Usage: ./main <case name>" << std::endl;

		return 1;
	}

	auto t0 = time_clock::now();

	Parser parser;
	Input::ptr input = parser.parseInput(case_name);

	auto t1 = time_clock::now();

	Abacus abacus_algo(input.get());
	Writer::ptr writer = abacus_algo.solve();

	auto t2 = time_clock::now();

	writer->writeResult(case_name);

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
