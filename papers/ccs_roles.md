CCS roles/modes
===============


Canonical modes
---------------

In the following description, when referring to concrete semantics rendered by the Contract Checking Statements (CCS) and 
build configuration, we use terms defined in [[R1333R0]](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1333r0.txt):

* "ignore" -- validate syntax correctness and otherwise ignore: no assumptions, no optimizations, no run-time checks
* "assume" -- no run-time checks; eliminate branches in front of and behind the CSS that would be taken only if the condition in the CSS were to be violated. This is referred to as CCS-based optimization.
* "check (never continue)" -- perform run-time checks; compiler is allowed to assume that control never gets past the CSS if the condition is evaluated to false.
* "check (always continue)" -- perform run-time checks; compiler is allowed to assume that control always gets past the CSS, even if the condition is evaluated to false.
* "check (maybe continue)" -- perform run-time checks; compiler can assume if control gets or not past the CSS in case cthe consition is evaluated to false.

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


Other possible roles
--------------------


### Guarantee

CCS guarantees that the condition holds. This is possible if the author is in control of the preceeding code. This is similar to GCC's or Clang's `__builtin_unreachable()`.

This role is most likely to be assigned the CCS-based optimization semantics.

This 'kind' makes sense for asserts and postconditions, but not really for precondiitons: you usually are not in control of your callers.


### Review

The code after the CCS does *not* depend on the condition. The goal is to have a place to evaluate the condition, and record the fact of failure, but otherwise not to affect the program.


### Review + audit

Same as *Review* but there are reasons to believe that the evaluation of the condition will noticeably impact program performance.

References
----------
[[R1333R0]](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1333r0.txt) Joshua Berne, John Lakos, "Assigning Concrete Semantics to Contract-Checking Levels at Compile Time".

