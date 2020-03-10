// STATUS: finds very minimal relevancy, and runs.  tests are hardcoded.  relevancy-ways are hardcoded.  links are global strings.

#include "loaded_relevance.hpp"

#include <cassert>

#include <iostream>

std::vector<std::string> string_split(std::string const & string, std::string const & delimiter)
{
	std::vector<std::string> substrings;
	size_t lastidx = 0, idx;
	while ((idx = string.find("-", lastidx)) != std::string::npos) {
		substrings.push_back(string.substr(lastidx, idx - lastidx));
		lastidx = idx + 1;
	}
	substrings.push_back(string.substr(lastidx));
	return substrings;
}

bool already_tried(std::string const & context, std::string const & idea)
{
	auto & set = sets[context];
	auto string = context + "-" + idea;
	if (set.count(string) == 0) {
		std::cerr << " consider '" << context << "' '" << idea << "' ..." << std::endl;
		set.insert(string);
		return false;
	} else {
		return true;
	}
}

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
	// the problem is not that we are returning many values, but that we are rereviewing the nearness of values, forever.
	//decltype(Context::relevance)* old_ways = &contexts[inputcontext + "-old-ways"];
		// TODO: prevent cycling by storing thing-nearness in old_ways, and not repeating.
		// you could also make a context for each thing-context-nearness.

	if (!ideas[0]->empty()) {
		auto it = ideas[0]->begin();
		context.relevance["output"] = it->second;
		ideas[1]->insert(*it);
		ideas[0]->erase(it);
		return;
	}

	// propose: 3 states: helps-find, good-result, and already-tried

	if (contexts[inputcontext].relevance.empty()) {
		contexts[inputcontext].relevance["seen-before"] = "yes";
		ideas[0]->insert({input,input});
		ideas[0]->insert({contextuality,contextuality});
	}

	size_t index = 1;
	while (ideas[0]->empty()) {
		if (ideas[index]->empty()) {
			++ index;
			assert(index < ideas.size()); // it sounds like our ideas are getting exhausted before being used up.
			continue;
		}
		auto & oldideas = *ideas[index];
		for (auto & pair : oldideas) {
			auto & oldidea = pair.second;
			if (!contexts.count(oldidea)) { continue; }

			auto & other = contexts[oldidea].relevance;

			// way #2: find context keys
			for (auto pair : other) {
				if (already_tried(inputcontext, pair.first)) { continue; }
				ideas[0]->insert({pair.first,pair.first});
				return find_relevant_string(context);
			}

			// way #3: find context values based on using contents as keys and other contents as contexts
			for (auto & pair : oldideas) {
				auto & otheridea = pair.second;
				if (other.count(otheridea)) {
					auto & information = other[otheridea];
					if (already_tried(inputcontext, information)) { continue; }
					ideas[0]->insert({information,information});
					return find_relevant_string(context);
				}
			}
		}
		for (auto it = oldideas.begin(); it != oldideas.end();) {
			typename std::string::size_type idx, lastidx = 0;
			auto oldidea = it->second;
			// way #1: break hyphens
			if (oldidea.find("-") != std::string::npos) {
				for (auto & substring : string_split(oldidea, "-")) {
					if (!already_tried(inputcontext, substring)) {
						ideas[0]->insert({substring, substring});
						// stopping at 1st idea found supports goal of checking with use
						// on value of information.  [to know where to store it]
						// see EXPANSION-ATTEMPT down in file
						return find_relevant_string(context);
					}
				}
			}


			std::cout << "Dropping '" << it->second << "' from level " << index << std::endl;
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
}

/*
EXPANSION ATTEMPT (attempt to consolidate to clarity below it, also labeled with same label)

make find-relevance stop and reconsider whenever it found a new relevant concept helped build access to its
proper structure.
        [karl is focusing on using above clearer expression to build more clear descriptions of a process for
         working with stored state on finding relevance]

                something about a hierarchy of how-helpful a concept might be
                and combining things that are more-helpful together to see if any results are more-helpful
                it looks like you want to place ideas at different spots on that hierarcht than always at the top.
                        means feedback on helpfulness.

                                feedback-on-helpfulness: how does it relate to place-on-hierarchy
                                        if we can move topmost concepts down quickly (or never even place them at the top)
                                        then we can expand inner-product with more relevance.
                                                (for only-look-at-and-calculate-parts-of-inner-product-that-help)
                        so, for investment it would be best if we could check if a concept is helpful or not before
                        deciding where to place it.
                        for current-situation, _any_ information where system can adjust value of a concept is helpful.
                                presently just-used concepts are going to be either in level 0 or level 1.
                                        if they are helpful, we want them in level 1 or 0
                                        if they are not-helpful, we want them lower
                                                we actually have n-depth to handle deep exploration.
                                                [it looks like karl's brain has 3-depth, with depth=3 only used once depth=2
                                                 is migrated to depth=1 from exhaustion of depth=0]^B
                                                [karl's relevancy is depth=1 first.  depth=0 is active memory, and there are
                                                 links between brainstorming and active memory.]
                                                        [so it would be clearer for karl if the system didn't hold depth=0]
                                                [above are very rough, to solve goal with limited resources]
EXPANSION ATTEMPT {{this was expanded a little more deeply in RESUMMARIZATION below.  there is more expansion available, unwritten.}}

make find-relevance stop and reconsider whenever it found a new relevant concept helped build access to its
proper structure. {{after this thorough investigation, a lot of the introspective links feel altered, now}}

Summary of Relevancy Process, more expansion towards goal:
        [as relevancy, it sounds like we have a 3-depth list of ideas]
        [and the user of us advises us on what to do with ideas in the upper level, or new ideas we find in lower levels]
                [it sounds like we just offer our upper level to the user, read-only]
                [and they can decide what to expand more]

                        [how is third depth managed?]
                                [third depth is ideas that didn't work, that haven't had their inner product fully expanded
                                 with any new ideas.  since new ideas are new, that means all used-up ideas.]
                                [ideas in third level are associated with ideas in second level that they might help in combination with]
                                [ideas in second level are in the midst of having their inner product expanded with ideas in the
                                 first level]
                [inner product is made between concepts in the store, with relevant wayness, first level first]
                        [so we also have relevancy on combination-habits] {this is in the design queue, not currently present, not valued focus atm}
                [new concepts are found using relevant-first similarity habits, with creativity to come up with new]
                        {for this we have things like similar-string, or usually-useful-after-the-other-is-useful}

        [this process is the most important process we know for making intellect based on human cognition.]

RESUMMARIZATION 2020-03-08 0403A EST
	Karl is describing a quick-model of relevant access using 3 levels.
	When a possibly-relevant idea is found, it is classified as helpful or nonhelpful.  Helpful ideas go in level 1.
	Nonhelpful ideas go in level 2.
	Users of the relevancy have access to level 1, and can trigger it to produce more ideas.
	A primary use of the relevancy is to combine level 1 with itslef and with level 2 to find more ideas, via creative methods.
	When an idea has exhausted in use, it is placed in level 3, for use when highly determined to think of something new.
			[when level 1 is used up, migreate level 2 to level 1, and level 3 to level 2, karl says]
	
	The above is a quick summary of a process that is actually a deep, complex graph [a tree].  Each 'level' is 'distance' in the
	graph from the focus [the root of the tree].  Each
		[the relevancy process is viewing memory as a tree, by considering only links it/they value[s]]
		Each parent of a node in the tree is a more-helpful idea.
		Ideas deeper in the tree are helpful in combination with their parents.
			So, ideas can have multiple parents.
				Each child-parent link is associated with the kind[s] of combination that are likely to produce
				better ideas.

ABSTRACT NOTES:
	[with trees we can truly have N levels, and I see now how relevency is how memory is stored, accessed, used, and reinforced]
	[there is general relevency with combination-ways [creativity-habits] too.  first good ones are used, then lower-priority ones]
		[by 'general relevancy' we mean the whole system: wait until tried all good ones before trying lower-priority ones]
		[but obviously internally a mature system would try the best habits first on what they are relevant too]
			[[[wondering on a link-structure that could represent simplified-and-complex-relevancy]]]
				[[[similar to storage of groups of objects same as storage of 1 object]]]
					[[[above line just means our core processes use relevancy, and provide for multiple
					   options or uses everywhere]]]

RELEVANCY HEURISTIC 2
Relevancy-2 works with:
1) A cluster of relevant ideas
    with links to relevant-for-each parts of 2)
2) ideas that could be helpful in finding more ideas, with links to relevant-for-each parts of 3)
3) old ideas that didn't work, kept for extreme reinvestigation

New ideas are placed in 1 or 2.  Ideas from 1 that don't work are moved to 2 or 3.
When 1 empties because all the ideas weren't good enough, move 2 to 1 and 3 to 2.

So a relevent-for link type is kept, and added or removed to find the goal.  There's one starting reference of interest, that
has links to level 1 items.


missing: feedback
	We want to inform the relevancy not only which ideas are good and bad, but which are helpful and unhelpful.
	When looking for something, some things are 'closer' to it, and some things are 'farther' away.
	There is also something associated with how or why.  Something we already know.
	
missing: relevancy on tools used.
	For one thing, we have _ways_ of finding new ideas.  These ways may take only one idea, or a few, and work with them to
	find or more new ones.  For example, looking at sub-parts of a word, or looking at nearness in a use-of-it.

MONOLOGUE-STRUGGLE ON FEEDBACK.
	when learning relevancy with advice, we get told what is helpful or unhelpful, and how we would know.
		so while 'tying knot', we may need to find 'string', because 'knot' associated with 'string'
			in meaning-vm, that's a reference to a link
			here, that's a context and a key, or two paired strings.

					we can use a learning tool like 05.cpp's, to learn these?

						yes.  more generally, this raises ways of judging relevancy
TRYING TO COMPLETE FEEDBACK ASSUMPTIONS

is it possible to use relevant-associations to judge which associations to pursue first?

	say 'tying-knot', 'knot'+'string'

		so, the learning-structure _is_ the 3-level hierarchy in reference.
		this is associated with the task.  if tasks are similar, the relevent thing is the similar-part of them,
		to the relevancy of the other, roughly.

			okay, so now we have contextual-relevancy-parts.  we're shaking, so maybe just have relevancy associated
			with tasks.  maybe we can reuse a task's relevancy, pin some parts as staying.

		what would the 3-level hierarchy do if i said 'knot' implies 'string'?
			'knot' is good idea
			the _way_ that 'knot' goes to 'string' is good idea
			'string' is better idea

		so 'knot' would go in level 2
		and 'string' would go in level 1


let's open relevancy-for-ways.  when i call way-relevancy inside a relevancy, the context is the context of the relevancy, plus that
we are looking for a relevancy-way.
[btw we save because of pattern-mirror 'boss steals and erases when confirmed to be easy to complete and make workable']
*/
