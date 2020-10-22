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
#include "grid.hpp"

#include <algorithm>
#include <queue>

std::array<grid::position, 8> operator+(
	const grid::position _position,
	const std::array<grid::offset, 8> _offset) {
	
	std::array<grid::position, 8> _newPositions;

	for(size_t i = 0; i < _offset.size(); i++) {
		_newPositions[i] = {_position.first + _offset[i].first, _position.second + _offset[i].second};
		
	}
	return _newPositions;
}

grid::position operator+(const grid::position _position, grid::offset _offset) {
	return grid::position(_position.first + _offset.first, _position.second + _offset.second);
}

bool operator<(grid::position _p1, grid::position _p2) {
	return (_p1.first < _p2.first) && (_p1.second < _p2.second);
}

grid::grid(const size_t _numLines, const size_t _numColumns, const std::vector<int> &_gridData):
grid{{_numLines, _numColumns}, _gridData} {}

grid::grid(const position _size, const std::vector<int> &_gridData):
size{_size},
gridData{_gridData} {
	gridData.resize(size.first * size.second);
	
}

void grid::fill(const int _value) {
	std::fill(gridData.begin(), gridData.end(), _value);
	
}

size_t grid::num_lines() const {
	return size.first;
}

size_t grid::num_columns() const {
	return size.second;
}

grid::position grid::get_size() const {
	return size;
}

int& grid::operator()(const size_t _line, const size_t _column) {
	return gridData[_line * size.second + _column];
}

int& grid::operator()(const position _position) {
	return operator()(_position.first, _position.second);
}

int grid::operator()(const size_t _line, const size_t _column) const {
	return gridData[_line * size.second + _column];
}

int grid::operator()(const position _position) const {
	return operator()(_position.first, _position.second);
}

grid::position grid::max_neighbour(const position _targetPosition) const {
	std::array<position, 8> _neighbours = _targetPosition + NEIGHBOUROFFSET;
	size_t _min = 0;

	for(_min = 0; !(_neighbours[_min] < size); _min++) {
		if(_min == _neighbours.size()) break;

	}

	for(size_t i = _min; i < _neighbours.size(); i++) {
		if(_neighbours[i] < size) {
			if(operator()(_neighbours[_min]) < operator()(_neighbours[i])) _min = i;

		}
	}

	return _neighbours[_min];
}

grid::position grid::min_neighbour(const position _targetPosition) const {
	std::array<position, 8> _neighbours = _targetPosition + NEIGHBOUROFFSET;
	size_t _min = 0;

	for(_min = 0; !(_neighbours[_min] < size); _min++) {
		if(_min == _neighbours.size()) break;

	}

	for(size_t i = _min; i < _neighbours.size(); i++) {
		if(_neighbours[i] < size) {
			if(operator()(_neighbours[_min]) > operator()(_neighbours[i])) _min = i;

		}
	}

	return _neighbours[_min];
}

std::vector<int> grid::positions_data(const std::vector<position> _positions) const {
	std::vector<int> _data;
	_data.reserve(_positions.size());

	for(auto i: _positions) {
		if(i < size) _data.push_back(operator()(i));

	}

	return _data;
}

std::array<int, 8> grid::neighbours_data(const position _targetPosition) const {
	std::array<int, 8> _data;
	auto _neighbourPositions = _targetPosition + NEIGHBOUROFFSET;
	for(size_t i = 0; i < _neighbourPositions.size(); i++) {
		if(_neighbourPositions[i] < size) _data[i] = operator()(_neighbourPositions[i]);
		else _data[i] = NOTHING;
		
	}
	
	return _data;
}

std::array<int, 4> grid::diagonal_neighbours_data(const position _targetPosition) const {
	std::array<int, 4> _data;
	auto _neighbourPositions = _targetPosition + NEIGHBOUROFFSET;
	for(size_t i = 0; i < _neighbourPositions.size(); i+=2) {
		if(_neighbourPositions[i] < size) _data[i / 2] = operator()(_neighbourPositions[i]);
		else _data[i / 2] = NOTHING;
		
	}
	
	return _data;
}

