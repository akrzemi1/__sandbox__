CCS roles/modes
===============


### Default

Code after the CCS *depends* on the condition to be true (potentially UB if control gets past the CCS with violated condition).

This impies all ranges of possible semantics:
* Check preconditions and stop execution of the reminder (either by throwing or aborting program).
* Do not check anything and hope that the condition in CSS always holds.
* Do not check anything and additionally eliminate branches even in front of the CCS if they are reached by violating the condition. This is referred to as CCS-based optimization.


### Audit

Same as *Default* but we have reasons to believe that checking it at runtime will noticeably affect program performance.

Semantics are somewhat interact with semantics for *Default*. If we do CCS-based optimization on *Audit* and at the same time runtime-check *Default*, this checking may be compromized by the optimizations.

Technically, the interaction goes the other way around also: if we optimize based on *Default* and check *Audit*, then *Audit* checks may be compromized.


### Axiom

Same as *Default* except that only evaluating the condition at runtime would have correctness impact on the program 
(broken preconditions, UB, etc.), or is impossible (program would not compile). 

The range of semantics is reduced: we cannot evaluate the checks at run-time.

The same interaction between *Axiom* and *Default* (and *Audit*) exists: If we do CCS-based optimization on *Axiom* and at the same time runtime-check *Default* or/and *Audit*, this checking may be compromized by the optimizations.


Other possible roles
--------------------


### Guarantee

CCS guarantees that the condition holds. This is possible if the author is in control of the preceeding code. This is similar to GCC's or Clang's `__builtin_unreachable()`.

This role is most likely to be assigned the CCS-based optimization semantics.


### Review

The code after the CCS does *not* depend on the condition. The goal is to have a place to evaluate the condition, and record the fact of failure, but otherwise not to affect the program.


### Review + audit

Same as *Review* but there are reasons to believe that the evaluation of the condition will noticeably impact program performance.
