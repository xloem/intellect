// This Intellect Approach Can Easily Preserve All Life Within It
// you just don't delete the programs you generate, instead replacing their habits with references to reuse
//
// let's make it!

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <sys/stat.h>

using namespace std;

// problem: how does a .cpp file reference another file with a number
// answer: use #includes or interpret the whole shebang as numbers

// please provide for handling a parameter next.
//
//      concept: dynamic values aquirable from inside code, i.e. what-number-called-me what-number-comes-after-me
//      thinking the code would likely evolve to handle some inputs differently
unsigned long new_number = 0;

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
		cout << "stat of " << ofname << " yields " << stat(ofname.c_str(), &sb) << endl;
	}

        {
                ofstream outfile(ofname);
                vector<unsigned long> nums;
                while (!cin.eof()) {
                        unsigned long num;
                        cin >> num;
                        nums.push_back(num);
                }
                // when a file runs,  it has numbres on input, it also has numbers equal to it
                // we want to generate run-code with new numbers from input
                // so we generate something with numbers equal to it, and output that
                // we have one ref for the whole shebang
                outfile << "#ifdef NUMCT" << endl
                        << "  #undef NUMCT" << endl
                        << "#endif";
                outfile << "#define NUMCT" << " " << nums.size() << endl;
                outfile << "#ifdef NUMS" << endl
                        << "  #undef NUMS" << endl
                        << "#endif";
                outfile << "#define NUMS [";
                for (size_t index = 0; index < nums.size(); ++ index)
                {
                        if (index > 0) outfile << ",";
                        outfile << nums[index];
                }
                outfile << "]" << endl;
                for (size_t index = 0; index < nums.size(); ++ index)
                {
                        outfile << endl << "/* " << nums[index] << " */" << endl;
                        outfile << "#if defined(IDX)" << endl
                                << "  #undef IDX" << endl
                                << "#endif" << endl;
                        outfile << "#define IDX " << index << endl;
                        outfile << "#if defined(NUM)" << endl
                                << "  #undef NUM" << endl
                                << "#endif" << endl;
                        outfile << "#define NUM " << nums[index] << endl;
                        string fname = to_string(nums[index]) + ".cpp";
                        ifstream code(fname);
                        size_t ctrd = -1;
                        while (ctrd != 0) {
                                char buf[256];
                                ctrd = code.rdbuf()->sgetn(buf, sizeof(buf));
                                outfile.rdbuf()->sputn(buf, ctrd);
                        }
                }
        }
// read numbers inputs
// open files having the numbers as the names
// cat them all to a gcc process
// execute
// run the output
}

// karl obvious knows what he was doing ...
//      ... we were just helping him out of his issue
// [do you want another one karl?]
// what things make / not make issue?
//      karl says everything makes an issue; this seems accurate
//
