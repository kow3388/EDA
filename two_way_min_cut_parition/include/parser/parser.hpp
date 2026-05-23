// File: parser.hpp
// Author: YU-WEN WANG
// Created: 2026-05-16

#pragma once
#include "../structure/structure.hpp"
#include <string>

// parse input file to get all cell & net
class Parser{
private:
	void readCell(const std::string &file_path, Input *input);
	void readNet(const std::string &file_path, Input *input);
public:
	Parser();
	Input* parseInput(const std::string &cell_path, const std::string &net_path);
};
