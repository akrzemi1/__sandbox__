# Outcome 2.0

Outcome is a C++14 library for reporting and handling function failures
in contexts where using C++ exception handling is impossible or not desireable.

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


## Sample usage

Function that may need to report failure needs to reflect that in its return type:

```c++
auto read_int_from_file(string_view path) noexcept
  -> outcome::result<int>;
```

Function `read_int_from_file` will either return an `int` or information about 
the reason for failure.

It is possible to inspect the state manualy:

```c++
if (auto rslt = read_int_from_file("config.cfg"))
  use_int(rslt.value());
else
  report_error(rslt.error()); // returns error_code_extended
```

But most of the time you would use a control statement. An implementation of `read_int_from_file`
that has to (1) open the file, (2) read raw data to a buffer, and (3) interpret it as `int`, using the following three functions

```c++
auto open_file(string_view path) noexcept -> outcome::result<Handle>;
auto read_data(Handle& h)        noexcept -> outcome::result<Buffer>;     
auto parse(const Buffer& b)      noexcept -> outcome::result<int>;
```

Will look like this:

```c++
auto read_int_from_file(string_view path) noexcept
  -> outcome::expected<int>
{
  BOOST_OUTCOME_TRY(handle, open_file(path));        // decltype(handle) == Handle
  BOOST_OUTCOME_TRY(buffer, read_data(handle));      // decltype(buffer) == Buffer
  BOOST_OUTCOME_TRY(val, parse(buffer));             // decltype(val) == int
  return val;
}
```

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
