HAVE_ARMADILLO=$(g++ -include armadillo -x c++ -E - </dev/null >/dev/null && echo 1)
ifeq ($(HAVE_ARMADILLO),1)
	LDLIBS+=armadillo
	CPPFLAGS+=-DLIBLIBRARY_HAVE_ARMADILLO=1
	OPTIONALS+=framebuffer-armadillo
endif
