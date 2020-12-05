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

#include <vector>
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
}