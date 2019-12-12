#ifndef DRAW_MODE_HPP
#define DRAW_MODE_HPP

#include "complex.hpp"
#include "edit_mode.hpp"
#include "fourier.hpp"
#include "screen_view.hpp"

class c_drawMode;

typedef std::function<void(c_drawMode*, const SDL_KeyboardEvent&)> t_drawModeKeyHandler;
typedef std::function<void(c_drawMode*, const SDL_MouseButtonEvent&)> t_drawModeMouseButtonHandler;

class c_drawMode : public c_view {
	public:
		c_drawMode(int _width, int _height);
		~c_drawMode();
		
		void operator()(const SDL_Event &_event);
		void render_image(const double &_time);
		unsigned get_max_framerate() {return 60;}
		uint32_t get_id() {return 2;}
		void get_active();
		
		void resize(int _width, int _height);
	
	private:
		const s_color BACKGROUNDCOLOR = WHITE;
		const s_color CIRCLECOLOR = BLUE;
		const s_color TRACECOLOR = GREEN;
		const s_color POINTCOLOR = BLACK;
		
		bool drawCircles;
		bool drawPoints;
		float speed;
		c_fourier fourier;
		c_image auxScreen;
		std::vector<c_complex> points;
	
		c_complex draw_series_truncation(float _time, float _speed);
		
		void key_handler_p(const SDL_KeyboardEvent &_event);
		void key_handler_s(const SDL_KeyboardEvent &_event);
		void key_handler_c(const SDL_KeyboardEvent &_event);
		void key_handler_m(const SDL_KeyboardEvent &_event);
		void key_handler_arrow_up(const SDL_KeyboardEvent &_event);
		void key_handler_arrow_down(const SDL_KeyboardEvent &_event);
	
		static const std::map<SDL_Keycode, t_drawModeKeyHandler> KEYHANDLERS;
		static const std::map<uint8_t, t_drawModeMouseButtonHandler> MOUSEBUTTONHANDLERS;
	
};

#endif