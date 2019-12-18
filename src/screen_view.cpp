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

void c_view::draw_to_surface(SDL_Surface *_surface) {
	screen.draw_to_surface(_surface);
	
	return;
}

std::shared_ptr<c_request> c_view::get_requests() {
	std::shared_ptr<c_request> _request;
	
	if(!requests.empty()) {
		_request = requests.front();
		requests.pop();
		
	}
	
	return _request;
}