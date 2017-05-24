# The initial example of Niall Douglas's Boost.Outcome library.

```c++
namespace outcome = BOOST_OUTCOME_V1_NAMESPACE;      // import namespace outcome

// The task is to write function read_int_from_file() which takes the name of the file
// and returns an int parsed from this file, or reports failure to do so.
// But for some reason, we do not want to signal failures with exceptions.

auto read_int_from_file(string_view path) noexcept
  -> outcome::expected<int>                          // returns either an int or std::error_code

// We will be using the following types and functions in implementation. They also signal failures
// by a 'combined' return type: outcome::expected<T>.

struct Handle; // A RAII-like file handle
struct Buffer; // Represents chunk of read data

auto open_file(string_view path) noexcept
  -> outcome::expected<Handle>;                      // returns either a Handle or std::error_code

auto read_data(Handle& h) noexcept
  -> outcome::expected<Buffer>;                      // returns either a Buffer or std::error_code

auto parse(const Buffer& b) noexcept
  -> outcome::expected<int>;                         // returns either an int or std::error_code

// This is the implementation of read_int_from_file(). It combines the thre funcitons:
// parse(read_data(open_file(path))) but with a proper error handling

auto read_int_from_file(string_view path) noexcept
  -> outcome::expected<int>
{
  outcome::expected<Handle> rslt = open_file(path);  // rslt is either a Handle or an std::error_code 
  
  // 'manual' inspection of an expected<>:
  
  if (!rslt)                                         // does rslt store an error_code (failure)?
    return outcome::make_unexpected(rslt.error());   // return the error_code up
  Handle & handle = rslt.value();                    // access the stored value of type Handle
    
  // 'automated' inspection, error reporting, and value inspection:
   
  BOOST_OUTCOME_TRY(buffer, read_data(handle));      // if read_data() returns an error_code
                                                     // it is returned up; otherwise
                                                     // object buffer of type Buffer is move-constructed
  
  BOOST_OUTCOME_TRY(val, parse(buffer));             // unless this returns, val is of type int
  
  return val;                                        // a positive return
}
```

### The full .cpp version:

```c++
#include <boost/outcome.hpp>
#include <experimental/string_view>
namespace outcome = BOOST_OUTCOME_V1_NAMESPACE;
using std::experimental::string_view;

struct Handle {}; // A RAII-like file handle
struct Buffer {}; // Represents chunk of read data

auto open_file(string_view path) noexcept
  -> outcome::expected<Handle> { return Handle{}; }                      // returns either a Handle or std::error_code

auto read_data(Handle& h) noexcept
  -> outcome::expected<Buffer>                      // returns either a Buffer or std::error_code
{ return Buffer{}; }

auto parse(const Buffer& b) noexcept
  -> outcome::expected<int>                         // returns either an int or std::error_code
{ return 1; }

auto read_int_from_file(string_view path) noexcept
  -> outcome::expected<int>                          // returns either an int or std::error_code
{
  // this function implements parse(read_data(open_file(path))) 
  // plus error handling
  
  outcome::expected<Handle> rslt = open_file(path);  // rslt is either a Handle or an std::error_code 
  
  // 'manual' inspection of a result<>:
  
  if (!rslt)                                         // does rslt store an error_code (failure)?
    return outcome::make_unexpected(rslt.error());   // return the error_code up
  Handle & handle = rslt.value();                    // access the stored value of type Handle
    
  // 'automated' inspection, error reporting, and value inspection:
   
  BOOST_OUTCOME_TRY(buffer, read_data(handle));     // if read_data() returns an error_code
                                                     // it is returned up; otherwise
                                                     // object buffer of type Buffer is move-constructed
  
  BOOST_OUTCOME_TRY(val, parse(buffer));             // unless this returns, val is of type int
  
  return val;                                        // a positive return
}

int main () {}
```
