# Tutorial

## Outcome 2.0 namespace

It is recommended that you refer to entities from this Outcome 2.0 via the following namespace alias:

```c++
namespace outcome = OUTCOME_V2_NAMESPACE;
```

As patches and modifications are applied to this library, namespaces get added in order
not to break any backward compatibility. At some point namespace `outcome::v2` will be defined,
and this will be the prefered namespace. Until then `OUTCOME_V2_NAMESPACE` denotes the most recently
updated version, getting closer to `outcome::v2`.

## Creating `result<>`

We will define a function that converts an `std::string` to an `int`. This function can fail for a number of reasons,
if it does we want to communicate the failure reason.

```c++
outcome::result<int> convert(const std::string& str) noexcept;
```

Class template `result<T, EC>` has two template parameters. The first (`T`) represents the type of the object
returned from the function upon succes; the second (`EC`) is type of object storing information about the reason
for failure when function fails. A `result<T, EC>` object either stores a `T` or an `EC` at any given moment,
and is therefore conceptually similar to `boost::variant<T, EC>`. `EC` is defaulted to `std::error_code`.
If both `T` and `EC` are trivially copyable, `result<T, EC>` is also trivially copyable.

Now, we will define an enumeration describing different failure situations during conversion.

```c++
enum class ConversionErrc
{
  EmptyString = 1; // 0 is never an error
  IllegalChar = 2;
  TooLong     = 3;
};

// all boilerplate necessary to plug ConversionErrc
// into std::error_code framework
```

Assume we have plugged it into `std::error_code` framework, as described in this series of articles by Chris Kohlhoff:
 * [System error support in C++0x - part 1](http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-1.html)
 * [System error support in C++0x - part 2](http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-2.html)
 * [System error support in C++0x - part 3](http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-3.html)
 * [System error support in C++0x - part 4](http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-4.html)
 * [System error support in C++0x - part 5](http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-5.html)

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

`result<T, EC>` is convertible from ant `T2` convertible to `T` as well as any `EC2` convertible to `EC`,
provided that the conversion is not ambiguous. If some type `X` is both convertible to `T` and `EC`, 
conversion to `result<T, EC>` fails to compile. In this case you need to use one of the tagged constructors:


```c++
outcome::result<int> r {outcome::in_place_type<std::error_code>, ConversionErrc::EmptyString};
outcome::result<int> s {outcome::in_place_type<int>, 1};
```
