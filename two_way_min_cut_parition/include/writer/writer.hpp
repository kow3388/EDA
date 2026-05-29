// File: writer.hpp
// Author: YU-WEN WANG
// Created: 2026-05-28

#pragma once
#include "../structure/structure.hpp"
#include <vector>
#include <string>
#include <filesystem>
#include <memory>

class Writer
{
private:
	int cut_size;
	std::vector<std::vector<std::string>> groups;
public:
	using ptr = std::unique_ptr<Writer>;

	Writer();
	void setCutSize(int cut_size);
	void addCell(Cell *ipnut_cell);
	void writeResult(std::filesystem::path &file_name);
};
