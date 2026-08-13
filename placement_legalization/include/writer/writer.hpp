// File: writer.hpp
// Author: YU-WEN WANG
// Created: 2026-08-09

#pragma once
#include "../structure/structure.hpp"
#include <filesystem>

class Writer
{
private:
	std::vector<std::string> names;
	std::vector<std::pair<int, int>> coordinates;
	std::vector<bool> isCells;
public:
	using ptr = std::unique_ptr<Writer>;
	using Path = std::filesystem::path;

	Writer();
	void addCell(Cell *cell);
	void addBlockage(Cell *blockage);
	void writeResult(Path &file_name);
};
