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



Responses to some ideas
-----------------------

### Evaluate contract and if false skip subsequent operations as if by an `if` expression. 

It is like throw and premature catch-and-conceal. It doesn't address the `review` goal of assertions.

### `assume()` and `check()` are two different things

No: they are two behaviors of the same thing: like `const`:
* what I can assume
* what the caller is prevented from doing


-------------------------

notes

villes example but when we replace 2 with 42: we get self contradictory assumptions

https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAKxAEZSAbAQwDtRkBSAJgCFufSAZ1QBXYskwgA5NwDMeFsgYisAag6yAwpgAeEgA4E8qFhuwcADAEFLNrrKwAzBZlXYAGgAVsmgCoBlCAZMADdMBlJVXX1iAEp1AFZNW1VU1TxHCDBIaLj4jiSUtI4Adh5E5Os06vTHVTQWQQJc1QgAfTbNAHkAOV8AJSs/Np4AVQBJABkAETbJ7AA1bEn1WXNZadVgsIZ8wqqa1NLygsqrQ8OO5CYGBjaQ42YjEzaEVnRg4ghYjRP984u1Qy9RMTRa7U6vQGQ18kL64x6oysvnGvTaAFkutN1tMNNMLHszoDAU10CAQM1iABbBRMZrfX4VIqHUqbU7MmrhQSudkHFllJl84kdABGIjwDCMLDaIhYxEwTGQbxFwQZsj%2BRIurMFAKOJVx1lsukpLFUIVQeHQqiuNzuD1QT2M0reLA%2BmC%2BP3Vtm99icLlUXQAYoHVBYfQ5MM4WK5sYGrKNJr5VLRw37o6p49NxkmuN7Db7I/6Ot0%2BoNhmMprN5ksVrH44nU4X08WoWXYSWUYjkaiehisdhQ3m7PJFMo1BpNKTjAA6BBmcMKJQqHlaKcsAiz%2Bf5kdL8erggfPAizdrIfmy2qTIKAiqHTxAD098SfASPGimGQBEEqncv5At7xPFcwSXEQO9MoOQ8bw/ECDNRizXxIh0QCNi4VQfkNIUYmvTJuFzLgEnQU5TC4LgkM9PhDX1M8LStYAIGvW8HyfAoXzfHR9A/L9/2Q08NgsdQuAANm4ISAK0WQClAg0bAgoUoJ8AIIFUOsE0Q8ScQE0TRPEzRZHQocgXXVQmFYySwK9IVTNfZCQNWTYABZc0s2SZNsc8rUcVBUDVSirA81QRSYD1fnc2jAqYAAvXyaIvBAGOMu9VEfZ9WPfT9BB4%2BcNloQSROE3S8XM6TwL86oFJg5TVMTJDsumXLtIK3jkg2fSMJsIVBAAd0IJUIDvDljg5aprm5ZN/y8nyKMC%2BUmAAa1CoURqYMauH/IKQvVGaFQWlzAVG1xZHWqLfO2%2BbFt1dRqKotzrAC5omgZKiyrSRiQhuezVFJcliHeGLMMughiAATyu8pMne3ZGXvAAqad4Zhlj9XqOk%2Bvh6d8gFWH0cRq6ZOqIHQeOVR6Mh6bsYRpHNmuAg0fhzHygp6dcdZDlCbB1R4rJ6G4cpvGUdp%2BL0YZ5LeeZqnwJkqRYkYaQEikUgWGkCwFdQaRkl4fgvtEcQeXsWgFYIZXpZluaQASCxZakByFaVqQVdINWpAVzLLaN%2B3pdIOBYCQNAqX0CV3XISg/YDz4QGAQQWCYfRBAQVACFIZxJXdTKIBFY3SBFWkQekA3SD9qlMHXLoWAYYHM6wKlWGAYJK7weVPzwMJMo90hdA/ERmjzhXr3CTOgbwKljZl5g2BQfh%2BEYI9MtgaOi5AEVvKaUgwmIPOhIADhNx3DCdVuAFoun0g/STxThNd4WgShdnWJDoUe5dtzOnZ0TehIPoSHJJ5BkFaIHZRzXiBAXAhASCCVkPQVQmhUD%2B0DsQCBtB4gaz4LwQ2I9Tbm0tgwaQNtFYv2kC7EAbsMFWy4M/NuTt0Ee1iDLNeggnQgAckAA%3D%3D%3D

```c++
void f(int *p)
{
   [[assert : p != nullptr]]  // #1
   [[assert : *p > 5 ]]       // #2
}
```
Any competent optimizer will remove #1, because *p in #2
subsumes #1.  This was pointed out as early as Albuquerque:


http://wiki.edg.com/bin/view/Wg21albuquerque/CoreWorkingGroup?twiki_redirect_cache=5baafa4e9cd32a2890412e82817b01f5
