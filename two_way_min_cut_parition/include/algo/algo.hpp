// File: algo.hpp
// Author: YU-WEN WANG
// Created: 2026-05-16

#pragma once
#include "../structure/structure.hpp"
#include "../writer/writer.hpp"

class FmAlgo{
private:
	Input *input;
	GainBucketList::ptr bucket_list;
	int group1_size;
	int group2_size;
	int epochs;

	void initialGroup();
	void updateAllNet();
	void updateAllGain();
	void updateCellGain(Cell *input_cell);
	int iteration();
	int getCutSize();
public:
	FmAlgo(Input *input);
	Writer::ptr solve();
};
