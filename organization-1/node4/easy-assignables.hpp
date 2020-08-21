#pragma once

#include "basic-assignables.hpp"

class easy;

DECLARE reference kind-inverse;

// I was thinking of _not_ representing inversion, towards the preference of understand-by-observation prior to understand-by-instruction

// link objects would be easy to include here

// this was made to be the contextual name class.
// 	[how can we tell such things in the future?]
// hmmm welll hmmm it was a process shape of not knowing why you abandoned
// something, and then bumping into a use of it.  the observer missed that
// the goals are singular and identical.
// SINGULAR IDENTICAL GOAL: contextual-unique-name-contexts
// this is for-easy-class, but has some path-parts to generality
// what is needed to extend this to contextual names?
// 	1. use it for the context
// 	2. pick a kind for having a contextual name <- maybe?
// 		[GRR! there is inhibition on this that has roughly-reviewed-valid
// 		 reason-shape but ?[other thing also inhibited]]
// 		 	[uhhh for time-value, if main.cpp were efficient,
// 		 	 and thought-palace weren't the immediate goal,
// 		 	 then it would make sense to mergre this into
// 		 	 main.cpp .  concepts intertwined in there would
// 		 	 be sensical to use.]
// 		 [our very waynesses are intertwined through each other?]
// 		 [marked with reasons so should be okay to give for other]
// 		 [remember to make sure to produce lasting contribution in choices]


// WARNING: this was intended to maintain inverse information, but structure
//  was altered without verifying correctness of that part
// inverse information helps generalize attribute, category, and name lookups
// 	[the structure is that every kind may have an inverse linked with
// 	 kind-inverse().  inverses are linked back, which requires multiple
// 	 same-kind links.  these are held transparently by sub-references with
// 	 ordered data.]
template <typename Type>
class inverting-kinded-assignable : public Type
{
public:

<<<<<<< HEAD
	inverting-kinded-assignable(easy source, reference kind)
	: basic-assignable(source, kind)
	{ }

	// TODO? use structured typedness to provide constness of attributes?
	inverting-kinded-assignable & operator=(reference const & other)
=======
	inverting-kinded-assignable(reference source, reference kind)
	: Type(reference::null()), source(source), kind(kind)
	{
		Type::reseat(get());
	}

	// TODO? use structured typedness to provide constness of attributes?
	inverting-kinded-assignable & operator=(reference const & _other)
>>>>>>> 31143fb32f1542f4f93ecaec21c432b9774aa36b
	{
		reference other = _other;

		{
			reference old = source.kind-get(kind);
			if (reference::null() != old) {
				if (old.order-count().data<index_t>() > 0) {
					throw "assigned reference is ordered; try to fix in a way that doesn't make a new big issue later?  or just add code to handle it?";
				}
			}
		}

		/// set
		source.kind-set(kind, other);
		this->reseat(other);
		/// ---

		reference inverse-kind = kind.kind-get(kind-inverse());
		// if inverse is made wrongly, merge it with the right node
		// may require enumeration of all nodes or a hook placed
		// on the wrong one to notice when used [might be helpful
		//  to make 'accessed' or somesuch method gets called before
		//  returning it]
		if (reference::null() == inverse-kind) {
			inverse-kind = reference();
			kind.kind-set(kind-inverse(), inverse-kind);
			inverse-kind.kind-set(kind-inverse(), kind);
		}
		reference inverses = other.kind-get(inverse-kind);
		if (reference::null() == inverses) {
			other.kind-set(inverse-kind, source);
		} else if (inverses.order-count().data<index_t>() == 0) {
			reference inverse_set;
			inverse_set.order-set((std::any)(index_t)0, inverses);
			other.kind-set(inverse-kind, inverse_set);
			inverse_set.order-set((std::any)(index_t)1, source);
		} else {
			inverses.order-set(inverses.order-count(), source);
		}
		return *this;
	}

	reference get()
	{
		// this being different from kinded-assignable was part of why the classes got separated in a quick fix attempt
		reference result = source.kind-get(kind);
		if (reference::null() == result) {
			return result;
		}
		reference count = result.order-count();
		if (count.data<index_t>() == 0) {
			return result;
		} else if (count.data<index_t>() == 1) {
			return result.order-get((std::any)(index_t)(0));
		} else {
			throw "do you think this ambiguity could make a big issue later? reference is ordered with count > 1; could use link objects to resolve, or just ignore: presently handled with begin(), end()";
		} 
	}

	ordered-assignable begin()
	{
		reference result = source.kind-get(kind);
		return {result, reference((std::any)(index_t)(0))};
	}

	ordered-assignable end()
	{
		reference result = source.kind-get(kind);
		return {result, result.order-count()};
	}

protected:
	reference source;
	reference kind;
};
