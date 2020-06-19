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
#include "DIP_window.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#include <wx/listbox.h>
#include <wx/tglbtn.h>
#pragma GCC diagnostic pop

#include <algorithm>

wxBEGIN_EVENT_TABLE(DIPPanel, wxPanel)
	
	// Mouse event
	EVT_LEFT_DOWN(DIPPanel::on_left_down)
	
	// Rendering event
	EVT_PAINT(DIPPanel::on_paint)
	
wxEND_EVENT_TABLE()

DIPPanel::DIPPanel(const wxImage &_image, wxWindow *_parent):
wxPanel(_parent, ID::DIPW::PANEL),
origBitmap(_image) {
	tools.reserve(3);
	SetSize(origBitmap.GetSize());
	
	SetBackgroundStyle(wxBG_STYLE_PAINT);
	
	selectedTool = -1;
	
	showPoints = false;
	
}

void DIPPanel::on_paint([[maybe_unused]]wxPaintEvent &_event) {
	wxAutoBufferedPaintDC _dc(this);
	PrepareDC(_dc);
	wxBitmap _processedBitmap = origBitmap.GetSubBitmap(
		wxRect(0, 0, origBitmap.GetWidth(), origBitmap.GetHeight()));
	
	for(auto &i: tools) i.apply(_processedBitmap);
	
	if(_dc.CanDrawBitmap()) _dc.DrawBitmap(_processedBitmap, 0, 0);
	
	if(selectedTool >= 0) {
		wxASSERT(selectedTool < int(tools.size()));
		if(tools[selectedTool].uses_source()) render(_dc);
		
	}
}

void DIPPanel::on_left_down(wxMouseEvent &_event) {
	if(selectedTool >= 0) {
		if(tools[selectedTool].uses_source()) {
			tools[selectedTool].source = wxPoint(_event.GetX(), _event.GetY());
			Refresh();
			
		}
	}
}

void DIPPanel::render(wxDC &_dc) {
	_dc.SetBrush(*wxTRANSPARENT_BRUSH);
	int _pointRadius = int(_dc.GetPPI().GetWidth() * 0.065);
	_dc.SetPen(wxPen(wxColour(255, 0, 0)));
	
	wxPoint _selectedPoint = tools[selectedTool].source;
	
	if(_selectedPoint != wxDefaultPosition) {
		_dc.DrawCircle(_selectedPoint, _pointRadius);
		_dc.DrawPoint(_selectedPoint);
		
	}
}

void DIPPanel::add_tool(const DIPTool::type _toolType) {
	if(selectedTool >= 0) {
		tools.insert(tools.begin() + selectedTool + 1, DIPTool(_toolType));
		selectedTool++;
		
	}
	else {
		tools.push_back(DIPTool(_toolType));
		selectedTool = 0;
		
	}
	Refresh();
	
}

void DIPPanel::remove_tool() {
	tools.erase(tools.begin() + selectedTool);
	if(size_t(selectedTool) >= tools.size()) selectedTool--;
	Refresh();
	
}

void DIPPanel::clear_tools() {
	tools.clear();
	selectedTool = -1;
	Refresh();
	
}

void DIPPanel::select_tool(const int _selectedTool) {
	if(tools.size() != 0) selectedTool = _selectedTool % tools.size();
	else selectedTool = -1;
	Refresh();
	
}

void DIPPanel::set_tool_intensity(const double _intensity) {
	if(selectedTool >= 0) {
		tools[selectedTool].intensity = std::max(0.0, std::min(1.0, _intensity));
		Refresh();
		
	}
}

double DIPPanel::get_tool_intensity() {
	double _intensity = 0.5;
	
	if(selectedTool >= 0) _intensity = tools[selectedTool].intensity;
	
	return _intensity;
}

void DIPPanel::set_points_state(const bool _showPoints) {
	showPoints = _showPoints;
	Refresh();
	
}

bool DIPPanel::should_show_slider() {
	bool _show = false;
	
	if(selectedTool >= 0) _show = tools[selectedTool].uses_intensity();
	
	return _show;
}

wxBEGIN_EVENT_TABLE(DIPFrame, wxFrame)
	
	// Tool bar
	EVT_BUTTON(ID::DIPW::TB_CLEAR, DIPFrame::on_clear)
	EVT_BUTTON(ID::DIPW::TB_REMOVE, DIPFrame::on_remove)
	EVT_BUTTON(ID::DIPW::TB_THRESHOLD, DIPFrame::on_add_threshold)
	EVT_BUTTON(ID::DIPW::TB_SELECTION, DIPFrame::on_add_selection)
	EVT_BUTTON(ID::DIPW::TB_SKELETONIZATION, DIPFrame::on_add_skeletonization)
	EVT_TOGGLEBUTTON(ID::DIPW::TB_SHOWPOINTS, DIPFrame::on_show_points)
	EVT_BUTTON(ID::DIPW::TB_GETPOINTS, DIPFrame::on_get_points)
	
	// List box
	EVT_LISTBOX(ID::DIPW::LISTBOX, DIPFrame::on_list_select)
	
	// Status bar
	EVT_SLIDER(ID::DIPW::SB_SLIDER, DIPFrame::on_slider_scroll)
	
wxEND_EVENT_TABLE()

DIPFrame::DIPFrame(const wxImage &_image, wxWindow *_parent):
wxFrame(_parent, ID::DIPW::FRAME, _("Image processing"), wxDefaultPosition, wxDefaultSize,
	wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN),
