// Remember Failure Indicates Bad-Goal, to prevent hack-computer
//
// [ ] please merge this into meaning-vm.  this could aid human-communication-during-nascent-growth
// 1. [ ] Make small functions that can produce code that copies themselves by exploration.
	// - [ ] use mprotect() to launch code
//    [ ] Verify they do this, and then safely switch to using them instead of the source code.
//    	// - [ ] when there are bugs, use a disassembler or merge with meaning-vm
// 2. [ ] Produce code that makes a safe bubble to try unknown code in.  Prevent this code from being altred by #1.
	// - [ ] copy from previous work a start for how to launch a subprocess and discern success?
// 3. [ ] Combine 1&2 to produce learning.  If no learning, shrink the functions to smaller components, and build focus around
//    combining them in new ways.  Produce a path towards a goal of learning what choices work better.

// To clarify one effective learning path, we want to learn what decisions work better in the exploration process.

/*
template <typename T>
class vector {
public:
	vector(size_t length, size_t allocated = 64)
	: allocated(allocated)
	  data(malloc(sizeof(T) * allocated)),
	  length(0)
	{
		resize(length);
	}
	~vector()
	{
		free(data);
	}

	T & operator[](size_t index)
	{
		return data[index];
	}

	void resize(size_t new_length)
	{
		while (new_length > allocated) {
			allocated *= 2;
			data = realloc(data, allocated);
		}
		length = new_length;
	}

	size_t size() const
	{
		return length;
	}

private:
	size_t allocated;
	T * data;
	size_t length;
};

template <typename A, typename B>
class pair
{
public:
	pair(A a, B b)
	: a(a), b(b)
	{ }

	A a;
	B b;
}

template <typename T>
class priority_queue : public vector<pair<double, T>>
{
public:

	// each entry has a priority, and they are kept in sorted order
	// we can use the binary-search paradigm
	
};
*/

#include <vector>
#include <map>

template <typename T>
class vector
{
public:
	T & operator[](size_t index) { return data[index]; }
	void resize(size_t new_length) { data.resize(new_length); }
	size_t size() const { return data.size(); }
private:
	std::vector<T> data;
};

template <typename _Priority, typename T>
class priority_queue
{
public:
	using Priority = _Priority;
	void store(Priority priority, T item)
	{
		data.insert(priority, item);
	}
	template <typename T>
	void store(T const & others)
	{
		data.insert(T.begin(), T.end());
	}
	T peek()
	{
		return *data.begin();
	}
	T use()
	{
		auto it = data.begin();
		T ret = *it;
		data.erase(it);
		return ret;
	}
private:
	std::multimap<Priority, T> data;
};



template <typename F>
using function_provider_A = F(*)();

template <typename Workpoint>
using idea_improvement_A = void(*)(Workpoint & workpoint);

template <typename Workpoint>
using idea_improvement_generator_A = idea_improvement_A(*)(Workpoint & workpoint)

// we need way to pick what code to use to engage workpoint.
// i think it can just be idea_improvement
// we want it to grow, so it will want to use a function to pick its
// behavior.
/*
template <typename Workpoint, typename Priority, typename Idea>
using brainstorm_explorer_token = std::unique_ptr<Workpoint>(*)(Workpoint&, void(*)(), function_provider_A<idea_generation_A<Priority,Index>>);
template <typename Workpoint, typename Priority, typename Idea>
using brainstorm_explorer_A = std::unique_ptr<Workpoint>(*)(Workpoint&,function_provider_A<brainstorm_explorer_token<Priority,Index>>, function_provider_A<idea_generation_A<Priority,Index>>);
*/

template <typename Workpoint>
void try_all_values(Workpoint & workpoint)
{
	auto & data = workpoint.data[try_all_values<Workpoint>];
	if (!data) { data = decltype(workpoint.ideas::Priority)(0); }
	Workpoint::Part a;
	for (a = std::numeric_limits<Workpoint::Part>::lowest(); a < std::numeric_limits<Workpoint::Part>::max();) {
		workpoint.ideas.store(0, a);
	}
	workpoint.ideas.store(0, a);
}

template <Workpoint> idea_improvement_A<Workpoint>
try_all_values_generator()
{
	return try_all_values<Workpoint>;
}

// priority-queue is a form of workspace?
// 	priority-queue holds steps.
// 	workspace is overall.
// workspaces of brainstorming are recursion stacks.
// we will have pointers to where we are in them

class BasicWorkpoint
{
public:
	std::map<void*, void*(*)(BasicWorkpoint&)> data;

	/*
	virtual void init(BasicWorkpoint * parent);
	virtual BasicWorkpoint & workspace();
	virtual BasicWorkpoint & parent();
	*/

	virtual bool hasParent();
};

template <typename Reflection>
class LocalWorkpoint : public BasicWorkpoint
{
public:
	LocalWorkpoint(Reflection & image)
	: ideas(image.ideas),
	  workspace(image.workspace),
	  image(image)
	{ }

	using Reflection::Workspace;
	using Reflection::Part;

	decltype(Reflection::ideas) & ideas;
	decltype(Reflection::workspace) & workspace;

	Part * place() { return image.place(); }
	decltype(Reflection::parent()) parent() { return image.parent(); }
	bool hasParent() { return image.hasParent(); }

private:
	Reflection & image;
};

// confusing generalization? try more virtual functions

template <typename Parent, typename _Part = uint8_t, typename Priority = double>
class TrialWorkpoint : public BasicWorkpoint
{
public:
	using Workspace = Parent::Workspace;
	using Part = _Part;

	TrialWorkpoint(Parent * parent)
	{
		init(parent);
	};

