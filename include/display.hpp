#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <SDL2/SDL.h>

#include <chrono>
#include <queue>
#include <stack>
#include <thread>
#include <vector>

#include "complex.hpp"
#include "draw_mode.hpp"
#include "edit_mode.hpp"
#include "fourier.hpp"
#include "image.hpp"

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

#endif