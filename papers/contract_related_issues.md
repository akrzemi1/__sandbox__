Contract-related issues
=======================


This paper attempts to summarize the issues with the current contract design that have came up
in the recent discussions in the EWG reflector. The goal is to give the picture of the situation to all Committee members.



1\. Contract-based optimizations that cause concerns.
----------------------------------------------------

The problem that [[P1290r1]][3] intends to address is related to the following statement in the [[WD]][1] ([dcl.attr.contract.check]/p4):

> [...] it is unspecified whether the predicate for a contract that is not checked under the current build level is evaluated;
  if the predicate of such a contract would evaluate to false, the behavior is undefined.


This unspecified behavior is referred to as "predicate can be assumed to be true". This works under the general mechanism of UB: compiler is allowed to assume that UB never happens in the program. This is the heart of UB-based optimizations. Because evaluating unchecked predicates is implementation-defined the implementation is allowed (but not required) to nonetheless perform a run-time check. Because in another place ([dcl.attr.contract.check]/p6) we require that any side effect while evaluating the precondition is UB, under the as-if rule, evaluating the precondition at run-time is equivalent to evaluating it symbolically (at compile-time), so the implementation is allowed to understand the condition. If it can convince itself (prove) that there is a path in the program that would cause the predicate to evaluate to false it would mean that this path leads directly to UB. Since compiler is allowed to assume that UB never happens, it can infer from this that this path is never taken, and from the latter it can infer that the predicate in the CCS is never false. then it can use this assumptions in other places, in particular, *before* the place where the predicate appears, which we often call "time travel" optimizations.

Some people read the above [dcl.attr.contract.check]/p4 as follows: compilers are allowed to implement this line of reasoning, so there is a risk that they will do it *without* giving the programmers the chance to turn this optimization off. Therefore giving the compiler vendors the *possibility* to implement this optimization is perceived as a risk.

A different way of looking at this definition is that compiler vendors can communicate with programmers by other means than the International Standard. For instance, even though the International Standard does not demand that implementations give the programmers control to opt out of any optimizations, compilers nonetheless give them this option. Or, even though the International Standard allows optimizations based on strict type aliasing rules (UB-based optimizations) and does not mandate a "mode" where these optimizations (assumptions) would be disallowed, compilers like clang or gcc still allow the programmers to control this with flag `-fstrict-aliasing`.

This point of view is based on trust that compiler vendors work in the best interests of the programmers, that they are aware of the issues related to assuming the CCS predicates, and that their natural course of action would be to either never implement such assumptions, or enable them under a compiler flag. Under this view there is no problem related to contract-based optimizations in the current [[WD]][1]: it gives the compiler vendors a provision for implementing contract-based optimizations in a responsible way.

Thus, to great extent the decision whether to mandate no-contract-based-optimization guarantee in the Standard boils down to the question whether we trust that compiler vendors will be responsible when doing their job.


2\. The goal of having axiom-level CCS-es.
-----------------------------------------

We have seen two positions here. One position is that an axiom-level CCS can be used to express a precondition to a function (something that we expect, but we are not sure if the callers will comply with) formally where the predicate must never be evaluated at run-time, because it is unimplementable, or would have side effects or has an unacceptable complexity even for test builds. A common example is `is_reachable()` for a pair of iterators that are expected to represent a range.

The other position, is that axiom-level CCSes, unlike other CSS-es do not express expectations (something we need but cannot be sure about), but declare program-wide "absolute truths" that compiler is allowed to take for granted: they enable UB-based optimizations and are not allowed to be checked. This position is backed by the fact that preprocessing token `axiom` spells the same as the notion of an axiom in mathematics. This position precludes defining preconditions like `is_reachable()` described above.

The current [[WD]][1] technically accommodates two positions but this is a superficial unification. Programmers and tool writers will need to be given instructions how to use axiom-level CCSes. And if they are given contradictory guidance this will create a fracture in the community even though the International Standard clearly defines what happens under each mode with axiom-level CCSes. This is because the use of CSS-es is beyond what the IS can describe (like static analysis tools).

