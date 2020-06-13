#pragma once

#include <cstdint>
#include <armadillo>

#include "framebuffer-basic.hpp"

class framebuffer_armadillo : public framebuffer_basic, public arma::Cube<uint8_t>
{
public:
	framebuffer_armadillo(char const * name);
	~framebuffer_armadillo();

	using pixel_type = arma::Col<uint8_t>::fixed<4>;

	inline arma::subview_cube<uint8_t>
	pixel(arma::uvec2 where)
	{ return (*this)(arma::span::all, arma::span{where[0]}, arma::span{where[1]}); }

	inline arma::subview_cube<uint8_t>
	pixel(arma::ivec2 where)
	{ return (*this)(arma::span::all, arma::span{arma::uword(where[0])}, arma::span{arma::uword(where[1])}); }

	inline arma::subview_cube<uint8_t>
	rectangle(arma::uvec2 lower_bound, arma::uvec2 upper_bound)
	{ return (*this)(arma::span::all, arma::span(lower_bound[0], lower_bound[1]), arma::span(upper_bound[0], upper_bound[1])); }

	inline arma::subview_cube<uint8_t>
	rectangle(arma::ivec2 lower_bound, arma::uvec2 upper_bound)
	{ return (*this)(arma::span::all, arma::span(arma::uword(lower_bound[0]), arma::uword(lower_bound[1])), arma::span(arma::uword(upper_bound[0]), arma::uword(upper_bound[1]))); }

	arma::uvec2 dimensions() { return {n_cols, n_slices}; }
};

