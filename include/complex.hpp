#ifndef COMPLEX_HPP
#define COMPLEX_HPP

#include <cmath>

const double PI = 2 * acos(0);

// Number in the form a + bi
class c_complex {
	public:
		c_complex(const double &_a = 0.0, const double &_b = 0.0);
		c_complex(const std::pair<double, double> _pair);
		c_complex operator+(const c_complex &_z) const noexcept;
		c_complex operator+=(const c_complex &_z) noexcept;
		c_complex operator-(const c_complex &_z) const noexcept;
		c_complex operator-=(const c_complex &_z) noexcept;
		c_complex operator*(const c_complex &_z) const noexcept;
		c_complex operator*=(const c_complex &_z) noexcept;
		c_complex operator/(const c_complex &_z) const noexcept;
		c_complex operator/=(const c_complex &_z) noexcept;
		c_complex operator+(const double &_v) const noexcept;
		c_complex operator+=(const double &_v) noexcept;
		c_complex operator-(const double &_v) const noexcept;
		c_complex operator-=(const double &_v) noexcept;
		c_complex operator*(const double &_v) const noexcept;
		c_complex operator*=(const double &_v) noexcept;
		c_complex operator/(const double &_v) const noexcept;
		c_complex operator/=(const double &_v) noexcept;
		c_complex conjugate() const noexcept;
		double modulus() const noexcept;
		static c_complex expi(const double &_v) noexcept;
		
		double a;
		double b;
	
	private:
	
};

c_complex operator+(const double &_v, const c_complex &_z) noexcept;
c_complex operator-(const double &_v, const c_complex &_z) noexcept;
c_complex operator*(const double &_v, const c_complex &_z) noexcept;
c_complex operator/(const double &_v, const c_complex &_z) noexcept;

#endif