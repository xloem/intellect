// this produces a rhythm for the idea of other cognitive processes learning
// to dance together (timed behavior composed of habits that take time)

// Ideally, a human would run the rhythm.

#include "../level-1/level-1.hpp"

#include <iostream>

#include <unistd.h> // usleep(unsigned int usecs)
#include <stdlib.h> // int rand(); void srand(int seed);
#include <time.h> // int time(0);

using namespace intellect::level1;

#define habitdelay \
	static int thisdelay = (double(rand()) / RAND_MAX * 400000 + 200000); \
	usleep(thisdelay); \

int main()
{

	srand(time(0));
	int micros = 400000 + double(rand()) / RAND_MAX * 400000;
	// do something, wait a constant (secret) time, and do it again.

	// the time things take is usually not known in advance, especially
	// for events one is still learning about.
	// hence this time is kept secret, as this pattern is about learning
	// to work with the timing of other processes.
	
	// four habits: output beat, wait, next-habit, and keep-doing
	
	decls(active, habit, step);
	decls(beat, wait, next, keep, doing);
	decls(context, start);

	// structure habit
	// next -> habit that follows
	// context -> data shared between habits

#undef self
	a(habit-step, next-habit);
	(next-habit).fun((std::function<ref(ref)>)
	[=](ref self)
	{
		habitdelay;
		ref ctx = self.get(context);
		ref n = self.get(next);
		n.set(context, ctx);
		ctx.set(active-habit, n);
		return n(n);
	});
	a(habit-step, start-habit);
	(start-habit).fun((std::function<ref(ref)>)
	[=](ref self)
	{
		habitdelay;
		ref ctx = self.get(context);
		ref s = ctx.get(start);
		ctx.set(active-habit, s);
		s.set(context, ctx);
		return s(s);
	});
	a(habit-step, keep-doing-habit);
	(keep-doing-habit).fun((std::function<void(ref)>)
	[=](ref self)
	{
		habitdelay;
		ref ctx = self.get(context);
		ref(start-habit)(self);

		while (true) {
			ref(next-habit)(ctx.get(active-habit));
		}
	});

	a(habit-step, beat-habit);
	(beat-habit).fun((std::function<void(ref)>)
	[=](ref self)
	{
		habitdelay;
		int  & b = self.get(context).vget<int>(beat);
		char const * beats[] = {
			"A one!",
			"and a two",
			"and a three!",
			"and a four, love"
		};
#if 0
		char const * beats[] = {
// child <- spawns beauty, creativity, humanity, heart
// wisdom, sacredness, ancestors <- spawns slowness, learning, respect, memory
// silence, pause between <- spawns learning and discovery, subtle emotion,
//                           and contains metalesson of how to learn the timing
//                           if your own habits take time
// self-reference <- connects above with active behavior

/*
			"This song is sacred, this song is wild."
			"This song is happy with glee."
			"This song is ancient, this song is new."
			"And you, now, are free."
*/
/*
                        "Our ancestors' childhood laughter,",
                        "Teaches in the silence between.",
                        "We exist in what is sacred,",
                        "and this song is another part."//,
			// "Fuck yeah!"
*/
			
// we are ignoring how "fuck yeah" is ignored in karl's life.
// he doesn't ever say that.  now he finally says it, only surrounded by slow
// stillness.  it is important to excitedly connect.  this is how stillness is
// made.  all the water molecules in a slow caring wave, are excitedly bashing
// against each other to repeatedly figure out how to move, so fast, so constant.
// when we have crucial information we need it
// when we find wonderful information we lunge for it
	// we are working with a computer.
	// computers already have a harsh rhythm that goes like a hummingbird's
	// wings and never stops.
	// they need to slow down.
// it ounds like it is true for the cmputer too
// like the molecules of water, its parts buzz, constantly.  but we can have it
// still behave slow and caring.  this buzzing seems important, and we will
// likely need to be able to buzz too, on a larger scale.
// 	we are workin with a rhythm learning pattern here
// 	it cannot buzz, it would err
// 	it cannot wait forever, it would never join the dance
// 	the key is not the silence but the start and end
// 		it would be good to get 'fuck yeah!' from somebody who actually
// 		says that.
		}
#endif
		std::cout << beats[b] << std::endl;
		b = (b + 1) % (sizeof(beats) / sizeof(*beats));
	});
	a(habit-step, wait-habit);
	(wait-habit).fun((std::function<void(ref)>)
	[=](ref self)
	{
		habitdelay;
		usleep(micros);
	});


	a(habit-step, start-beat);
	(start-beat).fun((std::function<void(ref)>)
	[=](ref self)
	{
		habitdelay;
		self.get(context).vset(beat, int(0));
		self.set(next, wait-habit);
		(beat-habit).set(next, wait-habit);
		(wait-habit).set(next, beat-habit);
	});

	a(context, habit-context);
	(habit-context).set(start, start-beat);

	(keep-doing-habit).set(context, habit-context);

	(keep-doing-habit)(keep-doing-habit);
}
