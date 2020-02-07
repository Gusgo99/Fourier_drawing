#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wtype-limits"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#pragma GCC diagnostic pop

#include <cmath>

#include "image.hpp"

c_image::c_image() {
	width = 0;
	height = 0;
	
	return;
}

c_image::c_image(int _width, int _height) {
	resize(_width, _height);
	
	return;
}

c_image::c_image(const std::string &_filename) {
	load_file(_filename);
	
	return;
}

s_color& c_image::operator[](const size_t &_i) {
	return colors[_i % colors.size()];
}

const s_color& c_image::get_color(const size_t &i) const {
	return colors[i];
}

void c_image::load_file(const std::string &_filename) {
	int _channels;
	
	stbi_uc *_pixels = stbi_load(_filename.c_str(), &width, &height, &_channels, STBI_rgb_alpha);
	
	resize(width, height);
	
	if(_pixels != nullptr) {
		for(size_t i = 0; i != colors.size(); i++) {
			colors[i].red = _pixels[_channels * i + 0] / float(0xFF);
			colors[i].green = _pixels[_channels * i + 1] / float(0xFF);
			colors[i].blue = _pixels[_channels * i + 2] / float(0xFF);
			
		}
	}
	
	stbi_image_free(_pixels);
	
	return;
}

int c_image::get_width() const {
	return width;
}

int c_image::get_height() const {
	return height;
}

size_t c_image::get_size() const {
	return colors.size();
}

void c_image::set_width(int _width) {
	resize(_width, height);
	
	return;
}

void c_image::set_height(int _height) {
	resize(width, _height);
	
	return;
}

void c_image::resize(int _width, int _height) {
	width = _width;
	height = _height;
	
	colors.resize(_width * _height);
	screenRatio = _width / float(_height);
	
	clear_image(WHITE);
	
	return;
}

void c_image::draw_to_surface(SDL_Surface *_surface) {
	int *_pixels = reinterpret_cast<int*>(_surface -> pixels);
	
	size_t _numPixels = std::min(size_t(_surface -> w * _surface -> h), colors.size());
	
	for(size_t i = 0; i < _numPixels; i++) {
		_pixels[i] = SDL_MapRGB(_surface -> format, colors[i].red * 0xFF, colors[i].green * 0xFF, colors[i].blue * 0xFF);
		
	}
	
	return;
}

void c_image::clear_image(const s_color &_color) {
	for(auto &i: colors) i = _color;
	
	return;
}

float& c_image::red(const size_t &_i) {
	return colors[_i % colors.size()].red;
}

float& c_image::green(const size_t &_i) {
	return colors[_i % colors.size()].green;
}

float& c_image::blue(const size_t &_i) {
	return colors[_i % colors.size()].blue;
}

size_t c_image::to_lin_coord(float _x, float _y) const {
	_y *= screenRatio;
	
	size_t _pos = width * size_t(height * (_y + 1.0f) / 2.0f);
	
	_pos += size_t((_x + 1.0f) * width / 2.0f);
	
	if(_pos > colors.size()) _pos = 0;
	
	return _pos;
}

size_t c_image::to_lin_coord(const int &_x, const int &_y) const {
	size_t _pos = _y * width + _x;
	
	if(_pos > colors.size()) _pos = 0;
	
	return _pos;
}

std::pair<float, float> c_image::to_rel_coord(const int &_x, const int &_y) const {
	float _xO = (_x * 2.0f / float(width)) - 1.0f;
	float _yO = (_y * 2.0f / float(height)) - 1.0f;
	
	_yO /= screenRatio;
	
	return std::pair(_xO, _yO);
}

std::pair<float, float> c_image::to_rel_coord(const size_t &_pos) const {
	float _xO = (_pos % width) * 2.0f / float(width) - 1.0f;
	float _yO = (_pos - (_pos % width)) * 2.0f / float(width) - 1.0f;
	
	_yO /= screenRatio;
	
	return std::pair(_xO, _yO);
}

std::pair<int, int> c_image::to_abs_coord(float _x, float _y) const {
	_y *= screenRatio;
	
	return std::pair(int((_x + 1.0f) * width) / 2, int(height * (_y + 1.0f) / 2));
}

