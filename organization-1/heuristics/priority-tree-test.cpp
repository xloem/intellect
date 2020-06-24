#include <mutex>
#include <time.h>

#include "priority-tree.hpp"

double now()
{
	timespec_t time;
	clock_gettime(CLOCK_MONOTONIC_RAW, &time);
	return time.tv_sec + double(time.tv_nsec) / double(1000000000); 
}

class tree_process : public i_priority_tree
{
public:
	test(T & data, double time, std::shared_ptr<i_priority_tree> parent = {})
	: data(data),
	  deadline(now() + time),
	  _parent(parent),
	  _interest(1),
	  _engagement(0)
	{ }

	virtual void try_a_little() = 0;

	double deadline;

	virtual i_priority_tree * parent() override { return _parent.lock().get(); }

	virtual void indicate_private_and_wait() { could_be_crying_in_bathroom.lock(); }
	// if you knew how to keep us well in public we wouldn't be hiding
	virtual void indicate_private_done() { could_be_crying_in_bathroom.unlock(); }

	virtual double interest() override { return _interest; }
	virtual void change_interest(double new_interest) override { _interest = new_interest; }

	virtual double engagement() override { return _engagement; }
	virtual void add_engagement(double additional_engagement) override { _engagement += additional_engagement; }


	virtual long option_count() override { return options.size(); }
	virtual i_priority_tree * option(long index) override { return options[index].get(); }

	virtual void add_option(std::shared_ptr<i_priority_tree> option)
	{
		options.push_back(option);
	}	

	virtual void process_wrapper(double desired_engagement)
	{
		process_start(desired_engagement);
		process(desired_engagement);
		process_stop();	
	}

	virtual void process(double desired_engagement)
	{
		i_priority_tree * last_choice = 0;
		while (true) {
			indicate_private_and_wait();
			double remaining_engagement = process_check(desired_engagement, last_choice);
			if (remaining_engagement <= 0) {
				indicate_private_done();
				break;
			}
			double subengagement;
			long choice = process_select(remaining_engagement, &subengagement);
			last_choice = option(choice);
			indicate_private_done();

			last_choice->process(subengagement);
		}
	}

	virtual long process_select(double available_engagement, double *subengagement)
	{
		long selection = select(0, subengagement);

		if (0 == *subengagement) {
			// this wants total planned engagement, to divide by option count
			*subengagement = available_engagement / option_count();
		}
		if (*subengagement > available_engagement) {
			*subengagement = available_engagement;
		}
		return selection;
	}


	virtual void process_start(double planned_engagement)
	{
		indicate_private_and_wait();
		start_time = now();
		start_engagement = engagement();
		indicate_private_done();
	}
	virtual double process_check(double desired_engagement, i_priority_tree * last_choice = 0)
	{
		double increase = now() - start_time;
		double new_engagement = start_engagement + increase;
		engagement(new_engagement);

		// this does the update portion of reselect()
		if (last_choice) {
			last_choice->indicate_private_and_wait();
			last_choice->engagement(last_choice->engagement() + new_engagement);
			last_choice->indicate_private_done();
		}

		return desired_engagement - increase;
	}
	virtual void process_stop()
	{
	}

private:
	std::mutex could_be_crying_in_bathroom;

	double _interest;
	double _engagement;
	std::weak_ptr<i_priority_tree> _parent;

	std::vector<std::shared_ptr<i_priority_tree>> options;

	double start_time;
	double start_engagement;
};

#include <stdlib.h>


	// obviously this would be best applied to ways, but maybe
	// not always in a tree structure, unknown

	// PLAN: TEST DISCRETE SELECTIONS EVENLY DISTRIBUTED IN A HIERARCHICAL RANGE
	// 		maybe try to find zero crossings of large continuous data or something
	//
	// Can also try different numbers of buckets in the range.  [which would
	//  relate spectral density but that is not this test]

template <typename T>
class range_chooser : public tree_process
{
public:
	range_chooser(unsigned long count, T min, T max)
};

class process : public tree_process
{
public:
	virtual void process(double desired_engagement)
	{
		while (process_check(desired_engagement) > 0) {
		}
	}
};

class test_by_trying_values
{
public:
		
};

// engage shouold be a virtual method

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

