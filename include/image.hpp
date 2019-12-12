#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <vector>

#include <SDL2/SDL.h>

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
union u_color {
	struct {
		unsigned char alpha;
		unsigned char red;
		unsigned char green;
		unsigned char blue;
	} channels;
	uint32_t colors;
	
};
#else
union u_color {
	struct {
		unsigned char blue;
		unsigned char green;
		unsigned char red;
		unsigned char alpha;
	} channels;
	uint32_t colors;
	
};
#endif

struct s_color {
	float red;
	float green;
	float blue;
	u_color to_24_bit_RGBA() const;
	
	
};

const s_color RED = {1.0f, 0.0f, 0.0f};
const s_color GREEN = {0.0f, 1.0f, 0.0f};
const s_color BLUE = {0.0f, 0.0f, 1.0f};
const s_color CYAN = {0.0f, 1.0f, 1.0f};
const s_color PINK = {1.0f, 0.0f, 1.0f};
const s_color YELLOW = {1.0f, 1.0f, 0.0f};
const s_color WHITE = {1.0f, 1.0f, 1.0f};
const s_color BLACK = {0.0f, 0.0f, 0.0f};

class c_image {
	public:
		c_image();
		c_image(int _width, int _height);
		c_image(const std::string &_filename);
		s_color& operator[](const size_t &_i);
		const s_color& get_color(const size_t &_i) const;
		
		void load_file(const std::string &_filename);
		int get_width() const;
		int get_height() const;
		void set_width(int _width);
		void set_height(int _height);
		void resize(int _width, int _height);
		
		void draw_to_buffer(void *_pixels);
		
		void clear_image(const s_color &_color);
		
		float& red(const size_t &_i);
		float& green(const size_t &_i);
		float& blue(const size_t &_i);
		
		size_t to_lin_coord(float _x, float _y) const;
		size_t to_lin_coord(const int &_x, const int &_y) const;
		std::pair<float, float> to_rel_coord(const size_t &_pos) const;
		std::pair<float, float> to_rel_coord(const int &_x, const int &_y) const;
		std::pair<int, int> to_abs_coord(float _x, float _y) const;
		std::pair<int, int> to_abs_coord(const size_t &_pos) const;
		
		void draw_dot(const float &_x, const float &_y, const s_color &_color);
		void draw_circle(const float &_x, const float &_y, const float &_r, const s_color &_color);
		void draw_line(float _x0, float _y0, float _x1, float _y1, const s_color &_color);
		
	private:
		int width;
		int height;
		float screenRatio;
		std::vector<s_color> colors;
	
	
};

#endif