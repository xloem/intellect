#!/usr/bin/env node
// we do not mean to discard the old attempts.  but it seems easier to, at the moment.

//const readline = require('readline')
//const rl = readline.createInterface({
//	input: process.stdin
//})

const fifo = require('./fifo')
const child_process = require('child_process')

console.log('begin')

ui = {
	user_input: fifo('user_input.fifo'),
	user_output: fifo('user_output.fifo'),
	vim_input: null,
	vim_output: null,
	output: process.stdout
}

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
ui.user_input.on('data', function(byte) {
	// USER KEYSTROKE
	ui.vim_input.write(byte)
})

setInterval(function(){
	// imagine stuff continuously with feedback
	
	//if (last_data !== null) {
	//	process.stdout.write(last_data)
	//	last_data = null
	//}
}, 0)
