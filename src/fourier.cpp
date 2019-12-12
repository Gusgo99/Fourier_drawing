#include "fourier.hpp"
#include <iostream>

c_fourier::c_fourier() {
	return;
}

void c_fourier::dft(const std::vector<c_complex> &_points) {
	if(_points.size() != 0) {
		pCoeficients.resize((_points.size() / 2) + 1);
		nCoeficients.resize((_points.size() / 2) + ((_points.size() % 2) == 0 ? 0 : 1));
		
		for(size_t i = 0; i != pCoeficients.size(); i++) {
			pCoeficients[i] = 0.0;
			for(size_t j = 0; j != _points.size(); j++) {
				pCoeficients[i] += _points[j] * c_complex::expi(-2 * PI * i * j / _points.size());
				
			}
			pCoeficients[i] /= _points.size();
			
		}
		
		for(size_t i = 1; i != nCoeficients.size(); i++) {
			nCoeficients[i] = 0.0;
			for(size_t j = 0; j != _points.size(); j++) {
				nCoeficients[i] += _points[j] * c_complex::expi( 2 * PI * i * j / _points.size());
				
			}
			nCoeficients[i] /= _points.size();
			
		}
	}
	else {
		pCoeficients.clear();
		nCoeficients.clear();
		
	}
	
	return;
}
