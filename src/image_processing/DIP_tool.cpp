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
#include "DIP_tool.hpp"

#include <algorithm>
#include <complex>
#include <queue>

#include "windows/convertions.hpp"

const std::array<wxPoint, 8> DIPTool::DIRECTIONS{
	// N
	wxPoint(0, -1),
	// NE
	wxPoint(1, -1),
	// E
	wxPoint(1, 0),
	// SE
	wxPoint(1, 1),
	// S
	wxPoint(0, 1),
	// SO
	wxPoint(-1, 1),
	// O
	wxPoint(-1, 0),
	// NO
	wxPoint(-1, -1)
	
};

const wxColour DIPTool::BACKGROUNDCOLOUR(255, 255, 255);
const wxColour DIPTool::FOREGROUNDCOLOUR(0, 0, 0);

DIPTool::DIPTool(const type _toolType):
toolType{_toolType} {
	
	intensity = 0.5;
	source = wxDefaultPosition;
	
}

void DIPTool::apply(wxBitmap &_bitmap) {
	switch(toolType) {
		case THRESHOLD:
			apply_threshold(_bitmap);
			break;
			
		case SELECTION:
			apply_selection(_bitmap);
			break;
			
		case SKELETONIZATION:
			apply_skeletonization(_bitmap);
			break;
			
		case PATHGENERATION:
			apply_path_generation(_bitmap);
			break;
			
		default:
			wxASSERT(false);
			break;
			
	}
}

bool DIPTool::uses_intensity() const {
	bool _usesIntensity = false;
	switch(toolType) {
		case THRESHOLD:
			_usesIntensity = true;
			break;
			
		case SELECTION:
			_usesIntensity = false;
			break;
			
		case SKELETONIZATION:
			_usesIntensity = false;
			break;
			
		case PATHGENERATION:
			_usesIntensity = false;
			break;
			
		default:
			wxASSERT(false);
			break;
			
	}
	
	return _usesIntensity;
}

bool DIPTool::uses_source() const {
	bool _usesSource = false;
	switch(toolType) {
		case THRESHOLD:
			_usesSource = false;
			break;
			
		case SELECTION:
			_usesSource = true;
			break;
			
		case SKELETONIZATION:
			_usesSource = false;
			break;
			
		case PATHGENERATION:
			_usesSource = false;
			break;
			
		default:
			wxASSERT(false);
			break;
			
	}
	
	return _usesSource;
}

bool DIPTool::generates_info() const {
	bool _generatesInfo = false;
	switch(toolType) {
		case THRESHOLD:
			_generatesInfo = false;
			break;
			
		case SELECTION:
			_generatesInfo = false;
			break;
			
		case SKELETONIZATION:
			_generatesInfo = false;
			break;
			
		case PATHGENERATION:
			_generatesInfo = true;
			break;
			
		default:
			wxASSERT(false);
			break;
			
	}
	
	return _generatesInfo;
}

std::any DIPTool::get_info() const {
	return info;	
}

void DIPTool::apply_threshold(wxBitmap &_bitmap) {
	const int _iIntensity = int(intensity * 3 * 255);
	
	for_each_pixel(
		_bitmap,
		[=](pixelData _pixel) {
			if((_pixel.Red() + _pixel.Green() + _pixel.Blue()) > _iIntensity) return BACKGROUNDCOLOUR;
			else return FOREGROUNDCOLOUR;
			
		});
	
}

void DIPTool::apply_selection(wxBitmap &_bitmap) {
	wxColour _auxColour(
		BACKGROUNDCOLOUR.Red() | FOREGROUNDCOLOUR.Red(),
		BACKGROUNDCOLOUR.Green() ^ FOREGROUNDCOLOUR.Green(),
		BACKGROUNDCOLOUR.Blue() & FOREGROUNDCOLOUR.Blue());
	
	if(source != wxDefaultPosition) {
		expand_source_pixel(
			_bitmap,
			SIDES | DIAGONALS,
			[=](pixelData, const std::pair<uint8_t, uint8_t>) {
				return _auxColour;
			});
		
		for_each_pixel(
			_bitmap,
			[=](pixelData _pixel) {
				if(_pixel == _auxColour) return FOREGROUNDCOLOUR;
				
				return BACKGROUNDCOLOUR;
			});
	}
	
}

