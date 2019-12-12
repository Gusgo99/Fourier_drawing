#ifndef EDIT_MODE_HPP
#define EDIT_MODE_HPP

#include "complex.hpp"
#include "screen_view.hpp"

class c_editMode;

typedef std::function<void(c_editMode*, const SDL_KeyboardEvent&)> t_editModeKeyHandler;
typedef std::function<void(c_editMode*, const SDL_MouseButtonEvent&)> t_editModeMouseButtonHandler;

class c_editMode : public c_view {
	public:
		c_editMode(int _width, int _height);
		~c_editMode();
		
		void operator()(const SDL_Event &_event);
		void render_image(const double &_time);
		unsigned get_max_framerate() {return 30;}
		uint32_t get_id() {return 1;}
		void get_active();
		
	private:
		const std::string STANDARDPOINTSFILENAME = "resources/points.dat";
		const s_color BACKGROUNDCOLOR = WHITE;
		const s_color POINTSCOLOR = BLACK;
		const s_color TRACECOLOR = GREEN;
		const s_color SELECTEDCOLOR = RED;
		
		std::vector<c_complex> points;
		std::vector<c_complex>::iterator currentPoint;
		bool drawPoints;
		
		void add_point(const c_complex &_position);
		void remove_point(const c_complex &_position);
		void select_point(const c_complex &_position);
		
		void save_points(const std::string &_fileName);
		void read_points(const std::string &_fileName);
		
		void mouse_button_handler_left(const SDL_MouseButtonEvent &_event);
		void mouse_button_handler_right(const SDL_MouseButtonEvent &_event);
		void mouse_button_handler_middle(const SDL_MouseButtonEvent &_event);
		void key_handler_c(const SDL_KeyboardEvent &_event);
		void key_handler_p(const SDL_KeyboardEvent &_event);
		void key_handler_m(const SDL_KeyboardEvent &_event);
		
		static const std::map<SDL_Keycode, t_editModeKeyHandler> KEYHANDLERS;
		static const std::map<uint8_t, t_editModeMouseButtonHandler> MOUSEBUTTONHANDLERS;
	
};



#endif