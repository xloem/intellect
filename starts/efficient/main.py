import types
import typing
from operator import * # this module has efficient functions for every operator
                # it also has some functions that could be great parts of building translation between general small interfaces

Lookup = types.MappingProxyType

# we're generalizing trial, with plan to grow to considering
# small-education-steps used on trial generalization

# GOAL: make a function with bytecode

@dataclass
class kind:
    identifier: str
    use: typing.Callable
    way_to_pick: object

@dataclass
class need:
    relevence: object
    kind: object

@dataclass
class item:
    kind: object
    identifier: str
    needs: typing.Iterable[need]

# current plan is to make a function-representation consisting of a list of mapping from values to variables via ordered function calls
# rather than using control flow, we plan to use function-parts as data and have pre-existing functions that select from and use them (functional programming)

#@dataclass:
# SUBGOAL: dataclasses should be convertable to dictionaries, making them indexable with values in a more generalized way
# .__dict__ is the way

# 'get' is the same as 'getattr' from builtins
#def get(class_object, member:str):
#    return class_object.__dict__[member]

@dataclass
class function_workspace:
    bytecode: bytes
    values: typing.Dict[str,typing.Dict[str,int]] = {
        'local': {},
        'nonlocal': {}
    }

    value_push_bytes: Lookup = Lookup({
        'variable': b'\x7c',
        'global': b'\x74',
        #'method': b'\xa0',
    })

    def get_index_value_exists(self, kind, name):
        values = getattr(self, 'values')
        kind = str(kind)
        indexes = getitem(values, kind)
        return getitem(indexes, value)

    def add_index_value_new(self, kind, value):
        values = getattr(self, 'values')
        kind = str(kind)
        indexes = getitem(values, kind)
        index = len(value)
        setitem(indexes, value, index)
        return index

    get_index_whether_exists: Lookup({
        True: get_index_value_exists,
        False: add_index_value_new
    })

    @dataclass
    class value_reference:
        name: str
        value_type: str

    def [symbols-and-words-related-to-context-and-goals]

    def push_value(self, value: value_reference):
        # make sure that value has an index
        # that means adding value to values, if it is not already present
        # this could be done with a branch, or with a call that indexes two options based on true or false
        # condition: is value.name within values[value.value_type]
        # index on two functions: get-index-of-name and make-new-name


def value_index

def push_variable(workspace:function_workspace, name:value_reference):
    

def push_value(workspace:function_workspace, value:value_reference):
    # the way to not use control flow here is to call an index of handlers

def add_function(workspace:function_workspace, function:value_reference, arguments:typing.List[value_reference], output:value_reference):
    # beginning use of control flow
    for 
    workspace.bytecode += 

# make a function for a considered need (trial == consider-stuff)
# ignore below for now
# needs a way to make a function for a considered need
# one way involves making functions and seeing if they match
# needs make-a-new-function
# ignore above for now


# python functions use a stack and arrays of references
# the arguments are the first variables on the array

# basic bytecode: load pushes something onto stack
#                 store pops something off of stack

# a callwrap(a, b) func:
# 74 00     # push global 0
# 7c 00     # push variable 0
# 7c 01     # push variable 1
# 83 02     # call function, 2 arguments, 3 pops and pushes return value
# 53 00     # return

# an __init__(self, a, b) func:
# 7c 01     # push variable 1
# 7c 00     # push variable 0
# 5f 00     # store pop().[name 0] = pop(), in that pop-order
# 7c 02     # push variable 2
# 7c 00     # push variable 0
# 5f 01     # store pop().[name 1] = pop()
# 64 00     # push constant 0 {'None' here}
# 53 00     # return

# a new function can be made with
#    [last arg is line number mapping]
# code_objects = types.CodeType(argcount, posonlyargcount, kwonlyargcount, numvars, stacksize, flags, codebytes, constants, globals, varnames, filename, name, firstlineno, b'')
#    [second arg is globals dictionary]
# func = types.FunctionType(code_object, {})

# you can see disassembly with dis.dis(func): the second
# column shows offset into bytecode.  dis contains a list
# of operator mappings in dis.opmap[NAMESTRING]
# dis is part of cpython and documentation enumerates
# all opcodes.  can disassemble into Instruction objects
# specifying attributes with dis.Bytecode(code) or
# dis.get_instructions(code)

# note: python attributes of objects use special bytecodes,
# store_attr and load_attr likely that likely use the
# list of symbols ('names') associated with the block of
# code, as attribute indices.
# this adds another list to functions parallel to variable
# lists.

# propose production of the function happens after generation.



# considering value of including method call instead of function call.
# significantly shrinks each call, we suspect

# first we have to call getattr
# then we call function with result
# object reference (self) is passed to both

# instead we could push self once, and use method_call instead of function_call

# propose method idea is good, provides a kind of context for everything
# global methods are within 'builtins' module which can be imported
# still keeps 1 single way, possibly speeds hand development.

# value-kinds: can be local variable or attribute of something?



