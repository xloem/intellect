// this process loads a dynamic library built by dylfunc_compile
// and executes it with the passed data

// we are moving towards judging that we no longer need dynamic loading
// and could just compile new code ... it seems valuable but slow-start.
// why not just build processes?
// 	need to structure input and output
//
// okay, let's look into atomspace serialization
