#include <fstream>

#include "edit_mode.hpp"

const std::map<SDL_Keycode, t_editModeKeyHandler> c_editMode::KEYHANDLERS = {
	{SDLK_c, key_handler_c},
	{SDLK_p, key_handler_p},
	{SDLK_m, key_handler_m}
};

const std::map<uint8_t, t_editModeMouseButtonHandler> c_editMode::MOUSEBUTTONHANDLERS = {
	{SDL_BUTTON_LEFT, mouse_button_handler_left},
	{SDL_BUTTON_RIGHT, mouse_button_handler_right},
	{SDL_BUTTON_MIDDLE, mouse_button_handler_middle}
};

c_editMode::c_editMode(int _width, int _height) : c_view(_width, _height){
	read_points(STANDARDPOINTSFILENAME);
	drawPoints = true;
	
	return;
}

c_editMode::~c_editMode() {
	save_points(STANDARDPOINTSFILENAME);
	
	return;
}

void c_editMode::operator()(const SDL_Event &_event) {
	switch(_event.type) {
		case SDL_KEYDOWN: [[fallthrough]]
		case SDL_KEYUP:
		{
			auto _keyHandler = KEYHANDLERS.find(_event.key.keysym.sym);
			if(_keyHandler != KEYHANDLERS.end()) {
				if(_keyHandler -> second != nullptr) _keyHandler -> second(this, _event.key);
				
			}
		}
			break;
		
		case SDL_MOUSEBUTTONDOWN: [[fallthrough]]
		case SDL_MOUSEBUTTONUP:
		{
			auto _mouseButtonHandler = MOUSEBUTTONHANDLERS.find(_event.button.button);
			if(_mouseButtonHandler != MOUSEBUTTONHANDLERS.end()) {
				if(_mouseButtonHandler -> second != nullptr) _mouseButtonHandler -> second(this, _event.button);
				
			}
		}
			break;
		
	}
	
	return;
}

void c_editMode::render_image(const double &_time) {
	if(_time == 0.0f) {}
	screen.clear_image(BACKGROUNDCOLOR);
	for(size_t i = 1; i < points.size(); i++) {
		screen.draw_line(points[i].a, points[i].b, points[i - 1].a, points[i - 1].b, TRACECOLOR);
		
	}
	
	if(drawPoints) {
		for(const auto &i: points) {
			screen.draw_circle(i.a, i.b, 0.0075, POINTSCOLOR);
			
		}
		if(!points.empty()) {
			screen.draw_circle(currentPoint -> a, currentPoint -> b, 0.0075, SELECTEDCOLOR);
			
		}
	}
	
	return;
}

void c_editMode::activate(const c_screenChangeRequest *_request) {
	auto _changeRequest = dynamic_cast<const c_screenChangeRequestPKG<std::vector<c_complex>>*>(_request);
	if((_changeRequest != nullptr) && (_changeRequest -> get_new_id() == get_id())) {
		
	}
	
	return;
}

void c_editMode::add_point(const c_complex &_position) {
	currentPoint = points.emplace(currentPoint, _position);
	
	return;
}

void c_editMode::remove_point(const c_complex &_position) {
	if(!points.empty()) {
		auto _min = min_element(points.begin(), points.end(),
					[_position](const c_complex &_a, const c_complex &_b) {
						return (_a - _position).modulus() < (_b - _position).modulus();});
		
		if(((*_min) - _position).modulus() < 0.01) {
			points.erase(_min);
			if(currentPoint == points.end()) currentPoint = points.begin();
			
		}
	}
	
	return;
}

void c_editMode::select_point(const c_complex &_position) {
	if(!points.empty()) {
		auto _min = min_element(points.begin(), points.end(),
					[_position](const c_complex &_a, const c_complex &_b) {return (_a - _position).modulus() < (_b - _position).modulus();});
		
		if(((*_min) - _position).modulus() < 0.01) {
			currentPoint = _min;
		}
	}
	
	return;
}

void c_editMode::save_points(const std::string &_fileName) {
	std::ofstream _file(_fileName);
	
	if(_file.is_open()) {
		for(const auto &i: points) {
			_file << i.a << " " << i.b << std::endl;
			
		}
	}
	
	return;
}

void c_editMode::read_points(const std::string &_fileName) {
	std::ifstream _file(_fileName);
	
	points.clear();
	
	if(_file.is_open()) {
		c_complex _temp;
		
		while(!_file.eof()) {
			_file >> _temp.a >> _temp.b;
			if(_file.eof()) break;
			points.push_back(_temp);
			
		}
		currentPoint = points.begin();
		
		_file.close();
	}
	
	return;
}

void c_editMode::mouse_button_handler_left(const SDL_MouseButtonEvent &_event) {
	if(_event.type == SDL_MOUSEBUTTONDOWN) {
		add_point(c_complex(screen.to_rel_coord(_event.x, _event.y)));
		
	}
	
	return;
}

void c_editMode::mouse_button_handler_right(const SDL_MouseButtonEvent &_event) {
	if(_event.type == SDL_MOUSEBUTTONDOWN) {
		remove_point(c_complex(screen.to_rel_coord(_event.x, _event.y)));
		
	}
	
	return;
}

void c_editMode::mouse_button_handler_middle(const SDL_MouseButtonEvent &_event) {
	if(_event.type == SDL_MOUSEBUTTONDOWN) {
		select_point(c_complex(screen.to_rel_coord(_event.x, _event.y)));
		
	}
	
	return;
}

void c_editMode::key_handler_c(const SDL_KeyboardEvent &_event) {
	if(_event.type == SDL_KEYDOWN) {
		points.clear();
		currentPoint = points.end();
		
	}
	
	return;
}

void c_editMode::key_handler_p(const SDL_KeyboardEvent &_event) {
	if(_event.type == SDL_KEYDOWN) drawPoints = !drawPoints;
	
	return;
}

void c_editMode::key_handler_m(const SDL_KeyboardEvent &_event) {
	if(_event.type == SDL_KEYDOWN) {
		auto _request = dynamic_cast<c_request*>(new c_screenChangeRequestPKG<std::vector<c_complex>>(2, &points));
		requests.push(std::shared_ptr<c_request>(_request));
		
	}
	
	return;
}