// from 'HOW TO QUICKLY BUILD A GALAXY' in community-notes.txt

// we quickly ran into an expected issue of not properly using memory mapping.

#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
using namespace std;
class memory_runner
{
public:
	int8_t * memory;
	size_t length;
	int urandomfd;
	memory_runner()
	{
		struct stat sb;
		urandomfd = open("/dev/urandom", O_RDONLY);
		int fd = open("test"/*"/dev/fb0"*/, O_RDWR);
		size_t width = 1366;
		size_t height = 768;
		size_t bpp = 32;
		length = width * height * bpp / 8;
		cout << length << endl;
		memory = (int8_t*)mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		if (memory == MAP_FAILED) { perror("mmap"); throw "=/"; }
	}
	void run() {
		read(urandomfd, memory, length);
		/*for (int i = 0; i < length; ++ i) {
			memory[i] = (i % 8) - 4; // TODO: random
			//cout << memory[i] << " ";
			//if (i  % 16 == 0) { cout << endl; }
		}*/
		while (true) {
			for (int i = 2; i < sizeof(length)-2; ++ i) {
				process(i);
			}
		}
	}
	void process(int address, bool base = true) {
		// this could get more reusable by storing the argument first
		int8_t * pointer = &memory[address];
		int8_t instruction = *pointer;
		int8_t * argument1;
		if (instruction <0) {
			argument1 = pointer - 1;
			instruction = -instruction;
		} else {
			argument1 = pointer + 1;
		}
		// this makes too much wind in simulation but likely there is
		// possible wind-life in some form.  may be hard to see.
		switch(instruction %4) {
		case 0:
			return;
		case 1:
			*argument1 ++;
			break;
		case 2:
			*argument1 --;
			break;
		case 3:
			if (base) { process(memory[*argument1], false); }
			break;
		}
	}
};

int main()
{
	memory_runner runner;
	runner.run();
}
