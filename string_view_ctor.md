# Narrow contracts in `string_view` versus P0903R1

Currently `string_view`'s converting constructor from `const char *` has narrow contract: invoking it with a null pointer
is undefined behavior. Paper P0903R1 proposes to widen the contract so that passing a null pointer is equivalent to calling the
default constructor instead. In this paper we argue that having a narrow contract is a desired and useful language feature 
which would be compromised by the change. We also argue with the rationale provided in P0903R1.

## On narrow contracts

### Narrow-contract functions are not bug-prone

We need to clarify one thing up fromt. When a function `ncf()` has a <em>narrow contract</em>, it does not mean that it "has UB"
or that it is "bug prone". It means that the correct program does not invoke `ncf()` with certain values of its parameter types. When the program has no bugs, no disallowed values are passed to `ncf()`, and no UB is reached.

When the program has a bug, it may result in calling `ncf()` with a disallowed value: this in turn results in reaching UB. The C++ Standard does not specify what happens with the program in such case, but implementations may specify what happens; and many implementations do. We are focusing here on a very popular, well understood and well explored type of UB: dereferencing a null pointer. Current implementations respond to dereferencing a null pointer by:

1. Reporting error during statc analysis, before the program is even run. This is comparable to detecting bugs at compile-time.
2. When compiled with an UB-sanitizer, logging null pointer dereference and halting the program.
3. On particular implementations of the Standard Library, an exception is thrown.

UB is always a symptom of a bug, but is never a bug on itself. It is possible to artificially widen the contract of function `ncf()` and say: null pointer is a valid input: in such case we will do *something*, maybe not intuitive, but at least it will not cause UB. Widening the contract removes UB, but does not remove the bug: someone is still incorrectly (probably inadvertantly) passing a null pointer to a function, whereas he was supposed to pass a pointer that points ot something. THe symptom is cured, but not the disease. Worse, with the contract widened, there is no symptom that would help the tools like static analyzers or UB-sanitizers, or even human code reviewers, detect the bug. We reduce the chances of finding the bug before the program is shipped.

This can be illustrated with an example. The following code is supposed to open the indicated file, 
interpret its contents as names, find the best matching name, and finally do something with it:

```c++
const char * fileName = "contents.txt";
const char * foundName = 0; // will be assigned to later
Names names = parse_file(foundName);
foundName = find(names);
// ...
```

There is a bug in this piece of code: pointer `fileName` was supposed to be passed to funciton `parse_file`,
but a different one with a similar name was passed instead. Type-system-wise it is a valid C++ program. 
Whether the bug is detected by tools depends on the type of parameter in `parse_file` and whether this type's converting constructor from `const char *` has a narrow or wide contract.

If the parameter type is `const char *`, this is a null-pointer-dereference type of UB, easily recognized by the tools, and clang's static analyzer issues a warning in this case.

If the parameter type is `std::string`, this is an UB in library and the vendor is allowed to inject arbitrary code in this path.
For instance, cstdlib++ chooses to throw `std::logic_error` in this case.

But if parameter is `string_view` with the widened contract in constructor (as proposed in P0903R1), 
the null pointer is replaced with an arbitrary (zero-sized) valid range, and the bug goes unnoticed: 
neither statically nor at run-time, potentially causing damage.

It seems that people sometimes are concerned about UB in the Standard more than about bugs. But bugs have actually the same characteristics and consequence as UB: you do not know what is going to happen, wehn you code something else than you intended.


### Narrow contract means implementation flexibility

The Standard does not specify what happens when the precondition is violated (even in the currently proposed contract support), because there is no universal good way of handling them. Choosing a solution that satisfies one project in one environment, makes the solution suboptimal or inacceptable in other projects or environments. Therefore the decision is left to the programmer to choose the best option by using tools, compiler switches, `#define`s or collaborating with implementation vendor. In the case of null pointer in `string_view`'s constructor, programmers and implementation vendore can:

1. Throw an exception,
2. Signal error in any other way, statically or in run-time,
3. Go with empty range,
4. Go with a unigue range value, distinct from empty range (e.g., `{&_unique_global, 0}`),
5. Do any of the above based on vendor-speciffic switches.

