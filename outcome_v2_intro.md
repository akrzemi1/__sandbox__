# Outcome 2.0

Outcome is a set of tools for reporting and handling function failures in contexts where using C++ exception handling is unsuitable. Such contexts include:

  - situations, like in `filesystem` library, where the decision whether the failure should be handled remotely
  (use exceptions) or locally cannot be made inside the function and needs to be moved onto the caller;
  - parts of program that have a lot of branches depending on types of failures,
  where if-statements are cleaner than try-catch blocks;
  - parts of the programs/frameworks that themselves implement exception handling and cannot afford to use exceptions,
  like propagating failure reports across threads, tasks, fibers;
  - requirement that failure path of execution should not be longer than the succesful path of execution;
  - propagating exceptions trough layers that do not follow RAII design;
  - parts of the program that are compiled with exceptions disabled;
  - external requirement (such as company-wide policy) that failure handling paths are explicitly indicated in the code.

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

But most of the time you would inspect the object indirectly through a dedicated control statement. An implementation of `read_int_from_file` that has to (1) open the file, (2) read raw data to a buffer, and (3) interpret it as `int`, using the following three functions

```c++
auto open_file(string_view path) noexcept -> outcome::result<Handle>;
auto read_data(Handle& h)        noexcept -> outcome::result<Buffer>;     
auto parse(const Buffer& b)      noexcept -> outcome::result<int>;
```

Will look like this:

```c++
auto read_int_from_file(string_view path) noexcept
  -> outcome::result<int>
{
  OUTCOME_TRY(handle, open_file(path));    // decltype(handle) == Handle
  OUTCOME_TRY(buffer, read_data(handle));  // decltype(buffer) == Buffer
  OUTCOME_TRY(val, parse(buffer));         // decltype(val) == int
  return val;
}
```

Each occurence of `OUTCOME_TRY` is a control statement. If the returned `result<T>` contains an error information, the function is immediatelly returned with `result<U>` containing the same error information; otherwise object of type `T` is move-constructed on the stack.

---
**NOTE**

This is the v2 Outcome designed in response to feedback from a [Boost peer review held in May 2017](https://lists.boost.org/boost-announce/2017/06/0510.php). This library is expected to pass a second Boost peer review, and be submitted to WG21 for standardisation into the standard C++ library. 

---
