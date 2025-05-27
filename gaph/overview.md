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

| Symbol | Meaning                                |
|--------|----------------------------------------|
| `G`    | the type of the graph representation   |
| `g`    | lvalue or lvalue reference of type `G` |

### Random access to vertices

Expression `graph::vertices(g)` must be valid and return an object of type satisfying type-requirements `std::ranges::sized_range` and `std::ranges::random_access_range`. We will use this return type to define two type aliases:

   * `graph::vertex_range_t<G>` as `decltype(graph::vertices(g))`,
   * `graph::vertex_iterator_t<G>` as `std::ranges::iterator_t<graph::vertex_range_t<G>>`,
   * `graph::vertex_t<G>` as `std::ranges::range_value_t<graph::vertex_range_t<G>>`,
   * `graph::vertex_reference_t<G>` as `std::ranges::range_reference_t<graph::vertex_range_t<G>>`.

`graph::vertices` is a _Customization Point Object_ (CPO). In order to make it valid for your type `G` you have to do one of the following:

 * provide member function `g.vertices()`,
 * provide an ADL-discoverable free function `vertices(g)`,
 * make `G` a `std::ranges::random_access_range`.


------

- CPOs
- Graph concept
- Adapting
- use our container
- views (for your own algos)
- algorithms
