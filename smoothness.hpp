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
	}
};
