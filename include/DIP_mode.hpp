#ifndef DIP_MODE_HPP
#define DIP_MODE_HPP

#include <functional>
#include <map>

#include "screen_view.hpp"

#include "image_processing_tool.hpp"

class c_DIPMode;

typedef std::function<void(c_DIPMode*, const SDL_KeyboardEvent&)> t_DIPModeKeyHandler;
typedef std::function<void(c_DIPMode*, const SDL_MouseButtonEvent&)> t_DIPModeMouseButtonHandler;
typedef std::function<void(c_DIPMode*, const SDL_DropEvent&)> t_DIPModeDropEventHandler;

class c_DIPMode : public c_view {
	public:
		c_DIPMode(int _width, int _height);
		~c_DIPMode();
		
		void render_image(const double &_time);
		void operator()(const SDL_Event &_event);
		void activate(const c_screenChangeRequest *_request);
		
		unsigned get_max_framerate() {return 0;};
		uint32_t get_id() {return 3;};
	
	private:
		c_image baseImage;
		
		std::vector<std::unique_ptr<c_imageProcessingTool>> processingTools;
		std::vector<std::unique_ptr<c_imageProcessingTool>>::iterator currentTool;
		
		void key_handler_c(const SDL_KeyboardEvent &_event);
		void key_handler_F1(const SDL_KeyboardEvent &_event);
		void key_handler_arrow_up(const SDL_KeyboardEvent &_event);
		void key_handler_arrow_down(const SDL_KeyboardEvent &_event);
		void key_handler_arrow_left(const SDL_KeyboardEvent &_event);
		void key_handler_arrow_right(const SDL_KeyboardEvent &_event);
		void key_handler_escape(const SDL_KeyboardEvent &_event);
		
		void mouse_button_handler_left(const SDL_MouseButtonEvent &_event);
		
		void drop_file_handler(const SDL_DropEvent &_event);
	
		static const std::map<SDL_Keycode, t_DIPModeKeyHandler> KEYHANDLERS;
		static const std::map<uint8_t, t_DIPModeMouseButtonHandler> MOUSEBUTTONHANDLERS;
		static const std::map<uint32_t, t_DIPModeDropEventHandler> DROPHANDLERS;
	
};

#endif