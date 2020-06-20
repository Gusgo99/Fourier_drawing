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
#include <stack>
#include <queue>

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

DIPTool::DIPTool(const type _toolType) {
	toolType = _toolType;
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
		
		default:
			wxASSERT(false);
			break;
		
	}
}

bool DIPTool::uses_intensity() {
	bool usesIntensity = false;
	switch(toolType) {
		case THRESHOLD:
			usesIntensity = true;
			break;
			
		case SELECTION:
			usesIntensity = false;
			break;
			
		case SKELETONIZATION:
			usesIntensity = false;
			break;
		
		default:
			wxASSERT(false);
			break;
			
	}
	
	return usesIntensity;
}

bool DIPTool::uses_source() {
	bool usesSource = false;
	switch(toolType) {
		case THRESHOLD:
			usesSource = false;
			break;
			
		case SELECTION:
			usesSource = true;
			break;
			
		case SKELETONIZATION:
			usesSource = false;
			break;
		
		default:
			wxASSERT(false);
			break;
			
	}
	
	return usesSource;
}

void DIPTool::apply_threshold(wxBitmap &_bitmap) {
	const wxColour _backgroundColour = get_background_colour();
	const wxColour _foregroundColour = get_foreground_colour();
	const int _intIntensity = int(intensity * 3 * 255);
	
	for_each_pixel(_bitmap, [=](wxNativePixelData::Iterator _it) {
		if((_it.Red() + _it.Green() + _it.Blue()) > _intIntensity) return _backgroundColour;
		else return _foregroundColour;
		
	});
	
}

void DIPTool::apply_selection(wxBitmap &_bitmap) {
	const wxColour _backgroundColour = get_background_colour();
	const wxColour _foregroundColour = get_foreground_colour();
	wxColour _auxColour(_backgroundColour.Red() | _foregroundColour.Red(),
		_backgroundColour.Green() ^ _foregroundColour.Green(),
		_backgroundColour.Blue() & _foregroundColour.Blue());
	
	if(source != wxDefaultPosition) {
		expand_source_pixel(_bitmap, SIDES | DIAGONALS, [=](wxNativePixelData::Iterator, const std::pair<uint8_t, uint8_t>) {
			return _auxColour;
		});
		
		for_each_pixel(_bitmap, [=](wxNativePixelData::Iterator _pixel) {
			if(_pixel == _auxColour) return _foregroundColour;
			
			return _backgroundColour;
		});
	}
	
}

void DIPTool::apply_skeletonization(wxBitmap &_bitmap) {
	const wxColour _backgroundColour = get_background_colour();
	const wxColour _foregroundColour = get_foreground_colour();
	wxColour _auxColour(_backgroundColour.Red() | _foregroundColour.Red(),
		_backgroundColour.Green() ^ _foregroundColour.Green(),
		_backgroundColour.Blue() & _foregroundColour.Blue());
	bool repeat;
	
	do {
		repeat = false;
		find_foreground(_bitmap);
		expand_source_pixel(_bitmap, SIDES | DIAGONALS, [=](wxNativePixelData::Iterator, std::pair<uint8_t, uint8_t> _neighbourhood) {
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
			
			return _foregroundColour;
		});
		
		expand_source_pixel(_bitmap, SIDES | DIAGONALS, [&](wxNativePixelData::Iterator _pixel, std::pair<uint8_t, uint8_t> _neighbourhood) {
			if(_pixel == _auxColour) {
				if(!causes_connection(_neighbourhood)) {
					repeat = true;
					return _backgroundColour;
					
				}
			}
			
			return _foregroundColour;
		});
	
	} while(repeat);
}

void DIPTool::for_each_pixel(wxBitmap &_bitmap, forEachCallback _callback) const {
	wxSize _size = _bitmap.GetSize();
	wxNativePixelData _pixels(_bitmap);
	wxNativePixelData::Iterator _it = _pixels.GetPixels();
	_it.Offset(_bitmap, 0, 0);
	if(_pixels) {
		for(int y = 0; y < _size.GetHeight(); y++) {
			_it.MoveTo(_bitmap, 0, y);
			for(int x = 0; x < _size.GetWidth(); x++) {
				wxColour _colour = _callback(_it);
				_it.Red() = _colour.Red();
				_it.Green() = _colour.Green();
				_it.Blue() = _colour.Blue();
				++_it;
				
			}
		}
	}
}

