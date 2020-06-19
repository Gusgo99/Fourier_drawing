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
#ifndef DIP_WINDOW_HPP
#define DIP_WINDOW_HPP
#pragma once
#include "debug.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#include <wx/wx.h>
#include <wx/dcbuffer.h>
#pragma GCC diagnostic pop

#include <memory>

#include "image_processing_tools/DIP_tool.hpp"
#include "IDs.hpp"

class DIPPanel : public wxPanel {
	public:
		DIPPanel(const wxImage &_image, wxWindow *_parent = nullptr);
		
		void on_paint(wxPaintEvent &_event);
		
		void on_left_down(wxMouseEvent &_event);
		
		void render(wxDC &_dc);
		
		void add_tool(const DIPTool::type _type);
		void remove_tool();
		void clear_tools();
		void select_tool(const int _selectedTool);
		void set_tool_intensity(const double _intensity);
		double get_tool_intensity();
		
		bool should_show_slider();
		
		void set_points_state(const bool _showPoints);
		
	private:
		std::vector<DIPTool> tools;
		int selectedTool;
		wxBitmap origBitmap;
	
		bool showPoints;
		
		wxDECLARE_EVENT_TABLE();
	
};

class DIPFrame : public wxFrame {
	public:
		DIPFrame(const wxImage &_image, wxWindow *_parent = nullptr);
		
		void on_clear(wxCommandEvent &_event);
		void on_remove(wxCommandEvent &_event);
		void on_add_threshold(wxCommandEvent &_event);
		void on_add_selection(wxCommandEvent &_event);
		void on_add_skeletonization(wxCommandEvent &_event);
		void on_show_points(wxCommandEvent &_event);
		void on_get_points(wxCommandEvent &_event);
		
		void on_list_select(wxCommandEvent &_event);
		
		void on_slider_scroll(wxCommandEvent &_event);
	
	private:
		DIPPanel *panel;
		wxListBox *list;
		wxSlider *slider;
		
		void add_tool(const std::string _toolname, const DIPTool::type _type);
		void refresh_tool_info();
		
		static constexpr int SLIDERLIMIT = 2048;
		
		wxDECLARE_EVENT_TABLE();
	
};

#endif