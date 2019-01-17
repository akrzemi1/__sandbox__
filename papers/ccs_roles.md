CCS roles/modes/contexts/intentions
===================================

Motivation
----------

This paper tries to list all 'circumstances' that can affect what semantics (ignore, run-time check, assume) we want to
associate with different Contract Checking Statements (CCS) at different places in code. Our observation is that neither the current wording, nor the *roles*, as defined in [[P1332R0]](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1332r0.txt),
fully grasp the perceived needs of the users.

Our conclusion is that while not all possible user needs can or should be reflected in the Standard, the Standard should make 
it clear that users may expect from implementations more control over the semantics associated with their CSS-es than what the 
Standard requires as minimum.


What should affect the CCS semantics
------------------------------------

In the following description, when referring to concrete semantics rendered by CCS-es and 
build configuration, we use terms defined in [[R1333R0]](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1333r0.txt):

* "ignore" -- validate syntax correctness and otherwise ignore: no assumptions, no optimizations, no run-time checks
* "assume" -- no run-time checks; eliminate branches in front of and behind the CSS that would be taken only if the condition in the CSS were to be violated. This is referred to as CCS-based optimization.
* "check (never continue)" -- perform run-time checks; compiler is allowed to assume that control never gets past the CSS if the condition is evaluated to false.
* "check (always continue)" -- perform run-time checks; compiler is allowed to assume that control always gets past the CSS, even if the condition is evaluated to false.
* "check (maybe continue)" -- perform run-time checks; compiler can assume if control gets or not past the CSS in case cthe consition is evaluated to false.

What particular semantics gets chosen for different CCS-es can be controlled by a number of things:
* Purpose of the binary: for testing, debuggig, or release.
* CCS's intended 'puprpose' stated in the CSS, "evaluate to prevent UB", vs "evaluate to check what happens" vs "just indicate unimplementable condition".
* Predicted cost of evaluating the check.
* Perceived probability of a CSS-es condition evaluating to `false`.
* Whether it is a precondition or a postcondition/assertion.

Concepts in [[p0542r5]](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0542r5.html), CCS "level" or "continuation mode" are not able to model programmer expectations.


Canonical assertion levels (intentions)
-----------------------------------

[[p0542r5]](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0542r5.html) lists three assertion levels: Default, Audit and Axiom. But declaring such one-dementional scale may be an oversimplification. Yes, Default and Audit indeed seem to differ only by "weight", but Axiom is different in quality than the former two. They are more like "intentions" of the author that can assume more than three types.


### Default

Code after the CCS *depends* on the condition to be true (potentially UB if control gets past the CCS with violated condition).

All five concrete semantics make sense for this "kind" of assertion.


### Audit

Same as *Default* but we have reasons to believe that checking it at runtime will noticeably affect program performance.

Semantics somewhat interact with semantics for *Default*. If we apply "assume" semantics to *Audit* and "check" semantics to *Default*, this checking may be compromized by the optimizations.

Technically, the interaction goes the other way around also: if we apply "assume" semantics to *Default* and "check" semantics to *Audit*, then *Audit* checks may be compromized.


### Axiom

Same as *Default* except that only evaluating the condition at runtime would have correctness impact on the program 
(broken preconditions, UB, etc.), or is impossible (program would not compile). 

The range of semantics is reduced: we cannot evaluate the checks at run-time.

The same interaction between *Axiom* and *Default* (and *Audit*) exists: If we apply "expect" semantics to *Axiom* and at the same time apply "check" semantics *Default* or/and *Audit*, this checking may be compromized by the optimizations.

This is a more modest interpretation of what an *Axiom* CSS is. This is to separate two conflated meanings of *Axiom* CSS-es. The other meaning is represented by "kind" *Guarantee* below.


Other possible intentions
--------------------


### Guarantee

CCS guarantees that the condition holds. This is possible if the author is in control of the preceeding code. This is similar to GCC's or Clang's `__builtin_unreachable()`.

This role is most likely to be assigned semantics "assume".

This 'kind' makes sense for asserts and postconditions, but not really for precondiitons: you usually are not in control of your callers.


### Review

The code after the CCS does *not* depend on the condition. The goal is to have a place to evaluate the condition, and record the fact of failure, but otherwise not to affect the program.

