#line 22 "graph/graph.hpp"
#include <ranges>
#include <concepts>
#include <type_traits>
#include <stdexcept>
#line 3 "graph/graph_descriptors.hpp"
namespace std::graph {
#line 9 "graph/graph_descriptors.hpp"
template <class VId, class V, class VV>
struct vertex_descriptor {
  using id_type     = VId;
  using vertex_type = V;
  using value_type  = VV;

  id_type     id;
  vertex_type vertex;
  value_type  value;
};
template <class VId, class V>
struct vertex_descriptor<VId, V, void> {
  using id_type     = VId;
  using vertex_type = V;
  using value_type  = void;

  id_type     id;
  vertex_type vertex;
};
template <class VId, class VV>
struct vertex_descriptor<VId, void, VV> {
  using id_type     = VId;
  using vertex_type = void;
  using value_type  = VV;

  id_type    id;
  value_type value;
};
template <class VId>
struct vertex_descriptor<VId, void, void> {
  using id_type     = VId;
  using vertex_type = void;
  using value_type  = void;

  id_type id;
};

template <class VId, class VV>
using copyable_vertex_t = vertex_descriptor<VId, void, VV>;
#line 58 "graph/graph_descriptors.hpp"
template <class VId, bool Sourced, class E, class EV>
struct edge_descriptor {
  using source_id_type = VId;
  using target_id_type = VId;
  using edge_type      = E;
  using value_type     = EV;

  source_id_type source_id;
  target_id_type target_id;
  edge_type      edge;
  value_type     value;
};

template <class VId, class E>
struct edge_descriptor<VId, true, E, void> {
  using source_id_type = VId;
  using target_id_type = VId;
  using edge_type      = E;
  using value_type     = void;

  source_id_type source_id;
  target_id_type target_id;
  edge_type      edge;
};
template <class VId>
struct edge_descriptor<VId, true, void, void> {
  using source_id_type = VId;
  using target_id_type = VId;
  using edge_type      = void;
  using value_type     = void;

  source_id_type source_id;
  target_id_type target_id;
};
template <class VId, class EV>
struct edge_descriptor<VId, true, void, EV> {
  using source_id_type = VId;
  using target_id_type = VId;
  using edge_type      = void;
  using value_type     = EV;

  source_id_type source_id;
  target_id_type target_id;
  value_type     value;
};

template <class VId, class E, class EV>
struct edge_descriptor<VId, false, E, EV> {
  using source_id_type = void;
  using target_id_type = VId;
  using edge_type      = E;
  using value_type     = EV;

  target_id_type target_id;
  edge_type      edge;
  value_type     value;
};
template <class VId, class E>
struct edge_descriptor<VId, false, E, void> {
  using source_id_type = void;
  using target_id_type = VId;
  using edge_type      = E;
  using value_type     = void;

  target_id_type target_id;
  edge_type      edge;
};

template <class VId, class EV>
struct edge_descriptor<VId, false, void, EV> {
  using source_id_type = void;
  using target_id_type = VId;
  using edge_type      = void;
  using value_type     = EV;

  target_id_type target_id;
  value_type     value;
};
template <class VId>
struct edge_descriptor<VId, false, void, void> {
  using source_id_type = void;
  using target_id_type = VId;
  using edge_type      = void;
  using value_type     = void;

  target_id_type target_id;
};
#line 167 "graph/graph_descriptors.hpp"
template <class VId, class E, class EV>
using edgelist_edge = edge_descriptor<VId, true, E, EV>;
#line 173 "graph/graph_descriptors.hpp"
template <class VId, class EV>
using copyable_edge_t = edge_descriptor<VId, true, void, EV>;
#line 179 "graph/graph_descriptors.hpp"
template <class VId, bool Sourced, class V, class VV>
struct neighbor_descriptor {
  using source_id_type = VId;
  using target_id_type = VId;
  using vertex_type    = V;
  using value_type     = VV;

  source_id_type source_id;
  target_id_type target_id;
  vertex_type    target;
  value_type     value;
};

template <class VId, class V, class VV>
struct neighbor_descriptor<VId, false, V, VV> {
  using source_id_type = void;
  using target_id_type = VId;
  using vertex_type    = V;
  using value_type     = VV;

  target_id_type target_id;
  vertex_type    target;
  value_type     value;
};

template <class VId, class V>
struct neighbor_descriptor<VId, false, V, void> {
  using source_id_type = void;
  using target_id_type = VId;
  using vertex_type    = V;
  using value_type     = void;

  target_id_type target_id;
  vertex_type    target;
};

template <class VId, class VV>
struct neighbor_descriptor<VId, false, void, VV> {
  using source_id_type = void;
  using target_id_type = VId;
  using vertex_type    = void;
  using value_type     = VV;

  target_id_type target_id;
  value_type     value;
};

template <class VId>
struct neighbor_descriptor<VId, false, void, void> {
  using source_id_type = void;
  using target_id_type = VId;
  using vertex_type    = void;
  using value_type     = void;

  target_id_type target_id;
};

template <class VId, class V>
struct neighbor_descriptor<VId, true, V, void> {
  using source_id_type = VId;
  using target_id_type = VId;
  using vertex_type    = V;
  using value_type     = void;

  source_id_type source_id;
  target_id_type target_id;
  vertex_type    target;
};

template <class VId, class VV>
struct neighbor_descriptor<VId, true, void, VV> {
  using source_id_type = VId;
  using target_id_type = VId;
  using vertex_type    = void;
  using value_type     = VV;

  source_id_type source_id;
  target_id_type target_id;
  value_type     value;
};

template <class VId>
struct neighbor_descriptor<VId, true, void, void> {
  using source_id_type = VId;
  using target_id_type = VId;
  using vertex_type    = void;
  using value_type     = void;

  source_id_type source_id;
  target_id_type target_id;
};
#line 274 "graph/graph_descriptors.hpp"
template <class VId, class VV>
using copyable_neighbor_t = neighbor_descriptor<VId, true, void, VV>;
#line 280 "graph/graph_descriptors.hpp"
template <class T, class VId, class VV>
concept copyable_vertex = convertible_to<T, copyable_vertex_t<VId, VV>>;

template <class T, class VId, class EV>
concept copyable_edge = convertible_to<T, copyable_edge_t<VId, EV>>;

template <class T, class VId, class EV>
concept copyable_neighbor = convertible_to<T, copyable_neighbor_t<VId, EV>>;
#line 292 "graph/graph_descriptors.hpp"
template <class T>
inline constexpr bool is_sourced_v = false;
template <class VId, class V, class VV>
inline constexpr bool is_sourced_v<edge_descriptor<VId, true, V, VV>> = true;
template <class VId, class V, class VV>
inline constexpr bool is_sourced_v<neighbor_descriptor<VId, true, V, VV>> = true;

}
#line 13 "graph/detail/tag_invoke.hpp"
#include <type_traits>
#include <utility>




namespace std {
namespace tag_invoke_detail {
  struct tag_invoke_t {
    template <typename Tag, typename... Args>
    constexpr auto operator()(Tag tag, Args&&... args) const
          noexcept(noexcept(tag_invoke(static_cast<Tag&&>(tag), static_cast<Args&&>(args)...)))
                -> decltype(tag_invoke(static_cast<Tag&&>(tag), static_cast<Args&&>(args)...)) {
      return tag_invoke(static_cast<Tag&&>(tag), static_cast<Args&&>(args)...);
    }
  };

  template <class Tag>
  struct tag_invoke_value_t {
    static constexpr Tag value{};
  };

  template <class Tag>
  constexpr Tag tag_invoke_value_t<Tag>::value;

  static constexpr auto& tag_invoke = tag_invoke_value_t<tag_invoke_t>::value;

  template <typename Tag, typename... Args>
  auto tag_invoke_check(int) noexcept(noexcept(tag_invoke(::std::declval<Tag&&>(), ::std::declval<Args&&>()...)))
        -> decltype(static_cast<void>(tag_invoke(::std::declval<Tag&&>(), ::std::declval<Args&&>()...)),
                    std::true_type{});

  template <typename Tag, typename... Args>
  std::false_type tag_invoke_check(...) noexcept(false);
}
#line 56 "graph/detail/tag_invoke.hpp"
using tag_invoke_detail::tag_invoke;
#line 64 "graph/detail/tag_invoke.hpp"
template <typename Tag, typename... Args>
using tag_invoke_result_t = decltype(tag_invoke(::std::declval<Tag>(), ::std::declval<Args>()...));
#line 73 "graph/detail/tag_invoke.hpp"
template <typename Tag, typename... Args>
struct tag_invoke_is_nothrow {
  static constexpr bool value =
        std::integral_constant<bool, noexcept(tag_invoke_detail::tag_invoke_check<Tag, Args...>(0))>::value;
};

template <typename _Tag, typename... _Args>
concept tag_invocable =
      requires(_Tag tag, _Args... args) { std::tag_invoke(static_cast<_Tag&&>(tag), static_cast<_Args&&>(args)...); };

template <typename _Tag, typename... _Args>
concept nothrow_tag_invocable = requires(_Tag tag, _Args... args) {
  { std::tag_invoke(static_cast<_Tag&&>(tag), static_cast<_Args&&>(args)...) } noexcept;
};
#line 100 "graph/detail/tag_invoke.hpp"
template <typename Tag>
struct tag {
  template <typename... Args>
  constexpr auto operator()(Args&&... args) const noexcept(::std::tag_invoke_is_nothrow<Tag, decltype(args)...>::value)
        -> ::std::tag_invoke_result_t<Tag, decltype(args)...> {
    return ::std::tag_invoke(*static_cast<const Tag*>(this), ::std::forward<Args>(args)...);
  }
};
#line 116 "graph/detail/tag_invoke.hpp"
template <typename T>
constexpr const T& tag_invoke_v(T&&) {
  return tag_invoke_detail::tag_invoke_value_t<T>::value;
}
#line 139 "graph/detail/tag_invoke.hpp"
}
#line 12 "graph/detail/graph_cpo.hpp"
namespace std::graph {




struct _Decay_copy final {
  template <typename _Tp>
  constexpr decay_t<_Tp> operator()(_Tp&& __t) const noexcept(is_nothrow_convertible_v<_Tp, decay_t<_Tp>>) {
    return std::forward<_Tp>(__t);
  }
} inline constexpr _Fake_copy_init{};

template <class _Ty>
concept _Has_class_or_enum_type =
      __is_class(remove_reference_t<_Ty>) || __is_enum(remove_reference_t<_Ty>) || __is_union(remove_reference_t<_Ty>);

template <class>

inline constexpr bool _Always_false = false;

template <class _Ty>
inline constexpr bool _Is_nonbool_integral = is_integral_v<_Ty> && !is_same_v<remove_cv_t<_Ty>, bool>;

template <class _Ty>
inline constexpr bool _Integer_class = requires {
  typename _Ty::_Signed_type;
  typename _Ty::_Unsigned_type;
};

template <class _Ty>
concept _Integer_like = _Is_nonbool_integral<remove_cv_t<_Ty>> || _Integer_class<_Ty>;

template <class _Ty>
concept _Signed_integer_like = _Integer_like<_Ty> && static_cast<_Ty>(-1) < static_cast<_Ty>(0);

template <class _Ty>
struct _Choice_t {
  _Ty  _Strategy = _Ty{};
  bool _No_throw = false;
};



template <class _G>
concept _range_of_ranges = ranges::forward_range<_G> && ranges::forward_range<ranges::range_value_t<_G>>;


template <_range_of_ranges _G>
using _rr_edge_t = ranges::range_value_t<ranges::range_value_t<_G>>;

template <class _G>
concept _rr_simple_id = _range_of_ranges<_G> && integral<_rr_edge_t<_G>>;

template <class _G>
concept _rr_tuple_id = _range_of_ranges<_G> && integral<tuple_element_t<0, _rr_edge_t<_G>>>;


template <class _G>
struct _rr_vertex_id {
  using type = size_t;
};

template <_rr_simple_id _G>
struct _rr_vertex_id<_G> {
  using type = _rr_edge_t<_G>;
};

template <_rr_tuple_id _G>
struct _rr_vertex_id<_G> {
  using type =
        tuple_element_t<0, _rr_edge_t<_G>>;
};

template <class _G>
using _rr_vertex_id_t = typename _rr_vertex_id<_G>::type;
#line 112 "graph/detail/graph_cpo.hpp"
template <class G>
using graph_reference_t = add_lvalue_reference<G>;
#line 136 "graph/detail/graph_cpo.hpp"
template <class G>
struct define_adjacency_matrix : public false_type {};

template <class G>
struct is_adjacency_matrix : public define_adjacency_matrix<G> {};

template <class G>
inline constexpr bool is_adjacency_matrix_v = is_adjacency_matrix<G>::value;

template <class G>
concept adjacency_matrix = is_adjacency_matrix_v<G>;
#line 157 "graph/detail/graph_cpo.hpp"
namespace _Vertices {



  void vertices();


  template <class _G, class _UnCV>
  concept _Has_ref_member = _Has_class_or_enum_type<_G> &&
                            requires(_G&& __g) {
                              { _Fake_copy_init(__g.vertices()) };
                            };
  template <class _G, class _UnCV>
  concept _Has_ref_ADL = _Has_class_or_enum_type<_G>
                         && requires(_G&& __g) {
                              { _Fake_copy_init(vertices(__g)) };
                            };

  template <class _G, class _UnCV>
  concept _Can_ref_eval = _Has_class_or_enum_type<_G> && ranges::random_access_range<_G>;

  class _Cpo {
  private:
    enum class _St_ref { _None, _Member, _Non_member, _Auto_eval };

    template <class _G>
    [[nodiscard]] static consteval _Choice_t<_St_ref> _Choose_ref() noexcept {
      static_assert(is_lvalue_reference_v<_G>);
      using _UnCV = remove_cvref_t<_G>;

      if constexpr (_Has_ref_member<_G, _UnCV>) {
        return {_St_ref::_Member, noexcept(_Fake_copy_init(declval<_G>().vertices()))};
      } else if constexpr (_Has_ref_ADL<_G, _UnCV>) {
        return {_St_ref::_Non_member, noexcept(_Fake_copy_init(vertices(declval<_G>())))};
      } else if constexpr (_Can_ref_eval<_G, _UnCV>) {
        return {_St_ref::_Auto_eval, noexcept(true)};
      } else {
        return {_St_ref::_None};
      }
    }

    template <class _G>
    static constexpr _Choice_t<_St_ref> _Choice_ref = _Choose_ref<_G>();

  public:
#line 215 "graph/detail/graph_cpo.hpp"
    template <class _G>
    requires(_Choice_ref<_G&>._Strategy != _St_ref::_None)
    [[nodiscard]] constexpr auto operator()(_G&& __g) const noexcept(_Choice_ref<_G&>._No_throw) -> decltype(auto) {
      constexpr _St_ref _Strat_ref = _Choice_ref<_G&>._Strategy;

      if constexpr (_Strat_ref == _St_ref::_Member) {
        return __g.vertices();
      } else if constexpr (_Strat_ref == _St_ref::_Non_member) {

        return vertices(__g);
      } else if constexpr (_Strat_ref == _St_ref::_Auto_eval) {
        return std::forward<_G>(__g);
      } else {
        static_assert(_Always_false<_G>, "vertices(g) is not defined");
      }
    }
  };
}

inline namespace _Cpos {
  inline constexpr _Vertices::_Cpo vertices;
}
#line 265 "graph/detail/graph_cpo.hpp"
template <class G>
using vertex_range_t = decltype(std::graph::vertices(declval<G&&>()));





template <class G>
using vertex_iterator_t = ranges::iterator_t<vertex_range_t<G>>;





template <class G>
using vertex_t = ranges::range_value_t<vertex_range_t<G>>;





template <class G>
using vertex_reference_t = ranges::range_reference_t<vertex_range_t<G>>;
#line 294 "graph/detail/graph_cpo.hpp"
namespace _Vertex_id {



  void vertex_id();


  template <class _G, class _UnCV>
  concept _Has_ref_member = requires(_G&& __g, vertex_iterator_t<_G> ui) {
    { _Fake_copy_init(ui->vertex_id(__g)) };
  };
  template <class _G, class _UnCV>
  concept _Has_ref_ADL = _Has_class_or_enum_type<_G>
                         && requires(_G&& __g, const vertex_iterator_t<_G> ui) {
                              { _Fake_copy_init(vertex_id(__g, ui)) };
                            };
  template <class _G, class _UnCV>
  concept _Can_ref_eval = ranges::random_access_range<vertex_range_t<_G>>;

  class _Cpo {
  private:
    enum class _St_ref { _None, _Member, _Non_member, _Auto_eval };

    template <class _G>
    [[nodiscard]] static consteval _Choice_t<_St_ref> _Choose_ref() noexcept {
      static_assert(is_lvalue_reference_v<_G>);
      using _UnCV = remove_cvref_t<_G>;

      if constexpr (_Has_ref_member<_G, _UnCV>) {
        return {_St_ref::_Member,
                noexcept(_Fake_copy_init(declval<vertex_iterator_t<_G>>()->vertex_id(declval<_G>())))};
      } else if constexpr (_Has_ref_ADL<_G, _UnCV>) {
        return {
              _St_ref::_Non_member,
              noexcept(_Fake_copy_init(vertex_id(declval<_G>(), declval<vertex_iterator_t<_G>>())))};
      } else if constexpr (_Can_ref_eval<_G, _UnCV>) {
        return {_St_ref::_Auto_eval,
                noexcept(_Fake_copy_init(declval<vertex_iterator_t<_G>>() -
                                         ranges::begin(vertices(declval<_G>()))))};
      } else {
        return {_St_ref::_None};
      }
    }

    template <class _G>
    static constexpr _Choice_t<_St_ref> _Choice_ref = _Choose_ref<_G>();
#line 347 "graph/detail/graph_cpo.hpp"
    template <class _G>
    using _vid_t = _rr_vertex_id_t<_G>;

  public:
#line 376 "graph/detail/graph_cpo.hpp"
    template <class _G>
    requires(_Choice_ref<_G&>._Strategy != _St_ref::_None)
    [[nodiscard]] constexpr auto operator()(_G&& __g, vertex_iterator_t<_G> ui) const
          noexcept(_Choice_ref<_G&>._No_throw) {
      constexpr _St_ref _Strat_ref = _Choice_ref<_G&>._Strategy;

      if constexpr (_Strat_ref == _St_ref::_Member) {
        return ui->vertex_id(__g);
      } else if constexpr (_Strat_ref == _St_ref::_Non_member) {
        return vertex_id(__g, ui);
      } else if constexpr (_Strat_ref == _St_ref::_Auto_eval) {
        return static_cast<_vid_t<_G>>(ui - ranges::begin(vertices(__g)));
      } else {
        static_assert(_Always_false<_G>, "vertices(g) is not defined or is not random-access");
      }
    }
  };
}

inline namespace _Cpos {
  inline constexpr _Vertex_id::_Cpo vertex_id;
}
#line 408 "graph/detail/graph_cpo.hpp"
template <class G>
using vertex_id_t = decltype(vertex_id(declval<G&&>(), declval<vertex_iterator_t<G>>()));
#line 417 "graph/detail/graph_cpo.hpp"
namespace _Find_vertex {



  void find_vertex();


  template <class _G, class _UnCV>
  concept _Has_member = requires(_G&& __g, const vertex_id_t<_G>& uid) {
    { _Fake_copy_init(__g.find_vertex(uid)) };
  };

  template <class _G, class _UnCV>
  concept _Has_ADL = _Has_class_or_enum_type<_G>
                     && requires(_G&& __g, const vertex_id_t<_G>& uid) {
                          { _Fake_copy_init(find_vertex(__g, uid)) };
                        };

  class _Cpo {
  private:
    enum class _St { _None, _Member, _Non_member };

    template <class _G>
    [[nodiscard]] static consteval _Choice_t<_St> _Choose() noexcept {
      static_assert(is_lvalue_reference_v<_G>);
      using _UnCV = remove_cvref_t<_G>;

      if constexpr (_Has_member<_G, _UnCV>) {
        return {_St::_Member, noexcept(_Fake_copy_init(declval<_G>().find_vertex(declval<vertex_id_t<_G>>())))};
      } else if constexpr (_Has_ADL<_G, _UnCV>) {
        return {_St::_Non_member,
                noexcept(_Fake_copy_init(find_vertex(declval<_G>(), declval<vertex_id_t<_G>>())))};
      } else {
        return {_St::_None};
      }
    }

    template <class _G>
    static constexpr _Choice_t<_St> _Choice = _Choose<_G>();

  public:
#line 470 "graph/detail/graph_cpo.hpp"
    template <class _G>
    [[nodiscard]] constexpr auto operator()(_G&& __g, const vertex_id_t<_G>& uid) const
          noexcept(_Choice<_G&>._No_throw) {
      constexpr _St _Strat = _Choice<_G&>._Strategy;

      if constexpr (_Strat == _St::_Member) {
        return __g.find_vertex(uid);
      } else if constexpr (_Strat == _St::_Non_member) {
        return find_vertex(__g, uid);
      } else if constexpr (random_access_iterator<vertex_iterator_t<_G>>) {
        auto uid_diff = static_cast<ranges::range_difference_t<vertex_range_t<_G>>>(uid);
        if (uid_diff < ssize(vertices(__g)))
          return begin(vertices(__g)) + uid_diff;
        else
          return end(vertices(__g));
      } else {
        static_assert(_Always_false<_G>,
                      "find_vertex(g,uid) has not been defined and the default implemenation cannot be evaluated");
      }
    }
  };
}

inline namespace _Cpos {
  inline constexpr _Find_vertex::_Cpo find_vertex;
}
#line 500 "graph/detail/graph_cpo.hpp"
namespace _Partition_id {



  void partition_id();


  template <class _G, class _UnCV>
  concept _Has_ref_member = requires(_G&& __g, vertex_reference_t<_G> u) {
    { _Fake_copy_init(u.partition_id(__g)) };
  };
  template <class _G, class _UnCV>
  concept _Has_ref_ADL = _Has_class_or_enum_type<_G>
                         && requires(_G&& __g, const vertex_reference_t<_G>& u) {
                              { _Fake_copy_init(partition_id(__g, u)) };
                            };
  template <class _G, class _UnCV>
  concept _Can_ref_eval = _Has_class_or_enum_type<_G> && integral<vertex_id_t<_G>>
                          && requires(_G&& __g, vertex_id_t<_G> uid) {
                               { _Fake_copy_init(vertex_id_t<_G>{0}) };
                             };

  template <class _G, class _UnCV>
  concept _Has_id_ADL = _Has_class_or_enum_type<_G>
                        && requires(_G&& __g, const vertex_id_t<_G>& uid) {
                             { _Fake_copy_init(partition_id(__g, uid)) };
                           };
  template <class _G, class _UnCV>
  concept _Can_id_eval = _Has_class_or_enum_type<_G> && integral<vertex_id_t<_G>>
                         && requires(_G&& __g) {
                              { _Fake_copy_init(vertex_id_t<_G>{0}) };
                            };

  class _Cpo {
  private:
    enum class _St_id { _None, _Non_member, _Auto_eval };
    enum class _St_ref { _None, _Member, _Non_member, _Auto_eval };

    template <class _G>
    [[nodiscard]] static consteval _Choice_t<_St_id> _Choose_id() noexcept {
      static_assert(is_lvalue_reference_v<_G>);
      using _UnCV = remove_cvref_t<_G>;

      if constexpr (_Has_id_ADL<_G, _UnCV>) {
        return {_St_id::_Non_member,
                noexcept(_Fake_copy_init(partition_id(declval<_G>(), declval<vertex_id_t<_G>>())))};
      } else if constexpr (_Can_id_eval<_G, _UnCV>) {
        return {_St_id::_Auto_eval, noexcept(_Fake_copy_init(vertex_id_t<_G>(0)))};
      } else {
        return {_St_id::_None};
      }
    }

    template <class _G>
    static constexpr _Choice_t<_St_id> _Choice_id = _Choose_id<_G>();

    template <class _G>
    [[nodiscard]] static consteval _Choice_t<_St_ref> _Choose_ref() noexcept {
      static_assert(is_lvalue_reference_v<_G>);
      using _UnCV = remove_cvref_t<_G>;

      if constexpr (_Has_ref_member<_G, _UnCV>) {
        return {_St_ref::_Member,
                noexcept(_Fake_copy_init(declval<vertex_reference_t<_G>>().partition_id(declval<_G>())))};
      } else if constexpr (_Has_ref_ADL<_G, _UnCV>) {
        return {_St_ref::_Non_member, noexcept(_Fake_copy_init(partition_id(
                                            declval<_G>(), declval<vertex_reference_t<_G>>())))};
      } else if constexpr (_Can_ref_eval<_G, _UnCV>) {
        return {_St_ref::_Auto_eval, noexcept(_Fake_copy_init(vertex_id_t<_G>(0)))};
      } else {
        return {_St_ref::_None};
      }
    }

    template <class _G>
    static constexpr _Choice_t<_St_ref> _Choice_ref = _Choose_ref<_G>();

  public:
#line 590 "graph/detail/graph_cpo.hpp"
    template <class _G>
    requires(_Choice_ref<_G&>._Strategy != _St_ref::_None)
    [[nodiscard]] constexpr auto operator()(_G&& __g, vertex_reference_t<_G> u) const
          noexcept(_Choice_ref<_G&>._No_throw) {
      constexpr _St_ref _Strat_ref = _Choice_ref<_G&>._Strategy;

      if constexpr (_Strat_ref == _St_ref::_Member) {
        return u.partition_id(__g);
      } else if constexpr (_Strat_ref == _St_ref::_Non_member) {
        return partition_id(__g, u);
      } else if constexpr (_Strat_ref == _St_ref::_Auto_eval) {
        return vertex_id_t<_G>{0};
      } else {
        static_assert(_Always_false<_G>,
                      "partition_id(g,u) is not defined and the default implementation cannot be evaluated");
      }
    }
#line 620 "graph/detail/graph_cpo.hpp"
    template <class _G>
    requires(_Choice_id<_G&>._Strategy != _St_id::_None)
    [[nodiscard]] constexpr auto operator()(_G&& __g, const vertex_id_t<_G>& uid) const
          noexcept(_Choice_id<_G&>._No_throw) {
      constexpr _St_id _Strat_id = _Choice_id<_G&>._Strategy;

      if constexpr (_Strat_id == _St_id::_Non_member) {
        return partition_id(__g, uid);
      } else if constexpr (_Strat_id == _St_id::_Auto_eval) {
        return vertex_id_t<_G>{0};
      } else {
        static_assert(_Always_false<_G>,
                      "partition_id(g,uid) is not defined and the default implementation cannot be evaluated");
      }
    }
  };
}

inline namespace _Cpos {
  inline constexpr _Partition_id::_Cpo partition_id;
}

template <class G>
using partition_id_t = decltype(partition_id(declval<G>(), declval<vertex_reference_t<G>>()));
#line 657 "graph/detail/graph_cpo.hpp"
namespace _Edges {



