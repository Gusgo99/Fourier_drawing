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
#include "DIP_skeletonization_strategy.hpp"

#include "wxWidgets_headers.hpp"

#include <queue>

namespace DIP {
	skeletonizationStrategy::skeletonizationStrategy(): strategy{false, true, false} {}

	void skeletonizationStrategy::apply(wxBitmap &_bitmap) {
		grid _image = generate_grid(
			_bitmap,
			{{BACKGROUNDCOLOUR, grid::NOTHING}, {FOREGROUNDCOLOUR, grid::EDGE}}
		);

		bool _wasGridModified;

		do {
			std::vector<grid::position> _contour = _image.get_contour();
			_wasGridModified = remove_non_connecting_cells(_image, _contour);

		} while(_wasGridModified);

		draw_grid_to_bitmap(
			_bitmap,
			_image,
			{{grid::NOTHING, BACKGROUNDCOLOUR}, {grid::EDGE, FOREGROUNDCOLOUR}}
		);
	}

	bool skeletonizationStrategy::remove_non_connecting_cells(
		grid &_grid,
		const std::vector<grid::position> &_positions
	) const {
		bool _modified = false;

		for(auto i: _positions) {
			if(!_grid.get_cell_neighbourhood(i).does_cell_causes_connection()) {
				_modified = true;
				_grid(i) = grid::NOTHING;

			}
		}
	
		return _modified;
	}
}