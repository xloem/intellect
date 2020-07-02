
#include <library/framebuffer-armadillo.hpp>
#include <library/framebuffer-basic.hpp>

#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <string>
using namespace std;
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

#include <library/quick.hpp>

struct fb_info
{
	size_t width;
	size_t height;
	size_t stride;
	size_t bpp;
	size_t Bpp;
	size_t pixel_stride;
	framebuffer_basic::pixel_type * back_buffer;
	int fd;
	framebuffer_basic::pixel_type * front_buffer;
};

struct unexpected_bpp : public std::exception {};

#define fb (*(fb_info*)storage)

void init(char const * name, void * & storage)
{
	storage = new fb_info();
	string fbname = "/dev/"; fbname += name;
	fb.fd = open(fbname.c_str(), O_RDWR);
	string sysdir = "/sys/class/graphics/";
	sysdir += name; sysdir += "/";
	to_number(file_read(sysdir + "stride"), fb.stride);
	auto dimensions = split(file_read(sysdir + "virtual_size"), ",");
	to_number(dimensions[0], fb.width);
	to_number(dimensions[1], fb.height);
	to_number(file_read(sysdir + "bits_per_pixel"), fb.bpp);
	if (fb.bpp != 32) { throw unexpected_bpp(); }
	fb.Bpp = fb.bpp >> 3;
	fb.pixel_stride = fb.stride / fb.Bpp;
	fb.front_buffer = (decltype(fb.front_buffer))mmap(NULL, fb.stride * fb.height, PROT_READ | PROT_EXEC | PROT_WRITE, MAP_SHARED, fb.fd, 0);
	if (fb.front_buffer == MAP_FAILED) { fb.front_buffer = nullptr; }
	fb.back_buffer = new framebuffer_basic::pixel_type[fb.pixel_stride * fb.height];
}

arma::Cube<uint8_t> make_cube(char const * name, void * & storage)
{
	//init(name, storage);

	return arma::Cube<uint8_t>(&fb.back_buffer[0][0], fb.Bpp, fb.pixel_stride, fb.height, false, true);
}

framebuffer_basic::framebuffer_basic(char const * name)
{
	init(name, storage);
	blit_from();
}

framebuffer_basic::~framebuffer_basic()
{
	if (fb.front_buffer) {
		munmap(fb.front_buffer, fb.stride * fb.height);
		fb.front_buffer = nullptr;
	}
	delete [] fb.back_buffer;
	fb.back_buffer = nullptr;
	close(fb.fd);
	fb.fd = 0;
}

framebuffer_armadillo::framebuffer_armadillo(char const * name)
: framebuffer_basic(name), Cube(make_cube(name, storage))
{ }

framebuffer_armadillo::~framebuffer_armadillo()
{ }

int framebuffer_basic::width()
{
	return fb.width;
}

int framebuffer_basic::height()
{
	return fb.height;
}

framebuffer_basic::pixel_type & framebuffer_basic::pixel(int column, int row)
{
	if (column + row * stride() * sizeof(pixel_type) > fb.stride * fb.height) {
		cerr << "out of range" << endl;
		throw std::exception();
	}
	return fb.back_buffer[column + row * stride()];
}

int framebuffer_basic::stride()
{
	return fb.pixel_stride;
}

framebuffer_basic::pixel_type * framebuffer_basic::buffer()
{
	return fb.back_buffer;
}

void framebuffer_basic::blit_to(int column0, int row0, int column1, int row1)
{
	if (!column1) { column1 = fb.width; }
	if (!row1) { row1 = fb.height; }
	if (fb.front_buffer) {
		if (column0 == 0 && (size_t)column1 >= fb.width) {
			size_t offset = row0 * fb.stride;
			memcpy(
				(uint8_t*)fb.front_buffer + offset,
				(uint8_t*)fb.back_buffer + offset,
				fb.stride * (row1 - row0));
		} else {
			column0 *= fb.Bpp;
			column1 *= fb.Bpp; 
			size_t span = column1 - column0;
			size_t start = column0 + row0 * fb.stride;
			size_t end = column0 + row1 * fb.stride;
			for (size_t offset = start; offset < end; offset += fb.stride) {
				memcpy(
					(uint8_t*)fb.front_buffer + offset,
					(uint8_t*)fb.back_buffer + offset,
					span);
			}
		}
	} else {
		if (column0 == 0 && (size_t)column1 >= fb.width) {
			size_t offset = row0 * fb.stride;
			::lseek(fb.fd, offset, SEEK_SET);
			::write(fb.fd, (uint8_t*)fb.back_buffer + offset, fb.stride * (row1 - row0));
		} else {
			column0 *= fb.Bpp;
			column1 *= fb.Bpp;
			size_t span = column1 - column0;
			size_t start = column0 + row0 * fb.stride;
			size_t end = column0 + row1 * fb.stride;
			::lseek(fb.fd, start, SEEK_SET);
			for (size_t offset = start; offset < end; offset += fb.stride) {
				::write(fb.fd, (uint8_t*)fb.back_buffer + offset, span);
				::lseek(fb.fd, fb.stride, SEEK_CUR);
			}
		}
		::fdatasync(fb.fd);
	}
}

void framebuffer_basic::blit_from(int column0, int row0, int column1, int row1)
{
	if (!column1) { column1 = fb.width; }
	if (!row1) { row1 = fb.height; }
	if (fb.front_buffer) {
		if (column0 == 0 && (size_t)column1 >= fb.width) {
			size_t offset = row0 * fb.stride;
			memcpy(
				(uint8_t*)fb.back_buffer + offset,
				(uint8_t*)fb.front_buffer + offset,
				fb.stride * (row1 - row0));
		} else {
			column0 *= fb.Bpp;
			column1 *= fb.Bpp;
			size_t span = column1 - column0;
			size_t start = column0 + row0 * fb.stride;
			size_t end = column0 + row1 * fb.stride;
			for (size_t offset = start; offset < end; offset += fb.stride) {
				memcpy(
					(uint8_t*)fb.back_buffer + offset,
					(uint8_t*)fb.front_buffer + offset,
					span);
			}
		}
	} else {
		if (column0 == 0 && (size_t)column1 >= fb.width) {
			size_t offset = row0 * fb.stride;
			::lseek(fb.fd, offset, SEEK_SET);
			::read(fb.fd, (uint8_t*)fb.back_buffer + offset, fb.stride * (row1 - row0));
		} else {
			column0 *= fb.Bpp;
			column1 *= fb.Bpp;
			size_t span = column1 - column0;
			size_t start = column0 + row0 * fb.stride;
			size_t end = column0 + row1 * fb.stride;
			::lseek(fb.fd, start, SEEK_SET);
			for (size_t offset = start; offset < end; offset += fb.stride) {
				::read(fb.fd, (uint8_t*)fb.back_buffer + offset, span);
				::lseek(fb.fd, fb.stride, SEEK_CUR);
			}
		}
	}
}

/*
void framebuffer::write(int column, int row, pixel_type what)
{
	::write(fb.fd, &what, sizeof(what));
}

framebuffer::pixel_type framebuffer::read(int column, int row)
{
	framebuffer::pixel_type result;
	::read(fb.fd, &result, sizeof(result));
	return result;
}
*/
