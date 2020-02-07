#ifndef MODIFYING_IMAGE_PROCESSING_TOOL
#define MODIFYING_IMAGE_PROCESSING_TOOL

#include "image_processing_tool.hpp"

class c_modifyingImageProcessingTool : public c_imageProcessingTool {
	public:
		c_modifyingImageProcessingTool(c_image &_image);
		virtual ~c_modifyingImageProcessingTool();
		
		virtual void apply() = 0;
		virtual void increase_effect() = 0;
		virtual void decrease_effect() = 0;
		virtual void reset_effect() = 0;
		virtual void affect_pixel(const int &_x, const int &_y) = 0;
		
	protected:
		
	
};

#endif