#ifndef THRESHOLD_TOOL_HPP
#define THRESHOLD_TOOL_HPP

#include "modifying_image_processing_tool.hpp"

class c_thresholdTool : public c_modifyingImageProcessingTool {
	public:
		c_thresholdTool(c_image &_image);
		~c_thresholdTool();
		
		void apply();
		void increase_effect();
		void decrease_effect();
		void reset_effect();
		void affect_pixel(const int &_x, const int &_y);
	
	private:
		double threshold;
	
};

#endif