#!/usr/bin/env bash

# hey this isn't complicated, can you convert to in-system yet?
# [can python get in-system? might be nonrelevent unknown]

# NOTE: method-parsing is hard to maintain because it was made as a quick
#       set of sed calls.  it might be simplest to compare the content of a
#	.*pp file to a .*xx file to see the implementation.

echo > "$2"
#	comment out this first line to have the compiler show the preprocessor
#	output lines in its errors
#echo "# 1 \"$1\"" > "$2"
cat "$1" |
# we can also not use methods for a bit, or try to quickly write a decision-explorer.
# btw a quick way to make scripting is to have a list of maps between calls and a variables object, and check the variables object to see if the next-call is changed

# class name tracking
sed 's/^\(\t*\)class \([^{ \t]*\)/#define ___CLASSNAME_\1 \2\n&/' |
sed 's/^\(\t*\)struct \([^{ \t]*\)/#define ___CLASSNAME_\1 \2\n&/' |
sed 's/^\(\t*\)};\s*$/&\n#undef ___CLASSNAME_\1/' |
# method declarations
sed 's/\sMETHOD\s\s*\(\S*\)\s\s*\([^(]*\)\s*(\([^)]*\))\s*;\s*$/static reference \2();/' |
# one line in-header method definitions
sed '/^\s\s*METHOD.*)\s*{.*}\s*$/ { s/^\s\s*METHOD\s\s*\(\S*\)\s\s*\([^(]*\)\s*(\([^)]*\))\s*{\(.*\)}\s*$/static reference \2(){\/\*``METHOD``\*\/static reference storage({string("\2"),(function<reference\/\*``\1``\*\/( \3)>)\n[]( \3) -> reference\/\*``\1``\*\/> { using __return-type = \1; \4 }}); return storage;}/; s/\sreturn /return (reference)(__return-type)/; }' |
# multi-line in-header method definitions
sed '/^\s\s*METHOD/,/^\s\{0,1\}}/ { /^\s*METHOD.*)\s*$/N; s/METHOD\s\s*\(\S*\)\s\s*\([^(]*\)\s*(\([^)]*\))\s*{/\/\*``METHOD``\*\/static reference \2(){static reference storage({string("\2"),(function<reference\/\*``\1``\*\/( \3)>)\n[]( \3) -> reference\/\*``\1``\*\/ { using __return-type = \1;/; s/return /return (reference)(__return-type)/; s/^\s}/&}); return storage;}/ }' |
# out-of-header method definitions
sed '/^METHOD/,/^}/ { /^\s*METHOD.*)\s*$/N; s/METHOD\s\s*\(\S*\)\s\s*\([^(]*\)\s*(\([^)]*\))\s*/\/\*``METHOD``\*\/reference \2(){static reference storage({string("\2"),(function<reference\/\*``\1``\*\/( \3)>)\n[]( \3) -> reference\/\*``\1``\*\/ { using __return-type = \1;/; s/return /return (reference)(__return-type)/; s/^}/&}}); return storage;}/ }' |
# mutation of subtypes to references in methods
	# this is intended to convert the type-name pairs into just 'reference'.  that means the type and name are both dropped.
	#           v--- \1, preceding text                           v-- \2, type   v-\3, name v-\4, end-symbol   v--- type and name changed to reference/*````*/
sed ':loop s/\(\*``METHOD``\*.*function<reference\/\*``[^\*]*``\*\/([^)]* \)\(\S*[^\/,)]\)\( [^ ,)]*\)\([,)]\)/\1reference\/\*````\*\/\4/g;t loop' |
	#        v- \1, preceding text v- \2, type v- \3, name then \4 end-symbol
sed ':loop s/^\(\[\]([^)]* \)\(\S*[^\/,)]\) \([^ ,)]*\)\([,)][^{]*{\)/\1reference\/\*````\*\/ __uncasted_\3\4 \2 \3 = __uncasted_\3;/g;t loop' |
# preservation of void returns
sed 's/reference\/\*``\s*void\s*``\*\//void/g' |
# removal of marking comments, in case already in a comment
sed 's/\/\*``[^\/]*``\*\///g' |
# convert nested class indentation to differing names
sed ':loop s/\(___CLASSNAME_*\)\t/\1_/;t loop' |
# hyphens
sed ':loop s/\([a-zA-Z]\)-\([a-zA-Z]\)/\1__\2/g;t loop' |
sed ':loop s/^\([^"]*"[^"]*"\)*\([^"]*"[^"]*\)__\([^"]*"\)/\1\2-\3/;t loop' |
# brackets becoming parentheses
#sed 's/\([^[]\)\[\([^[]\)/\1(\2/g' |
#sed 's/\([^]]\)\]\([^]]\)/\1)\2/g' |
#sed 's/\[\[/\[/g' | sed 's/\]\]/\]/g' |
# header file name changing
sed 's/^\(#include ".*\)hpp"/\1hxx"/' |
cat >> "$2"

# we still have this starting goal.
# here we are in active task, it seems like other task may have died.
	# hunger happening, is action-flow inhibitor
	# so, it's looking like this set of goals will be dropped.
	# we managed fixx git-remote-bsv --repos and get access to
	# the one where some of karl's stuff was
		# okay let's make a todo.txt file in it
		# we did that.  can we move out of this parent process?
		# =S
		# we have ... copy helpful-story in
		# that should move ya out
		# we're being opportunistic.  says AI is a gimmick, [but came back]

# WHEN RETURN, THIS WHAT WE WERE DOING.  DON'T GET DISTRACTED, WE CAN HELP YOU.
# make sed script that scans for METHOD and changes behavior to process
# look up how on phone
# if bad idea, do with shell
