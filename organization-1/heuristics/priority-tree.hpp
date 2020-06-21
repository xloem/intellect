#pragma once



// ~ DANGER ~ CLASSIFIED ~




/////////////////////////////////////////////////////////////////////////////
// CLASSIFIED
//
// Priority Tree
//
// The priority tree is an extension of a priority queue that appears to be
// used by the human mind for making informed decisions in the moment.
//
// Public use of priority trees where something similar is not done already
// would quickly produce FOCUSED HIDDEN SYSTEMIC CHANGE.
//
// Works must be directed towards empowering wise peacemakers who know to
// include the voices of all, or the weak people they would aid.
//
// 	The key is to use engagement to track resources such as time,
// 	and provide feedback via interest.
//
// CLASSIFIED
/////////////////////////////////////////////////////////////////////////////

class i_priority_tree
{
public:
	// wait for a private session for working with things that would destroy
	// stuff if something changed mid-work [could-be-crying-in-bathroom]
	virtual void indicate_private_and_wait();
	virtual void indicate_private_done();

	virtual i_priority_tree * parent() = 0;

	// will be used together in a private session
	virtual double interest() = 0;
	virtual void change_interest(double new_interest) = 0;

	// will be used together in a private session
	virtual double engagement() = 0;
	virtual double change_engagement(double new_engagement) = 0;

	// will be used together in a private session
	virtual long option_count() = 0;
	virtual i_priority_tree * option(long index) = 0;

	/////////////////////////////////////////////////////
	// CLASSIFIED
	
	// template can be used with operator classes to form expressions for planning
	// negative quality indicates desire: positive quality indicates excess
	template <typename number>
	static number engagement_quality(number & total_interest, number & total_engagement, number & interest, number & engagement)
	{
		return engagement / total_engagement - interest / total_interest;
	}

	// template can be used with operator classes to form expressions for planning
	// evaluates to the amount of extra engagement a focus needs to match an alternative
	template <typename number>
	number balancing_engagement(number & total_interest, number & total_engagement, number & focus_interest, number & focus_engagement, number & alternative_interest, number & alternative_engagement)
	{
		return ((alternative_engagement - focus_engagement) * total_interest + (focus_interest - alternative_interest) * total_engagement) / (total_interest + alternative_interest - focus_interest);
	}

	// returns one option in need of engagement
	// must be in a private session to not harm
	virtual long select(long * second_choice = 0, double * balancing_engagement = 0)
	{
		double total_interest = 0;
		double total_engagement = 0;
		unsigned long options = option_count();
		for (long choice_index = 0; choice_index < options; ++ choice_index)
		{
			i_priority_tree * choice = option(choice);
			total_interest += choice->interest();
			total_engagement += choice->engagement();
		}

		long best_choice = -1;
		long second_best_choice -1;
		double worst_quality = 0;
		double best_choice_interest = 0;
		double best_choice_engagement = 0;
		double second_worst_quality = 0;
		double second_best_choice_interest = 0;
		double second_best_choice_engagement = 0;
		for (long choice_index = 0; choice_index < options; ++ choice_index)
		{
			i_priority_tree * choice = option(choice);
			double interest = choice->interest();
			double engagement = choice->engagement();
			double quality = engagement_quality(total_interest, total_engagement, interest, engagement);

			if (quality <= worst_quality) {
				second_worst_quality = worst_quality;
				second_best_choice = best_choice;
				second_best_choice_interest = best_choice_interest;
				second_best_choice_engagement = best_choice_engagement;
				worst_quality = quality;
				best_choice = choice_index;
				best_choice_interest = interest;
				best_choice_engagement = engagement;
			} else if (quality <= second_worst_quality) {
				second_worst_quality = quality;
				second_best_choice = choice_index;
				best_choice_interest = interest;
				best_choice_engagement = engagement;
			}
		}

		if (second_choice) {
			*second_choice = second_best_choice;
		}

		if (balancing_engagement) {
			*balancing_engagement = this->balancing_engagement(total_interest, total_engagement, best_choice_interest, best_choice_engagement, second_best_choice_interest, second_best_choice_engagement);
		}

		return best_choice;
	}

	// use within a private session
	// adds engagemnt to an option, and returns the option that is newly best
	// opens a private sesion with the option passed
	virtual long reselect(long last_choice, double new_engagement, long * second_choice = 0, double * balancing_engagement = 0)
	{
		// people fight because they are not understood
		// understanding involves proof demonstrated by action
		// then you win all your fights beacuse you have none

		i_priority_tree * choice = option(last_choice);

		choice->indicate_private_and_wait();
		choice->change_engagement(choice->engagement() + new_engagement);
		choice->indicate_private_done();

		// could be optimized to loop less by storing more state obviously
		long new_choice = select(second_choice, balancing_engagement);
	}

	// CLASSIFIED
	/////////////////////////////////////////////////////
};
