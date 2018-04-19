NOTE: this is a draft version. Work on this document is still in prograss.


# Narrow contracts in `string_view` versus P0903R1

Currently `string_view`'s converting constructor from `const char *` has a precondition (narrow contract):
invoking it with a null pointer is undefined behavior. Paper P0903R1 proposes to widen the contract so that
passing a null pointer is equivalent to calling the default constructor instead. The declared motivation for the change is to be able to migrate `const char*` interfaces to `std::string_view`. That proposal triggered a long discussion
in the reflector. In this paper we provide the summary of the discussion. In particular, we describe the following:

1. What is the purpose of `string_view`.
2. Does/can/should `string_view` hold a not-a-string value, which is distinct from an empty string value.
3. Should `nullptr` passed as `const char *` indicate a not-a-string.
4. Can `string_view` be used as a replacement for `const char *` in interfaces?
5. How do C and C++ treat null pointers that are supposed to represent strings.
6. The scope of the change: should it be only `string_view`'s constructor, or also `string`'s constructor.
7. What is gained by keeping the contract narrow, and what is gained by making it wide.


## 1. The purpose of `string_view`

The purpose of `string_view` is indicated in [N3442](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3442.html). Previously, a uniform way of accepting a string in any form (`std::string`, a string literlal, `std::vector<char>`, array of characters of known size, pointer to a null-terminated array of characters), was to use signature:

```c++
void f(const std::string& s);
```

This often required the creation of a temporary object of type `std::string` which potenitally allocated heap memory and copied characters. This copying is not really necessary, given that the same sequence already exists somewhere. Now, `std::string_view` offers te ability to represent a string in all the above forms but without incurring any resource allocation or copying of characters. However, its interface is slightly different; in particular, `sv.data()` is not required to point a null-terminated sequence of characters:

```c++
std::vector<char> v {'A', 'B', 'C'};
std::string_view sv(v.data(), v.size());
// no null in array under sv.data()
```

This is the goal and the motivation for `std::string_view`. But now that we have it, we can explore if it could fit other
purposes as well, for instance as a replacement for interfaces that currently only accept pointers to null-terminated char
sequences, or a replacement for interfaces that currently accept `const char *` with semantics slightly different than these
of pointers to null-terminated char sequences. The motivation for P0903R1 is one such interface, which allows a null pointer as valid input. 

While the extending use cases for `std::string_view` are worth exploring, they should not compromise the first and primary goal:
uniformly handle the cases previously handled by `const std::string &` (but faster), without compromising the functionality, safety and performance features.


### 1.1. `string_view`'s contract

The contents of `string_view` consist of a pointer to the beginning of the character sequence and the size of the sequence. This bears similarity to `pair<const char*, size_t>`. However, `string_view` cannot be thought of as `pair<const char*, size_t>`. The latter simply contains a pointer and a number, and assumes no connection between the two pieces of data: they are just two numbers; e.g., `{nullptr, 10}` is fine, or a random address and a random number is fine. This is reflected in `pair`'s `operator==`: two pairs compare equal if their corresponding members all compare equal.

In contrast the intended usage of `string_view` is:

```c++
for (char ch : sv)
  read(ch);
```

Therefore the type has an *invariant*: the value of `sv.data()` is such that `sv.data()[0]`, `sv.data()[1]`, ..., `sv.data()[sv.size() - 1]` are valid rvalues. Or, in other words, `{sv.data(), sv.size()}` should represent a valid counted range. Also, `operator==`, which defines the value of any type takes inot accounts only the values of these characters, not the addresses. Thus two `string_view`s can contain different pointers, but still compare equal. This also implies a precondition on the constructor taking a pointer and a size: these two should represent a valid counted range.


## 2. Passing null pointer as `const char*`

`string_view` has a converting constructor taking argument of type `const char*`. It follows the semantics of "C-string interface", which are: the length of the string is determined by dereferencing a pointer, iterating through the array and looking for the null character. The distance from the beginning to the found null character is the stirng's length. This implies a precondition on the input: 
* The pointer cannot be null (UB otherwise).
* The pointer must point to a valid array or characters, or a single character with value '\0' (UB otherwise).
* The array pointed to must contain chatacter with value '\0' (UB otherwise).

Clearly, this constructor has a precondition, and even applying P0903R1 cannot remove it: it can only widen the precondition slightly. There are many ways to assign different semantics to type `const char*`. The one described above will be referred to as *C-string interface* in this paper.


### 2.1. Null pointers as `const char*` in C

Whenever functions in C want a single argument of type `const char*` to represent a string they use the C-string interface. This is reflected in the C Standard:

7.1.1/1
> A *string* is a contiguous sequence of characters terminated by and including the first null
character. [...] A *pointer to a string* is a pointer to its initial (lowest addressed)
character. The *length of a string* is the number of bytes preceding the null character and
the *value of a string* is the sequence of the values of the contained characters, in order.

