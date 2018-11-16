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
