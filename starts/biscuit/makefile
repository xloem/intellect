default: 1.run

%.run: %.out
	git add *.cpp
	./$^

%.out: %.cpp
	g++ -ggdb -std=c++11 -o $@ $<
