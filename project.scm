; This is our intellect project.

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


