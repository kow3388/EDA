// File: algo.cpp
// Author: YU-WEN WANG
// Created: 2026-08-17

#include "algo.hpp"
#include <algorithm>
#include <cstdlib>
#include <iostream>

A_Star::A_Star(Input *input)
{
	this->input = input;
	this->r_size = input->r_size;
	this->c_size = input->c_size;
	vertical.resize(input->r_size - 1, std::vector<Edge>(input->c_size, input->v_capacity));
	horizontal.resize(input->r_size, std::vector<Edge>(input->c_size - 1, input->h_capacity));

	dir = {1, 0, -1, 0, 1};
}

int A_Star::getWL()
{
	int wl = 0;
	for(Net::ptr &net : input->nets)
		wl += net->wl;

	return wl;
}

void A_Star::updateEdgeDemand(int y, int x, bool inc, Direction direction)
{
	Edge cur;
	if(direction == Direction::up)
		cur = vertical[y][x];
	else if(direction == Direction::left)
		cur = horizontal[y][x - 1];
	else if(direction == Direction::down)
		cur = vertical[y - 1][x];
	else
		cur = horizontal[y][x];

	if(inc)
		cur.demand++;
	else
		cur.demand--;
}

double A_Star::getEdgeCost(int y, int x, Direction &direction)
{
	Edge cur;
	if(direction == Direction::up)
		cur = vertical[y][x];
	else if(direction == Direction::left)
		cur = horizontal[y][x - 1];
	else if(direction == Direction::down)
		cur = vertical[y - 1][x];
	else
		cur = horizontal[y][x];

	double present = std::max(0, cur.demand - cur.capacity + 1);
	double history = cur.history * static_cast<double>(cur.demand + 1)/cur.capacity;

	return present + history;
}

double A_Star::getCost(const std::tuple<double, int, int> &target,
		       const std::tuple<double, int, int> &cur,
		       Direction &direction)
{
	auto [trg_cost, trg_y, trg_x] = target;
	auto [cur_cost, cur_y, cur_x] = cur;
	double next_cost = cur_cost;

	if(direction == Direction::up)
	{
		if(cur_y >= trg_y)
			next_cost++;
	}
	else if(direction == Direction::left)
	{
		if(cur_x <= trg_x)
			next_cost++;
	}
	else if(direction == Direction::down)
	{
		if(cur_y <= trg_y)
			next_cost++;
	}
	else
	{
		if(cur_x >= trg_x)
			next_cost++;
	}

	next_cost += getEdgeCost(cur_y, cur_x, direction);

	return next_cost;
}

void A_Star::wavePropagation(const std::tuple<double, int, int> &source,
			     const std::tuple<double, int, int> &target,
			     std::vector<std::vector<std::pair<Direction, double>>> &graph)
{
	auto [src_cost, src_y, src_x] = source;
	auto [trg_cost, trg_y, trg_x] = target;
	
	// A* initial
	graph[src_y][src_x] = {Direction::stop, 0.0};
	
	std::priority_queue<std::tuple<double, int, int>,
			    std::vector<std::tuple<double, int, int>>,
			    std::greater<std::tuple<double, int, int>>> pq;
	pq.push(source);

	// A*
	while(!pq.empty())
	{
		std::tuple<double, int, int> cur = pq.top();
		pq.pop();

		auto [cur_cost, cur_y, cur_x] = cur;

		// no need to move any more
		if(cur_y == trg_y && cur_x == trg_x)
			return;

		// keep moving the cost will be larger
		if(trg_cost != -1 && cur_cost >= trg_cost)
			continue;

		for(int d = 0; d < 4; d++)
		{
			int next_y = cur_y + dir[d];
			int next_x = cur_x + dir[d + 1];
			
			if(std::min(next_y, next_x) < 0 || next_y >= r_size || next_x >= c_size)
				continue;
			
			Direction direction = Direction::empty;
			if(d == 0)
				direction = Direction::up;
			else if(d == 1)
				direction = Direction::left;
			else if(d == 2)
				direction = Direction::down;
			else
				direction = Direction::right;

			double next_cost = getCost(target, cur, direction);

			// the cost is larger
			double next_graph_cost = graph[next_y][next_x].second;
			if(next_graph_cost != -1 && next_cost >= next_graph_cost)
				continue;

			graph[next_y][next_x] = {direction, next_cost};
			pq.push({next_cost, next_y, next_x});
		}
	}
}

