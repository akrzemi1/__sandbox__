#include <cassert>
#include <vector>
#include <ranges>
#include <stack>
#include <iostream>
#include <unordered_map>


namespace graph {
    namespace cpo_vertices {
        template <class G>
        concept has_member = requires(G&& g) {
            { g.vertices() } -> std::move_constructible;
        };
                            
        template <class G>
        concept has_adl = requires(G&& g) {
            { vertices(g) } -> std::move_constructible;
        };
        
        class cpo
        {
        public:
            template <class G>
                requires has_member<G> || has_adl<G>
            constexpr decltype(auto) operator()(G&& g) const {
                if constexpr (has_member<G>)
                    return g.vertices();
                else if constexpr (has_adl<G>)
                    return vertices(g);
                else 
                    static_assert(false);
            }
        };
    }
    
    inline namespace cpo {
        cpo_vertices::cpo vertices;
    }
    
    template <typename G>
    using vertex_nav_range_t = decltype(vertices(std::declval<G&&>()));
    
    template <typename G>
    using vertex_nav_ref_t = std::ranges::range_reference_t<vertex_nav_range_t<G>>;
    
    
    namespace cpo_vertex_id {
        template <class G>
        concept has_member = requires(G&& g, vertex_nav_ref_t<G> u) {
            { g.vertex_id(u) } -> std::copyable;
        };
                            
        template <class G>
        concept has_adl = requires(G&& g, vertex_nav_ref_t<G> u) {
            { vertex_id(g, u) } -> std::copyable;
        };
        
        class cpo
        {
        public:
            template <class G>
                requires has_member<G> || has_adl<G>
            constexpr auto operator()(G&& g, vertex_nav_ref_t<G> u) const {
                if constexpr (has_member<G>)
                    return g.vertex_id(u);
                else if constexpr (has_adl<G>)
                    return vertex_id(g, u);
                else if constexpr (std::ranges::contiguous_range<vertex_nav_range_t<G>>)
                    return (std::addressof(u) - std::ranges::begin(vertices(g)));
                else 
                    static_assert(false);
            }
        };
    }
    
    inline namespace cpo {
        cpo_vertex_id::cpo vertex_id;
    }
    
    template <typename G>
    using vertex_id_t = decltype(vertex_id(std::declval<G&&>(), std::declval<vertex_nav_ref_t<G>>()));
    

    //---
    namespace cpo_vertex {
        template <class G>
        concept has_member = requires(G&& g, vertex_id_t<G> const& uid) {
            { g.vertex(uid) } -> std::move_constructible;
        };
                            
        template <class G>
        concept has_adl = requires(G&& g, vertex_id_t<G> const& uid) {
            { vertex(g, uid) } -> std::move_constructible;
        };
        
        class cpo
        {
        public:
            template <class G>
                requires has_member<G> || has_adl<G> || std::ranges::contiguous_range<vertex_nav_range_t<G>>
            constexpr decltype(auto) operator()(G&& g, vertex_id_t<G> const& uid) const {
                if constexpr (has_member<G>)
                    return g.vertex(uid);
                else if constexpr (has_adl<G>)
                    return vertex(g, uid);
                else if constexpr (std::ranges::contiguous_range<vertex_nav_range_t<G>>)
                    return *(std::ranges::begin(vertices(g)) + uid);
                else 
                    static_assert(false);
            }
        };
    }
    
    inline namespace cpo {
        cpo_vertex::cpo vertex;
    }
    
    //---
    namespace cpo_edges {
        template <class G>
        concept has_member = requires(G&& g, vertex_nav_ref_t<G> u) {
            { g.edges(u) } -> std::move_constructible;
        };
        
        template <class G>
        concept has_inv_member = requires(G&& g, vertex_nav_ref_t<G> u) {
            { u.edges(g) } -> std::move_constructible;
        };
                            
        template <class G>
        concept has_adl = requires(G&& g, vertex_nav_ref_t<G> u) {
            { edges(g, u) } -> std::move_constructible;
        };
        
        class cpo
        {
        public:
            template <class G>
                requires has_member<G> || has_inv_member<G> || has_adl<G>
            constexpr auto operator()(G&& g, vertex_nav_ref_t<G> u) const {
                if constexpr (has_member<G>)
                    return g.edges(u);
                else if constexpr (has_inv_member<G>)
                    return u.edges(g);
                else if constexpr (has_adl<G>)
                    return edges(g, u);
                else 
                    static_assert(false);
            }
        };
    }
    
