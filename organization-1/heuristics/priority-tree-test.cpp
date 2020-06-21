
#include <any>
#include <mutex>

// ~ DANGER ~ CLASSIFIED ~

/////////////////////////////////////////////////////////////////////////////
// CLASSIFIED
//
// DANGER: FOCUSED HIDDEN SYSTEMIC CHANGE
//
// Works must be directed towards empowering wise peacemakers who know to
// include the voices of all, or the weak people they would aid.
//
// This work uses priority trees, demonstrating them.  See header file.
// draft, w.i.p.
#include "priority-tree.hpp"

double now()
{
	timespec_t time;
	clock_gettime(CLOCK_MONOTONIC_RAW, &time);
	return time.tv_sec + double(time.tv_nsec) / double(1000000000); 
}

template <typename T>
class test : public i_priority_tree
{
public:
	test(T & data, double time, std::shared_ptr<i_priority_tree> parent = {})
	: data(data),
	  deadline(now() + time),
	  _parent(parent),
	  _interest(1),
	  _engagement(0)
	{ }

	double deadline;
	T data;

	virtual double interest() override
	{
		return _interest;
	}
	virtual void change_interest(double new_interest) override
	{
		_interest = new_interest;
	}

	virtual double engagement() override
	{
		return _engagement;
	}
	virtual void add_engagement(double additional_engagement) override
	{
		_engagement += additional_engagement;
	}

	virtual i_priority_tree * parent() override
	{
		return _parent.lock().get();
	}

	virtual long option_count() override
	{
		return options.size();
	}

	virtual i_priority_tree * option(long index) override
	{
		return options[index].get();
	}

	virtual void indicate_private_and_wait()
	{
		// if you knew how to keep us well in public we wouldn't be hiding
		could_be_crying_in_bathroom.lock();
	}

	virtual void indicate_private_done()
	{
		could_be_crying_in_bathroom.unlock();
	}

private:
	std::mutex could_be_crying_in_bathroom;

	double _interest;
	double _engagement;
	std::weak_ptr<i_priority_tree> _parent;

	std::vector<std::shared_ptr<i_priority_tree>> options;
};

#include <stdlib.h>
#include <time.h>

using element = test<unsigned long>;

// so most of our work is option-picking here
// we need to know our last set of resources, and the option we are considering
// what structure? [a tree]
// how is structure navigated?
// 	[we want to explore to the root from a leaf, and have access to full elements]
// so we want parent links.  how are items added and removed?
// 	[we add multiple leafs to each node, usually to the last one added.  we remove entire nodes ... or maybe just leaves]
//
// it sounds like if the nodes are stored in an array thta we will always have mid-removal, and that we will need more structure than a linked list provides.
// so it would make sense to use our own datastructure.  we can store nodes in a set if needed.

// should like be merged into i_priority_tree, since each choice selection tracks one resource


/*
 * pairs of elements and deadlines
 * element has deadline now
 */

void engage(element & element)
{
	double recent = now();
	long choice;
	while (element.deadline > recent) {
	
		double subdeadline = element.deadline;
	
		element.indicate_private_and_wait();
		double balancing_engagement;
		choice = element.select(0, &balancing_engagement);
		if (choice < 0) {
			return; // no-choices
			// TODO: make-new-choice
		}
		// TODO: we can fix balancing_engagement algorithm to relate with available time to consider all
		if (0 == balancing_engagement) {
			balancing_engagement = (element.deadline - recent) / element.option_count();
		}
	
		if (balancing_engagement + recent < subdeadline) {
			subdeadline = balancing_engagement + recent;
		}
		i_priority_tree * option = element.option(choice);
		element.indicate_private_done();
	
		// some small danger here: options could change, and are held by index
		engage(option, subdeadline);
		
		double next = now();
		element.indicate_private_and_wait();
		choice = element.reselect(choice, next - recent, 0, &balancing_engagement);
		element.indicate_private_done();
		recent = next;
	
	}
}

int main()
{
	// let us maybe find seeds that produce low random numbers recursively
		// we don't have nearness?
			// can we discover it?
		// you want to make a graph to see what's near?
	element base(0);

	// redo recursive function attempt
	
		// you want to move around in a tree ...
			// we engage the child-nodes for some time
}
// CLASSIFIED
/////////////////////////////////////////////////////////////////////////////
