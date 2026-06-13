// File: parser.hpp
// Author: YU-WEN WANG
// Created: 2026-06-10

#pragma once
#include "../structure/structure.hpp"
#include <filesystem>

class Parser
{
public:
	using Path = std::filesystem::path;

private:
	void readBlock(const Path &file_path, Input *input);
	void readPin(const Path &file_path, Input *input);
	void readNet(const Path &file_path, Input *input);
public:
	Parser();
	Input::ptr parseInput(const Path &block_path, const Path &pin_path, const Path &net_path);
};