7.1.4/1
> Each of the following statements applies unless explicitly stated otherwise in the detailed
descriptions that follow: If an argument to a function has an invalid value (such as [...]
a null pointer[...]) [...], the behavior is undefined.

C functions with C-string interface include `atoi`, `puts`, `fprintf`, `fopen`, or string manipulating functions like `strcpy`, `strcat`.

There are some exceptions to that rule. First, some functions do specify what happens when a null-pointer is passed and there are good reasons to pass one: `setlocale` (null pointer means "only get current locale, don't set anything"), `system` (null -> determine if command processor exists, non-null -> pass command to processor), `mblen` (not-null -> determine the number of bytes in multi-byte character, null -> determine if state-dependent encodings are supported). In all these cases passing null pointer simply triggers a different path in the functions. This is the C way of reducing the number of function names. In C++ we would probably have two functions per each null and non-null input.

Second, functions with bounds-checking interfaces, such as `strcpy_s` do have a defensive check for null-pointer inputs. Such an input is considered an error and results in the call to a *constraint handler* function: it might call `abort()` or ignore the situation by default, or do whatever handler is installed by in the program.

Third, functions like `strncpy` determine the length in a mixed way. an additional length `len` is provided, if `'\0'` cannot be found in the first `len` locations, value `len` is used as string length. This departs from the C-string interface, but still treats null-pointer input as invalid (UB).

Fourth, functions like `memcpy` do not take `const char*`, but `const void*` arguments, but they are still relevant in the discussion. In this case the length of the sequences is provided separately by additional argument of type `size_t`. Passing null pointers is still UB.


### 2.2. Null pointers as `const char*` in C++

C++ also follows C-string interface whenever it uses single `const char*` argument to represent a string. This includes constructors of `std::fstream`, `std::string`, `std::filesystem::path`, or argument to `string::find_first_of()`, or `std::char_traits::length()`. 

Occasionally, two `const char*` pointers are used to represent a string, in the same way as ranges are represented in STL by two iterators `[begin, end)`. This is the case for functions like `std::ctype::is`, `std::ctype::do_is` dealing with locales. These can be null pointers provided that `begin == end`. This is logical: because the length of the string can be determined without dereferencing the pointer, the pointer is allowed to be null.

Some functions in `std::char_traits` take two pointers and the size: `ct::copy(s, p, n)`, `ct::move(s, p, n)`. In each case the string is represented by the pointer to the beginning of the string and the size: a *counted nange*. Actually three arguments represent two  ranges: `[s, s + n)` and `[p, p + n)`. Again, because we do not need to dereference the pointer to determine the string's length, the pointers are allowed to be null.

Another similar situation:

```c++
template <class InputIt, class OutputIt>
OutputIt copy(InputIt first, InputIt last, OutputIt d_first);
```

This algorithm can be instantiated with three `const char*` values. THese three values represent two ranges: `[first, last)` and `[d_first, d_first + distance(first, last)]`. And again, all three pointers can be null (tey would represent two zero-sized ranges), because the sizes of the underlying sequences can be determined without dereferencing the pointers.

A question has been asked, if and how the Ranges TS handles the case of a null pointer passed as type `const char*`. The answer is, Ranges TS describes constraints on inputs representing *ranges* denoted by begin and end. A C interface for strings is not a range in the sense of Ranges TS. If we want to talk about the analogy to ranges, passing a null pointer to a function with the C-string interface would be analogous to calling:

```c++
std::for_each(v.begin(), fun); // no v.end()
```
and expecting that the algorithm will deduce that we intended a zero-sized range.

### 2.3. Other possible semantics of `const char*` arguments

The above are the semantics associated with type `const char*` in C-string interfaces. They are encouraged by the Standard library. But one can imagine functions in other libraries that interpret the value of type `const char*` differently, especially the null pointer value.

First, you can treat the null pointer value as an empty string  (`""`). Whenever you call `f(nullptr)` the effect is the same as if you called `f("")`.

Second, you can treat the null pointer value as distinct from any sting, even empty. This implies that `const char *` is similar to `optional<string>`:`optstr == nullopt` is a different state than `optstr == ""s`, and calling `f(nullptr)` can give different results than calling `f("")`.

Also, type `const char*` can be used for other purposes than representing a string: it can represent an address of a single character. Migrating such usage to `string_view` would be unwise. 


### 2.4. Is passing a null pointer where a string is expected fundamentally wrong?

