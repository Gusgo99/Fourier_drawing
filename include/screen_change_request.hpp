#ifndef SCREEN_CHANGE_REQUEST_HPP
#define SCREEN_CHANGE_REQUEST_HPP

#include <cstdint>
#include <vector>

#include "complex.hpp"
#include "request.hpp"

class c_screenChangeRequest : public c_request {
	public:
		c_screenChangeRequest(uint32_t _newID);
		virtual ~c_screenChangeRequest();
		uint32_t get_new_id() const;
		
	private:
		uint32_t newID;
		
};

template<typename PACKAGE>
class c_screenChangeRequestPKG : public c_screenChangeRequest {
	public:
		c_screenChangeRequestPKG(uint32_t _newID, PACKAGE *_package);
		~c_screenChangeRequestPKG();
		const PACKAGE* get_package() const;
		
	private:
		PACKAGE *package;
		
};

class c_screenChangeRequestNPKG : public c_screenChangeRequest {
	public:
		c_screenChangeRequestNPKG(uint32_t _newID);
		~c_screenChangeRequestNPKG();
		
	private:
		
};

template class c_screenChangeRequestPKG<std::vector<c_complex>>;

#endif