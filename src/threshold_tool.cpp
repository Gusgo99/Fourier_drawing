#include "threshold_tool.hpp"

c_thresholdTool::c_thresholdTool(c_image &_image) : c_modifyingImageProcessingTool(_image) {
	reset_effect();
	
	return;
}

c_thresholdTool::~c_thresholdTool() {
	
	return;
}

void c_thresholdTool::apply() {
	for(size_t i = 0; i != image.get_size(); i++) {
		auto &_pixel = image[i];
		if((_pixel.red + _pixel.green + _pixel.blue) < threshold) _pixel = BLACK;
		else _pixel = WHITE;
		
	}
	
	return;
}

void c_thresholdTool::increase_effect() {
	threshold += 0.01;
	if(threshold > 2.99) threshold = 2.99;
	
	return;
}

void c_thresholdTool::decrease_effect() {
	threshold -= 0.01;
	if(threshold < 0.01) threshold = 0.01;
	
	return;
}

void c_thresholdTool::reset_effect() {
	threshold = 1.5;
	
	return;
}

void c_thresholdTool::affect_pixel(const int &_x, const int &_y) {
	if(_x == _y) {}
	
	return;
}