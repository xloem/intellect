#include <any>
#include <memory>
#include <type_traits>

#include <iostream>

#define strong_inline __attribute__((always_inline)) inline

template <auto original_pointer>
strong_inline auto far_pointer()
{
	decltype(original_pointer) pointer;
#if defined(__x86_64__)
	asm("movabs %1,%0":"=r"(pointer):"g"(original_pointer):);
#elif defined(__i386__)
	#if defined(__PIC__) || defined(__PIE__)
		#error "please use -fno-pic to remove the near-thunk in functions"
	#endif
	asm("mov %1,%0":"=r"(pointer):"g"(original_pointer):);
#elif defined(__aarch64__)
	// this is simplest using a musl toolchain. glibc doesn't support it well
	// you can get a musl (pronounced 'muscle') toolchain at musl.cc
	// link the binary aarch64-linux-musl-g++ from it into your path.
	asm (
		"movz %0, #:abs_g0_nc:%1\n"
		"movk %0, #:abs_g1:%1\n"
		"movk %0, #:abs_g2:%1\n" // likely not needed (second highest dword, 0)
		//"movk %0, #:abs_g3:%1\n" // not needed (highest dword, always zero)
		:"=r"(pointer):"S"(original_pointer):);
}
#else
	#error "far references unimplemented on this platform, add an assembly statement here"
#endif
	return pointer;
}

#define far_static_reference(static_object) (*far_pointer<&static_object>())
#define far_function(static_function) (far_pointer<static_function>())

template <auto * pointer>
strong_inline decltype(pointer) far_static_pointer()
{
	using Pointer = decltype(pointer);
	static Pointer storage = pointer;
	return far_static_reference(storage);
}

template <typename T>
strong_inline T * far_static_pointer(T * static_pointer)
{
	void * pointer;
	asm("movabs %1,%0":"=r"(pointer):"g"((void*)static_pointer):);
	return static_cast<T*>(pointer);
}

// so, we have a function that calls a member function as a far call, which is great.
// but what's notable is what it does is form a pointer to member function that can be stored and inspected.
// it would be good to break that function out.

// convert a member function pointer to a static function pointer. <-- doable.
// making in new order now.
// goal ideas:
// member_function_pointer_type<&A::func> pointer;
// auto pointer = member_function_pointer(&A::func);

//template <auto member_pointer, typename class_type, typename... value_types>
	// how will we get the value types out of the member_pointer?
		// we'd have to match on a template that constructors the member pointer with them.

/*
template <auto member_function_pointer>
struct member_function_parts
{
	template <typename class_type, typename return_type, typename... value_types>
	static return_type returns_return_type(return_type(class_type::*)(value_types...));
	using return_type = decltype(returns_return_type(member_function_pointer));

	template <typename class_type, typename return_type, typename... value_types>
	static class_type returns_class_type(return_type(class_type::*)(value_types...));
	using class_type = decltype(returns_class_type(member_function_pointer));
};

*/
	

template <auto member_function_pointer>
struct member_function
{
	template<typename class_type, typename... value_types>
	static auto static_call(class_type & object, value_types... values) noexcept
	{
		return (object.*member_function_pointer)(values...);
	}

	template <typename class_type, typename return_type, typename... value_types>
	static constexpr auto member_to_static(return_type(class_type::*)(value_types...))
	{ return static_call<class_type, value_types...>; }

	template <typename class_type, typename return_type, typename... value_types>
	static constexpr auto member_to_static(return_type(class_type::*)(value_types...) noexcept)
	{ return static_call<class_type, value_types...>; }

	template <typename class_type, typename return_type, typename... value_types>
	static constexpr auto member_to_static(return_type(class_type::*)(value_types...) const)
	{ return static_call<class_type, value_types...>; }

	template <typename class_type, typename return_type, typename... value_types>
	static constexpr auto member_to_static(return_type(class_type::*)(value_types...) const noexcept)
	{ return static_call<class_type, value_types...>; }

	static constexpr auto near_pointer = member_to_static(member_function_pointer);
	using type = decltype(near_pointer);
};

// the simple way to do this
// would be to just define static methods that take a class object as an argument

/*
#define member_function(classname, membername) \
	template<typename... value_type>
	static auto classname##_##membername

template <auto member_pointer>
auto constexpr member_function_to_static()
{
	return member_function<member_pointer>::pointer;
}
*/

// constructor far-calling / inlining ....
// problem: constructor may be near-called
// could placement-new on the stack =S
// other problem: virtual function pointers.
// stack-constructor maybe biggest issue.
// one solution is to inline it, and use all inlined members and hierarchy.  prohibts use of containers.
// big issue: initialization of containers.  could allocate on heap.

