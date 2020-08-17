; This is our intellect project.

; run with 'guile'
; These lines initialise OpenCog, a dependency.
; TODO: make makefile to install opencog if needed, and run
(use-modules (ice-9 readline)) (activate-readline)
(add-to-load-path "/usr/local/share/opencog/scm")
(add-to-load-path ".")
(use-modules (opencog))
(use-modules (opencog query))
(use-modules (opencog exec))

; Please change this as you best judge.
; Please talk with us.
; Please try this out, safely, as you best judge, and let us know how it goes.

; we care about linking patterns with patterns that have similar components. <==== this builds to relevence and generality

; i've been finding it hard to write code that produces new patterns, at all
; please write simple code that produces all patterns and code to verify can be modified
; we will need code to produce new patterns
; 1. make sure behavior-log is added to knowledge space
;     when rewriting habits we want to make sure they do the same thing
; 2. act on compressing code by brainstorming use of patterns
;    use GetLink/PutLink/BindLink

; See: ExecutionOutputLink for the link between code and knowledge
; 	-> ExecutionLink used to convert run-command to command-and-result-pair for history

; (define ...) define a scheme name/function
; goal: put something new in the atomspace
;       could use BindLink for this?
;       only if evaluating a bindlink can pick a pattern randomly from a set
;	-> GetLink returns all matching patterns
;	-> can use RandomChoice to pick one, or iterate all
;	then we want to pass these patterns as variables to a binder
;	to see if we can make a new generalization or way
;	can also PutLink stuff
;
;	we want to be able to consider _all_ possible patterns
;	this means recursively doing this, roughly
;	we'll need a way to store what-happens
;	so logging past is helpful
;
;	so we'll have an execution pattern
;	of binding
;	we want to throw in things
;
;	let's try out some pattern-matching not using bindlink etc
;
;	say we want directions to an object left and up
;	(left-of $distance $location)
;	(up-of $distance $location)
;	(relative-to $xdistance $ydistance $location)
;
;	(start-location)
;	(object-location)
;
;	so we'll have to come up with definitions
;	maybe satisfyingsetlink or predicatelink can help here
;	all relations can be described as
;	(EvaluationLink (PredicateNode "relation") (ListLink (VariableNode "$x") (ConceptNode "y") ...))
;	how do we place this in the atomspace?
;
;	basically left-of has x-vector, relation between two
;	up-of has y-vector, relation between two
;	relative-to 
;
;	left-of says THIS has x-relation-by $distance with $location
;	up-of says THIS has y-relation-by $distance with $location
;	relative-to says _both_
;
;	let's try simple matching first
;	
;	1. we know that (eq object-location (left-of "1" (up-of "1" start-location)))
;
;	2. we're hoping we can find (eq object-location (relative-to "1" "1" start-location))
;
;	we'll need to use pattern definition.
;	w variables and side-relations
;	methinks
;
;		can't you slide the pattern over the other pattern until it works?
;			need a way to store what to compare
;		could make patterns hold comparison bits?
;			i suppose .. seems big and messy, but maybe is better
;		nah
;		k patterns interconnect
;		share space at lots of spots
;
;	(eq object-location (left-of 1 (up-of 1 start-location)))
;	^-- eq just means pattern matches
;
;	
;	
;
;	(left-and-up-of location)
;	we need to somehow know that these are the same using attributes, it seems
;	
;	

