#include <library/unix_socket.hpp>

#include <library/stackvector_definition.cpp>

#ifndef _XPG4_2 /* Solaris */
# define _XPG4_2
#endif

#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/un.h>
#include <unistd.h>
#if defined(__FreeBSD__)
# include <sys/param.h> /* FreeBSD */
#endif

#include <memory>
#include <stdexcept>

namespace library {

template class stackvector<int, 253>;

static int handle(int result)
{
	if (result < 0) {
		throw std::runtime_error("failed to open unix socket: " + string(strerror(errno)).std());
	}
	return result;
}

unix_socket::unix_socket(string pathname, bool server)
: pathname(pathname),
  socket(::socket(AF_UNIX, SOCK_STREAM, 0))
{
	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	if (pathname.size() >= sizeof(addr.sun_path)) {
		throw std::runtime_error("path too long");
	}
	strncpy(addr.sun_path, pathname.c_str(), sizeof(addr.sun_path) - 1);
	if (server) {
		unlink(pathname.c_str());
		handle(bind(socket, (struct sockaddr*)&addr, sizeof(addr)));
		handle(listen(socket, 1));
		int result = handle(accept(socket, NULL, NULL));
		//close(socket);
		const_cast<int&>(socket) = result;
	} else {
		handle(connect(socket, (struct sockaddr*)&addr, sizeof(addr)));
	}
}

unix_socket::unix_socket(int socket)
: socket(socket)
{ }

unix_socket::~unix_socket()
{
	close(socket);
	unlink(pathname.c_str());
}

template <unsigned long reserved_bytes>
struct ancillary_message {
	struct msghdr msghdr;
	struct cmsghdr *cmsg;
	unsigned char count;
	struct iovec count_ptr;
	union {
		unsigned char bytes[CMSG_SPACE(reserved_bytes)];
		struct cmsghdr align;
	} buffer;

	ancillary_message()
	: msghdr{0}
	{
		count_ptr.iov_base = &count;
		count_ptr.iov_len = sizeof(count);
		msghdr.msg_name = NULL;
		msghdr.msg_namelen = 0;
		msghdr.msg_iov = &count_ptr;
		msghdr.msg_iovlen = 1;
		msghdr.msg_flags = 0;
		msghdr.msg_control = buffer.bytes;
		msghdr.msg_controllen = CMSG_SPACE(reserved_bytes);
		cmsg = CMSG_FIRSTHDR(&msghdr);
		cmsg->cmsg_len = CMSG_LEN(reserved_bytes);
		cmsg->cmsg_level = SOL_SOCKET;
		cmsg->cmsg_type = SCM_RIGHTS;
	}

	void set_data(void const * source, unsigned long element_size, unsigned long count)
	{
		this->count = count;
		cmsg->cmsg_len = CMSG_LEN(count * element_size);
		msghdr.msg_controllen = CMSG_SPACE(count * element_size);
		memcpy(CMSG_DATA(cmsg), source, count * element_size);
	}

	unsigned long get_data(void * destination, unsigned long element_size)
	{
		count = (cmsg->cmsg_len - CMSG_LEN(0)) / element_size; // should be able to comment this out and use transferred count too
		memcpy(destination, CMSG_DATA(cmsg), count * element_size);
		return count;
	}

	int send(int socket)
	{
		return sendmsg(socket, &msghdr, 0);
	}

	int recv(int socket)
	{
		return recvmsg(socket, &msghdr, 0);
	}
	
};

void unix_socket::send_fds(stackvector<int,253> const & fds)
{
	stderr::line("sending " + string(fds.size()) + " fds ...");

	ancillary_message<fds.reserved_bytes()> message;
	message.set_data(fds.data(), fds.size(), sizeof(int));
	int result = message.send(socket);

	if (result < 0) {
		throw std::runtime_error("fds failed sendmsg: " + string(strerror(errno)).std());
	}
}

stackvector<int,253> unix_socket::recv_fds()
{
	stackvector<int,253> fds;
	ancillary_message<fds.reserved_bytes()> message;
	if (message.recv(socket) < 0) {
		throw std::runtime_error("fds failed recvmsg: " + string(strerror(errno)).std());
	}

	fds.resize(message.get_data(fds.data(), sizeof(int)));

	return fds;
}

} // namespace library