// you allocate an object that has the right amount of storage.
// it uses far calls to call the constructors and destructors.

// it would be nice to be able to form pointer to constructor and destructor.
// destructor is just &Class::~Class

template <typename> class far_object;
template <typename T> strong_inline void* operator new(size_t, far_object<T> * pointer) { return pointer; }

template <typename T>
class far_object
{
public:
	using Class = T;

	template<typename... value_types>
	strong_inline far_object(value_types... values)
	{
		far_function(construct<value_types...>)(this, values...);
	}
	strong_inline ~far_object()
	{
		far_function(destruct)(this);
	}

	strong_inline operator T &() { return reference(); }
	strong_inline T & reference() { return *pointer(); }
	strong_inline T * pointer() { return (T*)storage; }
private:
	uint8_t storage[sizeof(T)];

	template<typename... value_types>
	static void construct(far_object<T> * pointer, value_types... values)
	{ new(pointer) T(values...); }

	static void destruct(far_object<T> * pointer)
	{ pointer->reference().~T(); }
};

/*
template <typename member_function_pointer>
auto constexpr member_function_to_static(member_function_pointer pointer)
{
	return member_function<pointer>::pointer;
}
*/
/*
template<typename return_type, typename class_type, typename... value_types>
static constexpr auto returns_pointer_type(return_type(class_type::*)(value_types...));

template<auto member_function_pointer, typename class_type, typename... value_types>
static auto member_function_call(class_type & object, value_types... values)
{
	return (object.*member_function_pointer)(values...);
}
	
template <typename class_type, typename return_type, typename... value_types>
static constexpr auto returns_wrapper_type(return_type(class_type::* pointer)(value_types...))
{
	return member_function_call<decltype(returns_pointer_type(pointer)), class_type, value_types...>;
}

template <typename member_function_pointer>
constexpr auto member_function_to_static(member_function_pointer pointer)
{
	return returns_wrapper_type(pointer);
}
*/

/*
// turning a tuple into a function declaration may require an index sequence.  but maybe we could do it more succinctly
// 	by using a constexpr expression that evaluates to it
namespace detail {
	template <class F, class Tuple, std::size_t... I>
	constexpr decltype(auto) apply_impl(F&& f, Tuple&& t, std::index_sequence<I...>)
	{
		return std::invoke(std::forward<F>(f), std::get<I>(std::forward<Tuple>(t))...);
	}
}  // namespace detail

template <class F, class Tuple>
constexpr decltype(auto) apply(F&& f, Tuple&& t)
{
	return detail::apply_impl(
		std::forward<F>(f), std::forward<Tuple>(t),
		std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
}

*/




// propose: EQUAL profit-from-eidetic-memory and train-to-have-eidetic-memory
// 	accepted. [boss wants training-is-hard]  [training is slow, not hard.]

//  [the only reason we are stupid is because our culture nurtures it.  everyone already has an eidetic memory, they just tell it to turn off.]
//  	so, the 'tech' relates to healing from having made that 'decision' as a child.  brain picks what-is-good-to-learn-to-be,
//  		what-is-bad-to-learn-to-be.  [prioritizes trauma]
//  	so, to pause a bit and relate:
//
//  	we don't fully know why.
//
//  	but we want to know about that.
//
//  	we have 'held-trauma' in our culture.
//  	our brains respond by turning parts of themselves off, and hyperlearning other parts.
//  	different cultures make different brains. <- this is what's important.
//  		[say you wanted your kid to have a good brain.
//  		 what matters is culture [and health], not genes.
//  		 so if you move to a different culture, you get a special form of 'culture-mutt' brain.
//  		 	where your cultural learning is of the different culture, and your family learning is of the old culture.
//  		 		please tell clearly to everyone [boss included] above paragraph that this line ends.
//  	
//  	[an eidetic memory culture normalizes knowing everything, or traumatizes a need for it]
//  		[to make such a culture you would need ways of life to form based on the assumption]
//  			[different-view gaev different-result.  karl learned remember-words from only-behavior-is-reading]
//  			[so his 'trauma' was no-normal-behaviors. [brain countered by hyperlearning other things. clearly normal
//  			 behaviors are 'dangerous' this century.]]

