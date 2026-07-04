// File: writer.cpp
// Author: YU-WEN WANG
// Created: 2026-06-26

#include "writer.hpp"
#include <fstream>

Writer::Writer():
	wl(0)
{
}

void Writer::setWL(int wl)
{
	this->wl = wl;
}

void Writer::addBlock(Block *block)
{
	names.push_back(block->name);
	coordinates.push_back(block->bottom_left);
	rotate.push_back(block->rotate);
}

void Writer::writeResult(Writer::Path &file_name)
{
	// mkdir
	Writer::Path dir = "output";
	if(!std::filesystem::is_directory(dir))
		std::filesystem::create_directories(dir);

	Writer::Path out_path = dir / file_name;
	out_path += ".floorplan";

	// write output file
	std::ofstream ofs(out_path);

	ofs << "Wirelength " << wl << "\n";
	ofs << "Blocks\n";

	const int n = names.size();
	for(int i = 0; i < n; i++)
	{
		std::string name = names[i];
		int x = coordinates[i].first;
		int y = coordinates[i].second;
		int r = rotate[i];

		ofs << name << " " << x << " " << y << " " << r << "\n";
	}

	ofs.close();
}
