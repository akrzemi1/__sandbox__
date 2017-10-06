Outcome
=======

```c++
auto Layer1::f() -> int; // may throw

auto Layer2NX::g() -> outcome::result<int> noexcept;

auto Layer2NX::f() -> outcome::outcome<int> noexcept
try {
  return Layer1::f();
}
catch (...) {
  return std::current_exception();
}

auto Layer2NX::fun() -> outcome::outcome<int> noexcept
{
  int * i = new int{0};
  OUTCOME_TRY (fr, Layer2NX::f());
  *i += fr;
  OUTCOME_TRY (gr, Layer2NX::g());
  *i += gr;
  int ans = *i;
  delete i;
  return ans;
}

auto Layer3::run() -> int // may throw
{
  if (outcome::outcome<int> o = Layer2NX::fun())
    return o.value();
  else if (o.has_exception())
    std::rethrow_exception(o.exception());
  else
    throw std::system_error{o.error()};
}
```
