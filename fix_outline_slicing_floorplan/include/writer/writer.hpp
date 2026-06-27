// File: writer.hpp
// Author: YU-WEN WANG
// Created: 2026-06-26

#pragma once
#include "../structure/structure.hpp"
#include <vector>
#include <filesystem>
#include <memory>
#include <string>
#include <utility>

class Writer
{
private:
	int wl;
	std::vector<std::string> names;
	std::vector<std::pair<int, int>> coordinates;
	std::vector<bool> rotate;
public:
	using ptr = std::unique_ptr<Writer>;
	using Path = std::filesystem::path;

	Writer();
	void setWL(int wl);
	void addBlock(Block *block);
	void writeResult(Path &file_name);
};
