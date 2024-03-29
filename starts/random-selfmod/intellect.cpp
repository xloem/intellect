// The extreme of Trial Exploration and Self Coding
// brainstorm_by(assembly code,running it)

// the advantage of assembly code is that it is very easy to serialize
// the advantage of random self-modification is that it is very easy to make
// propose this is the first AI karl makes, and it runs in a safe isolated box, on its own
// 	how to judge success? does-not-crash?
// 	have to teach it learning.  first is to not crash.  then is to output requested string, likely copied from request.  run it until it outputs requested string.
// 	it's stupid enough to be safe if run in an isolated box.
// due to how fast it should be to make this, let's finish barebones first, and then merge.  XO computer likely doesn't have read-only-execute problem.

// the below is a brief attempt to make an intellect by random self-modification.  it does not run.
// DO NOT RUN THIS UNLESS YOU KNOW WHAT YOU ARE DOING

#include <cstdint>
#include <vector>
#include <set>
#include <iostream>

#include <signal.h>
//#include <sys/mmap.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

// Signal Catching has the problem of how to resume execution generally.
// So we propose to fork a new process, and switch to it if it works.
// This will take forming a simple communication method between the processes.
// We can also compile a whole new program to learn, copying our source.

// TODO: Currently implementing fork/exec.  changed approach 

// TODO: make sure to run inside a vm once it does something

// TODO: try out rewriting and recompiling code, rather than forking ram

class RamGetter
{
public:
	RamGetter()
	{
		randfile = open("/dev/urandom", O_RDONLY);
		if (randfile <= 0) throw 0;
	}
	~RamGetter()
	{
		close(randfile);
	}
	void * getRandAddr(size_t len)
	{
		// TODO: this is a heuristic for picking an inner memory location, and learning what doesn't work; could be used in learning concept
		void * blkp;
		do {
			uint32_t blk;
			int ct = read(randfile, &blk, sizeof(blk));
			if (ct < sizeof(blk)) throw 0;
			blkp = reinterpret_cast<void*>(blk);
			// fork here.  check length
			pid_t forkpid = fork();
			if (forkpid == -1) throw "failed to fork";
			if (forkpid == 0) {
				// child: test ram
				volatile uint8_t tst;
				for (size_t i = 0; i < len; ++ i) {
				 tst = ((uint8_t*)blkp)[i];
				}
			} else {
				// parent: wait for child
				int status;
				wait(&status);
				if(WIFCONTINUED(status)) {
					// child succeeded, so we can stop
					// TODO: need a way to indicate child success
					exit(0);
				} else {
					// child failed, so we keep going, but TODO: failure is not stored
					continue;
				}
			}
		} while (isbad(blkp));
		return blkp;
	}
	void markbad(void *bad)
	{
		bad = (void*)((long)(bad) & ~0xfff);
		badpages.insert(bad);
		cout << "Bad: " << bad << endl;
	}
	bool isbad(void *chk)
	{
		chk = (void*)((long)(chk) & ~0xfff);
		return badpages.count(chk);
	}

private:
	int randfile;
	set<void*> badpages;
	// TODO: some concern that the badpages set will allocate on the heap.
	// maybe let's use a stack-based storage back for it (allocator)
};


uint8_t someram[0x10000000]; // when this is removed, heap moves to 64-bit?
struct instruction {
	void * verb;
	void * object1;
	void * object2;
	void * attr;
	void * result;
};

int main() {
	// reference process memory
	// all of it, please
	RamGetter selfmem; // an object to access RAM with

	// TODO: try making an instruction list

	void * mem = 0;
	// make a loop
	while (true) {
		// TODO: move fork/exec into this loop, outside of RamGetter

		// we propose copying some blocks to other blocks, randomly
		// need blocksize, how about 4 // TODO: that's kinda small, maybe random instead?
		void *blk1, *blk2;
		blk1 = selfmem.getRandAddr(4);
		blk2 = selfmem.getRandAddr(4);
		memmove(blk1, blk2, 4);
		
		// continue the loop forever
		// run it
	}
	// end when the loop is done =)
	
	// /proc/pid/maps shows the addresses of allocated memory
	// /proc/pid/mem provides access to this memory
	// 	^-- will error when wrong part is read
	// 	just load straight; error is recoverable, just a refusal
	// 	no need to map file, can just catch segfaults?
	
	/*
	FILE *maps;
	char pathbuf[1024];
	snprintf("/proc/%d/maps", sizeof(pathbuf), pid);
	open(pathbuf, O_RDONLY);

	snprintf("/proc/%d/mem", sizeof(pathbuf), pid);
	void * procmem = mmap(0, length, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_SHARED
	*/
	/*
	try {
		int e = *(int*)39;
	} catch(...) {
		throw;
	}
	*/
}
