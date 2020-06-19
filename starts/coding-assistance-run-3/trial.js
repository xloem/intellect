#!/usr/bin/env node
// we do not mean to discard the old attempts.  but it seems easier to, at the moment.

//const readline = require('readline')
//const rl = readline.createInterface({
//	input: process.stdin
//})

const fifo = require('./fifo')
const child_process = require('child_process')
const load = require('./loadeval')

// let's make a simple key-val system.
// 	that can read/write
// how to make it be part-'s
// 	we could exporto the whole interface =S kinda big
// ^- a small thing is in basic_nodes.js .
//

// making-investment-without-feedback. <- keep for now
// 	we could invest in feedback but not sure how to add learning
// 	quickest, simplest learning?
// 		try-see-if-right seems appropriate
// 	link to work?
// 		1. useful to have 2 editors, yes
// 		2. it's nice to use part-'s 


console.log('begin')

ui = {
	input: process.stdin,
	user_input: fifo('user_input.fifo'),
	user_output: fifo('user_output.fifo'),
	vim_input: null,
	vim_output: null,
	output: process.stdout
}

ui.input.setRawMode(true)
ui.input.resume()

ui.input.setEncoding('utf8')
ui.user_input.setEncoding('utf8')
ui.user_output.setEncoding('utf8')
ui.output.setEncoding('utf8')

function spawnvim()
{
	vim = child_process.spawn('vim', ['-T', 'screen'])
	ui.vim_input = vim.stdin
	ui.vim_output = vim.stdout

	// copy vim output to user
	ui.vim_output.pipe(ui.output)

	vim.on('exit', spawnvim)
}
spawnvim()

//var last_data = null

// copy user input to vim
/*
ui.user_input.on('data', function(byte) {
	// USER KEYSTROKE
	ui.vim_input.write(byte)
})
*/

// let's use files that have functions that are evaled
// and have code that makes them quick
// maybe i'll pursue terminal dimensions

ui.input.on('data', function(byte) {
	// USER KEYSTROKE
	ui.vim_input.write(byte)

		// do not always do it for the user.  make sure they stay
		// knowing it.

	// some way of running functions
	// load file, evaluate content.
})

setInterval(function(){
	// imagine stuff continuously with feedback
	
	//if (last_data !== null) {
	//	process.stdout.write(last_data)
	//	last_data = null
	//}
}, 0)
