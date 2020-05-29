#!/usr/bin/env bash
echo "# 1 \"$1\"" > "$2"
sed 's/\([a-zA-Z]\)-\([a-zA-Z]\)/\1__\2/g' < "$1" |
sed 's/\([^[]\)\[\([^[]\)/\1(\2/g' |
sed 's/\([^]]\)\]\([^]]\)/\1)\2/g' |
sed 's/\[\[/\[/g' | sed 's/\]\]/\]/g' > "$2"
