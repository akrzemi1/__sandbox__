Contracts
=========

Use cases
---------

### Evaluate cheap checks in production (CCS 0.1/1)

"Cheap" means evaluating them does not add significant overhead compared to the cost of the function that is guarded by the check.

If such check evaluetes to false, we may want to:
* Terminate the program
* Throw an exception: this is not quite safe because program in an incorrect state cannot be guaranteed to safely perform stack unwinding. But there is a likelihood that stack unwinding will escape a module that contains a bug, and the bug becomes isolated.

Q: can I make the right choice and impose it on others as to which checks are "sufficiently inexpensive"?
O: Throwing from contract violations can breach `noexcept`.

### Some checks should be ignored in production (CCS 0.1/2)

But sometimes when debugging we may want to turn them on at the expense of slowing down the program.

### Some checks with expressions that have no definition (CSS 0.1/3)

### Unit tests using exceptions (CSS 0.1/4)

### Only syntax check but no other effects (CSS 0.1/5, CSS 0.1/7)

### Optimizations (CSS 0.1/6)

### Switch all checks from optimization to run-time validation without changing the control paths (CSS 0.2)

Should static analysis still detect them?

### Add a bit more checks in review mode without compromizing the previous ones (CCS 0.3)

Should static analysis still detect them?

### Only syntax-check contracts but do nothing. Maybe later will promote them (CCS 0.4)

### Add debug checks that are never optimized (CCS 0.5)

### Assign different semantics to contracts in two different libraries (CCS 0.6)

What if header only libraries?

What if GodVendor and BadVendor use the same roles? Or BadVendor uses default role?


Responses to some ideas
-----------------------

### Evaluate contract and if false skip subsequent operations as if by an `if` expression. 

It is like throw and premature catch-and-conceal. It doesn't address the `review` goal of assertions.

### `assume()` and `check()` are two different things

No: they are two behaviors of the same thing: like `const`:
* what I can assume
* what the caller is prevented from doing


-----------

do we want to address the use case where `[[assert review]]` is used to log before a suspected UB?

Goal: that runtime-check proponents are not threatened by UB proponents and vice versa. (Show exampples how one disturbs the other).

---------

show example with `for_each`. Show example with `string` and `string_view` ?

Do we need a a "library-level UB" definition? How it interacts with axioms? Axioms are different syntactically.

Is axiom contract potentially evaluated?

-------------------------

Different mode: different control paths: different programs. You have to accept this

We can still use `std::unreachable()` or `__builtin_unreachable()`. Contracts are different only in that they have more than one mode.

"Reliable software" this is when preconditions are obeyed, not when they are checked and kill applicaitons or cancel operations.

Preconditions do not add more UB/bugs -> they make it more explicit, managable

Control checking-vs-ignoring separately and assuming-vs-ignoring separately.

Infinite recursion upon contract violation when executing a contract violation handler.

do we aim at never changing the contract declarations after added (not even the roles)?

what is someone is using "default" level for too expensive checks?

tags, or maybe command-line provided tags

what is the purpose of `[[assert axiom %review : e]]`?

 Alisdair: Contracts are not designed to be an optimization feature. Any optimization is incidental. Axioms are not there necessarily to be an optimization. 
 
 Herb: I agree with what you said Alisdair… Assertions are not assumptions. I believe that is the distinctions between axioms and any other contracts. We can argue about spelling, but fundamentally, the things we assume and the things we assert are different categories of things. 
 
 -- `[[assert]]` and `[[expect]]` may be different here 
 
 Relation between UB and optimization: compiler can assume that UB never happens
 
  The Lakosian notion of UB which mean Library UB, 
  
  Ville: But the flip side is that if I would like to keep that wide contract in my code when I add a narrow one  -- what?
 
 Herb: I thought I said that some contract designers thought that way, and the rest of the answer is that assertions and assumptions are really fundamentally different. 
 
 Why are you affraid of UB but not bugs?
 
 Daniel: I would like to show a proposal to solve 1321 
 
 Daniel: During constant expression evaluation, only predicates of checked contracts are evaluated. That’s all I suggest 
 
 Herb suggests: you can assume if /if not the control reaches the call to a violation handler much like you can assume control never reaches `__builtin_unreachable()`.
 
 ---------
 
