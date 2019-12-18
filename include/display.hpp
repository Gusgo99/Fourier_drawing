#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <SDL2/SDL.h>

#include <chrono>
#include <exception>
#include <queue>
#include <memory>
#include <thread>
#include <vector>

#include "draw_mode.hpp"
#include "edit_mode.hpp"
#include "image.hpp"
#include "request.hpp"

class c_window {
	public:
		c_window(const std::string &_title, float _w, float _h);
		~c_window();
		
		void main_loop();
	
	private:
		static void initialize_SDL();
		static void finalize_SDL();
		
		void create_window_and_surface(const std::string &_title, float _w, float _h);
		void init_timers();
		void init_screens();
		void event_handler();
		void window_event_handler(const SDL_WindowEvent &_event);
		void request_handler();
		void resize_handler(const int &_width, const int &_height);
		void resize(const int &_width, const int &_height);
		
		void fps_limiter(unsigned _max);
		void fps_counter();
		
		float elapsed_time() const;
		
		SDL_Window *window;
		SDL_Surface *windowSurface;
		bool isOpen;
		int width;
		int height;
		std::string windowTitle;
		
		std::vector<std::unique_ptr<c_view>> screens;
		std::vector<std::unique_ptr<c_view>>::iterator currentScreen;
		
		std::chrono::high_resolution_clock::time_point tStart;
		std::chrono::high_resolution_clock::time_point tRefresh;
		std::chrono::high_resolution_clock::time_point tFrame;
		unsigned frame;
	
};

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

#endif