#include "ref.hpp"
#include "exception.hpp"

#define self (*this)

ref::operator bool() const
{
	return self != sym::nothing;
}

ref const ref::get(ref what) const
{
	auto result = (**this).refs.find(what);
	if (result == (**this).refs.end()) {
		return sym::nothing;
	} else {
		return result->second;
	}
}

void ref::verify_has(il<il<ref>> refs) const
{
	std::unordered_set<unsigned long> links;
	unsigned long bucket_count = (**this).refs.bucket_count();
	for (std::vector<ref> assumption : refs) {
		bool found = false;
		ref found_other = sym::nothing;
		if (!assumption.size()) { continue; }
		unsigned long bucket = (**this).refs.bucket(assumption[0]);
		unsigned long index = 0;
		for (auto link = (**this).refs.begin(bucket); link != (**this).refs.end(bucket); ++ link) {
			index += bucket_count;
			if (link->first != assumption[0]) {
				continue;
			}
			if (assumption.size() > 1 && link->second != assumption[1]) {
				found_other = link->second;
				continue;
			}
			unsigned long hash = index + bucket;
			if (links.count(hash)) {
				continue;
			}
			links.insert(hash);
			found = true;
		}
		if (!found) {
			if (assumption.size() == 1) {
				throw exception{
					{sym::assumed_has_a, assumption[0]},
				};
			}
			if (!found_other) {
				throw exception{
					{sym::assumed_has_a, assumption[0]},
					{sym::assumed_has_to, assumption[1]}
				};
			} else {
				throw exception{
					{sym::assumed_has_a, assumption[0]},
					{sym::assumed_has_to, assumption[1]},
					{sym::actually_has_to, found_other}
				};
			}
		}
	}
}

/*
std::unordered_set<ref> ref::getall(ref what) const
{
	std::unordered_set<ref> result;
	auto range = self->refs.equal_range(what);
	for (auto item : range) {
		
	}
	if (result == (**this).refs.end()) {
		return sym::nothing;
	} else {
		return result->second;
	}
}
*/

basic_ref create(il<std::pair<basic_ref,basic_ref>> refs, std::any data)
{
	return ref((il<std::pair<ref,ref>> &)refs, data);
}
