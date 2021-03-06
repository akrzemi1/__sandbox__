## The Programming Model

### Declaring criteria for bugs

The contract support facility that we propose enables programmers to declare in the source code that certain values of
objects observed at certain times necessarily indicate a bug somewhere in the program. 


```c++
T& Container::get(int i)
  [[pre: i >= 0]]       // precondition
  [[pre: i < size()]]   // precondition
{
  [[assert: buffer_ != nullptr]];  // block-scope invariant
  return buffer_[i];
}
```

Here, the two preconditions specify that whenever function `Container::get` is invoked, just before its execution, if either of expressions `i >= 0` or `i < size()` were to evaluate to `false`, it would indicate a bug in the program. Class author can make that declaration because she understands the human-to-human class/function contract: that the `int` value passed to this function represents an abstract notion of an *index* in a container.

The assertion inside the function body indicates that when control reaches this statement, if expression `buffer_ != nullptr` were to evaluate to `false` it would indicate a bug in the program. Class author can make that declaration, because she knows the purpose for which she put pointer `buffer_` as a class member, and she knows the rules based on which it will change value. 

In either case, the programmer has a knowledge about the program that the machines do not. By making these declarations machines obtain part of this knowledge: they can now precisely determine that a program has a bug. 

Different machines can make different use of these contract annotations. Static analyzers can perform a symbolic evaluation of the program and determine if there exist combinations of external input that lead to any of the contract annotations producing value `false`.

Compilers, under special flags, can inject runtime-checks in place of these declarations, which would make the above function behave as if it was defined so:

```c++
T& Container::get(int i)
{
  if ((i >= 0) == false) { std::cerr << violation("i >= 0"); std::abort(); }
  if ((i < size()) == false) { std::cerr << violation("i < size()"); std::abort(); }
  if ((buffer_ != nullptr) == false) { std::cerr << violation("buffer_ != nullptr"); std::abort(); }
  return buffer_[i];
}
```

Tools can do these things only because the programmer gave them this additional information about what constitutes a bug.


### Controlling the tools

Tools are resource-limited. In order for them to make an effective use of contract annotations, the programmer or someone else needs
to be able to instruct them to treat different contract annotations diffeently. 

In case of static analyzer, a programmer may want to say, "just take for granted that contract declarations flagged with `checked` are always satisfied, and only check for potential vioations in the remaining ones."

In case of compiler injecting runtime checks, a programmer might want to say, "inject checks only for contract annotations flagged as `critical`."


### Other applications of the model

Technically, it is possible to use contract annotations for purposes different than bug detection. 
One such notable purpose is code transformations that guarantee to leave the bug-free programs unchanged 
and possibly optimized, while at the same time arbitrarily changing the semantics of programs that have 
bugs diagnosable through contract annotations. This is sometimes (imprecisely) referred to as contract-based optimizations. 

Contract-based optimizations are compatible with our programming model (users declare what they consider a bug, 
and tools make us of these declarations); but concerns have been expressed about the potential interference with our primary goal: 
improving safety. Every useful program in practice does contain bugs, and often programs can behave reasonably 
well even in the face of these bugs. Allowing contract-based optimizations may turn programs that behave reasonably
in the face of bugs into programs that behave unreasonably in the face of bugs. Wherever it can be demonstrated that 
any secondary goals are in a direct and an irreconcilable conflict with out prmary goal, we commit to resolving 
any such conflict in favor of our primary goal: improving safety.
