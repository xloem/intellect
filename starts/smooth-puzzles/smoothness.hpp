#pragma one

template <typename Context, typename Value>
class smooth_puzzle_part
{
public:
	Context context;

	// we start with a guess
	Value guess;

	// then we try the guess
	
	// after learning the guess, we discover what would have been better
	void update(Value answer)
	{
		// but we want to update the guess fluidly, so as to not overshoot.
		guess = (guess + answer) / 2;

		// puzzle branch idea 2: we could make this update smoothness be another puzzle.
			
			// same context, different puzzle.
			// the guess + answer are how much to update.
			// but we don't get the answer until we know how smooth it is.
	}
};

// we found we can solve more smooth puzzles
	// one idea: keep discreteness small.  in little bundles.
