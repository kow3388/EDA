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

	void preProcess();						// divide all row in to subrow due to blockages
	int bsRowIndex(int y, bool find_cell=false);			// binary search find the cell & blockage's row
	double getCost(Cell *cell);					// calculate the move cost
	int getSubrowIndex(Row *row, Cell *cell);			// get the subrow index by giving row & cell
	std::pair<int, double> testPlace(Row *row, Cell *cell);		// try to place cell in row return {subrow index, place cost}
	void realPlace(Subrow *subrow, Cell *cell);			// real place cell in row
	void process();							// abacus main algo
	std::pair<double, double> calculateFinalResult();		// calculate the final coordidante of cell & placement cost, return {total cost, max cost}
public:
	Abacus(Input *input);
	writer::ptr solve();
};
