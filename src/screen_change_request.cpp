#include "screen_change_request.hpp"

c_screenChangeRequest::c_screenChangeRequest(uint32_t _newID) {
	newID = _newID;
	
	return;
}

c_screenChangeRequest::~c_screenChangeRequest() {
	return;
}

uint32_t c_screenChangeRequest::get_new_id() const {
	return newID;
}

template<typename PACKAGE>
c_screenChangeRequestPKG<PACKAGE>::c_screenChangeRequestPKG(uint32_t _newID, PACKAGE *_package) : c_screenChangeRequest(_newID) {
	package = _package;
	
	return;
}

template<typename PACKAGE>
c_screenChangeRequestPKG<PACKAGE>::~c_screenChangeRequestPKG() {
	return;
}

template<typename PACKAGE>
const PACKAGE* c_screenChangeRequestPKG<PACKAGE>::get_package() const {
	return package;
}

c_screenChangeRequestNPKG::c_screenChangeRequestNPKG(uint32_t _newID) : c_screenChangeRequest(_newID) {
	return;
}

c_screenChangeRequestNPKG::~c_screenChangeRequestNPKG() {
	return;
}