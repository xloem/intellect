#include <library/framebuffer-armadillo.hpp>

#include <library/heapvector_definition.cpp>
#include <library/time.hpp>

#include <cmath>

#include "test.hpp"

using namespace arma;

int main()
{
	framebuffer_armadillo fb0("fb0");
	stderr::line("fb0 dimensions: " + string(fb0.dimensions()[0]) + "x" + string(fb0.dimensions()[1]));

	sleep_for(0.1);

	uword center_width = fb0.width() / 2;
	uword center_height = fb0.height() / 2;
	uword radius = center_height / 2;
	heapvector<uword> half_widths(radius * 2 + 1);

	for (uword row = center_height - radius; row <= center_height + radius; ++ row) {
		uword row_distance = row - center_height;
		half_widths[row_distance + radius] = sqrt(radius * radius - row_distance * row_distance);
	}

	for (uword row = center_height - radius; row <= center_height + radius; ++ row) {
		uword half_width = half_widths[row + radius - center_height];
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
			worry(fb0.pixel(uvec2{column,row})[1] != 128 || fb0.pixel(uvec2{column,row})[3] != 0, "Wrote pixel {?,128,?,0} and read pixel {" + string(fb0.pixel(uvec2{column,row})[0]) + "," + string(fb0.pixel(uvec2{column,row})[1]) + "," + string(fb0.pixel(uvec2{column,row})[2]) + "," + string(fb0.pixel(uvec2{column,row})[3]) + "}");
			fb0.pixel(uvec2{column,row})[0] = 128;
			fb0.pixel(uvec2{column,row})[1] = 0;
			fb0.pixel(uvec2{column,row})[2] = 128;
		}
	}
	fb0.blit_to({center_width - radius, center_height - radius}, {center_width + radius + 1, center_height + radius + 1});
	for (uword row = center_height - radius; row <= center_height + radius; ++ row) {
		uword half_width = half_widths[row + radius - center_height];
		for (uword column = center_width - half_width; column <= center_width + half_width; ++ column) {
			worry(fb0.pixel(uvec2{column,row})[0] != 0x80 || fb0.pixel(uvec2{column,row})[1] != 0 || fb0.pixel(uvec2{column,row})[2] != 0x80 || fb0.pixel(uvec2{column,row})[3] != 0, "Wrote pixel {0x80,0x00,0x80,0x00} and read pixel {" + string(fb0.pixel(uvec2{column,row})[0], 16, true) + "," + string(fb0.pixel(uvec2{column,row})[1], 16, true) + "," + string(fb0.pixel(uvec2{column,row})[2], 16, true) + "," + string(fb0.pixel(uvec2{column,row})[3], 16, true) + "}");
		}
	}

	stderr::line("framebuffer_armadillo passed test");
}
