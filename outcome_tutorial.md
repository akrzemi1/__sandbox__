# Tutorial



## Creating `result<>`

One notable effect of such plugging is that `ConversionErrc` is now convertible to `std::error_code`.
Now we can implement function `convert` as follows: 

```c++
outcome::result<int> convert(const std::string& str) noexcept
{
  if (str.empty())
    return ConversionErrc::EmptyString;
    
  if (!std::all_of(str.begin(), str.end(), ::isdigit))
    return ConversionErrc::IllegalChar;
    
  if (str.length() > 9)
    return ConversionErrc::TooLong;
  
  return atoi(str.c_str());
}
```

## Inspecting `result<>`

Suppose we will be wiriting function `print_halve` that takes an integral number (however big) represented in `std::string` and outputs a nuber which is twice smaller:


```c++
outcome::result<void> print_halve(const std::string& text)
```

The implementation will do the following: if the integral number can be represnted by an `int`, we will convert to `int` and use its arithmetical operations. If the number is too large, we will fall back to using a custom `BigInt` implementation taht needs to allocate memory. In the implementation we will use function `convert` defined in the previous section.

```c++
outcome::result<void> print_halve(const std::string& text)
{
    if (outcome::result<int> r = convert(text))
    {
        std::cout << (r.value() / 2) << std::endl;
    }
    else
    {
        if (r.error() == ConversionErrc::TooLong)
        {
            OUTCOME_TRY (i, BigInt::fromString(text));
            std::cout << i.half() << std::endl;
        }
        else
        {
            return r.as_void();
        }
    }
    return outcome::success();  
}
```
