
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
	int fd;
	size_t width;
	size_t height;
	size_t stride;
	size_t bpp;
	framebuffer_basic::pixel_type * memory;
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
	fb.memory = (decltype(fb.memory))mmap(NULL, fb.stride * fb.height, PROT_READ | PROT_EXEC | PROT_WRITE, MAP_SHARED, fb.fd, 0);
	if (fb.memory == MAP_FAILED) { perror("mmap"); throw std::exception(); }
}

arma::Cube<uint8_t> make_cube(char const * name, void * & storage)
{
	//init(name, storage);

	return arma::Cube<uint8_t>(&fb.memory[0][0], 4, fb.stride / 4, fb.height, false, true);
}

framebuffer_basic::framebuffer_basic(char const * name)
{
	init(name, storage);
}

framebuffer_basic::~framebuffer_basic()
{
	munmap(fb.memory, fb.stride * fb.height);
	fb.memory = nullptr;
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
	return fb.memory[column + row * stride()];
}

int framebuffer_basic::stride()
{
	return fb.stride / (fb.bpp >> 3);
}

framebuffer_basic::pixel_type * framebuffer_basic::memory()
{
	return fb.memory;
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
