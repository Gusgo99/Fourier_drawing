#include "screen_view.hpp"

c_view::c_view(int _width, int _height) {
	screen.resize(_width, _height);
	
	return;
}

c_view::~c_view() {
	
	return;
}

const c_image& c_view::get_image() const {
	return screen;
}

void c_view::resize(int _width, int _height) {
	screen.resize(_width, _height);
	
	return;
}

void c_view::draw_to_buffer(void *_buffer) {
	screen.draw_to_buffer(_buffer);
	
	return;
}

bool c_view::should_screen_change() {
	if(screenChange) {
		screenChange = false;
		return true;
		
	}
	
	return false;
}