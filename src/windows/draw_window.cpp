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
#include "draw_window.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#include <wx/dcbuffer.h>
#include <wx/tglbtn.h>
#include <wx/xrc/xmlres.h>
#pragma GCC diagnostic pop

#include "convertions.hpp"
#include "math/kiss_fft.h"

wxBEGIN_EVENT_TABLE(drawPanel, wxPanel)
	
	// Rendering event
	EVT_PAINT(drawPanel::on_paint)
	
	// Timer event
	EVT_TIMER(TIMERID, drawPanel::on_refresh_timer)
	
wxEND_EVENT_TABLE()

drawPanel::drawPanel(
	const std::vector<std::complex<float>> &_POINTS,
	wxWindow *_parent,
	const wxSize &_SIZE):
wxPanel(_parent, wxID_ANY) {
	SetSize(_SIZE);
	
	SetBackgroundStyle(wxBG_STYLE_PAINT);
	
	refreshTimer.SetOwner(this, TIMERID);
	refreshTimer.Start(50, wxTIMER_CONTINUOUS);
	
	points = _POINTS;
	if(points.size() > 0) generate_coefficients(points);
	
	angle = 0.0;
	angularSpeed = 3.1415 / (points.size() * 100);
	drawCircles = true;
	drawPoints = false;
	
}

void drawPanel::on_paint([[maybe_unused]]wxPaintEvent &_event) {
	wxAutoBufferedPaintDC _dc(this);
	_dc.Clear();
	wxMemoryDC _auxDC(permanentBitmap);
	PrepareDC(_dc);
	
	if(!permanentBitmap.IsOk() || GetSize() != permanentBitmap.GetSize()) {
		permanentBitmap.Create(GetSize(), 32);
		_auxDC.SelectObject(permanentBitmap);
		_auxDC.Clear();
		
	}
	
	_dc.Blit(0, 0, GetSize().GetWidth(), GetSize().GetHeight(), &_auxDC, 0, 0);
	
	if(!coefficients.empty()) render(_dc, _auxDC);
	
}

void drawPanel::on_refresh_timer([[maybe_unused]]wxTimerEvent &_event) {
	angle += angularSpeed;
	if(angle > 6.28) angle = 0;
	Refresh();
	
}

void drawPanel::clear_image() {
	permanentBitmap.Create(1, 1);
	
}

void drawPanel::set_circle_state(const bool _DRAWCIRCLES) {
	drawCircles = _DRAWCIRCLES;
	
}

void drawPanel::set_point_state(const bool _DRAWPOINTS) {
	drawPoints = _DRAWPOINTS;
	
}

void drawPanel::render(wxDC &_temporary, wxDC &_permanent) {
	const wxPoint _SCREENCENTER(GetSize().GetWidth() / 2, GetSize().GetHeight() / 2);
	const int _POINTRADIUS = int(_temporary.GetPPI().GetWidth() * 0.065);
	int _pixelCoef = get_pixel_coefficient(_SCREENCENTER);
	wxPoint _center = _SCREENCENTER;
	wxPoint _newCenter = wxPoint(0, 0);
	std::complex<float> _coef(std::cos(coefficients.size() * angle), std::sin(coefficients.size() * angle));
	std::complex<float> _currentCoef(1, 0);
	std::complex<float> _currentPosition(0, 0);
	
	_temporary.SetPen(wxPen(CIRCLECOLOUR));
	_temporary.SetBrush(*wxTRANSPARENT_BRUSH);
	
	for(size_t i = 0; i < coefficients.size(); i++) {
		_currentPosition += _currentCoef * coefficients[i];
		if((i % 2) == 0) {
			_currentCoef = std::conj(_currentCoef);
			_currentCoef *= _coef;
			
		}
		else _currentCoef = std::conj(_currentCoef);
		_newCenter = to_screen_coord(_currentPosition, _SCREENCENTER);
		if(drawCircles) {
			_temporary.DrawLine(_center, _newCenter);
			_temporary.DrawCircle(_center, std::abs(coefficients[i]) * _pixelCoef);
			
		}
		_center = _newCenter;
		
	}
	
	if(drawPoints) {
		_temporary.SetPen(wxPen(POINTSCOLOUR));
		
		for(auto i: points) {
			wxPoint _pointPosition = to_screen_coord(i, _SCREENCENTER);
			
			_temporary.DrawCircle(_pointPosition, _POINTRADIUS);
			_temporary.DrawPoint(_pointPosition);
			
		}
	}
	
	_permanent.SetPen(*wxTRANSPARENT_PEN);
	_permanent.SetBrush(wxBrush(TRACECOLOUR));
	
	_permanent.DrawCircle(_newCenter, 4);
	
	lastCenter -= _newCenter;
	lastCenter.x *= lastCenter.x;
	lastCenter.y *= lastCenter.y;
	if(lastCenter.x + lastCenter.y > 30) angularSpeed *= 0.85;
	else if(lastCenter.x + lastCenter.y < 16) angularSpeed *= 1.1;
	
	lastCenter = _newCenter;
	
	
}

