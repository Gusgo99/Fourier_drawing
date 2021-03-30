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
#ifndef DIP_STRATEGY_HPP
#define DIP_STRATEGY_HPP
#pragma once

#include "wxWidgets_headers.hpp"

#include <any>
#include <array>
#include <functional>
#include <vector>

#include  "image_processing/grid.hpp"

namespace DIP {
	class strategy {
	public:
		strategy(const bool _usesIntensity, const bool _usesSource, const bool _hasOutput);
		virtual ~strategy() = default;
		
		virtual void apply(wxBitmap &_bitmap) = 0;

		const std::any& get_info() const;

		strategy& set_source(const wxPoint &_source);
		strategy& set_source(const wxCoord _x, const wxCoord _y);
		const wxPoint& get_source();

		strategy& set_intensity(const double _intensity);
		double get_intensity();

		const std::any& get_output();

	public:
		const bool usesIntensity;
		const bool usesSource;
		const bool hasOutput;
		
	public:
		using pixelData = wxNativePixelData::Iterator;
		
	protected:
		using forEachCallback = std::function<wxColour(pixelData)>;
		using expandCallback = std::function<wxColour(pixelData, std::pair<uint8_t, uint8_t>)>;
		using selectedCallback = std::function<wxColour(pixelData)>;
		
		// Functions to iterate through images
		void for_each_pixel(wxBitmap &_bitmap, forEachCallback _callback);
		
		// Binary images analysis
		grid generate_grid(wxBitmap &_bitmap, const std::function<int(wxColour)> &_converter);
		grid generate_grid(
			wxBitmap &_bitmap,
			const std::vector<std::pair<wxColour, grid::data>> &_colours = {}
		);
		void draw_grid_to_bitmap(
			wxBitmap &_bitmap,
			const grid &_grid,
			const std::vector<std::pair<grid::data, wxColour>> &_colours = {}
		);
		
		// General helper functions
		static bool is_inside_screen(const wxPoint &_point, const wxSize &_size);
		void find_foreground(wxBitmap &_bitmap);
		
	protected:
		std::any info;
		double intensity;
		wxPoint source;
		
		static const wxColour BACKGROUNDCOLOUR;
		static const wxColour FOREGROUNDCOLOUR;
	};
}

#endif