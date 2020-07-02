#include <library/framebuffer-basic.hpp>

#include <cmath>

#include "test.hpp"

int main()
{
	framebuffer_basic fb0("fb0");
	stderr::line(string("fb0 width: ") + fb0.width());
	stderr::line(string("fb0 height: ") + fb0.height());
	stderr::line(string("fb0 stride: ") + fb0.stride());
	worry(fb0.width() < 50 || fb0.width() > 5000, string("Unlikely width: ") + fb0.width());
	worry(fb0.height() < 50 || fb0.height() > 5000, string("Unlikely height: ") + fb0.height());
	worry(fb0.stride() < 50 || fb0.stride() > 5000, string("Unlikely height: ") + fb0.stride());

	int center_width = fb0.width() / 2;
	int center_height = fb0.height() / 2;
	int radius = center_height / 2;

	for (int row = center_height - radius; row <= center_height + radius; ++ row) {
		int row_distance = row - center_height;
		int half_width = sqrt(radius * radius - row_distance * row_distance);

		for (int column = center_width - half_width; column <= center_width + half_width; ++ column) {
			fb0.pixel(column, row)[1] = 128;
			fb0.pixel(column, row)[3] = 0;
		}
		fb0.blit_to(center_width - half_width, row, center_width + half_width + 1, row + 1);
		for (int column = center_width - half_width; column <= center_width + half_width; ++ column) {
			fb0.pixel(column, row)[1] = 0;
			fb0.pixel(column, row)[3] = 128;
		}
		fb0.blit_from(center_width - half_width, row, center_width + half_width + 1, row + 1);
		for (int column = center_width - half_width; column <= center_width + half_width; ++ column) {
			worry(fb0.pixel(column, row)[1] != 128 || fb0.pixel(column, row)[3] != 0, "Wrote pixel {?,128,?,0} and read pixel {" + string(fb0.pixel(column, row)[0]) + "," + string(fb0.pixel(column, row)[1]) + "," + string(fb0.pixel(column, row)[2]) + "," + string(fb0.pixel(column, row)[3]) + "}");
			fb0.pixel(column, row)[0] = 128;
			fb0.pixel(column, row)[1] = 0;
			fb0.pixel(column, row)[2] = 128;
		}
	}
	fb0.blit_to();

	stderr::line("framebuffer_basic passed test");
}