void drawPanel::generate_coefficients(const std::vector<std::complex<float>> &_POINTS) {
	kiss_fft_cfg _cfg = kiss_fft_alloc(_POINTS.size(), 0, nullptr, nullptr);
	std::vector<kiss_fft_cpx> _samples(_POINTS.size()), _pOutput(_POINTS.size()), _nOutput(_POINTS.size());
    for(size_t i = 0; i < _POINTS.size(); i++) {
		_samples[i].r = _POINTS[i].real();
		_samples[i].i = _POINTS[i].imag();
		
	}
    
	kiss_fft(_cfg, _samples.data(), _pOutput.data());
	for(auto &i: _samples) i.i = -i.i;
	kiss_fft(_cfg, _samples.data(), _nOutput.data());
	
    kiss_fft_free(_cfg);
	
	coefficients.reserve(_POINTS.size());
	for(size_t i = 0; i < _POINTS.size(); i++) {
		if((i % 2) == 0) {
			coefficients.emplace_back(
				_nOutput[i / 2].r / _POINTS.size(),
				-_nOutput[i / 2].i / _POINTS.size());
				
		}
		else {
			coefficients.emplace_back(
				_pOutput[1 + i / 2].r / _POINTS.size(),
				_pOutput[1 + i / 2].i / _POINTS.size());
				
		}
	}
}

wxBEGIN_EVENT_TABLE(drawFrame, wxFrame)
	
	// Tool bar buttons
	EVT_TOGGLEBUTTON(XRCID("DrawWindow_HideCircles"), drawFrame::on_hide_circles)
	EVT_TOGGLEBUTTON(XRCID("DrawWindow_ShowPoints"), drawFrame::on_show_points)
	EVT_BUTTON(XRCID("DrawWindow_Clear"), drawFrame::on_clear)
	
wxEND_EVENT_TABLE()

drawFrame::drawFrame(
	const std::vector<std::complex<float>> &_POINTS,
	wxWindow *_parent,
	const wxSize _SIZE,
	const bool _MAXIMIZE) {
	
	wxXmlResource::Get() -> LoadFrame(this, _parent, "DrawWindow");
	
	SetSize(_SIZE);
	Maximize(_MAXIMIZE);
	
	panel = new drawPanel(_POINTS, this, GetSize());
	
	Show(true);
	
}

void drawFrame::on_clear([[maybe_unused]]wxCommandEvent &_event) {
	panel -> clear_image();
	
}

void drawFrame::on_hide_circles(wxCommandEvent &_event) {
	wxToggleButton *_button = dynamic_cast<wxToggleButton*>(_event.GetEventObject());
	if(_button != nullptr) panel -> set_circle_state(!_button -> GetValue());
	
}

void drawFrame::on_show_points(wxCommandEvent &_event) {
	wxToggleButton *_button = dynamic_cast<wxToggleButton*>(_event.GetEventObject());
	if(_button != nullptr) panel -> set_point_state(_button -> GetValue());
	
}