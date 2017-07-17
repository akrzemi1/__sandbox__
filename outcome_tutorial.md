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
outcome::result<void> print_half(const std::string& text)
```

Type `result<void>` means that there is no value to be retuned upon success, but that the operation might still fail, and we may be interested in inspecting the cause of the failure. Class template `rebind` is declared with attribute `[[nodiscard]]`, which means compiler will warn you if you forget to inspect the returned object.

The implementation will do the following: if the integral number can be represnted by an `int`, we will convert to `int` and use its arithmetical operations. If the number is too large, we will fall back to using a custom `BigInt` implementation taht needs to allocate memory. In the implementation we will use function `convert` defined in the previous section.

```c++
outcome::result<void> print_half(const std::string& text)
{
  if (outcome::result<int> r = convert(text))     // #1
  {
    std::cout << (r.value() / 2) << std::endl;    // #2
  }
  else
  {
    if (r.error() == ConversionErrc::TooLong)     // #3
    {
      OUTCOME_TRY (i, BigInt::fromString(text));  // #4
      std::cout << i.half() << std::endl; 
    }
    else
    {
      return r.as_void();                         // #5
    }
  }
  return outcome::success();                      // #6
}
```

1. You test if `result` object represents a cuccessful operation with contextual conversion to `bool`.
2. Function `value` extracts the successfully returned result.
3. Function `error` allows you to inspect the error sub-object, representing information about the reason for failure.
4. Macro `OUTCOME_TRY` represents a control statement. It implies that function call in the second argument returns a `result<>`. It is defined as:

   ```c++
   /*static*/ result<BigInt> BigInt::fromString(const std::string& s);
   ```

   Our control statement means: if `fromString` returned failure, this same error information should be returned from `print_half`, even though the type of `result<>` is different. If `fromString` returned success, we create  variable `i` of type `int` with the value returned from `fromString`. If control goes to subsequent line, it means `fromString` succeeded and variable of type `int` is in scope.

5. Function `as_void` must only be called on  a `result<>` object representing failure. It returns object of type `result<void>` containing the same error object, and can be converted to any `result<T>`, in particular to `result<void>`, while still preserving the same error information.
6. Function `success` returns an object of type `result<foid>` representing success.