void DIPTool::apply_skeletonization(wxBitmap &_bitmap) {
	bool repeat;
	wxColour _auxColour(
		BACKGROUNDCOLOUR.Red() | FOREGROUNDCOLOUR.Red(),
		BACKGROUNDCOLOUR.Green() ^ FOREGROUNDCOLOUR.Green(),
		BACKGROUNDCOLOUR.Blue() & FOREGROUNDCOLOUR.Blue());
	
	do {
		repeat = false;
		find_foreground(_bitmap);
		expand_source_pixel(
			_bitmap,
			SIDES | DIAGONALS,
			[=](pixelData, std::pair<uint8_t, uint8_t> _neighbourhood) {
				std::pair<uint8_t, uint8_t> _copy(0, _neighbourhood.second);
				if(_neighbourhood.first & 0x01) _copy.first |= 0x01;
				if(_neighbourhood.first & 0x02) _copy.second >>= 1;
				if(_neighbourhood.first & 0x04) _copy.first |= 0x02;
				if(_neighbourhood.first & 0x08) _copy.second >>= 1;
				if(_neighbourhood.first & 0x10) _copy.first |= 0x04;
				if(_neighbourhood.first & 0x20) _copy.second >>= 1;
				if(_neighbourhood.first & 0x40) _copy.first |= 0x08;
				if(_neighbourhood.first & 0x80) _copy.second >>= 1;
				if(_copy.first != 0x0F) return _auxColour;
				
				return FOREGROUNDCOLOUR;
			});
		
		expand_source_pixel(
			_bitmap,
			SIDES | DIAGONALS,
			[&](pixelData _pixel, std::pair<uint8_t, uint8_t> _neighbourhood) {
				if(_pixel == _auxColour) {
					if(!causes_connection(_neighbourhood)) {
						repeat = true;
						return BACKGROUNDCOLOUR;
						
					}
				}
				
				return FOREGROUNDCOLOUR;
			});
	
	} while(repeat);
}

void DIPTool::apply_path_generation(wxBitmap &_bitmap) {
	grid image = generate_grid(
		_bitmap,
		{
			{BACKGROUNDCOLOUR, grid::NOTHING},
			{FOREGROUNDCOLOUR, grid::EDGE}
		});

	std::vector<grid::position> _path = image.solve_chinese_postman();

	std::vector<std::complex<float>> _floatPath;
	_floatPath.reserve(_path.size());

	for(auto i: _path) {
		_floatPath.push_back(to_complex_number(wxPoint(i.first, i.second), _bitmap.GetSize()));
	}

	info = _floatPath;
	
}

