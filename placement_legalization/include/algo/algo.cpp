// File: algo.cpp
// Author: YU-WEN WANG
// Created: 2026-08-01

#include "algo.hpp"
#include <algorithm>
#include <cmath>
#include <climits>
#include <limits>
#include <iostream>

Abacus::Abacus(Input *input)
{
	this->input = input;
}

void Abacus::preProcess()
{
	const int n = input->rows.size();

	std::sort(input->rows.begin(), input->rows.end(), [](const Row::ptr &a, const Row::ptr &b) {return a->y < b->y;} );
	std::sort(input->blockages.begin(), input->blockages.end(), [](const Cell::ptr &a, const Cell::ptr &b) {return a->x_global < b->x_global;} );

	for(Cell::ptr &blockage : input->blockages)
	{
		int y_start_idx = bsRowIndex(blockage->y_global);
		int y_end_idx = bsRowIndex(blockage->y_global + blockage->height);

		for(int i = y_start_idx; i < y_end_idx; i++)
		{
			if(input->rows[i]->x_start >= blockage->x_global)
				input->rows[i]->x_start = std::max(input->rows[i]->x_start, static_cast<int>(blockage->x_global) + blockage->width);
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

int Abacus::bsRowIndex(double y, bool find_cell)
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
			if(l > 0 && std::abs(input->rows[l]->y - y) >= std::abs(input->rows[l - 1]->y - y))
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
	double x = cell->x_global;
	int width = cell->width;

	std::vector<int> candidate_idxs;
	for(int i = 0; i < row->subrows.size(); i++)
		if(row->subrows[i]->free_space >= width)
			candidate_idxs.push_back(i);

	int best_idx = -1, best_dist = INT_MAX;
	for(int &idx : candidate_idxs)
	{
		int x_l = row->subrows[idx]->x_left;
		int x_r = row->subrows[idx]->x_right;

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

	if(subrow_idx == -1)
		return {-1, std::numeric_limits<double>::infinity()};

	Subrow *subrow = row->subrows[subrow_idx].get();

	double cell_x = cell->x_global;
	if(cell_x < subrow->x_left)
		cell_x = subrow->x_left;
	else if(cell_x > subrow->x_right - cell->width)
		cell_x = subrow->x_right - cell->width;

	if(subrow->stk.empty() || subrow->stk.back()->x + subrow->stk.back()->width <= cell_x)
	{
		cell->x = cell_x;
		cell->y = row->y;
	}
	else
	{
		// weighted average
		int i = subrow->stk.size() - 1;

		double cluster_weight = subrow->stk[i]->weight + cell->weight;
		double cluster_q = subrow->stk[i]->q + cell->weight * (cell_x - subrow->stk[i]->width);
		double cluster_width = subrow->stk[i]->width + cell->width;

		double cluster_x = 0.0;

		for(i = subrow->stk.size() - 2; i >= 0; i--)
		{
			cluster_x = cluster_q / cluster_weight;

			if(cluster_x < subrow->x_left)
				cluster_x = subrow->x_left;
			else if(cluster_x > subrow->x_right - cluster_width)
				cluster_x = subrow->x_right - cluster_width;

			if(subrow->stk[i]->x + subrow->stk[i]->width < cluster_x)
				break;

			// merge cluster
			cluster_q = subrow->stk[i]->q + cluster_q - cluster_weight * subrow->stk[i]->width;
			cluster_weight += subrow->stk[i]->weight;
			cluster_width += subrow->stk[i]->width;
		}

		cluster_x = cluster_q / cluster_weight;
		if(cluster_x < subrow->x_left)
			cluster_x = subrow->x_left;
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

	subrow->free_space -= cell->width;

	double cell_x = cell->x_global;
	if(cell_x < subrow->x_left)
		cell_x = subrow->x_left;
	else if(cell_x > subrow->x_right - cell->width)
		cell_x = subrow->x_right - cell->width;

	if(subrow->stk.empty() || subrow->stk.back()->x + subrow->stk.back()->width <= cell_x)
	{
		Cluster::ptr cluster = std::make_unique<Cluster>(cell_x);
		cluster->weight += cell->weight;
		cluster->q += cell->weight * cell_x;
		cluster->width += cell->width;
		cluster->cells.push_back(cell);

		subrow->stk.push_back(std::move(cluster));
	}
	else
	{
		// weighted average
		int i = subrow->stk.size() - 1;

		subrow->stk[i]->weight += cell->weight;
		subrow->stk[i]->q += cell->weight * (cell_x - subrow->stk[i]->width);
		subrow->stk[i]->width += cell->width;
		subrow->stk[i]->x = subrow->stk[i]->q / subrow->stk[i]->weight;

		if(subrow->stk[i]->x < subrow->x_left)
			subrow->stk[i]->x = subrow->x_left;
		else if(subrow->stk[i]->x > subrow->x_right - subrow->stk[i]->width)
			subrow->stk[i]->x = subrow->x_right - subrow->stk[i]->width;

		subrow->stk[i--]->cells.push_back(cell);

		for(; i >= 0; i--)
		{

			if(subrow->stk[i]->x + subrow->stk[i]->width < subrow->stk[i + 1]->x)
				break;

			// merge cluster
			subrow->stk[i]->weight += subrow->stk[i + 1]->weight;
			subrow->stk[i]->q += subrow->stk[i + 1]->q - subrow->stk[i + 1]->weight * subrow->stk[i]->width;
			subrow->stk[i]->width += subrow->stk[i + 1]->width;
			subrow->stk[i]->x = subrow->stk[i]->q / subrow->stk[i]->weight;

			if(subrow->stk[i]->x < subrow->x_left)
				subrow->stk[i]->x = subrow->x_left;
			else if(subrow->stk[i]->x > subrow->x_right - subrow->stk[i]->width)
				subrow->stk[i]->x = subrow->x_right - subrow->stk[i]->width;

			subrow->stk[i]->cells.insert(subrow->stk[i]->cells.end(),
						     subrow->stk[i + 1]->cells.begin(),
						     subrow->stk[i + 1]->cells.end());

			subrow->stk.pop_back();
		}
	}
}

void Abacus::process()
{
	std::sort(input->cells.begin(), input->cells.end(), [](const Cell::ptr &a, const Cell::ptr &b) {return a->x_global < b->x_global;} );

	for(Cell::ptr &cell : input->cells)
	{
		// get global placement row index
		int r = bsRowIndex(cell->y_global, true);


		int best_row = -1, best_subrow = -1;
		double best_cost = std::numeric_limits<double>::infinity();

		// try to place row above
		for(int i = r; i >= 0; i--)
		{
			auto [subrow_idx, cost] = testPlace(input->rows[i].get(), cell.get());
			if(subrow_idx < 0 || !std::isfinite(cost))
				continue;
			if(cost < best_cost)
			{
				best_cost = cost;
				best_row = i;
				best_subrow = subrow_idx;
			}
			else
				break;
		}

		// try to place row below
		for(int i = r + 1; i < input->rows.size(); i++)
		{
			auto [subrow_idx, cost] = testPlace(input->rows[i].get(), cell.get());
			if(subrow_idx < 0 || !std::isfinite(cost))
				continue;
			if(cost < best_cost)
			{
				best_cost = cost;
				best_row = i;
				best_subrow = subrow_idx;
			}
			else
				break;
		}

		if(best_row < 0 || best_subrow < 0)
			continue;

		realPlace(input->rows[best_row]->subrows[best_subrow].get(), cell.get());
	}
}

std::pair<double, double> Abacus::calculateFinalResult()
{
	double total_cost = 0.0, max_cost = 0.0;
	for(Row::ptr &row : input->rows)
	{
		const int base = row->x_left;   // SubrowOrigin
		const int sitew = row->width;  // Sitewidth

		for(Subrow::ptr &subrow : row->subrows)
		{
			for(Cluster::ptr &cluster : subrow->stk)
			{
				// Snap cluster start to the row's site grid: base + k * sitew.
				// Keep the snapped position within the feasible subrow range.
				const int cluster_w = static_cast<int>(std::lround(cluster->width));
				const int min_x = subrow->x_left;
				const int max_x = subrow->x_right - cluster_w;

				double x0 = cluster->x;
				if(x0 < min_x)
					x0 = min_x;
				else if(x0 > max_x)
					x0 = max_x;

				auto snap_nearest = [&](double x) -> int {
					int k = static_cast<int>(std::lround((x - base) / static_cast<double>(sitew)));
					return base + k * sitew;
				};
				auto snap_up = [&](double x) -> int {
					int k = static_cast<int>(std::ceil((x - base) / static_cast<double>(sitew)));
					return base + k * sitew;
				};
				auto snap_down = [&](double x) -> int {
					int k = static_cast<int>(std::floor((x - base) / static_cast<double>(sitew)));
					return base + k * sitew;
				};

				int x = (sitew > 0) ? snap_nearest(x0) : static_cast<int>(std::lround(x0));
				if(sitew > 0)
				{
					if(x < min_x)
						x = snap_up(min_x);
					if(x > max_x)
						x = snap_down(max_x);
				}
				for(Cell *cell : cluster->cells)
				{
					cell->x = x;
					cell->y = row->y;

					x += cell->width;

					double cost = getCost(cell);
					total_cost += cost;
					max_cost = std::max(max_cost, cost);
				}
			}
		}
	}

	return {total_cost, max_cost};
}

Writer::ptr Abacus::solve()
{
	std::string line(32, '-');

	std::cout << "Start preprocess (divide row into subrow)" << std::endl;

	preProcess();

	std::cout << "Finish preprocess" << std::endl;
	std::cout << "Start process (Abacus main algorithm)" << std::endl;

	process();

	std::cout << "Finish process" << std::endl;
	std::cout << line << std::endl;
	std::cout << "Abacus algorithm result" << std::endl;

	auto [total_cost, max_cost] = calculateFinalResult();
	std::cout << "Total displacement cost: " << total_cost << std::endl;
	std::cout << "Max displacement cost: " << max_cost << std::endl;

	Writer::ptr writer = std::make_unique<Writer>();

	for(Cell::ptr &cell : input->cells)
		writer->addCell(cell.get());

	for(Cell::ptr &blockage : input->blockages)
		writer->addBlockage(blockage.get());

	return writer;
}
