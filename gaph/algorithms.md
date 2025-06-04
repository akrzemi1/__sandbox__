# Algorithms

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

*Hardened preconditions:* 
  * `0 <= source && source < num_vertices(g)` is `true`,
  * `std::size(distances) >= num_vertices(g)` is `true`,
  * `std::size(predecessor) >= num_vertices(g)` is `true`.

*Preconditions:* 
  * <code>distances[<i>i</i>] == shortest_path_infinite_distance()</code> for each <code><i>i</i></code> in range [`0`; `num_vertices(g)`),
  * <code>predecessors[<i>i</i>] == <i>i</i></code> for each <code><i>i</i></code> in range [`0`; `num_vertices(g)`),
  * `weight` returns non-negative values.