  void edges();


  template <class _G, class _UnCV>
  concept _Has_ref_member = requires(_G&& __g, vertex_reference_t<_G> u) {
    { _Fake_copy_init(u.edges(__g)) };
  };
  template <class _G, class _UnCV>
  concept _Has_ref_ADL = _Has_class_or_enum_type<_G>
                         && requires(_G&& __g, const vertex_reference_t<_G>& u) {
                              { _Fake_copy_init(edges(__g, u)) };
                            };
  template <class _G, class _UnCV>
  concept _Can_ref_eval = _Has_class_or_enum_type<_G> && ranges::forward_range<vertex_t<_G>>;

  template <class _G, class _UnCV>
  concept _Has_id_ADL = _Has_class_or_enum_type<_G>
                        && requires(_G&& __g, const vertex_id_t<_G>& uid) {
                             { _Fake_copy_init(edges(__g, uid)) };
                           };
  template <class _G, class _UnCV>
  concept _Can_id_eval = _Has_class_or_enum_type<_G> && ranges::forward_range<vertex_t<_G>>
                         && requires(_G&& __g, vertex_id_t<_G> uid) {
                              { _Fake_copy_init(find_vertex(__g, uid)) };
                            };

  class _Cpo {
  private:
    enum class _St_id { _None, _Non_member, _Auto_eval };
    enum class _St_ref { _None, _Member, _Non_member, _Auto_eval };

    template <class _G>
    [[nodiscard]] static consteval _Choice_t<_St_id> _Choose_id() noexcept {
      static_assert(is_lvalue_reference_v<_G>);
      using _UnCV = remove_cvref_t<_G>;

      if constexpr (_Has_id_ADL<_G, _UnCV>) {
        return {_St_id::_Non_member,
                noexcept(_Fake_copy_init(edges(declval<_G>(), declval<vertex_id_t<_G>>())))};
      } else if constexpr (_Can_id_eval<_G, _UnCV>) {
        return {_St_id::_Auto_eval,
                noexcept(_Fake_copy_init(*find_vertex(declval<_G>(), declval<vertex_id_t<_G>>())))};
      } else {
        return {_St_id::_None};
      }
    }

    template <class _G>
    static constexpr _Choice_t<_St_id> _Choice_id = _Choose_id<_G>();

    template <class _G>
    [[nodiscard]] static consteval _Choice_t<_St_ref> _Choose_ref() noexcept {
      static_assert(is_lvalue_reference_v<_G>);
      using _UnCV = remove_cvref_t<_G>;

      if constexpr (_Has_ref_member<_G, _UnCV>) {
        return {_St_ref::_Member, noexcept(_Fake_copy_init(declval<vertex_reference_t<_G>>().edges(declval<_G>())))};
      } else if constexpr (_Has_ref_ADL<_G, _UnCV>) {
        return {_St_ref::_Non_member,
                noexcept(_Fake_copy_init(edges(declval<_G>(), declval<vertex_reference_t<_G>>())))};
      } else if constexpr (_Can_ref_eval<_G, _UnCV>) {
        return {_St_ref::_Auto_eval,
                noexcept(_Fake_copy_init(*find_vertex(declval<_G>(), declval<vertex_id_t<_G>>())))};
      } else {
        return {_St_ref::_None};
      }
    }

    template <class _G>
    static constexpr _Choice_t<_St_ref> _Choice_ref = _Choose_ref<_G>();

  public:
#line 745 "graph/detail/graph_cpo.hpp"
    template <class _G>
    requires(_Choice_ref<_G&>._Strategy != _St_ref::_None)
    [[nodiscard]] constexpr auto operator()(_G&& __g, vertex_reference_t<_G> u) const
          noexcept(_Choice_ref<_G&>._No_throw) -> decltype(auto) {
      constexpr _St_ref _Strat_ref = _Choice_ref<_G&>._Strategy;

      if constexpr (_Strat_ref == _St_ref::_Member) {
        return u.edges(__g);
      } else if constexpr (_Strat_ref == _St_ref::_Non_member) {
        return edges(__g, u);
      } else if constexpr (_Strat_ref == _St_ref::_Auto_eval) {
        return u;
      } else {
        static_assert(_Always_false<_G>,
                      "edges(g,u) is not defined and the default implementation cannot be evaluated");
      }
    }
#line 775 "graph/detail/graph_cpo.hpp"
    template <class _G>
    requires(_Choice_id<_G&>._Strategy != _St_id::_None)
    [[nodiscard]] constexpr auto operator()(_G&& __g, const vertex_id_t<_G>& uid) const
          noexcept(_Choice_id<_G&>._No_throw) -> decltype(auto) {
      constexpr _St_id _Strat_id = _Choice_id<_G&>._Strategy;

      if constexpr (_Strat_id == _St_id::_Non_member) {
        return edges(__g, uid);
      } else if constexpr (_Strat_id == _St_id::_Auto_eval) {
        return *find_vertex(__g, uid);
      } else {
        static_assert(_Always_false<_G>,
                      "edges(g,uid) is not defined and the default implementation cannot be evaluated");
      }
    }
  };
}

inline namespace _Cpos {
  inline constexpr _Edges::_Cpo edges;
}
#line 854 "graph/detail/graph_cpo.hpp"
template <class G>
using vertex_edge_range_t = decltype(edges(declval<G&&>(), declval<vertex_reference_t<G>>()));





template <class G>
using vertex_edge_iterator_t = ranges::iterator_t<vertex_edge_range_t<G>>;





template <class G>
using edge_t = ranges::range_value_t<vertex_edge_range_t<G>>;





template <class G>
using edge_reference_t = ranges::range_reference_t<vertex_edge_range_t<G>>;
#line 884 "graph/detail/graph_cpo.hpp"
namespace _Target_id {



  void target_id();


  template <class _G>
  concept _Has_ref_member = requires(_G&& __g, edge_reference_t<_G> uv) {
    { _Fake_copy_init(uv.target_id(__g)) };
  };
  template <class _G>
  concept _Has_ref_ADL = requires(_G&& __g, edge_reference_t<_G> uv) {
    { _Fake_copy_init(target_id(__g, uv)) };
  };

  template <class _G>
  concept _Is_basic_id_adj = integral<_rr_edge_t<_G>>;

  template <class _G>
  concept _Is_tuple_id_adj = integral<tuple_element_t<0, _rr_edge_t<_G>>>;

  class _Cpo {
  private:
    enum class _St_ref { _None, _Member, _Non_member, _Basic_id, _Tuple_id, _Auto_eval };

    template <class _G>
    [[nodiscard]] static consteval _Choice_t<_St_ref> _Choose_ref() noexcept {
      static_assert(is_lvalue_reference_v<_G>);

      if constexpr (_Has_ref_member<_G>) {
        return {_St_ref::_Member, noexcept(_Fake_copy_init(declval<edge_reference_t<_G>>().target_id(declval<_G>())))};
      } else if constexpr (_Has_ref_ADL<_G>) {
        return {
              _St_ref::_Non_member,
              noexcept(_Fake_copy_init(target_id(declval<_G>(), declval<edge_reference_t<_G>>())))};
      } else if constexpr (_Is_basic_id_adj<_G>) {
        return {_St_ref::_Basic_id,
                noexcept(_Fake_copy_init(declval<edge_reference_t<_G>>()))};
      } else if constexpr (_Is_tuple_id_adj<_G>) {
        return {
              _St_ref::_Tuple_id,
              noexcept(_Fake_copy_init(get<0>(declval<edge_reference_t<_G>>())))};
      } else {
        return {_St_ref::_None};
      }
    }

    template <class _G>
    static constexpr _Choice_t<_St_ref> _Choice_ref = _Choose_ref<_G>();

  public:
#line 948 "graph/detail/graph_cpo.hpp"
    template <class _G>

    [[nodiscard]] constexpr auto operator()(_G&& __g, edge_reference_t<_G> uv) const
          noexcept(_Choice_ref<_G&>._No_throw) {
      constexpr _St_ref _Strat_ref = _Choice_ref<_G&>._Strategy;

      if constexpr (_Strat_ref == _St_ref::_Member) {
        return uv.target_id(__g);
      } else if constexpr (_Strat_ref == _St_ref::_Non_member) {
        return target_id(__g, uv);
      } else if constexpr (_Strat_ref == _St_ref::_Basic_id) {
        return uv;
      } else if constexpr (_Strat_ref == _St_ref::_Tuple_id) {
        return get<0>(uv);
      } else {
        static_assert(_Always_false<_G>, "target_id(g,uv) or g.target_id(uv) is not defined");
      }
    }
  };
}

inline namespace _Cpos {
  inline constexpr _Target_id::_Cpo target_id;
}
#line 980 "graph/detail/graph_cpo.hpp"
namespace _Target {



  void target();


  template <class _G>
  concept _Has_ref_ADL = _Has_class_or_enum_type<_G>
                         && requires(_G&& __g, const edge_reference_t<_G>& uv) {
                              { _Fake_copy_init(target(__g, uv)) };
                            };
  template <class _G>
  concept _Can_ref_eval = ranges::random_access_range<vertex_range_t<_G>>
                          && requires(_G&& __g, edge_reference_t<_G> uv, vertex_id_t<_G> uid) {
                               { _Fake_copy_init(find_vertex(__g, uid)) };
                               { _Fake_copy_init(target_id(__g, uv)) } -> integral;
                             };

  class _Cpo {
  private:
    enum class _St_ref { _None, _Non_member, _Auto_eval };

    template <class _G>
    [[nodiscard]] static consteval _Choice_t<_St_ref> _Choose_ref() noexcept {
      static_assert(is_lvalue_reference_v<_G>);

      if constexpr (_Has_ref_ADL<_G>) {
        return {_St_ref::_Non_member,
                noexcept(_Fake_copy_init(target(declval<_G>(), declval<edge_reference_t<_G>>())))};
      } else if constexpr (_Can_ref_eval<_G>) {
        return {_St_ref::_Auto_eval,
                noexcept(_Fake_copy_init(*find_vertex(
                      declval<_G>(), target_id(declval<_G>(), declval<edge_reference_t<_G>>()))))};
      } else {
        return {_St_ref::_None};
      }
    }

    template <class _G>
    static constexpr _Choice_t<_St_ref> _Choice_ref = _Choose_ref<_G>();

  public:
#line 1035 "graph/detail/graph_cpo.hpp"
    template <class _G>
    requires(_Choice_ref<_G&>._Strategy != _St_ref::_None)
    [[nodiscard]] constexpr auto&& operator()(_G&& __g, edge_reference_t<_G> uv) const
          noexcept(_Choice_ref<_G&>._No_throw) {
      constexpr _St_ref _Strat_ref = _Choice_ref<_G&>._Strategy;

      if constexpr (_Strat_ref == _St_ref::_Non_member) {
        return target(__g, uv);
      } else if constexpr (_Strat_ref == _St_ref::_Auto_eval) {
        return *find_vertex(__g, target_id(__g, uv));
      } else {
        static_assert(_Always_false<_G>, "target(g,uv) or uv.target(g) or g.target_id(g,uv) is not defined");
      }
    }
  };
}

inline namespace _Cpos {
  inline constexpr _Target::_Cpo target;
}
#line 1061 "graph/detail/graph_cpo.hpp"
namespace _EL_Source_id {



  void source_id();


  template <class _G, class _UnCV>
  concept _Has_ref_member = requires(_G&& __g, edge_reference_t<_G> uv) {
    { _Fake_copy_init(uv.source_id(__g)) };
  };
  template <class _G, class _UnCV>
  concept _Has_ref_ADL = _Has_class_or_enum_type<_G>
                         && requires(_G&& __g, const edge_reference_t<_G>& uv) {
                              { _Fake_copy_init(source_id(__g, uv)) };
                            };

  class _Cpo {
  private:
    enum class _St_ref { _None, _Member, _Non_member, _Auto_eval };

    template <class _G>
    [[nodiscard]] static consteval _Choice_t<_St_ref> _Choose_ref() noexcept {
      static_assert(is_lvalue_reference_v<_G>);
      using _UnCV = remove_cvref_t<_G>;

      if constexpr (_Has_ref_member<_G, _UnCV>) {
        return {_St_ref::_Member, noexcept(_Fake_copy_init(declval<edge_reference_t<_G>>().source_id(declval<_G>())))};
      } else if constexpr (_Has_ref_ADL<_G, _UnCV>) {
        return {
              _St_ref::_Non_member,
              noexcept(_Fake_copy_init(source_id(declval<_G>(), declval<edge_reference_t<_G>>())))};
      } else {
        return {_St_ref::_None};
      }
    }

    template <class _G>
    static constexpr _Choice_t<_St_ref> _Choice_ref = _Choose_ref<_G>();

  public:
#line 1114 "graph/detail/graph_cpo.hpp"
    template <class _G>
    requires(_Choice_ref<_G&>._Strategy != _St_ref::_None)
    [[nodiscard]] constexpr auto operator()(_G&& __g, edge_reference_t<_G> uv) const
          noexcept(_Choice_ref<_G&>._No_throw) {
      constexpr _St_ref _Strat_ref = _Choice_ref<_G&>._Strategy;

      if constexpr (_Strat_ref == _St_ref::_Member) {
        return uv.source_id(__g);
      } else if constexpr (_Strat_ref == _St_ref::_Non_member) {
        return source_id(__g, uv);
      } else {
        static_assert(_Always_false<_G>, "source_id(g,uv) or g.source_id(uv) is not defined");
      }
    }
  };
}

inline namespace _Cpos {
  inline constexpr _EL_Source_id::_Cpo source_id;
}
#line 1142 "graph/detail/graph_cpo.hpp"
namespace _Source {



  void source();


  template <class _G, class _UnCV>
  concept _Has_ref_ADL = _Has_class_or_enum_type<_G>
                         && requires(_G&& __g, const edge_reference_t<_G>& uv) {
                              { _Fake_copy_init(source(__g, uv)) };
                            };
  template <class _G, class _UnCV>
  concept _Can_ref_eval = ranges::random_access_range<vertex_range_t<_G>>
                          && requires(_G&& __g, edge_reference_t<_G> uv) {
                               { _Fake_copy_init(source_id(__g, uv)) } -> integral;
                             };

  class _Cpo {
  private:
    enum class _St_ref { _None, _Non_member, _Auto_eval };

    template <class _G>
    [[nodiscard]] static consteval _Choice_t<_St_ref> _Choose_ref() noexcept {
      static_assert(is_lvalue_reference_v<_G>);
      using _UnCV = remove_cvref_t<_G>;

      if constexpr (_Has_ref_ADL<_G, _UnCV>) {
        return {_St_ref::_Non_member,
                noexcept(_Fake_copy_init(source(declval<_G>(), declval<edge_reference_t<_G>>())))};
      } else if constexpr (_Can_ref_eval<_G, _UnCV>) {
        return {
              _St_ref::_Auto_eval,
              noexcept(_Fake_copy_init(begin(vertices(declval<_G>())) +
                                       source_id(declval<_G>(), declval<edge_reference_t<_G>>())))};
      } else {
        return {_St_ref::_None};
      }
    }

    template <class _G>
    static constexpr _Choice_t<_St_ref> _Choice_ref = _Choose_ref<_G>();

  public:
#line 1203 "graph/detail/graph_cpo.hpp"
    template <class _G>
    requires(_Choice_ref<_G&>._Strategy != _St_ref::_None)
    [[nodiscard]] constexpr auto&& operator()(_G&& __g, edge_reference_t<_G> uv) const
          noexcept(_Choice_ref<_G&>._No_throw) {
      constexpr _St_ref _Strat_ref = _Choice_ref<_G&>._Strategy;

      if constexpr (_Strat_ref == _St_ref::_Non_member) {
        return source(__g, uv);
      } else if constexpr (_Strat_ref == _St_ref::_Auto_eval) {
        return *(begin(vertices(__g)) + source_id(__g, uv));
      } else {
        static_assert(_Always_false<_G>, "source(g,uv) or g.source(uv) is not defined");
      }
    }
  };
}

inline namespace _Cpos {
  inline constexpr _Source::_Cpo source;
}
#line 1231 "graph/detail/graph_cpo.hpp"
template <class _G>
using edge_id_t = edge_descriptor<vertex_id_t<_G>, true, void, void>;

namespace _Edge_id {



  void edge_id();


  template <class _G, class _UnCV>
  concept _Has_ref_member = requires(_G&& __g, edge_reference_t<_G> uv) {
    { uv.edge_id(__g) } -> convertible_to<edge_id_t<_G>>;
  };
  template <class _G, class _UnCV>
  concept _Has_ref_ADL = _Has_class_or_enum_type<_G>
                         && requires(_G&& __g, const edge_reference_t<_G>& uv) {
                              { _Fake_copy_init(edge_id(__g, uv)) } -> convertible_to<edge_id_t<_G>>;
                            };

  template <class _G, class _UnCV>
  concept _Can_id_eval = _Has_class_or_enum_type<_G>
                         && requires(_G&& __g, edge_reference_t<_G> uv) {
                              { _Fake_copy_init(edge_id_t<_G>{source_id(__g, uv), target_id(__g, uv)}) };
                            };
  class _Cpo {
  private:
    enum class _St_ref { _None, _Member, _Non_member, _Auto_eval };

    template <class _G>
    [[nodiscard]] static consteval _Choice_t<_St_ref> _Choose_ref() noexcept {
      static_assert(is_lvalue_reference_v<_G>);
      using _UnCV = remove_cvref_t<_G>;

      if constexpr (_Has_ref_member<_G, _UnCV>) {
        return {_St_ref::_Member, noexcept(_Fake_copy_init(declval<edge_reference_t<_G>>().edge_id(declval<_G>())))};
      } else if constexpr (_Has_ref_ADL<_G, _UnCV>) {
        return {_St_ref::_Non_member,
                noexcept(_Fake_copy_init(edge_id(declval<_G>(), declval<edge_reference_t<_G>>())))};
      } else if constexpr (_Can_id_eval<_G, _UnCV>) {
        return {_St_ref::_Auto_eval,
                noexcept(_Fake_copy_init(edge_id_t<_G>{source_id(declval<_G>(), declval<edge_reference_t<_G>>()),
                                                       target_id(declval<_G>(), declval<edge_reference_t<_G>>())}))};
      } else {
        return {_St_ref::_None};
      }
    }

    template <class _G>
    static constexpr _Choice_t<_St_ref> _Choice_ref = _Choose_ref<_G>();

  public:
#line 1297 "graph/detail/graph_cpo.hpp"
    template <class _G>
    requires(_Choice_ref<_G&>._Strategy != _St_ref::_None)
    [[nodiscard]] constexpr auto operator()(_G&& __g, edge_reference_t<_G> uv) const
          noexcept(_Choice_ref<_G&>._No_throw) {
      constexpr _St_ref _Strat_ref = _Choice_ref<_G&>._Strategy;

      if constexpr (_Strat_ref == _St_ref::_Member) {
        return uv.edge_id(__g);
      } else if constexpr (_Strat_ref == _St_ref::_Non_member) {
        return edge_id(__g, uv);
      } else if constexpr (_Strat_ref == _St_ref::_Auto_eval) {
        return edge_id_t<_G>{source_id(__g, uv), target_id(__g, uv)};
      } else {
        static_assert(_Always_false<_G>,
                      "edge_id(g,uv) is not defined, or target_id(g,uv) and source_id(g,uv) are not defined");
      }
    }
  };
}

inline namespace _Cpos {
  inline constexpr _Edge_id::_Cpo edge_id;
}
#line 1329 "graph/detail/graph_cpo.hpp"
namespace _Find_vertex_edge {



  void find_vertex_edge();


  template <class _G, class _UnCV>
  concept _Has_ref_member = requires(_G&& __g, vertex_reference_t<_G> u, const vertex_id_t<_G>& vid) {
    { _Fake_copy_init(u.find_vertex_edge(__g, vid)) };
  };

  template <class _G, class _UnCV>
  concept _Has_ref_ADL = _Has_class_or_enum_type<_G>
                         && requires(_G&& __g, vertex_reference_t<_G> u, const vertex_id_t<_G>& vid) {
                              { _Fake_copy_init(find_vertex_edge(__g, u, vid)) };
                            };

  template <class _G, class _UnCV>
  concept _Can_ref_eval = requires(_G&& __g, vertex_reference_t<_G> u) {
    { _Fake_copy_init(edges(__g, u)) };
  };

  template <class _G, class _UnCV>
  concept _Has_id_ADL = _Has_class_or_enum_type<_G>
                        && requires(_G&& __g, vertex_id_t<_G> uid, const vertex_id_t<_G>& vid) {
                             { _Fake_copy_init(find_vertex_edge(__g, uid, vid)) };
                           };

  template <class _G, class _UnCV>
  concept _Can_id_eval = requires(_G&& __g, vertex_id_t<_G> uid) {
    { _Fake_copy_init(edges(__g, uid)) };
  };

  class _Cpo {
  private:
    enum class _St_ref { _None, _Member, _Non_member, _Auto_eval };
    enum class _St_id { _None, _Non_member, _Auto_eval };

    template <class _G>
    [[nodiscard]] static consteval _Choice_t<_St_ref> _Choose_ref() noexcept {
      static_assert(is_lvalue_reference_v<_G>);
      using _UnCV = remove_cvref_t<_G>;

      if constexpr (_Has_ref_member<_G, _UnCV>) {
        return {_St_ref::_Member, noexcept(_Fake_copy_init(declval<vertex_reference_t<_G>>().find_vertex_edge(
                                        declval<_G>(), declval<vertex_id_t<_G>>())))};
      } else if constexpr (_Has_ref_ADL<_G, _UnCV>) {
        return {_St_ref::_Non_member,
                noexcept(_Fake_copy_init(find_vertex_edge(declval<_G>(), declval<vertex_reference_t<_G>>(),
                                                          declval<vertex_id_t<_G>>())))};
      } else if constexpr (_Can_ref_eval<_G, _UnCV>) {
        using fnc_find_t = decltype([](edge_reference_t<_G>) -> bool { return true; });
        return {_St_ref::_Auto_eval,
                noexcept(_Fake_copy_init(ranges::find_if(edges(declval<_G>(), declval<vertex_reference_t<_G>>()),
                                                         declval<fnc_find_t>())))};
      } else {
        return {_St_ref::_None};
      }
    }

    template <class _G>
    static constexpr _Choice_t<_St_ref> _Choice_ref = _Choose_ref<_G>();

    template <class _G>
    [[nodiscard]] static consteval _Choice_t<_St_id> _Choose_id() noexcept {
      static_assert(is_lvalue_reference_v<_G>);
      using _UnCV = remove_cvref_t<_G>;

      if constexpr (_Has_id_ADL<_G, _UnCV>) {
        return {_St_id::_Non_member,
                noexcept(_Fake_copy_init(find_vertex_edge(declval<_G>(), declval<vertex_id_t<_G>>(),
                                                          declval<vertex_id_t<_G>>())))};
      } else if constexpr (_Can_id_eval<_G, _UnCV>) {
        using fnc_find_t = decltype([](edge_reference_t<_G>) -> bool { return true; });
        return {_St_id::_Auto_eval,
                noexcept(_Fake_copy_init(ranges::find_if(edges(declval<_G>(), declval<vertex_id_t<_G>>()),
                                                         declval<fnc_find_t>())))};
      } else {
        return {_St_id::_None};
      }
    }

    template <class _G>
    static constexpr _Choice_t<_St_id> _Choice_id = _Choose_id<_G>();

  public:
#line 1429 "graph/detail/graph_cpo.hpp"
    template <class _G>
    [[nodiscard]] constexpr auto operator()(_G&& __g, vertex_reference_t<_G> u, const vertex_id_t<_G>& vid) const
          noexcept(_Choice_ref<_G&>._No_throw) {
      constexpr _St_ref _Strat = _Choice_ref<_G&>._Strategy;

      if constexpr (_Strat == _St_ref::_Member) {
        return u.find_vertex_edge(__g, vid);
      } else if constexpr (_Strat == _St_ref::_Non_member) {
        return find_vertex_edge(__g, u, vid);
      } else if constexpr (_Strat == _St_ref::_Auto_eval) {
        return ranges::find_if(edges(__g, u), [&__g, &vid](auto&& uv) { return target_id(__g, uv) == vid; });
      } else {
        static_assert(_Always_false<_G>,
                      "find_vertex_edge(g,uid) has not been defined and the default implemenation cannot be evaluated");
      }
    }
#line 1459 "graph/detail/graph_cpo.hpp"
    template <class _G>
    [[nodiscard]] constexpr auto operator()(_G&& __g, vertex_id_t<_G> uid, const vertex_id_t<_G>& vid) const
          noexcept(_Choice_id<_G&>._No_throw) {
      constexpr _St_id _Strat = _Choice_id<_G&>._Strategy;

      if constexpr (_Strat == _St_id::_Non_member) {
        return find_vertex_edge(__g, uid, uid);
      } else if constexpr (_Strat == _St_id::_Auto_eval) {
        return ranges::find_if(edges(__g, uid), [&__g, &vid](auto&& uv) { return target_id(__g, uv) == vid; });
      } else {
        static_assert(_Always_false<_G>,
                      "find_vertex_edge(g,uid) has not been defined and the default implemenation cannot be evaluated");
      }
    }
  };
}

inline namespace _Cpos {
  inline constexpr _Find_vertex_edge::_Cpo find_vertex_edge;
}
#line 1486 "graph/detail/graph_cpo.hpp"
namespace _Contains_edge {



  void contains_edge();


  template <class _G, class _UnCV>
  concept _Has_ref_ADL = _Has_class_or_enum_type<_G>
                         && requires(_G&& __g, const vertex_id_t<_G>& uid, const vertex_id_t<_G>& vid) {
                              { _Fake_copy_init(contains_edge(__g, uid, vid)) };
                            };

  template <class _G, class _UnCV>
  concept _Can_matrix_eval = _Has_class_or_enum_type<_G> && is_adjacency_matrix_v<_G>
                             && ranges::sized_range<vertex_range_t<_G>>;

  template <class _G, class _UnCV>
  concept _Can_id_eval = _Has_class_or_enum_type<_G>
                         && requires(_G&& __g, vertex_reference_t<_G> u, vertex_id_t<_G> uid, vertex_id_t<_G> vid) {
                              { _Fake_copy_init(find_vertex(__g, uid)) };
                              { _Fake_copy_init(edges(__g, u)) };
                              { _Fake_copy_init(find_vertex_edge(__g, u, vid)) };
                            };
  class _Cpo {
  private:
    enum class _St_ref { _None, _Non_member, _Matrix_eval, _Auto_eval };

