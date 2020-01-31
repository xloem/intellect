

// let's make file=notepad.

void loadfile(std::string file, std::string notepad)
{
	ifstream f(file);
	intellect::level2::notepad() = newnotepad(notepad);
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
