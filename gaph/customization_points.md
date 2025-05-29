# Customization Points

Most of the graph interface is defined in terms of _Customization Point Objects_. 
An arbitrary graph representation can be adapted to model this library's graph concepts via customizing the library's customization point objects (CPO).

Each customization point can be customized in a number of ways. For each customization point we provide the list of  ways in which it can be customized. The order in this list matters: the customizations are matched in order and if a given customization is determined to be valid, the subsequent ways of customizing, even if they would be valid, are ignored.

Often, the customization method specified at the end of the list, serves the purpose of a "fallback" or "default" customization.

If none of the customization ways is valid for a given type, or set of types, the customization point is considered _valid_ for this set of types.


## The list of customization points

### `vertices`

`vertices(g)` is used to obtain the list of all vertices, in form of a `std::ranges::random_access_range`, from the graph-representing object `g`.
We also use its return type to determine the type of the vertex: `vertex_t<G>`.

#### Customization

 1. `g.vertices()`, if such member function exists and returns a `std::copyable` type.
 2. `vertices(g)` if such function is ADL-discoverable and returns a `std::copyable` type.
 3. `g` if it is a `std::ranges::random_access_range`.


### `vertex_id(g, ui)`

Given a graph-representing object `g` and an object of type `vertex_iterator_t<G>` representing the iterator to a vertex in `g`, it is used obtain the _id_ of the vertex.
We also use its return type to determine the type of the vertex id: `vertex_id_t<G>`.

#### Coustomization

TBD...

### `find_vertex(g, uid)`

### `edges(g, u)`

### `edges(g, uid)`

### `num_edges(g)`

### `target_id(g, uv)`

### `target_id(e)`

### `source_id(g, uv)`

### `source_id(e)`

### `target(g, uv)`

### `source(g, uv)`

### `find_vertex_edge(g, u, vid)`

### `find_vertex_edge(g, uid, vid)`

### `contains_edge(g, uid, vid)`

### `partition_id(g, u)`

### `partition_id(g, uid)`

### `num_vertices(g, pid)`

### `num_vertices(g)`

### `degree(g, u)`

### `degree(g, uid)`

### `vertex_value(g, u)`

### `edge_value(g, uv)`

### `edge_value(e)`

### `graph_value(g)`

### `num_partitions(g)`

### `has_edge(g)`