std::array<int, 4> grid::non_diagonal_neighbours_data(const position _targetPosition) const {
	std::array<int, 4> _data;
	auto _neighbourPositions = _targetPosition + NEIGHBOUROFFSET;
	for(size_t i = 1; i < _neighbourPositions.size(); i+=2) {
		if(_neighbourPositions[i] < size) _data[i / 2] = operator()(_neighbourPositions[i]);
		else _data[i / 2] = NOTHING;
		
	}
	
	return _data;
}

template<typename inputIt>
int grid::count_regions(const inputIt _begin, const inputIt _end) {
	int _counter = 0;
	auto _lastValue = *_begin;

	for(auto i = _begin; i != _end; i++) {
		if(_lastValue != *i) {
			_counter++;
			_lastValue = *i;

		}
	}

	if(_lastValue != *_begin) _counter++;

	return _counter;
}

grid grid::distance_heightmap(const position _targetPosition) const {
	grid _distances(size);
	_distances.fill(INT_MAX);
	
	std::queue<std::pair<size_t, size_t>> _boundary;
	if(_targetPosition < size) {
		_boundary.push(_targetPosition);
		_distances(_targetPosition) = 0;
		
	}
	
	while(!_boundary.empty()) {
		position _currentPos = _boundary.front();
		_boundary.pop();
		for(auto i: _currentPos + NEIGHBOUROFFSET) {
			if(i < size) {
				if(operator()(i) != NOTHING && _distances(i) == INT_MAX) {
					_distances(i) = _distances(_currentPos) + 1;
					_boundary.push(i);
				}
			}
		}
	}
	
	return _distances;
}

std::vector<grid::position> grid::descend_heightmap(const position _start) const {
	std::vector<position> _path;
	if(operator()(_start) != INT_MAX) {
		_path.reserve(operator()(_start) + 1);

		_path.push_back(_start);

		while(operator()(_path.back()) != 0) _path.push_back(min_neighbour(_path.back()));

	}

	return _path;
}

std::vector<grid::position> grid::find_every_value(const int _value) const {
	std::vector<position> _found;
	position _pos{0, 0};
	
	for(size_t i = 0; i < gridData.size(); i++) {
		if(_pos.second == size.second) {
			_pos.first++;
			_pos.second = 0;

		}
		if(operator()(_pos) == _value) _found.push_back(_pos);
		_pos.second++;

	}
	
	return _found;
}

grid::position grid::find_value(const int _value) const {
	position _pos{0, 0};

	for(auto i: gridData) {
		if(_pos.second == size.second) {
			_pos.first++;
			_pos.second = 0;

		}
		if(i == _value) break;
		_pos.second++;

	}

	return _pos;
}

std::vector<grid::position> grid::get_contour() const {
	std::vector<position> _contour;

	for(size_t i = 0; i < size.first; i++) {
		for(size_t j = 0; j < size.second; j++) {
			if(operator()({i, j}) != NOTHING) {
				std::array<int, 4> _neighbourhood = non_diagonal_neighbours_data({i, j});
				int _nothingCount = std::count(_neighbourhood.begin(), _neighbourhood.end(), NOTHING);
				if(_nothingCount != 0) {
					_contour.push_back({i, j});
	
				}
			}
		}
	}

	return _contour;
}

void grid::remove_unconnected_cells(const position _targetPosition) {
	grid _aux = distance_heightmap(_targetPosition);

	for(size_t i = 0; i < gridData.size(); i++) {
		gridData[i] = (_aux.gridData[i] == INT_MAX) ? NOTHING : EDGE;

	}
}

