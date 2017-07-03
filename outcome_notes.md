
# Notes


```c++
auto open_file(string_view path) noexcept
  -> outcome::result<Handle>;                // returns either a Handle or error_code_extended

auto read_data(Handle& h) noexcept
  -> outcome::result<Buffer>;                // returns either a Buffer or error_code_extended

auto parse(const Buffer& b) noexcept
  -> outcome::result<int>;                   // returns either an int or error_code_extended
```

`error_code_extended`

Once you have decided not to use exceptions in some part of the program,
Outcome offers convenient abstractions for you, superior to error codes.

Even if you want to use exception handling in your program, you may still want
to use Outcome in some places


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
