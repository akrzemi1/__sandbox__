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

The graph representation that is most commonly used in this library is [_adjacency list_](https://en.wikipedia.org/wiki/Adjacency_list). 
Very conceptually, we call it a random access range (corresponding to vertices) of forward ranges (corresponding to outgoing edges of a vertex).

This representation allows the algorithms to:

 1. Perform an iteration over vertices, and to count them.
 2. For each vertex, perform an iteration over its outgoing edges.
 3. For each edge to to look up its target vertex in constant time.

Algorithms in this library express the requirements on the adjacency list representations via concept `graph::index_adjacency_list`.
This concept expresses its syntactic requirments mostly via [_customization points_](./customization_points.md).
We use the following notation to represent the constraints:

| Symbol | Meaning                                                 |
|--------|---------------------------------------------------------|
| `G`    | the type of the graph representation                    |
| `g`    | lvalue or lvalue reference of type `G`                  |
| `u`    | lvalue reference of type `graph::vertex_reference_t<G>` |
| `ui`   | value of type `graph::vertex_iterator_t<G>`             |
| `uid`  | value of type `graph::vertex_id_t<G>`                   |
| `uv`   | lvalue reference of type `graph::edge_reference_t<G>`   |

### Random access to vertices

Customization point `graph::vertices(g)` must be valid and its return type must satisfy type-requirements `std::ranges::sized_range` and `std::ranges::random_access_range`. 
The algorithms will use it to access the vertices of graph represented by `g` in form of a random-access range. 

Customization point `graph::vertex_id(g, ui)` must be valid and its return type must satisfy type-requirements `std::integral`.
The algorithms will use this function to convert the iterator pointing to a vertex to the _id_ of the vertex.


### Forward access to target edges

The following customization points must be valid and their return type shall satisfy type requirements `std::ranges::forward_range`:

 * `graph::edges(g, uid)`,
 * `graph::edges(g, u)`.

The algorithms will use this function to iterate over out edges of the vertex represended by either `uid` or `u`.


### Linking from target edges back to vertices

Customization point `graph::target_id(g, uv)` must be valid and its return type must satisfy type-requirements `std::integral`.
The algorithms will use this value to access a vertex in `graph::vertices(g)`. 
Therefore we have a _semantic_ constraint: that the look up of the value returned from `graph::target_id(g, uv)` returns value `uid` that satisfies the condition
`0 <= uid && uid < graph::num_vertices(g)`.


------

TODO:

- Adapting
- use our container
- views (for your own algos)
- algorithms
