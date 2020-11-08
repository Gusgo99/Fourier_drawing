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
#include "DIP_threshold_strategy.hpp"

#include "wxWidgets_headers.hpp"

namespace DIP {
	void thresholdStrategy::apply(wxBitmap &_bitmap) {
        const int _intIntensity = int(intensity * 3 * 255);
        
        for_each_pixel(
            _bitmap,
            [=](pixelData _pixel) {
                if((_pixel.Red() + _pixel.Green() + _pixel.Blue()) > _intIntensity) {
                    return strategy::BACKGROUNDCOLOUR;
                }
                else return strategy::FOREGROUNDCOLOUR;
            }
        );
    }

    bool thresholdStrategy::uses_intensity() const {
        return true;
    }

    bool thresholdStrategy::uses_source() const {
        return false;
    }

    bool thresholdStrategy::generates_info() const {
        return false;
    }
}