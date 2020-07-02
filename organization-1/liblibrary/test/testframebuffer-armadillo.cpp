#include <library/framebuffer-armadillo.hpp>

#include <cmath>
#include <string.h>

#include "test.hpp"

using namespace arma;

int main()
{
	framebuffer_armadillo fb0("fb0");
	cerr << "fb0 dimensions: " << fb0.dimensions()[0] << "x" << fb0.dimensions()[1] << endl;

	uword center_width = fb0.width() / 2;
	uword center_height = fb0.height() / 2;
	uword radius = center_height / 2;
	for (uword row = center_height - radius; row <= center_height + radius; ++ row) {
		uword row_distance = row - center_height;
		uword half_width = sqrt(radius * radius - row_distance * row_distance);
		for (uword column = center_width - half_width; column <= center_width + half_width; ++ column) {
			fb0.pixel(uvec2{column, row})[1] = 128;
			fb0.pixel(uvec2{column, row})[3] = 0;
		}
		fb0.framebuffer_basic::blit_to(center_width - half_width, row, center_width + half_width + 1, row + 1);
		for (uword column = center_width - half_width; column <= center_width + half_width; ++ column) {
			fb0.pixel(uvec2{column, row})[1] = 0;
			fb0.pixel(uvec2{column, row})[3] = 128;
		}
		fb0.framebuffer_basic::blit_from(center_width - half_width, row, center_width + half_width + 1, row + 1);
		for (uword column = center_width - half_width; column <= center_width + half_width; ++ column) {
			worry(fb0.pixel(uvec2{column, row})[1] != 128 || fb0.pixel(uvec2{column, row})[3] != 0, "Failed to write pixel");
			auto pixel = fb0.pixel(uvec2{column, row});
			pixel[0] = 128;
	       		pixel[1] = 0;
			pixel[2] = 128;
		}
	}
	fb0.blit_to();

	stderr::line("framebuffer_armadillo passed test");
}