Much of controversy around P0903R1 is about whether it is valid to pass a null pointer where `const char*` is expected to represent a string. If by a "string" we mean a sequence of 0 or more characters, then clearly there is no need to intentionally use the null pointer value. Any value, including empty string, can be represented by null-terminated byte sequence that the pointer need to point to. Plus, it is a well established idiom in C and C++ Standard Library that passing null pointer in such case is illegal, and therefore is an indication of the porgrammer bug. When such bug is diagnosed the code is changed so that a non-null pointer is passed to function `f()` or function `f()` is not called at all. So, at least when dealing with the Standard Library functions, passing null pointer that is supposed to indicate a string happens only temporarily, and inadvertantly, and is ideally corrected as soon as possible.

But this is only the C-string interface. In custom libraries developers can associate other semantics with a sinngle `const char *` parameters intended to represent strings. The will typically be similar to the C-string interface except that null pointer value is treated in a different manner, which can be one of:

1. Null pointer represents the zero-length string.
2. Null pointer represents a yet another value distinct from any sequence of characters. This is equivalent to `optional<string>` not containing a value, which is different even from an empty string.
3. Null pointer value is unwelcome and indicates a bug, but we want functions to accept it and deal with the bug internally at run-time, trough defensive if statements or exceptions or something similar.
4. Null pointer value is unwelcome and indicates a bug, but we expect the function to accept it and not cause language-level UB, but we fave no further expectations of what happens.

In a third party library with lots of functions taking single `const char *` to indicate a string with semantics different than the C-string interface, different functions may have different answers to the above question. This means that one cannot automatically refactor all these functions to take `some_string_view` in place of `const char *`, Instead, for each of these functions the semantics need to be determined and only then an appropriate refactoring action can be taken.

Whatever answer 1, 2, 3, 4 is chosen the semantics are different than these for the C-string interface. C-string interface, being selected in the Standard Library of C and C++, is automatically a recomentdation to the developers. Its UB cases (preconditions) are designed carefully to bring closer the concrete type `const char *` to the abstract notion of a string. Many libraries also adapt the C-string interface, and expect its preconditions as a safety feature. For the programmers that choose to adapt the C-string interface, weakening its preconditions means that the concrete type `const char *` looses the connection with the concept of a string. This will be discussed later.

## 3. Can `string_view` be used to replace `const char*` interfaces?

The question in the section title is ambiguous. This reflects the ambiguity in the discussions around P0903R1. An interface is not only a type alone, but also the semantics associated with the type. Type alone, especially a general purpose basic type like `const char*`, can be interpret by different functions in different ways.

Here is one example:

```c++
bool is_default_separator(const char * ch) { return *ch == '-'; }

const char SEPARATOR = '/';
return is_default_separator(&SEPARATOR);
```

Type `const char *` is assumed to be a reference to a single character, here represented by the object's address. No terminating null character is assumed. Migrting this usage to `string_view` would be an error. There is no concept of string associated wit this usage of type `const char *`.

The above is a very obvious case, but consider migrating the C-string interface to `string_view`. We have a user-defined function with C-string interface which forwards to the Standard Library function with C-string interface:

```c++
void display(const char* message)
{
  std::puts(message);
}
```

If we change the interface of function `display()` to take `string_view` we need to answer the quesion what should be passed to function `std::puts()`. The only candidate matching by type is `message.data()`:

```c++
void display2(string_view message)
{
  std::puts(message.data());
}
```

But `message.data()` after refactoring has different semantics than pointer `message` before refactoring. Before refactoring we had guarantee that the pointed to array is null terminated. After refactoring this guarantee does not exist. Well, it exists only for a short time after refactoring because the only usages by the callers were through the C-string interface. But now that the type of the argument has changed, so has the contract: callers are sent a message that they can also pass strings that are not null-terminated:

```c++
display2({"abcd", 2}); // supposedly allowed
display2({vec.data(), vec.size()}); // supposedly allowed
```

Technically, to fix it one can add a precondition on `display2()`: "`message.data()` should point to a null-terminated sequence", but this is probably nobody's intention.

This illustrates that a top-bottom approach to migrating C-string interfaces to `std::string_view` is impossible in general. Only bottom-up is possible in a subset of cases. 

This also illustrates that it is not possible to migrate even C-string interfaces to `std::string_view`. At least not all of them. Regardless if we accept P0903R1 or not.


## 4. Holding not-a-string value in `std::string_view`

P0903R1 simply proposes to represent a `string_view` converted from null pointer as `{nullptr, 0}`, without going into details of what use would be made of this value. In the Standard Library component we have to answer a quesition: should a value in a `string_view` converted from null pointer be distinguishable from a zero-sized string?

One thing that needs to be observed is that value `{nullptr, 0}` can be produced when constructing a `string_view` that point to a zero-sized string. This is becuse a `string_view` can be constructed from the contents of `vector<char>`, and the default constructed `vector<char>` is allowed to return `vec.data() == nullptr`. And in fact the libstdc++ implementation does this:

```c++
std::vector<char> vec {};
assert (vec.data() == nullptr); // on some implementations

std::string_view sv {vec.data(), vec.size()};
assert (sv.data() == nullptr); // on some implementations
assert (sv.empty()); 
```

