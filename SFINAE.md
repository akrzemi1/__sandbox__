A *hard ill-formedness reason* is one of the following situations:  
- The evaluation of the substituted types and expressions causes the instantiation of class template specialization and/or function template specialization, and such a specialization is ill-formed.
- The evaluation of the substituted types and expressions causes the generation of non-deleted implicitly-defined function, and the generated definition is ill-formed.

An expression `E` is well formed *declaration-wise* when:
- All subexpressions of `E` involve calls to functions and operators that are unambiguously selected in the course of overload resolution,
- All selected functions and operators are accessible in the sense of access checking in the context unrelated to the types of sub-expressions of `E`.
- None of the selected funciotns or operators is deleted.
- During the substitution no situation occurs that would make the program ill-formed, with a diagnostic required. 
  [*Note:* If no diagnostic is required, the program is still ill-formed. *--end note*]

No instantiation of class template specializations or function template specializations or
the generation of implicitly-defined functions is performed when determining if `E` is well formed declaration-wise.

[*Note:* The evaluation of the substituted types and expressions can result in side effects
such as the instantiation of class template specializations and/or function template specializations,
the generation of implicitly-defined functions, etc. Such side effects resulting from substitutions
in `E` can result in the program being ill-formed, but `E` may still be well formed declaration-wise.
*--end note*]

----------------------------
return type
`typename trait<T>::type`
`auto f(x) { ... }; auto fun() -> decltype(f(x))`
