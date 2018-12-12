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

In constant expressions the situation is different, because whether a conversion is narrowing or not depends not only on the types but also on the velaues, which are known at compile-time. We think that after [P0907r4](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0907r4.html) was adopted, and `bool` has a well defined representation, conversion from `1` to `true` is now defined as non-narrowing.


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

