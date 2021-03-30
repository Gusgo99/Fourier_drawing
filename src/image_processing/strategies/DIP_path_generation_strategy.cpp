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
#include "DIP_path_generation_strategy.hpp"

#include "wxWidgets_headers.hpp"

#include <complex>

#include "windows/convertions.hpp"

namespace DIP {
	void pathGenerationStrategy::apply(wxBitmap &_bitmap) {
		grid _image = generate_grid(
			_bitmap,
			{
				{BACKGROUNDCOLOUR, grid::NOTHING},
				{FOREGROUNDCOLOUR, grid::EDGE}
			});

		std::vector<grid::position> _path = _image.solve_chinese_postman();

		draw_grid_to_bitmap(
			_bitmap,
			_image,
			{
				{grid::NOTHING, BACKGROUNDCOLOUR},
				{grid::EDGE, FOREGROUNDCOLOUR}
			}
		);

		std::vector<std::complex<float>> _floatPath;
		_floatPath.reserve(_path.size());

		for(auto i: _path) {
			_floatPath.push_back(to_complex_number(wxPoint(i.first, i.second), _bitmap.GetSize()));

		}

		info = _floatPath;
	}

	bool pathGenerationStrategy::uses_intensity() const {
		return false;
	}

	bool pathGenerationStrategy::uses_source() const {
		return false;
	}

	bool pathGenerationStrategy::generates_info() const {
		return true;
	}

	std::vector<grid::position> pathGenerationStrategy::solve_chinese_postman(grid &_grid) const {
		pathFinderContext _context{
			_grid,
			_grid,
			{_grid.find_value(grid::EDGE)},
			{find_odd_cells(_grid)}
		};

		_context.path.reserve(_grid.get_size().first * _grid.get_size().second / 10);

		while(!has_visited_all_cells(_context)) {
			visit_next_cell(_context);
		}
		
		return _context.path;
	}

	pathGenerationStrategy::path pathGenerationStrategy::find_odd_cells(grid &_grid) const {
		std::vector<grid::position> _oddNodes;

		for(auto i: _grid.find_every_value(grid::EDGE)) {
			if(_grid.get_cell_neighbourhood(i).has_odd_connection_count()) {
				_oddNodes.push_back(i);
			}
		}

		return _oddNodes;
	}

	bool pathGenerationStrategy::has_visited_all_cells(pathFinderContext &_context) const {
		if(_context.oddNodes.empty()) {
			for(size_t i = 0; i < _context.currentGrid.get_size().first; i++) {
				for(size_t j = 0; j < _context.currentGrid.get_size().second; j++) {
					const auto _neighbourhood = _context.currentGrid.get_cell_neighbourhood({i, j});
					
					const bool _hasUnvisitedNeighbours = std::count(
						_neighbourhood.begin(),
						_neighbourhood.end(),
						grid::NOTHING
					) != 8;

					if(_hasUnvisitedNeighbours) return true;
					else _context.currentGrid(i, j) = grid::NOTHING;
				}
			}
		}

		return false;
	}

	void pathGenerationStrategy::visit_next_cell(pathFinderContext &_context) const {
		const grid::position _maxNeighbour = _context.currentGrid.max_neighbour(_context.path.back());

		if(_context.currentGrid(_maxNeighbour) != grid::NOTHING) {
			_context.currentGrid(_maxNeighbour) = grid::NOTHING;
			_context.path.push_back(_maxNeighbour);

		}
		else {
			visit_path_to_odd_cell(_context);

		}
	}

	void pathGenerationStrategy::visit_path_to_odd_cell(pathFinderContext &_context) const {
		if(!_context.oddNodes.empty()) {
			const auto _heightmap = _context.originalGrid.distance_heightmap(_context.path.back());

			const auto _pathExtension = _heightmap.descend_heightmap(_context.oddNodes[0]);

			const auto _toRemove1 = std::find(
				_context.oddNodes.begin(),
				_context.oddNodes.end(),
				_context.path.back()
			);

			if(_toRemove1 != _context.oddNodes.end()) _context.oddNodes.erase(_toRemove1);

			_context.path.insert(_context.path.end(), _pathExtension.rbegin(), _pathExtension.rend());

			const auto _toRemove2 = std::find(
				_context.oddNodes.begin(),
				_context.oddNodes.end(),
				_context.path.back()
			);

			if(_toRemove2 != _context.oddNodes.end()) _context.oddNodes.erase(_toRemove2);
		}
	}
}