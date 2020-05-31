#!/usr/bin/env bash

{ cat script.input | while read line
do
	echo "$line" 1>&2
	echo "$line"
	sleep 0.1
done | ./simple; } > script.output 2>&1

# mkfifo test_input.fifo
# mkfifo test_output.fifo
# mkfifo test_extra.fifo
# ./simple < test_input.fifo > test_output.fifo 2> test_error.fifo &
# {
# 	cat script.input | while read line
# 	do
# 		echo "$line"
# 		head -n 1 test_output.fifo 1>&2
# 	done > test_input.fifo &
# 	cat test_error.fifo 1>&2 &
# 	wait
# } 2> script.output
# wait

git diff script.output > script.output.diff
if [ -s script.output.diff ]
then
	cat script.output.diff
else
	cat script.output | tr '\n' '\0' | sed 's/\0\([^ ]\)/\n\1/g' | tail -n 2 | tr '\0' '\n'
fi
rm script.output.diff
git add script.output
