#ifndef IMAGE_PROCESSING_TOOL_HPP
#define IMAGE_PROCESSING_TOOL_HPP

#include "image.hpp"

class c_imageProcessingTool {
	public:
		c_imageProcessingTool(c_image &image);
		virtual ~c_imageProcessingTool();
		
	protected:
		c_image &image;
};

#endif