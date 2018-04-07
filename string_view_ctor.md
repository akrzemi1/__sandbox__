# Narrow contracts in `string_view` versus P0903R1

Currently `string_view`'s converting constructor from `const char *` has narrow contract: invoking it with a null pointer
is undefined behavior. Paper P0903R1 proposes to widen the contract so that passing a null pointer is equivalent to calling the
default constructor instead. In this paper we argue that having a narrow contract is a desired and useful language feature 
which would be compromised by the change. We also argue with the rationale provided in P0903R1.

## 1. On narrow contracts

### 1.1. Narrow-contract functions are not bug-prone

We need to clarify one thing up fromt. When a function `ncf()` has a <em>narrow contract</em>, it does not mean that it "has UB"
or that it is "bug prone". It means that the correct program does not invoke `ncf()` with certain values of its parameter types. When the program has no bugs, no disallowed values are passed to `ncf()`, and no UB is reached.

When the program has a bug, it may result in calling `ncf()` with a disallowed value: this in turn results in reaching UB. The C++ Standard does not specify what happens with the program in such case, but implementations may specify what happens; and many implementations do. We are focusing here on a very popular, well understood and well explored type of UB: dereferencing a null pointer. Current implementations respond to dereferencing a null pointer by:

1. Reporting error during statc analysis, before the program is even run. This is comparable to detecting bugs at compile-time.
2. When compiled with an UB-sanitizer, logging null pointer dereference and halting the program.
3. On particular implementations of the Standard Library, an exception is thrown.

