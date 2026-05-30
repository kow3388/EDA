// File: writer.cpp
// Author: YU-WEN WANG
// Created: 2026-05-28

#include "writer.hpp"
#include <fstream>

Writer::Writer():
	cut_size(0), groups(2)
{
}

void Writer::setCutSize(int cut_size)
{
	this->cut_size = cut_size;
}

void Writer::addCell(Cell *input_cell)
{
	if(input_cell->group_id == 1)
		groups[0].push_back(input_cell->name);
	else
		groups[1].push_back(input_cell->name);
}

void Writer::writeResult(std::filesystem::path &file_name)
{
	// mkdir
	std::filesystem::path dir = "output";
	if(!std::filesystem::is_directory(dir))
		std::filesystem::create_directories(dir);

	std::filesystem::path out_path = dir / file_name;
	out_path += ".out";

	// write output file
	std::ofstream ofs(out_path);

	ofs << "cut_size " << cut_size << "\n";

	ofs << "A " << groups[0].size() << "\n";
	for(int i = 0; i < groups[0].size(); i++)
		ofs << groups[0][i] << "\n";

	ofs << "B " << groups[1].size() << "\n";
	for(int i = 0; i < groups[1].size(); i++)
		ofs << groups[1][i] << "\n";

	ofs.close();
}
