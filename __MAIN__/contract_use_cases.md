1
=

Josh: The fundamental conceit of defensive checking is that your checks verify that the program when executed without those checks will still not violate those contracts.
This depends on the checks not intrinsically changing the control flow of the enclosing program when they are evaluated.  

2
=

Need a handler to re-initialize the globals. Or to commit transaction before the abort.

3
=

If I do not have `Check_and_recover`, I may need to resort to using exceptions.
