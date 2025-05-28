# Customization Points

Most of the graph interface is defined in terms of _Customization Point Objects_. 
An arbitrary graph representation can be adapted to model this library's graph concepts via customizing the library's customization point objects (CPO).


## The list of customization points

### `vertices`

`vertices(g)` is used to obtain the list of all vertices, in form of a `std::ranges::random_access_range`, from the  graph-representing object `g`.
It is defined as follows:

 1. `g.vertices()` if such member function exists and returns a `std::copyable` type.
 2. `vertices(g)` if such function is ADL-discoverable and returns a `std::copyable` type.
 3. `g` if it is a `std::ranges::random_access_range`.

If `g` satisfies more than one criteria from the list above, the first one, in the order presented above, is chosen.
If `g` satisfies none of the above criteria, the CPO `vertices(g)` is considered invald for `g`.
