; prolog for opencog
(use-modules (ice-9 readline)) (activate-readline)
(add-to-load-path "/usr/local/share/opencog/scm")
(add-to-load-path ".")
(use-modules (opencog))
(use-modules (opencog query))
(use-modules (opencog exec))

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
	; 	simplest form: equality

(define brainstorm (ConceptNode "Brainstorm"))
	; finds a new set of data that matches a pattern

(define generalization (ConceptNode "Generalization"))
	; finds a new pattern that matches a set of data


; write steps in terms of each other with meaning
; our data is our process & code


