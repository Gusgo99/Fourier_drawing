#include <algorithm>
#include <exception>
#include <iostream>
#include <cmath>
#include <chrono>
#include <fstream>

#include <SDL2/SDL.h>

#include "display.hpp"

class bad_initialization : std::runtime_error {
	public:
		bad_initialization(const char* _str = "Failed to initialize module\n") : runtime_error(_str) {};
};

class bad_SDLInit : bad_initialization {
	public:
		bad_SDLInit() : bad_initialization("Failed to initialize SDL\n") {};
};

class bad_windowCreate : bad_initialization {
	public:
		bad_windowCreate() : bad_initialization("Failed to create SDL window\n") {};
};

class bad_surfaceCreate : bad_initialization {
	public:
		bad_surfaceCreate() : bad_initialization("Failed to create SDL surface") {};
};

void c_window::initialize_SDL() {
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		throw bad_SDLInit();
		
	}
	
	return;
}

void c_window::finalize_SDL() {
	SDL_Quit();
	
	return;
}

size_t c_window::position(const unsigned _x, const unsigned _y) const {
	size_t _pos = _x + _y * width;
	
	if((_x >= width) || (_y >= height)) _pos = 0;
	
	return _pos;
}

c_window::c_window(const std::string &_title, float _w, float _h) {
	initialize_SDL();
	
	create_window_and_surface(_title, _w, _h);
	
	init_timers();
	init_screens();
	
	return;
}

c_window::~c_window() {
	SDL_DestroyWindow(window);
	
	finalize_SDL();
	
	return;
}

void c_window::create_window_and_surface(const std::string &_title, float _w, float _h) {
	isOpen = true;
	
	SDL_DisplayMode _screen;
	SDL_GetCurrentDisplayMode(0, &_screen);
	width = _screen.w * _w;
	height = _screen.h * _h;
	windowTitle = _title;
	
	window = SDL_CreateWindow(	_title.c_str(),
								SDL_WINDOWPOS_UNDEFINED,
								SDL_WINDOWPOS_UNDEFINED,
								width,
								height,
								SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	
	if(window == nullptr) throw bad_windowCreate();
	
	windowSurface = SDL_GetWindowSurface(window);
	
	if(windowSurface == nullptr) throw bad_surfaceCreate();
	
	return;
}

void c_window::init_timers() {
	tStart = std::chrono::high_resolution_clock::now();
	tRefresh = tStart;
	tFrame = tRefresh;
	
	return;
}

void c_window::init_screens() {
	screens.push_back(std::unique_ptr<c_view>(new c_editMode(width, height)));
	screens.push_back(std::unique_ptr<c_view>(new c_drawMode(width, height)));
	
	currentScreen = screens.begin();
	
	return;
}

void c_window::main_loop() {
	while(isOpen) {
		event_handler();
		
		(*currentScreen) -> render_image(elapsed_time());
		
		(*currentScreen) -> draw_to_buffer(windowSurface -> pixels);
		SDL_UpdateWindowSurface(window);
		
		if((*currentScreen) -> should_screen_change()) {
			currentScreen++;
			if(currentScreen == screens.end()) currentScreen = screens.begin();
			(*currentScreen) -> get_active();
			
		}
		
		fps_limiter((*currentScreen) -> get_max_framerate());
		
		fps_counter();
		
	}
	
	return;
}

void c_window::event_handler() {
	SDL_Event _event;
	
	while(SDL_PollEvent(&_event) != 0) {
		switch(_event.type) {
			case SDL_QUIT:
				isOpen = false;
				break;
				
			case SDL_WINDOWEVENT:
				window_event_handler(_event.window);
				break;
				
			default:
				(*(*currentScreen))(_event);
			
		}
	}
	
	return;
}

void c_window::window_event_handler(const SDL_WindowEvent &_event) {
	switch(_event.event) {
		case SDL_WINDOWEVENT_RESIZED:
			resize(_event.data1, _event.data2);
			break;
		
	}
	
	return;
}

void c_window::resize(const int _width, const int _height) {
	SDL_SetWindowSize(window, _width, _height);
	width = _width;
	height = _height;
	SDL_FreeSurface(windowSurface);
	windowSurface = SDL_GetWindowSurface(window);
	if(windowSurface == nullptr) throw bad_surfaceCreate();
	for(auto &i: screens) i -> resize(width, height);
	
	return;
}

float c_window::elapsed_time() const {
	return std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - tStart).count();
}

void c_window::fps_limiter(unsigned _max) {
	tRefresh += std::chrono::duration<int, std::ratio<1, 10000>>(10000 / _max);
	std::this_thread::sleep_until(tRefresh);
	
	return;
}

void c_window::fps_counter() {
	if(std::chrono::high_resolution_clock::now() > tFrame) {
		tRefresh = std::chrono::high_resolution_clock::now();
		tFrame += std::chrono::duration<int, std::ratio<1, 1>>(1);
		char title[50];
		sprintf(title, "%s - %i FPS", windowTitle.c_str(), frame);
		SDL_SetWindowTitle(window, title);
		frame = 0;
		
	}
	frame++;
	
	return;
}
