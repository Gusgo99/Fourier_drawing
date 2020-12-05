/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
MIT License

Copyright (c) 2020 Gustavo Pacola Gonçalves

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef GRID_HPP
#define GRID_HPP
#pragma once

#include <iostream>

#include <array>
#include <utility>
#include <vector>

class grid {
public:
	using position = std::pair<size_t, size_t>;
	using offset = std::pair<int, int>;
	using iterator = std::vector<int>::iterator;
	using const_iterator = std::vector<int>::const_iterator;
	enum data {NOTHING, EDGE};
	
	grid(
		const size_t _numLines,
		const size_t _numColumns,
		const std::vector<int> &_gridData = {});
	grid(
		const position _size = {0, 0},
		const std::vector<int> &_gridData = {});
		
	void fill(const int _value);
	size_t num_lines() const;
	size_t num_columns() const;
	position get_size() const;

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
	
	int& operator()(const size_t _line, const size_t _column);
	int& operator()(const position _position);
	int operator()(const size_t _line, const size_t _column) const;
	int operator()(const position _position) const;
	
	position max_neighbour(const position _targetPosition) const;
	position min_neighbour(const position _targetPosition) const;
	std::vector<int> positions_data(const std::vector<position> _positions) const;
	std::array<int, 8> neighbours_data(const position _targetPosition) const;
	std::array<int, 4> diagonal_neighbours_data(const position _targetPosition) const;
	std::array<int, 4> non_diagonal_neighbours_data(const position _targetPosition) const;
	template<typename inputIt>
	static int count_regions(const inputIt _begin, const inputIt _end);
	grid distance_heightmap(const position _targetPosition) const;
	std::vector<position> descend_heightmap(const position _start) const;
	std::vector<position> find_every_value(const int _value) const;
	position find_value(const int _value) const;
	std::vector<position> get_contour() const;
	void remove_unconnected_cells(const position _targetPosition);
	bool remove_non_connecting_cells(const std::vector<position> &_positions);
	void skeletonize();
	std::vector<position> solve_chinese_postman();

	class cellNeighbourhood {
	public:
		cellNeighbourhood();
		cellNeighbourhood(const grid &_grid, const grid::position _position);

		int& operator[](const size_t _position);
		int operator[](const size_t _position) const;

		std::array<int, 4> diagonal_cells() const;
		std::array<int, 4> non_diagonal_cells() const;

		using iterator = std::array<int, 8>::iterator;
		using const_iterator = std::array<int, 8>::const_iterator;

		iterator begin();
		iterator end();
		const_iterator begin() const;
		const_iterator end() const;

		size_t size() const;

		bool does_cell_causes_connection() const;

	private:
		size_t count_connected_cells(const size_t _startIndex) const;

		std::array<int, 8> neighbourData;
	};

	cellNeighbourhood get_cell_neighbourhood(const position _position) const;
	
private:
	position size;
	std::vector<int> gridData;
	
	static constexpr std::array<grid::offset, 8> NEIGHBOUROFFSET{
		grid::offset(-1, -1), grid::offset(0, -1), grid::offset(1, -1),
		grid::offset(1, 0),
		grid::offset(1, 1), grid::offset(0, 1), grid::offset(-1, 1),
		grid::offset(-1, 0)
	};
};

#endif