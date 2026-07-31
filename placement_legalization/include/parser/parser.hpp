// File: parser.hpp
// Author: YU-WEN WANG
// Created: 2026-07-31

#pragma once
#include "../structure/structure.hpp"
#include <filesystem>

class Parser
{
private:
	std::vector<Path> readAux(Path testcase_dir, Path file_path);
	void readNode(Path file_path, Input *input);
	void readPl(Path file_path);
	void readScl(Path file_path, Input *input);
public:
	using Path = std::filesystem::path;

	Parser();
	Input::ptr parseInput(Path case_name);
};