    template <class _G>
    [[nodiscard]] static consteval _Choice_t<_St_ref> _Choose_ref() noexcept {
      static_assert(is_lvalue_reference_v<_G>);
      using _UnCV = remove_cvref_t<_G>;

      if constexpr (_Has_ref_ADL<_G, _UnCV>) {
        return {_St_ref::_Non_member,
                noexcept(_Fake_copy_init(contains_edge(declval<_G>(), declval<vertex_id_t<_G>>(),
                                                       declval<vertex_id_t<_G>>())))};
      } else if constexpr (_Can_matrix_eval<_G, _UnCV>) {
        return {_St_ref::_Matrix_eval,
                noexcept(_Fake_copy_init(declval<vertex_id_t<_G>>() < ranges::size(vertices(declval<_G>()))))};
      } else if constexpr (_Can_id_eval<_G, _UnCV>) {
        return {_St_ref::_Auto_eval,
                noexcept(_Fake_copy_init(
                      find_vertex_edge(declval<_G>(), declval<vertex_reference_t<_G>>(), declval<vertex_id_t<_G>>()) !=
                      declval<vertex_iterator_t<_G>>()))};
      } else {
        return {_St_ref::_None};
      }
    }

    template <class _G>
    static constexpr _Choice_t<_St_ref> _Choice_ref = _Choose_ref<_G>();

  public:
#line 1554 "graph/detail/graph_cpo.hpp"
    template <class _G>
    requires(_Choice_ref<_G&>._Strategy != _St_ref::_None)
    [[nodiscard]] constexpr auto operator()(_G&& __g, vertex_id_t<_G>& uid, vertex_id_t<_G>& vid) const
          noexcept(_Choice_ref<_G&>._No_throw) -> bool {
      constexpr _St_ref _Strat_ref = _Choice_ref<_G&>._Strategy;

      if constexpr (_Strat_ref == _St_ref::_Non_member) {
        return contains_edge(__g, uid, vid);
      } else if constexpr (_Strat_ref == _St_ref::_Matrix_eval) {
        return uid < ranges::size(vertices(__g)) && vid < ranges::size(vertices(__g));
      } else if constexpr (_Strat_ref == _St_ref::_Auto_eval) {
        auto ui = find_vertex(__g, uid);
        return find_vertex_edge(__g, *ui, vid) != ranges::end(edges(__g, *ui));
      } else {
        static_assert(_Always_false<_G>,
                      "contains_edge(g,uv) is not defined, or find_vertex_(g,uid) and source_id(g,uv) are not defined");
      }
    }
  };
}

inline namespace _Cpos {
  inline constexpr _Contains_edge::_Cpo contains_edge;
}
#line 1584 "graph/detail/graph_cpo.hpp"
namespace _NumVertices {



  void num_vertices();


  template <class _G, class _UnCV>
  concept _Has_ref_member = requires(_G&& __g) {
    { _Fake_copy_init(__g.num_vertices(__g)) };
  };
  template <class _G, class _UnCV>
  concept _Has_ref_ADL = _Has_class_or_enum_type<_G>
                         && requires(_G&& __g) {
                              { _Fake_copy_init(num_vertices(__g)) };
                            };
  template <class _G, class _UnCV>
  concept _Can_ref_eval = ranges::sized_range<vertex_range_t<_G>>
                          && requires(_G&& __g) {
                               { _Fake_copy_init(ranges::size(vertices(__g))) };
                             };

  template <class _G, class _UnCV>
  concept _Has_id_ADL = _Has_class_or_enum_type<_G>
                        && requires(_G&& __g, partition_id_t<_G> pid) {
                             { _Fake_copy_init(num_vertices(__g, pid)) };
                           };
  template <class _G, class _UnCV>
  concept _Can_id_eval = ranges::sized_range<vertex_edge_range_t<_G>>
                         && requires(_G&& __g, partition_id_t<_G> pid) {
                              { _Fake_copy_init(ranges::size(vertices(__g, pid))) };
                            };

  class _Cpo {
  private:
    enum class _St_id { _None, _Non_member, _Auto_eval };
    enum class _St_ref { _None, _Member, _Non_member, _Auto_eval };

    template <class _G>
    [[nodiscard]] static consteval _Choice_t<_St_ref> _Choose_ref() noexcept {
      static_assert(is_lvalue_reference_v<_G>);
      using _UnCV = remove_cvref_t<_G>;

      if constexpr (_Has_ref_member<_G, _UnCV>) {
        return {_St_ref::_Member, noexcept(_Fake_copy_init(declval<_G>().num_vertices()))};
      } else if constexpr (_Has_ref_ADL<_G, _UnCV>) {
        return {_St_ref::_Non_member, noexcept(_Fake_copy_init(num_vertices(declval<_G>())))};
      } else if constexpr (_Can_ref_eval<_G, _UnCV>) {
        return {_St_ref::_Auto_eval, noexcept(_Fake_copy_init(ranges::size(vertices(declval<_G>()))))};
      } else {
        return {_St_ref::_None};
      }
    }

    template <class _G>
    static constexpr _Choice_t<_St_ref> _Choice_ref = _Choose_ref<_G>();

    template <class _G>
    [[nodiscard]] static consteval _Choice_t<_St_id> _Choose_id() noexcept {
      static_assert(is_lvalue_reference_v<_G>);
      using _UnCV = remove_cvref_t<_G>;

      if constexpr (_Has_id_ADL<_G, _UnCV>) {
        return {
              _St_id::_Non_member,
              noexcept(_Fake_copy_init(num_vertices(declval<_G>(), declval<partition_id_t<_G>>())))};
      } else if constexpr (_Can_id_eval<_G, _UnCV>) {
        return {_St_id::_Auto_eval, noexcept(_Fake_copy_init(ranges::size(
                                          vertices(declval<_G>(), declval<partition_id_t<_G>>()))))};
      } else {
        return {_St_id::_None};
      }
    }

    template <class _G>
    static constexpr _Choice_t<_St_id> _Choice_id = _Choose_id<_G>();

  public:
#line 1674 "graph/detail/graph_cpo.hpp"
    template <class _G>

    [[nodiscard]] constexpr auto operator()(_G&& __g, const partition_id_t<_G>& pid) const
          noexcept(_Choice_id<_G&>._No_throw) {
      constexpr _St_id _Strat_id = _Choice_id<_G&>._Strategy;
      static_assert(_Strat_id == _St_id::_Auto_eval);

      if constexpr (_Strat_id == _St_id::_Non_member) {
        return num_vertices(__g, pid);
      } else if constexpr (_Strat_id == _St_id::_Auto_eval) {
        return ranges::size(vertices(__g, pid));
      } else {
        static_assert(_Always_false<_G>,
                      "num_vertices(g,pid) is not defined and the default implementation cannot be evaluated");
      }
    }
#line 1702 "graph/detail/graph_cpo.hpp"
    template <class _G>
    requires(_Choice_ref<_G&>._Strategy != _St_ref::_None)
    [[nodiscard]] constexpr auto operator()(_G&& __g) const noexcept(_Choice_ref<_G&>._No_throw) {
      constexpr _St_ref _Strat_id = _Choice_ref<_G&>._Strategy;

      if constexpr (_Strat_id == _St_ref::_Member) {
        return __g.num_vertices();
      } else if constexpr (_Strat_id == _St_ref::_Non_member) {
        return num_vertices(__g);
      } else if constexpr (_Strat_id == _St_ref::_Auto_eval) {
        return ranges::size(vertices(__g));
      } else {
        static_assert(_Always_false<_G>,
                      "num_vertices(g) is not defined and the default implementation cannot be evaluated");
      }
    }
  };
}

inline namespace _Cpos {
  inline constexpr _NumVertices::_Cpo num_vertices;
}
#line 1729 "graph/detail/graph_cpo.hpp"
namespace _Degree {



  void degree();


  template <class _G, class _UnCV>
  concept _Has_ref_member = requires(_G&& __g, vertex_reference_t<_G> u) {
    { _Fake_copy_init(u.degree(__g)) };
  };
  template <class _G, class _UnCV>
  concept _Has_ref_ADL = _Has_class_or_enum_type<_G>
                         && requires(_G&& __g, const vertex_reference_t<_G>& u) {
                              { _Fake_copy_init(degree(__g, u)) };
                            };
  template <class _G, class _UnCV>
  concept _Can_ref_eval = ranges::sized_range<vertex_edge_range_t<_G>>
                          && requires(_G&& __g, vertex_reference_t<_G> u) {
                               { _Fake_copy_init(edges(__g, u)) };
                             };

  template <class _G, class _UnCV>
  concept _Has_id_ADL = _Has_class_or_enum_type<_G>
                        && requires(_G&& __g, const vertex_id_t<_G>& uid) {
                             { _Fake_copy_init(degree(__g, uid)) };
                           };
  template <class _G, class _UnCV>
  concept _Can_id_eval = ranges::sized_range<vertex_edge_range_t<_G>>
                         && requires(_G&& __g, vertex_id_t<_G>& uid) {
                              { _Fake_copy_init(edges(__g, uid)) };
                            };

  class _Cpo {
  private:
    enum class _St_id { _None, _Non_member, _Auto_eval };
    enum class _St_ref { _None, _Member, _Non_member, _Auto_eval };

    template <class _G>
    [[nodiscard]] static consteval _Choice_t<_St_id> _Choose_id() noexcept {
      static_assert(is_lvalue_reference_v<_G>);
      using _UnCV = remove_cvref_t<_G>;

      if constexpr (_Has_id_ADL<_G, _UnCV>) {
        return {_St_id::_Non_member,
                noexcept(_Fake_copy_init(degree(declval<_G>(), declval<vertex_id_t<_G>>())))};
      } else if constexpr (_Can_id_eval<_G, _UnCV>) {
        return {_St_id::_Auto_eval, noexcept(_Fake_copy_init(ranges::size(
                                          edges(declval<_G>(), declval<vertex_id_t<_G>>()))))};
      } else {
        return {_St_id::_None};
      }
    }

    template <class _G>
    static constexpr _Choice_t<_St_id> _Choice_id = _Choose_id<_G>();

    template <class _G>
    [[nodiscard]] static consteval _Choice_t<_St_ref> _Choose_ref() noexcept {
      static_assert(is_lvalue_reference_v<_G>);
      using _UnCV = remove_cvref_t<_G>;

      if constexpr (_Has_ref_member<_G, _UnCV>) {
        return {_St_ref::_Member, noexcept(_Fake_copy_init(declval<vertex_reference_t<_G>>().degree(declval<_G>())))};
      } else if constexpr (_Has_ref_ADL<_G, _UnCV>) {
        return {_St_ref::_Non_member,
                noexcept(_Fake_copy_init(degree(declval<_G>(), declval<vertex_reference_t<_G>>())))};
      } else if constexpr (_Can_ref_eval<_G, _UnCV>) {
        return {_St_ref::_Auto_eval,
                noexcept(_Fake_copy_init(ranges::size(edges(declval<_G>(), declval<vertex_reference_t<_G>>()))))};
      } else {
        return {_St_ref::_None};
      }
    }

    template <class _G>
    static constexpr _Choice_t<_St_ref> _Choice_ref = _Choose_ref<_G>();

  public:
#line 1820 "graph/detail/graph_cpo.hpp"
    template <class _G>
    requires(_Choice_ref<_G&>._Strategy != _St_ref::_None)
    [[nodiscard]] constexpr auto operator()(_G&& __g, vertex_reference_t<_G> u) const
          noexcept(_Choice_ref<_G&>._No_throw) {
      constexpr _St_ref _Strat_ref = _Choice_ref<_G&>._Strategy;

      if constexpr (_Strat_ref == _St_ref::_Member) {
        return u.degree(__g);
      } else if constexpr (_Strat_ref == _St_ref::_Non_member) {
        return degree(__g, u);
      } else if constexpr (_Strat_ref == _St_ref::_Auto_eval) {
        return ranges::size(edges(__g, u));
      } else {
        static_assert(_Always_false<_G>,
                      "degree(g,u) is not defined and the default implementation cannot be evaluated");
      }
    }
#line 1850 "graph/detail/graph_cpo.hpp"
    template <class _G>
    requires(_Choice_id<_G&>._Strategy != _St_id::_None)
    [[nodiscard]] constexpr auto operator()(_G&& __g, const vertex_id_t<_G>& uid) const
          noexcept(_Choice_id<_G&>._No_throw) {
      constexpr _St_id _Strat_id = _Choice_id<_G&>._Strategy;

      if constexpr (_Strat_id == _St_id::_Non_member) {
        return degree(__g, uid);
      } else if constexpr (_Strat_id == _St_id::_Auto_eval) {
        return ranges::size(edges(__g, uid));
      } else {
        static_assert(_Always_false<_G>,
                      "degree(g,uid) is not defined and the default implementation cannot be evaluated");
      }
    }
  };
}

inline namespace _Cpos {
  inline constexpr _Degree::_Cpo degree;
}
#line 1878 "graph/detail/graph_cpo.hpp"
namespace _Vertex_value {



  void vertex_value();


  template <class _G, class _UnCV>
  concept _Has_ref_member = requires(_G&& __g, vertex_reference_t<_G> u) {
    { _Fake_copy_init(u.vertex_value(__g)) };
  };
  template <class _G, class _UnCV>
  concept _Has_ref_ADL = _Has_class_or_enum_type<_G>
                         && requires(_G&& __g, vertex_reference_t<_G> u) {
                              { _Fake_copy_init(vertex_value(__g, u)) };
                            };

  class _Cpo {
  private:
    enum class _St_ref { _None, _Member, _Non_member };

    template <class _G>
    [[nodiscard]] static consteval _Choice_t<_St_ref> _Choose_ref() noexcept {
      static_assert(is_lvalue_reference_v<_G>);
      using _UnCV = remove_cvref_t<_G>;

      if constexpr (_Has_ref_member<_G, _UnCV>) {
        return {_St_ref::_Member, noexcept(_Fake_copy_init(declval<vertex_reference_t<_G>>().vertex_value(
                                        declval<graph_reference_t<_G>>())))};
      } else if constexpr (_Has_ref_ADL<_G, _UnCV>) {
        return {_St_ref::_Non_member, noexcept(_Fake_copy_init(vertex_value(
                                            declval<_G>(), declval<vertex_reference_t<_G>>())))};
      } else {
        return {_St_ref::_None};
      }
    }

    template <class _G>
    static constexpr _Choice_t<_St_ref> _Choice_ref = _Choose_ref<_G>();

  public:
#line 1931 "graph/detail/graph_cpo.hpp"
    template <class _G>
    requires(_Choice_ref<_G&>._Strategy != _St_ref::_None)
    [[nodiscard]] constexpr auto operator()(_G&& __g, vertex_reference_t<_G> u) const
          noexcept(_Choice_ref<_G&>._No_throw) -> decltype(auto) {
      constexpr _St_ref _Strat_ref = _Choice_ref<_G&>._Strategy;

      if constexpr (_Strat_ref == _St_ref::_Member) {
        return u.vertex_value(__g);
      } else if constexpr (_Strat_ref == _St_ref::_Non_member) {
        return vertex_value(__g, u);
      } else {
        static_assert(_Always_false<_G>, "vertex_value(g,u) must be defined for the graph");
      }
    }
  };
}

inline namespace _Cpos {
  inline constexpr _Vertex_value::_Cpo vertex_value;
}

template <class G>
using vertex_value_t = decltype(vertex_value(declval<G&&>(), declval<vertex_reference_t<G>>()));
#line 1960 "graph/detail/graph_cpo.hpp"
namespace _Edge_value {



  void edge_value();


  template <class _G, class _UnCV>
  concept _Has_ref_member = requires(_G&& __g, edge_reference_t<_G> uv) {
    { _Fake_copy_init(uv.edge_value(__g)) };
  };
  template <class _G, class _UnCV>
  concept _Has_ref_ADL = _Has_class_or_enum_type<_G>
                         && requires(_G&& __g, edge_reference_t<_G> uv) {
                              { _Fake_copy_init(edge_value(__g, uv)) };
                            };
  template <class _G, class _UnCV>
  concept _Can_ref_eval =
        _Has_class_or_enum_type<_G> && ranges::forward_range<vertex_range_t<_G>>
        && requires(edge_reference_t<_G> uv) { uv; };

  class _Cpo {
  private:
    enum class _St_ref { _None, _Member, _Non_member, _Auto_eval };

    template <class _G>
    [[nodiscard]] static consteval _Choice_t<_St_ref> _Choose_ref() noexcept {
      static_assert(is_lvalue_reference_v<_G>);
      using _UnCV = remove_cvref_t<_G>;

      if constexpr (_Has_ref_member<_G, _UnCV>) {
        return {_St_ref::_Member, noexcept(_Fake_copy_init(
                                        declval<edge_reference_t<_G>>().edge_value(declval<graph_reference_t<_G>>())))};
      } else if constexpr (_Has_ref_ADL<_G, _UnCV>) {
        return {
              _St_ref::_Non_member,
              noexcept(_Fake_copy_init(edge_value(declval<_G>(), declval<edge_reference_t<_G>>())))};
      } else if constexpr (_Can_ref_eval<_G, _UnCV>) {
        return {_St_ref::_Auto_eval, noexcept(_Fake_copy_init(declval<edge_reference_t<_G>>()))};
      } else {
        return {_St_ref::_None};
      }
    }

    template <class _G>
    static constexpr _Choice_t<_St_ref> _Choice_ref = _Choose_ref<_G>();

  public:
#line 2020 "graph/detail/graph_cpo.hpp"
    template <class _G>
    requires(_Choice_ref<_G&>._Strategy != _St_ref::_None)
    [[nodiscard]] constexpr auto operator()(_G&& __g, edge_reference_t<_G> uv) const
          noexcept(_Choice_ref<_G&>._No_throw) -> decltype(auto) {
      constexpr _St_ref _Strat_ref = _Choice_ref<_G&>._Strategy;

      if constexpr (_Strat_ref == _St_ref::_Member) {
        return uv.edge_value(__g);
      } else if constexpr (_Strat_ref == _St_ref::_Non_member) {
        return edge_value(__g, uv);
      } else if constexpr (_Strat_ref == _St_ref::_Auto_eval) {
        return uv;
      } else {
        static_assert(_Always_false<_G>, "edge_value(g,uv) must be defined for the graph");
      }
    }
  };
}

inline namespace _Cpos {
  inline constexpr _Edge_value::_Cpo edge_value;
}



template <class G>
using edge_value_t = decltype(edge_value(declval<G&&>(), declval<edge_reference_t<G>>()));
#line 2053 "graph/detail/graph_cpo.hpp"
namespace _Graph_value {



  void graph_value();


  template <class _G, class _UnCV>
  concept _Has_ref_member = requires(_G&& __g) {
    { _Fake_copy_init(__g.graph_value()) };
  };
  template <class _G, class _UnCV>
  concept _Has_ref_ADL = _Has_class_or_enum_type<_G>
                         && requires(_G&& __g) {
                              { _Fake_copy_init(graph_value(__g)) };
                            };

  class _Cpo {
  private:
    enum class _St_ref { _None, _Member, _Non_member };

    template <class _G>
    [[nodiscard]] static consteval _Choice_t<_St_ref> _Choose_ref() noexcept {
      static_assert(is_lvalue_reference_v<_G>);
      using _UnCV = remove_cvref_t<_G>;

      if constexpr (_Has_ref_member<_G, _UnCV>) {
        return {_St_ref::_Member, noexcept(_Fake_copy_init(declval<graph_reference_t<_G>>().graph_value()))};
      } else if constexpr (_Has_ref_ADL<_G, _UnCV>) {
        return {_St_ref::_Non_member, noexcept(_Fake_copy_init(graph_value(declval<_G>())))};
      } else {
        return {_St_ref::_None};
      }
    }

    template <class _G>
    static constexpr _Choice_t<_St_ref> _Choice_ref = _Choose_ref<_G>();

  public:
#line 2104 "graph/detail/graph_cpo.hpp"
    template <class _G>
    requires(_Choice_ref<_G&>._Strategy != _St_ref::_None)
    [[nodiscard]] constexpr auto operator()(_G&& __g) const noexcept(_Choice_ref<_G&>._No_throw) -> decltype(auto) {
      constexpr _St_ref _Strat_ref = _Choice_ref<_G&>._Strategy;

      if constexpr (_Strat_ref == _St_ref::_Member) {
        return __g.graph_value();
      } else if constexpr (_Strat_ref == _St_ref::_Non_member) {
        return graph_value(__g);
      } else {
        static_assert(_Always_false<_G>, "graph_value(g) must be defined for the graph");
      }
    }
  };
}

inline namespace _Cpos {
  inline constexpr _Graph_value::_Cpo graph_value;
}


namespace edgelist {

  namespace _Edges {



    void edges();


    template <class EL, class _UnCV>
    concept _Has_ref_ADL = _Has_class_or_enum_type<EL>
                           && ranges::forward_range<EL> && requires(EL&& el) {
                                { _Fake_copy_init(edges(el)) };
                              };

    class _Cpo {
    private:
      enum class _St_ref { _None, _Non_member };

      template <class EL>
      [[nodiscard]] static consteval _Choice_t<_St_ref> _Choose_ref() noexcept {
        using _UnCV = remove_cvref_t<EL>;

        if constexpr (_Has_ref_ADL<EL, _UnCV>) {
          return {_St_ref::_Non_member, noexcept(_Fake_copy_init(edges(declval<EL>())))};
        } else {
          return {_St_ref::_None};
        }
      }

      template <class EL>
      static constexpr _Choice_t<_St_ref> _Choice_ref = _Choose_ref<EL>();

    public:
#line 2170 "graph/detail/graph_cpo.hpp"
      template <class EL>
      requires(_Choice_ref<EL&>._Strategy != _St_ref::_None)
      [[nodiscard]] constexpr auto&& operator()(EL&& el) const noexcept(_Choice_ref<EL&>._No_throw) {
        constexpr _St_ref _Strat_ref = _Choice_ref<EL&>._Strategy;

        if constexpr (_Strat_ref == _St_ref::_Non_member) {
          return el;
        } else {
          static_assert(_Always_false<EL>,
                        "edges(el) is not defined and the default implementation cannot be evaluated");
        }
      }
    };
  }

  inline namespace _Cpos {
    inline constexpr _Edges::_Cpo edges;
  }
#line 2199 "graph/detail/graph_cpo.hpp"
  template <class EL>
  using edgelist_range_t = decltype(edges(declval<EL&&>()));

  template <class EL>
  using edgelist_iterator_t = ranges::iterator_t<edgelist_range_t<EL&&>>;

  template <class EL>
  using edge_t = ranges::range_value_t<edgelist_range_t<EL>>;

  template <class EL>
  using edge_reference_t = ranges::range_reference_t<edgelist_range_t<EL>>;

  namespace _Source_id {



    void source_id();


    template <class _G, class _UnCV>
    concept _Has_ref_member = requires(_G&& __g, edge_reference_t<_G> uv) {
      { _Fake_copy_init(uv.source_id(__g)) };
    };
    template <class _G, class _UnCV>
    concept _Has_ref_ADL = _Has_class_or_enum_type<_G>
                           && requires(_G&& __g, const edge_reference_t<_G>& uv) {
                                { _Fake_copy_init(source_id(__g, uv)) };
                              };

    class _Cpo {
    private:
      enum class _St_ref { _None, _Member, _Non_member, _Auto_eval };

      template <class _G>
      [[nodiscard]] static consteval _Choice_t<_St_ref> _Choose_ref() noexcept {
        static_assert(is_lvalue_reference_v<_G>);
        using _UnCV = remove_cvref_t<_G>;

        if constexpr (_Has_ref_member<_G, _UnCV>) {
          return {_St_ref::_Member,
                  noexcept(_Fake_copy_init(declval<edge_reference_t<_G>>().source_id(declval<_G>())))};
        } else if constexpr (_Has_ref_ADL<_G, _UnCV>) {
          return {_St_ref::_Non_member, noexcept(_Fake_copy_init(source_id(
                                              declval<_G>(), declval<edge_reference_t<_G>>())))};
        } else {
          return {_St_ref::_None};
        }
      }

      template <class _G>
      static constexpr _Choice_t<_St_ref> _Choice_ref = _Choose_ref<_G>();

    public:
#line 2264 "graph/detail/graph_cpo.hpp"
      template <class _G>
      requires(_Choice_ref<_G&>._Strategy != _St_ref::_None)
      [[nodiscard]] constexpr auto operator()(_G&& __g, edge_reference_t<_G> uv) const
            noexcept(_Choice_ref<_G&>._No_throw) {
        constexpr _St_ref _Strat_ref = _Choice_ref<_G&>._Strategy;

        if constexpr (_Strat_ref == _St_ref::_Member) {
          return uv.source_id(__g);
        } else if constexpr (_Strat_ref == _St_ref::_Non_member) {
          return source_id(__g, uv);
        } else {
          static_assert(_Always_false<_G>, "source_id(g,uv) or g.source_id(uv) is not defined");
        }
      }
    };
  }

  inline namespace _Cpos {
    inline constexpr _Source_id::_Cpo source_id;
  }

  template <class EL>
  using source_id_t = decltype(source_id(declval<EL&&>(), declval<edge_reference_t<EL>>()));

  namespace tag_invoke {
    static struct target_id_fn_t final : ::std::tag<target_id_fn_t> { } const&                 target_id = ::std::tag_invoke_v(target_id_fn_t{});
  }

  template <class EL>
  auto target_id(EL&& el, edge_reference_t<EL> uv) {
    return tag_invoke::target_id(el, uv);
  }

  template <class EL>
  using target_id_t = decltype(target_id(declval<EL&&>(), declval<edge_reference_t<EL>>()));


  namespace tag_invoke {
    static struct edge_value_fn_t final : ::std::tag<edge_value_fn_t> { } const&                 edge_value = ::std::tag_invoke_v(edge_value_fn_t{});
  }

  template <class EL>
  auto edge_value(EL&& el, edge_reference_t<EL> uv) {
    return tag_invoke::edge_value(el, uv);
  }

  template <class EL>
  using edge_value_t = decltype(edge_value(declval<EL&&>(), declval<edge_reference_t<EL>>()));
}
#line 2317 "graph/detail/graph_cpo.hpp"
namespace _Partition_count {



  void partition_count();


  template <class _G, class _UnCV>
  concept _Has_ref_member = requires(_G&& __g, vertex_reference_t<_G> u) {
    { _Fake_copy_init(__g.partition_count()) };
  };
  template <class _G, class _UnCV>
  concept _Has_ref_ADL = _Has_class_or_enum_type<_G>
                         && requires(_G&& __g) {
                              { _Fake_copy_init(partition_count(__g)) };
                            };
  template <class _G, class _UnCV>
  concept _Can_ref_eval = integral<vertex_id_t<_G>>
                          && requires(_G&& __g) {
                               { _Fake_copy_init(vertex_id_t<_G>(1)) };
                             };

  class _Cpo {
  private:
    enum class _St_ref { _None, _Member, _Non_member, _Auto_eval };

