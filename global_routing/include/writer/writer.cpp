// File: writer.cpp
// Author: YU-WEN WANG
// Created: 2026-08-28

#include "writer.hpp"
#include <algorithm>
#include <fstream>

Writer::Writer() {}

void Writer::addNet(Net* net)
{
	nets.push_back(net);
}

void Writer::writeResult(Path &file_name)
{
	// mkdir
	Path dir = "output";
	if(!std::filesystem::is_directory(dir))
		std::filesystem::create_directories(dir);

	Path out_path = dir / file_name;
	out_path += ".result";

	std::sort(nets.begin(), nets.end(), [](const Net *a, const Net *b) {return a->id < b->id;});

	// write output file
	std::ofstream ofs(out_path);

	for(Net *net : nets)
	{
		std::string name = net->name;
		int id = net->id;
		std::vector<std::pair<int, int>> path = net->path;

		ofs << name << " " << id << "\n";
		for(int i = path.size() - 1; i > 0; i--)
		{
			std::pair<int, int> point1 = path[i];
			std::pair<int, int> point2 = path[i - 1];

			int x1 = point1.first, y1 = point1.second;
			int x2 = point2.first, y2 = point2.second;

			ofs << "(" << x1 << ", " << y1 << ", 1)-(" << x2 << ", " << y2 << ", 1)\n";
		}

		ofs << "!\n";
	}
}
