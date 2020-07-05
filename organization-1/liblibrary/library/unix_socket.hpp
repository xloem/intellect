#pragma once

#include <library/string.hpp>
#include <library/stackvector.hpp>

namespace library {

class unix_socket
{
public:
	unix_socket(string pathname, bool server);
	unix_socket(int socket);
	~unix_socket();

	void send_fds(stackvector<int,253> const & fds);
	stackvector<int,253> recv_fds();

	string const pathname;
	int const socket;
};

} // namespace library