panel(new DIPPanel(_image, this)) {
	wxSize _size;
	
	// Toolbar
	CreateToolBar();
	
	wxButton *_button;
	
	GetToolBar() -> AddControl(_button = new wxButton(GetToolBar(), ID::DIPW::TB_CLEAR, "Clear"));
	GetToolBar() -> AddControl(new wxButton(GetToolBar(), ID::DIPW::TB_REMOVE, "Remove"));
	GetToolBar() -> AddControl(new wxButton(GetToolBar(), ID::DIPW::TB_THRESHOLD, "Add threshold"));
	GetToolBar() -> AddControl(new wxButton(GetToolBar(), ID::DIPW::TB_SELECTION, "Add selection"));
	GetToolBar() -> AddControl(new wxButton(GetToolBar(),
		ID::DIPW::TB_SKELETONIZATION,
		"Add skeletonization"));
	GetToolBar() -> AddControl(new wxToggleButton(GetToolBar(), ID::DIPW::TB_SHOWPOINTS, "Show points"));
	GetToolBar() -> AddControl(new wxButton(GetToolBar(), ID::DIPW::TB_GETPOINTS, "Get points from image"));
	
	GetToolBar() -> Realize();
	
	// Increase the size of the toolbar to fit buttons
	GetToolBar() -> SetSize(GetToolBar() -> GetSize() + wxSize(0, _button -> GetPosition().y));
	
	_size = _image.GetSize();
	
	_size.SetWidth(std::max(100, GetToolBar() -> GetSize().GetWidth() - _size.GetWidth()));
	
	list = new wxListBox(this, ID::DIPW::LISTBOX, wxDefaultPosition, _size);
	
	// Configure status bar
	CreateStatusBar(2);
	_size = GetStatusBar() -> GetSize();
	slider = new wxSlider(GetStatusBar(),
		ID::DIPW::SB_SLIDER,
		SLIDERLIMIT / 2,
		0,
		SLIDERLIMIT,
		wxDefaultPosition,
		_size);
	slider -> Enable(false);
	
	// Configure sizer for the window
	SetSizer(new wxFlexGridSizer(1, 2, 0, 0));
	dynamic_cast<wxFlexGridSizer*>(GetSizer()) -> AddGrowableCol(1);
	dynamic_cast<wxFlexGridSizer*>(GetSizer()) -> AddGrowableRow(0);
	
	GetSizer() -> Add(list, 1, wxEXPAND | wxALL);
	GetSizer() -> Add(panel, 1, wxEXPAND | wxALL);
	
	_size = _image.GetSize();
	_size += wxSize(list -> GetSize().GetWidth(), 0);
	
	GetSizer() -> SetMinSize(_size);
	GetSizer() -> Fit(this);
	
	SetAutoLayout(true);
	Show(true);
	
}

void DIPFrame::on_clear([[maybe_unused]]wxCommandEvent &_event) {
	list -> Clear();
	panel -> clear_tools();
	
}

void DIPFrame::on_remove([[maybe_unused]]wxCommandEvent &_event) {
	if(list -> GetSelection() != wxNOT_FOUND) {
		int _removed = list -> GetSelection();
		list -> Delete(_removed);
		if(_removed >= int(list -> GetCount())) list -> SetSelection(list -> GetCount() - 1);
		else list -> SetSelection(_removed);
		panel -> remove_tool();
		refresh_tool_info();
		
	}
}

void DIPFrame::on_add_threshold([[maybe_unused]]wxCommandEvent &_event) {
	add_tool("Threshold", DIPTool::threshold);
	
}

void DIPFrame::on_add_selection([[maybe_unused]]wxCommandEvent &_event) {
	add_tool("Selection", DIPTool::selection);
	
}

void DIPFrame::on_add_skeletonization([[maybe_unused]]wxCommandEvent &_event) {
	add_tool("Skeletonization", DIPTool::skeletonization);
	
}

void DIPFrame::on_show_points([[maybe_unused]]wxCommandEvent &_event) {
	wxToggleButton *_button = dynamic_cast<wxToggleButton*>(_event.GetEventObject());
	if(_button != nullptr) panel -> set_points_state(_button -> GetValue());
	
}

void DIPFrame::on_get_points([[maybe_unused]]wxCommandEvent &_event) {
	std::cout << "Sending points to editWindow\n";
	Close();
	
}

void DIPFrame::on_list_select(wxCommandEvent &_event) {
	panel -> select_tool(_event.GetInt());
	refresh_tool_info();
	
}

void DIPFrame::on_slider_scroll(wxCommandEvent &_event) {
	panel -> set_tool_intensity(_event.GetInt() / double(SLIDERLIMIT));
	
}

void DIPFrame::add_tool(const std::string _toolname, const DIPTool::type _toolType) {
	if(list -> GetSelection() != wxNOT_FOUND) {
		list -> Insert(_(_toolname), list -> GetSelection() + 1);
		panel -> add_tool(_toolType);
		list -> SetSelection(list -> GetSelection() + 1);
		
	}
	else {
		list -> Append(_(_toolname));
		panel -> add_tool(_toolType);
		list -> SetSelection(list -> GetCount() - 1);
		
	}
	refresh_tool_info();
}

void DIPFrame::refresh_tool_info() {
	slider -> Enable(panel -> should_show_slider());
	panel -> select_tool(list -> GetSelection());
	slider -> SetValue(panel -> get_tool_intensity() * SLIDERLIMIT);
	
}