Combining a path's _distance_ with an edge's _weight_ requires four parameters to be provided by the user, 
which together abide by certain rules. In the case of Dijkstra's Shortest Paths algorithm, the distinguishing
rule is that combining a distance with a new weight must never improve the distance.

These four elements expressed as a C++20 concept with additional `axiom` notation for expressing semantic constraints:

```c++
template <typename Distance, typename Weight, typename Combine, typename Compare>
concept distance_computation = requires(Distance d, Weight w, Combine combine, Compare better)
{
  requires semiregular<Distance>; 
  requires strict_waeak_order<Compare, Distance>;
  requires regular_invocable<Combine, Distance, Weight>; 
  axion better(combine(d, w), d) == false;
  axiom better(d, zero<Distance>) == false;
  axiom better(inf<Distance>, d) == false;
}; 
```


By the "maximum bandwidth path" problem I mean finding path between two indicated vertices that maximizes the minimum weight that can be found on the path edges.

This problem can be solved by Dijkstra's Shortest Paths algorigh if we provide:
 1. `min` for `Combine` funciton,
 2. `greater` for `Compare` funciton
 3. `0` for the maximum possible weight,
 4. `numeric_limits<T>::max()` for the minimum possible weight.

This [Compiler Explorer example](https://godbolt.org/z/fTvrzqTPe) shows how this can be done with Boost.Graph. 

This cannot be done with graph-v3, though, because it only allows customizing the first two (functions) but not the last two (special values). I cannot easily suggest a fix as it affects the design questions: how many arguments to an algorithm can you afford to have?

Also these forur values need to make sense together. They together form a concept. A concept with semantic constriants:

* `compare(w, min_weight) == false` -- the most desireable value
* `compare(max_weight, w) == false` -- the least desireable value
* `compare(combine(w, v), w) == false` -- combining a value can only worsen it or leave the same
* `compare(w, combine(w, min_weight)) == false` -- combining with the most desireable value doesn't change the result
* `compare(max_weight, combine(max_weight, w)) == false` -- combining with the least desireable value produces the least desireable result

One option is to have these four travel together as one aggregate parameter.