    template <class _G>
    [[nodiscard]] static consteval _Choice_t<_St_ref> _Choose_ref() noexcept {
      static_assert(is_lvalue_reference_v<_G>);
      using _UnCV = remove_cvref_t<_G>;

      if constexpr (_Has_ref_member<_G, _UnCV>) {
        return {_St_ref::_Member, noexcept(_Fake_copy_init(declval<_G>().partition_count()))};
      } else if constexpr (_Has_ref_ADL<_G, _UnCV>) {
        return {_St_ref::_Non_member, noexcept(_Fake_copy_init(partition_count(
                                            declval<_G>(), declval<vertex_reference_t<_G>>())))};
      } else if constexpr (_Can_ref_eval<_G, _UnCV>) {
        return {_St_ref::_Auto_eval, noexcept(_Fake_copy_init(vertex_id_t<_G>(1)))};
      } else {
        return {_St_ref::_None};
      }
    }

    template <class _G>
    static constexpr _Choice_t<_St_ref> _Choice_ref = _Choose_ref<_G>();

  public:
#line 2375 "graph/detail/graph_cpo.hpp"
    template <class _G>
    requires(_Choice_ref<_G&>._Strategy != _St_ref::_None)
    [[nodiscard]] constexpr auto operator()(_G&& __g) const noexcept(_Choice_ref<_G&>._No_throw) {
      constexpr _St_ref _Strat_ref = _Choice_ref<_G&>._Strategy;

      if constexpr (_Strat_ref == _St_ref::_Member) {
        return __g.partition_count();
      } else if constexpr (_Strat_ref == _St_ref::_Non_member) {
        return partition_count(__g);
      } else if constexpr (_Strat_ref == _St_ref::_Auto_eval) {
        return vertex_id_t<_G>(1);
      } else {
        static_assert(_Always_false<_G>,
                      "partition_count(g) is not defined and the default implementation cannot be evaluated");
      }
    }
  };
}

inline namespace _Cpos {
  inline constexpr _Partition_count::_Cpo partition_count;
}
#line 2402 "graph/detail/graph_cpo.hpp"
namespace tag_invoke {


  template <class G>
  concept _has_vertices_pid_adl = requires(G&& g, partition_id_t<G> pid) {
    { vertices(g, pid) };
  };
}
#line 2441 "graph/detail/graph_cpo.hpp"
template <class G>
struct _partition_vertex_id {
  partition_id_t<G> partition_id;
  vertex_id_t<G>    vertex_id;
};

template <class G>
using partition_vertex_id_t = _partition_vertex_id<G>;
#line 2458 "graph/detail/graph_cpo.hpp"
namespace _Partition_vertex_id {



  void partition_vertex_id();


  template <class _G>
  concept _Has_UId_member = requires(_G&& __g, vertex_id_t<_G> uid) {
    { _Fake_copy_init(__g.partition_vertex_id(uid)) };
  };

  template <class _G>
  concept _Has_UId_ADL = _Has_class_or_enum_type<_G>
                         && requires(_G&& __g, vertex_id_t<_G> uid) {
                              { _Fake_copy_init(partition_vertex_id(__g, uid)) };
                            };

  template <class _G>
  concept _Has_UIter_member = requires(_G&& __g, vertex_iterator_t<_G> ui) {
    { _Fake_copy_init(__g.partition_vertex_id(ui)) };
  };

  template <class _G>
  concept _Has_UIter_ADL = _Has_class_or_enum_type<_G>
                           && requires(_G&& __g, vertex_iterator_t<_G> ui) {
                                { _Fake_copy_init(partition_vertex_id(__g, ui)) };
                              };

  class _Cpo {
  private:
    enum class _StId { _None, _Member, _Non_member };
    enum class _StIter { _None, _Member, _Non_member };

    template <class _G>
    [[nodiscard]] static consteval _Choice_t<_StId> _ChooseId() noexcept {
      static_assert(is_lvalue_reference_v<_G>);


      if constexpr (_Has_UId_member<_G>) {
        return {_StId::_Member,
                noexcept(_Fake_copy_init(declval<_G>().partition_vertex_id(declval<vertex_id_t<_G>>())))};
      } else if constexpr (_Has_UId_ADL<_G>) {
        return {_StId::_Non_member, noexcept(_Fake_copy_init(partition_vertex_id(
                                          declval<_G>(), declval<vertex_id_t<_G>>())))};
      } else {
        return {_StId::_None};
      }
    }

    template <class _G>
    [[nodiscard]] static consteval _Choice_t<_StIter> _ChooseIter() noexcept {
      static_assert(is_lvalue_reference_v<_G>);


      if constexpr (_Has_UIter_member<_G>) {
        return {_StIter::_Member,
                noexcept(_Fake_copy_init(declval<_G>().partition_vertex_id(declval<vertex_iterator_t<_G>>())))};
      } else if constexpr (_Has_UIter_ADL<_G>) {
        return {_StIter::_Non_member, noexcept(_Fake_copy_init(partition_vertex_id(
                                            declval<_G>(), declval<vertex_iterator_t<_G>>())))};
      } else {
        return {_StIter::_None};
      }
    }

    template <class _G>
    static constexpr _Choice_t<_StId> _ChoiceId = _ChooseId<_G>();
    template <class _G>
    static constexpr _Choice_t<_StIter> _ChoiceIter = _ChooseIter<_G>();

  public:
#line 2548 "graph/detail/graph_cpo.hpp"
    template <class _G>
    requires(_ChoiceId<_G&>._Strategy != _StId::_None)
    [[nodiscard]] constexpr auto operator()(_G&& __g, vertex_id_t<_G> uid) const noexcept(_ChoiceId<_G&>._No_throw) {
      constexpr _StId _Strat = _ChoiceId<_G&>._Strategy;

      if constexpr (_Strat == _StId::_Member) {
        return __g.partition_vertex_id(uid);
      } else if constexpr (_Strat == _StId::_Non_member) {
        return partition_vertex_id(__g, uid);
      } else {
        return partition_vertex_id_t<_G>{0, uid};
      }
    }
#line 2580 "graph/detail/graph_cpo.hpp"
    template <class _G>
    requires(_ChoiceIter<_G&>._Strategy != _StIter::_None)
    [[nodiscard]] constexpr auto operator()(_G&& __g, vertex_iterator_t<_G> ui) const
          noexcept(_ChoiceIter<_G&>._No_throw) {
      constexpr _StIter _Strat = _ChoiceIter<_G&>._Strategy;

      if constexpr (_Strat == _StIter::_Member) {
        return __g.partition_vertex_id(ui);
      } else if constexpr (_Strat == _StIter::_Non_member) {
        return partition_vertex_id(__g, ui);
      } else {
        return (*this)(__g, vertex_id(__g, ui));
      }
    }
  };
}

inline namespace _Cpos {
  inline constexpr _Partition_vertex_id::_Cpo partition_vertex_id;
}
#line 2606 "graph/detail/graph_cpo.hpp"
namespace _Find_partition_vertex {



  void find_partition_vertex();


  template <class _G>
  concept _Has_UId_member = requires(_G&& __g, partition_vertex_id_t<_G> puid) {
    { _Fake_copy_init(__g.find_partition_vertex(puid)) };
  };

  template <class _G>
  concept _Has_UId_ADL = _Has_class_or_enum_type<_G>
                         && requires(_G&& __g, partition_vertex_id_t<_G> puid) {
                              { _Fake_copy_init(find_partition_vertex(__g, puid)) };
                            };

  class _Cpo {
  private:
    enum class _StId { _None, _Member, _Non_member };

    template <class _G>
    [[nodiscard]] static consteval _Choice_t<_StId> _ChooseId() noexcept {
      static_assert(is_lvalue_reference_v<_G>);


      if constexpr (_Has_UId_member<_G>) {
        return {_StId::_Member,
                noexcept(_Fake_copy_init(declval<_G>().find_partition_vertex(declval<partition_vertex_id_t<_G>>())))};
      } else if constexpr (_Has_UId_ADL<_G>) {
        return {_StId::_Non_member, noexcept(_Fake_copy_init(find_partition_vertex(
                                          declval<_G>(), declval<partition_vertex_id_t<_G>>())))};
      } else {
        return {_StId::_None};
      }
    }

    template <class _G>
    static constexpr _Choice_t<_StId> _ChoiceId = _ChooseId<_G>();

  public:
#line 2666 "graph/detail/graph_cpo.hpp"
    template <class _G>
    requires(_ChoiceId<_G&>._Strategy != _StId::_None)
    [[nodiscard]] constexpr auto operator()(_G&& __g, partition_vertex_id_t<_G> puid) const
          noexcept(_ChoiceId<_G&>._No_throw) {
      constexpr _StId _Strat = _ChoiceId<_G&>._Strategy;

      if constexpr (_Strat == _StId::_Member) {
        return __g.find_partition_vertex(puid);
      } else if constexpr (_Strat == _StId::_Non_member) {
        return find_partition_vertex(__g, puid);
      } else {
        return find_vertex(__g, puid.vertex_id);
      }
    }
  };
}

inline namespace _Cpos {
  inline constexpr _Find_partition_vertex::_Cpo find_partition_vertex;
}


template <class G>
using partition_edge_range_t = vertex_edge_range_t<G>;
#line 2764 "graph/detail/graph_cpo.hpp"
namespace _Partition_target_id {



  void partition_target_id();


  template <class _G>
  concept _Has_UVRef__member = requires(_G&& __g, edge_reference_t<_G> uv) {
    { _Fake_copy_init(__g.partition_target_id(uv)) };
  };

  template <class _G>
  concept _Has_UVRef__ADL = _Has_class_or_enum_type<_G>
                            && requires(_G&& __g, edge_reference_t<_G> uv) {
                                 { _Fake_copy_init(partition_target_id(__g, uv)) };
                               };

  class _Cpo {
  private:
    enum class _StRef { _None, _Member, _Non_member };

    template <class _G>
    [[nodiscard]] static consteval _Choice_t<_StRef> _ChooseRef() noexcept {
      static_assert(is_lvalue_reference_v<_G>);


      if constexpr (_Has_UVRef__member<_G>) {
        return {_StRef::_Member,
                noexcept(_Fake_copy_init(declval<_G>().partition_target_id(declval<edge_reference_t<_G>>())))};
      } else if constexpr (_Has_UVRef__ADL<_G>) {
        return {_StRef::_Non_member, noexcept(_Fake_copy_init(partition_target_id(
                                           declval<_G>(), declval<edge_reference_t<_G>>())))};
      } else {
        return {_StRef::_None};
      }
    }

    template <class _G>
    static constexpr _Choice_t<_StRef> _ChoiceId = _ChooseRef<_G>();

  public:
#line 2824 "graph/detail/graph_cpo.hpp"
    template <class _G>
    requires(_ChoiceId<_G&>._Strategy != _StRef::_None)
    [[nodiscard]] constexpr auto operator()(_G&& __g, edge_reference_t<_G> uv) const
          noexcept(_ChoiceId<_G&>._No_throw) {
      constexpr _StRef _Strat = _ChoiceId<_G&>._Strategy;

      if constexpr (_Strat == _StRef::_Member) {
        return __g.partition_target_id(uv);
      } else if constexpr (_Strat == _StRef::_Non_member) {
        return partition_target_id(__g, uv);
      } else {
        return partition_vertex_id_t<_G>{0, target_id(__g, uv)};
      }
    }
  };
}

inline namespace _Cpos {
  inline constexpr _Partition_target_id::_Cpo partition_target_id;
}
#line 2850 "graph/detail/graph_cpo.hpp"
namespace _Partition_source_id {



  void partition_source_id();


  template <class _G>
  concept _Has_UVRef__member = requires(_G&& __g, edge_reference_t<_G> uv) {
    { _Fake_copy_init(__g.partition_source_id(uv)) };
  };

  template <class _G>
  concept _Has_UVRef__ADL = _Has_class_or_enum_type<_G>
                            && requires(_G&& __g, edge_reference_t<_G> uv) {
                                 { _Fake_copy_init(partition_source_id(__g, uv)) };
                               };

  class _Cpo {
  private:
    enum class _StRef { _None, _Member, _Non_member };

    template <class _G>
    [[nodiscard]] static consteval _Choice_t<_StRef> _ChooseRef() noexcept {
      static_assert(is_lvalue_reference_v<_G>);


      if constexpr (_Has_UVRef__member<_G>) {
        return {_StRef::_Member,
                noexcept(_Fake_copy_init(declval<_G>().partition_source_id(declval<edge_reference_t<_G>>())))};
      } else if constexpr (_Has_UVRef__ADL<_G>) {
        return {_StRef::_Non_member, noexcept(_Fake_copy_init(partition_source_id(
                                           declval<_G>(), declval<edge_reference_t<_G>>())))};
      } else {
        return {_StRef::_None};
      }
    }

    template <class _G>
    static constexpr _Choice_t<_StRef> _ChoiceId = _ChooseRef<_G>();

  public:
#line 2910 "graph/detail/graph_cpo.hpp"
    template <class _G>
    requires(_ChoiceId<_G&>._Strategy != _StRef::_None)
    [[nodiscard]] constexpr auto operator()(_G&& __g, edge_reference_t<_G> uv) const
          noexcept(_ChoiceId<_G&>._No_throw) {
      constexpr _StRef _Strat = _ChoiceId<_G&>._Strategy;

      if constexpr (_Strat == _StRef::_Member) {
        return __g.partition_source_id(uv);
      } else if constexpr (_Strat == _StRef::_Non_member) {
        return partition_source_id(__g, uv);
      } else {
        return partition_vertex_id_t<_G>{0, source_id(__g, uv)};
      }
    }
  };
}

inline namespace _Cpos {
  inline constexpr _Partition_source_id::_Cpo partition_source_id;
}


}
#line 60 "graph/graph.hpp"
namespace std::graph {
#line 86 "graph/graph.hpp"
template <class E>
inline constexpr bool is_undirected_edge_v = false;
#line 106 "graph/graph.hpp"
template <class G>
concept _common_vertex_range = ranges::sized_range<vertex_range_t<G>> &&
                               requires(G&& g, vertex_iterator_t<G> ui) { vertex_id(g, ui); };

template <class G>
concept vertex_range = _common_vertex_range<G> && ranges::forward_range<vertex_range_t<G>>;

template <class G>
concept index_vertex_range = _common_vertex_range<G> &&
                             ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>;
#line 127 "graph/graph.hpp"
template <class G, class E>
concept targeted_edge = requires(G&& g, edge_reference_t<G> uv) {
  target_id(g, uv);
  target(g, uv);
};
#line 142 "graph/graph.hpp"
template <class G, class E>
concept sourced_edge = requires(G&& g, E& uv) {
  source_id(g, uv);
  source(g, uv);
};
#line 156 "graph/graph.hpp"
template <class G, class E>
struct is_sourced_edge : public integral_constant<bool, sourced_edge<G, E>> {};
#line 167 "graph/graph.hpp"
template <class G, class E>
inline constexpr bool is_sourced_edge_v = is_sourced_edge<G, E>::value;
#line 179 "graph/graph.hpp"
template <class G>
concept basic_adjacency_list = vertex_range<G> && targeted_edge<G, edge_t<G>> && requires(G&& g, vertex_id_t<G>& uid) {
  { edges(g, uid) } -> ranges::forward_range;
};

template <class G>
concept basic_index_adjacency_list =
      index_vertex_range<G> && targeted_edge<G, edge_t<G>> && requires(G&& g, vertex_id_t<G>& uid) {
        { edges(g, uid) } -> ranges::forward_range;
      };
#line 198 "graph/graph.hpp"
template <class G>
concept adjacency_list = basic_adjacency_list<G> && requires(G&& g, vertex_reference_t<G> u) {
  { edges(g, u) } -> ranges::forward_range;
};

template <class G>
concept index_adjacency_list = basic_index_adjacency_list<G> && requires(G&& g, vertex_reference_t<G> u) {
  { edges(g, u) } -> ranges::forward_range;
};
#line 225 "graph/graph.hpp"
template <class G>
concept sourced_adjacency_list =
      adjacency_list<G> && sourced_edge<G, edge_t<G>> && requires(G&& g, edge_reference_t<G> uv) { edge_id(g, uv); };
#line 249 "graph/graph.hpp"
template <class G>
concept has_degree = requires(G&& g, vertex_reference_t<G> u) {
  { degree(g, u) };
};
#line 266 "graph/graph.hpp"
template <class G>
concept has_find_vertex = requires(G&& g, vertex_id_t<G> uid) {
  { find_vertex(g, uid) } -> forward_iterator;
};
#line 279 "graph/graph.hpp"
template <class G>
concept has_find_vertex_edge = requires(G&& g, vertex_id_t<G> uid, vertex_id_t<G> vid, vertex_reference_t<G> u) {
  { find_vertex_edge(g, u, vid) } -> forward_iterator;
  { find_vertex_edge(g, uid, vid) } -> forward_iterator;
};
#line 293 "graph/graph.hpp"
template <class G>
concept has_contains_edge = requires(G&& g, vertex_id_t<G> uid, vertex_id_t<G> vid) {
  { contains_edge(g, uid, vid) } -> convertible_to<bool>;
};
#line 333 "graph/graph.hpp"
template <class E>
struct define_unordered_edge : public false_type {};

template <class G, class E>
struct is_unordered_edge : public conjunction<define_unordered_edge<E>, is_sourced_edge<G, E>> {};

template <class G, class E>
inline constexpr bool is_unordered_edge_v = is_unordered_edge<G, E>::value;

template <class G, class E>
concept unordered_edge = is_unordered_edge_v<G, E>;
#line 348 "graph/graph.hpp"
template <class G, class E>
struct is_ordered_edge : public negation<is_unordered_edge<G, E>> {};

template <class G, class E>
inline constexpr bool is_ordered_edge_v = is_ordered_edge<G, E>::value;

template <class G, class E>
concept ordered_edge = is_ordered_edge_v<G, E>;
#line 361 "graph/graph.hpp"
class graph_error : public runtime_error {
public:
  explicit graph_error(const string& what_arg) : runtime_error(what_arg) {}
  explicit graph_error(const char* what_arg) : runtime_error(what_arg) {}
};

}
#line 3 "graph/graph_utility.hpp"
namespace std::graph {


enum three_colors : int8_t { black, white, grey };
enum struct cancel_search : int8_t { continue_search, cancel_branch, cancel_all };


template <class G, bool Sourced>
class source_vertex {
public:
  using vertex_id_type = vertex_id_t<G>;

  source_vertex(vertex_id_type id) : id_(id) {}

  source_vertex()                         = default;
  source_vertex(const source_vertex& rhs) = default;
  source_vertex(source_vertex&&)          = default;
  ~source_vertex()                        = default;

  source_vertex& operator=(const source_vertex&) = default;
  source_vertex& operator=(source_vertex&&)      = default;

  constexpr vertex_id_type source_vertex_id() const noexcept { return id_; }

protected:
  vertex_id_type id_ = 0;
};

template <class G>
class source_vertex<G, false> {
public:
  using vertex_id_type = vertex_id_t<G>;

  source_vertex(vertex_id_type id) {}
  source_vertex() = default;
};
#line 52 "graph/graph_utility.hpp"
namespace _detail {
  template <class T>
  class ref_to_ptr {
  public:
    static_assert(is_object_v<T> || is_function_v<T>,
                  "ref_to_ptr<T> requires T to be an object type or a function type.");

    using type = T;

    constexpr ref_to_ptr() = default;
    constexpr ref_to_ptr(const T& rhs) : value(rhs) {}
    constexpr ~ref_to_ptr() = default;

    constexpr ref_to_ptr& operator=(const T& rhs) {
      value = rhs;
      return *this;
    }

    constexpr operator bool() const noexcept { return true; }

    constexpr T*       get() noexcept { return &value; }
    constexpr const T* get() const noexcept { return &value; }

    constexpr operator T&() noexcept { return value; }
    constexpr operator const T&() const noexcept { return value; }

  private:
    T value = {};
  };

  template <class T>
  class ref_to_ptr<T&> {
  public:
    static_assert(is_object_v<T> || is_function_v<T>,
                  "ref_to_ptr<T> requires T to be an object type or a function type.");
    using type = T;

    constexpr ref_to_ptr() = default;
    constexpr ref_to_ptr(T& rhs) noexcept : value(&rhs) {}
    constexpr ~ref_to_ptr() = default;

    ref_to_ptr& operator=(T& rhs) noexcept {
      value = &rhs;
      return *this;
    }

    constexpr T*       get() noexcept { return value; }
    constexpr const T* get() const noexcept { return value; }

    constexpr operator bool() const noexcept { return value != nullptr; }

    constexpr operator T&() noexcept {
      assert(value);
      return *value;
    }
    constexpr operator const T&() const noexcept {
      assert(value);
      return *value;
    }

  private:
    T* value = nullptr;
  };

  template <class T>
  class ref_to_ptr<T*> {
  public:
    static_assert(is_object_v<T> || is_function_v<T>,
                  "ref_to_ptr<T> requires T to be an object type or a function type.");
    using type = T;

    constexpr ref_to_ptr() = default;
    constexpr ref_to_ptr(T* rhs) noexcept : value(rhs) {}
    constexpr ~ref_to_ptr() = default;

    constexpr ref_to_ptr& operator=(T* rhs) noexcept {
      value = rhs;
      return *this;
    }

    constexpr T*       get() noexcept { return value; }
    constexpr const T* get() const noexcept { return value; }

    constexpr operator bool() const noexcept { return value != nullptr; }

    constexpr operator T&() noexcept {
      assert(value);
      return *value;
    }
    constexpr operator const T&() const noexcept {
      assert(value);
      return *value;
    }

  private:
    T* value = nullptr;
  };


  template <class A>
  concept is_allocator_v = is_copy_constructible_v<A> && requires(A alloc, size_t n) {
    { alloc.allocate(n) };
  };


}


}
#line 34 "graph/views/breadth_first_search.hpp"
#include <queue>
#include <vector>
#include <functional>




namespace std::graph {
#line 49 "graph/views/breadth_first_search.hpp"
template <adjacency_list G, class Alloc>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>
class bfs_base : public ranges::view_base {
public:
  using graph_type       = remove_reference_t<G>;
  using vertex_type      = vertex_t<G>;
  using vertex_id_type   = vertex_id_t<graph_type>;
  using vertex_reference = vertex_reference_t<graph_type>;
  using vertex_iterator  = vertex_iterator_t<graph_type>;
  using edge_type        = edge_t<G>;
  using edge_reference   = edge_reference_t<G>;
  using edge_iterator    = vertex_edge_iterator_t<graph_type>;

private:
  using graph_ref_type = reference_wrapper<graph_type>;
  using Queue          = queue<vertex_id_t<G>>;

  using queue_elem = vertex_id_type;

  using parent_alloc = typename allocator_traits<typename Queue::container_type::allocator_type>::template rebind_alloc<
        vertex_id_type>;

public:
  bfs_base(graph_type& g, vertex_id_type seed, const Alloc& alloc)
        : graph_(&g), Q_(alloc), colors_(ranges::size(vertices(g)), white, alloc) {
    if (seed < ranges::size(vertices(*graph_)) && !ranges::empty(edges(*graph_, seed))) {
      uv_ = ranges::begin(edges(*graph_, seed));
      Q_.push(queue_elem{seed});
      colors_[seed] = grey;
    }
  }

  template <class VKR>
  requires ranges::input_range<VKR> && convertible_to<ranges::range_value_t<VKR>, vertex_id_t<G>>
  bfs_base(graph_type& g, const VKR& seeds = 0) : graph_(&g), colors_(ranges::size(vertices(g)), white) {
    for (auto&& [seed] : seeds) {
      if (seed < ranges::size(vertices(*graph_)) && !ranges::empty(edges(*graph_, seed))) {
        if (Q_.empty()) {
          uv_ = ranges::begin(edges(*graph_, seed));
        }
        Q_.push(queue_elem{seed});
        colors_[seed] = grey;
      }
    }

    while (!Q_.empty()) {
      auto          u_id = Q_.front();
      edge_iterator uvi  = find_unvisited(u_id, ranges::begin(edges(*graph_, u_id)));
      if (uvi != ranges::end(edges(*graph_, u_id))) {
        uv_ = uvi;
        break;
      } else {
        Q_.pop();
        colors_[u_id] = black;
      }
    }
  }

  bfs_base()                = default;
  bfs_base(const bfs_base&) = delete;
  bfs_base(bfs_base&&)      = default;
  ~bfs_base()               = default;

  bfs_base& operator=(const bfs_base&) = delete;
  bfs_base& operator=(bfs_base&&)      = default;

  constexpr bool empty() const noexcept { return Q_.empty(); }

  constexpr auto size() const noexcept { return Q_.size(); }


  constexpr void          cancel(cancel_search cancel_type) noexcept { cancel_ = cancel_type; }
  constexpr cancel_search canceled() noexcept { return cancel_; }

protected:
  constexpr vertex_id_type real_target_id(edge_reference uv, vertex_id_type) const
  requires ordered_edge<G, edge_type>
  {
    return target_id(*graph_, uv);
  }
  constexpr vertex_id_type real_target_id(edge_reference uv, vertex_id_type src) const
  requires unordered_edge<G, edge_type>
  {
    if (target_id(*graph_, uv) != src)
      return target_id(*graph_, uv);
    else
      return source_id((*graph_), uv);
  }

  constexpr vertex_edge_iterator_t<G> find_unvisited(vertex_id_t<G> uid, vertex_edge_iterator_t<G> first) {
    return ranges::find_if(first, ranges::end(edges(*graph_, uid)), [this, uid](edge_reference uv) -> bool {
      return colors_[real_target_id(uv, uid)] == white;
    });
  }

  void advance() {

    auto           u_id = Q_.front();
    vertex_id_type v_id = real_target_id(*uv_, u_id);

    switch (cancel_) {
    case cancel_search::continue_search:
      Q_.push(queue_elem{v_id});
      colors_[v_id] = grey;
      uv_           = find_unvisited(u_id, ++uv_);
      break;
    case cancel_search::cancel_branch:
      cancel_       = cancel_search::continue_search;
      colors_[v_id] = black;
      uv_           = find_unvisited(u_id, ++uv_);
      break;
    case cancel_search::cancel_all:
      while (!Q_.empty())
        Q_.pop();
      return;
    }


    if (uv_ == ranges::end(edges(*graph_, u_id))) {
      colors_[u_id] = black;
      Q_.pop();
      while (!Q_.empty()) {
        u_id = Q_.front();
        uv_  = find_unvisited(u_id, ranges::begin(edges(*graph_, u_id)));
        if (uv_ != ranges::end(edges(*graph_, u_id))) {
          break;
        } else {
          Q_.pop();
          colors_[u_id] = black;
        }
      }
    }
  }

protected:
  graph_type*               graph_ = nullptr;
  Queue                     Q_;
  vertex_edge_iterator_t<G> uv_;
  vector<three_colors>      colors_;
  cancel_search             cancel_ = cancel_search::continue_search;
};
#line 198 "graph/views/breadth_first_search.hpp"
template <adjacency_list G, class VVF = void, class Alloc = allocator<bool>>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>
class vertices_breadth_first_search_view : public bfs_base<G, Alloc> {
public:
  using base_type        = bfs_base<G, Alloc>;
  using graph_type       = G;
  using vertex_type      = vertex_t<G>;
  using vertex_id_type   = vertex_id_t<graph_type>;
  using vertex_reference = vertex_reference_t<graph_type>;
  using vertex_iterator  = vertex_iterator_t<graph_type>;
  using edge_type        = edge_t<G>;
  using edge_reference   = edge_reference_t<G>;
  using edge_iterator    = vertex_edge_iterator_t<graph_type>;
  using bfs_range_type   = vertices_breadth_first_search_view<graph_type, VVF, Alloc>;

