# Overview

This is a _generic_ library. Graph algorithms operate on various graph representations through a well defined interface: a concept. The primary concept in this library is `index_adjacency_list`.

```c++
#include <graph/graph.hpp>  // (1)

static_assert(graph::index_adjacency_list<MyType>);  // (2) (3)
```
Notes:
 1. Graph concepts are defined in header `<graph/graph.hpp>`.
 2. All declarations reside in namespace `::graph`.
 3. Use concept `graph::index_adjacency_list` to test if your type satisfies the syntactic requirements of an index adjacency list.

- CPOs
- Graph concept
- Adapting
- use our container
- views (for your own algos)
- algorithms
