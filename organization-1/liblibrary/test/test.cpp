#include <library/framebuffer-armadillo.hpp>

#include <cmath>
#include <iostream>
#include <string>

#include <string.h>

using namespace std;
using namespace arma;
string error() { cerr << endl; exit(1); return ""; }

int main()
{
	framebuffer_armadillo fb0("fb0");
	cerr << "fb0 dimensions: " << fb0.dimensions()[0] << "x" << fb0.dimensions()[1] << endl;
	cerr << "fb0 dimensions: " << fb0.width() << "x" << fb0.height() << endl;

	uword center_width = fb0.width() / 2;
	uword center_height = fb0.height() / 2;
	uword radius = center_height / 2;
	for (uword row = center_height - radius; row <= center_height + radius; ++ row) {
		uword row_distance = row - center_height;
		uword half_width = sqrt(radius * radius - row_distance * row_distance);
		for (uword column = center_width - half_width; column <= center_width + half_width; ++ column) {
			fb0.pixel(uvec2{column, row})[1] = 128;
			// fb0(1, column, row) = 128;
			/*fb0.pixel(column, row)[1] = 128;
			if (fb0.pixel(column, row)[1] != 128) {
				cerr << "Failed to write pixel " << error();
			}*/
		}
	}

	/*
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
	*/

	return 0;
}
