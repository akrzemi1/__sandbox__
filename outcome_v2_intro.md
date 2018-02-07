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
  
Outcome addresses failure handling through returning a special type form functions which is able to store either a succesfully computed value (or `void`) or the information about failure. Outcome also comes with a set of idioms for dealing with such types.

## Sample usage

One of the tools in the Outcome library is `result<T>`: it represents either a succesfully computed value of type `T` or an `std::erro_code` representing the reason for failure. You use it in the function's return type:

```c++
auto read_data_from_file(string_view path) noexcept
  -> outcome::result<string>;
```

It is possible to inspect the state manualy:

```c++
if (auto rslt = read_data_from_file("config.cfg"))
  use_string(rslt.value());   // returns string
else
  report_error(rslt.error()); // returns error_code
```

Or, if this function is called in anoter function that also returns `result<T>` you can use a dedicated control statement:

```c++
auto process(const string& content) noexcept
  -> outcome::result<int>;
  
auto get_int_from_file(string_view path) noexcept
  -> outcome::result<int>
{
  OUTCOME_TRY(str, read_data_from_file(path)); // decltype(str) == string
  return process(str);
}
```

Each occurence of `OUTCOME_TRY` is a control statement. If the returned `result<T>` contains an error information, the function is immediatelly returned with `result<U>` containing the same error information; otherwise object of type `T` is move-constructed on the stack.

---
**NOTE**

This is the v2 Outcome designed in response to feedback from a [Boost peer review held in May 2017](https://lists.boost.org/boost-announce/2017/06/0510.php). This library is expected to pass a second Boost peer review, and be submitted to WG21 for standardisation into the standard C++ library. 

---
