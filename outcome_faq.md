
## Why two types `result<>`  and `outcome<>` rather than just one?

The two types represent slightly different things. Using two types allows expressing programmer's intentions more directly.

Function signature `result<T> f() noexcept` means that any failures in `f` are exclusively reported by returning an error code. 

Similarly, signature `result<T> f()` means that in addition to the above some failures in `f` are additionally signalled by throwing exceptions. 

Now, signature `outcome<T> g() noexcept` means that in the lower layers of the implementation we have two kinds of libraries/modules: some report failures via error codes, and some by throwing exceptions. But at the level of our interface we do not want to throw exceptions, so we forward both error codes and exceptions (unaltered). And the callers can easily tell which type of failure reporting was chosen end extract it appropriately.
