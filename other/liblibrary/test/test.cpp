#include <library/framebuffer.hpp>

#include <cmath>
#include <iostream>
#include <string>

#include <string.h>

using namespace std;
string error() { cerr << endl; exit(1); return ""; }

int main()
{
	framebuffer fb0("fb0");
	cerr << "fb0 width: " << fb0.width() << endl;
	cerr << "fb0 height: " << fb0.height() << endl;
	cerr << "fb0 stride: " << fb0.stride() << endl;
	if (fb0.width() < 50 || fb0.width() > 5000) {
		cerr << "Unlikely width: " << fb0.width() << error();
	}
	if (fb0.height() < 50 || fb0.height() > 5000) {
		cerr << "Unlikely height: " << fb0.height() << error();
	}
	if (fb0.stride() < 50 || fb0.stride() > 5000) {
		cerr << "Unlikely stride: " << fb0.stride() << error();
	}

	int center_width = fb0.width() / 2;
	int center_height = fb0.height() / 2;
	int radius = center_height / 2;
	for (int row = center_height - radius; row <= center_height + radius; ++ row) {
		int row_distance = row - center_height;
		int half_width = sqrt(radius * radius - row_distance * row_distance);
		for (int column = center_width - half_width; column <= center_width + half_width; ++ column) {
			fb0.pixel(column, row)[1] = 128;
			if (fb0.pixel(column, row)[1] != 128) {
				cerr << "Failed to write pixel " << error();
			}
		}
	}

	return 0;
}
