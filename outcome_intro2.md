```c++
struct Handle; // A RAII-like file handle
struct Buffer; // Represents chunk of read data

auto open_file(std::string_view path) noexcept
  -> outcome::expected<Handle>                 // returns either a Handle or std::error_code
{
  if (file_does_not_exist(path))
    return outcome::make_unexpected(std::errc::no_such_file_or_directory);
    
  if (system_cannot_open_yet_another_file())
    return outcome::make_unexpected(std::too_many_files_open);
    
  Handle h {/*...*/} // initialize
  return h;          // convert to outcome::expected<Handle>
}

auto read_data(Handle& h) noexcept
  -> outcome::expected<Buffer>
{
  if (invalid_handle(h))
    return outcome::make_unexpected(std::bad_file_descriptor);
    
  if (cannot_read_data(h))
    return outcome::make_unexpected(std::io_error);
    
  Buffer b {/*...*/} // populate
  return b;          // outcome::expected<Buffer>
}

auto read_config_from_file(std::string_view path) noexcept
  -> outcome::expected<Buffer>
{
  outcome::expected<Handle> handle = open_file(path); // either a Handle or an std::error_code 
  if (!handle)                                        // open_file() failed?
    return outcome::make_unexpected(handle.error())   // return the error_code up
    
  BOOST_OUTCOME_TRY(buffer, read_data(*handle));      // if read_data() succeeds, initialize
                                                      // object buffer of type Buffer
                                                      // otherwise return the error_code up
  
  return buffer;                                      // a positive return
}
```