This flexibility would no longer be possible if the Standard harcodes the behavior to a single one.


### Narrow contract is not a TBD

Finally, because the Standard imposes no requirement on implementations about what happens when the precondition is violated,
one might think that it is an uncontroversial change to actually specify the behavior,
because "no one could rely on that behavior anyway". In order words, one might be tempted to think that UB means
"behavior to be specified in the future releases of the Standard". 

Such reasoning is not correct because, as has been indicated above, the UB in the standard is in many cases a feature that users rely on. The Standard by guaranteeing nothing implies that the behavior is settled between the programmer and the compiler vendor. These guarantees from vendors would suddenly be compromised if the Standard widens the contract in the interface. 


## Criticism of P0903R1

In this section we summarize and challenge the rationale provided in P0903R1.


### Analogy with 0- and 2-argument constructors

Proponents of P0903R1 argue that because the default constructor and the constructor `string_view((const char*)0, 0)` render the same state:
`sv.data() == nullptr` and `sv.size() == 0`, it would be "more consistent" or "symmetrical" if `string_view((const char*)0)` also rendered the same state.

We believe that speaking about the "symmetry" or "uniformity" of two constructors is illogical. How does one define when two different constructors are "consistent"? Different constructors serve different purposes, expose different signatures, provide different semantics, and it does not makesense to expect the same postconditions of them.

Let's get back to the purpose of `string_view`. It is supposed to be a replacement (type-safe, type-erased, requiring no memory allocation), primarily in function arguments, for three different ways of passing strings for reading in C++ and (by inheritance) in C:

1. As type `std::string` or a const reference to one.
2. As a pointer to NTBS (Null-terminated Byte String).
3. As a `char` array denoted the pointer to its beginning and its size.

A `string_view` *is not* a pair `pair<const char*, size_t>`. The latter accepts any two values of the corresponding types and doesn't care whether they make sense or not, for instance `{reinterpret_cast<const char*>(&this), 4}` or `{(const char*)nullptr, 7}`. In contrast, an object of `string_view` represents a *string*: a sequence of characters that *really* resides somewhere in memory.

The constructor taking a pointer `p` and a size `s` has a purpose: it is an interface for constructing a view to a string form anything that provides "counted range" interface: a pointer to the beginning of the sequence and the sequence size. This implies the precondition: `[p, p + s)` must be a valid range. You cannot just pass an arbitrary pointer and an arbitrary number. It does allow initialization `string_view((const char*)0, 0)`, but not as a "singular" value representing not-a-range, but because some containers representing valid ranges really encode the state as two zeros:

```c++
std::vector<char> v {};

assert (v.data() == nullptr); // on some implementations
assert (v.size() == 0);
```

`string_view` also provides the default constructor. For a number of reasons this constructor should not belong to the
interface of `string_view`: it does not create an object that refers to an existing string. What would be the point of declaring a function that requires a string as argument and then calling it with no string? It would still be auseful type without the default constructor. But `string_view` can be used in contexts other than function arguments, and some contexts require a two-phase initialization (first default-construct, next assign the proper value), such as `map[key] = val` in STL `map`s. In those cases the value set by the default constructor is irrelevant as long as it can be safely overwritten, because no-one will read it before it is set again in the second phase of the initialization. So, some valid value just has to be chosen, and "all zeros" makes sense -- this is the only valid (0-size) range that can be constructed without an address to any particular object.

The converting constructor taking `const char*` has a purpose: provide the *C interface for strings*. After all, `string_view` was created to provide one interface replacing both C++-style `std::string`s and C-style `const char*`. The C interface for strings is not just type `const char*` alone, but also the semantics characteristic of C-style strings, which are:
- UB if pointer is null,
- UB if pointer points to invalid memory
- UB if there is no character `'\0'` in the pointed to sequence.

Given its purpose, it is expected that this constructor also provides semantics of the C interface for strings.
That the constructor intended for handling C-style strings preserves both the type and the semantics of the C interface for strings seems to us more important than providing "consistency" with other constructors that were designed to handle different interfaces. What we find important is the consistency with the intended purpose.


