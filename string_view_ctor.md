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
and reported even without running the program. Second, it can be detected by UB-sanitizers. Third, because UB means anything can happen, implementations are
allowed to actually specify what happens. For instance, they can guarantee that an exception is thrown.

UB is a symptom of a bug but is never a bug on itself. Widening the contracts removes UBs and therefore hides the 
symptoms of bugs. The bug is still in the code but can no longer be detected by static analysis (or code reviews) or
UB sanitizers.

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

It seems that people sometimes are concened about UB in the Standard more than abou bugs. But bugs have actually the same characteristics and consequence as UB: you do not know what is going to happen, wehn you type something else than you intended.


## Narrow contract means implementation flexibility

Leaving undefined, at the Standard level, what happens on null pointer passed to `string_view`'s constructor, leaves the implementations the ability to define the behavior in a way that suits their custormers best. They can:

1. Throw an exception,
2. Signal error in any other way statically or in run-time,
3. Go with empty range,
4. Go with a unigue range value, distinct from empty range (e.g., `{&_unique_global, 0}`),
5. Do any of the above based on vendor-speciffic switches.

This flexibility is not possible if the Standard harcodes this behavior to a single one.


## Narrow contract is not a TBD

Finally, because the Standard imposes no requirement on implementations on what happens when the narrow contract is violated,
one might think that it is an uncontroversial change to actually specify the behavior,
because "no one could rely on that behavior anyway". In order words, it might be tempted to think that UB means
"behavior to be specified in the future releases of the Standard". 

This is not correct because, as has been indicated above, the UB in the standard is a feature that users rely on. The Standard by guaranteeing nothing implies that the behavior is settled between the programmer and the compiler vendor. These guarantees from vendors would suddenly be compromised if the Standard widens the contract in the interface. 


## Criticism of P0903R1

In this section we summarize and challenge the rationale provided in P0903R1.


### Analogy with 0- and 2-argument constructors

P0903R1 argues that because the default constructor and constructor `string_view((const char*)0, 0)` render the same state:
`sv.data() == nullptr` and `sv.size() == 0`, it would be "more consistent" if `string_view((const char*)0)` also rendered the same state.

We do not see value added with providing such consistency. Different constructors serve different purposes, provide different semantics, and it does not makesense to expect the same postconditions of them. 

The constructor taking a pointer `p` and a size `s` has a purpose: it is an interface for constructing form anything that provides "counted range" interface: a pointer to the beginning of the sequence and the sequence size. This implies the precondition: `[p, p + s)` must be a valid range. it allows initialization `string_view((const char*)0, 0)` not as a "singular" value representing not-a-range, but because some containers representing valid ranges really encode the state as two zeros:

```c++
std::vector<char> v {};

assert (v.data() == nullptr); // on some implementations
assert (v.size() == 0);
```

The value created by default constructor is probably not that relevant. This is becauese the default constructed object of this type will likely be overwritten with another value before it is read. So, some valid value just has to be chosen, and "all zeros" makes sense -- this is the only valid (0-size) range that can be constructed without an address to any particular object.

The converting constructor taking `const char*` has a purpose: provide the *C interface for strings*. After all, `string_view` was created to provide one interface replacing both C++-style `std::string`s and C-style `const char*`. The C interface for strings is not only the type `const char*` but also the semantics, which are:
- UB if pointer is not null,
- UB if there is no character `'\0'` in the pointed to sequence.

Given its purpose it is expected that this constructor also provides semantics of an C interface for strings.
That the constructor intended for handling C-style strings preserves both the type and the semantics of the C interface for strings seems to us more important than providing similarity with other constructors that were designed to handle different cases.


### Migrating `char*` APIs to `string_view` APIs made easier?

The goal for P0903R1 is to enable migration to `std::string_view` of funcitions like this one, taking `const char *`: 

```c++
X* foo(const char* p) // precondition: p != nullptr
{
  if (p == nullptr) {
    log_error();
    return nullptr;
  }
  
  return process(p);
}
```

as well as this one:

