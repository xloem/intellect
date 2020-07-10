#pragma once

#include <Kokkos_Core.hpp>

#include <library/framebuffer-basic.hpp>

class framebuffer_kokkos : public framebuffer_basic, public Kokkos::View<uint8_t**[4], Kokkos::MemoryUnmanaged>
{
public:
	framebuffer_kokkos(char const * name);
	~framebuffer_kokkos();

	using pixel_type = Kokkos::View<uint8_t[4]>;
};
