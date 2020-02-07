#include <thread>

#include "DIP_mode.hpp"
#include "modifying_image_processing_tool.hpp"
#include "resize_request.hpp"
#include "threshold_tool.hpp"

#include <iostream>

const std::string HELPMESSAGE = "You are in digital image processing mode.\n\n\
In this mode, you can use an image to generate a curve that will be used in the other modes.\n\
The following list presents the possible commands in this mode:\n\
 - Press the ESC key to return to edit mode\n\
 - Press the C key to clear the effect of the current processing stage\n\
 - Press the S key to touggle the spinning circles\n\
 - Press the arrow up key to increase the tool effect\n\
 - Press the arrow down key to decrease tool effect\n\
 - Left click on a pixel to apply the effect starting from that pixel\n\
 - Drop a file on the window to open the image\n";

const std::map<SDL_Keycode, t_DIPModeKeyHandler> c_DIPMode::KEYHANDLERS = {
	{SDLK_c, key_handler_c},
	{SDLK_F1, key_handler_F1},
	{SDLK_UP, key_handler_arrow_up},
	{SDLK_DOWN, key_handler_arrow_down},
	{SDLK_LEFT, key_handler_arrow_left},
	{SDLK_RIGHT, key_handler_arrow_right},
	{SDLK_ESCAPE, key_handler_escape}
};

const std::map<uint8_t, t_DIPModeMouseButtonHandler> c_DIPMode::MOUSEBUTTONHANDLERS = {
	{SDL_BUTTON_LEFT, mouse_button_handler_left}
};

const std::map<uint32_t, t_DIPModeDropEventHandler> c_DIPMode::DROPHANDLERS = {
	{SDL_DROPFILE, drop_file_handler}
};

c_DIPMode::c_DIPMode(int _width, int _height) : c_view(_width, _height) {
	processingTools.push_back(nullptr);
	processingTools.push_back(std::unique_ptr<c_imageProcessingTool>(new c_thresholdTool(screen)));
	
	currentTool = processingTools.begin();
	
	return;
}

c_DIPMode::~c_DIPMode() {
	
	return;
}

void c_DIPMode::render_image(const double &_time) {
	if(_time == 0.0f) {}
	
	if((baseImage.get_width() == screen.get_width()) && (baseImage.get_height() == screen.get_height())) {
		auto _appliedTool = processingTools.begin();
		screen = baseImage;
		
		while((_appliedTool != currentTool) && (_appliedTool != processingTools.end())) {
			auto _tool = dynamic_cast<c_modifyingImageProcessingTool*>(_appliedTool -> get());
			if(_tool != nullptr) _tool -> apply();
			_appliedTool++;
			
		}
		if(_appliedTool == currentTool) {
			auto _tool = dynamic_cast<c_modifyingImageProcessingTool*>(_appliedTool -> get());
			if(_tool != nullptr) _tool -> apply();
			
		}
	}
	
	return;
}

void c_DIPMode::operator()(const SDL_Event &_event) {
	switch(_event.type) {
		case SDL_KEYDOWN: [[fallthrough]]
		case SDL_KEYUP: {
			auto _keyHandler = KEYHANDLERS.find(_event.key.keysym.sym);
			if(_keyHandler != KEYHANDLERS.end()) {
				if(_keyHandler -> second != nullptr) _keyHandler -> second(this, _event.key);
				
			}
			break;
		}
		
		case SDL_MOUSEBUTTONDOWN: [[fallthrough]]
		case SDL_MOUSEBUTTONUP: {
			auto _mouseButtonHandler = MOUSEBUTTONHANDLERS.find(_event.button.button);
			if(_mouseButtonHandler != MOUSEBUTTONHANDLERS.end()) {
				if(_mouseButtonHandler -> second != nullptr) _mouseButtonHandler -> second(this, _event.button);
				
			}
			break;
		}
		
		case SDL_DROPFILE: [[fallthrough]]
		case SDL_DROPTEXT: [[fallthrough]]
		case SDL_DROPBEGIN: [[fallthrough]]
		case SDL_DROPCOMPLETE: {
			auto _dropHandler = DROPHANDLERS.find(_event.type);
			if(_dropHandler != DROPHANDLERS.end()) {
				if(_dropHandler -> second != nullptr) {
					_dropHandler -> second(this, _event.drop);
					
				}
			}
			break;
		}
		
	}
	
	return;
}

