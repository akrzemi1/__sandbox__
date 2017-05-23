```c++
struct Handle; // A RAII-like file handle
struct Buffer; // for chunk of data

auto open_file(std::string_view path) noexcept
  -> outcome::expected<Handle>
{
  if (file_does_not_exist(path))
    return outcome::make_unexpected(std::errc::no_such_file_or_directory);
    
  if (system_cannot_open_yet_another_file())
    return outcome::make_unexpected(std::too_many_files_open);
    
  Handle h {/*...*/}
  return h;
}

auto read_data(Handle& h) noexcept
  -> outcome::expected<Buffer>
{
  if (invalid_handle(h))
    return outcome::make_unexpected(std::bad_file_descriptor);
    
  if (cannot_read_data(h))
    return outcome::make_unexpected(std::io_error);
    
  Buffer b {/*...*/} // populate
  return b;
}

auto read_config_from_file(std::string_view path) noexcept
  -> outcome::expected<Buffer>
{
  BOOST_OUTCOME_TRY(handle, open_file(path));   // if open_file() is successful
                                                // initializes object of type Handle
                                                // otherwise returns with the same error_code
    
  BOOST_OUTCOME_TRY(buffer, read_data(handle));
  
  return buffer;
}
```