    inline namespace cpo {
        cpo_edges::cpo edges;
    }
    
    template <typename G>
    using edge_nav_range_t = decltype(edges(std::declval<G&&>(), std::declval<vertex_nav_ref_t<G>>()));
    
    template <typename G>
    using edge_nav_ref_t = std::ranges::range_reference_t<edge_nav_range_t<G>>;
    
    
    //---
    namespace cpo_target {
        template <class G>
        concept has_member = requires(G&& g, edge_nav_ref_t<G> uv) {
            { g.target(uv) } -> std::move_constructible;
        };
                            
        template <class G>
        concept has_adl = requires(G&& g, edge_nav_ref_t<G> uv) {
            { target(g, uv) } -> std::move_constructible;
        };
        
        class cpo
        {
        public:
            template <class G>
                requires has_member<G> || has_adl<G>
            constexpr decltype(auto) operator()(G&& g, edge_nav_ref_t<G> uv) const {
                if constexpr (has_member<G>)
                    return g.target(uv);
                else if constexpr (has_adl<G>)
                    return target(g, uv);
                else 
                    static_assert(false);
            }
        };
    }
    
    inline namespace cpo {
        cpo_target::cpo target;
    }
    
    //---
    
    template <typename T> 
    class type {};        // hack to encode a type in an object
    
    //---
    namespace cpo_registry {
        template <class G, class T>
        concept has_member = requires(G&& g, type<T> t) {
            { g.registry(t) } -> std::move_constructible;
        };
        
        template <class G, class T>
        concept has_adl = requires(G&& g, type<T> t) {
            { registry(g, t) } -> std::move_constructible;
        };
                            
        template <class G, class /*T*/>
        concept has_native_ra =         
            std::ranges::random_access_range<vertex_nav_range_t<G>> &&
            std::integral<vertex_id_t<G>>;
        
        
        class cpo
        {
        public:
            template <class G, class T>
                /*requires has_member<G, T> || 
                         has_native_ra<G, T> || 
                         has_adl<G, T>*/
            constexpr auto operator()(G&& g, type<T> t) const {
                if constexpr (has_member<G, T>)
                    return g.registry(t);
                else if constexpr (has_adl<G, T>)
                    return registry(g, t);
                else if constexpr (has_native_ra<G, T>)
                    return std::vector<T>(std::ranges::size(vertices(g)));
                else 
                    return std::unordered_map<vertex_id_t<G>, T>();
            }
        };
    }
    
    inline namespace cpo {
        cpo_registry::cpo registry;
    }
    
    //---
    //===
    template <typename T>
    concept reference = std::is_reference_v<T>;
    
    template <typename G>
    concept adjacency_list = requires (G&& g) {
        { vertices(g) } -> std::ranges::forward_range;
    } 
    && requires (G&& g, vertex_nav_ref_t<G&&> u) {
        { vertex_id(g, u) } -> std::copyable;
        { edges(g, u) } -> std::ranges::forward_range;
    }
    && requires (G&& g, vertex_id_t<G&&> const& uid) {
        { vertex(g, uid) } -> reference;
    }
    && requires (G&& g, edge_nav_ref_t<G&&> uv) {
        { target(g, uv) } -> std::same_as<vertex_nav_ref_t<G&&>>;
    };
    
    namespace archetype { // TBD
        
        template <typename T>
        class iter
        {
        public:
            iter& operator++() { return *this; }
            iter operator++(int) { return *this; }
            using difference_type = int;
            T& operator*() const { throw 0; }
            friend bool operator==(iter const&, iter const&) = default;
            using value_type = T;
            using reference = T&;
            using iterator_category = std::forward_iterator_tag;
        };

        struct id
        {
            id() = delete;
            friend bool operator==(id const&, id const&) = default;
        };
        
        struct val
        {
            val(val&&) = delete;
        };
        
        template <typename T> 
        class fwr
        {
            friend iter<T> begin(fwr&) { throw 0; }
            friend iter<T> end(fwr&) { throw 0; }
        };
        
        class edge_nav
        {
            edge_nav(edge_nav&&) = delete;
            ~edge_nav() = delete;
        };
        
