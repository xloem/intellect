#!/usr/bin/env bash
echo "# 1 \"$1\"" > "$2"
sed ':loop s/\([a-zA-Z]\)-\([a-zA-Z]\)/\1__\2/g;t loop' < "$1" |
sed ':loop s/^\([^"]*"[^"]*"\)*\([^"]*"[^"]*\)__\([^"]*"\)/\1\2-\3/;t loop' |
sed 's/\([^[]\)\[\([^[]\)/\1(\2/g' |
sed 's/\([^]]\)\]\([^]]\)/\1)\2/g' |
sed 's/\[\[/\[/g' | sed 's/\]\]/\]/g' >> "$2"