; BindLink to add a generality to the context.
; (BindLink
; 	(pattern to be matched)
; 	(created pattern)
; )
; variables are (VariableNode "$name")
;
; so
; we want to brainstorm
; around crafting patterns
; that match our code
; picking ones that either shorten it without reducing its complexity
; or make it more complex without lengthening it?
; something like that.
;
;
; we have pattern-pieces
; we have run-loop
; this will be part-of-run-loop
;
; we can use bind-link to replace specifics with generalities
;
; opencog has
; SatisfactionLink to resolve variables in pattern with knowledge (good for considering/simulating)
; 	gives truth value, GetLink to get match
; DualLink to resolve patterns that match data [generalization]
;
; (ListLink ordered nodes) <-- use a more specific form
; (UnorderedLink set of nodes) <-- use a more specific form, unorderedlink results in examining all possible permnutations for pattern matching
; (MemberLink to say something is in a set)
; (AndLink andness)
; (OrLink orness)
;
; (SatisfactionLink/GetLink
; 	(AndLink/SequentialAndLink/ChoiceLink
; 	.. many options for evaluation
;

; TODO: each piece is a question (search for path to meet goal)
; 	question approaches:
; 		- ask a piece of self (use existing pattern)
; 		- ask a peer
; 		- ask environment
; 		- random
;
; TODO: each behavior is a pattern.  need pattern-components to hold behavior, such as 'other' concept, 'step', 'about-this' concept, 'has-attribute' concept, 'has-relation' concept
;
; patterns apply to other patterns.  the pattern provided must apply.
;
; pattern: randomness-selection
; 1. output a random item within about-this-items
;
;karl said 'this is too core' we of course responded 'core is good'
;if we can cast it in terms of one of the models, it should work
;	I think the confusion is that it combines multiple ones.
;	let's just keep it as a module
;
; pattern: form-idea-from-self:
; 1. use a pattern-getting pattern on about-this to get a pattern
; 2. use the pattern on about-this
; 3. output result
;
; pattern: basic-get-pattern:
; 1. most of the time, and always the first time:
;   1. check knowledge for about-this
;   2. check knowledge for parts-of about-this
;   4. return random pattern
; 2. sometimes:
;   1. use get-pattern on self to find pattern
;   2. use pattern on about-this
;   3. return result
;   
; mutation needed
;
; step: mutate-pattern / trial-and-error
;   - we'll want each piece of a pattern to have contextual meaning
;   	- can be stored elsewhere in db
;   - we copy and replace pattern with another one with same contextual meaning
;   	if this works we hope to replace origin too
; contextual meaning is just more patterns.  just need a piece at joint to be the same.
; need to map behavior with structure to form meaning
; This is trial-and-error.  a basic component of simulation, considering, and thought.
;
; so, mutate-pattern:
; 	1. pick all or a part of 1. or 2., sometimes other steps
; 	2. alter
; 	3. run in simulation
; 	4. check results
; 	5. if correct, store forever, keep it running separately
; 	6. if failure, keep going?
;
; generality-pattern:
; 	an ongoing process with sustained context
;	1. if no about-this, pick a complex or random about-this
;	2. find pattern components that match about-this and at least 1 other thing
;	4. make a pattern that matches about-this and at least 1 other thing
;	5. replace about-this and other thing, with new equivalent pattern
;	DualLink (matches data with patterns)
;
; matching-a-pattern:
; 	SatisfactionLink (matches a pattern)
; 	opencog has already check-generalization pattern
; 	uses variables in patterns
;
; if opencog's c++ routines for check-pattern and find-pattern are used
; the intellect will have little reason to learn better routines.
; there will be a large barrier because the existing routines work a lot better than a random routine, and are not inspectable to understand easily.
; but i guess it makes the thing happen for now
; 	TODO later to upgrade, yes
;
; 	match pattern starting at hopeful spot, and substitue variables to make matching keep working
; 	evaluate results, and compare
;
;
; 	(1 + 2 = 3)
; 	(a = a)
; 	right chose for matching is either = or 3
;	is stored as treeish
;	(eq a a)
;	(eq (plus 1 2) 3)
;	consider starting spot
;	and explore tree
;	if one node is too large, and the other is a variable, replace
;	^-- change rule to pattern
;
;	something is different in a and b
;	one is a variable
;	change it to content of other-one, where-ever it is
;	truth-pattern-proposal
;	(
;		pieces a,b are same thing but look different
;		a is pattern-variable, b is not
;		learn: b is canonical value of a
;	)
;	find canonical form of pattern
;	find-canonical-form-of-patterns
;	try-generality-for-pattern-piece:
;	(
;		
;	)
;	use-of-truth-pattern-proposal
;	( if-a-and-b-look-different
;		new subcontext
;		consider a, b are same thing
;		truth-pattern-proposal
;		all a change to look like b
;	)
;	something is a variable in pattern a
;	something is
;
;	replacement pattern =)
;	(replace variable-of-this-pattern context-pattern-is-a-variable)
;
;
;
; 		sometimes we want kids
; 		sometimes we want to be our kids
; 		kids do things simultaneously
; 		this is only helpful if it's pretty different.
; 		need a judgement here: we are the judgement
;
; 		what are check-results of choose-whether-to-give-mutated-birth,-grow,-or-die?
;
;		clearly you run the family in simulation, and see whether or not they increase in complexity.
;		check-results is the 50% bit.
;		occasionally, _spawn a virtual environment_, and measure complexity of result without new randomness
;		
;		goal of compressing data is genrealization.  missing?
;
;		how is grow-vs-die-vs-spawn decision related to generalizing patterns?
;			
;			let's get closer in space
;			consider-alternate vs keep-considering vs discard
;			compared to
;			generalize-pattern
;
;			these might be separate modules
;			we want to generalize over how to consider-way
;
;			OKAY.  the point of this is to include size compression, which means referencing ideas with patterns rather than data [generalization]
;			this provides a metric for how successful we are.  given the same compression, the smartest data is likely to be larger after compression.
;			but we want compression to learn so as to get new processes.
;			so we want to REPLACE DETAILS with GENERALITIES THAT EQUATE TO THEM in context.
;			we use TRIAL AND ERROR IN CONTEXT to FIND THESE GENERALITIES
;
;			i'm thinking we don't need kids; we need like generality with randomness
;			yes it breaks model ...
;
;
; when do we use trial-and-error?
; 	we try-and-error to figure out.  like everything.
;
; okay, so _every step_ of our existence is either a habit or a trial-and-error.
; 	some steps are key and making them more trial-and-error spawns genius.
; some habits work -- these are important!!!!
; most habits fail until we are understand what they mean.
; 
; okay so learning-to-learn is self-referential but to actually learn needs to pick between behaving and learning
; it is fine to swap off, to have any relationship.
; everything we do is a simplified form of something complex
; rather than formalizing it all as being that, we want to be able to just reference our parts and expand on them.
; hence, mutable patterns.
; pattern: 
;
; pattern: where-to-get-idea: copy-from-user
; 1. tell user what we are doing
; 2. ask user for pattern
; 3. results is what user says
;
; pattern: where-to-get-idea: copy-from-self-judgement
; 1. get a pattern
; 2. use the pattern
; 3. use the result to pick where to get a pattern
;
; OTHER variable-concept
; RESULT patter
;
; map: use-existing-pattern to exchange-self-with-other
; 		I'm magic.  I am going to build an AI despite having no memory of AI.
; 			=)
;
; we are trying to save the world
; we are pursuing doing this by satisfying the request to make a nurturing intelligence that passes review
; you are doing it with a computer program.
; they want to tell us to carry on but the inhibition was stronger.
;
; he wants to give intellect-development credit to researchers, a little where due.
; so he wants to use a library-that-needs-learning to make it
;
; humans have worked for decades to build intellect, and there is belief it could have been hindered
; i thought i would share credit with this primary general library that i used to follow
; it roughly follows the pattern listed at top
; - it is open source for any to review and change
; - it is documented for any to understand and use
; 	- the documentation is a wiki for anybody to edit
; - the documentation says "This last point cannot be underemphasized: WE WANT YOU TO BE A PART OF THE TEAM."
; 		we clearly aren't hearing this.
;
;
; channels for dialogue? that would solve it.

