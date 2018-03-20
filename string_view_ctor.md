# Narrow contracts in `string_view` versus P0903R1

Currently `string_view`'s converting constructor from `const char *` has narrow contract: invoking it with a null pointer
is undefined behavior. Paper P0903R1 proposes to widen the contract so that passing a null pointer is equivalent to calling the
default constructor instead. In this paper we argue that having a narrow contract is a desired and useful language feature 
which would be compromised by the change. We also argue with the rationale provided in P0903R1.

## Narrow contract is not UB

We need to make a distinction here. When a function has a narrow contract it does not mean it has UB
or that it is "bug prone". When the program has no bugs, no UB will ever occur. When the program has a bug, one way it
can manifest when it interacts with the narrow contract interface is by invoking UB. UB can in turn manifest in a number of ways,
especially the one with dereferencing a null pointer, which has been well studied. First, it can be deftected by static analyzers
and reported even without running the program. Second, because UB means anything can happen, implementations are
allowed to actuall specify what happens. For instance, they can guarantee that an exception is thrown.

UB is a symptom of a bug but is never a bug on itself. Widening the contracts removes UBs and therefore hides the 
symptoms of bugs: they can no longer be detected by static analysis (or code reviews).

This can be illustrated with the following example. The following code is supposed to open the indicated file, 
represent its contents as names, fint the best matching name, and finally do something with it:

```c++
const char * fileName = "contents.txt";
const char * foundName = 0; // will be assigned to later
Names names = parse_file(foundName);
foundName = find(names);
// ...
```

There is a bug in this piece of code: pointer `fileName` was supposed to be passed to funciton `parse_file`,
but a different one with a similar name was passed instead. Type-system-wise it is a valid C++ program. 
Whether the bug is detected by tools depends on the type of parameter in `parse_file` and whether this type's constructor
taking `const char *` has a narrow or wide contract.

If the parameter type is `const char *`, this is UB easily recognized by the tools, and clang's static analyzer issues 
a warning in this case.

If the parameter type is `std::string`, this is an UB in library and the vendor is allowed to inject arbitrary code in this path.
For instance, cstdlib++ chooses to throw `std::logic_error` in this case.

But if parameter is `string_view` with the widened contract in constructor (as proposed in P0903R1), 
the null pointer is replaced with an arbitrary (zero-sized) valid range, and the bug goes unnoticed: 
neither statically nor at run-time, potentially causing damage.

----------------------

http://wiki.edg.com/pub/Wg21jacksonville2018/P0903/d0903r1.pdf

