#ifndef SCREEN_VIEW_HPP
#define SCREEN_VIEW_HPP

#include <vector>
#include <memory>

#include <SDL2/SDL.h>

#include <functional>
#include <map>
#include <memory>
#include <queue>

#include "complex.hpp"
#include "image.hpp"

class c_view {
	public:
		c_view(int _width, int _heights);
		virtual ~c_view();
		const c_image& get_image() const;
		void draw_to_buffer(void* _buffer);
		virtual void resize(int _width, int _height);
		bool should_screen_change();
		
		virtual void render_image(const double &_time) = 0;
		virtual void operator()(const SDL_Event &_event) = 0;
		virtual unsigned get_max_framerate() = 0;
		virtual uint32_t get_id() = 0;
		virtual void get_active() = 0;
	
	protected:
		c_image screen;
		
		// Temporary solution -> will be changed in the furure:
		bool screenChange = false;
		static std::vector<c_complex> *tempPoints;
		
		
};

#endif