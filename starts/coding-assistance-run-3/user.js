#!/usr/bin/env node

const fifo = require('./fifo')
const child_process = require('child_process')

ui = {
	input: process.stdin,
	input_fifo: fifo('user_input.fifo'),
	output_fifo: fifo('user_output.fifo'),
	output: process.stdout
}

ui.input.setRawMode(true)
ui.input.resume()

//vim = child_process.spawn('vim', ['-T', 'screen'])

//ui.input.pipe(vim.stdin)
ui.input.on('data', (data) =>
{
	if (data === '\u0003') {
		process.exit()
	}
	ui.input_fifo.write(data)
})
//ui.input.pipe(ui.input_fifo)

//vim.stdout.pipe(ui.output_fifo)
//vim.stdout.pipe(ui.output)

//vim.on('exit', process.exit);//(code) => { console.log('exit',code); ui.input.close(); ui.output.close(); ui.tee.close(); console.log('closed'); process.exit(code); })
