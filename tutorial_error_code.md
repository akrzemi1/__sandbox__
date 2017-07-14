
## Custom error code

This section illustrates how you can hook into the `std::error_code` system from
the Standard Library in order to work with your own set of error codes. As is usually
the case in C++, doing this is straightforward but requires typing boilerplate
to tell the C++ STL about your custom error type. This is not part of Outcome library, 
but we still provide this short guide here, because how to do this is not well documented [1].

Suppose you want to report all reasons for failure in conveting an `std::string` to a non-negative `int`.
The list is:

* `EmptyString` -- the input string is empty,
* `IllegalChar` -- input contains characters that are not digits,
* `TooLong` -- input represents a number, but this number would not fit into a variable of type `int`.

\snippet error_code_registration.cpp error_code_registration

This might look like a lot of extra boilerplate over simply using your custom
error code enum directly, but look at the advantages:

1. Any code which can speak `std::error_code` can now work with errors from your
   code, AND without being recompiled.
2. `std::system_error` can now wrap your custom error codes seamlessly, allowing
   your custom error code to be converted into a C++ exception *and back out again*
   without losing information.
3. `std::error_code` knows how to print itself, and will print your custom error
   code without extra work from you. As usually you'd need to define a print routine
   for any custom error code you'd write anyway, there is actually very little extra
   boilerplate here.
4. If you implement the `default_error_condition()` override, you can allow code
   exclusively written to understand `std::errc` alone to examine your custom error
   code domain for equivalence to the standard error conditions, AND without being
   recompiled.

[1]: The only documentation I'm aware of is the quite old guide by Chris Kohlhoff, founder of
ASIO and the Networking TS:

- http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-1.html
- http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-2.html
- http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-3.html
- http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-4.html
- http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-5.html


----


## Notes
Most people very keen on Expected entering C++ will be coming from a Swift or especially
Rust background where when you use `Result<T, E>`, you invariably supply a bespoke enum of
error codes for type `E` like this fragment of Rust:

~~~{.rs}
pub enum MathError {
  DivisionByZero,
  NegativeLogarithm,
  NegativeSquareRoot,
}

pub type MathResult = Result<f64, MathError>;
~~~

I will tell you now that this is a **bad idea** in large C++ codebases or in any code
expected to be used by others, and you should not do this with `expected<T, E>`.

The first reason is because C++ 11 brought in an infrastructure from Boost which allows
you to *non-intrusively* mark up **any** arbitrary third party error code system to
tell the C++ standard library about it such that any other C++ code using that system
can handle any third party error situation *without being recompiled* and *without
losing information*. If you refuse to use the C++ 11 standard way of working with
error codes, then you are being **anti-social** to other C++ code and devaluing the
experience of anybody else who works with your code. Moreover, it is extremely easy
to do the right thing and use the standard C++ 11 infrastructure for error codes, and
a worked example follows below.

The second reason is that if everybody goes off and locally defines their own custom error
code system, you force programmers coming later to your code to deal with a multitude
of local error code domains which may or may not mean the same thing. Expect a multiplication of lots of
little switch statements mapping one small domain of error code to another, all of which
must be maintained and parsed by programmers coming later to your codebase. The growing
maintenance burden over time is obvious, and a further worked example follows below.

The third reason is that the C++ 11 standard library already provides an enum of the
most common error codes for you so you don't feel like going off and reinventing
the wheel. It's called <a href="http://en.cppreference.com/w/cpp/error/errc">`std::errc`</a>
brought in by <tt>\#include &lt;system_error&gt;</tt> and as you'll see, it contains the standard
POSIX error codes, and most of the time you'll find that whatever custom error code
domain you are about to write can be adequately covered by `std::errc`.
In fact, let's try it:

~~~{.cpp}
// Mathematical "errors" we want to catch
using MathError = std::error_code;
static const MathError DivisionByZero = std::make_error_code(std::errc::result_out_of_range);
static const MathError NegativeLogarithm = std::make_error_code(std::errc::argument_out_of_domain);
static const MathError NegativeSquareRoot = std::make_error_code(std::errc::argument_out_of_domain);  

