#!/usr/bin/env bash

# hey this isn't complicated, can you convert to in-system yet?
# [can python get in-system? might be nonrelevent unknown]
	
echo "# 1 \"$1\"" > "$2"
sed ':loop s/\([a-zA-Z]\)-\([a-zA-Z]\)/\1__\2/g;t loop' < "$1" |
sed ':loop s/^\([^"]*"[^"]*"\)*\([^"]*"[^"]*\)__\([^"]*"\)/\1\2-\3/;t loop' |
#sed 's/\([^[]\)\[\([^[]\)/\1(\2/g' |
#sed 's/\([^]]\)\]\([^]]\)/\1)\2/g' |
#sed 's/\[\[/\[/g' | sed 's/\]\]/\]/g' |
sed 's/^\(#include ".*\)hpp"/\1hxx"/' |
cat >> "$2"

# make sed script that scans for METHOD and changes before
# look up how on phone
# if bad idea, do with shell