  using vertex_value_func = remove_reference_t<VVF>;
  using vertex_value_type = invoke_result_t<VVF, vertex_reference>;

public:
  vertices_breadth_first_search_view(graph_type&    g,
                                     vertex_id_type seed,
                                     const VVF&     value_fn,
                                     const Alloc&   alloc = Alloc())
        : base_type(g, seed, alloc), value_fn_(&value_fn) {}
  template <class VKR>
  requires ranges::input_range<VKR> && convertible_to<ranges::range_value_t<VKR>, vertex_id_t<G>>
  vertices_breadth_first_search_view(graph_type&  graph,
                                     const VKR&   seeds,
                                     const VVF&   value_fn,
                                     const Alloc& alloc = Alloc())
        : base_type(graph, seeds), value_fn_(&value_fn) {}

  vertices_breadth_first_search_view()                                          = default;
  vertices_breadth_first_search_view(const vertices_breadth_first_search_view&) = delete;
  vertices_breadth_first_search_view(vertices_breadth_first_search_view&&)      = default;
  ~vertices_breadth_first_search_view()                                         = default;

  vertices_breadth_first_search_view& operator=(const vertices_breadth_first_search_view&) = delete;
  vertices_breadth_first_search_view& operator=(vertices_breadth_first_search_view&&)      = default;

public:
  class iterator;
  struct end_sentinel {
    bool operator==(const iterator& rhs) const noexcept { return rhs.the_range_->Q_.empty(); }
  };

  class iterator {
  public:
    using iterator_category = input_iterator_tag;
    using value_type        = vertex_descriptor<const vertex_id_type, vertex_type&, vertex_value_type>;
    using reference         = value_type&;
    using const_reference   = const value_type&;
    using rvalue_reference  = value_type&&;
    using pointer           = value_type*;
    using const_pointer     = value_type*;
    using size_type         = ranges::range_size_t<vertex_range_t<graph_type>>;
    using difference_type   = ranges::range_difference_t<vertex_range_t<graph_type>>;

  private:


    using shadow_vertex_type = remove_reference_t<vertex_reference>;
    using shadow_value_type =
          vertex_descriptor<vertex_id_t<graph_type>, shadow_vertex_type*, _detail::ref_to_ptr<vertex_value_type>>;

    union internal_value {
      value_type        value_;
      shadow_value_type shadow_;

      internal_value(vertex_id_type start_at) : shadow_{start_at, nullptr} {}
      internal_value(const internal_value& rhs) : shadow_(rhs.shadow_) {}
      internal_value() : shadow_{} {}
      ~internal_value() {}
      internal_value& operator=(const internal_value& rhs) { value_.shadow = rhs.value_.shadow; }
    };

  public:
    iterator(const bfs_range_type& range) : the_range_(&const_cast<bfs_range_type&>(range)) {}
    iterator()                = default;
    iterator(const iterator&) = default;
    iterator(iterator&&)      = default;
    ~iterator()               = default;

    iterator& operator=(const iterator&) = default;
    iterator& operator=(iterator&&)      = default;

    iterator& operator++() {
      the_range_->advance();
      return *this;
    }
    iterator operator++(int) const {
      iterator temp(*this);
      ++*this;
      return temp;
    }

    reference operator*() const noexcept {
      auto&          g    = *the_range_->graph_;
      auto&&         u_id = the_range_->Q_.front();
      auto&&         uvi  = the_range_->uv_;
      vertex_id_type v_id = the_range_->real_target_id(*uvi, u_id);
      auto&          v    = *find_vertex(g, v_id);
      value_.shadow_      = {v_id, &v, invoke(*the_range_->value_fn_, v)};
      return value_.value_;
    }

    constexpr bool operator==(const end_sentinel&) const noexcept { return the_range_->Q_.empty(); }
    constexpr bool operator!=(const end_sentinel& rhs) const noexcept { return !operator==(rhs); }

  private:
    mutable internal_value value_;
    bfs_range_type*        the_range_ = nullptr;
    friend end_sentinel;
  };

  auto begin() { return iterator(*this); }
  auto begin() const { return iterator(*this); }
  auto cbegin() const { return iterator(*this); }

  auto end() { return end_sentinel(); }
  auto end() const { return end_sentinel(); }
  auto cend() const { return end_sentinel(); }

private:
  const vertex_value_func* value_fn_ = nullptr;
};


template <adjacency_list G, class Alloc>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>
class vertices_breadth_first_search_view<G, void, Alloc> : public bfs_base<G, Alloc> {
public:
  using base_type        = bfs_base<G, Alloc>;
  using graph_type       = G;
  using vertex_type      = vertex_t<G>;
  using vertex_id_type   = vertex_id_t<graph_type>;
  using vertex_reference = vertex_reference_t<graph_type>;
  using vertex_iterator  = vertex_iterator_t<graph_type>;
  using edge_type        = edge_t<G>;
  using edge_reference   = edge_reference_t<G>;
  using edge_iterator    = vertex_edge_iterator_t<graph_type>;
  using bfs_range_type   = vertices_breadth_first_search_view<graph_type, void, Alloc>;

public:
  vertices_breadth_first_search_view(graph_type& g, vertex_id_type seed, const Alloc& alloc = Alloc())
        : base_type(g, seed, alloc) {}
  template <class VKR>
  requires ranges::forward_range<VKR> && convertible_to<ranges::range_value_t<VKR>, vertex_id_t<G>>
  vertices_breadth_first_search_view(graph_type& g, const VKR& seeds, const Alloc& alloc = Alloc())
        : base_type(g, seeds, alloc) {}

  vertices_breadth_first_search_view()                                          = default;
  vertices_breadth_first_search_view(const vertices_breadth_first_search_view&) = delete;
  vertices_breadth_first_search_view(vertices_breadth_first_search_view&&)      = default;
  ~vertices_breadth_first_search_view()                                         = default;

  vertices_breadth_first_search_view& operator=(const vertices_breadth_first_search_view&) = delete;
  vertices_breadth_first_search_view& operator=(vertices_breadth_first_search_view&&)      = default;

public:
  class iterator;
  struct end_sentinel {
    bool operator==(const iterator& rhs) const noexcept { return rhs.the_range_->Q_.empty(); }
  };

  class iterator {
  public:
    using iterator_category = input_iterator_tag;
    using value_type        = vertex_descriptor<const vertex_id_type, vertex_type&, void>;
    using reference         = value_type&;
    using const_reference   = const value_type&;
    using rvalue_reference  = value_type&&;
    using pointer           = value_type*;
    using const_pointer     = value_type*;
    using size_type         = ranges::range_size_t<vertex_range_t<graph_type>>;
    using difference_type   = ranges::range_difference_t<vertex_range_t<graph_type>>;

  private:


    using shadow_vertex_type = remove_reference_t<vertex_reference>;
    using shadow_value_type  = vertex_descriptor<vertex_id_t<graph_type>, shadow_vertex_type*, void>;

    union internal_value {
      value_type        value_;
      shadow_value_type shadow_;

      internal_value(vertex_id_type start_at) : shadow_{start_at, nullptr} {}
      internal_value(const internal_value& rhs) : shadow_(rhs.shadow_) {}
      internal_value() : shadow_{} {}
      ~internal_value() {}
      internal_value& operator=(const internal_value& rhs) { value_.shadow = rhs.value_.shadow; }
    };

  public:
    iterator(const bfs_range_type& range) : the_range_(&const_cast<bfs_range_type&>(range)) {}
    iterator()                = default;
    iterator(const iterator&) = default;
    iterator(iterator&&)      = default;
    ~iterator()               = default;

    iterator& operator=(const iterator&) = default;
    iterator& operator=(iterator&&)      = default;

    iterator& operator++() {
      the_range_->advance();
      return *this;
    }
    iterator operator++(int) const {
      iterator temp(*this);
      ++*this;
      return temp;
    }

    reference operator*() const noexcept {
      auto&          g    = *the_range_->graph_;
      auto&&         u_id = the_range_->Q_.front();
      auto&&         uvi  = the_range_->uv_;
      vertex_id_type v_id = the_range_->real_target_id(*uvi, u_id);
      auto&          v    = *find_vertex(g, v_id);
      value_.shadow_      = {v_id, &v};
      return value_.value_;
    }

    bool operator==(const end_sentinel&) const noexcept { return the_range_->Q_.empty(); }


  private:
    mutable internal_value value_;
    bfs_range_type*        the_range_ = nullptr;
    friend end_sentinel;
  };

  auto begin() { return iterator(*this); }
  auto begin() const { return iterator(*this); }
  auto cbegin() const { return iterator(*this); }

  auto end() { return end_sentinel(); }
  auto end() const { return end_sentinel(); }
  auto cend() const { return end_sentinel(); }
};
#line 448 "graph/views/breadth_first_search.hpp"
template <adjacency_list G, class EVF = void, bool Sourced = false, class Alloc = allocator<bool>>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>
class edges_breadth_first_search_view : public bfs_base<G, Alloc> {
public:
  using base_type           = bfs_base<G, Alloc>;
  using graph_type          = G;
  using vertex_id_type      = vertex_id_t<graph_type>;
  using vertex_iterator     = vertex_iterator_t<graph_type>;
  using edge_reference_type = edge_reference_t<graph_type>;
  using bfs_range_type      = edges_breadth_first_search_view<G, EVF, Sourced, Alloc>;

  using edge_value_func = remove_reference_t<EVF>;
  using edge_value_type = invoke_result_t<EVF, edge_reference_type>;

public:
  edges_breadth_first_search_view(G& g, vertex_id_type seed, const EVF& value_fn, const Alloc& alloc = Alloc())
        : base_type(g, seed, alloc), value_fn_(&value_fn) {}
  template <class VKR>
  requires ranges::forward_range<VKR> && convertible_to<ranges::range_value_t<VKR>, vertex_id_t<G>>
  edges_breadth_first_search_view(G& graph, const VKR& seeds, const EVF& value_fn, const Alloc& alloc = Alloc())
        : base_type(graph, seeds, alloc), value_fn_(&value_fn) {}

  edges_breadth_first_search_view()                                       = default;
  edges_breadth_first_search_view(const edges_breadth_first_search_view&) = delete;
  edges_breadth_first_search_view(edges_breadth_first_search_view&&)      = default;
  ~edges_breadth_first_search_view()                                      = default;

  edges_breadth_first_search_view& operator=(const edges_breadth_first_search_view&) = delete;
  edges_breadth_first_search_view& operator=(edges_breadth_first_search_view&&)      = default;

  class iterator;
  struct end_sentinel {
    bool operator==(const iterator& rhs) const noexcept { return rhs.the_range_->Q_.empty(); }
  };

  class iterator {
  public:
    using iterator_category = input_iterator_tag;
    using value_type        = edge_descriptor<const vertex_id_type, Sourced, edge_reference_type, edge_value_type>;
    using reference         = value_type&;
    using const_reference   = const value_type&;
    using rvalue_reference  = value_type&&;
    using pointer           = value_type*;
    using const_pointer     = value_type*;
    using size_type         = ranges::range_size_t<vertex_range_t<graph_type>>;
    using difference_type   = ranges::range_difference_t<vertex_range_t<graph_type>>;

  private:


    using shadow_edge_type = remove_reference_t<edge_reference_type>;
    using shadow_value_type =
          edge_descriptor<vertex_id_type, Sourced, shadow_edge_type*, _detail::ref_to_ptr<edge_value_type>>;

    union internal_value {
      value_type        value_;
      shadow_value_type shadow_;

      internal_value(vertex_id_type start_at) : shadow_{start_at, nullptr} {}
      internal_value(const internal_value& rhs) : shadow_(rhs.shadow_) {}
      internal_value() : shadow_{} {}
      ~internal_value() {}
      internal_value& operator=(const internal_value& rhs) { value_.shadow = rhs.value_.shadow; }
    };

  public:
    iterator(const bfs_range_type& range) : the_range_(&const_cast<bfs_range_type&>(range)) {}
    iterator()                = default;
    iterator(const iterator&) = default;
    iterator(iterator&&)      = default;
    ~iterator()               = default;

    iterator& operator=(const iterator&) = default;
    iterator& operator=(iterator&&)      = default;

    iterator& operator++() {
      the_range_->advance();
      return *this;
    }
    iterator operator++(int) const {
      iterator temp(*this);
      ++*this;
      return temp;
    }

    reference operator*() const noexcept {
      auto&& u_id = the_range_->Q_.front();
      auto&& uvi  = the_range_->uv_;
      if constexpr (Sourced) {
        value_.shadow_.source_id = u_id;
      }
      value_.shadow_.target_id = the_range_->real_target_id(*uvi, u_id);
      value_.shadow_.edge      = &*uvi;
      value_.shadow_.value     = invoke(*the_range_->value_fn_, *uvi);
      return value_.value_;
    }

    bool operator==(const end_sentinel&) const noexcept { return the_range_->Q_.empty(); }
    bool operator!=(const end_sentinel& rhs) const noexcept { return !operator==(rhs); }

  private:
    mutable internal_value value_;
    bfs_range_type*        the_range_ = nullptr;
    friend end_sentinel;
  };

  auto begin() { return iterator(*this); }
  auto begin() const { return iterator(*this); }
  auto cbegin() const { return iterator(*this); }

  auto end() { return end_sentinel(); }
  auto end() const { return end_sentinel(); }
  auto cend() const { return end_sentinel(); }

private:
  const edge_value_func* value_fn_ = nullptr;
};

template <adjacency_list G, bool Sourced, class Alloc>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>
class edges_breadth_first_search_view<G, void, Sourced, Alloc> : public bfs_base<G, Alloc> {
public:
  using base_type           = bfs_base<G, Alloc>;
  using graph_type          = G;
  using vertex_id_type      = vertex_id_t<graph_type>;
  using vertex_iterator     = vertex_iterator_t<graph_type>;
  using edge_reference_type = edge_reference_t<graph_type>;
  using bfs_range_type      = edges_breadth_first_search_view<G, void, Sourced, Alloc>;

public:
  edges_breadth_first_search_view(G& g, vertex_id_type seed, const Alloc& alloc = Alloc())
        : base_type(g, seed, alloc) {}
  template <class VKR>
  requires ranges::forward_range<VKR> && convertible_to<ranges::range_value_t<VKR>, vertex_id_t<G>>
  edges_breadth_first_search_view(G& g, const VKR& seeds, const Alloc& alloc()) : base_type(g, seeds, alloc) {}

  edges_breadth_first_search_view()                                       = default;
  edges_breadth_first_search_view(const edges_breadth_first_search_view&) = delete;
  edges_breadth_first_search_view(edges_breadth_first_search_view&&)      = default;
  ~edges_breadth_first_search_view()                                      = default;

  edges_breadth_first_search_view& operator=(const edges_breadth_first_search_view&) = delete;
  edges_breadth_first_search_view& operator=(edges_breadth_first_search_view&&)      = default;

  class iterator;
  struct end_sentinel {
    bool operator==(const iterator& rhs) const noexcept { return rhs.the_range_->Q_.empty(); }
  };

  class iterator {
  public:
    using iterator_category = input_iterator_tag;
    using value_type        = edge_descriptor<const vertex_id_type, Sourced, edge_reference_type, void>;
    using reference         = value_type&;
    using const_reference   = const value_type&;
    using rvalue_reference  = value_type&&;
    using pointer           = value_type*;
    using const_pointer     = value_type*;
    using size_type         = ranges::range_size_t<vertex_range_t<graph_type>>;
    using difference_type   = ranges::range_difference_t<vertex_range_t<graph_type>>;

  private:


    using shadow_edge_type  = remove_reference_t<edge_reference_type>;
    using shadow_value_type = edge_descriptor<vertex_id_type, Sourced, shadow_edge_type*, void>;

    union internal_value {
      value_type        value_;
      shadow_value_type shadow_;

      internal_value(vertex_id_type start_at) : shadow_{start_at, nullptr} {}
      internal_value(const internal_value& rhs) : shadow_(rhs.shadow_) {}
      internal_value() : shadow_{} {}
      ~internal_value() {}
      internal_value& operator=(const internal_value& rhs) { value_.shadow = rhs.value_.shadow; }
    };

  public:
    iterator(const bfs_range_type& range) : the_range_(&const_cast<bfs_range_type&>(range)) {}
    iterator()                = default;
    iterator(const iterator&) = default;
    iterator(iterator&&)      = default;
    ~iterator()               = default;

    iterator& operator=(const iterator&) = default;
    iterator& operator=(iterator&&)      = default;

    iterator& operator++() {
      the_range_->advance();
      return *this;
    }
    iterator operator++(int) const {
      iterator temp(*this);
      ++*this;
      return temp;
    }

    reference operator*() const noexcept {
      auto&& u_id = the_range_->Q_.front();
      auto&& uvi  = the_range_->uv_;
      if constexpr (Sourced) {
        value_.shadow_.source_id = u_id;
      }
      value_.shadow_.target_id = the_range_->real_target_id(*uvi, u_id);
      value_.shadow_.edge      = &*uvi;
      return value_.value_;
    }

    bool operator==(const end_sentinel&) const noexcept { return the_range_->Q_.empty(); }
    bool operator!=(const end_sentinel& rhs) const noexcept { return !operator==(rhs); }

  private:
    mutable internal_value value_;
    bfs_range_type*        the_range_ = nullptr;
    friend end_sentinel;
  };

  auto begin() { return iterator(*this); }
  auto begin() const { return iterator(*this); }
  auto cbegin() const { return iterator(*this); }

  auto end() { return end_sentinel(); }
  auto end() const { return end_sentinel(); }
  auto cend() const { return end_sentinel(); }
};


namespace views {
#line 681 "graph/views/breadth_first_search.hpp"
  namespace _Vertices_BFS {



    void vertices_breadth_first_search();


    template <class _G, class _Alloc, class _UnCV>
    concept _Has_ref_ADL = _Has_class_or_enum_type<_G>
                           && requires(_G&& __g, const vertex_id_t<_G>& uid, _Alloc alloc) {
                                { _Fake_copy_init(vertices_breadth_first_search(__g, uid, alloc)) };
                              };
    template <class _G, class _Alloc, class _UnCV>
    concept _Can_ref_eval = index_adjacency_list<_G>
                            && requires(_G&& __g, vertex_id_t<_G> uid, _Alloc alloc) {
                                 { _Fake_copy_init(vertices_breadth_first_search_view<_G, void>(__g, uid, alloc)) };
                               };

    template <class _G, class _VVF, class _Alloc, class _UnCV>
    concept _Has_ref_vvf_ADL = _Has_class_or_enum_type<_G>
                               && invocable<_VVF, vertex_reference_t<_G>>
                               && requires(_G&& __g, const vertex_id_t<_G>& uid, _VVF vvf, _Alloc alloc) {
                                    {
                                      _Fake_copy_init(vertices_breadth_first_search(__g, uid, vvf, alloc))
                                    };
                                  };
    template <class _G, class _VVF, class _Alloc, class _UnCV>
    concept _Can_ref_vvf_eval =
          index_adjacency_list<_G>
          && invocable<_VVF, vertex_reference_t<_G>>
          && requires(_G&& __g, vertex_id_t<_G> uid, _VVF vvf, _Alloc alloc) {
               { _Fake_copy_init(vertices_breadth_first_search_view<_G, _VVF>(__g, uid, vvf, alloc)) };
             };

    class _Cpo {
    private:
      enum class _St_ref { _None, _Non_member, _Auto_eval };
      enum class _St_ref_vvf { _None, _Non_member, _Auto_eval };

      template <class _G, class _Alloc>
      [[nodiscard]] static consteval _Choice_t<_St_ref> _Choose_ref() noexcept {

        using _UnCV = remove_cvref_t<_G>;

        if constexpr (_Has_ref_ADL<_G, _Alloc, _UnCV>) {
          return {_St_ref::_Non_member,
                  noexcept(_Fake_copy_init(vertices_breadth_first_search(declval<_G>(), declval<vertex_id_t<_G>>(),
                                                                         declval<_Alloc>())))};
        } else if constexpr (_Can_ref_eval<_G, _Alloc, _UnCV>) {
          return {_St_ref::_Auto_eval, noexcept(_Fake_copy_init(vertices_breadth_first_search_view<_G, void>(
                                             declval<_G>(), declval<vertex_id_t<_G>>(), declval<_Alloc>())))};
        } else {
          return {_St_ref::_None};
        }
      }

      template <class _G, class _Alloc>
      static constexpr _Choice_t<_St_ref> _Choice_ref = _Choose_ref<_G, _Alloc>();

      template <class _G, class _VVF, class _Alloc>
      [[nodiscard]] static consteval _Choice_t<_St_ref_vvf> _Choose_ref_vvf() noexcept {

        using _UnCV = remove_cvref_t<_G>;

        if constexpr (_Has_ref_vvf_ADL<_G, _VVF, _Alloc, _UnCV>) {
          return {_St_ref_vvf::_Non_member, noexcept(_Fake_copy_init(vertices_breadth_first_search(
                                                  declval<_G>(), declval<vertex_id_t<_G>>(), declval<_VVF>(),
                                                  declval<_Alloc>())))};
        } else if constexpr (_Can_ref_vvf_eval<_G, _VVF, _Alloc, _UnCV>) {
          return {_St_ref_vvf::_Auto_eval,
                  noexcept(_Fake_copy_init(vertices_breadth_first_search_view<_G, _VVF>(
                        declval<_G>(), declval<vertex_id_t<_G>>(), declval<_VVF>(), declval<_Alloc>())))};
        } else {
          return {_St_ref_vvf::_None};
        }
      }

      template <class _G, class _VVF, class _Alloc>
      static constexpr _Choice_t<_St_ref_vvf> _Choice_ref_vvf = _Choose_ref_vvf<_G, _VVF, _Alloc>();

    public:
#line 773 "graph/views/breadth_first_search.hpp"
      template <class _G, class _Alloc = allocator<bool>>
      requires(_Choice_ref<_G&, _Alloc>._Strategy != _St_ref::_None)
      [[nodiscard]] constexpr auto operator()(_G&& __g, const vertex_id_t<_G>& seed, _Alloc alloc = _Alloc()) const
            noexcept(_Choice_ref<_G&, _Alloc>._No_throw) {
        constexpr _St_ref _Strat_ref = _Choice_ref<_G&, _Alloc>._Strategy;

        if constexpr (_Strat_ref == _St_ref::_Non_member) {
          return vertices_breadth_first_search(__g, seed, alloc);
        } else if constexpr (_Strat_ref == _St_ref::_Auto_eval) {
          return vertices_breadth_first_search_view<_G, void>(__g, seed, alloc);
        } else {
          static_assert(_Always_false<_G>, "The default implementation of "
                                           "vertices_breadth_first_search(g,seed,alloc) cannot be evaluated and "
                                           "there is no override defined for the graph.");
        }
      }
#line 805 "graph/views/breadth_first_search.hpp"
      template <class _G, class _VVF, class _Alloc = allocator<bool>>
      requires(_Choice_ref_vvf<_G&, _VVF, _Alloc>._Strategy != _St_ref_vvf::_None)
      [[nodiscard]] constexpr auto
      operator()(_G&& __g, const vertex_id_t<_G>& seed, _VVF&& vvf, _Alloc alloc = _Alloc()) const
            noexcept(_Choice_ref_vvf<_G&, _VVF, _Alloc>._No_throw) {
        constexpr _St_ref_vvf _Strat_ref_vvf = _Choice_ref_vvf<_G&, _VVF, _Alloc>._Strategy;

        if constexpr (_Strat_ref_vvf == _St_ref_vvf::_Non_member) {
          return vertices_breadth_first_search(__g, seed, vvf, alloc);
        } else if constexpr (_Strat_ref_vvf == _St_ref_vvf::_Auto_eval) {
          return vertices_breadth_first_search_view<_G, _VVF>(__g, seed, vvf, alloc);
        } else {
          static_assert(_Always_false<_G>, "The default implementation of "
                                           "vertices_breadth_first_search(g,seed,vvf,alloc) cannot be evaluated and "
                                           "there is no override defined for the graph.");
        }
      }
    };
  }

  inline namespace _Cpos {
    inline constexpr _Vertices_BFS::_Cpo vertices_breadth_first_search;
  }
#line 834 "graph/views/breadth_first_search.hpp"
  namespace _Edges_BFS {



    void edges_breadth_first_search();


    template <class _G, class _Alloc, class _UnCV>
    concept _Has_ref_ADL = _Has_class_or_enum_type<_G>
                           && requires(_G&& __g, const vertex_id_t<_G>& uid, _Alloc alloc) {
                                { _Fake_copy_init(edges_breadth_first_search(__g, uid, alloc)) };
                              };
    template <class _G, class _Alloc, class _UnCV>
    concept _Can_ref_eval = index_adjacency_list<_G>
                            && requires(_G&& __g, vertex_id_t<_G> uid, _Alloc alloc) {
                                 { _Fake_copy_init(edges_breadth_first_search_view<_G, void, false>(__g, uid, alloc)) };
                               };

    template <class _G, class _EVF, class _Alloc, class _UnCV>
    concept _Has_ref_evf_ADL = _Has_class_or_enum_type<_G>
                               && invocable<_EVF, edge_reference_t<_G>>
                               && requires(_G&& __g, const vertex_id_t<_G>& uid, _EVF evf, _Alloc alloc) {
                                    {
                                      _Fake_copy_init(edges_breadth_first_search(__g, uid, evf, alloc))
                                    };
                                  };
    template <class _G, class _EVF, class _Alloc, class _UnCV>
    concept _Can_ref_evf_eval =
          index_adjacency_list<_G>
          && invocable<_EVF, edge_reference_t<_G>>
          && requires(_G&& __g, vertex_id_t<_G> uid, _EVF evf, _Alloc alloc) {
               { _Fake_copy_init(edges_breadth_first_search_view<_G, _EVF, false>(__g, uid, evf, alloc)) };
             };

    class _Cpo {
    private:
      enum class _St_ref { _None, _Non_member, _Auto_eval };
      enum class _St_ref_evf { _None, _Non_member, _Auto_eval };

