#include <library/criu.hpp>

#include <library/string.hpp>
#include <library/time.hpp>
#include <library/unix_socket.hpp>

#include <filesystem>

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <criu/criu.h>

namespace library {

static int handle(int result)
{
	if (result < 0) {
		string message;
		switch (result) {
		case -EBADE:
			message = "RPC has returned fail: " + string(errno);
			if (errno == 0) {
				message += " (you might need to chmod u+s /usr/sbin/criu or such)";
			}
			break;
		case -ECONNREFUSED:
			message = "Unable to connect to CRIU: " + string(strerror(errno));
			break;
		case -ECOMM:
			message = "Unable to send/recv msg to/from CRIU: " + string(strerror(errno));
			break;
		case -EINVAL:
			message = "CRIU doesn't support this kind of request.  You should probably update CRIU.";
			break;
		case -EBADMSG:
			message = "Unexpected response from CRIU.  You should probably update CRIU.";
			break;
		default:
			message = "Unexpected CRIU return value: " + string(result);
		}
		throw std::runtime_error(message.std());
	}
	return result;
}

criu::criu()
{
	criu_opts = 0;
	handle(criu_local_init_opts((::criu_opts**)&criu_opts));
	//criu_local_set_leave_running((::criu_opts*)criu_opts, true); // forking instead
	criu_local_set_shell_job((::criu_opts*)criu_opts, true);
	criu_local_set_log_level((::criu_opts*)criu_opts, 4);
}

// note this returns out of restoration here
char const * criu::dump(bool & restored, char const * label, char const * path)
{
	static thread_local string dirname;
	dirname = "criu-" + string(label) + "-" + timestamp_iso();
	string wholedirname = string(path) + "/" + dirname;
	std::filesystem::create_directories(wholedirname.std());

	// fork before dumping; then maybe can restore as child
	int forkpid = fork();
	if (forkpid) {
		// child is dumping
		restored = false;
		int result;
		waitpid(forkpid, &result, 0);
		handle(WEXITSTATUS(result));

		std::filesystem::remove(std::string(path) + "/criu-latest");
		std::filesystem::create_directory_symlink(dirname.std(), std::string(path) + "/criu-latest");
		stderr::line("... process dumped to " + wholedirname);
	} else {
		// we are child, dumping or restoring
		int fd = open(wholedirname.c_str(), O_DIRECTORY);
		criu_local_set_images_dir_fd((::criu_opts*)criu_opts, fd);
		criu_local_set_log_file((::criu_opts*)criu_opts, "dump.log");
		
		stderr::line("... dumping process to " + wholedirname);
		
		close(0); close(1); close(2);
		int result = criu_local_dump((::criu_opts*)criu_opts);
		close(fd);
		if (1 == result) {
			// restoring
			restored = true;
			
			// parent sends fds over unix socket
			unix_socket fd_server(getpid(), true);

			// when transferring, the kernel can allocate fds that actually clobber 0-2, so those are reassigned first
			int nullfd= open("/dev/tty", O_RDWR);
			dup2(nullfd, 0); dup2(nullfd, 1); dup2(nullfd, 2);

			// receive fds and assign to standard handles
			auto fds = fd_server.recv_fds();
			close(0); close(1); close(2); close(nullfd);
			dup2(fds[0], 0); dup2(fds[1], 1); dup2(fds[2], 2);
			close(fds[0]); close(fds[1]); close(fds[2]);

			stdout::line("stdout inside restored");
			stderr::line("... process restored from " + wholedirname);
		} else {
			// dump failure or success
			exit(result);
		}
	}

	return dirname.c_str();
}

void criu::restore(char const * name, char const * path)
{
	if (nullptr == name) {
		name = "criu-latest";
	}
	string wholedirname = string(path) + "/" + name;
	
	int fd = open(wholedirname.c_str(), O_DIRECTORY);
	if (-1 == fd) {
		perror("criu open");
		throw std::runtime_error(("failed to open criu path " + wholedirname).std());
	}
	criu_local_set_images_dir_fd((::criu_opts*)criu_opts, fd);
	criu_local_set_log_file((::criu_opts*)criu_opts, "restore.log");
	stderr::line("... restoring process from " + wholedirname);
	int pid = handle(criu_local_restore_child((::criu_opts*)criu_opts));
	stderr::line("... process restored as pid " + string(pid));
	close(fd);

	{
		// child runs quick unix socket server to receive fds
		/*do {
			try {*/
				unix_socket fd_client(pid, false);
				fd_client.send_fds({0, 1, 2});
	/*		} catch (std::runtime_error &) {
				continue;
			}
		} while (false);*/
	}

	int ret;
	waitpid(pid, &ret, 0);
	if (WEXITSTATUS(ret)) {
		throw WEXITSTATUS(ret);
	}
}

criu::~criu()
{
	criu_local_free_opts((::criu_opts*)criu_opts);
	criu_opts = nullptr;
}

} // namespace library
