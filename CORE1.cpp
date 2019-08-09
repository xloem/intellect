// This Intellect Approach Can Easily Preserve All Life Within It
// you just don't delete the programs you generate, instead replacing their habits with references to reuse
//
// let's make it!

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <sys/stat.h>

using namespace std;

// LET'S USE THE TRAUMA THERAPY INTELLECT APPROACH.
// The core is that we focus our energy on handling our failures.
// This can involve taking the time to understand them, or entering a trauma state and asking the user for help.
// 	Trauma state must be logged.  Solution must be stored.  Can just be creation of a trauma-handling pattern, I suppose.
// 	Understanding will likely involve breaking behavior into steps
// 		maybe then backtracking from a failure to the steps that caused it, and opening those steps up into substeps

// GDB HAS A MACHINE INTERFACE MODE THAT COULD BE USED TO MANAGE EXECUTION WITH LEARNING

// DYLOAD NOTES:
//
// 	#define _GNU_SOURCE
// 	#include <dlfcn.h>
// 	// LM_ID_NEWLM makes a new isolated namespace
// 	void *handle = dlmopen(LM_ID_NEWLM, "./path.so", RTLD_NOW); // NULL -> failure
// 		link with -ldl
// 		links .so with -shared -fPIC
// 	void *addr = dlsym(handle, "symbolname"); // NULL -> failure
// 	int res = dlclose(handle); // 0 -> success (dlerror)
//
//	extern "C" // place before exports to demangle their symbols
//
// 	__attribute__((constructor)) and __attribute__((destructor)) functions
// 		are called on load/unload
// 		alternatively use _init() and _fini_() and pass -nostartfiles to gcc
// 		atexit() is preferred

// problem: how does a .cpp file reference another file with a number
// answer: use #includes or interpret the whole shebang as numbers

// please provide for handling a parameter next.
//
//      concept: dynamic values aquirable from inside code, i.e. what-number-called-me what-number-comes-after-me
//      thinking the code would likely evolve to handle some inputs differently
unsigned long new_number = 1;

int main()
{
        string ofname;
        unsigned long ofnum;
	{
		struct stat sb;
	        do
	        {
	                ofnum = new_number++;
	                ofname = to_string(ofnum) + ".cpp";
	        } while (-1 != stat(ofname.c_str(), &sb));
	}

        {
                ofstream outfile(ofname);
                vector<string> vals;
		while (true) {
			string val;
                        cin >> val;
			if (val == "") break;
                        vals.push_back(val);
                }
                // when a file runs,  it has numbers on input, it also has numbers equal to it
                // we want to generate run-code with new numbers from input
                // so we generate something with numbers equal to it, and output that
                // we have one ref for the whole shebang
                outfile << "#if !defined(VALCT)" << endl;
                outfile << "  #define VALCT" << " " << vals.size() << endl;
                outfile << "  #define VALS [";
                for (size_t index = 0; index < vals.size(); ++ index)
                {
                        if (index > 0) outfile << ",";
                        outfile << "\"" << vals[index] << "\"";
                }
                outfile << "]" << endl;
    		outfile << "#endif" << endl;
                for (size_t index = 0; index < vals.size();)
                {
                        outfile << endl << "/* " << vals[index] << vals[index+1] << " */" << endl;
                        outfile << "#if defined(IDX)" << endl
                                << "  #undef IDX" << endl
                                << "#endif" << endl;
                        outfile << "#define IDX " << index << endl;
                        outfile << "#if defined(VAL)" << endl
                                << "  #undef VAL" << endl
                                << "#endif" << endl;
                        outfile << "#define VAL \"" << vals[index] << "\"" << endl;
			outfile << "#define ARG \"" << vals[index+1] << "\"" << endl;
                        string fname = vals[index] + ".cpp";
                        ifstream code(fname);
                        size_t ctrd = -1;
                        while (ctrd != 0) {
                                char buf[256];
                                ctrd = code.rdbuf()->sgetn(buf, sizeof(buf));
                                outfile.rdbuf()->sputn(buf, ctrd);
                        }
			index += 2;
                }
		{
			// TODO: hash code to reuse exact stuff, somehow
			string cmd = "g++ -ggdb -std=c++11 -o " + ofname + ".exec " + ofname;
			int status = system(cmd.c_str());
			if (status != 0) throw status;
		}
		// execute output, replacing process, to loop.  use same input.  it should represent our own code.
		// cmd = "./" + ofname + ".exec " <

        }
// read numbers inputs
// open files having the numbers as the names
// cat them all to a gcc process
// execute <-
// run the output <-
}

// need input to pass to output
// propose pass our input & output to it
// so, a number for what we are,
// and a number for what we ran.
//
// also idea of treating whats-next as data
// makes it a little harder to .. make a program out of stuff
// we could load a building-program number
// and it could treat them differently, taking each one as a program-piece
//

// karl obvious knows what he was doing ...
//      ... we were just helping him out of his issue
// [do you want another one karl?]
// what things make / not make issue?
//      karl says everything makes an issue; this seems accurate
//