This means that if P0903R1 is adopted, inside functions taking `string_view` it will be impossible to tell if the argument was constructed from a zero-sized string or from a special not-a-string value. This precludes the usages of `string_view` in places where a distinction needs to be made between not-a-string value and zero-sized-string value. 

One could ask if the same problem does not already occur in the current `std::string_view` when it is default-constructed. The answer is *no*. In the current model, `std::string_view`'s default constructor simply refers to a zero-sized string. No not-a-string value exists. No one should have any need to observe the numeric value of address `sv.data()` alone. This makes `string_view` compatible with `std::string` which also represents a zero-sized string when default-constructed.

Holding a distinct not-a-string value would be possible if another value, different than `{nullptr, 0}` is chosen. For instance:

```c++
inline const char private_global = '\0';

string_view::string_view(const char* s)
  : _data(s ? s : &private_global)
  , _size(traits::length(_data))
{}
```

But it is not clear if anyone wants this. Also, even this implementation has its problems. `operator==`, which traditionaly defines the value of an object, compares the contents of the strings: number of and values of the characters. It is not able to distinguish not-a-string value from zero-sized-string value. `std::hash<std::string_view>` does not distinguish not-a-string value from zero-sized-string value. This will make the type not refular: functions that attempt to distinguish the special not-a-string state and trigger a different control path will give different results for equal inputs (as defined by `hash`, `operator<`, `operator==`). Putting results of such functions to maps, sets, doing memoization, all these will break.

Of course, this is all acceptable if the requirement is, "anything else than immediate language-level UB". But wi will not pursue that path in tis document.

This above discussion implies that, unless more extensive changes are made to `string_view`, there are two self-consistent conceptual models for `string_view` that accepts null pointer as `const char *`.

1. Just treat it as any other zero-sized string. This is acceptable in programs where people want to have two ways of saying zero-sized string: `f("")` and `f(nullptr)`. This is also acceptable in programs that already treat a zero-sized string as inacceptable value: when all functions have a defensive if up front for the zero-sized string:

```c++
X* foo(const char* p) // desired: p != nullptr && p is not ""
{
  if (p == nullptr || *p == '\0') {
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
  
  return foo(p);
}
```

In this case conflating not-a-string with a zero-sized string is acceptable.

2. The not-a-string state is never expected to occur, but if it occurs it is treated as a bug in the program and functions that otain it should immediately stop execution, e.g. by calling `std::terminate()`, trowing an exception, or performing a comparable avasive action. In such case departing from regular/value semantics does not matter, as the program or a part thereof will be shut down anyway.

```c++
X* baz(const char* p) // desired: p != nullptr
{
  if (p == nullptr) {
    SIGNAL_BUG();
    return nullptr;   // may still be needed!!
  }
  
  return process(p);
}
```

## 5. Interchangeability of `std::string` and `std::string_view`

Given that `std::string_view` is intended to be a faster replacement for `const std::string&` a quesion need to be answered whether we can change the interface of one without changing the interface of the other.

P0903R1 proposes to widen the contract of `std::string_view`'s converting constructor. According to the theory of *design by contract*, in a correct (i.e., bug-free) program, a function with a narrower precondition `f1()` can be replaced by a function with a wider precondition `f2()` and it does not affect program correctness (measured by functions inuts satisfying their preconditions). But you cannot replace `f2()` back with `f1()`, because then the contract gets narrower. The only case when you can change from `f1()` to `f2()` and back is when both functions have identical contract.

This is relevant in preactice when dealing with functions with `std::string` as parameters. There are situations where you need to change the signature from `void f(std::string)` to `void f(const std::string&)` for performance reasosns; and sometimes you have to change the signature from `void f(const std::string&)` to `void f(std::string)`: also for performance reasons, depending on the situation (in case you need to make a copy of the argument inside the function). Contract-wise both these changes are correct: both signatures have the same contract.

Similarly, now that we have `std::string_view`, one may need to change the signature from `void f(const std::string&)` to `void f(std::string_view)`, and sometimes from `void f(std::string_view)` to `void f(std::string)`. Now, if both `std::string` and `std::string_view` have the same narrow contract in constructors (null pointer disallowed in both), the change in either direction is correct contract-wise. (Technically, `std::string` and `std::string_view` have different set of member functions, so mechanically replacing one type with the other may result in a program that does not compile. But in most of the cases we are only interested in the sequence as a whole and the only members that are used are `.begin()` and `.end()`.) But if `std::string_view` had a wider contract in constructor (null pointer is a valid input), migrating from `void f(std::string_view)` to `void f(std::string)` might introduce bugs if someone has started passing null pointers to function `f()`.

Widening the contract for `std::string_view`'s constructor breaks the interchangability of `std::string` and `std::string_view` in function arguments.

