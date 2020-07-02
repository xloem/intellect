
#include <library/framebuffer-armadillo.hpp>
#include <library/framebuffer-basic.hpp>

#include "fb_info.hpp"

arma::Cube<uint8_t> make_cube(char const * name, void * & storage)
{
	return arma::Cube<uint8_t>(&fb.back_buffer[0][0], fb.Bpp, fb.pixel_stride, fb.height, false, true);
}

framebuffer_armadillo::framebuffer_armadillo(char const * name)
: framebuffer_basic(name),
  Cube(make_cube(name, storage))
{ }

framebuffer_armadillo::~framebuffer_armadillo()
{ }
