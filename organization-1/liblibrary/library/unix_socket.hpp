#pragma once

#include <library/string.hpp>
#include <library/stackvector.hpp>

namespace library {

class unix_socket
{
public:
	unix_socket(string pathname, bool server);
	unix_socket(int socket);

	unix_socket accept(); // for server

	void send_fds(stackvector<int,253> const & fds);
	stackvector<int,253> recv_fds();

	void close();
	~unix_socket();


private:
	string pathname;
	int socket;
	bool server;
};

} // namespace library
