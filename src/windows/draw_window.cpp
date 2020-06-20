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
#include <wx/tglbtn.h>
#pragma GCC diagnostic pop

#include "convertions.hpp"
#include "math/kiss_fft.h"

wxBEGIN_EVENT_TABLE(drawPanel, wxPanel)
	
	// Rendering event
	EVT_PAINT(drawPanel::on_paint)
	
	// Timer event
	EVT_TIMER(ID::DW::TMR_REFRESH, drawPanel::on_refresh_timer)
	
wxEND_EVENT_TABLE()

drawPanel::drawPanel(const std::vector<std::complex<float>> &_points,
	wxWindow *_parent,
	const wxSize &_size):
wxPanel(_parent, ID::DW::PANEL) {
	SetSize(_size);
	
	SetBackgroundStyle(wxBG_STYLE_PAINT);
	
	refreshTimer.SetOwner(this, ID::DW::TMR_REFRESH);
	refreshTimer.Start(50, wxTIMER_CONTINUOUS);
	
	angle = 0.0;
	angularSpeed = 3.1415 / (_points.size() * 100);
	drawCircles = true;
	drawPoints = false;
	
	points = _points;
	if(points.size() > 0) generate_coefficients(points);
	
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

void drawPanel::set_circle_state(const bool _drawCircles) {
	drawCircles = _drawCircles;
	
}

void drawPanel::set_point_state(const bool _drawPoints) {
	drawPoints = _drawPoints;
	
}

void drawPanel::render(wxDC &_temporary, wxDC &_permanent) {
	const wxPoint _screenCenter(GetSize().GetWidth() / 2, GetSize().GetHeight() / 2);
	const int pointRadius = int(_temporary.GetPPI().GetWidth() * 0.065);
	int _pixelCoef = get_pixel_coefficient(_screenCenter);
	wxPoint _center = _screenCenter;
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
		_newCenter = to_screen_coord(_currentPosition, _screenCenter);
		if(drawCircles) {
			_temporary.DrawLine(_center, _newCenter);
			_temporary.DrawCircle(_center, std::abs(coefficients[i]) * _pixelCoef);
			
		}
		_center = _newCenter;
		
	}
	
	if(drawPoints) {
		_temporary.SetPen(wxPen(POINTSCOLOUR));
		
		for(auto i: points) {
			wxPoint _pointPosition = to_screen_coord(i, _screenCenter);
			
			_temporary.DrawCircle(_pointPosition, pointRadius);
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

void drawPanel::generate_coefficients(const std::vector<std::complex<float>> &_points) {
	kiss_fft_cfg _cfg = kiss_fft_alloc(_points.size(), 0, nullptr, nullptr);
	std::vector<kiss_fft_cpx> _samples(_points.size()), _pOutput(_points.size()), _nOutput(_points.size());
    for(size_t i = 0; i < _points.size(); i++) {
		_samples[i].r = _points[i].real();
		_samples[i].i = _points[i].imag();
		
	}
    
	kiss_fft(_cfg, _samples.data(), _pOutput.data());
	for(auto &i: _samples) i.i = -i.i;
	kiss_fft(_cfg, _samples.data(), _nOutput.data());
	
    kiss_fft_free(_cfg);
	
	coefficients.reserve(_points.size());
	for(size_t i = 0; i < _points.size(); i++) {
		if((i % 2) == 0) coefficients.emplace_back(_nOutput[i / 2].r / _points.size(), -_nOutput[i / 2].i / _points.size());
		else coefficients.emplace_back(_pOutput[1 + i / 2].r / _points.size(), _pOutput[1 + i / 2].i / _points.size());
		
	}
	
}

wxBEGIN_EVENT_TABLE(drawFrame, wxFrame)
	
	// Tool bar buttons
	EVT_TOGGLEBUTTON(ID::DW::TB_HIDECIRCLES, drawFrame::on_hide_circles)
	EVT_TOGGLEBUTTON(ID::DW::TB_SHOWPOINTS, drawFrame::on_show_points)
	EVT_BUTTON(ID::DW::TB_CLEAR, drawFrame::on_clear)
	
wxEND_EVENT_TABLE()

drawFrame::drawFrame(const std::vector<std::complex<float>> &_points, wxWindow *_parent, const wxSize &_size):
wxFrame(_parent, ID::DW::FRAME, _("Drawing"), wxDefaultPosition, _size),
panel(new drawPanel(_points, this, GetSize())) {
	if(_size == wxDefaultSize) Maximize(true);
	
	CreateToolBar();
	
	wxButton *_button;
	GetToolBar() -> AddControl(new wxToggleButton(GetToolBar(), ID::DW::TB_HIDECIRCLES, "Hide circles"));
	GetToolBar() -> AddControl (new wxToggleButton(GetToolBar(), ID::DW::TB_SHOWPOINTS, "Show points"));
	GetToolBar() -> AddControl(_button = new wxButton(GetToolBar(), ID::DW::TB_CLEAR, "Clear"));
	
	GetToolBar() -> Realize();
	
	GetToolBar() -> SetSize(GetToolBar() -> GetSize() + wxSize(0, _button -> GetPosition().y));
	
	SetAutoLayout(true);
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