// File: main.cpp
// Author: YU-WEN WANG
// Created: 2026-08-15

#include "../include/parser/parser.hpp"
#include "../include/structure/structure.hpp"
#include <filesystem>
#include <iostream>
#include <chrono>

using Path = std::filesystem::path;
using time_clock = std::chrono::steady_clock;

int main(int argc, char** argv)
{
	// read filename from command
	Path input_dir = "testcase";
	Path file_name = "";

	if(argc == 1)
		file_name = "ibm01";
	else if(argc == 2)
		file_name = argv[1];
	else
	{
		std::cout << "Usage: make run" << std::endl;
		std::cout << "Usage: ./main" << std::endl;
		std::cout << "Usage: ./main <file name>" << std::endl;
		return 1;
	}

	Path file_path = input_dir / file_name;
	file_path += ".modified.txt";

	auto t0 = time_clock::now();

	Parser parser;
	Input::ptr input = parser.parseInput(file_path);

	auto t1 = time_clock::now();

	return 0;
}
