Maybe I can answer this. The most general answer is that Boost.Outcome is
for people who want to reasonably decently report and handle failures and
at the same time choose, or are forced, not to use exceptions.

The reasons for not using exceptions may be different:

   - Real or imaginary concerns about performance
   - A personal style of programming
   - Company's policy to compile with exceptions disabled
   - Maintaining a code base that was never designed with exception-safety
   in mind
   - Making all control paths explicit, as opposed to having hidden control
   paths caused exceptions potentially thrown from any place
   - Parts of the programs/frameworks that themselves implement exception
   handling and cannot afford to use exceptions, like propagating failure
   reports across threads, tasks, fibers...

Once you have decided not to use exceptions in some part of the program,
Boost.Outcome offers convenient abstractions for you, likely superior to
error codes. It implements 95% the currently being standardized
`std::expected` library, plus a number of other useful additions.


> There is still this case where you want to avoid exceptions in one part of
> the program, but still use exceptions in other parts of the program. For
> one example. in one "task" you launch another task with a callback
> function `f`. You might want to throw from `f` and this is valid. But the
> framework for launching tasks cannot afford to spill exceptions, so they
> have to catch and somehow transport the exception (e.g., by exception_ptr)
> then in the other task, you might want to re-convert to an exception.
> (something similar to what std::future does.) 

> Other situation, is when you are using a third-party library, which throws
> exceptions. You want to wrap it into a component that does not throw.
> Boost.Outcome also supports in a convenient way this transition from and
> to exceptions. 