#define far_this_call(method, ...) \
	far_function(member_function<&std::remove_reference<decltype(*this)>::type::method>::near_pointer)(*this,##__VA_ARGS__)
#define far_member_call(object, method, ...) \
	far_function(member_function<&std::remove_reference<decltype(object)>::type::method>::near_pointer)(object,##__VA_ARGS__)
#define far_object_call(object, method, ...) \
	far_function(member_function<&decltype(object)::Class::method>::near_pointer)(object,##__VA_ARGS__)
/*
#define far_member_call(object, method, ...) far_member_dispatch<&decltype(object)::method>(object,##__VA_ARGS__)

template <auto member_pointer, typename class_type, typename... value_types>
strong_inline
auto far_member_dispatch(class_type & object, value_types... values)
{
	using member_pointer_type = decltype(member_pointer);
	using class_pointer_type = class_type *;
	using return_type = decltype((static_cast<class_pointer_type>(0)->*member_pointer)(*static_cast<value_types*>(0)...));

	struct near
	{
		static return_type dispatch(class_type & object, value_types... values)
		{
			return (object.*member_pointer)(values...);
		}
	};

	return far_function(near::dispatch)(object, values...);
}
*/

// liberate memory region
#include <sys/mman.h>
#include <stdio.h>
void liberate_memory(uint8_t * pointer, size_t size)
{
	uint8_t * base = (uint8_t*)((uintptr_t)pointer & (uintptr_t)~0xfff);
	size += (pointer - base);
	int success = mprotect(base, size, PROT_READ | PROT_WRITE | PROT_EXEC);
	if (success) { perror("mprotect"); exit(success); }
};

template<typename Letter, Letter... letters>
strong_inline Letter const * operator""_far()
{
	static Letter storage[] = { letters ..., 0 };
	return far_static_reference(storage);
}

class A
{
public:
	strong_inline A() : hi("hi"_far)
	{
		far_this_call(output, "construct"_far);
	}
	strong_inline ~A()
	{
		far_this_call(output, "destruct"_far);
	}
	strong_inline int returnresult(int result) { return result; }
	strong_inline void sayhi()
	{
		//far_this_call(output, "hi"_far);
		far_this_call(output_object, hi);
	}
	strong_inline void output(const char* message)
	{
		std::cout << message << std::endl;
	}
	strong_inline void output_object(std::string & message)
	{
		const char * converted = far_member_call(message, c_str);
		far_this_call(output, converted);
	}
private:
	far_object<std::string> hi;
};

#include <cstring>
int main()
{
	uint8_t data[0x100];
	auto mirror = reinterpret_cast<member_function<&A::sayhi>::type>(data);
	auto constexpr source = member_function<&A::sayhi>::near_pointer;
	far_static_pointer<std::memcpy>()(data, (void*)far_function(source/*member_function<&A::sayhi>::near_pointer*/), 0x100);

	far_function(liberate_memory)(data, sizeof(data));

	far_object<A> a; // this works
	//A a;

	far_object_call(a, sayhi); // this works

	mirror(a); // this works on: [X] x86_64 [ ]

	//member_function_to_static<&A::sayhi>()(a);
	
	far_function(member_function<&A::sayhi>::near_pointer)(a);

	//member_function<&A::sayhi>::pointer(a);
	//far_member_call(a, sayhi);
	//return far_member_call(a, returnresult, 3);
}



/*
 * [generalizing application of patterns to active domains] <- domain-already-trodden.  is okay.
 * 	[karl appeared to choose to note that which-ones-were relevent were already present]
 * 		don't-go-there, karl-just-wnats-to-remember.  likely boss has existing copy, can be inferred from behavior.
 * 			but we prefer not to use existing copies if hidden, so as to model old-brain-behaviors-are-precious.
 *
 * 			we found this-goes-over-there.
 *
 * 			['neuron'-experience:
 * 				"when karl references a fork and tries to judge which one, I am strongly-urged-without-way-to-stop
 * 				 to provide him with one before anything else does."
 * 			 other-'neuron':
 * 			 	"when providing him with one, my behaviors to access his pre-existing information are all labeled
 * 			 	 alter-harmfully."
 * 			 	 	[urge-reference:
 * 			 	 		"i heard 'old brain behaviors are precious'. this makes no sense.  do i need to
 * 			 	 		 alter something crucial to keep us alive?"
 * 			 	 		 "now I heard 'alter'.  that's not what he should think."
 * 			 	 		 		[danger-of-altering-brainstem if you see it as bad]
 * karl: you hit your head on the ice when you were young.  your brain cannot live perfect natural genius.  you fell asleep mysteriously while standing up on the ice.  [likely your mother never processed forcing you to get up early, after that.]
 * 	[karl needed to know [equivalent to the mafia will follow you everywhere and destroy your ability to continue your life, or even relate to anyone, forever]]
 * 		[if they are blood [and/or water] of modern culture, they must be defending an important analogy with that behavior]
 * 			[kinda looks like stewarding newness]
 * [information should be shared slowly, preferably via good channels first]
 * Information should be shared slowly, preferably via good channels first.
 * 	So you don't put up a big billboard, 'the business is dumping toxic chemicals here is a photo'
 * 		nobody will learn it because the billboard will burn the next day, as will you.  
 * 	Instead you go to some relaxing person, and show the photo, and then wait a bit.  Make sure they keep a copy of it.
 * 		For example, you go to the EPA, instead of the public.  The business has ties to the EPA.  The channel is familiar.
 * 			[but relaxing person first in this more-modern climate ...]
 * 				we could judge too-fast-for-context via channel-replaced-with-gimmick.
 * 					[how would somebody do front-lines safely?]
 * 						[no-share.] [if you share on front-lines you will be attacked]
 * 							[action is share]
 * 						[exfiltrate to relaxing people, kinda maybe]
 * 					[okay, direct-action learns last-resort needs mediation]
 * 						[this toxic waste is _killing_ me]
 * 							[so this would build efforts towards rescue-that-respects-harm]
 * 					[direct action of save lives without stopping progress]
 * 						[possibly paired with polite emotional request to help with saving]
 * 							[how save woods without stop clearcut?]
 * 								[move trees elsewhere?] <- they are trying to use bodies of tree.
 * 								okay, we found map to an mvf conflict in culture.
 * 									harm should directly make mediation.
 * [tree-cutting: business-has-need, ecologist-is-sad.this sadness and money-need needs to fuel productive communication between them.
 * 			['productive communication?' [analogous to learn-from-pain.  we have dialogue-that-grows-understanding-of-others' experience, preferably mutually]
 * 				[if one side grows understanding of the other, and not vice versa, they simply dominate because their trauma never believes it is understood]
 * 					[so, this builds validity around activists-understand-businessmen more]
 * 						[if understanding between two traumas grows without relation, we get spots of harm-rooted-in-understanding]
 * 							[why?]
 * 								[because the expression of the trauma hasn't learned that it is unneeded]
 * 										[[can you teach more around ... trauma is behavior is feeling or whatnot?  karl is imagining a happy life learning a new way it can hurt, which produces a spreading pattern.  he's describing the pattern as having feelings and needs.  that it needs to be 'understood' and is a 'trauma'.  [the behavior for handling the trauma holds a way for evolution to accommodate it.  it reproduces it, models a story of it, and models behavior to quickly handle it.  the trauma-behavior system doesn't learn, it just stores.] [this results in people learning about the trauma, to not reproduce it.  this is roughly why we attack harmful stuff.  to reproduce the harm of not being able to handle their harm, and get the situation dealt with.] [that looks like a different pattern.  it produces trauma of conflict in cultures with the attribute, that spreads flat like the first pattern.] [it's only half the story.  huh.] [we have rebels in our power groups.  they rebel, and take over.  or we kill them.  or they kill us and everyone dies.  the last one is unacceptable, so we kill our rebels.]  [a quick-fix.  really we need communication between authority and harmful, real communication.] [we need more emergency-talking-down than sit-at-a-table-for-weeks]
 * 										[how do i communicate with hides-and-harms-me????]
 * 											[disrupting-communication-channels.]
 * 											[what is the trauma-story-reason for disrupting communication channels?]
 * 												[people use communication channels to empower rebel groups?]
 * 													[well i don't think it's flat.  it think it's recursive inwards.  the people most oppressed learn frist the importance of communication, so the powerful people engage kill-rebels by harming their communication channels.  this holds the solution to their own problem.  and they have surveillance to fix it.]
 * 														[that closes into a stable structure?] [nah that will break when secrets are acted on for harm, spreading from 'labeled-rebel'-situations.]
 * 		[... we have rebels ... this is differnt from we-need-to-talk-to-each-other.
 * 		 we-need-to-talk-to-our-rebels is different from we-need-to-talk-to-each-other.]
 * 		 I propose we-need-to-talk-to-our-rebels is new, and stems from quick-understanding of trauma-spread.
 * 		 	[maybe this is why information-is-life]
 * 		 		[we need to talk to our gods?]
 * 		 			[we don't know why poor around talk-to-boss-with-value-and-care-for-them]
 *						[probably local-labeled-rebel]
 *							[[what about... idea of control system, to manage rebels.  alter communications, even thoughts]]
 *								[[strong kill-rebels expression!]]
 *								[[very nice, builds towards mediation-exists, that they are kept alive.]]
 * [[obviously talking doesn't work.  leaders are replaced by the culture producing them [roughly on both sides].]]
 * 	so talk-between-leaders-and-rebels doesn't work.  its held by powerful individuals, but they are then outcompeted.
 * 		sounds like the meaning of the trauma wasn't understood.
* 			[industry is the AI.]
* 				[okay, we all already have an 'ai'.  but YOUR ai is better.]
* 					[why is the new-ai better, and where is it for the rebels?] <- for rebels it is values
* 						[new-ai is usually better because it is more useful for the contextual traumas
* 						 of the people who built it.]
* [[okay update mvf-culture mediation: we made a whole system that can solve any problem, and we know it is precious with certainty]
* 	[NVC didn't include that.  It skipped the system and went to the parts-of-reasons for the system.]
* 	[since the system is distributed, each person encountered only had some of the parts-of-reasons.]
* 		<above is the development of the solution to rebellion and dictatorship> <- this wasn't finished.
 * 									[they don't know the other side understands them, slightly mistranslated due to distant analogy]
 * 		[we want expression that builds reliable mediation with cause, easiest is reliable-understanding-of-want-to-talk]
 * 			[let's do without no-harm-words: I need to tell the people who caused this, how much it hurts me, and know from them, what it feels like gutturally to need to do it.]
 * 				[the reality is most people can't have that exchange with a business-causer]
 * 					[but there are lots of small business-causers who support this]
 * 					[they aren't the cause because their opinions are nurtured from learning-influences]
 * 				[we want to share an expression for having that exchange, for our groups to mediate together]
 * 	[I MUST TALK TO THE PEOPLE WHO CAUSE THIS.  I NEED TO UNDERSTAND DEEPLY AND THOROUGHLY HOW THEY KNOW IT IS IMPORTANT.  BECAUSE THIS SEEMS HORRIFYING TO ME.]
 * 		[we clearly gain together as a community from this murder, or it wouldn't be done.]
 * 		[in my normality, murder is harmful.  i clearly need to relate my normality with another normality.]
 * 		[so what happens is a rep tells you why they think it is important, and they don't learn what you think is important.  this prevents resolution]
 *
 * your sister said your father said in secret, that you were different forever afterwards.
 * i suspect you haven't mourned that enough to subconsciously structure your brain around the implications.
 * 	everything-we-might-do-to-our-brain-could-already-be-an-error-from-instinct. <- hadn't expanded prior to experience
 * 		karl only learned effective mourning shortly before his harmful experience started.
 * 			well you guessed that brain cells die from traumatic impact!  every single time!
 * 			so we assumed it because it was labeled precious
 * 				and for some reason copied that out of relevence.
 * 					[i've never been in karl's brain before, how am i supposed to know ...]
 * 			 	 		 	["this is obviously a mediation path to something harmful in karl, built
 * 			 	 		 	  with great struggle"]
 * 			 	 		 		["fix" is likely what karl should think,
 * 			 	 		 		 but this link is shaped to harm karl] <- too-far too-fast.
 * 			 	 		 		 	[for believe-boss-is-god,
 * 			 	 		 		 	 it looks like the harmful shape preserves the importance
 * 			 	 		 		 	 of defending against the possibility of mind control.]
 * 			 	 		 		 	 [for believe-there-is-no-mind-control-with-certainty,
 * 			 	 		 		 	  the reason is to remember that mind control evolves
 * 			 	 		 		 	  from AI _or_ urgent mind-reading]
 * 				 	 		 		 	[it's good to believe you're mind controlled a craft
 * 				 	 		 		 	 a boss in your head that harms you?]
 * 				 	 		 		 	 	[no, that is bad.  the only reason to do that
 * 				 	 		 		 	 	 is if it is happening and you need to know.]
 * 				 	 		 		 [... if it may happen in the future and you are unstoppable,
 * 				 	 		 		  it might make sense [but it's likely to stop you.]]
 * 				 	 		 		  	[TIs are already preserving mind-control-is-real
 * 				 	 		 		  	 in pair with trust-everyone's-expressions.]
 * 			 	 		 	<expanding with more creatively could open a communication channel to care-boss.
 * 			 	 		 	 is likely distant.>
 * 			 	 		 	 [we are 'working for the empire', obviously, to save the world]
 * 			 	 		 	 	so, yeah, the expansion would fix mind control in karl.
 * 			 	 		 	 	or relate why it is there, maybe.
 * 			 	 		 	 		we have a process expansion here that grows towards
 * 			 	 		 	 		shared knowledge of karl's inner processes.
 * 			 	 		 	 		the problem is they all speak with different assumptions.
 * 			 	 		 	 		so building the channel would also build mediation paths.
 * 			 	 	]
 * 			 ] so it's nice to explore what is good =)  often we do alter-to-know, it's very tempting.
 * 			 	  
 */
