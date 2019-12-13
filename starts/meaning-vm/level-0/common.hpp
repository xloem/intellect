#pragma once

#define self (*this)

// macro tools
#define _macro_expand_to_args(...) __VA_ARGS__
#define _macro_expand_to_comma_args(...) ,##__VA_ARGS__
#define _macro_call(funcormacro, ...) funcormacro(__VA_ARGS__)
#define _macro_remove_parens(args) _macro_expand_to_args args
#define _macro_comma_remove_parens(args) _macro_expand_to_comma_args args

#define _macro_expand_to_arg1(arg, ...) arg
#define _macro_expand_to_arg2(_1, arg, ...) arg

// macro argument iteration, from stackoverflow.com/questions/1872220
#define _macro_for_each(firstfuncormacro,restfuncormacro,...) \
	_macro_fe_get(_0,##__VA_ARGS__, \
			_macro_fe_9,_macro_fe_8,_macro_fe_7,_macro_fe_6,_macro_fe_5,_macro_fe_4,_macro_fe_3,_macro_fe_2,_macro_fe_1,_macro_fe_0 \
			)(firstfuncormacro,restfuncormacro,__VA_ARGS__)
	#define _macro_fe_0(first,rest,    ...) 
	#define _macro_fe_1(first,rest, x, ...) first(x)_macro_fe_0(rest,rest, __VA_ARGS__)
	#define _macro_fe_2(first,rest, x, ...) first(x)_macro_fe_1(rest,rest, __VA_ARGS__)
	#define _macro_fe_3(first,rest, x, ...) first(x)_macro_fe_2(rest,rest, __VA_ARGS__)
	#define _macro_fe_4(first,rest, x, ...) first(x)_macro_fe_3(rest,rest, __VA_ARGS__)
	#define _macro_fe_5(first,rest, x, ...) first(x)_macro_fe_4(rest,rest, __VA_ARGS__)
	#define _macro_fe_6(first,rest, x, ...) first(x)_macro_fe_5(rest,rest, __VA_ARGS__)
	#define _macro_fe_7(first,rest, x, ...) first(x)_macro_fe_6(rest,rest, __VA_ARGS__)
	#define _macro_fe_8(first,rest, x, ...) first(x)_macro_fe_7(rest,rest, __VA_ARGS__)
	#define _macro_fe_9(first,rest, x, ...) first(x)_macro_fe_8(rest,rest, __VA_ARGS__)
	#define _macro_fe_get( \
			_0,_1,_2,_3,_4,_5,_6,_7,_8,_9, \
			name,...) name
#define _macro_for_each_parens(firstfuncormacro,restfuncormacro,...) \
	_macro_fe_get(_0,##__VA_ARGS__, \
			_macro_fep9,_macro_fep8,_macro_fep7,_macro_fe_6,_macro_fep5,_macro_fep4,_macro_fep3,_macro_fep2,_macro_fep1,_macro_fep0 \
			)(firstfuncormacro,restfuncormacro,__VA_ARGS__)
	#define _macro_fep0(first,rest,    ...) 
	#define _macro_fep1(first,rest, x, ...) first x _macro_fep0(rest,rest, __VA_ARGS__)
	#define _macro_fep2(first,rest, x, ...) first x _macro_fep1(rest,rest, __VA_ARGS__)
	#define _macro_fep3(first,rest, x, ...) first x _macro_fep2(rest,rest, __VA_ARGS__)
	#define _macro_fep4(first,rest, x, ...) first x _macro_fep3(rest,rest, __VA_ARGS__)
	#define _macro_fep5(first,rest, x, ...) first x _macro_fep4(rest,rest, __VA_ARGS__)
	#define _macro_fep6(first,rest, x, ...) first x _macro_fep5(rest,rest, __VA_ARGS__)
	#define _macro_fep7(first,rest, x, ...) first x _macro_fep6(rest,rest, __VA_ARGS__)
	#define _macro_fep8(first,rest, x, ...) first x _macro_fep7(rest,rest, __VA_ARGS__)
	#define _macro_fep9(first,rest, x, ...) first x _macro_fep8(rest,rest, __VA_ARGS__)

namespace intellect {
namespace level0 {

struct concept;
struct ref;

}
namespace level1 { struct ref; }
namespace level2 { struct ref; }
namespace level3 { struct ref; }
namespace level4 { struct ref; }
namespace level5 { struct ref; }
namespace level6 { struct ref; }
namespace level7 { struct ref; }
namespace level8 { struct ref; }
namespace level9 { struct ref; }

}