```c++
X* bar(const char* p) // no precondition
{
  if (p == nullptr)
    p = "";
  
  return process(p);
}
```

with a remark, "callers of `foo` can pass null or non-null pointers without worry." Next, it argues that if the type of
the argument is changed to `std::string_view` there is no way to check for null pointer input before UB is invoked.

We argue that maybe it is not a good idea to change the interface of this function from taking `const char*` to taking `std::string_view`. The goal of `std::string_view` is not to replace every single ocurrence of `const char*` in the program: only those ocurrences that come wiht the *C interface for strings*. `const char*` can be used for other purposes. For instance:

```c++
bool is_default_separator(const char * ch) { return *ch == '-'; }

const char SEPARATOR = '/';
return is_default_separator(&SEPARATOR);
```

And everyone will agree that it does not make sense to upgrade it to `std::string_view`. Similarly, if a function does want a string in `const char *` but offers a different interface, migrating to `std::string_view` would cange the semantic part of the interface, and alter the program behavior, likely causing bugs. In fact, the semantics of function `foo()` above do change if the argument type is replaced with `std::string_view` modified as per P0903R1. Before it had different path for a null pointer and a different on efor an empty string; after the change the two will be indistinguishable. The only case where it is acceptable is when the programmer's expectation is, "give me any well-defined beavior, as long as it does not crash my program and is repetible". The goal in such case is, "try to minimize the damage when the bug happens in production by arbitrarily selecting some valid value". But solving it this way prevents a more valueable approach: "help preventing the bug from ocurring in the first place, and help minimizing damage when the bug happens in production in a way that is considered best by the dev team (maybe via exceptions, maybe via `std::terminate`, maybe via some valid value, maybe via singular value)".

To summarize, P0903R1's motivation states, "if `basic_string_view(null_char_ptr)` becomes well-defined, APIs currently accepting `char*` or `const string&` can all move to `std::string_view` without worrying about whether parameters could ever be null." We do not agree with this statement, because:

1. One does not want to convert all APIs accepting `char*` to `std::string_view`: only these compliant with the C interface for strings.
2. After conversion there is still a reason to worry about. Either the callers do rely and want to trigger a particular behavior in the function, and it has silently changed; or they pass null pointer accidentaly (a bug), in which case the bug becomes concealed: you cannot use `.empty()` to chec for null pointer input, because it would blend together a valid empty string with an invalid null pointer. Some programs can afford to treat both empty string and a null pointer uniformly, but this is not the case in general.


## Recomendations for migrating from `char*` to `string_view`

In this section we provide a number of recomendations for migrating codebases that use `char*` in their interfaces to `string_view`.

First, determine if a function that has argument `char*` provides the contract of *C interface for strings* 
(null pointer is disallowed, pointer points to array of characters, character sequence without trailing zero is disallowed).
If not, do not update the interface to `string_view` as it would change the semantics of your program.

This also means that you cannot mechanically change all occurences of `char*` to `string_view`, because not every usage of `char*` stands for the *C interface for strings*.

If for a particular function you want to provide the *C interface for strings* with one exception: you want a particular well-defined semantics when a null pointer is passed (like: create empty range, create a not-a-range different from any valid range, throw an exception), provide a custom type that clearly reflects in the type the additional semantics. In case you want the semantics described in P0903R1, the type definition would be:

```c++
struct protective_string_view : std::string_view
{
  using std::string_view::string_view;
  protective_string_view (const char * p) : std::string_view(p ? p : "") {}
};

void foo(protective_string_view p) {
  if (p.empty()) return;
  // Process p
}
```

If the need for `string_view` + particular semantics appies to every replacement of `char*` and you never need  `string_view`
with the standard *C interface for strings*, see if you can customize the null pointer behavior in your vendor's implementation,
or if you can request of your vendor such customization.   


## Consistency with other interfaces in the library

fopen
strcpy
strncpy
strncpy_s http://en.cppreference.com/w/c/string/byte/strncpy

ifstream string
----------------------

http://wiki.edg.com/pub/Wg21jacksonville2018/P0903/d0903r1.pdf

mention optional string

