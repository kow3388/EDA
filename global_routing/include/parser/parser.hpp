// File: parser.hpp
// Author: YU-WEN WANG
// Created: 2026-08-15

#pragma once
#include "../structure/structure.hpp"
#include <filesystem>

class Parser
{
public:
	using Path = std::filesystem::path;
private:
	void readTxt(const Path &file_path, Input *input);
public:
	Parser();
	Input::ptr parseInput(const Path &file_path);
};
