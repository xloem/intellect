#!/usr/bin/env bash

# hey this isn't complicated, can you convert to in-system yet?
# [can python get in-system? might be nonrelevent unknown]

# this is getting complicated.  we often spend investment-time.
# using feature-addition to propose learning-around simplifying/integrating
# (1) please learn around simplify/integrate parsing instead of adding features
# (2) complex sed is measurably slow to add anything to.  please invest time in selecting an alternative approach: [python or c++?  lexer?] [propose python]

# NOTE: method-parsing is hard to maintain because it was made as a quick
#       set of sed calls.  it might be simplest to compare the content of a
#	.*pp file to a .*xx file to see the implementation.

LINENUMBERS=0

echo > "$2"
echo "#define ___STATIC_" >> "$2"
echo "#define ___EXTERN_ extern" >> "$2"
if ((LINENUMBERS))
then
	echo "# 1 \"$1\"" >> "$2"
else
	echo "//# 1 \"$1\"" >> "$2"
fi

# we can also not use methods for a bit, or try to quickly write a decision-explorer.
# btw a quick way to make scripting is to have a list of maps between calls and a variables object, and check the variables object to see if the next-call is changed

# sorry for need for translation, but we just hit a stopping point and he
# still wants to make methods more sugary.
# can we change the implementation to make resugarization easier, please.
# the closest option is parsing the parts, and then puting them together after
# parsing, as two separate steps.

	## we have declarations and definitions
	## we have a class-type, return-type, a list of arguments,
	## and definitions have a body.
	## each argument has a type and a name.
	## in sed, we make arguments be multiple lists for their uses.

	## there is a norm for parsers where code is tokenized and lexed.
	## not familiar with much

	## hwat's most intuitive is writing a generator for these things
	## doesn't line up well with this situation

		## let us try to make a sed script that first separates
		## the components, then puts them in place?

		## alternatively we would want the parsing to link in to the
		## system.  we could use the existing system to implement it.
		## it would be made of string processing functions.

		## it would be the simplest change to use regular expressions.

# 'e': execute command in pattern space, and replace with output 
# 'e command': execute written command, and output its output; use backslash for multiple lines

