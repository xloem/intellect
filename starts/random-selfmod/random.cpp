// let's try to make it very simple

// YOU NEED TO USE mprotect() TO LET MEMORY BE EXECUTABLE.

#include <fcntl.h>
#include <signal.h>
#include <sys/mman.h>
#include <unistd.h>

// 1. write a function to randomly put data in a buffer
// 		[apparently one loaded from a file]

#define MAPFNAME "random.bin"

pid_t child = 0;
bool child_terminated = false;
void handleSigChld(int)
{
	child_terminated = true;
}

static unsigned long long * count;

void random(int mapfd, int randfd, void *addr)
{
	pid_t pid = getpid();

	// *count is shared and stored
	for (*count = 0; ; ++ *count) {
		unsigned long long old_count = *count;

		// if successful child, update & kill parent
		if (getpid() != pid) {
			msync(addr + pos, size, MS_SYNC | MS_INVALIDATE);

			kill(pid, SIGTERM);
			pid = getpid();

			// add data to git.
			system("git add " MAPFNAME);
			system("git commit -m selfmod");
		}
		
		// handle crashes with fork()
		child = fork();
		if (child == 0) {
			// we are new child process
			// self-modify by 1-8 bytes
			uint16_t pos;
			uint8_t size;
			read(randfd, &pos, sizeof(pos));
			read(randfd, &size, sizeof(size));
			size = (size & 0x7) + 1; // self-modify by 1-8 bytes
			read(randfd, addr + pos, size);
			// loop
		} else {
			// old process: make sure new process succeeds within 1s
			time_t then = time(0);
			for (;;) {
				if (child_terminated || time(0) > then) {
					// child did not succeed
					kill(child, SIGTERM);
					child_terminated = false;
					// try again
					break;
				}
				if (old_count < *count + 1) {
					// incremented count by 1: success; hand off to child
					return;
				}
			}
		}
	}
}

void random_tail() {
}

void main()
{
	// handle SIGCHLD by setting a global flag
	struct sigaction sigchld;
	sigchld.sa_handler = handleSigChld;
	sigchld.sa_flags = SA_NOCLDWAIT;
	sigaction(SIGCHLD, &sigchld, NULL);

	int fd = open(MAPFNAME, O_RDWR);
	if (fd == -1 && errno == ENOENT) {
		// does not exist yet
		fd = open("random.bin", O_RDWR | O_CREAT);
		// 2. seed the buffer with the function that puts random data in it
		count = 0;
		write(fd, &count, sizeof(count));
		write(fd, &random, &random_tail - &random);
	}

	int randfd = open("/dev/random", O_RDONLY);

	// a buffer of 1 gig working space; random data will fill it
	void *buf = mmap(NULL,
	                 0x40000000,
	                 PROT_EXEC | PROT_READ | PROT_WRITE,
	                 MAP_SHARED | MAP_32BIT,
	                 fd,
	                 0;

	// 3. execute the buffer
	void (*buf_as_func)(void*) = (void*)((uint8_t*)buf + count);
	buf_as_func(mapfname, mapfd, randfd, buf);
}
