#include <library/framebuffer.hpp>

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
	framebuffer::pixel_type * memory;
};

#define fb (*(fb_info*)this->storage)

struct unexpected_bpp : public std::exception {};

framebuffer::framebuffer(char const * name)
{
	fb_info * storage = new fb_info();
	this->storage = storage;
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
	fb.memory = (pixel_type*)mmap(NULL, fb.stride * fb.height, PROT_READ | PROT_EXEC | PROT_WRITE, MAP_SHARED, fb.fd, 0);
	if (fb.memory == MAP_FAILED) { perror("mmap"); throw std::exception(); }

	//int urandomfd = open("/dev/urandom", O_RDONLY);
	//read(urandomfd, fb.memory, fb.stride * fb.height);
}

framebuffer::~framebuffer()
{
	munmap(fb.memory, fb.stride * fb.height);
	fb.memory = nullptr;
	close(fb.fd);
	fb.fd = 0;
}

int framebuffer::width()
{
	return fb.width;
}

int framebuffer::height()
{
	return fb.height;
}

framebuffer::pixel_type & framebuffer::pixel(int column, int row)
{
	if (column + row * stride() * sizeof(pixel_type) > fb.stride * fb.height) {
		cerr << "out of range" << endl;
		throw std::exception();
	}
	return fb.memory[column + row * stride()];
}

int framebuffer::stride()
{
	return fb.stride / (fb.bpp >> 3);
}

framebuffer::pixel_type * framebuffer::memory()
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
