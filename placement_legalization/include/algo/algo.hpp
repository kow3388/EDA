// File: algo.hpp
// Author: YU-WEN WANG
// Created: 2026-08-01

#pragma once
#include "../structure/structure.hpp"
#include "../writer/writer.hpp"
#include <utility>

class Abacus
{
private:
	Input *input;

	void preProcess();	// divide all row in to subrow due to blockages
	int bsRowIndex(int y, bool find_cell=false);	// binary search find the cell & blockage's row
	double getCost(Cell *cell);
	int getSubrowIndex(Row *row, Cell *cell);	// get the subrow index by giving row & cell
	std::pair<int, double> place(Row *row, Cell *cell);	// try to place cell in row return {subrow index, place cost}
	void process();
public:
	Abacus(Input *input);
	writer::ptr solve();
};
