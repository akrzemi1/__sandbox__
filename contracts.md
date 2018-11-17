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


-------------------------

Infinite recursion upon contract violation when executing a contract violation handler.

do we aim at never changing the contract declarations after added (not even the roles)?

what is someone is using "default" level for too expensive checks?

tags, or maybe command-line provided tags

what is the purpose of `[[assert axiom %review : e]]`?

notes
------

http://open-std.org/JTC1/SC22/WG21/docs/papers/2018/p1321r0.html

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
