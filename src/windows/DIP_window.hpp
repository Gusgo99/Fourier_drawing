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

#include "wxWidgets_headers.hpp"

#include <memory>

#include "edit_window.hpp"

#include "image_processing/strategies/DIP_strategy.hpp"
#include "image_processing/strategies/DIP_strategy_utilities.hpp"

class DIPPanel : public wxPanel {
	public:
		DIPPanel(const wxImage &_image, wxWindow *_parent, wxWindow *_pointsDestination);
		DIPPanel(DIPPanel&) = delete;
		DIPPanel& operator=(DIPPanel&) = delete;
		
		void on_paint(wxPaintEvent &_event);
		
		void on_left_down(wxMouseEvent &_event);
		void on_right_down(wxMouseEvent &_event);
		
		void render(wxDC &_dc);
		
		void add_tool(const DIP::strategies _toolType);
		void remove_tool();
		void clear_tools();
		void select_tool(const int _selectedTool);
		void set_tool_intensity(const double _intensity);
		double get_tool_intensity();
		
		bool should_show_slider();
		
		void set_points_state(const bool _showPoints);
		
	private:
		void paint_frame(wxDC &_dc);
		void apply_valid_tools_to_bitmap(wxBitmap &_bitmap);
		void handle_information_from_tools();

		std::vector<std::unique_ptr<DIP::strategy>> tools;
		size_t selectedTool;
		wxBitmap origBitmap;
		bool showPoints;

		editFrame *const pointsDestination;

		static constexpr size_t INVALIDTOOL = std::numeric_limits<size_t>::max();
		
		wxDECLARE_EVENT_TABLE();
	
};

class DIPFrame : public wxFrame {
	public:
		DIPFrame(const wxImage &_image, wxWindow *_parent);
		DIPFrame(DIPFrame&) = delete;
		DIPPanel& operator=(DIPFrame&) = delete;
		
		void on_clear(wxCommandEvent &_event);
		void on_remove_tool(wxCommandEvent &_event);
		void on_add_tool(wxCommandEvent &_event);
		void on_show_points(wxCommandEvent &_event);
		void on_get_points(wxCommandEvent &_event);
		
		void on_list_select(wxCommandEvent &_event);
		
		void on_slider_scroll(wxCommandEvent &_event);
		void on_intensity_entry(wxSpinEvent &_event);
	
	private:
		void add_tool(const wxString _toolName, const int _toolType);
		void refresh_tool_info();

		DIP::strategies map_selected_item_to_strategy(const int _selected);
		
		DIPPanel *panel;
		wxListBox *list;
		wxSlider *slider;
		wxChoice *toolAdded;
		wxSpinCtrl *intensityEntry;
		
		int sliderLimit;
		
		wxDECLARE_EVENT_TABLE();
	
};

#endif