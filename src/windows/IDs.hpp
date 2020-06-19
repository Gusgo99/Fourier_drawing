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
#ifndef IDS_HPP
#define IDS_HPP
#pragma once

namespace ID {
	// Prefix for windows IDs
	enum {EDITWINDOW, DRAWWINDOW, DIPWINDOW};
	
	// Edit Window
	namespace EW {
		enum {
			FRAME = EDITWINDOW << 10,
			PANEL,
			CLOSE,
			// MB = Menu bar
			MB_OPEN,
			MB_OPENIMAGE,
			MB_SAVE,
			MB_SAVEAS,
			
			MB_CLEAR,
			MB_DRAW,
			MB_TOGGLE_POINTS
			
		};
	}
	
	// Draw window
	namespace DW {
		enum {
			FRAME = DRAWWINDOW << 10,
			PANEL,
			// TMR = Timer
			TMR_REFRESH,
			// TB = Tool bar
			TB_CLEAR,
			TB_HIDECIRCLES,
			TB_SHOWPOINTS
			
		};
	}
	
	// Image processing window
	namespace DIPW {
		enum {
			FRAME = DIPWINDOW << 10,
			PANEL,
			// TB = Tool bar
			TB_CLEAR,
			TB_REMOVE,	
			TB_THRESHOLD,
			TB_SELECTION,
			TB_SKELETONIZATION,
			TB_SHOWPOINTS,
			TB_GETPOINTS,
			// List box
			LISTBOX,
			// SB = Status bar
			SB_SLIDER
		};
	}
}

#endif