#include <algorithm>
#include <list>
#include <queue>

#include "selection_tool.hpp"

c_selectionTool::c_selectionTool(c_image &_image) : c_modifyingImageProcessingTool(_image) {
	x = -1;
	y = -1;
	
	return;
}

c_selectionTool::~c_selectionTool() {
	
	return;
}

void c_selectionTool::apply() {
	if((x != -1) && (y != -1)) {
		auto _selected = get_selected_pixels();
		
		for(size_t i = 0; i != _selected.size(); i++) {
			if(_selected[i]) image[i] = SELECTEDCOLOR;
			else image[i] = BACKGROUNDCOLOR;
			
		}
	}
	
	return;
}

void c_selectionTool::increase_effect() {
	
	return;
}

void c_selectionTool::decrease_effect() {
	
	return;
}

void c_selectionTool::reset_effect() {
	x = -1;
	y = -1;
	
	return;
}

void c_selectionTool::affect_pixel(const int &_x, const int &_y) {
	x = _x;
	y = _y;
	
	return;
}

std::vector<bool> c_selectionTool::get_selected_pixels() {
	// Lookup table for the increment in index for adjacent directions
	std::vector<int> _DIRECTIONS = {
		// Up, down, left, right
		-image.get_width(), image.get_width(), -1, 1,
		// Up left, up right, down left, down right
		-image.get_width() - 1, -image.get_width() + 1, image.get_width() - 1, image.get_width() + 1
	};
	
	std::vector<bool> _selected(image.get_size());
	std::queue<size_t> _neighbourhood;
	
	_neighbourhood.push(image.to_lin_coord(x, y));
		
		while(!_neighbourhood.empty()) {
			auto _currentPixel = _neighbourhood.front();
			_neighbourhood.pop();
			for(const auto &i: _DIRECTIONS) {
				auto _neighbour = _currentPixel + i;
				if((_neighbour > 0) && (_neighbour < _selected.size())) {
					if(!_selected[_neighbour]) {
						auto _pixelColor = image[_neighbour];
						if(_pixelColor == BLACK) {
							_selected[_neighbour] = true;
							_neighbourhood.push(_neighbour);
							
						}
					}
				}
			}
		}
	
	return _selected;
}