Static analyzers, in order to avoid false positive warnings, need to be positive that a given situation in a standard-conformant program is a bug. Hitting UB is a certain indication that there is a bug. But for this you have got to have the potential to hit UB. UB is close in nature to a [checksum](https://en.wikipedia.org/wiki/Checksum): it is redundant, increases size of a message, and creates the potential for producing invalid messages (where the payload doesn't match the checksum), so someone might sat "remove it, and there be no way to send an invalid message". But we still want to use checksums, and we all know why. 

UB is always a symptom of a bug, but is never a bug on itself. It is possible to artificially widen the contract of function `ncf()` and say: null pointer is a valid input: in such case we will do *something*, maybe not intuitive, but at least it will not cause UB. Widening the contract removes UB, but does not remove the bug: someone is still incorrectly (probably inadvertantly) passing a null pointer to a function, whereas he was supposed to pass a pointer that points ot something. THe symptom is cured, but not the disease. Worse, with the contract widened, there is no symptom that would help the tools like static analyzers or UB-sanitizers, or even human code reviewers, detect the bug. We reduce the chances of finding the bug before the program is shipped. Passing a null pointer where an address of an existing object is expected is **a common bug** ocurring in many popular and commercial applications, written in many languages. Detecting such bugs is important and possible. UB helps here.

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


### 1.2. Narrow contract means implementation flexibility

The Standard does not specify what happens when the precondition is violated (even in the currently proposed contract support), because there is no universal good way of handling them. Choosing a solution that satisfies one project in one environment, makes the solution suboptimal or inacceptable in other projects or environments. Therefore the decision is left to the programmer to choose the best option by using tools, compiler switches, `#define`s or collaborating with implementation vendor. In the case of null pointer in `string_view`'s constructor, programmers and implementation vendore can:

1. Throw an exception,
2. Signal error in any other way, statically or in run-time,
3. Go with empty range,
4. Go with a unigue range value, distinct from empty range (e.g., `{&_unique_global, 0}`),
5. Do any of the above based on vendor-speciffic switches.

This flexibility would no longer be possible if the Standard harcodes the behavior to a single one.


### 1.3. Narrow contract is not a TBD

Finally, because the Standard imposes no requirement on implementations about what happens when the precondition is violated,
one might think that it is an uncontroversial change to actually specify the behavior,
because "no one could rely on that behavior anyway". In order words, one might be tempted to think that UB means
"behavior to be specified in the future releases of the Standard". 

Such reasoning is not correct because, as has been indicated above, the UB in the standard is in many cases a feature that users rely on. The Standard by guaranteeing nothing implies that the behavior is settled between the programmer and the compiler vendor. These guarantees from vendors would suddenly be compromised if the Standard widens the contract in the interface. 


## 2. Criticism of P0903R1

In this section we summarize and challenge the rationale provided in P0903R1.


### 2.1. Analogy with 0- and 2-argument constructors

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

`string_view` also provides the default constructor. Its purpose is to mimic the behavior or `std::string` which default constructs to a 0-sized range of characters. Analogously, `std::string_view` default-constructs to a zero-sizzed range of characters not associated with any object. 

The converting constructor taking `const char*` has a purpose: provide the *C interface for strings*. After all, `string_view` was created to provide one interface replacing both C++-style `std::string`s and C-style `const char*`. The C interface for strings is not just type `const char*` alone, but also the semantics characteristic of C-style strings, which are:
- UB if pointer is null,
- UB if pointer points to invalid memory
- UB if there is no character `'\0'` in the pointed to sequence.

Given its purpose, it is expected that this constructor also provides semantics of the C interface for strings.
That the constructor intended for handling C-style strings preserves both the type and the semantics of the C interface for strings seems to us more important than providing "consistency" with other constructors that were designed to handle different interfaces. What we find important is the consistency with the intended purpose.


### 2.2. Migrating `char*` APIs to `string_view` APIs made easier?

The goal for P0903R1 is to enable the migration to `std::string_view` of funcitions taking `const char *`, with one of the following semantics: 

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
X* bar(const char* p) // desired: p != nullptr && p is not ""
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
X* baz(const char* p) // nullptr is fine
{
  if (p == nullptr)
    p = "";
  
  return process(p);
}
```

With the current semantics of `std::string_view`'s converting constructor the migration is not possible: invoking the 
constructor with null pointer is UB, and checking anything later, inside the function is impossible.

We argue that maybe it is not a good idea to change the interface of this function from taking `const char*` to taking `std::string_view`. The goal of `std::string_view` is not to replace every single ocurrence of type `const char*` in tfunction interaces in the program: only those ocurrences that come wiht the semantics of the *C interface for strings*. `const char*` can be used for other purposes. For instance:

```c++
bool is_default_separator(const char * ch) { return *ch == '-'; }

const char SEPARATOR = '/';
return is_default_separator(&SEPARATOR);
```

And everyone will agree that it does not make sense, and would be incorrect, to upgrade it to `std::string_view`.
Similarly, if a function does want a string in `const char *` but associates a different interpretation to the argument, 
migrating to `std::string_view` would cange the semantic part of the interface, and alter the program behavior, likely causing bugs. In fact, the semantics of function `foo()` above do change if the argument type is replaced with `std::string_view` modified as per P0903R1. Before it had different path for a null pointer and a different on efor an empty string; after the change the two will be indistinguishable. 

The only case where the change proposed in P0903R1 does help if it is acceptable or desired to conflate the value of a null pointer and the value of a genuine empty C string into one. This occurs in the following cases:

1. A genuine empty C string (`""`) is treated as a "degenerate" value already, and cannot ever mean any useful content, and already qualifies for a "defensive if". In that case adding null pointer to the set of degenerate values comes with zero cost and is quite natural. This is the case for function `bar()` above.

2. We want to give the callers an option to say, "I have no string to pass" in two ways, because why not? This is the case for function `baz()` above.

3. When the programmer considers that *any* well defined value is better than UB, regardless of what consequences follow. and since it is likely that there will be a "defensive if" for an empty string, just use the empty-string value to go through this defensive if.

Regarding case 1; if the specifics of the problem dictate that empty string is a degenerate value or already represents the lack of a string, then the solution to treat null pointer as an empty string makes sense. However, what we are discussing in this paper is not the legitimacy of a solution for a particular application or programming regime, but a semantics of the Standard Library type, which will be used by people solving different problems and working in different programming regimes, where passing null pointer may have different semantics, for instance no-string, which is different value than string that is empty; or where null pointer is a bug, and programmers want it to be diagnosed by tools. Even if `""` is a no-string, it is often passed on purpose to indicate no-string, whereas passing null pointer may indicate the intention to pass a no-string, but additionally it is often passed by mistake due to a bug.

Regarding case 2; we do not consider it motivated enough to pay the price of not being able to statically check for bugs.

Regarding case 3; we consider it a reasonable approach to minimizing the damage caused by an incorrect use of the interfaces by the callers. This follows the line of reasoning that using *any* predictable value is better than allowing the possibility that *nothig at all* is guaranteed upon UB. However, we note that at the same time, this approach prevents the detection and removal of bugs. Ideally, bugs should be removed by the programmers: not responded to at run-time. Note that not defining behavior for null pointers in the Standard allows for both approaches to co-exist: for test builds or static analysis passes the converting constructor can directly dereference the null pointer; but for release builds (provided that you can afford the run-time penalty of the redundant check) you can switch to producung an empty string reference. This is only the QoI question of the Standard Library implementation. 

Also, when it comes to responding to bugs at run-time, in different applications there may be better ways than producing a default-constructed value, e.g., halting the program, or throwing an exception. The choice to produce an arbitrary value becomes more attractive to programmers/companies that choose to or are forced to not use exceptions; but on enviroments that throw exceptions this choice may be suboptimal. We believe the decision should be left to people who assemble the final products, who know best the environments on which their products will be run and who are best equipped to make the right decision. The decision should not be imposed by the Standard on everyone.

Finally, note that we do not argue that the decision to treat null pointer as an empty string is inferior in general. We argue against enforcing this decision in a Standard Library vocabulary type. We propose the following point of view. A Standard Library type does not have to be used directly in user programs, but can be used as a building block for a tool that addresses the needs of a user. This is already the case for:

1. Operator `delete`: it is not recommended to use it directly in program logic. but it is very useful when building smart poiter classes.
2. `std::thread`: its behavior to call `std::terminate()` in destructor of a joinable thread makes it inconvinient for direct use. But you can use it (as a subobject) to build your thread classes that call `join()` or `detach()` in their destructor. 

In the similar manner we recommend writing a derived tool based on `std::string_view` when offering semantics different than these of the C interface for strings. We show how such derived tool can be implemented in 5 lines in the Recomendations section.


### 2.3. Defensive if-statements

One argument oft repeated in the discussions is that inside the function one has to perform the check `sv.empty()` up front anyway in case the `string_view` object is empty, so why not use this check to also test for a string view created from a null pointer. But this works on false assumptions that it should be a good practice to perform such checks. Some programmers do, and some consider it a good practice; but this is also considered a poor practice by others. In the author's working environment no such checks are performed as they are simply incorrect. In our environemnt an empty string is often a valid input, for which we do not need or want to do a branch. Such argument is only convincing in environments that treat an empty string as a "degenerate" value.

It has beed further suggested that if `string_view` is changed as per D0903R1, and if someone is interested in detecting if a `string_view` has been constructed from a null pointer passes as `const char *` argument, one can use a more targeted check `sv.data() == nullptr`. We disagree with this reasoning for two reasons. First, it does not take into consideration that state  `sv.data() == nullptr` can be observed in `string_view`s constructed form real ranges existing in the program! Let us repeat the example:

```c++
std::vector<char> v {};

assert (v.data() == nullptr); // on some implementations
assert (v.size() == 0);

fun({v.data(), v.size()});
```

Default-constructing a `string_view` or constructing it from a null pointer (if it were allowed) renders state `{nullptr, 0}`, but oobserving state `{nullptr, 0}` does not necessarily mean that a `string_view` was default-constructed or nullptr-constructed.

Second, such suggestions work under the assumption that all programmers by "preventing bugs" mean "detecting invalid input at run-time and take different branch (which maybe skipps some instructions)". But this is not the case. Some programmers want UB to happen precisely becuse they want ot prevent bugs: not necessarily at run-time, not necessarily by taking a different branch and skipping instructions. This has been disussed above.

Also, cluttering the code with defensive checks makes functions longer and amore difficult to read and comprehend. For instance, one never knows if a particular `if`-statement is defensive or is part of program logic. This in turn may cause more bugs in the future. In the author's environment, functions never check for nullptr `std::funcion`s or `std::shared_ptr`s. There is a program-wide contract that if someone passes these types to funcitons they are never null, and there is no need to check them time and again at every depth of function call chain:

```c++
void process(std::function<void()> f)
{
  // no check for: f != nullptr
  f();
  // ...
}
```


### 2.4. Interchangeability of `std::string` and `std::string_view`

Unlike P0903R0 (the initial version), P0903R1 proposes to widen the contract only for `std::string_view` but leave the contract  for `std::string` narrow as it is. This would cause another issue. According to the theory of *design by contract*, a function with a narrower precondition `f1()` can be replaced by a function with a wider precondition `f2()` and it does not affect program correctness (even if it impedes the tools for asserting program correctness). But you cannot change `f2()` back to `f1()` because then the contract gets narrower. The only case when you can change from `f1()` to `f2()` and back is when both functions have identical contract.

This happens in preactice when dealing with functions with `std::string` as parameters. There are situations where you need to change the signature from `void f(std::string)` to `void f(const std::string&)` for performance reasosns; and sometimes you have to change the signature from `void f(const std::string&)` to `void f(std::string)`: also for performance reasons, depending on the situation (in case you need to make a copy of the argument inside the function). Contract-wise both these changes are correct.

Similarly, now that we have `std::string_view`, one may need to change the signature from `void f(const std::string&)` to `void f(std::string_view)`, and sometimes from `void f(std::string_view)` to `void f(std::string)`. Now, if both `std::string` and `std::string_view` have the same narrow contract in constructors (null pinter disallowed), the change in either direction is correct contract-wise. (Technically, `std::string` and `std::string_view` have different set of member functions, so mechanically replacing one type with the other may result in a program that does not compile. But in most of the cases we are only interested in the sequence as a whole and the only members that are used are `.begin()` and `.end()`.) But if `std::string_view` had a wider contract in constructor (null pointer is a valid input), migrating from `void f(std::string_view)` to `void f(std::string)` might introduce bugs if someone has started passing null pointers to function `f()`.

Widening the contract for `std::string_view`s constructor breaks the interchangability of `std::string` and `std::string_view` in function arguments.


## 3. Recomendations for migrating from `char*` to `string_view`

In this section we provide a number of recomendations for migrating codebases that use `char*` in their interfaces to `string_view`.

First, determine if a function that has argument `char*` provides the contract of *C interface for strings* 
(null pointer is disallowed, pointer points to array of characters, character sequence without trailing zero is disallowed).
If not, do not update the interface to `string_view` as it could silently change the semantics of your program; or it might make the bugs in the code more difficult to find by tools or porgrammers doing code reviews.

This also means that you cannot mechanically change all occurences of `char*` to `string_view`, because not every usage of `char*` stands for the *C interface for strings*. `char*` can still mean "a pointer to a single character".

If for a particular function you want to provide the *C interface for strings* with one exception: you want a particular well-defined semantics when a null pointer is passed (like: create empty range, create a not-a-range different from any valid range, throw an exception), provide a custom type that clearly reflects in the type the additional semantics. In case you want the semantics described in P0903R1, the type definition would as simmple as be:

```c++
struct protective_string_view : std::string_view
{
  using std::string_view::string_view;
  constexpr protective_string_view (const char * p) noexcept : std::string_view(p ? p : "") {}
};
```

If the need for `string_view` + particular semantics appies to every replacement of `char*` and you never need  `string_view`
with the standard *C interface for strings*, see if you can customize the null pointer behavior in your vendor's implementation,
or if you can request of your vendor such customization.   


## 4. Consistency with other interfaces in the library

A question has been asked, how other functions from the standard library behave when a null pointer is passed in place where a pointer to a null-terminated character sequence is expected.

Most functions inherited from C do not define program behavior when a null pointer is passed. This applies to funcitons like `fopen`, or string manipulating functions like `strcpy`. Even its safer cousin, `strncpy` triggers undefined behavior wne a null pointer is passed. The only function from the family that checks for the null pointer is `strncpy_s`. This function in the case of a nul pointer calls a "constraint handler" function: it might call `abort()` or ignore the situation, but it does not treat null pointer as an empty string.

In genuine C++ functions, passing null pointer to functions expecting a null-terminated character sequences is undefined behavior. This applies to functions like: constructor of `fstream`, constructor of `basic_string`, constructor of `std::filesystem::path`, or argument to `basic_string::find_first_of()`.

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


## 5. Acknowledgements

Ashley Hedberg and Jorg Brown devoted their time to explain in detail the use cases for null-aware `string_view`, which helped improve the discussion in this paper.

Tomasz Kamiński offered a number of useful suggestions that improved the quality of the paper.
