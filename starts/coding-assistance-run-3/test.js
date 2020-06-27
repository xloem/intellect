const fifo = require('./fifo')
const child_process = require('child_process')

ui = {
	input: process.stdin,
	user_input: fifo('user_input.fifo'),
	user_output: fifo('user_output.fifo'),
	vim_input: null,
	vim_output: null,
	output: process.stdout
}

vim = child_process.spawn('vim', ['-T', 'screen'], { stdio: ['inherit','inherit','inherit'] })

vim.stdin.write('a hello!')