void c_DIPMode::activate(const c_screenChangeRequest *_request) {
	auto _changeRequest = dynamic_cast<const c_screenChangeRequestPKG<std::vector<c_complex>>*>(_request);
	if((_changeRequest != nullptr) && (_changeRequest -> get_new_id() == get_id())) {
		
	}
	
	return;
}

void c_DIPMode::key_handler_c(const SDL_KeyboardEvent &_event) {
	if(_event.type == SDL_KEYDOWN) {
		if(currentTool != processingTools.end()) {
			auto _tool = dynamic_cast<c_modifyingImageProcessingTool*>(currentTool -> get());
			if(_tool != nullptr) _tool -> reset_effect();
			
		}
	}
	
	return;
}

void c_DIPMode::key_handler_F1(const SDL_KeyboardEvent &_event) {
	if(_event.type == SDL_KEYDOWN) {
		std::thread(SDL_ShowSimpleMessageBox, SDL_MESSAGEBOX_INFORMATION, "Help - Digital image processing mode", HELPMESSAGE.c_str(), nullptr).detach();
		
	}
	
	return;
}

void c_DIPMode::key_handler_arrow_up(const SDL_KeyboardEvent &_event) {
	if(_event.type == SDL_KEYDOWN) {
		if(currentTool != processingTools.end()) {
			auto _tool = dynamic_cast<c_modifyingImageProcessingTool*>(currentTool -> get());
			if(_tool != nullptr) _tool -> increase_effect();
			
		}
	}
	
	return;
}

void c_DIPMode::key_handler_arrow_down(const SDL_KeyboardEvent &_event) {
	if(_event.type == SDL_KEYDOWN) {
		if(currentTool != processingTools.end()) {
			auto _tool = dynamic_cast<c_modifyingImageProcessingTool*>(currentTool -> get());
			if(_tool != nullptr) _tool -> decrease_effect();
			
		}
	}
	
	return;
}

void c_DIPMode::key_handler_arrow_left(const SDL_KeyboardEvent &_event) {
	if(_event.type == SDL_KEYDOWN) {
		if(currentTool != processingTools.begin()) currentTool--;
		
	}
	
	return;
}

void c_DIPMode::key_handler_arrow_right(const SDL_KeyboardEvent &_event) {
	if(_event.type == SDL_KEYDOWN) {
		if((currentTool + 1) != processingTools.end()) currentTool++;
		
	}
	
	return;
}

void c_DIPMode::key_handler_escape(const SDL_KeyboardEvent &_event) {
	if(_event.type == SDL_KEYDOWN) {
		auto _request = dynamic_cast<c_request*>(new c_screenChangeRequestNPKG(1));
		requests.push(std::shared_ptr<c_request>(_request));
		
	}
	
	return;
}

void c_DIPMode::mouse_button_handler_left(const SDL_MouseButtonEvent &_event) {
	if(_event.type == SDL_MOUSEBUTTONDOWN) {
		if(currentTool != processingTools.end()) {
			auto _tool = dynamic_cast<c_modifyingImageProcessingTool*>(currentTool -> get());
			if(_tool != nullptr) _tool -> affect_pixel(_event.x, _event.y);
			
		}
	}
	
	return;
}

void c_DIPMode::drop_file_handler(const SDL_DropEvent &_event) {
	if(_event.file != nullptr) {
		baseImage.load_file(std::string(_event.file));
		auto _request = dynamic_cast<c_request*>(new c_resizeRequest(baseImage.get_width(), baseImage.get_height()));
		requests.push(std::shared_ptr<c_request>(_request));
		screen = baseImage;
		
	}
	
	SDL_free(_event.file);
	
	return;
}