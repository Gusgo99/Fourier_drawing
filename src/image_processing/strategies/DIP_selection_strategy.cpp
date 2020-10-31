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
#include "DIP_selection_strategy.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#include <wx/wx.h>
#include <wx/rawbmp.h>
#pragma GCC diagnostic pop

namespace DIP {
	void selectionStrategy::apply(wxBitmap &_bitmap) {
        if(source != wxDefaultPosition) {
            grid _image = generate_grid(
                _bitmap, 
                {{BACKGROUNDCOLOUR, grid::NOTHING}, {FOREGROUNDCOLOUR, grid::EDGE}}
            );

            _image.remove_unconnected_cells({source.x, source.y});

            draw_grid_to_bitmap(
                _bitmap,
                _image,
                {{grid::NOTHING, BACKGROUNDCOLOUR}, {grid::EDGE, FOREGROUNDCOLOUR}}
            );
        }
    }

    bool selectionStrategy::uses_intensity() const {
        return false;
    }

    bool selectionStrategy::uses_source() const {
        return true;
    }

    bool selectionStrategy::generates_info() const {
        return false;
    }
}