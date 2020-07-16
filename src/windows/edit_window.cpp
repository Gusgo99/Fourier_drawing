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
#include "edit_window.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#include <wx/dcbuffer.h>
#include <wx/aboutdlg.h>
#include <wx/xrc/xmlres.h>
#pragma GCC diagnostic pop

#include <fstream>

#include "convertions.hpp"
#include "DIP_window.hpp"
#include "draw_window.hpp"

wxBEGIN_EVENT_TABLE(editPanel, wxPanel)

	// Mouse event
	EVT_LEFT_DOWN(editPanel::on_left_down)
	EVT_LEFT_UP(editPanel::on_left_up)
	EVT_MOTION(editPanel::on_motion)
	EVT_RIGHT_UP(editPanel::on_right_up)

	// Rendering event
	EVT_PAINT(editPanel::on_paint)

	// Resize event
	EVT_SIZE(editPanel::on_resize)

wxEND_EVENT_TABLE()

editPanel::editPanel(wxWindow *_parent, const wxSize &_size):
wxPanel(_parent, wxID_ANY) {
	SetSize(_size);
	
	clear_points();
	
	SetBackgroundStyle(wxBG_STYLE_PAINT);
	
}

void editPanel::clear_points() {
	points.clear();
	selectedPoint = points.end();
	lastSelectedPoint = points.end();
	
	Refresh();
}

void editPanel::on_left_down(wxMouseEvent &_event) {
	for(auto i = points.begin(); i != points.end(); i++) {
		wxPoint _relPos = to_screen_coord(*i, GetSize()) - wxPoint(_event.GetX(), _event.GetY());
		int _dist = (_relPos .x * _relPos.x) + (_relPos.y * _relPos.y);
		if(_dist <= pointRadius * pointRadius) {
			selectedPoint = i;
			lastSelectedPoint = i;
			break;
			
		}
	}
	if(selectedPoint == points.end()) {
		selectedPoint = points.insert(lastSelectedPoint,
			to_complex_number(wxPoint(_event.GetX(), _event.GetY()), GetSize()));
		lastSelectedPoint = selectedPoint;
		
	}
	Refresh();
	
}

void editPanel::on_left_up([[maybe_unused]]wxMouseEvent &_event) {
	selectedPoint = points.end();
	
}

void editPanel::on_motion(wxMouseEvent &_event) {
	if(selectedPoint != points.end()) {
		*selectedPoint = to_complex_number(wxPoint(_event.GetX(), _event.GetY()), GetSize());
		Refresh();
		
	}
}

void editPanel::on_right_up(wxMouseEvent &_event) {
	for(auto i = points.begin(); i != points.end(); i++) {
		auto _relPos = to_screen_coord(*i, GetSize()) - wxPoint(_event.GetX(), _event.GetY());
		int _dist = (_relPos .x * _relPos.x) + (_relPos.y * _relPos.y);
		if(_dist <= pointRadius * pointRadius) {
			lastSelectedPoint = points.erase(i);
			selectedPoint = points.end();
			if(points.empty()) lastSelectedPoint = points.end();
			break;
			
		}
	}
	Refresh();
	
}

void editPanel::on_paint([[maybe_unused]]wxPaintEvent &_event) {
	wxAutoBufferedPaintDC _dc(this);
	_dc.Clear();
	
	render(_dc);
	
}

void editPanel::on_resize([[maybe_unused]]wxSizeEvent &_event) {
	Refresh();
	
}

const std::vector<std::complex<float>>& editPanel::get_points() const {
	return points;
}

bool editPanel::read_points(const std::string &_fileName) {
	std::ifstream _file(_fileName);
	bool _success = true;
	
	points.clear();
	
	if(_file.is_open()) {
		while(!_file.eof()) {
			std::complex<float> _temp;
			_file >> _temp;
			if(_file.eof()) break;
			points.push_back(_temp);
			
		}
		
		_file.close();
	}
	else _success = false;
	lastSelectedPoint = points.begin();
	selectedPoint = points.end();
	
	savedPoints = points;
	
	Refresh();
	
	return _success;
}

bool editPanel::save_points(const std::string &_fileName) {
	std::ofstream _file(_fileName);
	bool _success = true;
	
	if(_file.is_open()) {
		for(auto i: points) {
			_file << i << std::endl;
			
		}
	}
	else _success = false;
	
	savedPoints = points;
	
	return _success;
}

bool editPanel::verify_wip() {
	return points != savedPoints;
}

void editPanel::render(wxDC &_dc) {
	const wxPoint _center = wxPoint(GetSize().GetWidth() / 2, GetSize().GetHeight() / 2);
	pointRadius = int(_dc.GetPPI().GetWidth() * 0.065);
	
	std::vector<wxPoint> _pointsOnScreen;
	_pointsOnScreen.reserve(points.size());
	
	_dc.SetBrush(*wxTRANSPARENT_BRUSH);
	
	for(auto i: points) {
		_pointsOnScreen.push_back(to_screen_coord(i, _center));
		
	}
	
	_dc.SetPen(wxPen(TRACECOLOUR));
	
	_dc.DrawLines(_pointsOnScreen.size(), _pointsOnScreen.data());
	
	_dc.SetPen(wxPen(POINTSCOLOUR));
	
	for(auto i: points) {
		auto _currentPoint = to_screen_coord(i, _center);
		_dc.DrawCircle(_currentPoint, pointRadius);
		
	}
	
	if(lastSelectedPoint != points.end()) {
		_dc.SetPen(wxPen(SELECTEDCOLOUR));
		_dc.DrawCircle(to_screen_coord(*lastSelectedPoint, _center), pointRadius);
		
	}
	
}

