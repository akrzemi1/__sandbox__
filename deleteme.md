I learned the hard way how to add a rule parser for the purpose of adjusting the type and value of the parser attribute. I have a number of recommendations for making this experience easier for the newcommers.

**Observation 1.** When one defines a rule parser for the purpose of adjusting the type and the value of the attribute, unless a simple imlicit conversion does the job, one must immediately employ a semantic action and `_attr(ctx)` and `_val(ctx)`.

**Recommendation 1.** Add a section describing an end-to-end experience of defining a rule parser for the purpose of changing the type or value of the attribute, that would involve:

 * using a semantic action,
 * using `_attr()` and `_val()`.

**Recommendation 2.** Rename `_val()` to `_rule_attr()`. Name `_val` is misleading and uninformative. I understand that we have two attributes in sight -- one from the defined rule and the other from the present parser -- and we need to tell them apart. While the docs at [The Parse Context](https://tzlaine.github.io/parser/doc/html/boost_parser__proposed_/tutorial/the_parse_context.html) technically describe it, the difference between `_attr()` and `_val()` is impossible to grasp for a newcommer, overwhelmed with the amount of material.

**Recommendation 3.** Cahnge or remove the example in [Semantic Actions](https://tzlaine.github.io/parser/doc/html/boost_parser__proposed_/tutorial/semantic_actions.html). Its semantic action assigns to a global. In a robust parser you will not do this. Instead, you will use the context. The tutorial should not endorse bad practices.

**Observation 2.** In my first encounters with semantic actions, I tried to put my lambda expressions directly into the semantic actions:

```c++
const bp::rule<struct value, Value> value = "value";
const auto value_def = bp::int_[ [](auto & ctx){ _val(ctx) = {_attr(ctx)}; } ];
```

This resulted in a compiler error, because two consecutive `[` (even if separated by white space) can only introduce an attribute. I think this deserves a note in the docs.

**Observation 3.** The docs make an incorrect claim in [Directives](https://tzlaine.github.io/parser/doc/html/boost_parser__proposed_/tutorial/directives.html), when it says,

> Lexically, you can spot a directive by its use of `[]`. Non-directives never use `[]`, and directives always do.

Semantic actions also use `[]`, so when you see `[]` you do not immediately know if you are looking at a directive or a parser with a semantic action.
