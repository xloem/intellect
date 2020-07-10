KOKKOS_CPPFLAGS=-I/usr/include/trilinos
KOKKOS_LDLIBS=-ltrilinos_kokkoscore
HAVE_KOKKOS=$(g++ ${KOKKOS_CPPFLAGS} -include Kokkos_Core.hpp -x c++ -E -</dev/null >/dev/null && echo 1)
ifeq ($(HAVE_KOKKOS),1)
		LDLIBS+=$(KOKKOS_LDLIBS)
		CPPFLAGS+=$(KOKKOS_CPPFLAGS)
		OPTIONALS+=framebuffer-kokkos
endif
