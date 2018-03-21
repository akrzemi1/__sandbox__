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
one might think that it is an uncontroversial change to specify to impose the requirements,
because "no one could rely on that behavior anyway". In order words, it might be tempted to think that UB means
"to be specified in the future releases of the Standard". This is not correct because, as has been indicated above, the UB in the standard is a feature that users rely on. The Standard guarantees nothing, but implementation vendors can provide their guarantees. These guarantees are compromized if narrow contract is removed from the interface.


## Criticism of P0903R1

In this section we summarize and challenge the rationale provided in P0903R1.


### Analogy with 0- and 2-argument constructors

P0903R1 argues that because the default constructor and constructor `string_view((const char*)0, 0)` render the same state:
`sv.data() == nullptr` and `sv.size() == 0`, it would be "more consistent" if `string_view((const char*)0)` also rendered the same state.

We do not see a value added with providing such consistency. Different constructors serve different purposes, provide different semantics, and it does not makesense to expect the same postconditions of them. 

The constructor taking a pointer `p` and a size `s` has a purpose: it is an interface for constructing form anything that provides a pointer to the beginning of the sequence and the sequence size. This implies the precondition: `[p, p + s)` must be a valid range. it allows `string_view((const char*)0, 0)` not as a "singular" value representing not-a-range, but because some containers representing valid ranges really encode the state as two zeros:

```c++
std::vector<char> v {};

assert (v.data() == nullptr); // on some implementations
assert (v.size() == 0);
```

The value created by default constructor is probably not that relevant. This is becauese the default constructed object of this type will likely be overwritten with another value before it is read. So, some value just has to be chosen, and "all zeros" makes sense.

The converting constructor taking `const char*` has a purpose: provide interface for C-like APIs. After all `string_view` was created to provide one interface replacing C++-like `std::strings` and C-like `const char*`. Following its purpose it is expected that this constructor also provides semantics as these offered by the C APIs (UB on null pointer: it does not represent a 
string). That the constructor intended for handling C-like strings preserves interface and semantics characteristic of C-like strings seems to us more important than providing similarity with other constructors that were designed to handle different cases.


### Migrating `char*` APIs to `string_view` APIs made easier?

TBD...

#### Alternatives to P0903R1...

----------------------

http://wiki.edg.com/pub/Wg21jacksonville2018/P0903/d0903r1.pdf
