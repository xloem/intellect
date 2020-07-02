
#include <library/framebuffer_kokkos.hpp>

#include "fb_info.hpp"

framebuffer_kokkos::framebuffer_kokkos(char const * name)
: framebuffer_basic(name),
  View(&fb.back_buffer[0][0], fb.pixel_stride, fb.height)
{ }

framebuffer_kokkos::~framebuffer_kokkos()
{ }
