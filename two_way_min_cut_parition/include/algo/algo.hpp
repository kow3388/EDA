// File: algo.hpp
// Author: YU-WEN WANG
// Created: 2026-05-16

#pragma once
#include "../structure/structure.hpp"
#include "../writer/writer.hpp"

class FmAlgo{
private:
	Input *input;
	Group *group1;
	Group *group2;
	GainBucketList *bucket_list;
	Writer *writer;

	void initialGroup();
	void updateAllNet();
	void updateAllGain();
	void updateCellGain(Cell *input_cell);
	int iteration();
	int getCutSize();
public:
	FmAlgo(Input *input);
	Writer* solve();
};