sed -f - "$1" >> "$2" <<-"END"
	b skiplinenumber
	:linenumber
	n; =
	:skiplinenumber
	# class name tracking
	s/^\(\)class \([^{ \t;]*\)[^;]*$/#undef ___STATIC_\n#undef ___EXTERN_\n#define ___CLASSNAME_\1 \2\n#define ___CLASSNAMESTR_\1 "\2"\n#define ___STATIC_ static\n#define ___EXTERN_ static\n&/;t linenumber
	s/^\(\)struct \([^{ \t;]*\)[^;]*$/#undef ___STATIC_\n#undef ___EXTERN_\n#define ___CLASSNAME_\1 \2\n#define ___CLASSNAMESTR_\1 "\2"\n#define ___STATIC_ static\n#define ___EXTERN_ static\n&/;t linenumber
	s/^\(\)};\s*$/&\n#undef ___CLASSNAME_\1\n#undef ___CLASSNAMESTR_\1\n#undef ___STATIC_\n#define ___STATIC_\n#undef ___EXTERN_\n#define ___EXTERN_ extern/;t linenumber
	s/^\(\t\t*\)class \([^{ \t;]*\)[^;]*$/#define ___CLASSNAME_\1 \2\n#define ___CLASSNAMESTR_\1 "\2"\n&/;t linenumber
	s/^\(\t\t*\)struct \([^{ \t;]*\)[^;]*$/#define ___CLASSNAME_\1 \2\n#define ___CLASSNAMESTR_\1 "\2"\n&/;t linenumber
	s/^\(\t\t*\)};\s*$/&\n#undef ___CLASSNAME_\1\n#undef ___CLASSNAMESTR_\1/;t linenumber
	# method declarations
	s/^\(\t*\)METHOD\s\s*\(\S*\)\s\s*\([^(]*\)\s*(\([^)]*\))\s*;\s*$/\1DECLARE basic-\3;\n\1DECLARE method-\3;\n\1\2 \3(\4);/;t linenumber
	# one line in-header method definitions
	/^\t*METHOD.*)\s*{.*}\s*$/ { s/^\(\t*\)METHOD\s\s*\(\S*\)\s\s*\([^(]*\)\s*(\([^)]*\))\s*{\(.*\)}\s*$/\1DEFINE-REGISTER method-\3 register-method(storage, basic-\3(), ___CLASSNAMESTR\1, "\3");\n\1\2 \3(\4) { ret\/\*````\*\/urn (\*this)(method-\3(), \/\*````typeremove````\*\/{ \4}); }\n\1static reference \& basic-\3(){\/\*``METHOD``\*\/static reference storage({string("\3"),(function<reference\/\*``\2``\*\/(reference\/\*````\*\/, \4)>)\n[](reference __uncasted_self, \4) -> reference\/\*``\2``\*\/> { using __return-type = \2; ___CLASSNAME\1 self = __uncasted_self; \5 }}); return storage;}/; s/\sreturn /return (reference)(__return-type)/; b linenumber; }
	# multi-line in-header method definitions
	/\tMETHOD/,/^\t}/ { /^\t*METHOD.*)\s*$/N; s/^\(\t*\)METHOD\s\s*\(\S*\)\s\s*\([^(]*\)\s*(\([^)]*\))\s*{/\1DEFINE-REGISTER method-\3 register-method(storage, basic-\3(), ___CLASSNAMESTR\1, "\3");\n\1\2 \3(\4) { ret\/\*````\*\/urn (\*this)(method-\3(), \/\*````typeremove````\*\/{ \4}); }\n\1\/\*``METHOD``\*\/static reference \& basic-\3(){static reference storage({string("\3"),(function<reference\/\*``\2``\*\/(reference, \4)>)\n[](reference __uncasted_self, \4) -> reference\/\*``\2``\*\/ { using __return-type = \2; ___CLASSNAME\1 self = __uncasted_self;/; s/return /return (reference)(__return-type)/; s/^\s}/&}); return storage;}/; b linenumber; }
	# out-of-header method definitions
	/^METHOD/,/^}/ { /^METHOD.*)\s*$/N; s/METHOD\s\s*\(\S*\)\s\s*\(\S*\)::\([^\t :(]*\)\s*(\([^)]*\))\s*/DEFINE-REGISTER \2::method-\3 register-method(storage, basic-\3(), "\2", "\3");\n\1 \2::\3(\4) { ret\/\*````\*\/urn (\*this)(method-\3(), \/\*````typeremove````\*\/{ \4}); }\n\/\*``METHOD``\*\/reference \& \2::basic-\3(){static reference storage({string("\3"),(function<reference\/\*``\1``\*\/(reference, \4)>)\n[](reference __uncasted_self, \4) -> reference\/\*``\1``\*\/ { using __return-type = \1; \2 self = __uncasted_self;/; s/return /return (reference)(__return-type)/; s/^}/&}}); return storage;}/; b linenumber; }
	# mutation of subtypes to references in methods
	# this is intended to convert the type-name pairs into just 'reference'.  that means the type and name are both dropped.
	#           v--- \1, preceding text                           v-- \2, type   v-\3, name v-\4, end-symbol   v--- type and name changed to reference/*````*/
	:loop s/\(\*``METHOD``\*.*function<reference\/\*``[^\*]*``\*\/([^)]* \)\(\S*[^\/,)]\)\( [^ ,)]*\)\([,)]\)/\1reference\/\*````\*\/\4/g;t loop
	#        v- \1, preceding text v- \2, type v- \3, name then \4 end-symbol \
	:loop s/^\(\[\]([^)]* \)\(\S*[^\/,)]\) \([^ ,)]*\)\([,)][^{]*{\)/\1reference\/\*````\*\/ __uncasted_\3\4 \2 \3 = __uncasted_\3;/g;t loop
END
sed -i -f "-" "$2" <<-"END"
	# declarations
	s/^\t*DECLARE\s\s*\([^(; \t]*\)\s*;/___EXTERN_ reference \& \1();/
	s/^\t*DECLARE\s\s*\(\S*\)\s\s*\([^(; \t]*\)\s*;/___EXTERN_ \1 \& \2();/
	# definitions
	s/^\t*DEFINE-REGISTER\s\s*\([^(; \t]*\)\s\s*\([^;]*\)\s*;/___STATIC_ reference \& \1() { static reference storage({string("\1")}); static int registration-barrier = ((\2),0); (void)registration-barrier; return storage; }/
	s/^\t*DEFINE\s\s*\([^(; \t]*\)\s*;/___STATIC_ reference \& \1() { static reference storage({string("\1")}); return storage; }/
	s/^\t*DEFINE\s\s*\(\S*\)\s\s*\([^(; \t]*\)\s*;/___STATIC_ \1 \& \2() { static \1 storage; return storage; }/
	s/^\t*DEFINE\s\s*\(\S*\)\s\s*\([^(; \t]*\)(\([^;]*\))\s*;/___STATIC_ \1 \& \2() { static \1 storage(\3); return storage; }/
END
sed -i -f "-" "$2" <<-"END"
	:loop
	# removal of types from member function handoff
	s/\(\/\*````typeremove````\*\/{[^}*]* \)\([^ ,}][^ \/,}]*\)\( [^ ,}]*\)\([,}]\)/\1 \3\4/;t loop
	# removal of trailing commas for single-argument methods
	s/, )/)/g
	# preservation of void returns
	s/reference\/\*``\s*void\s*``\*\//void/g
	# removal of marking comments, in case already in a comment
	s/\/\*``[^\/]*``\*\///g
	# convert nested class indentation to differing names
	s/\(___CLASSNAME_*\)\t/\1_/;t loop
	s/\(___CLASSNAMESTR_*\)\t/\1_/;t loop
	# hyphens
	:redehyphenate s/\([a-zA-Z]\)-\([a-zA-Z]\)/\1__\2/g;t redehyphenate
	:undostringdehyphenation s/^\(\([^"]*"[^"]*"\)*\)\([^"]*"[^"]*\)__\([^"]*"\)/\1\3-\4/;t undostringdehyphenation
	# header file name changing
	s/^\(#include ".*\)hpp"/\1hxx"/
	# brackets becoming parentheses
	#s/\([^[]\)\[\([^[]\)/\1(\2/g
	#s/\([^]]\)\]\([^]]\)/\1)\2/g
	#s/\[\[/\[/g' | sed 's/\]\]/\]/g
END
# line number expansion
if ((LINENUMBERS))
then
	sed -i -e '/^[0-9][0-9]*$/ { s/^\([0-9][0-9]*\)$/echo "# $((&)) \\"'"$1"'\\""/; e' -e ' }' "$2"
else
	sed -i -e '/^[0-9][0-9]*$/ { s/^\([0-9][0-9]*\)$/echo "\/\/# $((&)) \\"'"$1"'\\""/; e' -e ' }' "$2"
fi

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
