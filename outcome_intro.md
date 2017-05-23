```c++
// Imagine the libraries you depend on each use different mechanisms for returning errors

namespace outcome = BOOST_OUTCOME_V1_NAMESPACE;

namespace Library1
{                                         // Like the Filesystem and Networking TS,
  auto fun(std::error_code &) noexcept    // returns errors via lvalue ref to an error_code.
    -> int;                               // Never throws exceptions.
}
  
namespace Library2                        // The Expected proposed for standardisation in C++ 20
{                                         // Returns an int (expected) or some custom 
  auto fun() noexcept                     // error code (unexpected). Never throws exceptions.
    -> outcome::expected<int, error_code>; 
}  
  
namespace Library3                        // Result is an int (not error)
{                                         // or an outcome::error_code_extended (error)
  auto fun() noexcept                     // Never throws exceptions.
    -> outcome::result<int>;
}
  
// function my_fun() can signal an error in a uniform way;
// outcome::result is an int, or an outcome::error_code_extended

auto my_fun() noexcept
  -> outcome::result<int>
{
  std::error_code ec;
  int i = Library1::fun(ec);
  if (ec)
    return outcome::make_errored_outcome(ec);  // error code returned inside outcome
    
  BOOST_OUTCOME_TRY(rslt2, Library2::fun());   // if fun() succeeds, rslt2 of type int contains result
                                               // otherwise returns the result of fun() up   
                                                          
  BOOST_OUTCOME_TRY(rslt3, Library3::fun());   // Similarly, either rslt3 is an int with successful result,
                                               // or my_fun() returns what Library3::fun() has returned

  return i + rslt3;                            // return a result with a value
};

// using functions' outcome:

void test()
{
  if (auto r = my_fun())                       // my_fun() succeeded
    process_int(r.value());                    // access the value
  else
    process_error_code(r.error());             // inspect error code
}
```
