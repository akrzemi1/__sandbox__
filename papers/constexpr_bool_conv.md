This paper proposes to allow narrowing conversions in *contextually converted constant expressiosn of type `bool`*. 

Motivation
----------

Clang currently fails to compile the following program, and it is Standard-compliant:

```c++
enum Flags { Write = 1, Read = 2, Exec = 4 };

template <Flags flags>
int f() {
  if constexpr (flags & Flags::Exec) // fails to compile due to narrowing
    return 0;
  else
    return 1;
}

int main() {
  return f<Flags::Exec>(); // when instantiated like this
}
```

This is because, as currently specified, narrowing is not allowed in contextual conversion to `bool` in
core constant expressions. If compilers were standard-compliant, even the following code would be ill-formed.

```c++
template <std::size_t N> 
class Array
{
  static_assert(N, "no 0-size Arrays");
  // ...
};

Array<16> a; // fails to compile 
```

All these situations can be fixed by aplying an explicit conversion to `bool` or comparing the result to 0, 
but the fact remains that this behavior is surprising. For instance, changing `if constexpr` in the first example into 
a normal `if` makes the code compile.

Note that the proposal only affects the contextual conversions to `bool`: it does not affect implicit conversions to `bool` in other contexts. 

Background
----------

The no-narrowing requirement was added in [CWG 2039](http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#2039), which indicates it was intentional. However, the issue documentation does not state the reason.

The no-narrowing requirement looks justified in the context of `noexcept` specifications, where the "double `noexcept`" syntax
is so strange that it can be easily misused. For instance, if I want to say that my function `f()` has the same `noexcept` specification as function `g()`, it doesn't seem impossible that I could mistakenly type this as:

```c++
void f() noexcept(g);
```

To the uninitiated this looks reasonable; and it compiles. Also, it `g()` is a `constexpr` function, the following works as well:

```c++
void f() noexcept(g());
```

The no-narrowing requirement helps minimize these bugs, so it has merit. But other contexts, like `static_assert`, are only victims thereof. 


Analysis
--------

### Implicit conversions to `bool`

Some have suggested that a conversion to `bool` in general should not be considered narrowing, that `bool` should not be treated as a small integral type, and that the conversion to `bool` should be treated as a request to classify the states of the object as one of the two categories.

We do not want to go there. Even this seemingly correct reasoning can lead to bugs like this one:

```c++
struct Container {
  explicit Container(bool zeroSize, bool zeroCapacity) {}
  explicit Container(size_t size, int* begin) {}
};

int main() {
  std::vector<int> v;
  X x (v.data(), v.size()); // bad order!
}
```

If narrowing conversions can detect that, we would like to use this opportunity.


### Implicit conversions to `bool` in constant expressions

Another situation brought up while discussing this problem was if the following code should be correct:

```c++
// a C++03 type trait
template <typename T>
struct is_small {
  enum { value = (sizeof(T) == 1) };
};

template <bool small> struct S {};
S<is_small<char>::value> s;
```

In constant expressions the situation is different, because whether a conversion is narrowing or not depends not only on the types but also on the velaues, which are known at compile-time.

We believe that this problem boils down to whether type `bool` can be thought of as
an itegral type with the range of values {0, 1}, so that conversion from `1` to `true` is non-narrowing
and conversion from `2` to `true` is narrowing. We do not want to address this problem in this paper.
Instead, we propose only to fix the *contextual* conversion to `bool`, where it is uncontroversial that  `2` should be
represented as `true`.


How to fix this
---------------

There are two ways to address this:

1. Redefine *contextually converted constant expressiosn of type `bool`* so that narrowng is allowed. This will change all the places that use it:
   * `static_assert`
   * `if constexpr`
   * `explicit(bool)`
   * `noexcept(bool)`

2. Apply different rules to the first three contexts listed above and retain the stricter rule for the `noexcept(bool)` case.

We request for guidance from EWG in which approach to adopt.



Acknowledgements
---------------

Jason Merrill originally reported this issue in CWG reflector.


------------

*static condition*

```
int main() {
  static_assert(2); // ill-formed, gcc accepts
  if constexpr (2); // ill-formed, gcc accepts
}
```

[1407. Integral to `bool` conversion in converted constant expressions](http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_closed.html#1407)

[2039. Constant conversions to `bool`](http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#2039)

Richard Smith: Also, despite the claim in the writeup of 1407,
I don't think the wording is clear on this: converting 0 to false can be done by either an integral conversion
or by a boolean conversion, and the integral conversion is non-narrowing but the boolean conversion is narrowing.
The integral conversion case says "If the destination type is bool, see [conv.bool]",
but doesn't actually say that such a case is not an integral conversion. The same problem applies to [conv.fpint],
for what it's worth. If we mean to exclude bool from the set of possible destination types for integral conversions
and floating-integral conversions, we should say so. 

Davis Herring: What I remember from that discussion was that we also considered whether a conversion _from_ bool
to int ought to be narrowing: the "natural" definition hinged on whether you considered {false,true} to be a subset
of {0,1,2,3} or not.

To me, it all contributes to the argument that any int (maybe even a 0 or 1) converted to bool should be narrowing
but that contextual conversion should allow it anyway. 

Smith:
The second part of this seems like an improvement to me. I find the first part slightly unsettling, given that unanimous implementation consensus is that conversion from 0 or 1 to bool is not narrowing, and both GCC and EDG (even in strict mode) believe that conversion from 2 to bool is not narrowing either (though Clang and MSVC disagree and diagnose).

As I recall, the reason that Clang doesn't implement 1407 is code (in boost, if memory serves) that does something like this:

```c++
template<typename T> struct some_trait {
  static constexpr int value = /*...some boolean expression...*/;
};
template<typename T, bool B = some_trait<T>::value> struct something_else {};
```

... where treating the conversion from an int with value 0 or 1 to bool as narrowing results in an error.

Example with enum?

```
 Recent bug reports against clang and GCC noted that we weren't
diagnosing narrowing conversions to bool. These bug reports have been
fixed, but do we really mean to consider conversion to bool to be a
problematic narrowing conversion? Conversion to bool is intended to
reduce all the possible values of the original expression to a truth
value; it is deliberately narrowing. It seems qualitatively different
from conversion to another arithmetic type which is intended to
represent a number. It seems to me that classifying boolean
conversion as narrowing is an accidental consequence of bool being "an
integer type", rather than intentional.

If conversion to bool is narrowing, that means that

  static_assert (42);

ill-formed, though nobody currently diagnoses it.

"In a static_assert-declaration, the constant-expression shall be a
contextually converted constant expression of type bool (8.6)."

"A contextually converted constant expression of type bool is an
expression, contextually converted to bool (Clause 7), where the
converted expression is a constant expression and the conversion
sequence contains only the conversions
above."

"-- integral conversions (7.8) other than narrowing conversions (11.6.4),"

I think it should be clarified that when 11.6.4 says "integer type",
it means the signed and unsigned integer types, not the category that
includes bool.

Jason 
```