      template <class _G, class _Alloc>
      [[nodiscard]] static consteval _Choice_t<_St_ref> _Choose_ref() noexcept {

        using _UnCV = remove_cvref_t<_G>;

        if constexpr (_Has_ref_ADL<_G, _Alloc, _UnCV>) {
          return {_St_ref::_Non_member,
                  noexcept(_Fake_copy_init(edges_breadth_first_search(declval<_G>(), declval<vertex_id_t<_G>>(),
                                                                      declval<_Alloc>())))};
        } else if constexpr (_Can_ref_eval<_G, _Alloc, _UnCV>) {
          return {_St_ref::_Auto_eval, noexcept(_Fake_copy_init(edges_breadth_first_search_view<_G, void, false>(
                                             declval<_G>(), declval<vertex_id_t<_G>>(), declval<_Alloc>())))};
        } else {
          return {_St_ref::_None};
        }
      }

      template <class _G, class _Alloc>
      static constexpr _Choice_t<_St_ref> _Choice_ref = _Choose_ref<_G, _Alloc>();

      template <class _G, class _EVF, class _Alloc>
      [[nodiscard]] static consteval _Choice_t<_St_ref_evf> _Choose_ref_evf() noexcept {

        using _UnCV = remove_cvref_t<_G>;

        if constexpr (_Has_ref_evf_ADL<_G, _EVF, _Alloc, _UnCV>) {
          return {_St_ref_evf::_Non_member, noexcept(_Fake_copy_init(edges_breadth_first_search(
                                                  declval<_G>(), declval<vertex_id_t<_G>>(), declval<_EVF>(),
                                                  declval<_Alloc>())))};
        } else if constexpr (_Can_ref_evf_eval<_G, _EVF, _Alloc, _UnCV>) {
          return {_St_ref_evf::_Auto_eval,
                  noexcept(_Fake_copy_init(edges_breadth_first_search_view<_G, _EVF, false>(
                        declval<_G>(), declval<vertex_id_t<_G>>(), declval<_EVF>(), declval<_Alloc>())))};
        } else {
          return {_St_ref_evf::_None};
        }
      }

      template <class _G, class _EVF, class _Alloc>
      static constexpr _Choice_t<_St_ref_evf> _Choice_ref_evf = _Choose_ref_evf<_G, _EVF, _Alloc>();

    public:
#line 928 "graph/views/breadth_first_search.hpp"
      template <class _G, class _Alloc = allocator<bool>>
      requires(_Choice_ref<_G&, _Alloc>._Strategy != _St_ref::_None)
      [[nodiscard]] constexpr auto operator()(_G&& __g, const vertex_id_t<_G>& seed, _Alloc alloc = _Alloc()) const
            noexcept(_Choice_ref<_G&, _Alloc>._No_throw) {
        constexpr _St_ref _Strat_ref = _Choice_ref<_G&, _Alloc>._Strategy;

        if constexpr (_Strat_ref == _St_ref::_Non_member) {
          return edges_breadth_first_search(__g, seed, alloc);
        } else if constexpr (_Strat_ref == _St_ref::_Auto_eval) {
          return edges_breadth_first_search_view<_G, void, false>(__g, seed, alloc);
        } else {
          static_assert(_Always_false<_G>, "The default implementation of "
                                           "edges_breadth_first_search(g,seed,alloc) cannot be evaluated and "
                                           "there is no override defined for the graph.");
        }
      }
#line 960 "graph/views/breadth_first_search.hpp"
      template <class _G, class _EVF, class _Alloc = allocator<bool>>
      requires(_Choice_ref_evf<_G&, _EVF, _Alloc>._Strategy != _St_ref_evf::_None)
      [[nodiscard]] constexpr auto
      operator()(_G&& __g, const vertex_id_t<_G>& seed, _EVF&& evf, _Alloc alloc = _Alloc()) const
            noexcept(_Choice_ref_evf<_G&, _EVF, _Alloc>._No_throw) {
        constexpr _St_ref_evf _Strat_ref_evf = _Choice_ref_evf<_G&, _EVF, _Alloc>._Strategy;

        if constexpr (_Strat_ref_evf == _St_ref_evf::_Non_member) {
          return edges_breadth_first_search(__g, seed, alloc);
        } else if constexpr (_Strat_ref_evf == _St_ref_evf::_Auto_eval) {
          return edges_breadth_first_search_view<_G, _EVF, false>(__g, seed, evf, alloc);
        } else {
          static_assert(_Always_false<_G>, "The default implementation of "
                                           "edges_breadth_first_search(g,seed,evf,alloc) cannot be evaluated and "
                                           "there is no override defined for the graph.");
        }
      }
    };
  }

  inline namespace _Cpos {
    inline constexpr _Edges_BFS::_Cpo edges_breadth_first_search;
  }
#line 989 "graph/views/breadth_first_search.hpp"
  namespace _Sourced_Edges_BFS {



    void sourced_edges_breadth_first_search();


    template <class _G, class _Alloc, class _UnCV>
    concept _Has_ref_ADL = _Has_class_or_enum_type<_G>
                           && requires(_G&& __g, const vertex_id_t<_G>& uid, _Alloc alloc) {
                                {
                                  _Fake_copy_init(sourced_edges_breadth_first_search(__g, uid, alloc))
                                };
                              };
    template <class _G, class _Alloc, class _UnCV>
    concept _Can_ref_eval = index_adjacency_list<_G>
                            && requires(_G&& __g, vertex_id_t<_G> uid, _Alloc alloc) {
                                 { _Fake_copy_init(edges_breadth_first_search_view<_G, void, true>(__g, uid, alloc)) };
                               };

    template <class _G, class _EVF, class _Alloc, class _UnCV>
    concept _Has_ref_evf_ADL = _Has_class_or_enum_type<_G>
                               && invocable<_EVF, edge_reference_t<_G>>
                               && requires(_G&& __g, const vertex_id_t<_G>& uid, _EVF evf, _Alloc alloc) {
                                    {
                                      _Fake_copy_init(sourced_edges_breadth_first_search(__g, uid, evf, alloc))
                                    };
                                  };
    template <class _G, class _EVF, class _Alloc, class _UnCV>
    concept _Can_ref_evf_eval =
          index_adjacency_list<_G>
          && invocable<_EVF, edge_reference_t<_G>>
          && requires(_G&& __g, vertex_id_t<_G> uid, _EVF evf, _Alloc alloc) {
               { _Fake_copy_init(edges_breadth_first_search_view<_G, _EVF, true>(__g, uid, evf, alloc)) };
             };

    class _Cpo {
    private:
      enum class _St_ref { _None, _Non_member, _Auto_eval };
      enum class _St_ref_evf { _None, _Non_member, _Auto_eval };

      template <class _G, class _Alloc>
      [[nodiscard]] static consteval _Choice_t<_St_ref> _Choose_ref() noexcept {

        using _UnCV = remove_cvref_t<_G>;

        if constexpr (_Has_ref_ADL<_G, _Alloc, _UnCV>) {
          return {_St_ref::_Non_member,
                  noexcept(_Fake_copy_init(sourced_edges_breadth_first_search(declval<_G>(), declval<vertex_id_t<_G>>(),
                                                                              declval<_Alloc>())))};
        } else if constexpr (_Can_ref_eval<_G, _Alloc, _UnCV>) {
          return {_St_ref::_Auto_eval, noexcept(_Fake_copy_init(edges_breadth_first_search_view<_G, void, true>(
                                             declval<_G>(), declval<vertex_id_t<_G>>(), declval<_Alloc>())))};
        } else {
          return {_St_ref::_None};
        }
      }

      template <class _G, class _Alloc>
      static constexpr _Choice_t<_St_ref> _Choice_ref = _Choose_ref<_G, _Alloc>();

      template <class _G, class _EVF, class _Alloc>
      [[nodiscard]] static consteval _Choice_t<_St_ref_evf> _Choose_ref_evf() noexcept {

        using _UnCV = remove_cvref_t<_G>;

        if constexpr (_Has_ref_evf_ADL<_G, _EVF, _Alloc, _UnCV>) {
          return {_St_ref_evf::_Non_member, noexcept(_Fake_copy_init(sourced_edges_breadth_first_search(
                                                  declval<_G>(), declval<vertex_id_t<_G>>(), declval<_EVF>(),
                                                  declval<_Alloc>())))};
        } else if constexpr (_Can_ref_evf_eval<_G, _EVF, _Alloc, _UnCV>) {
          return {_St_ref_evf::_Auto_eval,
                  noexcept(_Fake_copy_init(edges_breadth_first_search_view<_G, _EVF, true>(
                        declval<_G>(), declval<vertex_id_t<_G>>(), declval<_EVF>(), declval<_Alloc>())))};
        } else {
          return {_St_ref_evf::_None};
        }
      }

      template <class _G, class _EVF, class _Alloc>
      static constexpr _Choice_t<_St_ref_evf> _Choice_ref_evf = _Choose_ref_evf<_G, _EVF, _Alloc>();

    public:
#line 1085 "graph/views/breadth_first_search.hpp"
      template <class _G, class _Alloc = allocator<bool>>
      requires(_Choice_ref<_G&, _Alloc>._Strategy != _St_ref::_None)
      [[nodiscard]] constexpr auto operator()(_G&& __g, const vertex_id_t<_G>& seed, _Alloc alloc = _Alloc()) const
            noexcept(_Choice_ref<_G&, _Alloc>._No_throw) {
        constexpr _St_ref _Strat_ref = _Choice_ref<_G&, _Alloc>._Strategy;

        if constexpr (_Strat_ref == _St_ref::_Non_member) {
          return sourced_edges_breadth_first_search(__g, seed, alloc);
        } else if constexpr (_Strat_ref == _St_ref::_Auto_eval) {
          return edges_breadth_first_search_view<_G, void, true>(__g, seed, alloc);
        } else {
          static_assert(_Always_false<_G>, "The default implementation of "
                                           "sourced_edges_breadth_first_search(g,seed,alloc) cannot be evaluated and "
                                           "there is no override defined for the graph.");
        }
      }
#line 1117 "graph/views/breadth_first_search.hpp"
      template <class _G, class _EVF, class _Alloc = allocator<bool>>
      requires(_Choice_ref_evf<_G&, _EVF, _Alloc>._Strategy != _St_ref_evf::_None)
      [[nodiscard]] constexpr auto
      operator()(_G&& __g, const vertex_id_t<_G>& seed, _EVF&& evf, _Alloc alloc = _Alloc()) const
            noexcept(_Choice_ref_evf<_G&, _EVF, _Alloc>._No_throw) {
        constexpr _St_ref_evf _Strat_ref_evf = _Choice_ref_evf<_G&, _EVF, _Alloc>._Strategy;

        if constexpr (_Strat_ref_evf == _St_ref_evf::_Non_member) {
          return sourced_edges_breadth_first_search(__g, seed, alloc);
        } else if constexpr (_Strat_ref_evf == _St_ref_evf::_Auto_eval) {
          return edges_breadth_first_search_view<_G, _EVF, true>(__g, seed, evf, alloc);
        } else {
          static_assert(_Always_false<_G>,
                        "The default implementation of "
                        "sourced_edges_breadth_first_search(g,seed,evf,alloc) cannot be evaluated and "
                        "there is no override defined for the graph.");
        }
      }
    };
  }

  inline namespace _Cpos {
    inline constexpr _Sourced_Edges_BFS::_Cpo sourced_edges_breadth_first_search;
  }

}
}
#line 28 "graph/views/depth_first_search.hpp"
#include <stack>
#include <vector>
#include <functional>




namespace std::graph {
#line 43 "graph/views/depth_first_search.hpp"
template <adjacency_list G, class Alloc>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>
class dfs_base : public ranges::view_base {
public:
  using graph_type       = remove_reference_t<G>;
  using vertex_type      = vertex_t<G>;
  using vertex_id_type   = vertex_id_t<graph_type>;
  using vertex_reference = vertex_reference_t<graph_type>;
  using vertex_iterator  = vertex_iterator_t<graph_type>;
  using edge_type        = edge_t<G>;
  using edge_reference   = edge_reference_t<G>;
  using edge_iterator    = vertex_edge_iterator_t<graph_type>;

private:
  struct stack_elem {
    vertex_id_t<G>            u_id;
    vertex_edge_iterator_t<G> uv;
  };
  using Stack = stack<stack_elem>;

  using graph_ref_type = reference_wrapper<graph_type>;

  using parent_alloc = typename allocator_traits<typename Stack::container_type::allocator_type>::template rebind_alloc<
        vertex_id_type>;

public:
  dfs_base(graph_type& g, vertex_id_type seed, const Alloc& alloc)
        : graph_(&g), S_(alloc), colors_(ranges::size(vertices(g)), white, alloc) {
    if (seed < static_cast<vertex_id_type>(ranges::size(vertices(*graph_))) && !ranges::empty(edges(*graph_, seed))) {
      edge_iterator uvi = ranges::begin(edges(*graph_, seed));
      S_.push(stack_elem{seed, uvi});
      colors_[seed] = grey;


      if (uvi != ranges::end(edges(*graph_, seed))) {
        vertex_id_type v_id = real_target_id(*uvi, seed);
        colors_[v_id]       = grey;
      }
    }
  }
  dfs_base()                = default;
  dfs_base(const dfs_base&) = delete;
  dfs_base(dfs_base&&)      = default;
  ~dfs_base()               = default;

  dfs_base& operator=(const dfs_base&) = delete;
  dfs_base& operator=(dfs_base&&)      = default;

  constexpr bool empty() const noexcept { return S_.empty(); }

  constexpr auto size() const noexcept { return S_.size(); }
  constexpr auto depth() const noexcept { return S_.size(); }

  constexpr void          cancel(cancel_search cancel_type) noexcept { cancel_ = cancel_type; }
  constexpr cancel_search canceled() noexcept { return cancel_; }

protected:
  constexpr vertex_id_type real_target_id(edge_reference uv, vertex_id_type) const
  requires ordered_edge<G, edge_type>
  {
    return target_id(*graph_, uv);
  }
  constexpr vertex_id_type real_target_id(edge_reference uv, vertex_id_type src) const
  requires unordered_edge<G, edge_type>
  {
    if (target_id(*graph_, uv) != src)
      return target_id(*graph_, uv);
    else
      return source_id((*graph_), uv);
  }

  constexpr vertex_edge_iterator_t<G> find_unvisited(vertex_id_t<G> uid, vertex_edge_iterator_t<G> first) {
    return ranges::find_if(first, ranges::end(edges(*graph_, uid)), [this, uid](edge_reference uv) -> bool {
      return colors_[real_target_id(uv, uid)] == white;
    });
  }

  constexpr void advance() {

    auto [u_id, uvi]    = S_.top();
    vertex_id_type v_id = real_target_id(*uvi, u_id);

    edge_iterator vwi = ranges::end(edges(*graph_, v_id));
    switch (cancel_) {
    case cancel_search::continue_search:

      vwi = find_unvisited(v_id, ranges::begin(edges(*graph_, v_id)));
      break;
    case cancel_search::cancel_branch: {
      cancel_       = cancel_search::continue_search;
      colors_[v_id] = black;


      uvi = find_unvisited(u_id, ++uvi);
      if (uvi != ranges::end(edges(*graph_, u_id))) {
        S_.top().uv = uvi;
        return;
      }

    } break;
    case cancel_search::cancel_all:
      while (!S_.empty())
        S_.pop();
      return;
    }


    if (vwi != ranges::end(edges(*graph_, v_id))) {
      S_.push(stack_elem{v_id, vwi});
      vertex_id_type w_id = real_target_id(*vwi, v_id);
      colors_[w_id]       = grey;
    }

    else {
      colors_[v_id] = black;
      while (!S_.empty()) {
        auto [x_id, xyi] = S_.top();
        S_.pop();
        xyi = find_unvisited(x_id, ++xyi);


        if (xyi != ranges::end(edges(*graph_, x_id))) {
          S_.push({x_id, xyi});
          vertex_id_type y_id = real_target_id(*xyi, x_id);
          colors_[y_id]       = grey;
          break;
        } else {
          colors_[x_id] = black;
        }
      }
    }
  }

protected:
  graph_type*          graph_ = nullptr;
  Stack                S_;
  vector<three_colors> colors_;
  cancel_search        cancel_ = cancel_search::continue_search;
};
#line 191 "graph/views/depth_first_search.hpp"
template <adjacency_list G, class VVF = void, class Alloc = allocator<bool>>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>
class vertices_depth_first_search_view : public dfs_base<G, Alloc> {
public:
  using base_type        = dfs_base<G, Alloc>;
  using graph_type       = G;
  using vertex_type      = vertex_t<G>;
  using vertex_id_type   = vertex_id_t<graph_type>;
  using vertex_reference = vertex_reference_t<graph_type>;
  using vertex_iterator  = vertex_iterator_t<graph_type>;
  using edge_reference   = edge_reference_t<G>;
  using edge_iterator    = vertex_edge_iterator_t<graph_type>;
  using dfs_range_type   = vertices_depth_first_search_view<graph_type, VVF, Alloc>;

  using vertex_value_func = remove_reference_t<VVF>;
  using vertex_value_type = invoke_result_t<VVF, vertex_reference>;

public:
  vertices_depth_first_search_view(graph_type&    g,
                                   vertex_id_type seed,
                                   const VVF&     value_fn,
                                   const Alloc&   alloc = Alloc())
        : base_type(g, seed, alloc), value_fn_(&value_fn) {}
  vertices_depth_first_search_view()                                        = default;
  vertices_depth_first_search_view(const vertices_depth_first_search_view&) = delete;
  vertices_depth_first_search_view(vertices_depth_first_search_view&&)      = default;
  ~vertices_depth_first_search_view()                                       = default;

  vertices_depth_first_search_view& operator=(const vertices_depth_first_search_view&) = delete;
  vertices_depth_first_search_view& operator=(vertices_depth_first_search_view&&)      = default;

public:
  class iterator;
  struct end_sentinel {
    constexpr bool operator==(const iterator& rhs) const noexcept { return rhs.the_range_->S_.empty(); }

  };

  class iterator {
  public:
    using iterator_category = input_iterator_tag;
    using value_type        = vertex_descriptor<const vertex_id_type, vertex_type&, vertex_value_type>;
    using reference         = value_type&;
    using const_reference   = const value_type&;
    using rvalue_reference  = value_type&&;
    using pointer           = value_type*;
    using const_pointer     = value_type*;
    using size_type         = ranges::range_size_t<vertex_range_t<graph_type>>;
    using difference_type   = ranges::range_difference_t<vertex_range_t<graph_type>>;

  private:


    using shadow_vertex_type = remove_reference_t<vertex_reference>;
    using shadow_value_type =
          vertex_descriptor<vertex_id_t<graph_type>, shadow_vertex_type*, _detail::ref_to_ptr<vertex_value_type>>;

    union internal_value {
      value_type        value_;
      shadow_value_type shadow_;

      internal_value(const internal_value& rhs) : shadow_(rhs.shadow_) {}
      internal_value() : shadow_{} {}
      ~internal_value() {}
      internal_value& operator=(const internal_value& rhs) { value_.shadow = rhs.value_.shadow; }
    };

  public:
    iterator(const dfs_range_type& range) : the_range_(&const_cast<dfs_range_type&>(range)) {}
    iterator()                = default;
    iterator(const iterator&) = default;
    iterator(iterator&&)      = default;
    ~iterator()               = default;

    iterator& operator=(const iterator&) = default;
    iterator& operator=(iterator&&)      = default;

    iterator& operator++() {
      the_range_->advance();
      return *this;
    }
    iterator operator++(int) const {
      iterator temp(*this);
      ++*this;
      return temp;
    }

    reference operator*() const noexcept {
      auto& g             = *the_range_->graph_;
      auto&& [u_id, uvi]  = the_range_->S_.top();
      vertex_id_type v_id = the_range_->real_target_id(*uvi, u_id);
      auto&          v    = *find_vertex(g, v_id);
      value_.shadow_      = {v_id, &v, invoke(*the_range_->value_fn_, v)};
      return value_.value_;
    }

    constexpr bool operator==(const end_sentinel&) const noexcept { return the_range_->S_.empty(); }


  private:
    mutable internal_value value_;
    dfs_range_type*        the_range_ = nullptr;
    friend end_sentinel;
  };

  auto begin() { return iterator(*this); }
  auto begin() const { return iterator(*this); }
  auto cbegin() const { return iterator(*this); }

  auto end() { return end_sentinel(); }
  auto end() const { return end_sentinel(); }
  auto cend() const { return end_sentinel(); }

private:
  const vertex_value_func* value_fn_ = nullptr;
};


template <adjacency_list G, class Alloc>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>
class vertices_depth_first_search_view<G, void, Alloc> : public dfs_base<G, Alloc> {
public:
  using base_type        = dfs_base<G, Alloc>;
  using graph_type       = G;
  using vertex_type      = vertex_t<G>;
  using vertex_id_type   = vertex_id_t<graph_type>;
  using vertex_reference = vertex_reference_t<graph_type>;
  using vertex_iterator  = vertex_iterator_t<graph_type>;
  using edge_reference   = edge_reference_t<G>;
  using edge_iterator    = vertex_edge_iterator_t<graph_type>;
  using dfs_range_type   = vertices_depth_first_search_view<graph_type, void, Alloc>;

public:
  vertices_depth_first_search_view(graph_type& g, vertex_id_type seed, const Alloc& alloc = Alloc())
        : base_type(g, seed, alloc) {}
  vertices_depth_first_search_view()                                        = default;
  vertices_depth_first_search_view(const vertices_depth_first_search_view&) = delete;
  vertices_depth_first_search_view(vertices_depth_first_search_view&&)      = default;
  ~vertices_depth_first_search_view()                                       = default;

  vertices_depth_first_search_view& operator=(const vertices_depth_first_search_view&) = delete;
  vertices_depth_first_search_view& operator=(vertices_depth_first_search_view&&)      = default;

public:
  struct end_sentinel {
    bool operator==(const end_sentinel& rhs) const noexcept { return rhs.the_range_->S_.empty(); }
  };

  class iterator {
  public:
    using iterator_category = input_iterator_tag;
    using value_type        = vertex_descriptor<const vertex_id_type, vertex_type&, void>;
    using reference         = value_type&;
    using const_reference   = const value_type&;
    using rvalue_reference  = value_type&&;
    using pointer           = value_type*;
    using const_pointer     = value_type*;
    using size_type         = ranges::range_size_t<vertex_range_t<graph_type>>;
    using difference_type   = ranges::range_difference_t<vertex_range_t<graph_type>>;

  private:


    using shadow_vertex_type = remove_reference_t<vertex_reference>;
    using shadow_value_type  = vertex_descriptor<vertex_id_t<graph_type>, shadow_vertex_type*, void>;

    union internal_value {
      value_type        value_;
      shadow_value_type shadow_;

      internal_value(const internal_value& rhs) : shadow_(rhs.shadow_) {}
      internal_value() : shadow_{} {}
      ~internal_value() {}
      internal_value& operator=(const internal_value& rhs) { value_.shadow = rhs.value_.shadow; }
    };

  public:
    iterator(const dfs_range_type& range) : the_range_(&const_cast<dfs_range_type&>(range)) {}
    iterator()                = default;
    iterator(const iterator&) = default;
    iterator(iterator&&)      = default;
    ~iterator()               = default;

    iterator& operator=(const iterator&) = default;
    iterator& operator=(iterator&&)      = default;

    iterator& operator++() {
      the_range_->advance();
      return *this;
    }
    iterator operator++(int) const {
      iterator temp(*this);
      ++*this;
      return temp;
    }

    reference operator*() const noexcept {
      auto& g             = *the_range_->graph_;
      auto&& [u_id, uvi]  = the_range_->S_.top();
      vertex_id_type v_id = the_range_->real_target_id(*uvi, u_id);
      auto&          v    = *find_vertex(g, v_id);
      value_.shadow_      = {v_id, &v};
      return value_.value_;
    }

    bool operator==(const end_sentinel&) const noexcept { return the_range_->S_.empty(); }


  private:
    mutable internal_value value_;
    dfs_range_type*        the_range_ = nullptr;
    friend end_sentinel;
  };

  auto begin() { return iterator(*this); }
  auto begin() const { return iterator(*this); }
  auto cbegin() const { return iterator(*this); }

  auto end() { return end_sentinel{}; }
  auto end() const { return end_sentinel{}; }
  auto cend() const { return end_sentinel{}; }
};
#line 423 "graph/views/depth_first_search.hpp"
template <adjacency_list G, class EVF = void, bool Sourced = false, class Alloc = allocator<bool>>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>
class edges_depth_first_search_view : public dfs_base<G, Alloc> {
public:
  using base_type           = dfs_base<G, Alloc>;
  using graph_type          = G;
  using vertex_id_type      = vertex_id_t<graph_type>;
  using vertex_iterator     = vertex_iterator_t<graph_type>;
  using edge_reference_type = edge_reference_t<graph_type>;
  using dfs_range_type      = edges_depth_first_search_view<G, EVF, Sourced, Alloc>;

  using edge_value_func = remove_reference_t<EVF>;
  using edge_value_type = invoke_result_t<EVF, edge_reference_type>;

public:
  edges_depth_first_search_view(G& g, vertex_id_type seed, const EVF& value_fn, const Alloc& alloc = Alloc())
        : base_type(g, seed, alloc), value_fn_(&value_fn) {}

  edges_depth_first_search_view()                                     = default;
  edges_depth_first_search_view(const edges_depth_first_search_view&) = delete;
  edges_depth_first_search_view(edges_depth_first_search_view&&)      = default;
  ~edges_depth_first_search_view()                                    = default;

  edges_depth_first_search_view& operator=(const edges_depth_first_search_view&) = delete;
  edges_depth_first_search_view& operator=(edges_depth_first_search_view&&)      = default;

  class iterator;
  struct end_sentinel {
    constexpr bool operator==(const iterator& rhs) const noexcept { return rhs.the_range_->S_.empty(); }

  };

  class iterator {
  public:
    using iterator_category = input_iterator_tag;
    using value_type        = edge_descriptor<const vertex_id_type, Sourced, edge_reference_type, edge_value_type>;
    using reference         = value_type&;
    using const_reference   = const value_type&;
    using rvalue_reference  = value_type&&;
    using pointer           = value_type*;
    using const_pointer     = value_type*;
    using size_type         = ranges::range_size_t<vertex_range_t<graph_type>>;
    using difference_type   = ranges::range_difference_t<vertex_range_t<graph_type>>;

  private:


    using shadow_edge_type = remove_reference_t<edge_reference_type>;
    using shadow_value_type =
          edge_descriptor<vertex_id_type, Sourced, shadow_edge_type*, _detail::ref_to_ptr<edge_value_type>>;

    union internal_value {
      value_type        value_;
      shadow_value_type shadow_;

      internal_value(const internal_value& rhs) : shadow_(rhs.shadow_) {}
      internal_value() : shadow_{} {}
      ~internal_value() {}
      internal_value& operator=(const internal_value& rhs) { value_.shadow = rhs.value_.shadow; }
    };

