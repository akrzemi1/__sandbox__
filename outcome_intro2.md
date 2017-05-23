```c++
struct Handle; // A RAII-like file handle
struct Buffer; // Represents chunk of read data

auto open_file(std::string_view path) noexcept
  -> outcome::expected<Handle>;                       // returns either a Handle or std::error_code

auto read_data(Handle& h) noexcept
  -> outcome::expected<Buffer>;                       // returns either a Buffer or std::error_code

auto parse(const Buffer& b) noexcept
  -> outcome::expected<int>;                          // returns either an int or std::error_code

auto read_int_from_file(std::string_view path) noexcept
  -> outcome::expected<int>                           // returns either an int or std::error_code
{
  // this function implements parse(read_data(open_file(path))) 
  // plus error handling
  
  outcome::expected<Handle> rslt = open_file(path);  // rslt is either a Handle or an std::error_code 
  
  // 'manual' inspection of a result<>:
  
  if (!rslt)                                         // does rslt store an error_code (failure)?
    return outcome::make_unexpected(rslt.error());   // return the error_code up
  Handle & handle = rslt.value();                    // access the stored value of type Handle
    
  // 'automated' inspection, error reporting, and value inspection:
   
  BOOST_OUTCOME_TRY(buffer, read_data(*handle));     // if read_data() returns an error_code
                                                     // it is returned up; otherwise
                                                     // object buffer of type Buffer is move-constructed
  
  BOOST_OUTCOME_TRY(val, parse(buffer));             // unless this returns, value is of type int
  
  return val;                                        // a positive return
}
```
