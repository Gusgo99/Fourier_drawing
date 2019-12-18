#ifndef RESIZE_REQUEST_HPP
#define RESIZE_REQUEST_HPP

#include "request.hpp"

class c_resizeRequest : public c_request {
	public:
		c_resizeRequest(int _width, int _height);
		~c_resizeRequest();
		int get_width();
		int get_height();
		
	private:
		int width;
		int height;
		
};

#endif