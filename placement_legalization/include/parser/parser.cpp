// File: parser.cpp
// Author: YU-WEN WANG
// Created: 2026-07-31

#include "parser.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <utility>
#include <cctype>

using Path = Parser::Path;

// <cell/blockage name, cell/blockage>
std::unordered_map<std::string, Cell*> cell_mp;
std::unordered_map<std::string, Cell*> blockage_mp;

Parser::Parser() {}

std::string Parser::strip(const std::string& s) const
{
	int start = 0;
	while(start < s.size() &&
	      std::isspace(static_cast<unsigned char>(s[start])))
		++start;

	int end = s.size();
	while(end > start &&
	      std::isspace(static_cast<unsigned char>(s[end - 1])))
		--end;

	return s.substr(start, end - start);
}

std::vector<Path> Parser::readAux(Path testcase_dir, Path file_path, Input *input)
{
	// read aux file to get the file name we need
	std::ifstream file(file_path);
	if(!file.is_open())
	{
		std::cout << "Can't open file: " << file_path << std::endl;
		return {};
	}

	std::string line;
	std::getline(file, line);

	std::string _;
	Path node, pl, scl;

	std::stringstream ss(line);
	ss >> _ >> _ >> node >> pl >> scl;

	std::vector<Path> paths = {testcase_dir/node, testcase_dir/pl, testcase_dir/scl};

	int max_displace = 0;

	std::getline(file, line);
	ss << line;
	ss >> _ >> _ >> max_displace;

	input->max_displace = max_displace;

	return paths;
}

void Parser::readNode(Path file_path, Input *input)
{
	std::ifstream file(file_path);
	if(!file.is_open())
	{
		std::cout << "Can't open file: " << file_path << std::endl;
		return;
	}

	std::string line;

	std::string name, _;
	int width = 0, height = 0;

	// read untile node name
	while(std::getline(file, line))
	{
		// skip space empty
		line = strip(line);
		if(line.empty())
			continue;

		std::stringstream ss(line);

		ss >> name;
		if(name == "NumTerminals")
			break;
	}

	while(std::getline(file, line))
	{
		// skip space empty
		line = strip(line);
		if(line.empty())
			continue;

		std::stringstream ss(line);

		ss >> name >> width >> height;

		Cell::ptr cell = std::make_unique<Cell>(name, 0, 0, width, height);
		
		// blockage
		if(ss >> _)
		{
			input->blockages.push_back(std::move(cell));
			blockage_mp[name] = input->blockages.back().get();
		}
		// cell
		else
		{
			input->cells.push_back(std::move(cell));
			cell_mp[name] = input->blockages.back().get();
		}
	}
}

void Parser::readPl(Path file_path)
{
	std::ifstream file(file_path);
	if(!file.is_open())
	{
		std::cout << "Can't open file: " << file_path << std::endl;
		return;
	}

	std::string line;

	std::string name, _;
	int x = 0, y = 0;

	// read untile node name
	while(std::getline(file, line))
	{
		// skip space empty
		line = strip(line);
		if(line.empty())
			continue;

		std::stringstream ss(line);

		ss >> name;
		if(name == "UCLA")
			break;
	}

	while(std::getline(file, line))
	{
		// skip space empty
		line = strip(line);
		if(line.empty())
			continue;

		std::stringstream ss(line);

		ss >> name >> x >> y >> _;

		// blockage
		if(ss >> _)
		{
			Cell *blockage = blockage_mp[name];
			blockage->x = x;
			blockage->y = y;
		}
		// cell
		else
		{
			Cell *cell = cell_mp[name];
			cell->x = x;
			cell->y = y;
		}
	}
}

void Parser::readScl(Path file_path, Input *input)
{
	std::ifstream file(file_path);
	if(!file.is_open())
	{
		std::cout << "Can't open file: " << file_path << std::endl;
		return;
	}

	std::string line;

	std::string str, _;

	// read untile node name
	while(std::getline(file, line))
	{
		// skip space empty
		line = strip(line);
		if(line.empty())
			continue;

		std::stringstream ss(line);

		ss >> str;
		if(str == "NumRows")
			break;
	}

	int x_left = 0, y = 0;
	int width = 0, height = 0, num = 0;

	while(std::getline(file, line))
	{
		// skip space empty
		line = strip(line);
		if(line.empty())
			continue;
	
		Row::ptr row = std::make_unique<Row>();

		// get row information
		while(std::getline(file, line))
		{
			std::stringstream ss(line);

			ss >> str;
			if(str == "CoreRow" || str == "Sitespacing" || str == "Siteorient" || str == "Sitesymmetry")
				continue;
			else if(str == "Corrdinate")
			{
				ss >> _ >> y;
				row->y = y;
			}
			else if(str == "SiteWidth")
			{
				ss >> _ >> width;
				row->width = width;
			}
			else if(str == "SubrowOrigin")
			{
				ss >> _ >> x_left >> _ >> _ >> num;
				row->x_left = x_left;
				row->x_start = x_left;
				row->x_right = x_left + width * (num + 1);
			}
			else
				break;
		}

		input->rows.push_back(std::move(row));
	}
}

Input::ptr Parser::parseInput(Path case_name)
{
	Input::ptr input = std::make_unique<Input>();

	Path input_dir = "testcase";
	Path testcase_dir = input_dir / case_name;

	// find the file end with .aux
	std::string ext = ".aux";
	Path file_name = "";
	for(const auto &e : std::filesystem::directory_iterator(testcase_dir))
	{
		if(e.is_regular_file() && e.path().extension() == ext)
		{
			file_name = e.path();
			break;
		}
	}

	Path file_path = testcase_dir / file_name;
	std::vector<Path> paths = readAux(testcase_dir, file_path, input.get());

	readNode(paths[0], input.get());
	readPl(paths[1]);
	readScl(paths[2], input.get());

	input->width = input->rows[0]->width;
	input->height = input->rows[0]->height;

	return input;
}