The most likely semantics are "check" or "ignore". Of course, "assume" semantics, in *Default* (or in fact, any other) "kind" can affect these semantics. In the end, this is the point of "assume": that code can be generated differently based on the new information.


### Review + audit

Same as *Review* but there are reasons to believe that the evaluation of the condition will noticeably impact program performance.


### More...

One can imagine more such "intentions". For instance assertion costs can be finer grained than just *Default* and *Audit*. One could imagine three cost categories:

* Where assertion overhead is negligible compared to the containing function.
* Where assertion overhead is simialr to the function overhead (program slowdown by factor 2).
* Where assertion overhead has bigger complexity that the function.

Also "relatively fast" (that would qualify fo *Default* "level") is subjective and authors of different libraries can set the thresholt differently. The programmer may disagree with the decision in some of the libraries. Therefore there may be a need to enable *Default* assertions per-library (per module, per namespace, per tag in the assert).

The orthogonal division of CCS-es into levels and roles, as in 
[[P1332r0]](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1332r0.txt) also does not quite cut it. While it makes sense to have an *Audit* version of a *Review* assert, an *Axiom* version does not seem to make sense.


Preconditions vs other CCS-es
-----------------------------

It is a common situation in contract checking frameworks to runtime check only preconditions but ignore assertions, postconditions and invariants. E.g, this is possible in Eiffel
([[EIFFEL]](https://www.eiffel.org/doc/eiffel/ET-_Design_by_Contract_%28tm%29%2C_Assertions_and_Exceptions)) and in Boost.Contract ([[BOOST.CONTRACT]](https://www.boost.org/doc/libs/1_69_0/libs/contract/doc/html/index.html)). The reason for this is that the likelyhood of detecting a bug while evaluating a precondition is much much higher than in other types of CCS-es. This is because preconditions are the only type of CCS-es where a different person declares the expectation and a different person is expected to fulfill it: the llikelihood of micommunication is higher. In contrast, for preconditions, the author of the function declares the precondition and implements the function body.

Therefore it is likely that programmers will want only preconditions to be evaluated at run-time.


Different handlers and different continuation modes within ne program
---------------------------------------------------------------------

There may be a need to use more than one continuation mode in the program: continue after some failed assertions but make sure to abort on others. Similarly different callbacks may be needed for different assertions. This could be implemented through one callback that obtains sufficient input to determine what level/mode/role/intention/kind of a CCS has failed.


Recommendation
--------------

1. As per [[R1333r0]](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1333r0.txt), define the five semantics of CSS-es in the Standard.

2. As suggested in [[P1332r0]](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1332r0.txt) (section 5.4.2), provide a slightly different syntax for naming the returned object, e.g.: `[[ensures(r): r >= 0]]`.

3. Apart from `default`, `audit` and `axiom` allow arbitrary identifier or namespace-qualified identifier in that position. Don't call them "levels" but something else, like "tags". These identifiers are passed to the violation handler if run-time checking is requested.

4. Add a provision in the standard that, apart from these mandated in the Standard, there are implementation-defined ways to map CSS-es onto semantics that may include the tag, kind (precondition vs postcondition), enclosing namespace, translation unit. If some CCS tag is not recognized by the implementation, it should apply "ignore" semantics. 


References
----------

[[p0542r5]](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0542r5.html) G. Dos Reis, J. D. Garcia, J. Lakos, A. Meredith, N. Myers, B. Stroustrup, "Support for contract based programming in C++".

[[P1290r0]](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1290r0.pdf) J. Daniel Garcia, "Avoiding undefined behavior in contracts".

[[P1332r0]](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1332r0.txt) Joshua Berne, Nathan Burgers, Hyman Rosen, John Lakos, "Contract Checking in C++: A (long-term) Road Map".

[[R1333r0]](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1333r0.txt) Joshua Berne, John Lakos, "Assigning Concrete Semantics to Contract-Checking Levels at Compile Time".

[[EIFFEL]](https://www.eiffel.org/doc/eiffel/ET-_Design_by_Contract_%28tm%29%2C_Assertions_and_Exceptions) Eiffel Tutorial: "Design by Contract (tm), Assertions and Exceptions".

[[BOOST.CONTRACT]](https://www.boost.org/doc/libs/1_69_0/libs/contract/doc/html/index.html) Lorenzo Caminiti, Boost.Contract.

