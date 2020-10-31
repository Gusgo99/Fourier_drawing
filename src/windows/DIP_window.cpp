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
	EVT_RIGHT_DOWN(DIPPanel::on_right_down)
	
	// Rendering event
	EVT_PAINT(DIPPanel::on_paint)
	
wxEND_EVENT_TABLE()

DIPPanel::DIPPanel(const wxImage &_image, wxWindow *_parent, wxWindow *_pointsDestination):
	wxPanel{_parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE},
	selectedTool{INVALIDTOOL},
	origBitmap{_image},
	showPoints{false},
	pointsDestination{dynamic_cast<editFrame*>(_pointsDestination)} {

	tools.reserve(10);
	
	SetMinSize(origBitmap.GetSize());
	
	_parent -> GetSizer() -> Add(this, 0);
	
	SetBackgroundStyle(wxBG_STYLE_PAINT);

	selectedTool = INVALIDTOOL;
	
}

void DIPPanel::on_paint(wxPaintEvent&) {
	wxAutoBufferedPaintDC _dc(this);
	PrepareDC(_dc);

	paint_frame(_dc);

	handle_information_from_tools();
}

void DIPPanel::on_left_down(wxMouseEvent &_event) {
	if(selectedTool < tools.size()) {
		if(tools[selectedTool] -> uses_source()) {
			tools[selectedTool] -> set_source(_event.GetX(), _event.GetY());
			Refresh();
			
		}
	}
}

void DIPPanel::on_right_down(wxMouseEvent&) {
	if(selectedTool < tools.size()) {
		if(tools[selectedTool] -> uses_source()) {
			tools[selectedTool] -> set_source(wxDefaultPosition);
			Refresh();
			
		}
	}
}

void DIPPanel::render(wxDC &_dc) {
	_dc.SetBrush(*wxTRANSPARENT_BRUSH);
	int _pointRadius = int(_dc.GetPPI().GetWidth() * 0.065);
	_dc.SetPen(wxPen(wxColour(255, 0, 0)));
	
	wxPoint _selectedPoint = tools[selectedTool] -> get_source();
	
	if(_selectedPoint != wxDefaultPosition) {
		_dc.DrawCircle(_selectedPoint, _pointRadius);
		_dc.DrawPoint(_selectedPoint);
		
	}
}

void DIPPanel::add_tool(const DIP::strategies _toolType) {
	auto _addedStrategy = DIP::create_strategy(_toolType);
	tools.emplace(tools.begin() + ++selectedTool, std::move(_addedStrategy));
	Refresh();
	
}

void DIPPanel::remove_tool() {
	tools.erase(tools.begin() + selectedTool);
	if(size_t(selectedTool) >= tools.size()) selectedTool--;
	Refresh();
	
}

void DIPPanel::clear_tools() {
	tools.clear();
	selectedTool = INVALIDTOOL;
	Refresh();
	
}

void DIPPanel::select_tool(const int _selectedTool) {
	if(tools.size() != 0) selectedTool = _selectedTool % tools.size();
	else selectedTool = INVALIDTOOL;
	Refresh();
	
}

void DIPPanel::set_tool_intensity(const double _intensity) {
	if(selectedTool < tools.size()) {
		double _clampedIntensity = std::max(0.0, std::min(1.0, _intensity));
		tools[selectedTool] -> set_intensity(_clampedIntensity);
		Refresh();
		
	}
}

double DIPPanel::get_tool_intensity() {
	double _intensity = 0.5;
	
	if(selectedTool < tools.size()) _intensity = tools[selectedTool] -> get_intensity();
	
	return _intensity;
}

void DIPPanel::set_points_state(const bool _showPoints) {
	showPoints = _showPoints;
	Refresh();
	
}

bool DIPPanel::should_show_slider() {
	bool _show = false;
	
	if(selectedTool < tools.size()) _show = tools[selectedTool] -> uses_intensity();
	
	return _show;
}

void DIPPanel::paint_frame(wxDC &_dc) {
	// Copying a bitmap won't actually copy it, but instead increase the refcount.
	// This is done so there is an actual copy
	wxBitmap _processedBitmap = origBitmap.GetSubBitmap(
		wxRect(0, 0, origBitmap.GetWidth(), origBitmap.GetHeight())
	);
	
	apply_valid_tools_to_bitmap(_processedBitmap);
	
	if(_dc.CanDrawBitmap()) _dc.DrawBitmap(_processedBitmap, 0, 0);
	
	if(selectedTool < tools.size()) {
		if(tools[selectedTool] -> uses_source()) render(_dc);
		
	}
	
}

