#include <library/framebuffer-fastor_definition.cpp>

#include <library/time.hpp>

#include <cmath>

#include "test.hpp"

#include <Fastor/Fastor.h>

using namespace Fastor;

int main()
{
	framebuffer_fastor<FBWIDTH, FBHEIGHT> fb0("fb0");
	stdout::line("fb0 dimensions:");
	print(fb0.dimensions());

	sleep_for(0.2);

	using coord = decltype(fb0)::coord_type;

	unsigned constexpr radius = FBHEIGHT / 2 / 2;
	Tensor<unsigned, radius * 2 + 1> half_widths;
	coord center = fb0.dimensions() / 2;

	//stackvector<unsigned> half_widths(radius * 2 + 1);
	half_widths.iota(center(1) - radius);
	half_widths = sqrt(radius * radius - half_widths);

	// doesn't work =/
	for (unsigned row = center(1) - radius; row <= center(1) + radius; ++ row) {
		unsigned half_width = half_widths(row + radius - center(1));
		fb0(seq(row, row), seq(center(0) - half_width, center(0) + half_width), 1) = 128;
		fb0(seq(row, row), seq(center(0) - half_width, center(0) + half_width), 3) = 0;
		//fb0.blit_to({center(0) - half_width, row},{center(0) + half_width+1,row+1});
		fb0.framebuffer_basic::blit_to();
	}
}
