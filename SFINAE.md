Objective
=========

Define precisely when SFINAE occurs (formalize "immediate context" from 14.8.2)

Wording (2)
==============

A type or an expression `E` is said to be in a *remote context* of the function type and its template parameter types, when:
* a class template specialization is instantiated in the process of substitution, and `E` is a member thereof, or
* a function template specialization is instantiated in the process of substitution, and `E` occurs in its definition, or
* an implicitly-defined non-deleted function function is generated and `E` occurs in its definition.

An invalid type or expression is one that would be ill-formed, with a diagnostic required, if written using the substituted arguments. If a substitution results in an invalid type or expression in a remote context, the program is ill-formed.
If a substitution results in an invalid type or expression in a non-remote context, type deduction fails. Access checking is done as part of the substitution process, in the context unrelated to the types of sub-expressions of `E`.

An expressions `X` is said to be *superficially well-formed* when its substitution ends in type deduction success.


High level
==========

outcomes from checking surface well-formedness:


Old wording
=======

In [temp.deduct] replace paragraph 8 (minus examples) with:
-------------------------------

A *hard ill-formedness reason* is one of the following situations causing a program to be ill-formed:  
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

Modify [meta.unary.prop]/7:
----------------------------

the predicate condition for a template specialization `is_constructible<T, Args...>` shall be satisfied
if and only if the following variable definition would be well-formed **declaration-wise** for some invented variable `t`:

Also change:
-------------
Table 57, `result_of`




List of conditions
==================

Conditions applicable for SFINAE:
* referring to a non-existent member type-name
* referring to a deleted function
* refering to a class meber with insufficient access level (private, protected)
* using a function or operator, which results in overload resolution failure

!!
* (1) Attempting to instantiate a pack expansion containing multiple parameter packs of differing lengths.
* a substitution of a template argument for a non-type value template parameter would cause a narrowing conversion ([dcl.init.list] para 7).


Hard errors:
- The evaluation of the substituted types and expressions causes the instantiation of class template specialization and/or function template specialization, and such a specialization is ill-formed.
- The evaluation of the substituted types and expressions causes the generation of non-deleted implicitly-defined function, and the generated definition is ill-formed.
- The evaluation of the returned type of a function or function template requires the body of this function to be instantiated, and this instantiation causes the program to be ill-formed.
- The evaluation of the substituted types and expressions causes implementation limits to be exceeded.

``` (1)
template <typename ...Ts, typename ...Us>
auto f(std::tuple<Ts...> t, std::tuple<Us...>) -> std::tuple<std::common_type<Ts, Us>...> 
{ return {}; }

int f(...) { return -2; }
    
int main()
{
    std::tuple<int, float> ta;
    std::tuple<int> tb;
    std::cout << f(ta, tb) << std::endl;
    //std::cout << f<10000>(0) << std::endl;
}
```

Notes
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
