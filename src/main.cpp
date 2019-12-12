#include <iostream>

#include "display.hpp"

int main(int argc, char **argv) {
	for(int i = 0; i != argc; i++) {
		std::cout << argv[i] << std::endl;
		
	}
	
	std::unique_ptr<c_window> _window(new c_window("Fourier image", 0.75, 0.75));
	
	try {
		_window -> main_loop();
		
	}
	catch(std::exception &_e) {
		std::cout << _e.what() << std::endl;
		
	}
	
	return 0;
}
