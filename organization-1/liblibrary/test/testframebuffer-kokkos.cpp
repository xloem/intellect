#include <library/framebuffer-kokkos.hpp>

#include <library/time.hpp>

#include <cmath>

#include "test.hpp"

using namespace Kokkos;

int main(int argc, char ** argv)
{
	ScopeGuard kokkos(argc, argv);
	framebuffer_kokkos fb0("fb0");

	stderr::line("fb0 dimensions: " + string(fb0.extent(0)) + "x" + string(fb0.extent(1)) + "x" + string(fb0.extent(2)));
}