	virtual void init(Parent * parent)
	{
		_parent = parent;
		_workspace = &parent->workspace();
		_partspot = parent->idea();
		_part = workspace().get(_partspot);
	}

	priority_queue<Priority, Part> ideas;

	Part * place()
	{
		workspace.sequence[_partspot] = _part;
		return &workspace.sequence[_partspot];
	}

	virtual Workspace & workspace()
	{
		return _workspace;
	}

	virtual Parent & parent()
	{
		return _parent;
	}

	bool hasParent() {
		return _parent != 0;
	}


protected:
	virtual Part * idea()
	{
		Part * _nextspot = workspace.nextspot(*this, _partspot);
		*_nextspot = ideas.use();
		return _nextspot;
	}

	Parent * _parent;
	Workspace * _workspace;
	Parent::Part _part;
	Workspace::PartPosition _partspot;
};

template <typename _Part = uint8_t, typename Priority = double>
class TrialWorkspace : public TrialWorkpoint<TrialWorkspace<_Part,Priority>,_Part,Priority>
{
public:
	using Workspace = TrialWorkspace;
	using Part = _Part;
	using PartPosition = signed size_t;

	TrialWorkspace()
	: TrialWorkpoint(*this, this)
	{ }

	template <typename T>
	Part * nextspot(T workpoint, Part * lastspot)
	{
		auto newspot = lastspot + 1
		auto newsize = newspot - &sequence[0];
		if (newsize > sequence.size()) {
			sequence.resize(newsize);
		}
		return newspot;
	}

	Workspace & workspace;
	vector<Part> sequence;

protected:
	virtual Part * idea()
	{
		return &sequence[0] - 1;
	}

};


// Considering-parts has been moved into one kind of function: idea-improvement.
// it can spawn new ideas or reorder them
// it takes the workpoint, and alters it as it judges.

// it could be helpful to have meaning-vm in here, so the processes would have a knowledge graph to work with and associate with their work to find when resuming.
// but, instead we are using a similar kludge.
// 	similar-kludge could be meaning-vm itself <=====
// 		we delayed merging, and now the work requires struts to merge
// 		we could at least code in the same codebase.
// don't-worry-about-it-for-now?

/*
// all we really need is a function that calls others.
template <typename IdeaImprovement, typename Priority = double>
class IdeaImprovementWorkspace : public BasicWorkpoint
{
public:
	priority_queue<Priority, IdeaImprovement> ideas;
	
	IdeaImprovement * place()
	{
		
	}

private:
	IdeaImprovement * idea()
	{
		last_idea = ideas.use();
		return &last_idea;
	}

	IdeaImprovement last_idea;
};
*/

/*
template <typename Workpoint, typename Priority, typename Idea>
std::unique_ptr<Workpoint> brainstorm(Workpoint& workpoint, brainstorm_explorer_A<Priority,Idea>& explore, idea_generation_A<Priority,Idea>& idea_generation)
{
	idea_generation(workpoint.ideas);
	return new Workpoint(workpoint);
}

template <typename Workpoint, typename Priority, typename Idea> brainstorm_explorer_A<Priority,Idea>
simple_explorer_generator(Workpoint& workpoint, brainstorm_explorer_A<Priority,Idea>& explore, idea_generation_A<Priority,Idea>& ideas)
{
	return brainstorm<Workpoint,Priority,Idea>;
}
*/

/*
// workpoint is: recommended function (choice) and thread/process to run it in (worker needing decision) with pointer to the workpoint of their work
// with shared memory, could have them cross processes, but this seems a little scary to the idea of the bubble-bounds.

// if we treat actions as workpoint we make memory.  we'll want a way to prune or change memory, to consolidate it.
class BehaviorWorkpoint
{
public:
	// NOTE: you can generalize workspaces and workpoints to your own behavior, which lets you make better decisions about how you behave.
	
	// thread, thread-workpoint, and ?
	// may need check with idea of subprocesses
	// 	hmm.  forking copies memory, right?  and then we are all-parts running int he new system.
	// 	behavior-workpoint might be better with processes than threads, which makes handing data off harder.
	//	
	// note: there is likely no need for all this structure
};
*/

// make data hold functions only.  makes life easy.

// too much functions-of-functions ...
template <typename Workpoint>
void fillinname()(Workpoint & workpoint)
{
	auto & data = workpoint.data[fillinname<Workpoint>];
	if (!data) { data = try_all_values_generator; }
	data()
	// try_all_values_generator returns try_all_values
	
	try_all_values(workpoint);
}

// so, root calls this, and it returns a new workpoint to explore
// these will be put in a priority queue
// it's a pretty basic functional unit.
// try_all_values should be changeable in the workpoint.
template <typename SourceWorkpoint, typename NextWorkpoint>
NextWorkpoint explore_step_A(SourceWorkpoint & workpoint)
{
	idea_improvement_generator_A & stepper = workpoint.data[explore_step_A];
	if (!stepper) { stepper = try_all_values_generator; }
	idea_improvement_A step = stepper(workpoint);
	try_all_values(workpoint);
	return TrialWorkpoint(workpoint);
}


int main()
{
	TrialWorkspace habitworkspace;
	

	// let's copy-in how to use mprotect() to run buffers as functions.
	// int mprotect(void *addr, size_t len, int prot);
	// violating mprotect results in SIGSEGV
	// PROT is bitwise OR of what is allowed.  PROT_READ, PROT_WRITE, PROT_EXEC
	// additionals: PROT_GROWSUP PROT_GROWSDOWN for regions that stretch linearly, needs coordination with segment mapping
	// returns 0 on success, -1&errno on failure
}
