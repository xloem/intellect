#include <library/framebuffer-fastor.hpp>

#include <library/string.hpp>

namespace library {

template <size_t stride_, size_t height_, size_t Bpp_>
framebuffer_fastor<stride_, height_, Bpp_>::framebuffer_fastor(char const * name)
: framebuffer_basic(name),
  Fastor::TensorMap<uint8_t, height_, stride_, Bpp_>(&buffer()[0][0])
{
	if (stride_ != this->width() || height_ != this->height()) {
		stderr::line("incorrect compile-time framebuffer dimensions");
		throw std::exception();
	}
}

template <size_t stride_, size_t height_, size_t Bpp_>
framebuffer_fastor<stride_, height_, Bpp_>::~framebuffer_fastor()
{ }

} // namespace library
