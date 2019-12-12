#include "complex.hpp"

c_complex::c_complex(const double &_a, const double &_b) {
	a = _a;
	b = _b;
	
	return;
}

c_complex::c_complex(const std::pair<double, double> _pair) {
	a = _pair.first;
	b = _pair.second;
	
}

c_complex c_complex::operator+(const c_complex &_z) const noexcept {
	return c_complex(a + _z.a, b + _z.b);
}

c_complex c_complex::operator+=(const c_complex &_z) noexcept {
	*this = *this + _z;
	
	return *this;
}

c_complex c_complex::operator-(const c_complex &_z) const noexcept {
	return c_complex(a - _z.a, b - _z.b);
}

c_complex c_complex::operator-=(const c_complex &_z) noexcept {
	*this = *this - _z;
	
	return *this;
}

c_complex c_complex::operator*(const c_complex &_z) const noexcept {
	return c_complex(	(a * _z.a) - (b * _z.b),
						(a * _z.b) + (b * _z.a));
}

c_complex c_complex::operator*=(const c_complex &_z) noexcept {
	*this = *this * _z;
	
	return *this;
}

c_complex c_complex::operator/(const c_complex &_z) const noexcept {
	double _d = pow(_z.a, 2) + pow(_z.b, 2);
	
	// (a+bi)/(c+di) = (a+bi)(c-di)/(c2 + d2)
	
	return c_complex(	(a * _z.a + b * _z.b) / _d,
						(b * _z.a - a * _z.b) / _d);
}

c_complex c_complex::operator/=(const c_complex &_z) noexcept {
	*this = *this / _z;
	
	return *this;
}

c_complex c_complex::operator+(const double &_v) const noexcept {
	return c_complex(a + _v, b);
}

c_complex c_complex::operator+=(const double &_v) noexcept {
	*this = *this + _v;
	
	return *this;
}

c_complex c_complex::operator-(const double &_v) const noexcept {
	return c_complex(a - _v, b);
}

c_complex c_complex::operator-=(const double &_v) noexcept {
	*this = *this - _v;
	
	return *this;
}

c_complex c_complex::operator*(const double &_v) const noexcept {
	return c_complex(a * _v, b * _v);
}

c_complex c_complex::operator*=(const double &_v) noexcept {
	*this = *this * _v;
	
	return *this;
}

c_complex c_complex::operator/(const double &_v) const noexcept {
	return c_complex(a / _v, b / _v);
}

c_complex c_complex::operator/=(const double &_v) noexcept {
	*this = *this / _v;
	
	return *this;
}

double c_complex::modulus() const noexcept {
	return sqrt(pow(a, 2) + pow(b, 2));
}

c_complex c_complex::expi(const double &_t) noexcept {
	return c_complex(cos(_t), sin(_t));
}

c_complex operator+(const double &_v, const c_complex &_z) noexcept {
	return _z + _v;
}

c_complex operator-(const double &_v, const c_complex &_z) noexcept {
	return c_complex(_v - _z.a, -_z.b);
}

c_complex operator*(const double &_v, const c_complex &_z) noexcept {
	return _z * _v;
}

c_complex operator/(const double &_v, const c_complex &_z) noexcept {
	return c_complex(_v, 0) / _z;
}

c_complex c_complex::conjugate() const noexcept {
	return c_complex(a, -b);
}
