// int 3 is a 1-byte instruction.  sends a signal.
// the norm is to replace a part of code with the 1-byte instruction.

#include <vector>

extern std::vector<void *> functions;

void test()
{
}

#include <signal.h>
void handler(int signal, siginfo_t * siginfo, void * _details)
{
	// siginfo may contain timing information
	ucontext_t details = _details; // see sigreturn(2) and getcontext(3), contains registers
}
void sethandler()
{
	stack_t signal_stack;
	signal_stack.ss_sp = new uint8_t[SIGSTKSZ];
	signal_stack.ss_size = SIGSTKSZ;
	signal_stack.ss_flags = 0;
	sigaltstack(&signal_stack, NULL);
	struct sigaction action;
	action.sa_sigaction = trap;
	sigemptyset(&action.sa_mask);
	//                            let the signal be triggered by its handler
	//             use sa_sigaction    |  use alternate stack with sigaltstack(2)
	//                v                v           v
	action.sa_flags = SA_SIGINFO | SA_NODEFER | SA_ONSTACK;
	int result = sigaction(SIGTRAP, &action, NULL);
}

int main()
{
	sethandler();
	asm("int $03");
	// on arm: __breakpoint(42) or asm(".inst 0xd4200000") or thumb: asm(".inst 0xde01") or arm: asm(".inst 0xe7f001f0")
	// platform independent: raise(SIGTRAP)
	return 0;
}

std::vector<void*> functions = {
	(void*)test,
	(void*)main
};
