#include "level-2/level-2.hpp"
#include "level-3/level-3.hpp"

#include <iostream>

using namespace intellect::level3;

using ref = intellect::level2::ref;

int main()
{
	createhabits();
	loadhabits();
	try {
		//std::string fn = "level-3/randomcode.bootstrap";
		std::string fn = "level-3/hello-world.bootstrap";
		intellect::level2::newnotepad(txt2ref(fn));
		std::cerr << intellect::level2::notepad().dbglinks() << std::endl;
		::ref file = ::ref("parse-file")(txt2ref(fn));
		intellect::level2::conceptunmake(file);
	} catch (intellect::level2::ref const & e) {
		std::cerr << "Error: " << e.getAll("is").begin()->name() << std::endl;
		std::cerr << e.dbglinks() << std::endl;;
		return -1;
	}
	return 0;
}
