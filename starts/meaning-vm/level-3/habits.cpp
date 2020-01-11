#include "habits.hpp"

#include "../level-2/habits.hpp"
#include "../level-2/sugar.hpp"

using namespace intellect::level2;

namespace intellect {
namespace level3 {

void createhabits()
{
	intellect::level2::createhabits();

	ahabit(random-link, ((concept, c)), {
		size_t num = intellect::level2::rand(0.0, c.ptr()->links.size());
		size_t ct = 0;
		ref link = makeconcept();
		ref("first-link-entry")(link, c);
		while (ct < num) {
			ref("next-link-entry")(link);
			++ ct;
		}
		result = link;
	});
}

}
}
