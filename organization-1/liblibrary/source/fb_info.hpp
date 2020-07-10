#pragma once

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
#define fb (*(fb_info*)storage)