The consensus regarding the two positions needs to be reached even if it is not expressed directly in the IS. It can be used as a basis for applying fixes and future modifications to contracts. For instance, the solution in [[P1290r1]][3] takes the direction of the second position. It adds a new semantic distinction between axiom-level CCSes and other CCSes: the former are allowed to be used for UB-based optimizations under one mode, whereas other levels of CSS-es are not allowed for UB-based optimizations under any defined mode. This distinction is illogical if we assume the first position (where axiom-level CCS-es differ only in the guarantee that they will never be run-time evaluated). But it makes perfect sense if we assume the second position.

But the decision which position we adopt should be made prior to considering P1290R1.

(BTW, this is not meant as a critique of [[P1290r1]][3]. I acknowledge that it reflects the polls from EWG.)


3\. Are conditions in axiom-level CCS-es odr-used?
-------------------------------------------------

The current WD says that conditions in axiom-level CCS-es are as any other conditions: they are ODR used. And function templates need to be instantiated. This has some applications: for some predicates we know how to express them, but we do not want them to be evaluated (too expensive, have side effects), but static analysis tools may wish to instantiate the bodies to observe the code and draw conclusions.

On the other hand this disallows declaring (but not defining) predicates whose body just cannot be implemented, like `is_reachable()`.

Whichever is chosen, it may compromise the other's use cases. The design document for contracts, [[P0380r0]][2], seems to have intended the second (not ODR-used) approach:

> Note that the predicate of an axiom is not evaluated and may contain calls to undefined functions.



4\. Fewer use cases for continuation mode
----------------------------------------

The design document, [[P0380r0]][2], gave two use cases for continuation mode:

> There are programs that need to resume computation after executing a violations handler. This seems counterintuitive (“continue after a precondition violation!!!?”), but there are two important use cases:
>
> * Gradual introduction of contracts:  Experience shows that once you start introducing contracts into a large old code base, violations are found in “correctly working code.” In other words, contracts are violated in ways that did not cause crashes for the actual use of the code for the actual data used. There are examples where the number of such “currently harmless violations” is massive. The way to cope is to install a violation handler that logs the problem and continues. This allows gradual adoption.
>
> * Test harnesses: Contracts are code, so they can contain bugs. To test contracts, we need to execute examples of violations. A convenient way of organizing such a test suite is to have a violation handler throw an exception and have the main testing loop catch exceptions and proceed running the next test


During the recent discussions it has been shown that a global (or even TU-local) continuation mode flag is not capable of facilitating the first use case. For instance:

```c++
int f(int * p)
  [[expects: p != nullptr]]
  [[expects: *p >= 0]];
```

If default CCS-es are runtime-checked and continuation mode is on, evaluating the second condition will be UB, which means that (due to UB-based optimizations that do not come from assuming CCS-es, but from evaluating regular code inside a CSS) the first runtime check may entirely be elided! Even if checking is on.

Regarding the second use case, during the course of discussion and clarifications, it has been made clear that one does not need the continuation mode to safely throw from a violation handler and continue the program execution.

We are left with no use case for the continuation mode. Unless some new use cases have came up.

[[P1429r0]][4] attempts to still handle the first use case (gradual introduction of contracts) by offering a per-CCS control whether the program should continue on failure or not. This is a design change (at the last minute), but admittedly one that addresses the design goal of [[P0380r0]][2]. If applied, it still renders global continuation mode useless.

Regardless of whether this part of [[P1429r0]][4] is adopted, it seems the right course of action to remove the continuation mode. Unless new use cases come up.


References
----------

[1]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/n4800.pdf
[[WD]](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/n4800.pdf) -- Richard Smith, N4800, "Working Draft, Standard for Programming Language C++".

[2]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0380r0.pdf
[[P0380r0]](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0380r0.pdf) -- G. Dos Reis, J. D. Garcia, J. Lakos, A. Meredith, N. Myers, B. Stroustrup, "A Contract Design".

[3]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1290r1.pdf
[[P1290r1]](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1290r1.pdf) -- J. Daniel Garcia, Ville Voutilainen, "Avoiding undefined behavior in contracts".

[4]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1429r0.pdf
[[P1429r0]](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1429r0.pdf) --  	Joshua Berne, John Lakos, "Contracts That Work".