bool grid::remove_non_connecting_cells(const std::vector<position> &_positions) {
	bool _modified = false;

	for(auto i: _positions) {
		if(!causes_connections(i)) {
			_modified = true;
			operator()(i) = NOTHING;
		}
	}

	return _modified;
}

void grid::skeletonize() {
	bool _wasGridModified;
	
	do {
		std::vector<position> _contour = get_contour();
		_wasGridModified = remove_non_connecting_cells(_contour);

	} while(_wasGridModified);

}

bool grid::causes_connections(const position _targetPosition) const {
	std::array<int, 8> _neighbourhood = neighbours_data(_targetPosition);
	std::array<bool, 8> _reacheable = {false, false, false, false, false, false, false, false};
	std::queue<size_t> _frontier;

	for(size_t i = 0; i < _neighbourhood.size(); i++) {
		if(_neighbourhood[i] != NOTHING) {
			_reacheable[i] = true;
			_frontier.push(i);
			break;

		}
	}

	while(!_frontier.empty()) {
		size_t _current = _frontier.front();
		_frontier.pop();
		for(auto i: {1, 7}) {
			size_t _added = (_current + i) % _reacheable.size();
			if(_neighbourhood[_added] != NOTHING && !_reacheable[_added]) {
				_reacheable[_added] = true;
				_frontier.push(_added);

			}
		}
		if((_current % 2) == 1) {
			for(auto i: {2, 6}) {
				size_t _added = (_current + i) % _reacheable.size();
				if(_neighbourhood[_added] != NOTHING && !_reacheable[_added]) {
					_reacheable[_added] = true;
					_frontier.push(_added);

				}
			}
		}
	}

	bool _causesConnections = false;

	for(size_t i = 0; i < _neighbourhood.size(); i++) {
		if((_neighbourhood[i] != NOTHING) && !_reacheable[i]) {
			_causesConnections = true;
			break;

		}
	}

	int _count = std::count(_neighbourhood.begin(), _neighbourhood.end(), NOTHING);

	_causesConnections |= _count == 7;
	_causesConnections &= _count != 8;

	return _causesConnections;
}

std::vector<grid::position> grid::solve_chinese_postman() {
	std::vector<position> _path;
	std::vector<grid> _heightmaps;

	_path.reserve(gridData.size() / 10);

	for(auto i: find_every_value(EDGE)) {
		std::array<int, 8> _data = neighbours_data(i);
		size_t _connections = _data.size() - std::count(_data.begin(), _data.end(), NOTHING);
		if((_connections % 2) == 1) {
			_heightmaps.push_back(distance_heightmap(i));
			operator()(i) = 2;
			
		}
	}

	_path.push_back(_heightmaps[0].find_value(0));
	operator()(_path.back()) = NOTHING;

	while(std::any_of(gridData.begin(), gridData.end(), [](const int _data) { return _data != 0; })) {
		position _maxNeighbour = max_neighbour(_path.back());
		if(operator()(_maxNeighbour) != NOTHING) {
			operator()(_maxNeighbour) = NOTHING;
			_path.push_back(_maxNeighbour);

		}
		else {
			if(_heightmaps.size() == 1) break;
			auto _closest = _heightmaps.begin() + 1;
			if((*_closest)(_path.back()) == NOTHING) _closest = _heightmaps.erase(_closest);
			for(auto i = _closest; i != _heightmaps.end(); i++) {
				if((*i)(_path.back()) != NOTHING) {
					if((*_closest)(_path.back()) > (*i)(_path.back())) _closest = i;

				}
				else i = _heightmaps.erase(i) - 1;
			}
			std::vector<position> _newPoints = _closest -> descend_heightmap(_path.back());
			_path.insert(_path.end(), _newPoints.begin() + 1, _newPoints.end());
			_heightmaps.erase(_closest);

		}
	}

	std::vector<position> _newPoints = _heightmaps.front().descend_heightmap(_path.back());
	_path.insert(_path.end(), _newPoints.begin() + 1, _newPoints.end());
	
	return _path;
}