  public:
    iterator(const dfs_range_type& range) : the_range_(&const_cast<dfs_range_type&>(range)) {}
    iterator()                = default;
    iterator(const iterator&) = default;
    iterator(iterator&&)      = default;
    ~iterator()               = default;

    iterator& operator=(const iterator&) = default;
    iterator& operator=(iterator&&)      = default;

    iterator& operator++() {
      the_range_->advance();
      return *this;
    }
    iterator operator++(int) const {
      iterator temp(*this);
      ++*this;
      return temp;
    }

    reference operator*() const noexcept {
      auto&& [u_id, uvi] = the_range_->S_.top();
      if constexpr (Sourced) {
        value_.shadow_.source_id = u_id;
      }
      value_.shadow_.target_id = the_range_->real_target_id(*uvi, u_id);
      value_.shadow_.edge      = &*uvi;
      value_.shadow_.value     = invoke(*the_range_->value_fn_, *uvi);
      return value_.value_;
    }

    bool operator==(const end_sentinel&) const noexcept { return the_range_->S_.empty(); }


  private:
    mutable internal_value value_;
    dfs_range_type*        the_range_ = nullptr;
    friend end_sentinel;
  };

  auto begin() { return iterator(*this); }
  auto begin() const { return iterator(*this); }
  auto cbegin() const { return iterator(*this); }

  auto end() { return end_sentinel(); }
  auto end() const { return end_sentinel(); }
  auto cend() const { return end_sentinel(); }

private:
  const edge_value_func* value_fn_ = nullptr;
};

template <adjacency_list G, bool Sourced, class Alloc>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>
class edges_depth_first_search_view<G, void, Sourced, Alloc> : public dfs_base<G, Alloc> {
public:
  using base_type           = dfs_base<G, Alloc>;
  using graph_type          = G;
  using vertex_id_type      = vertex_id_t<graph_type>;
  using vertex_iterator     = vertex_iterator_t<graph_type>;
  using edge_reference_type = edge_reference_t<graph_type>;
  using dfs_range_type      = edges_depth_first_search_view<G, void, Sourced, Alloc>;

public:
  edges_depth_first_search_view(G& g, vertex_id_type seed, const Alloc& alloc = Alloc()) : base_type(g, seed, alloc) {}

  edges_depth_first_search_view()                                     = default;
  edges_depth_first_search_view(const edges_depth_first_search_view&) = delete;
  edges_depth_first_search_view(edges_depth_first_search_view&&)      = default;
  ~edges_depth_first_search_view()                                    = default;

  edges_depth_first_search_view& operator=(const edges_depth_first_search_view&) = delete;
  edges_depth_first_search_view& operator=(edges_depth_first_search_view&&)      = default;

  class iterator;
  struct end_sentinel {
    bool operator==(const end_sentinel& rhs) const noexcept { return rhs.the_range_->S_.empty(); }

  };

  class iterator {
  public:
    using iterator_category = input_iterator_tag;
    using value_type        = edge_descriptor<const vertex_id_type, Sourced, edge_reference_type, void>;
    using reference         = value_type&;
    using const_reference   = const value_type&;
    using rvalue_reference  = value_type&&;
    using pointer           = value_type*;
    using const_pointer     = value_type*;
    using size_type         = ranges::range_size_t<vertex_range_t<graph_type>>;
    using difference_type   = ranges::range_difference_t<vertex_range_t<graph_type>>;

  private:


    using shadow_edge_type  = remove_reference_t<edge_reference_type>;
    using shadow_value_type = edge_descriptor<vertex_id_type, Sourced, shadow_edge_type*, void>;

    union internal_value {
      value_type        value_;
      shadow_value_type shadow_;

      internal_value(const internal_value& rhs) : shadow_(rhs.shadow_) {}
      internal_value() : shadow_{} {}
      ~internal_value() {}
      internal_value& operator=(const internal_value& rhs) { value_.shadow = rhs.value_.shadow; }
    };

  public:
    iterator(const dfs_range_type& range) : the_range_(&const_cast<dfs_range_type&>(range)) {}
    iterator()                = default;
    iterator(const iterator&) = default;
    iterator(iterator&&)      = default;
    ~iterator()               = default;

    iterator& operator=(const iterator&) = default;
    iterator& operator=(iterator&&)      = default;

    iterator& operator++() {
      the_range_->advance();
      return *this;
    }
    iterator operator++(int) const {
      iterator temp(*this);
      ++*this;
      return temp;
    }

    reference operator*() const noexcept {
      auto&& [u_id, uvi] = the_range_->S_.top();
      if constexpr (Sourced) {
        value_.shadow_.source_id = u_id;
      }
      value_.shadow_.target_id = the_range_->real_target_id(*uvi, u_id);
      value_.shadow_.edge      = &*uvi;
      return value_.value_;
    }

    bool operator==(const end_sentinel&) const noexcept { return the_range_->S_.empty(); }


  private:
    mutable internal_value value_;
    dfs_range_type*        the_range_ = nullptr;
    friend end_sentinel;
  };

  auto begin() { return iterator(*this); }
  auto begin() const { return iterator(*this); }
  auto cbegin() const { return iterator(*this); }

  auto end() { return end_sentinel(); }
  auto end() const { return end_sentinel(); }
  auto cend() const { return end_sentinel(); }
};

namespace views {
#line 645 "graph/views/depth_first_search.hpp"
  namespace _Vertices_DFS {



    void vertices_depth_first_search();


    template <class _G, class _Alloc, class _UnCV>
    concept _Has_ref_ADL = _Has_class_or_enum_type<_G>
                           && requires(_G&& __g, const vertex_id_t<_G>& uid, _Alloc alloc) {
                                { _Fake_copy_init(vertices_depth_first_search(__g, uid, alloc)) };
                              };
    template <class _G, class _Alloc, class _UnCV>
    concept _Can_ref_eval = index_adjacency_list<_G>
                            && requires(_G&& __g, vertex_id_t<_G> uid, _Alloc alloc) {
                                 { _Fake_copy_init(vertices_depth_first_search_view<_G, void>(__g, uid, alloc)) };
                               };

    template <class _G, class _VVF, class _Alloc, class _UnCV>
    concept _Has_ref_vvf_ADL = _Has_class_or_enum_type<_G>
                               && invocable<_VVF, vertex_reference_t<_G>>
                               && requires(_G&& __g, const vertex_id_t<_G>& uid, _VVF vvf, _Alloc alloc) {
                                    {
                                      _Fake_copy_init(vertices_depth_first_search(__g, uid, vvf, alloc))
                                    };
                                  };
    template <class _G, class _VVF, class _Alloc, class _UnCV>
    concept _Can_ref_vvf_eval = index_adjacency_list<_G>
                                && invocable<_VVF, vertex_reference_t<_G>>
                                && requires(_G&& __g, vertex_id_t<_G> uid, _VVF vvf, _Alloc alloc) {
                                     {
                                       _Fake_copy_init(vertices_depth_first_search_view<_G, _VVF>(__g, uid, vvf, alloc))
                                     };
                                   };

    class _Cpo {
    private:
      enum class _St_ref { _None, _Non_member, _Auto_eval };
      enum class _St_ref_vvf { _None, _Non_member, _Auto_eval };

      template <class _G, class _Alloc>
      [[nodiscard]] static consteval _Choice_t<_St_ref> _Choose_ref() noexcept {

        using _UnCV = remove_cvref_t<_G>;

        if constexpr (_Has_ref_ADL<_G, _Alloc, _UnCV>) {
          return {_St_ref::_Non_member,
                  noexcept(_Fake_copy_init(vertices_depth_first_search(declval<_G>(), declval<vertex_id_t<_G>>(),
                                                                       declval<_Alloc>())))};
        } else if constexpr (_Can_ref_eval<_G, _Alloc, _UnCV>) {
          return {_St_ref::_Auto_eval, noexcept(_Fake_copy_init(vertices_depth_first_search_view<_G, void>(
                                             declval<_G>(), declval<vertex_id_t<_G>>(), declval<_Alloc>())))};
        } else {
          return {_St_ref::_None};
        }
      }

      template <class _G, class _Alloc>
      static constexpr _Choice_t<_St_ref> _Choice_ref = _Choose_ref<_G, _Alloc>();

      template <class _G, class _VVF, class _Alloc>
      [[nodiscard]] static consteval _Choice_t<_St_ref_vvf> _Choose_ref_vvf() noexcept {

        using _UnCV = remove_cvref_t<_G>;

        if constexpr (_Has_ref_vvf_ADL<_G, _VVF, _Alloc, _UnCV>) {
          return {_St_ref_vvf::_Non_member, noexcept(_Fake_copy_init(vertices_depth_first_search(
                                                  declval<_G>(), declval<vertex_id_t<_G>>(), declval<_VVF>(),
                                                  declval<_Alloc>())))};
        } else if constexpr (_Can_ref_vvf_eval<_G, _VVF, _Alloc, _UnCV>) {
          return {_St_ref_vvf::_Auto_eval,
                  noexcept(_Fake_copy_init(vertices_depth_first_search_view<_G, _VVF>(
                        declval<_G>(), declval<vertex_id_t<_G>>(), declval<_VVF>(), declval<_Alloc>())))};
        } else {
          return {_St_ref_vvf::_None};
        }
      }

      template <class _G, class _VVF, class _Alloc>
      static constexpr _Choice_t<_St_ref_vvf> _Choice_ref_vvf = _Choose_ref_vvf<_G, _VVF, _Alloc>();

    public:
#line 738 "graph/views/depth_first_search.hpp"
      template <class _G, class _Alloc = allocator<bool>>
      requires(_Choice_ref<_G&, _Alloc>._Strategy != _St_ref::_None)
      [[nodiscard]] constexpr auto operator()(_G&& __g, const vertex_id_t<_G>& seed, _Alloc alloc = _Alloc()) const
            noexcept(_Choice_ref<_G&, _Alloc>._No_throw) {
        constexpr _St_ref _Strat_ref = _Choice_ref<_G&, _Alloc>._Strategy;

        if constexpr (_Strat_ref == _St_ref::_Non_member) {
          return vertices_depth_first_search(__g, seed, alloc);
        } else if constexpr (_Strat_ref == _St_ref::_Auto_eval) {
          return vertices_depth_first_search_view<_G, void>(__g, seed, alloc);
        } else {
          static_assert(_Always_false<_G>, "The default implementation of "
                                           "vertices_depth_first_search(g,seed,alloc) cannot be evaluated and "
                                           "there is no override defined for the graph.");
        }
      }
#line 770 "graph/views/depth_first_search.hpp"
      template <class _G, class _VVF, class _Alloc = allocator<bool>>
      requires(_Choice_ref_vvf<_G&, _VVF, _Alloc>._Strategy != _St_ref_vvf::_None)
      [[nodiscard]] constexpr auto
      operator()(_G&& __g, const vertex_id_t<_G>& seed, _VVF&& vvf, _Alloc alloc = _Alloc()) const
            noexcept(_Choice_ref_vvf<_G&, _VVF, _Alloc>._No_throw) {
        constexpr _St_ref_vvf _Strat_ref_vvf = _Choice_ref_vvf<_G&, _VVF, _Alloc>._Strategy;

        if constexpr (_Strat_ref_vvf == _St_ref_vvf::_Non_member) {
          return vertices_depth_first_search(__g, seed, vvf, alloc);
        } else if constexpr (_Strat_ref_vvf == _St_ref_vvf::_Auto_eval) {
          return vertices_depth_first_search_view<_G, _VVF>(__g, seed, vvf, alloc);
        } else {
          static_assert(_Always_false<_G>, "The default implementation of "
                                           "vertices_depth_first_search(g,seed,vvf,alloc) cannot be evaluated and "
                                           "there is no override defined for the graph.");
        }
      }
    };
  }

  inline namespace _Cpos {
    inline constexpr _Vertices_DFS::_Cpo vertices_depth_first_search;
  }
#line 799 "graph/views/depth_first_search.hpp"
  namespace _Edges_DFS {



    void edges_depth_first_search();


    template <class _G, class _Alloc, class _UnCV>
    concept _Has_ref_ADL = _Has_class_or_enum_type<_G>
                           && requires(_G&& __g, const vertex_id_t<_G>& uid, _Alloc alloc) {
                                { _Fake_copy_init(edges_depth_first_search(__g, uid, alloc)) };
                              };
    template <class _G, class _Alloc, class _UnCV>
    concept _Can_ref_eval = index_adjacency_list<_G>
                            && requires(_G&& __g, vertex_id_t<_G> uid, _Alloc alloc) {
                                 { _Fake_copy_init(edges_depth_first_search_view<_G, void, false>(__g, uid, alloc)) };
                               };

    template <class _G, class _EVF, class _Alloc, class _UnCV>
    concept _Has_ref_evf_ADL = _Has_class_or_enum_type<_G>
                               && invocable<_EVF, edge_reference_t<_G>>
                               && requires(_G&& __g, const vertex_id_t<_G>& uid, _EVF evf, _Alloc alloc) {
                                    {
                                      _Fake_copy_init(edges_depth_first_search(__g, uid, evf, alloc))
                                    };
                                  };
    template <class _G, class _EVF, class _Alloc, class _UnCV>
    concept _Can_ref_evf_eval =
          index_adjacency_list<_G>
          && invocable<_EVF, edge_reference_t<_G>>
          && requires(_G&& __g, vertex_id_t<_G> uid, _EVF evf, _Alloc alloc) {
               { _Fake_copy_init(edges_depth_first_search_view<_G, _EVF, false>(__g, uid, evf, alloc)) };
             };

    class _Cpo {
    private:
      enum class _St_ref { _None, _Non_member, _Auto_eval };
      enum class _St_ref_evf { _None, _Non_member, _Auto_eval };

      template <class _G, class _Alloc>
      [[nodiscard]] static consteval _Choice_t<_St_ref> _Choose_ref() noexcept {

        using _UnCV = remove_cvref_t<_G>;

        if constexpr (_Has_ref_ADL<_G, _Alloc, _UnCV>) {
          return {_St_ref::_Non_member,
                  noexcept(_Fake_copy_init(edges_depth_first_search(declval<_G>(), declval<vertex_id_t<_G>>(),
                                                                    declval<_Alloc>())))};
        } else if constexpr (_Can_ref_eval<_G, _Alloc, _UnCV>) {
          return {_St_ref::_Auto_eval, noexcept(_Fake_copy_init(edges_depth_first_search_view<_G, void, false>(
                                             declval<_G>(), declval<vertex_id_t<_G>>(), declval<_Alloc>())))};
        } else {
          return {_St_ref::_None};
        }
      }

      template <class _G, class _Alloc>
      static constexpr _Choice_t<_St_ref> _Choice_ref = _Choose_ref<_G, _Alloc>();

      template <class _G, class _EVF, class _Alloc>
      [[nodiscard]] static consteval _Choice_t<_St_ref_evf> _Choose_ref_evf() noexcept {

        using _UnCV = remove_cvref_t<_G>;

        if constexpr (_Has_ref_evf_ADL<_G, _EVF, _Alloc, _UnCV>) {
          return {_St_ref_evf::_Non_member, noexcept(_Fake_copy_init(edges_depth_first_search(
                                                  declval<_G>(), declval<vertex_id_t<_G>>(), declval<_EVF>(),
                                                  declval<_Alloc>())))};
        } else if constexpr (_Can_ref_evf_eval<_G, _EVF, _Alloc, _UnCV>) {
          return {_St_ref_evf::_Auto_eval,
                  noexcept(_Fake_copy_init(edges_depth_first_search_view<_G, _EVF, false>(
                        declval<_G>(), declval<vertex_id_t<_G>>(), declval<_EVF>(), declval<_Alloc>())))};
        } else {
          return {_St_ref_evf::_None};
        }
      }

      template <class _G, class _EVF, class _Alloc>
      static constexpr _Choice_t<_St_ref_evf> _Choice_ref_evf = _Choose_ref_evf<_G, _EVF, _Alloc>();

    public:
#line 893 "graph/views/depth_first_search.hpp"
      template <class _G, class _Alloc = allocator<bool>>
      requires(_Choice_ref<_G&, _Alloc>._Strategy != _St_ref::_None)
      [[nodiscard]] constexpr auto operator()(_G&& __g, const vertex_id_t<_G>& seed, _Alloc alloc = _Alloc()) const
            noexcept(_Choice_ref<_G&, _Alloc>._No_throw) {
        constexpr _St_ref _Strat_ref = _Choice_ref<_G&, _Alloc>._Strategy;

        if constexpr (_Strat_ref == _St_ref::_Non_member) {
          return edges_depth_first_search(__g, seed, alloc);
        } else if constexpr (_Strat_ref == _St_ref::_Auto_eval) {
          return edges_depth_first_search_view<_G, void, false>(__g, seed, alloc);
        } else {
          static_assert(_Always_false<_G>, "The default implementation of "
                                           "edges_depth_first_search(g,seed,alloc) cannot be evaluated and "
                                           "there is no override defined for the graph.");
        }
      }
#line 925 "graph/views/depth_first_search.hpp"
      template <class _G, class _EVF, class _Alloc = allocator<bool>>
      requires(_Choice_ref_evf<_G&, _EVF, _Alloc>._Strategy != _St_ref_evf::_None)
      [[nodiscard]] constexpr auto
      operator()(_G&& __g, const vertex_id_t<_G>& seed, _EVF&& evf, _Alloc alloc = _Alloc()) const
            noexcept(_Choice_ref_evf<_G&, _EVF, _Alloc>._No_throw) {
        constexpr _St_ref_evf _Strat_ref_evf = _Choice_ref_evf<_G&, _EVF, _Alloc>._Strategy;

        if constexpr (_Strat_ref_evf == _St_ref_evf::_Non_member) {
          return edges_depth_first_search(__g, seed, alloc);
        } else if constexpr (_Strat_ref_evf == _St_ref_evf::_Auto_eval) {
          return edges_depth_first_search_view<_G, _EVF, false>(__g, seed, evf, alloc);
        } else {
          static_assert(_Always_false<_G>, "The default implementation of "
                                           "edges_depth_first_search(g,seed,evf,alloc) cannot be evaluated and "
                                           "there is no override defined for the graph.");
        }
      }
    };
  }

  inline namespace _Cpos {
    inline constexpr _Edges_DFS::_Cpo edges_depth_first_search;
  }
#line 954 "graph/views/depth_first_search.hpp"
  namespace _Sourced_Edges_DFS {



    void sourced_edges_depth_first_search();


    template <class _G, class _Alloc, class _UnCV>
    concept _Has_ref_ADL = _Has_class_or_enum_type<_G>
                           && requires(_G&& __g, const vertex_id_t<_G>& uid, _Alloc alloc) {
                                {
                                  _Fake_copy_init(sourced_edges_depth_first_search(__g, uid, alloc))
                                };
                              };
    template <class _G, class _Alloc, class _UnCV>
    concept _Can_ref_eval = index_adjacency_list<_G>
                            && requires(_G&& __g, vertex_id_t<_G> uid, _Alloc alloc) {
                                 { _Fake_copy_init(edges_depth_first_search_view<_G, void, true>(__g, uid, alloc)) };
                               };

    template <class _G, class _EVF, class _Alloc, class _UnCV>
    concept _Has_ref_evf_ADL = _Has_class_or_enum_type<_G>
                               && invocable<_EVF, edge_reference_t<_G>>
                               && requires(_G&& __g, const vertex_id_t<_G>& uid, _EVF evf, _Alloc alloc) {
                                    {
                                      _Fake_copy_init(sourced_edges_depth_first_search(__g, uid, evf, alloc))
                                    };
                                  };
    template <class _G, class _EVF, class _Alloc, class _UnCV>
    concept _Can_ref_evf_eval =
          index_adjacency_list<_G>
          && invocable<_EVF, edge_reference_t<_G>>
          && requires(_G&& __g, vertex_id_t<_G> uid, _EVF evf, _Alloc alloc) {
               { _Fake_copy_init(edges_depth_first_search_view<_G, _EVF, true>(__g, uid, evf, alloc)) };
             };

    class _Cpo {
    private:
      enum class _St_ref { _None, _Non_member, _Auto_eval };
      enum class _St_ref_evf { _None, _Non_member, _Auto_eval };

      template <class _G, class _Alloc>
      [[nodiscard]] static consteval _Choice_t<_St_ref> _Choose_ref() noexcept {

        using _UnCV = remove_cvref_t<_G>;

        if constexpr (_Has_ref_ADL<_G, _Alloc, _UnCV>) {
          return {_St_ref::_Non_member,
                  noexcept(_Fake_copy_init(sourced_edges_depth_first_search(declval<_G>(), declval<vertex_id_t<_G>>(),
                                                                            declval<_Alloc>())))};
        } else if constexpr (_Can_ref_eval<_G, _Alloc, _UnCV>) {
          return {_St_ref::_Auto_eval, noexcept(_Fake_copy_init(edges_depth_first_search_view<_G, void, true>(
                                             declval<_G>(), declval<vertex_id_t<_G>>(), declval<_Alloc>())))};
        } else {
          return {_St_ref::_None};
        }
      }

      template <class _G, class _Alloc>
      static constexpr _Choice_t<_St_ref> _Choice_ref = _Choose_ref<_G, _Alloc>();

      template <class _G, class _EVF, class _Alloc>
      [[nodiscard]] static consteval _Choice_t<_St_ref_evf> _Choose_ref_evf() noexcept {

        using _UnCV = remove_cvref_t<_G>;

        if constexpr (_Has_ref_evf_ADL<_G, _EVF, _Alloc, _UnCV>) {
          return {_St_ref_evf::_Non_member, noexcept(_Fake_copy_init(sourced_edges_depth_first_search(
                                                  declval<_G>(), declval<vertex_id_t<_G>>(), declval<_EVF>(),
                                                  declval<_Alloc>())))};
        } else if constexpr (_Can_ref_evf_eval<_G, _EVF, _Alloc, _UnCV>) {
          return {_St_ref_evf::_Auto_eval,
                  noexcept(_Fake_copy_init(edges_depth_first_search_view<_G, _EVF, true>(
                        declval<_G>(), declval<vertex_id_t<_G>>(), declval<_EVF>(), declval<_Alloc>())))};
        } else {
          return {_St_ref_evf::_None};
        }
      }

      template <class _G, class _EVF, class _Alloc>
      static constexpr _Choice_t<_St_ref_evf> _Choice_ref_evf = _Choose_ref_evf<_G, _EVF, _Alloc>();

    public:
#line 1050 "graph/views/depth_first_search.hpp"
      template <class _G, class _Alloc = allocator<bool>>
      requires(_Choice_ref<_G&, _Alloc>._Strategy != _St_ref::_None)
      [[nodiscard]] constexpr auto operator()(_G&& __g, const vertex_id_t<_G>& seed, _Alloc alloc = _Alloc()) const
            noexcept(_Choice_ref<_G&, _Alloc>._No_throw) {
        constexpr _St_ref _Strat_ref = _Choice_ref<_G&, _Alloc>._Strategy;

        if constexpr (_Strat_ref == _St_ref::_Non_member) {
          return sourced_edges_depth_first_search(__g, seed, alloc);
        } else if constexpr (_Strat_ref == _St_ref::_Auto_eval) {
          return edges_depth_first_search_view<_G, void, true>(__g, seed, alloc);
        } else {
          static_assert(_Always_false<_G>, "The default implementation of "
                                           "sourced_edges_depth_first_search(g,seed,alloc) cannot be evaluated and "
                                           "there is no override defined for the graph.");
        }
      }
#line 1082 "graph/views/depth_first_search.hpp"
      template <class _G, class _EVF, class _Alloc = allocator<bool>>
      requires(_Choice_ref_evf<_G&, _EVF, _Alloc>._Strategy != _St_ref_evf::_None)
      [[nodiscard]] constexpr auto
      operator()(_G&& __g, const vertex_id_t<_G>& seed, _EVF&& evf, _Alloc alloc = _Alloc()) const
            noexcept(_Choice_ref_evf<_G&, _EVF, _Alloc>._No_throw) {
        constexpr _St_ref_evf _Strat_ref_evf = _Choice_ref_evf<_G&, _EVF, _Alloc>._Strategy;

        if constexpr (_Strat_ref_evf == _St_ref_evf::_Non_member) {
          return sourced_edges_depth_first_search(__g, seed, alloc);
        } else if constexpr (_Strat_ref_evf == _St_ref_evf::_Auto_eval) {
          return edges_depth_first_search_view<_G, _EVF, true>(__g, seed, evf, alloc);
        } else {
          static_assert(_Always_false<_G>, "The default implementation of "
                                           "sourced_edges_depth_first_search(g,seed,evf,alloc) cannot be evaluated and "
                                           "there is no override defined for the graph.");
        }
      }
    };
  }

  inline namespace _Cpos {
    inline constexpr _Sourced_Edges_DFS::_Cpo sourced_edges_depth_first_search;
  }

}
}
#line 16 "graph/algorithm/shortest_paths.hpp"
#include <queue>
#include <vector>
#include <ranges>
#include <fmt/format.h>