void DIPPanel::apply_valid_tools_to_bitmap(wxBitmap &_bitmap) {
	for(auto &i: tools) {
		if(i != nullptr) i -> apply(_bitmap);
	}
}

void DIPPanel::handle_information_from_tools() {
	if(!tools.empty()) {
		if(tools.back() -> generates_info()) {
			if(pointsDestination != nullptr) {
				std::any _information = tools.back() -> get_info();
				auto _points = std::any_cast<std::vector<std::complex<float>>>(_information);
				pointsDestination -> set_points(_points);
			}
		}
	}
}

wxBEGIN_EVENT_TABLE(DIPFrame, wxFrame)
	
	// Tool bar
	EVT_BUTTON(XRCID("DIPWindow_Clear"), DIPFrame::on_clear)
	EVT_BUTTON(XRCID("DIPWindow_RemoveTool"), DIPFrame::on_remove_tool)
	EVT_BUTTON(XRCID("DIPWindow_AddTool"), DIPFrame::on_add_tool)
	EVT_SLIDER(XRCID("DIPWindow_Intensity"), DIPFrame::on_slider_scroll)
	
	// List box
	EVT_LISTBOX(XRCID("DIPWindow_Splitter_Left_List"), DIPFrame::on_list_select)
	
wxEND_EVENT_TABLE()

DIPFrame::DIPFrame(const wxImage &_image, wxWindow *_parent) {
	wxXmlResource::Get() -> LoadFrame(this, _parent, "DIPWindow");
	
	panel = new DIPPanel(_image, XRCCTRL(*this, "DIPWindow_Splitter_Right", wxScrolledWindow), _parent);
	
	list = XRCCTRL(*this, "DIPWindow_Splitter_Left_List", wxListBox);
	
	slider = XRCCTRL(*this, "DIPWindow_Intensity", wxSlider);
	
	// Gets the maximum value the slider can reach based on data loaded from the xml resource file
	// Obs: It does assume that the minimum value is 0
	sliderLimit = slider -> GetMax();
	
	toolAdded = XRCCTRL(*this, "DIPWindow_ToolSelection", wxChoice);
	
	// Makes sure the size is enough to display the toolbar
	SetMinSize(wxSize(550, 100));
	
	Show(true);
	
}

void DIPFrame::on_clear(wxCommandEvent&) {
	list -> Clear();
	panel -> clear_tools();
	
}

void DIPFrame::on_remove_tool(wxCommandEvent&) {
	if(list -> GetSelection() != wxNOT_FOUND) {
		int _removed = list -> GetSelection();
		list -> Delete(_removed);
		if(_removed >= int(list -> GetCount())) list -> SetSelection(list -> GetCount() - 1);
		else list -> SetSelection(_removed);
		panel -> remove_tool();
		refresh_tool_info();
		
	}
}

void DIPFrame::on_add_tool(wxCommandEvent&) {
	int _selected = toolAdded -> GetSelection();
	add_tool(toolAdded -> GetString(_selected), _selected);
	
}

void DIPFrame::on_list_select(wxCommandEvent &_event) {
	panel -> select_tool(_event.GetInt());
	refresh_tool_info();
	
}

void DIPFrame::on_slider_scroll(wxCommandEvent &_event) {
	panel -> set_tool_intensity(_event.GetInt() / double(sliderLimit));
	
}

void DIPFrame::add_tool(const wxString _toolName, const int _toolType) {
	if(_toolType != wxNOT_FOUND) {
		list -> Insert(_toolName, list -> GetSelection() + 1);
		DIP::strategies _addedTool = map_selected_item_to_strategy(_toolType);
		panel -> add_tool(_addedTool);
		list -> SetSelection(list -> GetSelection() + 1);
		
	}
	else {
		list -> Append(_toolName);
		DIP::strategies _addedTool = map_selected_item_to_strategy(_toolType);
		panel -> add_tool(_addedTool);
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

DIP::strategies DIPFrame::map_selected_item_to_strategy(const int _selected) {
	constexpr std::array<DIP::strategies, 4> EQUIVALENCETABLE = {
		DIP::strategies::THRESHOLD,
		DIP::strategies::SELECTION,
		DIP::strategies::SKELETONIZATION,
		DIP::strategies::PATHGENERATION
	};

	return EQUIVALENCETABLE[_selected];
}