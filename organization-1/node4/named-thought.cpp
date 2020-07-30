#include "named-thought.hpp"

#include "easy.hpp"

named-thought named-thought::find(std::initializer_list<reference> specifiers)
{
	// find a thought by looking for names and associations, in order of closest-first
	
	// we _won't_ include the actual references in the sets, but we will use them as exploration seeds
	
	std::list<reference> initial_seed_stack(specifiers.begin(), specifiers.end());

	// as we find thoughts, we want to compare their distance or order with existing thoughts.
	// if a specifier has _lower_ distance, it wins the comparison
	
	struct proposed-thought {
		reference thought;
		std::unordered_map<double> specifier-distance;
		std::unordered_set<reference> explored;

		proposed-thought(reference thought, std::initializer_list<reference> const & specifiers, std::initializer_list<std::pair<reference,double>> distances)
		: thought(thought)
		{
			for (auto & specifier : specifiers) {
				specifier-distance[specifier] = 1.0 / 0;
			}
			for (auto & distance : distances) {
				explored.insert(distance.first);
				specifier-distance[distance.first] = distance.second;
			}
		}
	};
	struct thought-exploration {
		thought which;
		double distance;
		reference spot;
	};
	std::map<reference, proposed-thought> proposed-thoughts;
	std::map<reference, thought-exploration> explorations;

	for (auto specifier & : specifiers) {
		auto kinds = specifier.get-all-kinds();
		for (auto kind-index = 0; kind-index < kinds.order-count(); ++ kind-index) {
			auto thought = specifier.kind-get(kinds.order-get(kind-index));
			proposed-thoughts.emplace(thought, specifiers, {{specifier, 1}});
			explorations[thought] = thought-exploration{thought, 0, thought};
		}
	}

	
	// after we find one thought, we can output names and associated names from it, to very it is correct

	
	std::set<reference> thoughts_found;
}

named-thought::named-thought(std::initializer_list<library::string> specifiers)
{
	// some thoughts have names
}

named-thought::named-thought(reference source, std::initializer_list<library::string> specifiers)
{

}