wxBEGIN_EVENT_TABLE(editFrame, wxFrame)

	// Menu bar events
	EVT_MENU(XRCID("EditWindow_File_Open"), editFrame::on_open_file)
	EVT_MENU(XRCID("EditWindow_File_OpenImage"), editFrame::on_open_image)
	EVT_MENU(XRCID("EditWindow_File_Save"), editFrame::on_save_file)
	EVT_MENU(XRCID("EditWindow_File_SaveAs"), editFrame::on_save_as_file)
	EVT_MENU(XRCID("EditWindow_File_Quit"), editFrame::on_exit)

	EVT_MENU(XRCID("EditWindow_Edit_Draw"), editFrame::on_draw)
	EVT_MENU(XRCID("EditWindow_Edit_Clear"), editFrame::on_clear)
	
	EVT_MENU(XRCID("EditWindow_Info_About"), editFrame::on_about)

wxEND_EVENT_TABLE()

editFrame::editFrame() {
	wxXmlResource::Get() -> LoadFrame(this, nullptr, "EditWindow");
	
	panel = new editPanel(this, GetSize());
	
	Show(true);
	
}

void editFrame::on_open_file(wxCommandEvent &_event) {
	while(panel -> verify_wip()) {
		bool _saveFile = wxMessageBox(_("The current points will be lost. Do you wish to save?"),
			_("Please confirm"),
			wxICON_QUESTION | wxYES_NO, this) == wxYES;
		if(_saveFile) {
			on_save_file(_event);
			if(panel -> verify_wip()) break;
			
		}
		else break;
	}
	
	wxFileDialog openDialog(this,
		_("Open points file"),
		"",
		"",
		_("Point file (*.dat;*.txt)|*.dat;*.txt"),
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	
	if(openDialog.ShowModal() != wxID_CANCEL) {
		if(panel -> read_points(std::string(openDialog.GetPath()))) currentFile = openDialog.GetPath();
		else wxLogError(_("Cannot open file '%s'."), openDialog.GetPath());
		
	}
	
	_event.Skip();
	
}

void editFrame::on_open_image(wxCommandEvent &_event) {
	while(panel -> verify_wip()) {
		bool _saveFile = wxMessageBox(_("The current points will be lost. Do you wish to save?"),
			_("Please confirm"),
			wxICON_QUESTION | wxYES_NO, this) == wxYES;
		if(_saveFile) {
			on_save_file(_event);
			if(panel -> verify_wip()) break;
			
		}
		else break;
	}
	
	wxFileDialog openDialog(this,
		_("Open points file"),
		"",
		"",
		_("Image files (*.png;*.bmp;*.jpg;*.jpeg)|*.png;*.bmp;*.jpg;*.jpeg"),
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	
	if(openDialog.ShowModal() != wxID_CANCEL) {
		if(true) {
			wxImage test;
			wxInitAllImageHandlers();
			if(test.LoadFile(openDialog.GetPath())) {
				new DIPFrame(test, this);
				
			}
		}
		else wxLogError(_("Cannot open file '%s'."), openDialog.GetPath());
		
	}
	
	_event.Skip();
	
}

void editFrame::on_save_file(wxCommandEvent &_event) {
	if(currentFile.empty()) {
		wxFileDialog openDialog(this,
			_("Save points file"),
			"",
			"",
			_("Point file (*.dat)|*.dat"),
			wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
		
		if(openDialog.ShowModal() != wxID_CANCEL) currentFile = openDialog.GetPath();
	}
	
	if(!panel -> save_points(currentFile)) {
		currentFile = "";
		
	}
	
	_event.Skip();
}

void editFrame::on_save_as_file(wxCommandEvent &_event) {
	std::string _tempFilename = currentFile;
	
	currentFile = "";
	
	on_save_file(_event);
	
	if(currentFile.empty()) currentFile = _tempFilename;
	
	_event.Skip();
}

void editFrame::on_exit(wxCommandEvent &_event) {
	Close(true);
	
	_event.Skip();
}

void editFrame::on_clear(wxCommandEvent &_event) {
	panel -> clear_points();
	
	_event.Skip();
}

void editFrame::on_draw(wxCommandEvent &_event) {
	new drawFrame(panel -> get_points(), this, GetSize(), IsMaximized());
	
	_event.Skip();
	
}

void editFrame::on_about(wxCommandEvent &_event) {
	constexpr wchar_t LICENSE[] = 
	L"MIT License\n"
	L"\n"
	L"Copyright (c) 2020 Gustavo Pacola Gonçalves\n"
	L"\n"
	L"Permission is hereby granted, free of charge, to any person obtaining a copy\n"
	L"of this software and associated documentation files (the \"Software\"), to deal\n"
	L"in the Software without restriction, including without limitation the rights\n"
	L"to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n"
	L"copies of the Software, and to permit persons to whom the Software is\n"
	L"furnished to do so, subject to the following conditions:\n"
	L"\n"
	L"The above copyright notice and this permission notice shall be included in all\n"
	L"copies or substantial portions of the Software.\n"
	L"\n"
	L"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
	L"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
	L"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n"
	L"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n"
	L"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n"
	L"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n"
	L"SOFTWARE.\n";
	wxAboutDialogInfo _aboutInfo;
	
	_aboutInfo.SetName("Fourier drawing");
	_aboutInfo.SetDescription("A program that draws curves using the Fourier series");
	_aboutInfo.SetLicense(LICENSE);
	
	wxAboutBox(_aboutInfo);
	
	_event.Skip();
	
}