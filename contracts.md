Contracts
=========

Use cases
---------

### Evaluate cheap checks in production

"Cheap" means evaluating them does not add significant overhead compared to the cost of the function that is guarded by the check.

If such check evaluetes to false, we may want to:
* Terminate the program
* Throw an exception: this is not quite safe because program in an incorrect state cannot be guaranteed to safely perform stack unwinding. But there is a likelihood that stack unwinding will escape a module that contains a bug, and the bug becomes isolated.

Q: can I make the right choice and impose it on others as to which checks are "sufficiently inexpensive"?

### Some checks should be ignored in production

But sometimes when debugging we may want to turn them on at the expense of slowing down the program.
