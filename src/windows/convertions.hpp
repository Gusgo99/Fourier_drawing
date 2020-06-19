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
#ifndef CONVERTIONS_HPP
#define CONVERTIONS_HPP
#pragma once

#include <algorithm>
#include <complex>
#include <cmath>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#include <wx/wx.h>
#include <wx/dcbuffer.h>
#pragma GCC diagnostic pop

inline int norm(const wxPoint &_point) {
	return std::sqrt(_point.x * _point.x + _point.y * _point.y);
}

inline int get_pixel_coefficient(const wxPoint &_center) {
	return std::min(_center.x, _center.y);
}

inline wxPoint to_screen_coord(const std::complex<float> &_position, const wxPoint &_center) {
	const auto _pixelCoef = get_pixel_coefficient(_center);
	
	return _center + wxPoint(_pixelCoef * _position.real(), _pixelCoef *  _position.imag());
}

inline wxPoint to_screen_coord(const std::complex<float> &_position, const wxSize &_screenSize) {
	return to_screen_coord(_position, wxPoint(_screenSize.GetWidth() / 2, _screenSize.GetHeight() / 2));
}

inline std::complex<float> to_complex_number(wxPoint _position, const wxPoint &_center) {
	const auto _pixelCoef = get_pixel_coefficient(_center); 
	
	_position -= _center;
	
	return std::complex(float(_position.x) / _pixelCoef, float(_position.y) / _pixelCoef);
}

inline std::complex<float> to_complex_number(wxPoint _position, const wxSize &_screenSize) {
	return to_complex_number(_position, wxPoint(_screenSize.GetWidth() / 2, _screenSize.GetHeight() / 2));
}

#endif