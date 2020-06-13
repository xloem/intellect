#include <cmath>
#include <vector>

#include <library/framebuffer-armadillo.hpp>

using namespace arma;
using namespace std;

class backed_by_screen
{
public:
	backed_by_screen()
	: fb0("fb0"),
	  _center(conv_to<Col<sword>>::from(fb0.dimensions())/2)
	{
	}

	void write(ivec2 where, double value)
	{
		framebuffer_armadillo::pixel_type to_write;
		to_write[0] = to_write[1] = to_write[2] = value * 255;
		where += _center;
		fb0.pixel(where).fill(value * 255);// = to_write;
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
		return fb0.pixel(where)[0] / 255.0;
		//return fb0.pixel(where.column, where.row)[0] / 255.0;
	}

	ivec2 const & center() { return _center; }

private:
	framebuffer_armadillo fb0;
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
			ivec2 spot_where = spot + where;
			plane.write(spot_where, strength * dropoff(sqrt(dot(spot,spot))/radius));
		}
	}
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
