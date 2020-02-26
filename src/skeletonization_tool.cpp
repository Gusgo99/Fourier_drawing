#include "skeletonization_tool.hpp"

#include <iostream>

c_skeletonizationTool::c_skeletonizationTool(c_image &_image) : c_modifyingImageProcessingTool(_image) {
	
	return;
}

c_skeletonizationTool::~c_skeletonizationTool() {
	
	return;
}

void c_skeletonizationTool::apply() {
	std::vector<bool> _contour;
	
	const std::vector<int> _DIR = {-1, 1, image.get_width(), -image.get_width()};
	
	do {
		_contour = get_contour();
		
	} while(contour_removal(_contour));
	
	return;
}

void c_skeletonizationTool::increase_effect() {
	return;
}

void c_skeletonizationTool::decrease_effect() {
	return;
}

void c_skeletonizationTool::reset_effect() {
	return;
}

void c_skeletonizationTool::affect_pixel(const int &_x, const int &_y) {
	if(_x == _y) {}
	
	return;
}

void c_skeletonizationTool::set_background_color(const s_color &_backgroundColor) {
	backgroundColor = _backgroundColor;
	
	return;
}

s_color c_skeletonizationTool::get_background_color() {
	return backgroundColor;
}

std::vector<bool> c_skeletonizationTool::get_contour() {
	const std::vector<int> _DIRECTIONS = {image.get_width(), -image.get_width(), 1, -1};
	
	std::vector<bool> _contour(image.get_size());
	
	for(size_t i = 0; i != _contour.size(); i++) {
		_contour[i] = false;
		if(image[i] != backgroundColor) {
			for(const auto &j: _DIRECTIONS) {
				auto _neighbour = i + j;
				if((_neighbour > 0) && (_neighbour < _contour.size())) {
					if(image[_neighbour] == backgroundColor) {
						_contour[i] = true;
						break;
						
					}
					
				}
			}
		}
	}
	
	return _contour;
}

bool c_skeletonizationTool::contour_removal(std::vector<bool> _contour) {
	const std::vector<int> _DIRECTIONS = {
		// Up left, Up, up right
		-image.get_width() - 1, -image.get_width(), -image.get_width() + 1,
		// right 
		1,
		// Down right, down, down left
		image.get_width() + 1, image.get_width(), image.get_width() - 1,
		// left
		-1
	};
	
	bool _changed = false;
	
	for(size_t i = 0; i != _contour.size(); i++) {
		if(_contour[i]) {
			int _neighbourhood = 0;
			int _neighbourCount = 1;
			for(const auto &j: _DIRECTIONS) {
				size_t _neighbour = i + j;
				_neighbourhood <<= 1;
				if(_neighbour < _contour.size()) {
					if((image[_neighbour] == BLACK) && ((_neighbourhood & 0x01) == 0)) {
						_neighbourhood |= 1;
						_neighbourCount <<= 1;
						
					}
				}
			}
			if(is_removable(_neighbourhood, _neighbourCount)) {
				image[i] = backgroundColor;
				_changed = true;
				
			}
		}
	}
	
	return _changed;
}

bool c_skeletonizationTool::is_removable(int _neighbourhood, int _neighbourCount) {
	bool _removable = false;
	if(_neighbourhood != 0) {
		if(_neighbourhood & 0x01) {
			_neighbourhood |= _neighbourhood << 8;
			while(_neighbourhood & 0x01) _neighbourhood >>= 1;
			
		}
		while(!(_neighbourhood & 0x01)) _neighbourhood >>= 1;
		_neighbourhood &= 0xFF;
		
		if((_neighbourhood == (_neighbourCount - 1)) && (_neighbourhood > 1)) _removable = true;
	}
	
	return _removable;
}