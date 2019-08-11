# How To Make An Intelligence

The key part of intelligence is a process that designs, improves, learns from, and understands itself.  That is, a process that works with itself.

On a computer, processes are made of instructions: steps.  Working with a process means
being able to represent possible steps as some kind of structured data that can be executed.
The data structure must represent enough meaning to interconnect the steps to do something,
and it must be able to refer to itself usefully.

1. Choose a simple algorithm for collecting steps together to perform a task.
2. Choose a simple representation of steps to use for #1.
3. Implement #2 enough to run code based on a goal.

Once #3 works, the goal now is to efficiently produce a form of #1 that can develop itself,
so make sure to reuse your work, and do work that is reusable, as much as possible.

4. Choose a simple algorithm for handling if something horrible goes wrong that must
   never be repeated (trauma).  Consider storing a detailed log forever.  Consider refusing to
   continue until somebody provides enough knowledge to prevent it happening again.
5. Choose a simple algorithm for testing if #2 is correct.  Use #4 if it is not.

Note: each way to do things, is a thing the whole process can do.
