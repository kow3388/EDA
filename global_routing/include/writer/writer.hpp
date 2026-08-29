// File: writer.hpp
// Author: YU-WEN WANG
// Created: 2026-08-28

#pragma once
#include "../structure/structure.hpp"
#include <filesystem>

class Writer
{
private:
	std::vector<Net*> nets;
public:
	using ptr = std::unique_ptr<Writer>;
	using Path = std::filesystem::path;

	Writer();
	void addNet(Net* net);
	void writeResult(Path &file_name);
};
