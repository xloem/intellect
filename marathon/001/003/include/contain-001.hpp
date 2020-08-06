#pragma once
#include "025.hpp"

namespace symbols {
	sym(encapsulated);
	sym(seq);
}

ref encapsulate(ref what)
{
	return create({{symbols::what, what}, {symbols::is, symbols::encapsulated}});
}

ref decapsulate(ref what)
{
	return what->get(symbols::what);
}

ref recapsulate(ref what)
{
	return encapsulate(decapsulate(what));
}

text to_text(ref, bool = true);
text encapsulated_to_detail_text(ref encapsulated)
{
	return to_text(decapsulate(encapsulated));
}

// Link concepts in a list with symbolic refs
ref seq(il<ref> items)
{
	ref result = create({{symbols::is, symbols::seq}});
	ref first = symbols::nothing;
	ref next;
	for (auto & item : items)
	{
		ref last = next;
		next = encapsulate(item);
		if (!last) {
			first = next;
		} else {
			last->set(symbols::next, next);
			next->set(symbols::previous, last);
		}
	}
	if (next) {
		result->set(symbols::first, first);
		result->set(symbols::last, next);
	}
	return result;
}

ref seq_append(ref sequence, ref what)
{
	if (!sequence->isthing()) {
		return seq({what});
	}
	what = encapsulate(what);
	ref last = sequence->get(symbols::last);
	sequence->set(symbols::last, what);
	if (!last->isthing()) {
		sequence->set(symbols::first, what);
	} else {
		last->set(symbols::next, what);
		what->set(symbols::previous, last);
	}
	return sequence;
}

ref seq_clone(ref sequence)
{
	ref result = seq({});
	for (ref item = sequence->get(symbols::first); item->isthing(); item=item->get(symbols::next)) {
		result = seq_append(result, decapsulate(item));
	}
	return result;
}

text seq_to_detail_text(ref seq)
{
	text result;
	ref item = seq->get(symbols::first);
	while (true) {
		result += encapsulated_to_detail_text(item);
		item = item->get(symbols::next);
		if (!item->isthing()) { break; }
		result += ",";
	}
	return result;
}
