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
#ifndef DIP_PATH_GENERATION_STRATEGY_HPP
#define DIP_PATH_GENERATION_STRATEGY_HPP
#pragma once

#include "DIP_strategy.hpp"
#include  "image_processing/grid.hpp"

#include <vector>

namespace DIP {
	class pathGenerationStrategy final: public strategy {
	public:
		pathGenerationStrategy() = default;
		virtual ~pathGenerationStrategy() = default;
		
		void apply(wxBitmap &_bitmap) ;
		bool uses_intensity() const;
		bool uses_source() const;
		bool generates_info() const;
		
	public:
		using path = std::vector<grid::position>;

	private:
		struct pathFinderContext {
			const grid originalGrid;
			grid &currentGrid;
			std::vector<grid::position> path;
			std::vector<grid::position> oddNodes;
		};

	private:
		path find_odd_cells(grid &_grid) const;
		bool has_visited_all_cells(pathFinderContext &_context) const;
		void visit_next_cell(pathFinderContext &_context) const;
		void visit_path_to_odd_cell(pathFinderContext &_context) const;

		pathGenerationStrategy::path get_path_to_closest(
			const grid &_heightmap,
			std::vector<grid::position> &_targetLocations
		) const;

		std::vector<grid::position> solve_chinese_postman(grid &_grid) const;
		std::vector<grid> get_odd_cells_heightmaps(const grid &_grid) const;
		size_t find_closest_position(
			const std::vector<grid> &_heightmaps,
			const grid::position _destination
		) const;

	};
}

#endif