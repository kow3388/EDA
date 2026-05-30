// File: parser.hpp
// Author: YU-WEN WANG
// Created: 2026-05-16

#pragma once
#include "../structure/structure.hpp"
#include <filesystem>

// parse input file to get all cell & net
class Parser
{
private:
	void readCell(std::filesystem::path &file_path, Input *input);
	void readNet(std::filesystem::path &file_path, Input *input);
public:
	Parser();
	Input::ptr parseInput(std::filesystem::path &cell_path, std::filesystem::path &net_path);
};