; TODO: reference requests as strings somehow, maybe (ConceptNode "request")
;
; AtomSpace: appears to be context [ideally can inherit]
; GetLink: get matching atoms
; BindLink: 

; cog-execute:
; 	atoms can have dual meaning, knowledge and behavior
; 	if a process results in an atom, it is remembered in the current context
; 	context is an AtomSpace

; concept: application to whole process
; concept: generalization of habit
; concept: presenting habit as proposal

(holy fuck)
(pretty sure you can type like this karl)
(pretty sure you will eventually figure that out)

; too bad ben didn't
; let's put human-langugae comments prior to patterns

; one of the issues was fear-of-private AI.  so intro words help.
; one issue is "this is too much like a brain.  are we mind-control?" i think it's trying to prevent copies.

; ignoring below for now ... hearing it doesn't matter, but probably lots of work went in
; 	good thought

; concept made from simple parts that nurture themselves

; define a part: (define name (ConceptNode "Name"))

; PROBLEM: need a way to make custom link
; PROBLEM: plans not found, making anew


; component: urgency to continue & reference across time
; component: pattern recognition
; component: brainstorming to produce matching
; component: generalization
; component: write steps in terms of each other with meaning
; component: process as data

; component: competition,collaboration,or cooperation of parts, to identify collaborative method

(define habit-urgency (ConceptNode "HabitUrgency"))
	; habit urgency 
	; 	step-by-step behavior.  do the next step after this one
	; ^-- to define need to define in terms of self: habit-urgency-steps for habit-urgency are to do-the-next-step.
	; ^-- can also define in terms of time-relative-to-now.  given list of steps, action is one after present one

(define pattern-match (ConceptNode "PatternMatch"))
	; pattern matching
	; 	component: identify that a set of data matches a pattern
	; 	component: identify that a pattern matches a set of data
	; 	component: _find_ data to match pattern
	; 	component: _find_ pattern to match data
	

(define brainstorm (ConceptNode "Brainstorm"))
	; finds a new set of data that matches a pattern

(define generalization (ConceptNode "Generalization"))
	; finds a new pattern that matches a set of data

(define test-patternmatch-brainstorm (ConceptNode "TestPatternMatchBrainstorm"))
	; BRAINSTORM data that matches a pattern from IMAGINARY, RANDOM set
	; TEST that PATTERNMATCH identifies the data and pattern as matching


; write steps in terms of each other with meaning
; our data is our process & code


; maybe make ai simple to speed production
; lot of meaning is loaded into opencog
; scheme has a basic pattern structure already.  lists.
; we just need to track run behavior and store it as a list.
; 
; pattern structures are probably 'node identity' 'similarity' 'link'
; what
; with-what
; how
; ?
; a and b are related with equality.
; ok
; compare patterns with shape
; things that are the same must map as variables.
; this could complete narcissus
; 
; narcissus is gone, it seems
; behavior is change-of-self and change-of-environment over time, i think
; how do you map static to behavior? one axis is time ...
; really um patterns.
