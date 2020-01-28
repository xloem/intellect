#include "level-2/level-2.hpp"
#include "level-3/level-3.hpp"

using namespace intellect::level3;

using ref = intellect::level2::ref;

int main()
{
	loadhabits();
	::ref file = ::ref("parse-file")(txt2ref("level-3/randomcode.bootstrap"));
	intellect::level2::conceptunmake(file);
	return 0;
}
