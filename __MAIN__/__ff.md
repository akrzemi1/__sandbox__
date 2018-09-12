
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



----------------------


Random notes
============

"Error translation only" may not be necessary with `error_code`.
overhead of exceptions:
* RTTI
* additional binary size vs more branches
* exception stacks and thread local storage
* no worse-case predictable guarantees


