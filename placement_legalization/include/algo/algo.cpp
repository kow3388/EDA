// File: algo.cpp
// Author: YU-WEN WANG
// Created: 2026-08-01

#include "algo.hpp"
#include <algorithm>
#include <cmath>
#include <climit>
#include <limits>

Abacus::Abacus(Input *input)
{
	this->input = input;
}

void Abacus::preProcess()
{
	const int n = input->rows.size();

	std::sort(input->rows.begin(), input->rows.end(), [](const Row::ptr &a, const Row::ptr &b) {return a->y < b->y;} );
	std::sort(input->blockages.begin(), input->blockages.end(), [](const Cell::ptr &a, const Cell::ptr &b) {return a->x < b->x;} );

	for(Cell::ptr &blockage : input->blocakges)
	{
		int y_start_idx = bsRowIndex(blockage->y_global);
		int y_end_idx = bsRowIndex(blockage->y_global + blocakge->height);

		for(int i = y_start_idx; i < y_end_idx; i++)
		{
			if(input->rows[i]->x_start >= blockage->x_global)
				input->rows[i]->x_start = std::max(input->rows[i]->x_start, blockage->x_global + blockage->width);
			else
			{
				Subrow::ptr subrow = std::make_unique<Subrow>(input->rows[i]->x_start,
									      blockage->x_global,
									      input->rows[i]->y,
									      input->width,
									      blockage->x_global - input->rows[i]->x_start);

				input->rows[i]->subrows.push_back(std::move(subrow));
				input->rows[i]->x_start = blockage->x_global + blockage->width;
			}
		}
	}

	for(Row::ptr &row : input->rows)
	{
		if(row->x_start < row->x_right)
		{
			Subrow::ptr subrow = std::make_unique<Subrow>(row->x_start,
								      row->x_right,
								      row->y,
								      row->width,
								      row->x_right - row->x_start);
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

int Abacus::getSubrowIndex(Row *row, Cell *cell)
{
	int x = cell->x, width = cell->width;

	std::vector<int> candidate_idxs;
	for(int i = 0; i < row->subrows.size(); i++)
		if(row->subrows[i]->free_space >= width)
			candidate_idxs.push_back(i);

	int best_idx = -1, best_dist = INT_MAX;
	for(int &idx : candidate_idxs)
	{
		int x_l = row->subrows[i]->x_left;
		int x_r = row->subrows[i]->x_right;

		// calculate cell & subrow distance
		int cur_dist = 0;
		if(x < x_l)
			cur_dist = x_l - x;
		else if(x > x_r - width)
			cur_dist = x + width - x_r;

		if(best_dist > cur_dist)
		{
			best_idx = idx;
			best_dist = cur_dist;
		}
	}

	return best_idx;
}

std::pair<int, double> Abacus::testPlace(Row *row, Cell *cell)
{
	int subrow_idx = getSubrowIndex(row, cell);
	Subrow *subrow = row->subrows[r].get();

	double cell_x = cell->x_global;
	if(cell_x < subrow->x_left)
		cell_x = subrow->x_left;
	else if(cell_x > subrow->x_right - cell->width)
		cell_x = subrow->x_right - cell->width;

	if(subrow->stk.empty() || subrow->stk.back()->x + subrow->stk.back()->width < cell_x)
	{
		cell->x = cell_x;
		cell->y = row->y;
	}
	else
	{
		// weighted average
		int i = subrow->stk.size() - 1;

		double cluster_weight = subrow->stk[i]->weight + cell->weight;
		double cluster_q = subrow->stk[i]->q + cell->wieght * (cell_x - cell->width);
		double cluster_width = subrow->stk[i]->width + cell->width;

		double cluster_x = 0.0;

		for(i = subrow->stk.size() - 2; i >= 0; i--)
		{
			cluster_x = cluster_q / cluster_weight;

			if(cluster_x < subrow->x_left)
				clsuter_x = subrow->x_left;
			else if(cluster_x > subrow->x_right - cluster_width)
				cluster_x = subrow->x_right - cluster_width;

			if(subrow->stk[i]->x + subrow->stk[i]->width < cluster_x)
				break;

			// merge cluster
			cluster_weight += subrow->stk[i]->weight;
			cluster_q += subrow->stk[i]->q;
			cluster_width += subrow->stk[i]->width;
		}

		cluster_x = cluster_q / cluster_weight;
		if(cluster_x < subrow->x_left)
			clsuter_x = subrow->x_left;
		else if(cluster_x > subrow->x_right - cluster_width)
			cluster_x = subrow->x_right - cluster_width;

		cell->x = cluster_x + cluster_width - cell->width;
		cell->y = row->y;
	}

	double cost = getCost(cell);

	return {subrow_idx, cost};
}

void Abacus::realPlace(Subrow *subrow, Cell *cell)
{
	subrow->cells.push_back(cell);

	double cell_x = cell->x_global;
	if(cell_x < subrow->x_left)
		cell_x = subrow->x_left;
	else if(cell_x > subrow->x_right - cell->width)
		cell_x = subrow->x_right - cell->width;

	if(subrow->stk.empty() || subrow->stk.back()->x + subrow->stk.back()->width < cell_x)
	{
		Cluster::ptr cluster = std::make_unique<Cluster>(cell_x, cell->width);
		cluster->weight += cell->weight;
		cluster->q += cell->weight * (cell_x - cell->width);
		cluster->width += cell->width;

		subrow->stk.push_back(std::move(cluster));
	}
	else
	{
		// weighted average
		int n = subrow->stk.size();
		int i = n - 1;

		double cluster_weight = subrow->stk[i]->weight + cell->weight;
		double cluster_q = subrow->stk[i]->q + cell->wieght * (cell_x - cell->width);
		double cluster_width = subrow->stk[i]->width + cell->width;

		double cluster_x = 0.0;

		for(i = n - 2; i >= 0; i--)
		{
			cluster_x = cluster_q / cluster_weight;

			if(cluster_x < subrow->x_left)
				clsuter_x = subrow->x_left;
			else if(cluster_x > subrow->x_right - cluster_width)
				cluster_x = subrow->x_right - cluster_width;

			if(subrow->stk[i]->x + subrow->stk[i]->width < cluster_x)
				break;

			// merge cluster
			subrow->stk.pop_back();

			cluster_weight += subrow->stk[i]->weight;
			cluster_q += subrow->stk[i]->q;
			cluster_width += subrow->stk[i]->width;
		}

		cluster_x = cluster_q / cluster_weight;
		if(cluster_x < subrow->x_left)
			clsuter_x = subrow->x_left;
		else if(cluster_x > subrow->x_right - cluster_width)
			cluster_x = subrow->x_right - cluster_width;	

		subrow->stk.back()->x = cluster_x;
		subrow->stk.back()->weight = cluster_weight;
		subrow->stk.back()->q = cluster_q;
		subrow->stk.back()->width = cluster_width;
	}
}

void Abacus::process()
{
	std::sort(input->cells.begin(), input->cells.end(), [](const Cell::ptr &a, const Cell::ptr &b) {return a->x < b->x;} );

	for(Cell::ptr &cell : input->cells)
	{
		// get global placement row index
		int r = bsRowIndex(cell->y_global, true);


		int best_row = -1, best_subrow = -1;
		double best_cost = std::numeric_limits<double>::infinity();

		// try to place row above
		for(int i = r; i >= 0; i--)
		{
			auto [subrow_idx, cost] = place(input->rows[r].get(), cell.get());
			if(cost < best_cost)
			{
				cost = best_cost;
				best_row = i;
				best_subrow = subrow_idx;
			}
			else
				break;
		}

		// try to place row below
		for(int i = r + 1; i < input->rows.size(); i++)
		{
			auto [subrow_idx, cost] = place(input->rows[r].get(), cell.get());
			if(cost < best_cost)
			{
				cost = best_cost;
				best_row = i;
				best_subrow = subrow_idx;
			}
			else
				break;
		}

		realPlace(input->rows[best_row]->subrows[best_subrow].get(), cell.get());
	}
}	