Ville : I want to be able to mix, in the same translation
unit, code that is safe under assumed contracts, and code that is not
safe under assumed contracts.
Therefore I need to be able to syntactically say when a contract can
be assumed and when it can't. TU-global flags and program-global flags
that control the assumedness
do not work for me.

 
 ----
 
 Making only axioms work for optimizations will not work: I want compilers to assume `operator[i]` for vectors.
 
 Explain that throwing from violation handler works in continuation mode off. 
 
 My concern: what if vendor V thinks it Ok to put expensive checks as `default`. This prevents me to enable my `default` checks as well as checks from other vendors.
 
 Concern: do you really want to assume every contract in every library? Who knows what they put into preconditions? If they just experiment? Review?
 
 ---------
 
 Peter:
 JOSE DANIEL GARCIA SANCHEZ wrote:
> > > void f(int * p)
> > >   [[expects: p!=nullptr]]
> > >   [[expects audit: *p>0]];

...

> It is pre-broken only if current WD. This is fixed by P1290R0. We propose 
> that you cannot assume a default or audit contract without checking it.

You are correct, I hadn't read the paper yet when I replied to the above.

Making it not possible to assume a contract without checking it is not a
solution to the issue above however. With continuation on, if the compiler
proves that the handler returns, you have the same problem of it eliminating
the p != nullptr check. And conversely, assuming contracts without checking
them can be completely safe. For instance, if default contracts are assumed
without being checked, nothing unexpected happens.
 
 
 -------
 
 Herb: 

> Hal:
> To add the necessary caveat: This is true except in cases where the library
> precondition is translated directly, because of the structure of the code, into UB
> directly associated with the core language. A classic example is something like
> this: "Requires: i is a valid dereferenceable iterator of x." (21.3.10.5p7) - if the
> code implementing splice unconditionally dereferences i, then a
> dereferenceability/nonnull assumption might certainly be propagated by the
> optimizer to other necessarily-executed parts of the code.

Thanks, that's true for simple preconditions that the function directly exercises and are language UB when violated.

FWIW I agree with the rest of the thread that it would be nice to have distinct terms for 'language UB' and 'library UB' since they aren't isomorphic, and have a clear definition for each to point to.

notes
------

http://wiki.edg.com/bin/view/Wg21sandiego2018/P1321R0-San18

http://open-std.org/JTC1/SC22/WG21/docs/papers/2018/p1321r0.html

http://wiki.edg.com/pub/Wg21sandiego2018/EvolutionWorkingGroup/p1343-contracts-update.html?twiki_redirect_cache=39c88ee2a375665982a3c7748490bafe

http://cantrip.org/p1343-contracts-update.html

```c++
   [[assert: no() || expression]]
```

Evaluated at runtime, no() is a user-supplied function that returns false. When the expression is not evaluated, no definition need be provided for no() (or, equivalently, it may be defined to return true).



villes example but when we replace 2 with 42: we get self contradictory assumptions