Also, the theory of *design by contract*, which was described for the purpose of Eiffel programming language, does not take into account speciffics of C++, where UB is in fact a guarantee that the programmers (at least some of them) rely on. For instance, specifying UB for conversion from null poiter to `std::string` is a guarantee that I can agree with the vendor of libstdc++ that the said conversion will throw an exception, and that the Standard will not try to compromize my agreement with the vendor. This sill be considered in more detail later. 

If widening the contract of `std::string`'s converting constructor is considered, one should answer another quesion. Should the same widening be applied to other types, such as `std::filesystem::path`? Should other member functions of `std::string` be also widened their contract? For instance, `string::find_first_of(nullptr)` is currently UB.


## 6. What is gained by keeping the contract narrow

### 6.1. The difference between bugs and precondition violations

A *Bug* (like a type-o, or misunderstanding of the interface, or reading one variable instead of another) is an inadverant logic in the code that makes the program do something else than what we intended. We cannot define it formally. But we definately do not want them inour source code. Bugs make programs misbihave and crash. Bugs may cause injuries and fatalities.

An *precondition violation* is a very formal situation. A function (or expression) has a *precondition*: a constraint on the set of values/states. This condition can be specified quite formally, e.g. "`[b, e)` represent a valid range"; sometimes the precondition can be expressed as a c++ expression, e.g. `i >= 0`. An *precondition violation* is a situation when a function has a precondition (a constraint on input values) and yet it is passed a value/state that does not meet the constraint. This is so formal that we can communicate unambigously using these terms. Even tools like compilers and static analyzers can understand the notion of precondition violations. Here, we are not talking about bad things that can happen to the outside world, or human intentions. The notion of *precondition violation* is in a different level of abstraction than the notion of a *bug*. Tools are not concerned with potential injuries, but tools can diagnose precondition violations in functions.

A precondition violation is a symptom of a bug; but it is not a bug on itself. By specifying preconditions and making the contracts narrow, you build a connection between the two notions: a precondition violation can now indicate a bug. This is possible only when there are some input values disallowed by the function. The notion of *undiefined behavior* or *invalid input* also come close to the notion of precondition violation.

Now, sometimes people are concerned with precondition violations (and invalid inputs) more than with bugs. They claim that if you get rid of preconditions (invalid inputs), you get rid of precondition violations; and when you get rid of precondition violations, you get rid of the problems. In consequence, they postulate to design function interfaces so that any input is "valid", i.e. have no preconditions. 

The first part of that claim is actually true: if you get rid of preconditions, you get rid of potential precondition violations. But with this you are only curing the symptoms and let the disease (the bug) spoil your system: and now that you have one symptom less, it will be more difficult to detect it. You simply loosen the connection between the technical notion of "precondition violation" and human notion of "bug". Consider this example containing a bug, where we inedvertantly pass a null pointer, whereas we intended to pass a different value:

```c++
const char * p = "contents.txt";
const char * q = 0;
f(q); // BUG: intended to call f(p);
```

First, suppose that function `f()` has a precondition: the argument cannot be a null pointer. If you plant this bug you are violating a precondition. Automatic tools, like static analyzers, do not know our intentions, they cannot recognize such things as "you wanted to pass different argument" or "you confused this name with that one", or "you wanted to pass that value instead", "you have a type-o". But they are good at detecting precondition violations, or illegal values. Invalid input is not a bug itself, but an indication of a bug (in our case: confusing pointers), but if this is reported it is enough for the programmer to kick in and correct it. Static analyzer has *helped* find the bug, but didn't actually find it: the programmer found it based on the information from static analyzer.

Now, consider what happens when you widen the contract. The bug is still there, the pointers are still confused, but there is no precondition fiolation anymore. Static analyzer is blind and cannot help you detect the bug.

To summarize. The goal is to detect bugs (early, statically). The notion of "precondition violation" or "invalid input" is only a tool that helps detect bugs (or, assert program correctness). The goal is not to detect invalid inputs: it is only a means to the real goal. By widening contracts you render the notion of invalid input unhelpful (or less helpful) in achieving the goal of detecting bugs.

Sometimes people worry more about hitting language-level UB than about bugs in their code. But bugs actually have the same dangerous characteristic as UB: if you hit them, the results are unpredictable.


### 6.2. Simpler conceptual model

Preconditions bring C++ types closer to the real-life concepts that they describe. Consider a function that takes a range of integer values indicated by the lower and uper bounds:

```c++
bool indicator::is_in_range(int lo, int hi) const
// precondition: lo <= hi
{
  return lo <= _value && _value <= hi;
}
```

Two value of type `int` are a concept in the C++ type-system. A *range* is a concept from the problem domain. Two types of type `int` can represent a range only to some extent. It is denoted by the precondition. If `lo` passed is greater than `hi` there is no way to think of them as a range. The abstraction breaks. One could say, "but let's make `lo > hi` nonetheless a valid input", but then what do you do with this input? One option:

