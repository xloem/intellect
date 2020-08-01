HAVE_FRAMEBUFFER=$(shell test -w /dev/fb0 && echo 1)
ifeq ($(HAVE_FRAMEBUFFER),1)
	OPTIONALS+=framebuffer-basic
else
	WARN:=$(warning cannot write to /dev/fb0, not building framebuffer-basic)
endif
