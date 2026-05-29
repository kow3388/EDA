// File: algo.hpp
// Author: YU-WEN WANG
// Created: 2026-05-16

#pragma once
#include "../structure/structure.hpp"
#include "../writer/writer.hpp"

class FmAlgo{
private:
	Input *input;
	Group::ptr group1;
	Group::ptr group2;
	GainBucketList::ptr bucket_list;

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