void DIPTool::for_each_pixel(wxBitmap &_bitmap, forEachCallback _callback) {
	wxSize _size = _bitmap.GetSize();
	wxNativePixelData _pixels(_bitmap);
	pixelData _pixel = _pixels.GetPixels();
	_pixel.Offset(_bitmap, 0, 0);
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

std::vector<bool> DIPTool::expand_source_pixel(wxBitmap &_bitmap,
	int _directions,
	expandCallback _callback) const {
	
	wxSize _size = _bitmap.GetSize();
	std::vector<bool> _visited(_size.GetWidth() * _size.GetHeight());
	
	wxNativePixelData _pixels(_bitmap);
	pixelData _pixel;
	std::queue<wxPoint> _frontier;
	
	if(_pixels) {
		_frontier.push(source);
		while(!_frontier.empty()) {
			wxPoint _currentPoint = _frontier.front();
			_frontier.pop();
			if(is_inside_screen(_currentPoint, _size)) {
				if(!_visited[_currentPoint.x + _currentPoint.y * _size.GetWidth()]) {
					_visited[_currentPoint.x + _currentPoint.y * _size.GetWidth()] = true;
					auto _neighbourhood = get_neighbour_pixels(_pixels, _currentPoint, _directions);
					uint8_t _tempNeighbours = _neighbourhood.first;
					for(auto i: DIRECTIONS) {
						if(_tempNeighbours & 0x01) _frontier.push(_currentPoint + i);
						_tempNeighbours >>= 1;
						
					}
					_pixel.MoveTo(_pixels, _currentPoint.x, _currentPoint.y);
					wxColour _colour = _callback(_pixel, _neighbourhood);
					_pixel.Red() = _colour.Red();
					_pixel.Green() = _colour.Green();
					_pixel.Blue() = _colour.Blue();
					
				}
			}
		}
	}
	
	return _visited;
}

grid DIPTool::generate_grid(
	wxBitmap &_bitmap,
	const std::vector<std::pair<wxColour, grid::data>> &_colours) {
	
	grid _result(_bitmap.GetWidth(), _bitmap.GetHeight());
	_result.fill(grid::NOTHING);
	grid::position _position{0, 0};

	for_each_pixel(
		_bitmap,
		[&_position, &_result, &_colours](pixelData _pixel) {
			for(auto i: _colours) {
				if(_pixel == i.first) {
					_result(_position) = i.second;
					break;
				}
			}
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

void DIPTool::draw_grid_to_bitmap(
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

bool DIPTool::causes_connection(const std::pair<uint8_t, uint8_t> _neighbourhood) {
	bool _causesConnection = true;
	int _modified = _neighbourhood.first;
	
	if(_modified != 0) {
		if(_modified & 0x01) {
			_modified |= _modified << 8;
			while(_modified & 0x01) _modified >>= 1;
			
		}
		while(!(_modified & 0x01)) _modified >>= 1;
		_modified &= 0xFF;
		
		if((_modified == _neighbourhood.second) && (_modified > 1)) _causesConnection = false;
		
	}
	
	return _causesConnection;
}

std::pair<uint8_t, uint8_t> DIPTool::get_neighbour_pixels(
	const wxNativePixelData &_pixels,
	const wxPoint _point,
	int _directions) const {
	std::pair<uint8_t, uint8_t> _neighbourhood(0, 0);
	pixelData _neighbour = _pixels.GetPixels();
	
	for(int i = 0; i < 8; i++) {
		if(_directions & 0x01) {
			_neighbourhood.first <<= 1;
			wxPoint _currentPoint = _point + DIRECTIONS[7 - i];
			if(is_inside_screen(_currentPoint, _pixels.GetSize())) {
				_neighbour.MoveTo(_pixels, _currentPoint.x, _currentPoint.y);
				if(_neighbour != BACKGROUNDCOLOUR) {
					_neighbourhood.second <<= 1;
					_neighbourhood.second |= 1;
					_neighbourhood.first |= 1;
					
				}
			}
			else {
				_neighbourhood.second <<= 1;
				_neighbourhood.second |= 1;
				
			}
		}
		_directions >>= 1;
		
	}
	
	return _neighbourhood;
}

bool DIPTool::is_inside_screen(const wxPoint &_point, const wxSize &_size) {
	return
		(_point.x >= 0) &&
		(_point.y >= 0) &&
		(_point.x < _size.GetWidth()) &&
		(_point.y < _size.GetHeight());
}

void DIPTool::find_foreground(wxBitmap &_bitmap) {
	wxNativePixelData _pixels(_bitmap);
	wxSize _size = _pixels.GetSize();
	pixelData _pixel;
	
	for(int i = 0; i < _size.GetWidth(); i++) {
		for(int j = 0; j < _size.GetHeight(); j++) {
			_pixel.MoveTo(_pixels, i, j);
			if(_pixel != BACKGROUNDCOLOUR) {
				source = wxPoint(i, j);
				goto end;
			}
		}
	}
	end:
	
	return;
}

bool operator==(DIPTool::pixelData _pixel, const wxColour &_COLOUR) {
	return
		(_pixel.Red() == _COLOUR.Red()) &&
		(_pixel.Green() == _COLOUR.Green()) &&
		(_pixel.Blue() == _COLOUR.Blue());
}

bool operator!=(DIPTool::pixelData _pixel, const wxColour &_COLOUR) {
	return !operator==(_pixel, _COLOUR);
}