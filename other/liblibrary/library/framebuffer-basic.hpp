#pragma once

#include <cstdint>

class framebuffer_basic
{
public:
	framebuffer_basic(char const * name);
	~framebuffer_basic();

	using pixel_type = uint8_t[4];
	
	int width();
	int height();

	//void write(int column, int row, pixel_type what);
	//pixel_type read(int column, int row);
	pixel_type & pixel(int column, int row);

	int stride();
	pixel_type * memory();

	// ?
	//void write_raw(int offset, pixel_type const * data, int length);
	//void read_raw(int offset, pixel_type * data, int length);

protected:
	void * storage;
};
