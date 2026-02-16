CWG1844  https://cplusplus.github.io/CWG/issues/1844.html (the term)
https://cplusplus.github.io/CWG/issues/2296.html (default fun)
https://cplusplus.github.io/CWG/issues/2776.html (implementaiton limits)
https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2025/p3606r0.html


https://godbolt.org/z/1bcr8arhM
(def arguments)
MSVC: 1
Clang: error
g++: 0
ICC: 1

Corentin Jabot: So context here, the issue we have is implementation divergence. GCC and MSVC have default arguments in the immediate context, and Clang does not. From the spec, it doesn't make sense to have it in the immediate context. In clang people thought it should be in the immediate contexts, others thought not. It affects lambdas, whether you instantiate the lambdas, it may mangle differently. It will affect ABI. I think we need to be really careful here. Defining immediate context is massively important. Maybe we postpone this and make it better

https://eel.is/c++draft/temp.inst#11

https://github.com/llvm/llvm-project/issues/63391

If the same default parameter for the same function renders a different expression and a different result in the instantiation is it an ODR violation?

JT: https://godbolt.org/z/vrv57ne3e

https://godbolt.org/z/3vn8Y7oqf -- Barry

https://godbolt.org/z/hv7To53r3


https://cplusplus.github.io/CWG/issues/3065

A program that refers to a deleted function implicitly or explicitly, other than to declare it, is ill-formed.

https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0348r0.html

the history
