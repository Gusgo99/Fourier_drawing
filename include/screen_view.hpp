#ifndef SCREEN_VIEW_HPP
#define SCREEN_VIEW_HPP

#include <queue>
#include <memory>

#include <SDL2/SDL.h>

#include "image.hpp"
#include "request.hpp"
#include "screen_change_request.hpp"

class c_view {
	public:
		c_view(int _width, int _heights);
		virtual ~c_view();
		const c_image& get_image() const;
		void draw_to_surface(SDL_Surface *_surface);
		std::shared_ptr<c_request> get_requests();
		virtual void resize(int _width, int _height);
		
		virtual void render_image(const double &_time) = 0;
		virtual void operator()(const SDL_Event &_event) = 0;
		virtual void activate(const c_screenChangeRequest *_request) = 0;
		
		virtual unsigned get_max_framerate() = 0;
		virtual uint32_t get_id() = 0;
	
	protected:
		c_image screen;
		std::queue<std::shared_ptr<c_request>> requests;
		
		
};

#endif