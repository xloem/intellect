#pragma once
//#include "026.hpp"

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
ref seq(il<ref> items = {});

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
		sequence->data<unsigned long>() = 1;
	} else {
		last->set(symbols::next, what);
		what->set(symbols::previous, last);
		++ sequence->data<unsigned long>();
	}
	return sequence;
}

ref seq_prepend(ref sequence, ref what)
{
	if (!sequence->isthing()) {
		return seq({what});
	}
	what = encapsulate(what);
	ref first = sequence->get(symbols::first);
	sequence->set(symbols::first, what);
	if (!first->isthing()) {
		sequence->set(symbols::last, what);
		sequence->data<unsigned long>() = 1;
	} else {
		first->set(symbols::previous, what);
		what->set(symbols::next, first);
		++ sequence->data<unsigned long>();
	}
	return sequence;
}

ref seq_shift(ref sequence)
{
	ref first = sequence->get(symbols::first);
	ref second = first->get(symbols::next);
	if (second->isthing()) {
		second->wipe(symbols::previous);
		sequence->set(symbols::first, second);
		-- sequence->data<unsigned long>();
	} else {
		sequence->wipe(symbols::first);
		sequence->wipe(symbols::last);
		sequence->data<unsigned long>() = 0;
	}
	return decapsulate(first);
}

ref seq_pop(ref sequence)
{
	ref last = sequence->get(symbols::last);
	ref secondlast = last->get(symbols::previous);
	if (secondlast->isthing()) {
		secondlast->wipe(symbols::next);
		sequence->set(symbols::last, secondlast);
		-- sequence->data<unsigned long>();
	} else {
		sequence->wipe(symbols::first);
		sequence->wipe(symbols::last);
		sequence->data<unsigned long>() = 0;
	}
	return decapsulate(last);
}

unsigned long seq_size(ref sequence)
{
	return sequence->data<unsigned long>();
}

// Link concepts in a list with symbolic refs
ref seq(il<ref> items)
{
	ref result = create({{symbols::is, symbols::seq}}, (unsigned long)0);
	for (auto & item : items)
	{
		seq_append(result, item);
	}
	return result;
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
