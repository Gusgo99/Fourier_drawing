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
#include "DIP_strategy_utilities.hpp"

#include "DIP_threshold_strategy.hpp"
#include "DIP_selection_strategy.hpp"
#include "DIP_skeletonization_strategy.hpp"
#include "DIP_path_generation_strategy.hpp"

namespace DIP {
    std::unique_ptr<strategy> create_strategy(const strategies &_selected) {
        std::unique_ptr<strategy> _newStrategy;

        switch(_selected) {
            case strategies::THRESHOLD:
                _newStrategy = std::make_unique<thresholdStrategy>();
                break;

            case strategies::SELECTION:
                _newStrategy = std::make_unique<selectionStrategy>();
                break;

            case strategies::SKELETONIZATION:
                _newStrategy = std::make_unique<skeletonizationStrategy>();
                break;

            case strategies::PATHGENERATION:
                _newStrategy = std::make_unique<pathGenerationStrategy>();
                break;

            default:
                wxASSERT(false);
                break;
        }

        return _newStrategy;
    }

}