void DIPTool::find_foreground(wxBitmap &_bitmap) {
	wxNativePixelData _pixels(_bitmap);
	wxSize _size = _pixels.GetSize();
	wxNativePixelData::Iterator _it;
	
	for(int i = 0; i < _size.GetWidth(); i++) {
		for(int j = 0; j < _size.GetHeight(); j++) {
			_it.MoveTo(_pixels, i, j);
			if(_it != get_background_colour()) {
				source = wxPoint(i, j);
				goto end;
			}
		}
	}
	end:
	
	return;
}

std::vector<bool> DIPTool::expand_source_pixel(wxBitmap &_bitmap, int _directions, expandCallback _callback) {
	wxSize _size = _bitmap.GetSize();
	std::vector<bool> _visited(_size.GetWidth() * _size.GetHeight());
	
	wxNativePixelData _pixels(_bitmap);
	wxNativePixelData::Iterator _it;
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
					_it.MoveTo(_pixels, _currentPoint.x, _currentPoint.y);
					wxColour _colour = _callback(_it, _neighbourhood);
					_it.Red() = _colour.Red();
					_it.Green() = _colour.Green();
					_it.Blue() = _colour.Blue();
					
				}
			}
		}
	}
	
	return _visited;
}

bool DIPTool::is_inside_screen(const wxPoint &_POINT, const wxSize &_SIZE) const {
	return
		(_POINT.x >= 0) &&
		(_POINT.y >= 0) &&
		(_POINT.x < _SIZE.GetWidth()) &&
		(_POINT.y < _SIZE.GetHeight());
}

wxColour DIPTool::get_background_colour() const {
	return wxColour(255, 255, 255);
}

wxColour DIPTool::get_foreground_colour() const {
	return wxColour(0, 0, 0);
}

std::pair<uint8_t, uint8_t> DIPTool::get_neighbour_pixels(
	const wxNativePixelData &_PIXELS,
	const wxPoint _POINT,
	int _directions) const {
	const wxColour _backgroundColour = get_background_colour();
	std::pair<uint8_t, uint8_t> _neighbourhood(0, 0);
	wxNativePixelData::Iterator _neighbour = _PIXELS.GetPixels();
	
	for(int i = 0; i < 8; i++) {
		if(_directions & 0x01) {
			_neighbourhood.first <<= 1;
			wxPoint _currentPoint = _POINT + DIRECTIONS[7 - i];
			if(is_inside_screen(_currentPoint, _PIXELS.GetSize())) {
				_neighbour.MoveTo(_PIXELS, _currentPoint.x, _currentPoint.y);
				if(_neighbour != _backgroundColour) {
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

bool DIPTool::causes_connection(const std::pair<uint8_t, uint8_t> _NEIGHBOURHOOD) const {
	bool _causesConnection = true;
	int _modified = _NEIGHBOURHOOD.first;
	
	if(_modified != 0) {
		if(_modified & 0x01) {
			_modified |= _modified << 8;
			while(_modified & 0x01) _modified >>= 1;
			
		}
		while(!(_modified & 0x01)) _modified >>= 1;
		_modified &= 0xFF;
		
		if((_modified == _NEIGHBOURHOOD.second) && (_modified > 1)) _causesConnection = false;
		
	}
	
	return _causesConnection;
}

bool operator==(wxNativePixelData::Iterator _pixel, const wxColour &_COLOUR) {
	return
		(_pixel.Red() == _COLOUR.Red()) &&
		(_pixel.Green() == _COLOUR.Green()) &&
		(_pixel.Blue() == _COLOUR.Blue());
}

bool operator!=(wxNativePixelData::Iterator _pixel, const wxColour &_COLOUR) {
	return !operator==(_pixel, _COLOUR);
}