https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAKxAEZSAbAQwDtRkBSAJgCFufSAZ1QBXYskwgA5NwDMeFsgYisAag6yAwpgAeEgA4E8qFhuwcADAEFLNrrKwAzBZlXYAGgAVsmgCoBlCAZMADdMBlJVXX1iAEp1AFZNW1VU1TxHCDBIaLj4jiSUtI4Adh5E5Os06vTHVTQWQQJc1QgAfTbNAHkAOV8AJSs/Np4AVQBJABkAETbJ7AA1bEn1WXNZadVgsIZ8wqqa1NLygsqrQ8OO5CYGBjaQ42YjEzaEVnRg4ghYjRP984u1Qy9RMTRa7U6vQGQ18kL64x6oysvnGvTaAFkutN1tMNNMLHszoDAU10CAQM1iABbBRMZrfX4VIqHUqbU7MmrhQSudkHFllJl84kdABGIjwDCMLDaIhYxEwTGQbxFwQZsj%2BRIurMFAKOJVx1lsukpLFUIVQeHQqiuNzuD1QT2M0reLA%2BmC%2BP3Vtm99icLlUXQAYoHVBYfQ5MM4WK5sYGrKNJr5VLRw37o6p49NxkmuN7Db7I/6Ot0%2BoNhmMprN5ksVrH44nU4X08WoWXYSWUYjkaiehisdhQ3m7PJFMo1BpNKTjAA6BBmcMKJQqHlaKcsAiz%2Bf5kdL8erggfPAizdrIfmy2qTIKAiqHTxAD098SfASPGimGQBEEqncv5At7xPFcwSXEQO9MoOQ8bw/ECDNRizXxIh0QCNi4VQfkNIUYmvTJuFzLgEnQU5TC4LgkM9PhDX1M8LStYAIGvW8HyfAoXzfHR9A/L9/2Q08NgsdQuAANm4ISAK0WQClAg0bAgoUoJ8AIIFUOsE0Q8ScQE0TRPEzRZHQocgXXVQmFYySwK9IVTNfZCQNWTYABZc0s2SZNsc8rUcVBUDVSirA81QRSYD1fnc2jAqYAAvXyaIvBAGOMu9VEfZ9WPfT9BB4%2BcNloQSROE3S8XM6TwL86oFJg5TVMTJDsumXLtIK3jkg2fSMJsIVBAAd0IJUIDvDljg5aprm5ZN/y8nyKMC%2BUmAAa1CoURqYMauH/IKQvVGaFQWlzAVG1xZHWqLfO2%2BbFt1dRqKotzrAC5omgZKiyrSRiQhuezVFJcliHeGLMMughiAATyu8pMne3ZGXvAAqad4Zhlj9XqOk%2Bvh6d8gFWH0cRq6ZOqIHQeOVR6Mh6bsYRpHNmuAg0fhzHygp6dcdZDlCbB1R4rJ6G4cpvGUdp%2BL0YZ5LeeZqnwJkqRYkYaQEikUgWGkCwFdQaRkl4fgvtEcQeXsWgFYIZXpZluaQASCxZakByFaVqQVdINWpAVzLLaN%2B3pdIOBYCQNAqX0CV3XISg/YDz4QGAQQWCYfRBAQVACFIZxJXdTKIBFY3SBFWkQekA3SD9qlMHXLoWAYYHM6wKlWGAYJK7weVPzwMJMo90hdA/ERmjzhXr3CTOgbwKljZl5g2BQfh%2BEYI9MtgaOi5AEVvKaUgwmIPOhIADhNx3DCdVuAFoun0g/STxThNd4WgShdnWJDoUe5dtzOnZ0TehIPoSHJJ5BkFaIHZRzXiBAXAhASCCVkPQVQmhUD%2B0DsQCBtB4gaz4LwQ2I9Tbm0tgwaQNtFYv2kC7EAbsMFWy4M/NuTt0Ee1iDLNeggnQgAckAA%3D%3D%3D


Interference between various assertions: If there is undefined behavior in the assertion itself in continuation-mode "on": 
```c++
void f(int *p)
{
   [[assert : p != nullptr]]  // #1
   int i = 0;
   [[assert : *p > 5 ]]       // #2
}
```
Any competent optimizer will remove #1, because *p in #2
subsumes #1.  This was pointed out as early as Albuquerque:


http://wiki.edg.com/bin/view/Wg21albuquerque/CoreWorkingGroup?twiki_redirect_cache=5baafa4e9cd32a2890412e82817b01f5

Lakos:
```
This is a bug!  It must never be the case that any contract checking within a given TU eclipses a runtime check in that TU.  If necessary to satisfy this requirement, we will need to insist that *all* optimizations derived from any contract specifications in a TU be disabled whenever *any* runtime checks in that TU are active.

Repeating what I said before here (for quick reference):

JOHN LAKOS WROTE:
I trust that it is clear that we will need to be able to control the extent of runtime checking separately from the extent to which we allow inactive runtime checks to be input to the optimizer.

My original idea was the we could give full control whereby we had two "slide switches": the left slide switch would control runtime checking, and the right one would control compile-time optimizations.  You could turn both off by placing them at their respective ends.  You could dial runtime checking all the way up by sliding that switch to the right, Or you could dial up compile-time optimization by sliding that switch all the way to the left.  The two slide switches, however, must never cross (or even meet).  

The axis for these two slide switches contains 'none', 'default', 'audit', 'axiom', and 'off'. One could imaging setting runtime checking all the way (up) right to 'audit', or compile-time optimization all the way (up) left to 'default'.  One could even imaging having all 'default'-level checks active at runime and all 'audit'-level checks assumed and forwarded to the optimizer (along with 'axiom' level assertions, of course).  I think this level of control, however, might be too much for a first pass, and so I would be happy to start with allowing at most one slide switch to be active at one time, and see how that goes.
```
