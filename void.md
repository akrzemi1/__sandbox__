(Interestingly, the C++ Standard does not describe this SFINAE behavior for class template specializations.
However, all compilers implement it and the Committee members agree that this behavior is intended.)

The entities we have defineded, `true_type`, `false_type` and `enable_if` are useful tools for meta-programming tricks and type traits,
and they are in fact already defined in the C++11 Standard Library. They are a bit more complicated and more developed,
but the idea stays the same. Additionally, in C++14 we have alias `enable_if_t`.

Building our type trait
----------------------

There is one more metaprogramming tool that we need to define before we solve our problem.

```c++
template <typename... T>
using void_t = void;
```

This meta-metafunction is perhaps even more bizarre than `enable_if_t`
