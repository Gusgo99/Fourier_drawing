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
#include "DIP_strategy.hpp"

#include <algorithm>
#include <complex>
#include <queue>

#include "windows/convertions.hpp"

namespace DIP {
	const wxColour strategy::BACKGROUNDCOLOUR(255, 255, 255);
	const wxColour strategy::FOREGROUNDCOLOUR(0, 0, 0);

	strategy::strategy(const bool _usesIntensity, const bool _usesSource, const bool _hasOutput):
		usesIntensity{_usesIntensity},
		usesSource{_usesSource},
		hasOutput{_hasOutput},
		info{},
		intensity{0.5},
		source{wxDefaultPosition} {}

	const std::any& strategy::get_info() const {
		return info;
	}

	strategy& strategy::set_source(const wxPoint &_source) {
		source = _source;

		return (*this);
	}

	strategy& strategy::set_source(const wxCoord _x, const wxCoord _y) {
		return set_source(wxPoint(_x, _y));
	}

	const wxPoint& strategy::get_source() {
		return source;
	}

	strategy& strategy::set_intensity(const double _intensity) {
		intensity = _intensity;

		return (*this);
	}

	double strategy::get_intensity() {
		return intensity;
	}

	const std::any& strategy::get_output() {
		return info;
	}

	void strategy::for_each_pixel(wxBitmap &_bitmap, forEachCallback _callback) {
		wxSize _size = _bitmap.GetSize();
		wxNativePixelData _pixels(_bitmap);
		pixelData _pixel = _pixels.GetPixels();

		if(_pixels) {
			for(int y = 0; y < _size.GetHeight(); y++) {
				_pixel.MoveTo(_bitmap, 0, y);
				for(int x = 0; x < _size.GetWidth(); x++) {
					wxColour _colour = _callback(_pixel);
					_pixel.Red() = _colour.Red();
					_pixel.Green() = _colour.Green();
					_pixel.Blue() = _colour.Blue();
					++_pixel;
					
				}
			}
		}
	}

	grid strategy::generate_grid(wxBitmap &_bitmap, const std::function<int(wxColour)> &_converter) {
		grid _result(_bitmap.GetWidth(), _bitmap.GetHeight());
		_result.fill(grid::NOTHING);
		grid::position _position{0, 0};

		_result.fill(grid::NOTHING);

		for_each_pixel(
			_bitmap,
			[&](pixelData _pixel) {
				_result(_position) = _converter(wxColour{_pixel.Red(), _pixel.Green(), _pixel.Blue()});

				_position.first++;
				if(_position.first == _result.num_lines()) {
					_position.first = 0;
					_position.second++;
				}

				return wxColour(_pixel.Red(), _pixel.Green(), _pixel.Blue());
			}
		);

		return _result;
	}

	grid strategy::generate_grid(
		wxBitmap &_bitmap,
		const std::vector<std::pair<wxColour, grid::data>> &_colours) {
		
		grid _result = generate_grid(_bitmap, [&](wxColour _pixelColour) -> grid::data {
			grid::data _found = grid::NOTHING;

			for(auto i: _colours) {
				if(i.first == _pixelColour) {
					_found = i.second;
					break;

				}
			}

			return _found;
		});

		return _result;
	}

	void strategy::draw_grid_to_bitmap(
		wxBitmap &_bitmap,
		const grid &_grid,
		const std::vector<std::pair<grid::data, wxColour>> &_colours) {
		
		grid::position _position{0, 0};

		for_each_pixel(
			_bitmap,
			[&_position, &_grid, &_colours](const pixelData) {
				wxColour _pixelColour = BACKGROUNDCOLOUR;
				for(auto i: _colours) {
					if(_grid(_position) == i.first) {
						_pixelColour = i.second;
						break;
					}
				}
				_position.first++;
				if(_position.first == _grid.num_lines()) {
					_position.first = 0;
					_position.second++;
				}

				return _pixelColour;
			}
		);
	}
}