        class vertex_nav
        {
            vertex_nav(vertex_nav&&) = delete;
            ~vertex_nav() = delete;
        };
        
        class adjacency_list_at
        {
            adjacency_list_at() = delete;
            ~adjacency_list_at() = delete;
            adjacency_list_at(adjacency_list_at&&) = delete;
        };
        
        fwr<vertex_nav> vertices(adjacency_list_at&) { throw 0; }
        fwr<edge_nav> edges(adjacency_list_at&, vertex_nav&) { throw 0; }
        id vertex_id(adjacency_list_at&, vertex_nav&) { throw 0; }
        vertex_nav& vertex(adjacency_list_at&, const id&) { throw 0; }
        vertex_nav& target(adjacency_list_at&, const edge_nav&) { throw 0; }
        
        struct Hash { std::size_t operator()(const id&) const { return 0; } };
        
        template <typename T>
        std::unordered_map<id, T, Hash> registry(adjacency_list_at&, type<T>) { return {}; }
    }
     
    static_assert(adjacency_list<archetype::adjacency_list_at>);
}

namespace graph {

template <adjacency_list G,
          std::invocable<vertex_id_t<G>> VF, 
          std::invocable<edge_nav_ref_t<G>> EWF,
          std::invocable<std::invoke_result_t<EWF, edge_nav_ref_t<G>>> CF>
void dfs(G && g, vertex_id_t<G> source, VF vf, EWF ewf, CF cf)
{
    auto _visited = registry(g, type<bool>{});
    std::stack<vertex_id_t<G>> stack;
    stack.push(std::move(source));
    
    while (!stack.empty()) {
        vertex_id_t<G> uid = stack.top();
        stack.pop();
        
        _visited[uid] = true;
        vf(uid);
        
        auto&& edgs = edges(g, vertex(g, uid));
        for (auto&& uv : edgs) {
            auto vid = vertex_id(g, target(g, uv));
            
            if (!_visited[vid]) {
                cf(ewf(uv));
                _visited[vid] = true;
                stack.push(vid);
            }
        }
    }
}


void test_dfs(archetype::adjacency_list_at& g, archetype::id uid)
{
    auto vf = [](archetype::id const&){};
    auto ewf = [](archetype::edge_nav&) -> archetype::val { throw 0; };
    auto cf = [](archetype::val const& ) {};
    
    dfs(g, uid, vf, ewf, cf);
}

}

namespace graph {

template <typename T, typename Tag>
struct tagged
{
    T value;
};

class csr
{  
    using vertex_nav_t = tagged<int, struct V_>;
    using edge_nav_t = tagged<int, struct E_>;
    using vertes_descriptor_t = int;
    using edge_descriptor_t = int;
    using vertex_range = std::ranges::subrange<std::ranges::iterator_t<std::vector<vertex_nav_t>>>;
    using edge_range = std::ranges::subrange<std::ranges::iterator_t<std::vector<edge_nav_t>>>;

public:    
    std::vector<vertex_nav_t> _edge_ranges = std::vector<vertex_nav_t> (1);
    std::vector<edge_nav_t> _edges;
    
public:
    int num_vertices() const { return _edge_ranges.size() - 1; }
    vertex_range vertices() { 
        return vertex_range(_edge_ranges.begin(), std::next(_edge_ranges.end(), -1)); 
    } 
    
    edge_range edges(const vertex_nav_t& u) { 
        auto uid = vertex_id(u);
        assert(uid < num_vertices()); 
        assert(_edge_ranges[uid].value <= _edge_ranges[uid + 1].value);
        auto ebegin = std::ranges::begin(_edges);
        return edge_range(std::next(ebegin, _edge_ranges[uid].value), 
                          std::next(ebegin, _edge_ranges[uid + 1].value)); 
    }
    
    vertex_nav_t& target(const edge_nav_t& uv) { return _edge_ranges[uv.value]; }
    
    vertes_descriptor_t vertex_id(const vertex_nav_t& u) { return &u - _edge_ranges.data(); } 
    vertex_nav_t& vertex(vertes_descriptor_t const& uid ) { return _edge_ranges[uid]; }
    edge_descriptor_t edge_id(const edge_nav_t& uv) { return &uv - _edges.data(); }
    double weight(const edge_nav_t& uv) { return 1.0; }
};

} // namespace graph



