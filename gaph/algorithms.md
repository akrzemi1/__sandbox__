# Algorithms

A graph path _p_ is a possibly empty sequence of graph edges (_e_<sub>0</sub>, _e_<sub>1</sub>, ..., _e_<sub>_N_</sub>) where:
  * _e_<sub>_i_</sub> ≠ _e_<sub>_j_</sub> for _i_ ≠ _j_,
  * target(_e_<sub>_i_</sub>) = source(_e_<sub>_i_+1</sub>),
  * source(_e_<sub>_i_</sub>) != source(_e_<sub>_j_</sub>) for _i_ ≠ _j_.

<code><i>path-source</i>(<i>p</i>)</code> = source(_e_<sub>0</sub>). <code><i>path-target</i>(<i>p</i>)</code> = target(_e_<sub>_N_</sub>).

<code><i>distance(p)</i></code> is a sum over _i_ of <code>weight</code>(_e_<sub>_i_</sub>).

<code><i>shortest-path</i>(g, u, v)</code> is a path in the set of all paths `p` in graph `g` with <code><i>path-source</i>(<i>p</i>)</code> = `u` 
and <code><i>path-target</i>(<i>p</i>)</code> = v that has the smallest value of <code><i>distance(p)</i></code>.

<code><i>shortest-path-distance</i>(g, u, v)</code> is <code><i>distance</i>(<i>shortest-path</i>(g, u, v))</code> if it exists and _infinite-distance_ otherwise.

<code><i>shortest-path-predecessor</i>(g, u, v)</code>, in the set of all shortest paths <code><i>shortest-path</i>(g, u, v)</code> for any `v`:
 * if there exists an edge _e_ with target(_e_) = v, then it is source(_e_),
 * otherwise it is `v`.
   

## `dijkstra_shortest_paths` (single source)

```c++
template <index_adjacency_list G,
          random_access_range  Distances,
          random_access_range  Predecessors,
          class WF      = function<range_value_t<Distances>(edge_reference_t<G>)>,
          class Visitor = empty_visitor,
          class Compare = less<range_value_t<Distances>>,
          class Combine = plus<range_value_t<Distances>>>
requires is_arithmetic_v<range_value_t<Distances>> && //
         sized_range<Distances> &&                    //
         sized_range<Predecessors> &&                 //
         convertible_to<vertex_id_t<G>, range_value_t<Predecessors>> &&
         basic_edge_weight_function<G, WF, range_value_t<Distances>, Compare, Combine>
constexpr void dijkstra_shortest_distances(
      G&&            g,
      vertex_id_t<G> source,
      Distances&     distances,
      Predecessors&  predecessor,
      WF&&      weight  = [](edge_reference_t<G> uv) { return range_value_t<Distances>(1); }, // default weight(uv) -> 1
      Visitor&& visitor = empty_visitor(),
      Compare&& compare = less<range_value_t<Distances>>(),
      Combine&& combine = plus<range_value_t<Distances>>());
```
*Constraints:*
  * `index_adjacency_list<G>` is `true`,
  * `std::ranges::random_access_range<Distances>` is `true`,
  * `std::ranges::sized_range<Distances>` is `true`,
  * `std::ranges::random_access_range<Predecessors>` is `true`,
  * `std::ranges::sized_range<Predecessors>` is `true`,
  * `std::is_arithmetic_v<std::ranges::range_value_t<Distances>>` is `true`,
  * `std::convertible_to<vertex_id_t<G>, std::ranges::range_value_t<Predecessors>>` is `true`,
  * `basic_edge_weight_function<G, WF, std::ranges::range_value_t<Distances>, Compare, Combine>` is `true`.

*Hardened preconditions:* 
  * `0 <= source && source < num_vertices(g)` is `true`,
  * `std::size(distances) >= num_vertices(g)` is `true`,
  * `std::size(predecessor) >= num_vertices(g)` is `true`.

*Preconditions:* 
  * <code>distances[<i>i</i>] == shortest_path_infinite_distance&lt;range_value_t&lt;Distances&gt;&gt;()</code> for each <code><i>i</i></code> in range [`0`; `num_vertices(g)`),
  * <code>predecessor[<i>i</i>] == <i>i</i></code> for each <code><i>i</i></code> in range [`0`; `num_vertices(g)`),
  * `weight` returns non-negative values.

*Postconditions:* For each <code><i>i</i></code> in range [`0`; `num_vertices(g)`):
  * <code>distances[<i>i</i>]</code> is <code><i>shortest-path-distance</i>(g, source, <i>i</i>)</code>,
  * <code>predecessor[<i>i</i>]</code> is <code><i>shortest-path-predecessor</i>(g, source, <i>i</i>)</code>.  
