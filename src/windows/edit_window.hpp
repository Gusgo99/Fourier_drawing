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
#ifndef EDIT_WINDOW_HPP
#define EDIT_WINDOW_HPP
#pragma once

#include "debug.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#include <wx/wx.h>
#pragma GCC diagnostic pop

#include <complex>
#include <string>
#include <vector>

class editPanel : public wxPanel {
	public:
		editPanel(wxWindow *_parent, const wxSize &_size);
		
		void clear_points();
		
		// Mouse event handlers
		void on_left_down(wxMouseEvent &_event);
		void on_left_up(wxMouseEvent &_event);
		void on_motion(wxMouseEvent &_event);
		void on_right_up(wxMouseEvent &_event);
		
		// Render event handler
		void on_paint(wxPaintEvent &_event);
		
		// Resize event handler
		void on_resize(wxSizeEvent &_event);
		
		const std::vector<std::complex<float>>& get_points() const;
		
		void show_points(const bool _show);
		
		bool read_points(const std::string &_fileName);
		bool save_points(const std::string &_fileName);
		bool verify_wip();
		
	private:
		void render(wxDC &_dc);
		
		std::vector<std::complex<float>> points;
		std::vector<std::complex<float>> savedPoints;
		std::vector<std::complex<float>>::iterator selectedPoint;
		std::vector<std::complex<float>>::iterator lastSelectedPoint;
		
		int pointRadius;
		
		const wxColour POINTSCOLOUR = wxColour(0, 0, 255);
		const wxColour TRACECOLOUR = wxColour(0, 255, 0);
		const wxColour SELECTEDCOLOUR = wxColour(255, 0, 0);
		
		wxDECLARE_EVENT_TABLE();
	
};

class editFrame : public wxFrame {
	public:
		editFrame();
		
		// File menu
		void on_open_file(wxCommandEvent &_event);
		void on_open_image(wxCommandEvent &_event);
		void on_save_file(wxCommandEvent &_event);
		void on_save_as_file(wxCommandEvent &_event);
		void on_exit(wxCommandEvent &_event);
		
		// Edit menu
		void on_clear(wxCommandEvent &_event);
		void on_draw(wxCommandEvent &_event);
		
		// Info menu
		void on_about(wxCommandEvent &_event);
		
		editPanel* get_panel() { return panel; }
		
	private:
		editPanel *panel;
		std::string currentFile;
		
		wxDECLARE_EVENT_TABLE();
	
};

#endif