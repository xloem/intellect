#include <cmath>
#include <vector>

#include <library/framebuffer-armadillo.hpp>

using namespace arma;
using namespace std;

class backed_by_screen : private framebuffer_armadillo, public mat
{
public:
	backed_by_screen()
	: framebuffer_armadillo("fb0"),
	  mat(height(), width()),
	  _center(conv_to<Col<sword>>::from(dimensions())/2)
	{
	}

	void blit()
	{
		// this is incredibly concise.  it took us like 30 minutes to
		// learn to do the below line, which converts a grayscale
		// floating-point array into our directly-mirrored system framebuffer

		// okay each_slice will take visual-rows.
		// gotta somehow map those with the other visual-rows, columns.
		// i guess a list of column indices would do it ...
		pixel_type color;
		uvec2 where;
		for (where[1] = 0; where[1] < (uword)height(); ++ where[1]) {
			for (where[0] = 0; where[0] < (uword)width(); ++ where[0]) {
				color.fill(mat::at(where[1],where[0]) * 255);
				pixel(where) = color;
			}
		}
		//framebuffer_armadillo::each_slice().each_row() = (*this) * 255;
		
		// uhh we are on fence around not-invest-more-to-learn-armadillo
		// and not-use-armadillo-at-all.  second option does not have
		// full plan.
		// 	maybe for this situation an alternative to armadillo?
		// 	we could try eigen later, maybe.
		// 	or we could only use the base class.
	}

	void write(ivec2 where, double value)
	{
		framebuffer_armadillo::pixel_type to_write;
		to_write[0] = to_write[1] = to_write[2] = value * 255;
		where += _center;
		pixel(where).fill(value * 255);// = to_write;
		//fb0.buffer(0, where.column, where.row) = to_write[0];
		//fb0.buffer(1, where.column, where.row) = to_write[1];
		//fb0.buffer(2, where.column, where.row) = to_write[2];
		/*
		*(uint32_t*)&fb0.pixel(where.column, where.row) = *(uint32_t*)&to_write;
		*/
	}

	double read(ivec2 where)
	{
		where += _center;
		return pixel(where)[0] / 255.0;
		//return fb0.pixel(where.column, where.row)[0] / 255.0;
	}

	ivec2 const & center() { return _center; }

private:
	ivec2 _center;
};

double sine_dropoff(double ratio)
{
	return cos(ratio * M_PI / 2);
}

#include <functional>
void paint_influence(backed_by_screen & plane, uvec2 where, double radius, double strength, function<double(double)> dropoff)
{
	ivec2 spot;
	for (spot[1] = -radius; spot[1] <= radius; ++ spot[1]) {
		sword half_width = sqrt(radius * radius - spot[1] * spot[1]);
		for (spot[0] = -half_width; spot[0] <= half_width; ++ spot[0]) {
			ivec2 spot_where = spot + where + plane.center();
			((mat&)(plane))(spot_where[1], spot_where[0]) = strength * dropoff(sqrt(dot(spot,spot))/radius);
			//plane.write(spot_where, strength * dropoff(sqrt(dot(spot,spot))/radius));
		}
	}
	plane.blit();
}

int main()
{
	backed_by_screen plane;

	double x =0;
	while (true) {
		x += 1.0/16;
		paint_influence(plane, {0,0}, plane.center()[1] / 2, sin(x), sine_dropoff);
	}
}