namespace std::graph {

template <class G, class WF, class DistanceValue, class Compare, class Combine>
concept basic_edge_weight_function =
      is_arithmetic_v<DistanceValue> && strict_weak_order<Compare, DistanceValue, DistanceValue> &&
      assignable_from<add_lvalue_reference_t<DistanceValue>,
                      invoke_result_t<Combine, DistanceValue, invoke_result_t<WF, edge_reference_t<G>>>>;

template <class G, class WF, class DistanceValue>
concept edge_weight_function =
      is_arithmetic_v<invoke_result_t<WF, edge_reference_t<G>>> &&
      basic_edge_weight_function<G, WF, DistanceValue, less<DistanceValue>, plus<DistanceValue>>;
#line 47 "graph/algorithm/shortest_paths.hpp"
template <class DistanceValue>
constexpr auto shortest_path_invalid_distance() {
  return numeric_limits<DistanceValue>::max();
}
#line 60 "graph/algorithm/shortest_paths.hpp"
template <class DistanceValue>
constexpr auto shortest_path_zero() {
  return DistanceValue();
}
#line 73 "graph/algorithm/shortest_paths.hpp"
template <class Distances>
constexpr void init_shortest_paths(Distances& distances) {
  ranges::fill(distances, shortest_path_invalid_distance<ranges::range_value_t<Distances>>());
}
#line 88 "graph/algorithm/shortest_paths.hpp"
template <class Distances, class Predecessors>
constexpr void init_shortest_paths(Distances& distances, Predecessors& predecessors) {
  init_shortest_paths(distances);

  using pred_t = ranges::range_value_t<Predecessors>;
  pred_t i     = pred_t();
  for (auto& pred : predecessors)
    pred = i++;
}
#line 106 "graph/algorithm/shortest_paths.hpp"
class _null_range_type : public std::vector<size_t> {
  using T         = size_t;
  using Allocator = std::allocator<T>;
  using Base      = std::vector<T, Allocator>;

public:
  _null_range_type() noexcept(noexcept(Allocator())) = default;
  explicit _null_range_type(const Allocator& alloc) noexcept {}
  _null_range_type(Base::size_type count, const T& value, const Allocator& alloc = Allocator()) {}
  explicit _null_range_type(Base::size_type count, const Allocator& alloc = Allocator()) {}
  template <class InputIt>
  _null_range_type(InputIt first, InputIt last, const Allocator& alloc = Allocator()) {}
  _null_range_type(const _null_range_type& other) : Base() {}
  _null_range_type(const _null_range_type& other, const Allocator& alloc) {}
  _null_range_type(_null_range_type&& other) noexcept {}
  _null_range_type(_null_range_type&& other, const Allocator& alloc) {}
  _null_range_type(std::initializer_list<T> init, const Allocator& alloc = Allocator()) {}
};

inline static _null_range_type _null_predecessors;
#line 147 "graph/algorithm/shortest_paths.hpp"
template <index_adjacency_list        G,
          ranges::random_access_range Distances,
          ranges::random_access_range Predecessors,
          class WF        = function<ranges::range_value_t<Distances>(edge_reference_t<G>)>,
          class Allocator = allocator<vertex_id_t<G>>
          >
requires is_arithmetic_v<ranges::range_value_t<Distances>> &&
         convertible_to<vertex_id_t<G>, ranges::range_value_t<Predecessors>> &&
         edge_weight_function<G, WF, ranges::range_value_t<Distances>>
void dijkstra_shortest_paths(
      G&&            g,
      vertex_id_t<G> source,
      Distances&     distances,
      Predecessors&  predecessors,
      WF&&           weight =
            [](edge_reference_t<G> uv) { return ranges::range_value_t<Distances>(1); },
      Allocator alloc = Allocator())
{
  using id_type     = vertex_id_t<G>;
  using weight_type = invoke_result_t<WF, edge_reference_t<G>>;

  size_t N(num_vertices(g));
  if (source < 0 || source >= N) {
    throw out_of_range(fmt::format("source {} is outside the vertices range [{},{})", source, 0, N));
  }


  distances[source] = 0;

  struct weighted_vertex {
    id_type     vertex_id = id_type();
    weight_type weight    = weight_type();
  };

  using q_compare = decltype([](const weighted_vertex& a, const weighted_vertex& b) { return a.weight > b.weight; });
  priority_queue<weighted_vertex, vector<weighted_vertex>, q_compare> Q(alloc);


  Q.push({source, distances[source]});

  while (!Q.empty()) {
    auto uid = Q.top().vertex_id;
    Q.pop();

    for (auto&& [vid, uv, w] : views::incidence(g, uid, weight)) {
      if (distances[uid] + w < distances[vid]) {
        distances[vid] = distances[uid] + w;
        if constexpr (!is_same_v<Predecessors, _null_range_type>)
          predecessors[vid] = uid;
        Q.push({vid, distances[vid]});
      }
    }
  }
}
#line 217 "graph/algorithm/shortest_paths.hpp"
template <index_adjacency_list        G,
          ranges::random_access_range Distances,
          class WF        = std::function<ranges::range_value_t<Distances>(edge_reference_t<G>)>,
          class Allocator = allocator<vertex_id_t<G>>
          >
requires is_arithmetic_v<ranges::range_value_t<Distances>> &&
         edge_weight_function<G, WF, ranges::range_value_t<Distances>>
void dijkstra_shortest_distances(
      G&&            g,
      vertex_id_t<G> seed,
      Distances&     distances,
      WF&&           weight =
            [](edge_reference_t<G> uv) { return ranges::range_value_t<Distances>(1); },
      Allocator alloc = Allocator())
{
  dijkstra_shortest_paths(g, seed, distances, _null_predecessors, forward<WF>(weight),
                          alloc);
}
#line 255 "graph/algorithm/shortest_paths.hpp"
template <index_adjacency_list        G,
          ranges::random_access_range Distances,
          ranges::random_access_range Predecessors,
          class Compare,
          class Combine,
          class WF        = function<ranges::range_value_t<Distances>(edge_reference_t<G>)>,
          class Allocator = allocator<vertex_id_t<G>>
          >
requires is_arithmetic_v<ranges::range_value_t<Distances>> &&
         convertible_to<vertex_id_t<G>, ranges::range_value_t<Predecessors>> &&
         basic_edge_weight_function<G, WF, ranges::range_value_t<Distances>, Compare, Combine>
void dijkstra_shortest_paths(
      G&&            g,
      vertex_id_t<G> source,
      Distances&     distances,
      Predecessors&  predecessors,
      Compare&&      compare,
      Combine&&      combine,
      WF&&           weight =
            [](edge_reference_t<G> uv) { return ranges::range_value_t<Distances>(1); },
      Allocator alloc = Allocator())
{
  using id_type     = vertex_id_t<G>;
  using weight_type = invoke_result_t<WF, edge_reference_t<G>>;

  const size_t N(num_vertices(g));
  if (source < 0 || source >= N) {
    throw out_of_range(fmt::format("source {} is outside the vertices range [{},{})", source, 0, N));
  }


  distances[source] = 0;

  struct weighted_vertex {
    id_type     vertex_id = id_type();
    weight_type weight    = weight_type();
  };

  using q_compare = decltype([](const weighted_vertex& a, const weighted_vertex& b) { return a.weight > b.weight; });
  priority_queue<weighted_vertex, vector<weighted_vertex>, q_compare> Q(alloc);


  Q.push({source, distances[source]});

  while (!Q.empty()) {
    auto uid = Q.top().vertex_id;
    Q.pop();

    for (auto&& [vid, uv, w] : views::incidence(g, uid, weight)) {
      if (compare(combine(distances[uid], w), distances[vid])) {
        distances[vid] = combine(distances[uid], w);
        if constexpr (!is_same_v<Predecessors, _null_range_type>)
          predecessors[vid] = uid;
        Q.push({vid, distances[vid]});
      }
    }
  }
}
#line 329 "graph/algorithm/shortest_paths.hpp"
template <index_adjacency_list        G,
          ranges::random_access_range Distances,
          class Compare,
          class Combine,
          class WF        = std::function<ranges::range_value_t<Distances>(edge_reference_t<G>)>,
          class Allocator = allocator<vertex_id_t<G>>
          >
requires is_arithmetic_v<ranges::range_value_t<Distances>> &&
         basic_edge_weight_function<G, WF, ranges::range_value_t<Distances>, Compare, Combine>
void dijkstra_shortest_distances(
      G&&            g,
      vertex_id_t<G> seed,
      Distances&     distances,
      Compare&&      compare,
      Combine&&      combine,
      WF&&           weight =
            [](edge_reference_t<G> uv) { return ranges::range_value_t<Distances>(1); },
      Allocator alloc = Allocator())
{
  dijkstra_shortest_paths(g, seed, distances, _null_predecessors, compare, combine, forward<WF>(weight),
                          alloc);
}


}
#line 20 "graph/views/incidence.hpp"
namespace std::graph {


template <adjacency_list G, bool Sourced = false, class EVF = void>
class incidence_iterator;
#line 32 "graph/views/incidence.hpp"
template <adjacency_list G, bool Sourced, class EVF>
class incidence_iterator : source_vertex<G, ((Sourced && !sourced_adjacency_list<G>) || unordered_edge<G, edge_t<G>>)> {
public:
  using base_type = source_vertex<G, ((Sourced && !sourced_adjacency_list<G>) || unordered_edge<G, edge_t<G>>)>;

  using graph_type      = remove_reference_t<G>;
  using vertex_type     = vertex_t<graph_type>;
  using vertex_id_type  = vertex_id_t<graph_type>;
  using vertex_iterator = vertex_iterator_t<graph_type>;

  using edge_range          = vertex_edge_range_t<graph_type>;
  using edge_iterator       = vertex_edge_iterator_t<graph_type>;
  using edge_type           = edge_t<graph_type>;
  using edge_reference_type = edge_reference_t<graph_type>;
  using edge_value_type     = invoke_result_t<EVF, edge_reference_type>;

  using iterator_category = forward_iterator_tag;
  using value_type        = edge_descriptor<const vertex_id_type, Sourced, edge_reference_type, edge_value_type>;
  using difference_type   = ranges::range_difference_t<edge_range>;
  using pointer           = value_type*;
  using const_pointer     = const value_type*;
  using reference         = value_type&;
  using const_reference   = const value_type&;
  using rvalue_reference  = value_type&&;

public:
  incidence_iterator(graph_type& g, vertex_iterator ui, edge_iterator iter, const EVF& value_fn)
        : base_type(vertex_id(g, ui)), g_(&g), iter_(iter), value_fn_(&value_fn) {}
  incidence_iterator(graph_type& g, vertex_id_type uid, const EVF& value_fn)
        : base_type(uid), g_(&g), iter_(ranges::begin(edges(g, uid))), value_fn_(&value_fn) {}

  constexpr incidence_iterator()                          = default;
  constexpr incidence_iterator(const incidence_iterator&) = default;
  constexpr incidence_iterator(incidence_iterator&&)      = default;
  constexpr ~incidence_iterator()                         = default;

  constexpr incidence_iterator& operator=(const incidence_iterator&) = default;
  constexpr incidence_iterator& operator=(incidence_iterator&&)      = default;

protected:


  using shadow_edge_type = remove_reference_t<edge_reference_type>;
  using shadow_value_type =
        edge_descriptor<vertex_id_type, Sourced, shadow_edge_type*, _detail::ref_to_ptr<edge_value_type>>;

  union internal_value {
    value_type        value_;
    shadow_value_type shadow_;

    internal_value(const internal_value& rhs) : shadow_(rhs.shadow_) {}
    internal_value() : shadow_{} {}
    ~internal_value() {}
    internal_value& operator=(const internal_value& rhs) { value_.shadow = rhs.value_.shadow; }
  };

public:
  constexpr reference operator*() const {
    if constexpr (unordered_edge<G, edge_type>) {
      if (target_id(*g_, *iter_) != this->source_vertex_id()) {
        value_.shadow_.source_id = source_id(*g_, *iter_);
        value_.shadow_.target_id = target_id(*g_, *iter_);
      } else {
        value_.shadow_.source_id = target_id(*g_, *iter_);
        value_.shadow_.target_id = source_id(*g_, *iter_);
      }
    } else if constexpr (Sourced) {
      if constexpr (sourced_adjacency_list<G>) {
        value_.shadow_.source_id = source_id(*g_, *iter_);
        value_.shadow_.target_id = target_id(*g_, *iter_);
      } else {
        value_.shadow_.source_id = this->source_vertex_id();
        value_.shadow_.target_id = target_id(*g_, *iter_);
      }
    } else {
      value_.shadow_.target_id = target_id(*g_, *iter_);
    }
    value_.shadow_.edge  = &*iter_;
    value_.shadow_.value = invoke(*value_fn_, *iter_);
    return value_.value_;
  }

  constexpr incidence_iterator& operator++() {
    ++iter_;
    return *this;
  }
  constexpr incidence_iterator operator++(int) const {
    incidence_iterator tmp(*this);
    ++*this;
    return tmp;
  }

  constexpr bool operator==(const incidence_iterator& rhs) const { return iter_ == rhs.iter_; }


private:

  mutable internal_value value_;
  graph_type*            g_ = nullptr;
  edge_iterator          iter_;
  const EVF*             value_fn_ = nullptr;

  friend bool operator==(const edge_iterator& lhs, const incidence_iterator& rhs) { return lhs == rhs.iter_; }
};


template <adjacency_list G, bool Sourced>
class incidence_iterator<G, Sourced, void>
      : public source_vertex<G, ((Sourced && !sourced_adjacency_list<G>) || unordered_edge<G, edge_t<G>>)> {
public:
  using base_type = source_vertex<G, ((Sourced && !sourced_adjacency_list<G>) || unordered_edge<G, edge_t<G>>)>;

  using graph_type      = remove_reference_t<G>;
  using vertex_type     = vertex_t<graph_type>;
  using vertex_id_type  = vertex_id_t<graph_type>;
  using vertex_iterator = vertex_iterator_t<graph_type>;

  using edge_range          = vertex_edge_range_t<graph_type>;
  using edge_iterator       = vertex_edge_iterator_t<graph_type>;
  using edge_type           = edge_t<graph_type>;
  using edge_reference_type = edge_reference_t<graph_type>;
  using edge_value_type     = void;

  using iterator_category = forward_iterator_tag;
  using value_type        = edge_descriptor<const vertex_id_type, Sourced, edge_reference_type, edge_value_type>;
  using difference_type   = ranges::range_difference_t<edge_range>;
  using pointer           = value_type*;
  using const_pointer     = const value_type*;
  using reference         = value_type&;
  using const_reference   = const value_type&;
  using rvalue_reference  = value_type&&;

protected:


  using shadow_edge_type  = remove_reference_t<edge_reference_type>;
  using shadow_value_type = edge_descriptor<vertex_id_type, Sourced, shadow_edge_type*, edge_value_type>;

  union internal_value {
    value_type        value_;
    shadow_value_type shadow_;

    internal_value(const internal_value& rhs) : shadow_(rhs.shadow_) {}
    internal_value() : shadow_{} {}
    ~internal_value() {}
    internal_value& operator=(const internal_value& rhs) { value_.shadow = rhs.value_.shadow; }
  };

public:
  incidence_iterator(graph_type& g, vertex_iterator ui, edge_iterator iter)
        : base_type(vertex_id(g, ui)), g_(&g), iter_(iter) {}
  incidence_iterator(graph_type& g, vertex_id_type uid) : base_type(uid), g_(&g), iter_(ranges::begin(edges(g, uid))) {}

  constexpr incidence_iterator()                          = default;
  constexpr incidence_iterator(const incidence_iterator&) = default;
  constexpr incidence_iterator(incidence_iterator&&)      = default;
  constexpr ~incidence_iterator()                         = default;

  constexpr incidence_iterator& operator=(const incidence_iterator&) = default;
  constexpr incidence_iterator& operator=(incidence_iterator&&)      = default;

public:
  constexpr reference operator*() const {
    if constexpr (unordered_edge<G, edge_type>) {
      static_assert(sourced_adjacency_list<G>);
      if (target_id(*g_, *iter_) != this->source_vertex_id()) {
        value_.shadow_.source_id = source_id(*g_.*iter_);
        value_.shadow_.target_id = target_id(*g_, *iter_);
      } else {
        value_.shadow_.source_id = target_id(*g_.*iter_);
        value_.shadow_.target_id = source_id(*g_, *iter_);
      }
    } else if constexpr (Sourced) {
      if constexpr (sourced_adjacency_list<G>) {
        value_.shadow_.source_id = source_id(*g_, *iter_);
        value_.shadow_.target_id = target_id(*g_, *iter_);
      } else {
        value_.shadow_.source_id = this->source_vertex_id();
        value_.target_id         = target_id(*g_, *iter_);
      }
    } else {
      value_.shadow_.target_id = target_id(*g_, *iter_);
    }
    value_.shadow_.edge = &*iter_;
    return value_.value_;
  }

  constexpr incidence_iterator& operator++() {
    ++iter_;
    return *this;
  }
  constexpr incidence_iterator operator++(int) const {
    incidence_iterator tmp(*this);
    ++*this;
    return tmp;
  }

  constexpr bool operator==(const incidence_iterator& rhs) const { return iter_ == rhs.iter_; }


private:
  mutable internal_value value_;
  graph_type*            g_ = nullptr;
  edge_iterator          iter_;

  friend bool operator==(const edge_iterator& lhs, const incidence_iterator& rhs) { return lhs == rhs.iter_; }
};

template <class G, bool Sourced, class EVF>
using incidence_view = ranges::subrange<incidence_iterator<G, Sourced, EVF>, vertex_edge_iterator_t<G>>;

namespace views {
#line 248 "graph/views/incidence.hpp"
  namespace _Incidence {



    void incidence();


    template <class _G, class _UnCV>
    concept _Has_id_ADL = adjacency_list<_G> && requires(_G&& __g, const vertex_id_t<_G>& uid) {
      { _Fake_copy_init(incidence(__g, uid)) };
    };
    template <class _G, class _UnCV>
    concept _Can_id_eval = adjacency_list<_G>;

    template <class _G, class _UnCV, class EVF>
    concept _Has_id_evf_ADL = adjacency_list<_G> && invocable<EVF, edge_reference_t<_G>> &&
                              requires(_G&& __g, const vertex_id_t<_G>& uid, const EVF& evf) {
                                { _Fake_copy_init(incidence(__g, uid, evf)) };
                              };
    template <class _G, class _UnCV, class EVF>
    concept _Can_id_evf_eval = adjacency_list<_G> && invocable<EVF, edge_reference_t<_G>>;

    class _Cpo {
    private:
      enum class _St_id { _None, _Non_member, _Auto_eval };


      template <class _G>
      [[nodiscard]] static consteval _Choice_t<_St_id> _Choose_id() noexcept {
        static_assert(is_lvalue_reference_v<_G>);
        using _UnCV = remove_cvref_t<_G>;

        if constexpr (_Has_id_ADL<_G, _UnCV>) {
          return {_St_id::_Non_member,
                  noexcept(_Fake_copy_init(incidence(declval<_G>(), declval<vertex_id_t<_G>>())))};
        } else if constexpr (_Can_id_eval<_G, _UnCV>) {
          return {_St_id::_Auto_eval,
                  noexcept(_Fake_copy_init(incidence_view<_G, false, void>(
                        incidence_iterator<_G, false, void>(declval<_G>(), declval<vertex_id_t<_G>>()),
                        ranges::end(edges(declval<_G>(), declval<vertex_id_t<_G>>())))))};
        } else {
          return {_St_id::_None};
        }
      }

      template <class _G>
      static constexpr _Choice_t<_St_id> _Choice_id = _Choose_id<_G>();

      template <class _G, class EVF>
      [[nodiscard]] static consteval _Choice_t<_St_id> _Choose_id_evf() noexcept {
        static_assert(is_lvalue_reference_v<_G>);
        using _UnCV = remove_cvref_t<_G>;

        if constexpr (_Has_id_evf_ADL<_G, _UnCV, EVF>) {
          return {_St_id::_Non_member, noexcept(_Fake_copy_init(incidence(declval<_G>(), declval<vertex_id_t<_G>>(),
                                                                          declval<EVF>())))};
        } else if constexpr (_Can_id_evf_eval<_G, _UnCV, EVF>) {
          return {_St_id::_Auto_eval,
                  noexcept(_Fake_copy_init(incidence_view<_G, false, EVF>(
                        incidence_iterator<_G, false, EVF>(declval<_G>(), declval<vertex_id_t<_G>>(), declval<EVF>()),
                        ranges::end(edges(declval<_G>(), declval<vertex_id_t<_G>>())))))};
        } else {
          return {_St_id::_None};
        }
      }

      template <class _G, class EVF>
      static constexpr _Choice_t<_St_id> _Choice_id_evf = _Choose_id_evf<_G, EVF>();

    public:
#line 332 "graph/views/incidence.hpp"
      template <class _G>
      requires(_Choice_id<_G&>._Strategy != _St_id::_None)
      [[nodiscard]] constexpr auto operator()(_G&& __g, const vertex_id_t<_G>& uid) const
            noexcept(_Choice_id<_G&>._No_throw) {
        constexpr _St_id _Strat_id = _Choice_id<_G&>._Strategy;

        if constexpr (_Strat_id == _St_id::_Non_member) {
          return incidence(__g, uid);
        } else if constexpr (_Strat_id == _St_id::_Auto_eval) {

          return incidence_view<_G, false, void>(incidence_iterator<_G, false, void>(__g, uid),
                                                 ranges::end(edges(__g, uid)));
        } else {
          static_assert(_Always_false<_G>,
                        "incidence(g,uid) is not defined and the default implementation cannot be evaluated");
        }
      }
#line 364 "graph/views/incidence.hpp"
      template <class _G, class EVF>
      requires(_Choice_id_evf<_G&, EVF>._Strategy != _St_id::_None)
      [[nodiscard]] constexpr auto operator()(_G&& __g, const vertex_id_t<_G>& uid, const EVF& evf) const
            noexcept(_Choice_id_evf<_G&, EVF>._No_throw) {
        constexpr _St_id _Strat_id = _Choice_id_evf<_G&, EVF>._Strategy;

        if constexpr (_Strat_id == _St_id::_Non_member) {
          return incidence(__g, uid);
        } else if constexpr (_Strat_id == _St_id::_Auto_eval) {

          return incidence_view<_G, false, EVF>(incidence_iterator<_G, false, EVF>(__g, uid, evf),
                                                ranges::end(edges(__g, uid)));
        } else {
          static_assert(_Always_false<_G>,
                        "incidence(g,uid,evf) is not defined and the default implementation cannot be evaluated");
        }
      }
    };
  }

  inline namespace _Cpos {
    inline constexpr _Incidence::_Cpo incidence;
  }
}
}
#line 5 "graph/algorithm/dijkstra_clrs.hpp"
#include <queue>

namespace std::graph {

template <class G, class F>
concept edge_weight_function =
      copy_constructible<F> && is_arithmetic_v<invoke_result_t<F, edge_reference_t<G>>>;
#line 18 "graph/algorithm/dijkstra_clrs.hpp"
template <class G, class WF, class Distance, class Compare, class Combine>
concept basic_edge_weight_function2 =
      is_arithmetic_v<ranges::range_value_t<Distance>> &&
      strict_weak_order<Compare, ranges::range_value_t<Distance>, ranges::range_value_t<Distance>> &&
      assignable_from<ranges::range_reference_t<Distance>, invoke_result_t<Combine, invoke_result_t<WF, edge_t<G>>>>;

template <class G, class WF, class Distance>
concept edge_weight_function2 =
      is_arithmetic_v<invoke_result_t<WF, edge_t<G>>> &&
      basic_edge_weight_function2<G,
                                  WF,
                                  Distance,
                                  less<ranges::range_value_t<Distance>>,
                                  plus<ranges::range_value_t<Distance>>>;


class null_range_type : public std::vector<size_t> {
  using T         = size_t;
  using Allocator = std::allocator<T>;
  using Base      = std::vector<T, Allocator>;

public:
  null_range_type() noexcept(noexcept(Allocator())) = default;
  explicit null_range_type(const Allocator& alloc) noexcept {}
  null_range_type(Base::size_type count, const T& value, const Allocator& alloc = Allocator()) {}
  explicit null_range_type(Base::size_type count, const Allocator& alloc = Allocator()) {}
  template <class InputIt>
  null_range_type(InputIt first, InputIt last, const Allocator& alloc = Allocator()) {}
  null_range_type(const null_range_type& other) : Base() {}
  null_range_type(const null_range_type& other, const Allocator& alloc) {}
  null_range_type(null_range_type&& other) noexcept {}
  null_range_type(null_range_type&& other, const Allocator& alloc) {}
  null_range_type(std::initializer_list<T> init, const Allocator& alloc = Allocator()) {}
};

inline static null_range_type null_predecessors;


template <class... Ts>
struct print_types_t;

template <class... Ts>
constexpr auto print_types(Ts...) {
  return print_types_t<Ts...>{};
}
#line 86 "graph/algorithm/dijkstra_clrs.hpp"
template <adjacency_list              G,
          ranges::random_access_range Distance,
          ranges::random_access_range Predecessor,
          class WF = std::function<ranges::range_value_t<Distance>(edge_reference_t<G>)>>
requires ranges::random_access_range<vertex_range_t<G>> &&
         integral<vertex_id_t<G>> &&
         is_arithmetic_v<ranges::range_value_t<Distance>> &&
         convertible_to<vertex_id_t<G>, ranges::range_value_t<Predecessor>> &&
         edge_weight_function<G, WF>
void dijkstra_clrs(
      G&&            g,
      vertex_id_t<G> seed,
      Distance&      distance,
      Predecessor&   predecessor,
      WF&&           weight =
            [](edge_reference_t<G> uv) { return ranges::range_value_t<Distance>(1); })
{
  using id_type     = vertex_id_t<G>;
  using weight_type = invoke_result_t<WF, edge_reference_t<G>>;



  size_t N(size(vertices(g)));
  assert(seed < N && seed >= 0);

  std::ranges::fill(distance, numeric_limits<weight_type>::max());
  distance[seed] = 0;

  struct weighted_vertex {
    id_type     vertex_id = id_type();
    weight_type weight    = weight_type();
  };



  using q_compare = decltype([](const weighted_vertex& a, const weighted_vertex& b) { return a.weight > b.weight; });
  std::priority_queue<weighted_vertex, vector<weighted_vertex>, q_compare> Q;


  Q.push({seed, distance[seed]});

  while (!Q.empty()) {

    auto uid = Q.top().vertex_id;
    Q.pop();

    for (auto&& [vid, uv, w] : views::incidence(g, uid, weight)) {

      if (distance[uid] + w < distance[vid]) {
        distance[vid] = distance[uid] + w;
        if constexpr (!is_same_v<Predecessor, null_range_type>)
          predecessor[vid] = uid;
        Q.push({vid, distance[vid]});
      }
    }
  }
}

template <index_adjacency_list        G,
          ranges::random_access_range Distance,
          ranges::random_access_range Predecessor,
          class WF = std::function<ranges::range_value_t<Distance>(edge_reference_t<G>)>>
requires is_arithmetic_v<ranges::range_value_t<Distance>> &&
         convertible_to<vertex_id_t<G>, ranges::range_value_t<Predecessor>> &&
         edge_weight_function<G, WF>
void dijkstra_clrs2(
      G&&            g,
      vertex_id_t<G> seed,
      Distance&      distance,
      Predecessor&   predecessor,
      WF&&           weight =
            [](edge_reference_t<G> uv) { return ranges::range_value_t<Distance>(1); })
{
  using id_type     = vertex_id_t<G>;
  using weight_type = invoke_result_t<WF, edge_reference_t<G>>;



  size_t N(size(vertices(g)));
  assert(seed < N && seed >= 0);

  std::ranges::fill(distance, numeric_limits<weight_type>::max());
  distance[seed] = 0;

  struct weighted_vertex {
    id_type     vertex_id = id_type();
    weight_type weight    = weight_type();
  };



  using q_compare = decltype([](const weighted_vertex& a, const weighted_vertex& b) { return a.weight > b.weight; });
  std::priority_queue<weighted_vertex, vector<weighted_vertex>, q_compare> Q;


  Q.push({seed, distance[seed]});

  while (!Q.empty()) {

    auto uid = Q.top().vertex_id;
    Q.pop();

    for (auto&& [vid, uv, w] : views::incidence(g, uid, weight)) {

      if (distance[uid] + w < distance[vid]) {
        distance[vid] = distance[uid] + w;
        if constexpr (!is_same_v<Predecessor, null_range_type>)
          predecessor[vid] = uid;
        Q.push({vid, distance[vid]});
      }
    }
  }
}

}
