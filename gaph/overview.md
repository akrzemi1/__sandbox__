# Overview

What this library has to offer:

 1. A number of ready-to-use algoritms, like Dijkstra's shortest paths computation.
 2. Make it easier for you to write your own graph algorithms: we provide a _view_ for traversing your graph in the preferred order (depth-first, breadth-first, topological), and you specify what is processed in each traversal step.
 3. Customization of your graph representation, so that it can be used with our algorithms and views.
 4. Our own container for representing a graph.  

## Concepts

This is a _generic_ library. Graph algorithms operate on various graph representations through a well defined interface: a concept. The primary concept in this library is `index_adjacency_list`.

```c++
#include <graph/graph.hpp>  // (1)

static_assert(graph::index_adjacency_list<MyType>);  // (2) (3)
```
Notes:
 1. Graph concepts are defined in header `<graph/graph.hpp>`.
 2. All declarations reside in namespace `::graph`.
 3. Use concept `graph::index_adjacency_list` to test if your type satisfies the syntactic requirements of an index adjacency list.

Concept `graph::index_adjacency_list` requires a number of things from a graph representation. We use the following notation to represent the constraints:

| Symbol | Meaning                                       |
|--------|-----------------------------------------------|
| `G`    | the type of the graph representation          |
| `g`    | lvalue or lvalue reference of type `G`        |
| `u`    | value of type `graph::vertex_reference_t<G>`  |
| `ui`   | value of type `graph::vertex_iterator_t<G>`   |
| `uid`  | value of type `graph::vertex_id_t<G>`         |
| `uv`   | variable of type `graph::edge_reference_t<G>` |

### Random access to vertices

Expression `graph::vertices(g)` must be valid and return an object of type satisfying type-requirements `std::ranges::sized_range` and `std::ranges::random_access_range`. We will use this return type to define two type aliases:

   * `graph::vertex_range_t<G>` as `decltype(graph::vertices(g))`,
   * `graph::vertex_iterator_t<G>` as `std::ranges::iterator_t<graph::vertex_range_t<G>>`,
   * `graph::vertex_t<G>` as `std::ranges::range_value_t<graph::vertex_range_t<G>>`,
   * `graph::vertex_reference_t<G>` as `std::ranges::range_reference_t<graph::vertex_range_t<G>>`.

`graph::vertices` is a _Customization Point Object_ (CPO). 
For more information about customization points in this library and how to make them valid, see 
[Customization Points](./customization_points.md).

The algorithms will use function `graph::vertices(g)` to access the vertices of graph represented by `g` in form of a random-access range. 

Another CPO related to vertices needs to be valid: `graph::vertex_id(g, ui)`. We will use it to define type alias:

 * `graph::vertex_id_t<G>` as `decltype(graph::vertex_id(g, ui))`.

The algorithms will use this function to convert the iterator pointing to a vertex to the _id_ object identidying the same vertex.


### Forward access to target edges

The following customization points need to be valid and their return value shall satisfy type requirements `std::ranges::forward_range`:

 * `graph::edges(g, uid)`,
 * `graph::edges(g, u)`.

We will use the latter to define edge-related type aliases:

 * `graph::vertex_edge_range_t<G>` as `decltype(edges(g, u))`,
 * `graph::vertex_edge_iterator_t<G>` as `std::ranges::iterator_t<vertex_edge_range_t<G>>`,
 * `graph::edge_t<G>` as `std::ranges::range_value_t<graph::vertex_edge_range_t<G>>`,
 * `graph::edge_reference_t<G>` as `std::ranges::range_reference_t<graph::vertex_edge_range_t<G>>`.

The algorithms will use this function to access edges corresponding to the vertex represended by either `uid` or `u`, as a forward range.

### Linking from target edges back to vertices

The following customization point has to be valid:

 * `graph::target_id(g, uv)`

Its values will be used to perform the look-up back in `graph::vertices(g)`. 
Therefore we have a _semantic_ constraints that the look up of the value returned from `graph::target_id(g, uv)` returns value other than `std::ranges::end(graph::vertices(g))`.

AK: Why don't we require the type `vertex_id_t<G>` to be `std::copyable`?

------

TODO:

- Adapting
- use our container
- views (for your own algos)
- algorithms
