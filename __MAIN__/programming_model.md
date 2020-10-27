## The Programming Model

The contract support facility that we propose enables programmers to declare in the source code that certain values of
objects observed at certain times necessarily indicate a bug somewhere in the program. Implementations —
either compilers or other automated tools, such as static analyzers — can then make use of these declarations 
in order to detect bugs in the programs — either at run-time or at static analysis time — more effectively.

The intended work-flow is that a programmer puts contract annotations (preconditions, postconditions, etc.) 
in their code, and then employs some tool that will try to detect bugs based on these annotations. For instance,
the user may instruct the compiler to inject runtime checks in the place of contract annotations that will evaluate 
if undesired values are present, and, if so, log a warning message. Because the injection of runtime checks is one
of possible uses of contract annotations, programmers need to accept all the consequences of the potential evaluation 
of their predicates, such as runtime impact or bugs inside the predicates.

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
