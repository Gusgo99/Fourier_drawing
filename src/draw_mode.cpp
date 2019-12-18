#include "draw_mode.hpp"

const std::map<SDL_Keycode, t_drawModeKeyHandler> c_drawMode::KEYHANDLERS = {
	{SDLK_p, key_handler_p},
	{SDLK_s, key_handler_s},
	{SDLK_c, key_handler_c},
	{SDLK_m, key_handler_m},
	{SDLK_UP, key_handler_arrow_up},
	{SDLK_DOWN, key_handler_arrow_down}
};

const std::map<uint8_t, t_drawModeMouseButtonHandler> c_drawMode::MOUSEBUTTONHANDLERS = {
	
	
};

c_drawMode::c_drawMode(int _width, int _height) : c_view(_width, _height) {
	drawCircles = true;
	drawPoints = true;
	speed = 1.0f;
	
	auxScreen.resize(_width, _height);
	auxScreen.clear_image(BACKGROUNDCOLOR);
	
	return;
}

c_drawMode::~c_drawMode() {
	
	return;
}

void c_drawMode::operator()(const SDL_Event &_event) {
	switch(_event.type) {
		case SDL_KEYDOWN: [[fallthrough]]
		case SDL_KEYUP: {
			auto _keyHandler = KEYHANDLERS.find(_event.key.keysym.sym);
			if(_keyHandler != KEYHANDLERS.end()) {
				if(_keyHandler -> second != nullptr) _keyHandler -> second(this, _event.key);
				
			}
		}
			break;
		
		case SDL_MOUSEBUTTONDOWN: [[fallthrough]]
		case SDL_MOUSEBUTTONUP: {
			auto _mouseButtonHandler = MOUSEBUTTONHANDLERS.find(_event.button.button);
			if(_mouseButtonHandler != MOUSEBUTTONHANDLERS.end()) {
				if(_mouseButtonHandler -> second != nullptr) _mouseButtonHandler -> second(this, _event.button);
				
			}
		}
			break;
		
	}
	
	return;
}

void c_drawMode::render_image(const double &_time) {
	screen = auxScreen;
	
	auto _s = draw_series_truncation(_time);
	auxScreen.draw_dot(_s.a, _s.b, TRACECOLOR);
	
	if(drawPoints) {
		for(const auto &i: points) {
			screen.draw_circle(i.a, i.b, 0.0075, POINTCOLOR);
			
		}
	}
	
	return;
}

void c_drawMode::activate(const c_screenChangeRequest *_request) {
	auto _changeRequest = dynamic_cast<const c_screenChangeRequestPKG<std::vector<c_complex>>*>(_request);
	if((_changeRequest != nullptr) && (_changeRequest -> get_new_id() == get_id())) {
		points = *(_changeRequest -> get_package());
		fourier.dft(points);
		
	}
	
	return;
}

void c_drawMode::resize(int _width, int _height) {
	screen.resize(_width, _height);
	auxScreen.resize(_width, _height);
	
}

c_complex c_drawMode::draw_series_truncation(float _time) {
	c_complex _center = 0.0;
	
	if(drawCircles) {
		screen.draw_line(0.0f, 0.0f, _center.a, _center.b, CIRCLECOLOR);
		screen.draw_circle(0.0f, 0.0f, _center.modulus(), CIRCLECOLOR);
		
	}
	
	for(size_t i = 0; i != fourier.pCoeficients.size(); i++) {
		double _angSpeed = PI * i * speed / fourier.pCoeficients.size();
		
		auto _newCenter = _center + fourier.pCoeficients[i] * c_complex::expi(_angSpeed * _time);
		if(drawCircles) {
			screen.draw_line(_center.a, _center.b, _newCenter.a, _newCenter.b, CIRCLECOLOR);
			screen.draw_circle(_center.a, _center.b, (_newCenter - _center).modulus(), CIRCLECOLOR);
		
		}
		_center = _newCenter;
	
		_newCenter = _center + fourier.nCoeficients[i] * c_complex::expi(-_angSpeed * _time);
		
		if(i < fourier.nCoeficients.size()) {
			if(drawCircles) {
				screen.draw_line(_center.a, _center.b, _newCenter.a, _newCenter.b, CIRCLECOLOR);
				screen.draw_circle(_center.a, _center.b, (_newCenter - _center).modulus(), CIRCLECOLOR);
			}
			_center = _newCenter;
			
		}
	}
	
	return _center;
}

void c_drawMode::key_handler_p(const SDL_KeyboardEvent &_event) {
	if(_event.type == SDL_KEYDOWN) drawPoints = !drawPoints;
	
	return;
}

void c_drawMode::key_handler_s(const SDL_KeyboardEvent &_event) {
	if(_event.type == SDL_KEYDOWN) drawCircles = !drawCircles;
	
	return;
}

void c_drawMode::key_handler_c(const SDL_KeyboardEvent &_event) {
	if(_event.type == SDL_KEYDOWN) auxScreen.clear_image(BACKGROUNDCOLOR);
	
	return;
}

void c_drawMode::key_handler_m(const SDL_KeyboardEvent &_event) {
	if(_event.type == SDL_KEYDOWN) {
		auto _request = dynamic_cast<c_request*>(new c_screenChangeRequestNPKG(1));
		requests.push(std::shared_ptr<c_request>(_request));
		
	}
	
	return;
}

void c_drawMode::key_handler_arrow_up(const SDL_KeyboardEvent &_event) {
	if(_event.type == SDL_KEYDOWN) speed += 0.1f;
	
	return;
}

void c_drawMode::key_handler_arrow_down(const SDL_KeyboardEvent &_event) {
	if(_event.type == SDL_KEYDOWN) {
		speed -= 0.1f;
	
		if(speed < 0.0f) speed = 0.0f;
		
	}
	
	return;
}