#ifndef SKELETONIZATION_TOOL_HPP
#define SKELETONIZATION_TOOL_HPP

#include "modifying_image_processing_tool.hpp"

#include <vector>

class c_skeletonizationTool : public c_modifyingImageProcessingTool {
	public:
		c_skeletonizationTool(c_image &_image);
		~c_skeletonizationTool();
		
		void apply();
		void increase_effect();
		void decrease_effect();
		void reset_effect();
		void affect_pixel(const int &_x, const int &_y);
		void set_background_color(const s_color &_foregroundColor);
		s_color get_background_color();
	
	private:
		s_color backgroundColor = WHITE;
	
		std::vector<bool> get_contour();
		bool contour_removal(std::vector<bool> _contour);
		bool is_removable(int _neighbourhood, int _neighbourCount);
		
	
};

#endif