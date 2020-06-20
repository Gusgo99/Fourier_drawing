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
#ifndef IMAGE_PROCESSING_TOOL_HPP
#define IMAGE_PROCESSING_TOOL_HPP
#pragma once
#include "debug.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#include <wx/wx.h>
#include <wx/rawbmp.h>
#pragma GCC diagnostic pop

#include <array>
#include <functional>
#include <vector>

class DIPTool {
	public:
		enum type {THRESHOLD, SELECTION, SKELETONIZATION};
		
		DIPTool(const type _toolType);
		~DIPTool() = default;
		
		void apply(wxBitmap &_bitmap);
		bool uses_intensity();
		bool uses_source();
		
		double intensity;
		wxPoint source;
		
		// Available directions
		static constexpr uint8_t N = 0x01;
		static constexpr uint8_t NE = 0x02;
		static constexpr uint8_t E = 0x04;
		static constexpr uint8_t SE = 0x08;
		static constexpr uint8_t S = 0x10;
		static constexpr uint8_t SO = 0x20;
		static constexpr uint8_t O = 0x40;
		static constexpr uint8_t NO = 0x80;
		static constexpr uint8_t SIDES = N | E | S | O;
		static constexpr uint8_t DIAGONALS = NE | SE | SO | NO;
		
	private:
		using forEachCallback = std::function<wxColour(wxNativePixelData::Iterator)>;
		using expandCallback = std::function<wxColour(
			wxNativePixelData::Iterator,
			std::pair<uint8_t, uint8_t>)>;
			
		void apply_threshold(wxBitmap &_bitmap);
		void apply_selection(wxBitmap &_bitmap);
		void apply_skeletonization(wxBitmap &_bitmap);
		
		// Functions to help threating bitmaps
		void for_each_pixel(wxBitmap &_bitmap, forEachCallback _callback) const;
		bool is_inside_screen(const wxPoint &_POINT, const wxSize &_SIZE) const;
		
		// Functions to treat binary images
		wxColour get_background_colour() const;
		wxColour get_foreground_colour() const;
		void find_foreground(wxBitmap &_bitmap);
		std::vector<bool> expand_source_pixel(
			wxBitmap &_bitmap,
			int _directions,
			expandCallback _callback);
		std::pair<uint8_t, uint8_t> get_neighbour_pixels(
			const wxNativePixelData &_PIXELS,
			const wxPoint _POINT,
			int _directions) const;
		bool causes_connection(const std::pair<uint8_t, uint8_t> _NEIGHBOURHOOD) const;
		
		type toolType;
		
		static const std::array<wxPoint, 8> DIRECTIONS;
};

// Helper functions
bool operator==(wxNativePixelData::Iterator _pixel, const wxColour &_colour);
bool operator!=(wxNativePixelData::Iterator _pixel, const wxColour &_colour);

#endif