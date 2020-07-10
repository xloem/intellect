HAVE_FASTOR=$(g++ include Fastor/tensor/Tensor.h -x c++ -E </dev/null >/dev/null && echo 1)
ifeq ($(HAVE_FASTOR),1)
	CPPFLAGS+=-DLIBLIBRARY_HAVE_FASTOR=1
	OPTIONALS+=framebuffer-fastor
endif