namespace MyLibrary 
{
    
struct MyEdge
{
  std::string content;
  int indexOfTarget;
  
  //int target_id(auto&&) const { return indexOfTarget; }
};

struct MyVertex
{
  std::string content;
  std::vector<MyEdge> outEdges;
  
  std::vector<MyEdge> /*const*/ & edges(auto&&) /*const*/ { return outEdges; }
  //std::vector<MyEdge>& edges(auto&&) { return outEdges; }
  //double vertex_value(auto const&) const { return 1.0; } 
};

class MyGraph 
{
public:
  std::vector<MyVertex> _vertices;
  
public:
  std::vector<MyVertex> /*const*/ & vertices() /*const*/  { return _vertices; }
  
    // AK: not needed?
  MyVertex & target(MyEdge /*const*/ & uv)  {
    return _vertices[uv.indexOfTarget];
  }
  
  int vertex_id(MyVertex /*const*/ & u) /*const*/  {
      return std::distance(_vertices.data(), &u);
  }
};

//std::vector<MyEdge> const& edges(MyGraph const& g, MyVertex const& u) { return u.edges(g); }
}

namespace string_id {
    
struct Edge
{
  std::string payload;
  std::string idOfTarget;
};

struct Vertex
{
  std::string payload;
  std::vector<Edge> outEdges;
  
  std::vector<Edge> const& edges(auto&&) const { return outEdges; }
};

class Graph 
{
public:
    using Container = std::unordered_map<std::string, Vertex>;
    using VertexNav = std::unordered_map<std::string, Vertex>::const_reference;

  Container _vertices;
  
public:
  Container const& vertices() const { return _vertices; }
  
  friend std::vector<Edge> const& edges(Graph const& g, VertexNav u)
  { return u.second.outEdges; }
  
  VertexNav vertex(std::string const& uid) const {
    auto it = _vertices.find(uid);
    assert(it != _vertices.end());
    return *it;
  }
  
  VertexNav target(Edge const& uv) const {
    return vertex(uv.idOfTarget);
  }
  
  std::string vertex_id(VertexNav u) const  {
    return u.first;
  }
  
};

} // namespace string_id




 // (0) <-> (1)
 //  |       |
 // (2) <-> (3)
 // 0 1 2
int main()
{
    auto print_uid = [](auto const& uid){ std::cout << uid << " "; };
    auto print_val = [](double val) { std::cout << " (w" << val << ") "; };
    
    {
        MyLibrary::MyGraph g;
        g._vertices = {
            {"0", {{"1", 1}, {"2", 2}}},
            {"1", {{"0", 0}, {"3", 3}}},
            {"2", {{"0", 0}, {"3", 3}}},
            {"3", {{"1", 1}, {"2", 2}}}
        };
        
        graph::dfs(g, 
            1, 
            print_uid,
            [&g](auto& edge) { return 1.0; },
            print_val);
        std::cout << "\n";
        
    }
    {
        using namespace std::string_literals;
        string_id::Graph g;
        g._vertices = string_id::Graph::Container{
            {"i0"s, string_id::Vertex{"pld",{{"pld", "i1"s}, {"pld", "i2"s}}}},
            {"i1"s, string_id::Vertex{"pld",{{"pld", "i0"s}, {"pld", "i3"s}}}},
            {"i2"s, string_id::Vertex{"pld",{{"pld", "i0"s}, {"pld", "i3"s}}}},
            {"i3"s, string_id::Vertex{"pld",{{"pld", "i1"s}, {"pld", "i2"s}}}}
        };
        
        graph::dfs(g, 
            "i1"s, 
            print_uid,
            [&g](auto& edge) { return 1.0; },
            print_val);
        std::cout << "\n";
    }
    
    graph::csr g;
    g._edge_ranges = {{0}, {2}, {4}, {6}, {8}};
    g._edges = {{1}, {2}, {0}, {3}, {0}, {3}, {1}, {2}}; 
    
    static_assert(graph::adjacency_list<graph::csr>);
    static_assert(graph::adjacency_list<MyLibrary::MyGraph>);
    static_assert(graph::adjacency_list<string_id::Graph>);


        
    dfs(g, 
        1, 
        print_uid,
        [&g](auto& edge) { return g.weight(edge); },
        print_val);
    std::cout << "\n";
    
    
}
