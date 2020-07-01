#include "habits.hpp"

#include "bootstrap-parser.hpp"

#include "../level-2/habits.hpp"
#include "../level-2/sugar.hpp"

#include <iostream>


using namespace intellect::level2;

namespace intellect {
namespace level3 {

void createhabits()
{
	intellect::level2::createhabits();

	/*
	ahabit(to, ((source, s)), {
		result = s;
	});
	*/
	ahabit(error, ((error, e)), {
		// e is name of error
		ref er = noteconcept();
		er.link("is", e);
		er.link("habit", ctx.get("outer-context").get("self"));
		for (auto l : ctx.links()) {
			if (l.first == "outer-context") { continue; }
			if (l.first == "root") { continue; }
			if (l.first == "active-state") { continue; }
			if (l.first == "error") { continue; }
			if (l.first == "quiet") { continue; }
			if (l.first == "self") { continue; }
			er.link(l.first, l.second);
		}
		throw er;
	});

	ahabit(same, ((concept-a, a), (concept-b, b)), {
		result = (a == b);
	});

	ahabit(random-link, ((concept, c)), {
		size_t num = intellect::level2::sugar::rand(0.0, c.ptr()->links.size());
		size_t ct = 0;
		ref link = noteconcept();
		ref("first-link-entry")(link, c);
		while (ct < num) {
			ref("next-link-entry")(link);
			++ ct;
		}
		result = link;
	});
	
	ahabit(random-of, ((concept, c), (type, t)), {
		auto linksall = c.ptr()->getAll(t);
		std::vector<ref> links;
		for (auto it = linksall.begin(); it != linksall.end(); ++ it) {
			links.push_back(*it);
		}
		size_t num = intellect::level2::sugar::rand(0.0, links.size());
		result = links[num];
	});

	ahabit(say-text, ((text, t)), {
		std::cout << ref2txt(t);
	});
	ahabit(say-name, ((concept, c)), {
		std::cout << c.name();
	});

}

}
}