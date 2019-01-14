Axioms
======

need potentially evaluated to instantiate templates execute expressions, and optimize based on it.

Definition of optimization: "UB if violated": this is how optimization on `int`s works.

Smith:
> The way this works today is:
> * if a template specialization is odr-used, then a definition of it is required to exist ([basic.def.odr]/10)
> * if a template specialization is referenced in a context that requires a definition to exist, one is instantiated ([temp.inst]/4)
>
> So odr-use triggers instantiation. (It's not the only thing that does.)

Smith:
> For contracts, we already have one, in [dcl.attr.contract.check]/4: "it is unspecified whether the predicate for a contract that is not checked under the current build level is evaluated; if the predicate of such a contract would evaluate to false, the behavior is undefined."
>
> That is: an implementation is allowed to evaluate a contract that's not checked and consider the program to have undefined behavior if the condition fails. (Then the actual optimization follows from as-if.)


Optimizations
=============

```
 #ifndef NDEBUG
  # define OPT x
  #else
  # define OPT(x) (x) || (std::unreachable(), false)
  #endif
```
so
```
  [[assert: OPT(x > 0)]
```

----

I can think of a yet another example though, in case of single-pass ranges, where the predicate can be evaluated, but if it is, the function that is guarded by it can no longer be safely called:

```
template <InputIterator It>
bool range_of_3_or_more(It b, it e) { return std::distance(b, e) >= 3; }

template <InputIterator It>
void read(It b, It e)
[[expects axiom: range_of_3_or_more(b, e)]];
```
But is this the use case that we want to address?

Regarding the usage of contract checks for optimization, I agree with Peter that this should be controlled by a build mode rather than a specific kind of particular precondition. I do not want only axioms to be assumed in an aggressive optimization mode. I also want other default and audit contracts to be assumed (of course, only in a special aggressive mode).

BTW, Indeed P0380r1 mentions explicitly: "the predicate of an axiom is not evaluated and may contain calls to undefined functions."

Ville:

> Here's the thing: I want to be able to mix, in the same translation
> unit, code that is safe under assumed contracts, and code that is not
> safe under assumed contracts.
> Therefore I need to be able to syntactically say when a contract can
> be assumed and when it can't. TU-global flags and program-global flags
> that control the assumedness
> do not work for me.

Daniel's solution does not allow optimizations based on default contracts

can the author of `main()` control assumptions about contracts in libraries used by other libraries?

`assume()` -- how is it different form axiom. "Assume for the purpose of codegen", "assume for the purpose of static analysis"?
