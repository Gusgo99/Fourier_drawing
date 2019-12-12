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
		size_t position(const unsigned _x, const unsigned _y) const;
		void main_loop();
	
	private:
		static void initialize_SDL();
		static void finalize_SDL();
		
		void create_window_and_surface(const std::string &_title, float _w, float _h);
		void init_timers();
		void init_screens();
		void event_handler();
		void window_event_handler(const SDL_WindowEvent &_event);
		void fps_limiter(unsigned _max);
		void fps_counter();
		void resize(const int _width, const int _height);
		float elapsed_time() const;
		c_complex draw_series_truncation(std::vector<uint32_t> &_frame, float _speed) const;
		
		const size_t FBSIZE = 3;
		bool isOpen;
		float speed;
		unsigned width;
		unsigned height;
		std::string windowTitle;
		std::vector<std::unique_ptr<c_view>> screens;
		std::vector<std::unique_ptr<c_view>>::iterator currentScreen;
		
		SDL_Window *window;
		SDL_Surface *windowSurface;
		
		std::chrono::high_resolution_clock::time_point tStart;
		std::chrono::high_resolution_clock::time_point tRefresh;
		std::chrono::high_resolution_clock::time_point tFrame;
		unsigned frame;
	
};

#endif