### Migrating `char*` APIs to `string_view` APIs made easier?

The goal for P0903R1 is to enable migration to `std::string_view` of funcitions taking `const char *`, with one of the following semantics: 

```c++
X* foo(const char* p) // desired: p != nullptr
{
  if (p == nullptr) {
    log_error();
    return nullptr;
  }
  
  return process(p);
}
```

```c++
X* foo(const char* p) // desired: p != nullptr && p is not ""
{
  if (p == nullptr) {
    log_error();
    return nullptr;
  }
  
  if (*p == '\0') {
    log_error();
    return nullptr;
  }
  
  return process(p);
}
```

```c++
X* bar(const char* p) // nullptr is fine
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

A question has been asked, how other functions from the standard library behave when a null pointer is passed in place where a pointer to a null-terminated character sequence is expected.

Most functions inherited from C do not define program behavior when a null pointer is passed. This applies to funcitons like `fopen`, or string manipulating functions like `strcpy`. Even its safer cousin, `strncpy` triggers undefined behavior wne a null pointer is passed. The only function from the family that checks for the null pointer is `strncpy_s`. This function in the case of a nul pointer calls a "constraint handler" function: it might call `abort()` or ignore the situation, but it does not treat null pointer as an empty string.

In genuine C++ functions, passing null pointer to functions expecting a null-terminated character sequences is undefined behavior. This applies to functions like: constructor of `fstream`, constructor of `basic_string`, or argument to `basic_string::find_first_of`.

There is the case of `std::copy()` which can be passed a null pointer of type `const char *` and no UB is triggered, so we have to analyze it in more detail. The relevant signature is:

```c++
template <class InputIt, class OutputIt>
OutputIt copy(InputIt first, InputIt last, OutputIt d_first);
```
A pointer is a special case of an iterator, so this type can be instantiated with type `const char*`. Next, the semantic requirements on the input aruments are that `[first, last)` is a valid range and that `[d_first, d_first + distance(first, last))` is a valid range. These three iterators represent two ranges. For some implementations of `std::vector` the default constructor sets the vector's begin and end to a null pointer value:

```c++
std::vector<char> v {};
assert (v.begin() == nullptr);
```

We can call `std::copy` on such default constructed vector:

```c++
std::vector<char> v {}, w {};
std::copy(v.begin(), v.end(), w.begin());
```
We have passed three null pointers of type `const char *`, and the behavior is well defined. But not because the algorithm has some protective checks for null pointers, but because these null pointers represent two *valid* ranges.


A question has been asked, if and how the Ranges TS handles the case of a null pointer passed as type `const char*`. The answer is, Ranges TS describes constraints on inputs representing *ranges* denoted by begin and end. A C interface for strings is not a range in the sense of Ranges TS. If we want to talk abut the analogy to ranges, passing a null pointer to a function with the C interface for strings would be analogous to calling:

```c++
`std::for_each(v.begin(), fun); // no v.end()
```
and expecting that the algorithm will deduce that we intended a zero-sized range.


----------------------

http://wiki.edg.com/pub/Wg21jacksonville2018/P0903/d0903r1.pdf

mention optional string
UB in Standard is not "something bad will happen": it is, the vendor and you will decide.
vendor may implement as illegal memory access

We provide no specific solution, because no good one exists: we put it on vendors and programmers
We do not argue with Googles decision. We argue with making it imposed on everyone.

I think the "empty string" angle here isn't quite right. An empty string is one for which data() == data() + size, regardless of the value of that pointer. The appropriate analogy here is with the constructor string_view(nullptr, 0), which already exists today and produces an empty string.

`string_view` is not a `pair<const char *, size_t>` -- you cannot create any combination.

describe why we want a narrow contract: determine at wchich opint the bug is located and fix it there.

d0903 will not allow migration from f(string_view) to f(string)

more complicated reasoning

analogu to `delete` : you do not use vocabulary types out of the box. you prepare your own types. 

`find_first_of`

we do not argue that protecting against nullptr is bad in general. We argue about putting it into string_view.

You do not discuss narrow contracts with clients.


wider precondition: wider postcondition
Mention CRC

path constructor?