void A_Star::backTrack(Net *net, std::vector<std::vector<std::pair<Direction, double>>> &graph)
{
	net->wl = 0;

	int x = net->pins[1].first, y = net->pins[1].second;

	while(graph[y][x].first != Direction::stop)
	{
		net->path.push_back({x, y});
		net->wl++;

		if(graph[y][x].first == Direction::up)
		{
			updateEdgeDemand(y, x, true, Direction::down);
			vertical[y - 1][x].nets.insert(net);
			y--;
		}
		else if(graph[y][x].first == Direction::left)
		{
			updateEdgeDemand(y, x, true, Direction::right);
			horizontal[y][x].nets.insert(net);
			x++;
		}
		else if(graph[y][x].first == Direction::down)
		{
			updateEdgeDemand(y, x, true, Direction::up);
			vertical[y][x].nets.insert(net);
			y++;
		}
		else
		{
			updateEdgeDemand(y, x, true, Direction::left);
			horizontal[y][x - 1].nets.insert(net);
			x--;
		}
	}

	net->path.push_back({x, y});
}

void A_Star::routNet(Net *net)
{
	std::pair<int, int> source = net->pins[0];
	std::pair<int, int> target = net->pins[1];

	std::vector<std::vector<std::pair<Direction, double>>> graph(r_size, std::vector<std::pair<Direction, double>>(c_size, {Direction::empty, -1.0}));

	// cost, y, x
	std::tuple<double, int, int> src = {0, source.second, source.first};
	std::tuple<double, int, int> trg = {-1, target.second, target.first};

	wavePropagation(src, trg, graph);
	backTrack(net, graph);
}

int A_Star::getOverflow()
{
	int overflow = 0;
	for(std::vector<Edge> &v_row : vertical)
		for(Edge &edge : v_row)
			overflow += std::max(0, edge.demand - edge.capacity);

	for(std::vector<Edge> &h_row : horizontal)
		for(Edge &edge : h_row)
			overflow += std::max(0, edge.demand - edge.capacity);

	return overflow;
}

std::priority_queue<Edge*, std::vector<Edge*>, Edge> A_Star::getRipupNet()
{
	std::priority_queue<Edge*, std::vector<Edge*>, Edge> pq;
	for(std::vector<Edge> &v_row : vertical)
	{
		for(Edge &edge : v_row)
		{
			if(edge.demand > edge.capacity)
			{
				edge.history++;
				pq.push(&edge);
			}
		}
	}

	for(std::vector<Edge> &h_row : horizontal)
	{
		for(Edge &edge : h_row)
		{
			if(edge.demand > edge.capacity)
			{
				edge.history++;
				pq.push(&edge);
			}
		}
	}

	return pq;
}

void A_Star::ripupRerout(std::unordered_set<Net*> nets)
{
	std::priority_queue<Net*, std::vector<Net*>, Net> pq;
	for(Net *net : nets)
	{
		net->overflow = 0;
		for(int i = 1; i < net->path.size(); i++)
		{
			std::pair<int, int> point1 = net->path[i - 1];
			std::pair<int, int> point2 = net->path[i];

			int x1 = point1.first, y1 = point1.second;
			int x2 = point2.first, y2 = point2.second;

			Edge cur;
			if(y1 > y2)
				cur = vertical[y1 - 1][x1];
			else if(y1 < y2)
				cur = vertical[y1][x1];
			else if(x1 > x2)
				cur = horizontal[y1][x1 - 1];
			else if(x1 < x2)
				cur = horizontal[y1][x1];
			else
				continue;

			net->overflow += std::max(0, cur.demand - cur.capacity);
			cur.demand--;
			cur.nets.erase(net);
		}

		net->path.clear();
		pq.push(net);
	}

	while(!pq.empty())
	{
		Net *net = pq.top();
		pq.pop();

		routNet(net);
	}
}

Writer::ptr A_Star::solve()
{
	std::string line(32, '_');

	std::sort(input->nets.begin(), input->nets.end(), [](const Net::ptr &a, const Net::ptr &b){
		  int manhattan1 = std::abs(a->pins[0].first - a->pins[1].first) + std::abs(a->pins[0].second - a->pins[1].second);
		  int manhattan2 = std::abs(b->pins[0].first - b->pins[1].first) + std::abs(b->pins[0].second - b->pins[1].second);

		  return manhattan1 > manhattan2;
	});

	for(Net::ptr &net : input->nets)
		routNet(net.get());

	int overflow = getOverflow();
	int wl = getWL();

	std::cout << "Initial route result" << std::endl;
	std::cout << "Overflow: " << overflow << std::endl;
	std::cout << "WL: " << wl << std::endl;
	std::cout << line << std::endl;

	// ripup need to remove Edge demand first
	while(overflow)
	{
		std::priority_queue<Edge*, std::vector<Edge*>, Edge> pq = getRipupNet();
		while(overflow && !pq.empty())
		{
			Edge *edge = pq.top();
			pq.pop();

			ripupRerout(edge->nets);

			overflow = getOverflow();
		}
	}

	wl = getWL();

	std::cout << "Final route result" << std::endl;
	std::cout << "Overflow: " << overflow << std::endl;
	std::cout << "WL: " << wl << std::endl;

	Writer::ptr writer = std::make_unique<Writer>();
	for(Net::ptr &net : input->nets)
		writer->addNet(net.get());

	return writer;
}
