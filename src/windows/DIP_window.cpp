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
#include <wx/dcbuffer.h>
#include <wx/listbox.h>
#include <wx/tglbtn.h>
#include <wx/xrc/xmlres.h>
#include <wx/scrolwin.h>
#pragma GCC diagnostic pop

#include <algorithm>

wxBEGIN_EVENT_TABLE(DIPPanel, wxPanel)
	
	// Mouse event
	EVT_LEFT_DOWN(DIPPanel::on_left_down)
	
	// Rendering event
	EVT_PAINT(DIPPanel::on_paint)
	
wxEND_EVENT_TABLE()

DIPPanel::DIPPanel(const wxImage &_IMAGE, wxWindow *_parent):
wxPanel(_parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE),
origBitmap(_IMAGE) {
	tools.reserve(3);
	
	// Creates a sizer cause for some reason the scrolls won't appear without it
	_parent -> SetSizer(new wxBoxSizer(wxHORIZONTAL));
	
	SetMinSize(origBitmap.GetSize());
	
	_parent -> GetSizer() -> Add(this, 0);
	
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

void DIPPanel::add_tool(const DIPTool::type _TOOLTYPE) {
	if(selectedTool >= 0) {
		tools.insert(tools.begin() + selectedTool + 1, DIPTool(_TOOLTYPE));
		selectedTool++;
		
	}
	else {
		tools.push_back(DIPTool(_TOOLTYPE));
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

void DIPPanel::select_tool(const int _SELECTEDTOOL) {
	if(tools.size() != 0) selectedTool = _SELECTEDTOOL % tools.size();
	else selectedTool = -1;
	Refresh();
	
}

void DIPPanel::set_tool_intensity(const double _INTENSITY) {
	if(selectedTool >= 0) {
		tools[selectedTool].intensity = std::max(0.0, std::min(1.0, _INTENSITY));
		Refresh();
		
	}
}

double DIPPanel::get_tool_intensity() {
	double _intensity = 0.5;
	
	if(selectedTool >= 0) _intensity = tools[selectedTool].intensity;
	
	return _intensity;
}

void DIPPanel::set_points_state(const bool _SHOWPOINTS) {
	showPoints = _SHOWPOINTS;
	Refresh();
	
}

bool DIPPanel::should_show_slider() {
	bool _show = false;
	
	if(selectedTool >= 0) _show = tools[selectedTool].uses_intensity();
	
	return _show;
}

wxBEGIN_EVENT_TABLE(DIPFrame, wxFrame)
	
	// Tool bar
	EVT_BUTTON(XRCID("DIPWindow_Clear"), DIPFrame::on_clear)
	EVT_BUTTON(XRCID("DIPWindow_RemoveTool"), DIPFrame::on_remove_tool)
	EVT_BUTTON(XRCID("DIPWindow_AddTool"), DIPFrame::on_add_tool)
	EVT_SLIDER(XRCID("DIPWindow_Intensity"), DIPFrame::on_slider_scroll)
	
	// List box
	EVT_LISTBOX(XRCID("DIPWindow_Splitter_Left_List"), DIPFrame::on_list_select)
	
	// Status bar
	
	
wxEND_EVENT_TABLE()

DIPFrame::DIPFrame(const wxImage &_IMAGE, wxWindow *_parent) {
	wxXmlResource::Get() -> LoadFrame(this, _parent, "DIPWindow");
	
	panel = new DIPPanel(_IMAGE, XRCCTRL(*this, "DIPWindow_Splitter_Right", wxScrolledWindow));
	
	list = XRCCTRL(*this, "DIPWindow_Splitter_Left_List", wxListBox);
	
	slider = XRCCTRL(*this, "DIPWindow_Intensity", wxSlider);
	
	// Gets the maximum value the slider can reach based on data loaded from the xml resource file
	// Obs: It does assume that the minimum value is 0
	sliderLimit = slider -> GetMax();
	
	toolAdded = XRCCTRL(*this, "DIPWindow_ToolSelection", wxChoice);
	
	Show(true);
	
}

void DIPFrame::on_clear([[maybe_unused]]wxCommandEvent &_event) {
	list -> Clear();
	panel -> clear_tools();
	
}

void DIPFrame::on_remove_tool([[maybe_unused]]wxCommandEvent &_event) {
	if(list -> GetSelection() != wxNOT_FOUND) {
		int _removed = list -> GetSelection();
		list -> Delete(_removed);
		if(_removed >= int(list -> GetCount())) list -> SetSelection(list -> GetCount() - 1);
		else list -> SetSelection(_removed);
		panel -> remove_tool();
		refresh_tool_info();
		
	}
}

void DIPFrame::on_add_tool([[maybe_unused]]wxCommandEvent &_event) {
	int _selected = toolAdded -> GetSelection();
	add_tool(toolAdded -> GetString(_selected), DIPTool::type(_selected));
	
}

void DIPFrame::on_list_select(wxCommandEvent &_event) {
	panel -> select_tool(_event.GetInt());
	refresh_tool_info();
	
}

void DIPFrame::on_slider_scroll(wxCommandEvent &_event) {
	panel -> set_tool_intensity(_event.GetInt() / double(sliderLimit));
	
}

void DIPFrame::add_tool(const wxString _TOOLNAME, const DIPTool::type _TOOLTYPE) {
	if(list -> GetSelection() != wxNOT_FOUND) {
		list -> Insert(_TOOLNAME, list -> GetSelection() + 1);
		panel -> add_tool(_TOOLTYPE);
		list -> SetSelection(list -> GetSelection() + 1);
		
	}
	else {
		list -> Append(_TOOLNAME);
		panel -> add_tool(_TOOLTYPE);
		list -> SetSelection(list -> GetCount() - 1);
		
	}
	toolAdded -> SetSelection(std::min(toolAdded -> GetSelection() + 1, int(toolAdded -> GetCount()) - 1));
	refresh_tool_info();
}

void DIPFrame::refresh_tool_info() {
	slider -> Enable(panel -> should_show_slider());
	panel -> select_tool(list -> GetSelection());
	slider -> SetValue(panel -> get_tool_intensity() * sliderLimit);
	
}