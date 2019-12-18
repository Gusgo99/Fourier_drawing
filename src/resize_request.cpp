#include "resize_request.hpp"

c_resizeRequest::c_resizeRequest(int _width, int _height) {
	width = _width;
	height = _height;
	
	return;
}

c_resizeRequest::~c_resizeRequest() {
	return;
}

int c_resizeRequest::get_width() {
	return width;
}

int c_resizeRequest::get_height() {
	return height;
}