```c++
bool indicator::is_in_range(int lo, int hi) const
// no precondition
{
  if (lo > hi) return false;
  return lo <= _value && _value <= hi;
}
```

Whetever you do, you are not dealing with a range now, but with two objects of type `int`. There is no notion of being *in range* for two arbitrary `int` values, and the value in the additional return statement is probably wrong, because there is no good answer. The logic of this function has descended from dealing with abstract ranges to performing operations on C++ type-system objects. Reasoning at this level is harder and more bug prone. Intuition fails, and code reviews are less effective.

Similarly, a null pointer value does not represent a string, not even a zero-sized string. Once it is accepted as valid input to `string_view` something has to be done with it: maybe call it an empty string, maybe call it a distinct value from any string value. In either case the abstraction is weakened. We cannot safely think interms of strings, character sequences, but one level down: about the numeric value of address `sv.data()`.


### 6.3. Improved static analysis

First, null pointer dereference is a common bug, breaking functionality in many popular commercial programs. Also in other languages. The ability to detect such bugs is very useful. Tools and techniques are being developed to help that task. Some tools like static analyzers need help from the programmer and the libraries: this help is provided via UB.

Any portion of code is suspicious and could technically contain a bug. In order for static analyzers not to produce too many false positive warnings, they need to be certain that a given piece of C++-conformant code does not reflect programmer's intentions. How does the static analyzer know what the programmer's intentions are? It doesn't in general, but sometimes the answer is clear: it is *never* the programmers intention to hit an UB. So, when a static analyzer can find an execution path in the program that triggers an UB, it can with certainty report a bug. But for this to work one needs a *potential* to hit an UB. Making a non-null ponter a precondition is such potential to hit an UB: a good one, because dereferencing a null pointer is well explored and tool-friendly type of UB. As explained above, this does not introduce a potential for bugs: it only makes bugs more visible.

