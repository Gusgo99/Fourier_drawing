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
#ifndef DRAW_MODE_HPP
#define DRAW_MODE_HPP
#pragma once
#include "debug.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#include <wx/wx.h>
#include <wx/dcbuffer.h>
#pragma GCC diagnostic pop

#include <complex>
#include <vector>

#include "IDs.hpp"

class drawPanel : public wxPanel {
	public:
		drawPanel(const std::vector<std::complex<float>> &_points,
			wxWindow *_parent = nullptr,
			const wxSize &_size = wxDefaultSize);
		
		void on_paint(wxPaintEvent &_event);
		void on_refresh_timer(wxTimerEvent &_event);
		
		void clear_image();
		void set_circle_state(const bool _drawCircles);
		void set_point_state(const bool _drawPoints);
	
	private:
		const wxColour POINTSCOLOUR = wxColour(0, 0, 0);
		const wxColour CIRCLECOLOUR = wxColour(0, 0, 255);
		const wxColour TRACECOLOUR = wxColour(0, 255, 0);
		
		void render(wxDC &_temporary, wxDC &_permanent);
		void generate_coefficients(const std::vector<std::complex<float>> &_points);
		
		std::vector<std::complex<float>> points;
		std::vector<std::complex<float>> coefficients;
		
		wxTimer refreshTimer;
		wxBitmap permanentBitmap;
		wxPoint lastCenter;
		double angularSpeed;
		double angle;
		bool drawCircles;
		bool drawPoints;
		
		wxDECLARE_EVENT_TABLE();
	
};

class drawFrame : public wxFrame {
	public:
		drawFrame(const std::vector<std::complex<float>> &_points,
			wxWindow *_parent = nullptr,
			const wxSize &_size = wxDefaultSize);
		
		void on_clear(wxCommandEvent &_event);
		void on_hide_circles(wxCommandEvent &_event);
		void on_show_points(wxCommandEvent &_event);
	
	private:
		drawPanel *panel;
		
		wxDECLARE_EVENT_TABLE();
	
};

/*#include <functional>
#include <map>

#include "complex.hpp"
#include "fourier.hpp"
#include "screen_view.hpp"

class c_drawMode;

typedef std::function<void(c_drawMode*, const SDL_KeyboardEvent&)> t_drawModeKeyHandler;

class c_drawMode : public c_view {
	public:
		c_drawMode(int _width, int _height);
		~c_drawMode();
		
		void resize(int _width, int _height);
		
		void render_image(const double &_time);
		void operator()(const SDL_Event &_event);
		void activate(const c_screenChangeRequest *_request);
		
		unsigned get_max_framerate() {return 60;}
		unsigned get_id() {return 2;}
	
	private:
		const s_color BACKGROUNDCOLOR = WHITE;
		const s_color CIRCLECOLOR = BLUE;
		const s_color TRACECOLOR = GREEN;
		const s_color POINTCOLOR = BLACK;
		
		bool drawCircles;
		bool drawPoints;
		float speed;
		c_fourier fourier;
		c_image auxScreen;
		std::vector<c_complex> points;
	
		c_complex draw_series_truncation(float _time);
		
		void key_handler_p(const SDL_KeyboardEvent &_event);
		void key_handler_s(const SDL_KeyboardEvent &_event);
		void key_handler_c(const SDL_KeyboardEvent &_event);
		void key_handler_m(const SDL_KeyboardEvent &_event);
		void key_handler_F1(const SDL_KeyboardEvent &_event);
		void key_handler_arrow_up(const SDL_KeyboardEvent &_event);
		void key_handler_arrow_down(const SDL_KeyboardEvent &_event);
	
		static const std::map<SDL_Keycode, t_drawModeKeyHandler> KEYHANDLERS;
	
};*/

#endif