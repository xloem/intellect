#pragma once

#include <library/framebuffer-basic.hpp>

#include <Fastor/tensor/Tensor.h>
#include <Fastor/tensor/TensorMap.h>

namespace library {

template <size_t stride_, size_t height_, size_t Bpp_ = 4>
class framebuffer_fastor : public framebuffer_basic, public Fastor::TensorMap<uint8_t, height_, stride_, Bpp_>
{
public:
	framebuffer_fastor(char const * name);
	~framebuffer_fastor();

	using coord_type = Fastor::Tensor<unsigned,2>;

	template <typename Coordinates>
	inline auto
	pixel(Coordinates coordinates)
	{ return (*this)(coordinates(1), coordinates(0), Fastor::all); }

	template <typename LowerCoordinates = coord_type, typename UpperCoordinates = coord_type>
	inline auto
	rectangle(LowerCoordinates lower_coordinates, UpperCoordinates upper_coordinates)
	{ return (*this)(Fastor::seq(lower_coordinates(1), upper_coordinates(1)), Fastor::seq(lower_coordinates(0), upper_coordinates(0)), Fastor::all); }

	template <typename LowerCoordinates = coord_type, typename UpperCoordinates = coord_type>
	inline void
	blit_to(LowerCoordinates lower_coordinates = {}, UpperCoordinates upper_coordinates = {})
	{ framebuffer_basic::blit_to(lower_coordinates(0), lower_coordinates(1), upper_coordinates(0), upper_coordinates(1)); }

	template <typename LowerCoordinates = Fastor::Tensor<unsigned,2>, typename UpperCoordinates = Fastor::Tensor<unsigned,2>>
	inline void
	blit_from(LowerCoordinates lower_coordinates = {}, UpperCoordinates upper_coordinates = {})
	{ framebuffer_basic::blit_from(lower_coordinates(0), lower_coordinates(1), upper_coordinates(0), upper_coordinates(1)); }

	inline coord_type constexpr
	dimensions()  { return {stride_, height_}; }
};

}
