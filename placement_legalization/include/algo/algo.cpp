// File: algo.cpp
// Author: YU-WEN WANG
// Created: 2026-08-01

#include "algo.hpp"
#include <algorithm>
#include <cmath>

Abacus::Abacus(Input *input)
{
	this->input = input;
	preProcess();
}

void Abacus::preProcess()
{
	const int n = input->rows.size();

	std::sort(input->rows.begin(), input->rows.end(), [](const Row::ptr &a, const Row::ptr &b) {return a->y < b->y;} );
	std::sort(input->blockages.begin(), input->blockages.end(), [](const Cell::ptr &a, const Cell::ptr &b) {return a->x < b->x;} );

	for(Cell::ptr &blockage : input->blocakges)
	{
		int y_start_idx = bsRowIndex(blockage->y);
		int y_end_idx = bsRowIndex(blockage->y + blocakge->height);

		for(int i = y_start_idx; i < y_end_idx; i++)
		{
			if(input->rows[i]->x_start >= blockage->x)
				input->rows[i]->x_start = std::max(input->rows[i]->x_start, blockage->x + blockage->width);
			else
			{
				Subrow::ptr subrow = std::make_unique<Subrow>(input->rows[i]->x_start, input->rows[i]->y, input->width, blockage->x - input->rows[i]->x_start);
				input->rows[i]->subrows.push_back(std::move(subrow));
				input->rows[i]->x_start = blockage->x + blockage->width;
			}
		}
	}

	for(Row::ptr &row : input->rows)
	{
		if(row->x_start < row->x_right)
		{
			Subrow::ptr subrow = std::make_unique<Subrow>(row->x_start, row->y, row->width, row->x_right - row->x_start);
			row->subrows.push_back(std::move(subrow));
			row->x_start = row->x_right;
		}
	}
}

int Abacus::bsRowIndex(int y, bool find_cell)
{
	const int n = input->rows.size();

	int l = 0, r = n - 1;
	while(l <= r)
	{
		int mid = l + (r - l)/2;

		if(input->rows[mid]->y == y)
			return mid;
		else if(input->rows[mid]->y > y)
			r = mid - 1;
		else
			l = mid + 1;
	}

	if(!find_cell)
		return l;
	else
	{
		if(l >= n)
			return l - 1;
		else
		{
			if(std::abs(input->rows[l]->y - y) >= std::abs(input->rows[l - 1]->y - y))
				return l - 1;
			else
				return l;
		}
	}
}

double Abacus::getCost(Cell *cell)
{
	double diff_x = cell->x - cell->x_global;
	double diff_y = cell->y - cell->y_global;

	return std::sqrt(diff_x*diff_x + diff_y*diff_y);
}

std::pair<int, double> Abacus::place(Row *row, Cell *cell)
{
}

void Abacus::process()
{
	std::sort(input->cells.begin(), input->cells.end(), [](const Cell::ptr &a, const Cell::ptr &b) {return a->x < b->x;} );

	for(Cell::ptr &cell : input->cells)
	{
		// get global placement row index
		int r = bsRowIndex(cell->y, true);

		auto [subrow_idx, cost] = place(input->rows[r].get(), cell.get());
	}
}	
