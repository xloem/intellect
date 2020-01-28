
#include "../level-2/funcs.hpp"
#include "../level-2/ref.hpp"
#include "../level-2/sugar.hpp"

#include <string>
#include <fstream>

// let's make file=notepad.

using ref = intellect::level2::ref;

void loadfile(std::string file, std::string notepad)
{
	std::ifstream f(file);
	intellect::level2::notepad() = intellect::level2::newnotepad(notepad);
	std::string word;

}

void storefile(std::string file, std::string notepad)
{
}

void createserialisationhabits()
{
	ahabit(loadfile, ((file, f), (notepad, n)), {
		loadfile(f.val<std::string>(), n.name());
	});
	ahabit(storefile, ((file, f), (notepad, n)), {
		storefile(f.val<std::string>(), n.name());
	});
}
