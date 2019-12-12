#ifndef FOURIER_HPP
#define FOURIER_HPP

#include <vector>

#include "complex.hpp"

class c_fourier {
	public:
		c_fourier();
		void dft(const std::vector<c_complex> &_points);
		std::vector<c_complex> pCoeficients;
		std::vector<c_complex> nCoeficients;
	
};

#endif