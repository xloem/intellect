HAVE_CRIU=$(shell pkg-config --exists criu && echo 1)
ifeq ($(HAVE_CRIU),1)
	LDLIBS+=$(shell pkg-config --libs criu)
	CPPFLAGS+=$(shell pkg-config --cflags criu) -DLIBLIBRARY_HAVE_CRIU=1
	OPTIONALS+=criu
#test/testcriu-run: test/testcriu
#	python -c 'import pty; pty.spawn("$^");'
else
	WARN:=$(warning criu not found, not building process checkpoint/restore component)
endif
