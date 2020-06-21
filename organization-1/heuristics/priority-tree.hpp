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
	// returns the option most in need of engagement
	// must be in a private session to not harm
	virtual long select()
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
		double worst_quality = 0;
		for (long choice_index = 0; choice_index < options; ++ choice_index)
		{
			i_priority_tree * choice = option(choice);
			double engagement_ratio = choice->engagement() / total_egagement;
			double interest_ratio = choice->interest() / total_interest;

			double ratio_quality = engagement_ratio - interest_ratio;

			if (ratio_quality < worst_quality) {
				worst_quality = ratio_quality;
				best_choice = choice_index;
			}
		}
		return best_choice;
	}

	// use within a private session
	// adds engagemnt to an option, and returns the option that is newly best
	// opens a private sesion with the option passed
	virtual long reselect(long last_choice, double new_engagement)
	{
		// people fight because they are not understood
		// understanding involves proof demonstrated by action
		// then you win all your fights beacuse you have none

		i_priority_tree * choice = option(last_choice);

		choice->indicate_private_and_wait();
		choice->change_engagement(choice->engagement() + new_engagement);
		choice->indicate_private_done();

		// can be optimized to loop less by storing more state obviously
		long new_choice = select();
	}

	// CLASSIFIED
	/////////////////////////////////////////////////////
};