UB is close in nature to a [checksum](https://en.wikipedia.org/wiki/Checksum): checksum is redundant, increases the  size of the message, and creates the potential for producing invalid messages (where the payload doesn't match the checksum), so someone might say, "remove it, and there will be no way to send an invalid message". But we still want to use checksums, and we all know why.


### 6.4. Flexibility in handling the bug at run-time

---------
WARNING: THE REMAINDER OF THE DOCUMENT WILL CHANGE.


## N. On narrow contracts

### N.1. Narrow-contract functions are not bug-prone

We need to clarify one thing up front. When a function `ncf()` has a <em>narrow contract</em>, it does not mean that it "has UB"
or that it is "bug prone". It means that the correct program does not invoke `ncf()` with certain values of its parameter types. When the program has no bugs, no disallowed values are passed to `ncf()`, and no UB is reached.

When the program has a bug, it may result in calling `ncf()` with a disallowed value: this in turn results in reaching UB. The C++ Standard does not specify what happens with the program in such case, but implementations may specify what happens; and many implementations do. We are focusing here on a very popular, well understood and well explored type of UB: dereferencing a null pointer. Current implementations respond to dereferencing a null pointer by:

1. Reporting error during statc analysis, before the program is even run. This is comparable to detecting bugs at compile-time.
2. When compiled with an UB-sanitizer, logging null pointer dereference and halting the program.
3. On particular implementations of the Standard Library, an exception is thrown.

Static analyzers, in order to avoid false positive warnings, need to be positive that a given situation in a standard-conformant program is a bug. Hitting UB is a certain indication that there is a bug. But for this to work you have got to have the potential to hit UB.  


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

It seems that people sometimes are more concerned about potential UB resulting from narrow contracts in the Standard than about bugs. But bugs have actually the same characteristics and consequence as UB: you do not know what is going to happen, wehn you code something else than you intended.


### N.2. Narrow contract means implementation flexibility

The Standard does not specify what happens when the precondition is violated (even in the currently proposed contract support), because there is no universal good way of handling such situation. Choosing a solution that satisfies one project in one environment, makes the solution suboptimal or inacceptable in other projects or environments. Therefore the decision is left to the programmer to choose the best option by using tools, compiler switches, `#define`s or collaborating with implementation vendor. In the case of null pointer in `string_view`'s constructor, programmers and implementation vendors can:

1. Throw an exception,
2. Signal error in any other way, statically or in run-time,
3. Go with a default-constructed range,
4. Go with a unique range value, distinct from any real empty range (e.g., `{&_unique_global, 0}`),
5. Do any of the above based on vendor-speciffic switches.

This flexibility would no longer be possible if the Standard hard-codes the behavior to a single decision.

This also means that due to this flexibility, if Abseil Authors want to implement "go with default-constructed `string_view` upon null pointer" in their implementation of `std::string_view`, that would also be a standard-conformant implementation. 


### N.3. Narrow contract is not a TBD

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

A `string_view` *is not* a pair `pair<const char*, size_t>`. The latter accepts any two values of the corresponding types and doesn't care whether they make sense or not, for instance `{reinterpret_cast<const char*>(&this), 4}` or `{(const char*)nullptr, 7}`. In contrast, an object of type `string_view` represents a *string*: a sequence of characters that *really* resides somewhere in memory.

The constructor taking a pointer `p` and a size `s` has a purpose: it is an interface for constructing a view to a string form anything that provides "counted range" interface: a pointer to the beginning of the sequence and the sequence size. This implies the precondition: `[p, p + s)` must be a valid range. You cannot just pass an arbitrary pointer and an arbitrary number. It does allow initialization `string_view((const char*)0, 0)`, but not as a "singular" value representing not-a-range, but because some containers representing valid ranges really encode the state as two zeros:

```c++
std::vector<char> v {};

assert (v.data() == nullptr); // on some implementations
assert (v.size() == 0);
```

`string_view` also provides the default constructor. Its purpose is to mimic the behavior or `std::string` which default constructs to a 0-sized range of characters. Analogously, `std::string_view` default-constructs to a zero-sized range of characters not associated with any object. *Not* in order to be consistent with `string_view{nullptr, 0}` but in order to be consistent with the interface of `std::string`.

The converting constructor taking `const char*` has a purpose: provide the *C interface for strings*. After all, `string_view` was created to provide one interface replacing both C++-style `std::string`s and C-style `const char*`. The C interface for strings is not just type `const char*` alone, but also the semantics characteristic of C-style strings, which are:
- UB if pointer is null,
- UB if pointer points to invalid memory
- UB if there is no character `'\0'` in the pointed to sequence.

Given its purpose, it is expected that this constructor also provides semantics of the C interface for strings. That is, the length of the string is determined by inspecting the pointed to array of charactes (and looking for a `'\0'`). If there is no array, the size cannot be determined.

That the constructor intended for handling C-style strings preserves both the type and the semantics of the C interface for strings seems to us more important than providing "consistency" with other constructors that were designed to handle different interfaces. What we find important is the consistency with the intended purpose for the constructor.


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
  if (p == nullptr || *p == '\0') {
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
constructor with null pointer is UB, and checking anything later, inside the function is unreliable.

We argue that maybe it is not a good idea to change the interface of all these functions from taking `const char*` to taking `std::string_view`. The goal of `std::string_view` is not to replace *every* single ocurrence of type `const char*` in function interfaces in the program: only those ocurrences that come with the semantics of the *C interface for strings*. `const char*` can be used for other purposes. For instance:

```c++
bool is_default_separator(const char * ch) { return *ch == '-'; }

const char SEPARATOR = '/';
return is_default_separator(&SEPARATOR);
```

And everyone will agree that it does not make sense, and would be incorrect, to upgrade it to `std::string_view`.
Similarly, if a function does want a string in `const char *` but associates a different interpretation of the argument, 
migrating to `std::string_view` would cange the semantic part of the interface, and alter the program behavior, likely causing bugs. In fact, the semantics of function `foo()` above do change if the argument type is replaced with `std::string_view` modified as per P0903R1. Before it had different path for a null pointer and a different one for an empty string; after the change the two will be indistinguishable. 

The only case where the change proposed in P0903R1 does help is when it is acceptable or desired to conflate the value of a null pointer and the value of a genuine empty C string into one. This occurs in the following cases:

1. A genuine empty C string (`""`) is treated as a "degenerate" value already, and cannot ever mean any useful content, and already qualifies for a "defensive if". In that case adding null pointer to the set of degenerate values comes with zero cost and is quite natural. This is the case for function `bar()` above.

2. We want to give the callers an option to say, "I have no string to pass" in two ways, because why not? This is the case for function `baz()` above.

3. When the programmer considers that *any* well defined value is better than UB, regardless of what consequences follow. And since it is likely that there will be a "defensive if" for an empty string, just use the empty-string value to go through this defensive if.

Regarding case 1; if the specifics of the problem dictate that empty string is a degenerate value or already represents the lack of a string, then the solution to treat null pointer as an empty string makes sense. However, what we are discussing in this paper is not the legitimacy of a solution for a particular application or programming regime, but the semantics of the Standard Library type, which will be used by people solving different problems and working in different programming regimes, where passing null pointer may have different semantics than an empty string, for instance no-string, which is different value than string that is empty; or where null pointer is a bug, and programmers want it to be diagnosed by tools. Even if `""` is a no-string, it is often passed on purpose to indicate no-string, whereas passing null pointer *might* indicate the intention to pass a no-string, but additionally it is often passed by mistake due to a bug.

Regarding case 2; we do not consider it motivated enough to pay the price of not being able to statically check for bugs.

Regarding case 3; we consider it a reasonable approach to minimizing the damage caused by an incorrect use of the interfaces by the callers. This follows the line of reasoning that using *any* predictable value is better than allowing the possibility that *nothig at all* is guaranteed upon UB. However, we note that at the same time, this approach prevents the detection and removal of bugs. Ideally, bugs should be removed by the programmers: not responded to at run-time. Note that not defining behavior for null pointers in the Standard allows for both approaches to co-exist: for test builds or static analysis passes the converting constructor can directly dereference the null pointer; but for release builds (provided that you can afford the run-time penalty of the redundant check) you can switch to producung an empty string reference. This is only the QoI question of the Standard Library implementation. 

Also, when it comes to responding to bugs at run-time, in different applications there may be better ways than producing a default-constructed value, e.g., halting the program, or throwing an exception. The choice to produce an arbitrary value becomes more attractive to programmers/companies that choose to, or are forced to, not use exceptions; but on enviroments that throw exceptions this choice may be suboptimal. We believe the decision should be left to people who assemble the final products, who know best the environments on which their products will be run and who are best equipped to make the right decision. A single decision should not be imposed by the Standard on everyone.

Finally, note that we do not argue that the decision to treat null pointer as an empty string is inferior in general. We argue against enforcing this decision in the Standard Library vocabulary type. We propose the following point of view. A Standard Library type does not have to be used directly in user programs, but can be used as a building block for a tool that addresses the needs of a user. This is already the case for:

1. Operator `delete`: it is not recommended to use it directly in program logic. but it is very useful when building smart poiter classes.
2. `std::thread`: its behavior to call `std::terminate()` in destructor of a joinable thread makes it inconvinient for direct use. But you can use it (as a subobject) to build your thread classes that call `join()` or `detach()` in their destructor. 

In the similar manner, we recommend writing a derived tool based on `std::string_view` when offering semantics different than these of the C interface for strings. We show how such derived tool can be implemented in 5 lines in the Recomendations section.


### 2.3. Defensive if-statements

One argument oft repeated in the discussions is that inside the function one has to perform the check `sv.empty()` up front anyway in case the `string_view` object is empty, so why not use this check to also test for a string view created from a null pointer. But this works on false assumptions that it should be a good practice to perform such checks. Some programmers do, and some consider it a good practice; but this is also considered a poor practice by others. In the author's working environment no such checks are performed as they are simply incorrect. In our environemnt an empty string is often a valid input, for which we do not need or want to do a branch. Such argument is only convincing in environments that treat an empty string as a "degenerate" value.

It has beed further suggested that if `string_view` is changed as per D0903R1, and if someone is interested in detecting if a `string_view` has been constructed from a null pointer passed as `const char *` argument, one can use a more targeted check `sv.data() == nullptr`. We disagree with this reasoning for two reasons. First, it does not take into consideration that state  `sv.data() == nullptr` can be observed in `string_view`s constructed form real ranges existing in the program! Let us repeat the example:

```c++
std::vector<char> v {};

assert (v.data() == nullptr); // on some implementations
assert (v.size() == 0);

fun({v.data(), v.size()});
```

Default-constructing a `string_view` or constructing it from a null pointer (if it were allowed) renders state `{nullptr, 0}`; but observing state `{nullptr, 0}` does not necessarily mean that a `string_view` was default-constructed or nullptr-constructed.

Second, such suggestions work under the assumption that all programmers by "preventing bugs" mean "detecting invalid input at run-time and taking a different branch (which maybe skips some instructions)". But this is not the case. Some programmers want UB to happen precisely becuse they want to prevent bugs: not necessarily at run-time, not necessarily by taking a different branch and skipping instructions. This has been disussed above.

Also, cluttering the code with defensive checks makes functions longer and more difficult to read and comprehend. For instance, one never knows if a particular `if`-statement is defensive or is part of program logic. This in turn may cause more bugs in the future. In the author's environment, functions never check for nullptr `std::funcion`s or `std::shared_ptr`s. There is a program-wide contract that if someone passes these types to funcitons they are never null, and there is no need to check them time and again at every depth of function call chain:

```c++
void process(std::function<void()> f)
{
  // no check for: f != nullptr
  f();
  // ...
}
```
 

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
  constexpr protective_string_view (const char * p) noexcept : std::string_view(p, p ? std::char_traits<char>::length(p) : 0) {}
};
```

If the need for `string_view` + particular semantics appies to every replacement of `char*` and you never need  `string_view`
with the standard *C interface for strings*, see if you can customize the null pointer behavior in your vendor's implementation,
or if you can request of your vendor such customization.   




## 5. Acknowledgements

Ashley Hedberg and Jorg Brown devoted their time to explain in detail the use cases for null-aware `string_view`, which helped improve the discussion in this paper.

Tomasz Kamiński offered a number of useful suggestions that improved the quality of the paper.