using MathResult = outcome::expected<double, MathError>;
~~~

Anything which can "speak" `std::error_code` can now work perfectly with your error
code system because you are using a standard error code domain (i.e. the POSIX error
code domain). And for reference, the C++ 11 STL ships with these ready to go predefined error
categories and error enumerations:

<dl>
  <dt><a href="http://en.cppreference.com/w/cpp/error/generic_category">`std::generic_category()`</a>
  with corresponding enumeration <a href="http://en.cppreference.com/w/cpp/error/errc">`std::errc`</a></dt>
  <dd>The POSIX standard error code domain as retrieved by the `errno` variable after calling
  a POSIX function.</dd>
  <dt><a href="http://en.cppreference.com/w/cpp/error/system_category">`std::system_category()`</a>
  with a platform specific enumeration</dt>
  <dd>The "usual" error code domain for your particular host OS, so on Linux/FreeBSD/OS X/Android
  this is the exact same domain as `generic_category()`. On Microsoft Windows this is the error code
  domain as returned by the Win32 `GetLastError()` function. Other platforms can vary.</dd>
  <dt><a href="http://en.cppreference.com/w/cpp/io/iostream_category">`std::iostream_category()`</a>
  with corresponding enumeration <a href="http://en.cppreference.com/w/cpp/io/io_errc">`std::io_errc`</a></dt>
  <dd>The STL iostreams error code domain.</dd>
  <dt><a href="http://en.cppreference.com/w/cpp/thread/future/future_category">`std::future_category()`</a>
  with corresponding enumeration <a href="http://en.cppreference.com/w/cpp/thread/future_errc">`std::future_errc`</a></dt>
  <dd>The STL future-promise error code domain.</dd>
</dl>

Because `std::error_code` is the C++ 11 standard way of doing error codes, `expected<T, E>`
defaults `E` to `std::error_code` both in the proposed standard and in Outcome's implementation.

The big problem with directly using `std::error_code` as `E` is the loss of type safety because
`std::error_code` will accept any input implicitly convertible. So you can write this code
and the compiler won't complain:

~~~{.cpp}
// Not a valid MathError, yet the compiler won't stop you doing this
return MathResult(make_unexpected(std::make_error_code(std::errc::executable_format_error)));
~~~

As is usually the case in C++, fixing this is straightforward but requires typing boilerplate
to tell the C++ STL about your custom error type. As how to do this is not well documented [1],
next follows a very quick howto guide.

[1]: The only documentation I'm aware of is the quite old guide by Chris Kohlhoff, founder of
ASIO and the Networking TS:
- http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-1.html
- http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-2.html
- http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-3.html
- http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-4.html
- http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-5.html


<hr><br>

\section custom_error_codes Marking up your custom error code domain so the STL understands it

Let's get into it! This is how you tell C++ 11's standard error code infrastructure about
your custom `expected<T, E>` error code type so it works well with anything speaking
`std::error_code`:

\snippet error_code_registration.cpp error_code_registration

This might look like a lot of extra boilerplate over simply using your custom
error code enum directly, but look at the advantages:
1. Any code which can speak `std::error_code` can now work with errors from your
code, AND without being recompiled.
2. `std::system_error` can now wrap your custom error codes seamlessly, allowing
your custom error code to be converted into a C++ exception *and back out again*
without losing information.
3. `std::error_code` knows how to print itself, and will print your custom error
code without extra work from you. As usually you'd need to define a print routine
for any custom error code you'd write anyway, there is actually very little extra
boilerplate here.
4. If you implement the `default_error_condition()` override, you can allow code
exclusively written to understand `std::errc` alone to examine your custom error
code domain for equivalence to the standard error conditions, AND without being
recompiled.
5. Type safety is preserved, you cannot construct a `MathError` from anything but
the officially sanctioned enums in `MathError`.

