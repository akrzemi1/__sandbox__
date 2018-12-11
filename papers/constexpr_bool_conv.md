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
