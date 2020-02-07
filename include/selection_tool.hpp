#ifndef SELECTION_TOOL_HPP
#define SELECTION_TOOL_HPP

#include "modifying_image_processing_tool.hpp"

class c_selectionTool : public c_modifyingImageProcessingTool {
	public:
		c_selectionTool(c_image &_image);
		~c_selectionTool();
		
		void apply();
		void increase_effect();
		void decrease_effect();
		void reset_effect();
		void affect_pixel(const int &_x, const int &_y);
	
	private:
		const s_color SELECTEDCOLOR = BLACK;
		const s_color BACKGROUNDCOLOR = WHITE;
		int x;
		int y;
		
		std::vector<bool> get_selected_pixels();
	
};

#endif