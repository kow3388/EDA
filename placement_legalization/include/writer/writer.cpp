// File: writer.cpp
// Author: YU-WEN WANG
// Created: 2026-08-09

#include "writer.hpp"
#include <fstream>

Writer::Writer() {}

void Writer::addCell(Cell *cell)
{
	names.push_back(cell->name);
	coordinates.push_back({cell->x, cell->y});
	isCells.push_back(true);
}

void Writer::addBlockage(Cell *blockage)
{
	names.push_back(blockage->name);
	coordinates.push_back({blockage->x_global, blockage->y_global});
	isCells.push_back(false);
}

void Writer::writeResult(Path &file_name)
{
	// mkdir
	Path dir = "output";
	if(!std::filesystem::is_directory(dir))
		std::filesystem::create_directories(dir);

	Path out_path = dir / file_name;
	out_path += ".pl";

	// write output file
	std::ofstream ofs(out_path);

	const int n = names.size();
	for(int i = 0; i < n; i++)
	{
		std::string name = names[i];
		int x = coordinates[i].first;
		int y = coordinates[i].second;
		std::string type = isCells[i] ? "N" : "N /FIXED";

		ofs << name << " " << x << " " << y << " : " << type << "\n";
	}

	ofs.close();
}
