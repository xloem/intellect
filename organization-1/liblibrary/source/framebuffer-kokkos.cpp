
#include <library/framebuffer-kokkos.hpp>

#include "fb_info.hpp"

framebuffer_kokkos::framebuffer_kokkos(char const * name)
: framebuffer_basic(name),
  View(&fb.back_buffer[0][0], fb.height, fb.pixel_stride)
{ }

framebuffer_kokkos::~framebuffer_kokkos()
{ }