std::pair<int, int> c_image::to_abs_coord(const size_t &_pos) const {
	return std::pair(_pos % width, _pos / width);
}

void c_image::draw_circle(const float &_x, const float &_y, const float &_r, const s_color &_color) {
	int _pixX = to_abs_coord(_x, _y).first;
	int _pixY = to_abs_coord(_x, _y).second;
	int _radius = _r * width / 2.0f;
	
	for(int i = 0; i < (_radius * 0.71); i++) {
		colors[to_lin_coord(_pixX + i, _pixY + static_cast<unsigned>(sqrt(_radius * _radius - i * i)))] = _color;
		colors[to_lin_coord(_pixX - i, _pixY + static_cast<unsigned>(sqrt(_radius * _radius - i * i)))] = _color;
		colors[to_lin_coord(_pixX + i, _pixY - static_cast<unsigned>(sqrt(_radius * _radius - i * i)))] = _color;
		colors[to_lin_coord(_pixX - i, _pixY - static_cast<unsigned>(sqrt(_radius * _radius - i * i)))] = _color;
		
		colors[to_lin_coord(_pixX + static_cast<unsigned>(sqrt(_radius * _radius - i * i)), _pixY + i)] = _color;
		colors[to_lin_coord(_pixX - static_cast<unsigned>(sqrt(_radius * _radius - i * i)), _pixY + i)] = _color;
		colors[to_lin_coord(_pixX + static_cast<unsigned>(sqrt(_radius * _radius - i * i)), _pixY - i)] = _color;
		colors[to_lin_coord(_pixX - static_cast<unsigned>(sqrt(_radius * _radius - i * i)), _pixY - i)] = _color;
		
	}
	
	return;
}

void c_image::draw_dot(const float &_x, const float &_y, const s_color &_color) {
	auto _pos = to_lin_coord(_x, _y);
	
	colors[_pos] = _color;
	if((_pos + 1) < colors.size()) colors[_pos + 1] = _color;
	if((_pos - 1) < colors.size()) colors[_pos - 1] = _color;
	if((_pos + width) < colors.size()) colors[_pos + width] = _color;
	if((_pos - width) < colors.size()) colors[_pos - width] = _color;
	if((_pos + width + 1) < colors.size()) colors[_pos + width + 1] = _color;
	if((_pos + width - 1) < colors.size()) colors[_pos + width - 1] = _color;
	if((_pos - width + 1) < colors.size()) colors[_pos - width + 1] = _color;
	if((_pos + width - 1) < colors.size()) colors[_pos + width - 1] = _color;
	
	return;
}

void c_image::draw_line(float _x0, float _y0, float _x1, float _y1, const s_color &_color) {
	if(_x0 > _x1) {
		std::swap(_x0, _x1);
		std::swap(_y0, _y1);
		
	}
	
	int _pixX1 = to_abs_coord(_x0, _y0).first;
	int _pixX2 = to_abs_coord(_x1, _y1).first;
	int _pixY1 = to_abs_coord(_x0, _y0).second;
	int _pixY2 = to_abs_coord(_x1, _y1).second;
	
	if(abs(_pixY2 - _pixY1) < abs(_pixX2 - _pixX1)) {
		float _slope = (_pixY2 - _pixY1) / float(_pixX2 - _pixX1);
		int _lim = _pixX2 - _pixX1;
		for(int i = 0; i != _lim; i++) {
			colors[to_lin_coord(_pixX1 + i, int(_pixY1 + i * _slope))] = _color;
			
		}
	}
	else {
		if(_y0 > _y1) {
			std::swap(_pixX1, _pixX2);
			std::swap(_pixY1, _pixY2);
			
		}
		float _slope = (_pixX2 - _pixX1) / float(_pixY2 - _pixY1);
		int _lim = _pixY2 - _pixY1;
		for(int i = 0; i < _lim; i++) {
			colors[to_lin_coord(int(_pixX1 + i * _slope), _pixY1 + i)] = _color;
			
		}
	}
	
	return;
}