Wording
=======

In [temp.deduct] replace paragraph 8 (minus examples) with:
-------------------------------

A *hard ill-formedness reason* is one of the following situations:  
- The evaluation of the substituted types and expressions causes the instantiation of class template specialization and/or function template specialization, and such a specialization is ill-formed.
- The evaluation of the substituted types and expressions causes the generation of non-deleted implicitly-defined function, and the generated definition is ill-formed.
- The evaluation of the returned type of a function or function template requires the body of this function to be instantiated, and this instantiation causes the program to be ill-formed.
- The evaluation of the substituted types and expressions causes implementation limits to be exceeded.

An expression or a type `E` is *ill-formed declaration-wise* when it is ill formed for at least one reason with a diagnostic required, other than hard ill-formedness reasons. In determining the ill-formedness declaration-wise access checking is done in the context unrelated to the types of sub-expressions of `E`.

An expression or type is *well-formed declaration-wise* when it is not ill-formed declaration-wise. [*Note:* In order to determine the well-formedness declaration-wise the implementation need not perform actions listed in hard ill-formedness reasons: instantiaitng class template specializations and/or function template specializations, generating the bodies of implicitly-defined functions or functions with deduced return type. *--end note*] [*Note:* An expression may be determined to be well-formed declaration-wise and yet its odr-use may cause a program to be ill-formed. *--end note*] 

If a substitution results in a type or expression that is ill-formed declaration-wise type deduction fails.

Modify [dcl.fct.def.delete]/2
-----------------------------

A program that refers to a deleted function implicitly or explicitly, other than to declare it **or to determine if some expression or type is well formed declaration-wise (see [temp.deduct])**, is ill-formed.
 
In Table 49 (Type property predicates) modify Condition for `is_assignable` to read:
-----------------------------------------------------------------

The expression `declval<T>() = declval<U>()` is well-formed **declaration-wise** when treated as an unevaluated operand (Clause 5).

=====

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