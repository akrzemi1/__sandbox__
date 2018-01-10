
## Why two types `result<>`  and `outcome<>` rather than just one?

The two types represent slightly different things. Using two types allows expressing programmer's intentions more directly.

Function signature `result<T> f() noexcept` means that any failures in `f` are exclusively reported by returning an error code. 

Similarly, signature `result<T> f()` means that in addition to the above, some failures in `f` are signalled by throwing exceptions. 

Now, signature `outcome<T> g() noexcept` means that in the lower layers of the implementation we have two kinds of libraries/modules: some report failures via error codes, and some by throwing exceptions. But at the level of our interface we do not want to throw exceptions; so we forward both error codes and exceptions unaltered. The callers can easily tell which type of failure reporting was chosen, and extract it appropriately.

This implies that `outcome<>` has different interface that allows you to ask whether we are storing an exception or an error code to rethrow the stored exception but to return by value the error code, etc.

There is one additional possibility. When you know there will be no error codes returned -- only exceptions thrown, but at some layer you want to temporarily change the stack unwinding into returning failures through return values, you can use signature

```
result<T, std::exception_ptr> f() noexcept;
```
