* pokaż, że `std::error_code` i `int` można konwertować do `result<int>`.
* Zmień tytuł na "Handling Function Failures"
* "rozwiń akronim RAII.
* "light-weight throw" - może być zmylające.
* Więcej o Boost.Exception. po co ten tag, co to path. Może dwa chunki: from_path i to_path.
* co to znaczy, że Outcome jest w Boost, skoro go nie ma?

---------

* Strzałki mają pojawiać się pojedynczo
* `convert()` który bierze output by reference: what failure safety?
* failure safety: not exception safety (return safety)
* guaranteed copy elision: nie mieszaj z `File::create()`, później mutex pookaź
* JSON z bad request, powinien mieć jeszcze numerek

-------------

* Remove ASIO example
* Divide: errors you have to be aware of, and those you dont
* END: (xml responses) design error reporting from the outset
* `save()` example: ealborate: what good is reporting failure to the user if we failed to save his data in a Word processor?

---------

show how you signal failure in constructor w/o exceptions (special state).

Begin: if you handle anyhow it is good already.
ignore by try-catch(...)

error hadling callbacks, coroutines

Mention RAII even in `return` example.

Mention `std::error_code` earlier, along Boost.Exception.

Mention Herb's definition, "cannot meet what is advertised" -> but what should we advertise?

Should File have strong or weak invariant? -> low-level library author will only want to write one function. 

```c++
template <Invocable F>
auto catch_(F f) noexcept -> result<invoke_result_t<F>>
{
  static_assert(throws(f()) == noexcept_t::static_except);
  try                 { return f();          }
  catch(std::error e) { return std::move(e); }
}

// ...

result<mutex> m = catch_([&]{ return make_mutex(); });
```

+ can throw through C functions. -- But what about exception safety in C functions?

-------------


Error codes (die)
-----------------

We start with error codes. (code is C++-ified, but C error handling is preserved)
(in robust programs die() has to be changed to "report failure and try to recover).


Error codes
-----------------

```c++
int e = f()
if (!e)
  return e;
// ...
```

Important observation: error codes in C are a proof that one can live without exceptions and report failures.

This pattern is all over, so common that it is worth optimizing the language for it...


The dependency pattern
----------------------

```c++
f(); 
g(); // <-- g being below f means "call g() only if f()" succeeds --> an instruction dependency is set and understood by compiler
h();
```

It scales.
This shows why you should not try-catch exceptions in random places: why break this dependency chain?.

--> Do not catch exceptions, unless in well designed places or for translation only

Exceptions compete with error codes in many ways, but the most important one is this dependency.


Exception handling vs errors codes
----------------------------------

For exceptions:
* The dependency pattern
* arbitrary data,
* throwing through C functions,
* does not require a "channel" in the signature:
  * does not affect function interface (if you change from not  throwing to throwing)
  * can use return value for transporting successfully computed result,
  * can use with constructors (and operators),
  * can use in destructors
* not disturbing positive program flow,
* avoiding two phase init.
* if you forget to handle, default action usually makes sense

For error codes:
* minimum overhead (expand)
* predictable worse case performance
* explicit exceptional paths (for some)
* fewer surprising control paths
  * (but not none: we still have early returns)

Some are contradictory expectations. Some are just deficiencies in the design.


RAII
----

```c++
int e = f()
if (!e)
  return e;
```

we have a memory leak. Also, this would not work:

```c++
acquire();
f();
g();
h();
release(); // (dont use fclose in example)
```

Another common use pattern (easily done wrong): acquire, and if succeeded, schedule release for the end of scope: error or not.

This is what we have got RAII for. RAII is useful even when exceptions are not used (although slightly different).
If used only for `release()` (no logic completion (mention fclose)), we can see why there is no need to throw from destructor:
you do not want to start cancellation cascade: function did its job.


RAII (beter than `finally`)
---------------------------
Because of scoping rules


Sending arbitrary data
----------------------
Is it useful?

What information should be carried with error?

* if errors are handled locally: not much -- every required data is in scope
* If you catch at distant places, you cannot even imagine all the types and datas. You need a common interface for extracting data
  * Use `boost::exception`: one type but store arbitrary data...
Usually I use two types: they differ on where to catch them.



Is it an error yet?
-------------------

`convert()`?
`find()`?
Typical guidance: 
* do not throw if the exception is expected to be handled locally.
* be prepared that your exception might unwind the app up to the entry point, and stop it.
* would this guidance hold if exception handling was cheap?


bad example: boost::lexical_cast
---------------------------------
1. Not being able to convert to T is not necessarily an error: often I call it to check if conversion is possible. -- it is the context that determines whether this is a failure or not.
2. Sometimes I just want to go with default value if I cannot produce a T.
3. If I want to throw, I want to throw a different type than the hard-coded bad_lexical_cast
4. I know I want to recover locally from the failure.


lets use try_convert instead:
-----------------------------

what if you forget to handle an error? --> `[[nodiscard]]`
with `[[nodiscard]]` the trade-off changes: you cannot forget to handle: the compiler will remind you: you decide if you want the avalanche or to stop locally.
You may still consider throwing a better solution, but you have to rethink the trade-off
dont want to spoil your function declarations with `[[nodiscard]]`?
-- annotate the type:

```c++
struct Failure [[nodiscard]]
{
  // ...
};
```





----------------------


Random notes
============

"Error translation only" may not be necessary with `error_code`.
overhead of exceptions:
* RTTI
* additional binary size vs more branches
* exception stacks and thread local storage
* no worse-case predictable guarantees

Most bugs (resource leaks, UB, crashes) manifests ater incorrect error handling: it occurs seldom and is least tested. 

study copy_file -- when exceptions
example with parse config (sometimes error sometimes throw

"exception safety issues" -> "failure safety issues"

clang implementation does not use exceptions

"difficult to test all control paths"? -- is it easier with error codes?
monad, but C++ style (imperative)


http://www.open-std.org/jtc1/sc22/wg14/www/docs/n2289.pdf

exceptions can cause a security concern: someone might be deliberately hitting exceptional path and slowing down the server by orders of magnitude.
