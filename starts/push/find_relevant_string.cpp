#include "loaded_relevance.hpp"

#include <cassert>

void find_relevant_string(Context & context)
{
	auto input = context.relevance["input"];
	auto contextuality = context.relevance["context"];

	auto inputcontext = input + "-" + contextuality + "-relevancy";

	std::array<decltype(Context::relevance)*,3> ideas = {
		&contexts[inputcontext + "-1"].relevance,
		&contexts[inputcontext + "-2"].relevance,
		&contexts[inputcontext + "-3"].relevance
	};

	if (!ideas[0]->empty()) {
		auto it = ideas[0]->begin();
		context.relevance["output"] = it->second;
		ideas[1]->insert(*it);
		ideas[0]->erase(it);
		return;
	}

	if (contexts[inputcontext].relevance.empty()) {
		contexts[inputcontext].relevance["seen-before"] = "yes";
		ideas[1]->insert({input,input});
		ideas[1]->insert({contextuality,contextuality});
	}

	size_t index = 1;
	while (ideas[0]->empty()) {
		if (ideas[index]->empty()) {
			++ index;
			assert(index < ideas.size());
			continue;
		}
		auto & oldideas = *ideas[index];
		for (auto & pair : oldideas) {
			auto & oldidea = pair.second;
			if (!contexts.count(oldidea)) { continue; }

			auto & other = contexts[oldidea].relevance;

			// way #2: find context keys
			for (auto pair : other) {
				ideas[0]->insert({pair.first,pair.first});
			}

			// way #3: find context values based on using contents as keys and other contents as contexts
			for (auto & pair : oldideas) {
				auto & otheridea = pair.second;
				if (other.count(otheridea)) {
					auto & information = other[otheridea];
					ideas[0]->insert({information,information});
				}
			}
		}
		for (auto it = oldideas.begin(); it != oldideas.end();) {
			typename std::string::size_type idx, lastidx = ~0;
			auto oldidea = it->second;
			// way #1: break hyphens
			if ((idx = oldidea.find("-")) != std::string::npos) {
				do {
					auto str = oldidea.substr(lastidx + 1, idx);
					ideas[0]->insert({str, str});
					lastidx = idx;
				} while((idx = oldidea.find("-")) != std::string::npos);
				break;
			}


			if (index + 1 < ideas.size()) {
				ideas[index + 1]->insert(*it);
				// error here, deal with later if matters
			}
			it = oldideas.erase(it);
		}

		// for LATER consideration: we could use associative pointers instead of just strings, for more flexibility
		// 	-> you might be able to use contextual association.  might just be pointer-maps.
		// 	   looks efficient to walk.
		// we are also going to want to find strings that contain our ideas as substrings. [MAYBE LATER THOUGH]
		// 	-> one approach is a comparison or lookup function
		// 	-> another approach is to build links in named contexts to parts of strings.
		// 	-> e.g. maybe-relevance: [dehyphenation]:[maybe-it-will-work]
	}

	return find_relevant_string(context);

}
