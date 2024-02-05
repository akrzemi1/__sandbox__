#line 7 "boost/parser/parser.hpp"
#define BOOST_PARSER_PARSER_HPP
#undef BOOST_PARSER_DOXYGEN
#line 7 "boost/parser/parser_fwd.hpp"
#define BOOST_PARSER_PARSER_FWD_HPP
#line 7 "boost/parser/config.hpp"
#define BOOST_PARSER_CONFIG_HPP
#line 7 "boost/parser/detail/debug_assert.hpp"
#define BOOST_PARSER_DETAIL_DEBUG_ASSERT_HPP

#include <boost/assert.hpp>
#define BOOST_PARSER_DEBUG_ASSERT(condition) BOOST_ASSERT(condition)
#define BOOST_PARSER_HAVE_BOOST_ASSERT
#line 12 "boost/parser/config.hpp"
#include <iterator>
#line 58 "boost/parser/config.hpp"
#        define BOOST_PARSER_ASSERT(condition) BOOST_ASSERT(condition)
#line 68 "boost/parser/config.hpp"
#    define BOOST_PARSER_ALGO_CONSTEXPR





#    define BOOST_PARSER_USE_CONCEPTS 0
#line 7 "boost/parser/subrange.hpp"
#define BOOST_PARSER_SUBRANGE_HPP
#line 7 "boost/parser/detail/text/config.hpp"
#define BOOST_PARSER_DETAIL_TEXT_CONFIG_HPP
#line 12 "boost/parser/detail/text/config.hpp"
#include <iterator>



#    define BOOST_PARSER_DETAIL_TEXT_USE_CONCEPTS 0
#line 27 "boost/parser/detail/text/config.hpp"
#define BOOST_PARSER_DETAIL_TEXT_USE_ALIAS_CTAD 0
#line 7 "boost/parser/detail/text/detail/begin_end.hpp"
#define BOOST_PARSER_DETAIL_TEXT_BEGIN_END_HPP
#line 7 "boost/parser/detail/detection.hpp"
#define BOOST_PARSER_DETAIL_DETECTION_HPP

#include <type_traits>


namespace boost::parser::detail {

    template<typename...>
    struct void_
    {
        using type = void;
        static constexpr bool value = true;
    };

    template<typename... T>
    using void_t = typename void_<T...>::type;

    template<typename T>
    struct fixup_ptr
    {
        using type = T;
    };

    template<typename T>
    using remove_v_t = typename std::remove_volatile<T>::type;

    template<typename T>
    struct fixup_ptr<T *>
    {
        using type = remove_v_t<T> const *;
    };

    template<typename T>
    using fixup_ptr_t = typename fixup_ptr<T>::type;

    template<typename T>
    using remove_cv_ref_t =
        typename std::remove_cv<typename std::remove_reference<T>::type>::type;

    struct nonesuch
    {};

    template<
        typename Default,
        typename AlwaysVoid,
        template<typename...> class Template,
        typename... Args>
    struct detector
    {
        using value_t = std::false_type;
        using type = Default;
    };

    template<
        typename Default,
        template<typename...> class Template,
        typename... Args>
    struct detector<Default, void_t<Template<Args...>>, Template, Args...>
    {
        using value_t = std::true_type;
        using type = Template<Args...>;
    };

    template<template<typename...> class Template, typename... Args>
    using is_detected =
        typename detector<nonesuch, void, Template, Args...>::value_t;

    template<template<typename...> class Template, typename... Args>
    constexpr bool is_detected_v = is_detected<Template, Args...>::value;

    template<template<typename...> class Template, typename... Args>
    using detected_t =
        typename detector<nonesuch, void, Template, Args...>::type;

    template<
        typename Default,
        template<typename...> class Template,
        typename... Args>
    using detected_or_t =
        typename detector<Default, void, Template, Args...>::type;

}
#line 11 "boost/parser/detail/text/detail/begin_end.hpp"
#include <initializer_list>


namespace boost::parser::detail { namespace text { namespace detail {

    template<typename T>
    T decay_copy(T) noexcept;

    template<typename T>
    struct static_const
    {
        static constexpr T value{};
    };
    template<typename T>
    constexpr T static_const<T>::value;

    namespace begin_impl {
        template<typename T>
        void begin(T &&) = delete;
        template<typename T>
        void begin(std::initializer_list<T>) = delete;

        template<typename T>
        using member_begin_expr = decltype(std::declval<T &>().begin());
        template<typename T>
        using adl_begin_expr = decltype(begin(std::declval<T &>()));
        template<typename T>
        constexpr bool has_member_begin_v = is_detected_v<member_begin_expr, T>;
        template<typename T>
        constexpr bool has_adl_begin_v = is_detected_v<adl_begin_expr, T>;

        template<typename R>
        using member_return_t =
            decltype(detail::decay_copy(std::declval<R &>().begin()));
        template<typename R>
        using adl_return_t =
            decltype(detail::decay_copy(begin(std::declval<R &>)));

        struct impl
        {
            template<typename R, std::size_t N>
            void operator()(R (&&)[N]) const = delete;

            template<typename R, std::size_t N>
            constexpr R * operator()(R (&array)[N]) const
            {
                return array;
            }

            template<typename R>
            constexpr std::
                enable_if_t<has_member_begin_v<R>, member_return_t<R>>
                operator()(R && r) const
            {
                return r.begin();
            }

            template<typename R>
            constexpr std::enable_if_t<
                !has_member_begin_v<R> && has_adl_begin_v<R>,
                adl_return_t<R>>
            operator()(R && r) const
            {
                return begin(r);
            }
        };
    }
#line 85 "boost/parser/detail/text/detail/begin_end.hpp"
    namespace {
        constexpr auto & begin = static_const<begin_impl::impl>::value;
    }


    namespace end_impl {
        template<typename T>
        void end(T &&) = delete;
        template<typename T>
        void end(std::initializer_list<T>) = delete;

        template<typename T>
        using member_end_expr = decltype(std::declval<T &>().end());
        template<typename T>
        using adl_end_expr = decltype(end(std::declval<T &>()));
        template<typename T>
        constexpr bool has_member_end_v = is_detected_v<member_end_expr, T>;
        template<typename T>
        constexpr bool has_adl_end_v = is_detected_v<adl_end_expr, T>;

        template<typename R>
        using member_return_t =
            decltype(detail::decay_copy(std::declval<R &>().end()));
        template<typename R>
        using adl_return_t =
            decltype(detail::decay_copy(end(std::declval<R &>)));

        struct impl
        {
            template<typename R, std::size_t N>
            void operator()(R (&&)[N]) const = delete;

            template<typename R, std::size_t N>
            constexpr R * operator()(R (&array)[N]) const
            {
                return array + N;
            }

            template<typename R>
            constexpr std::enable_if_t<has_member_end_v<R>, member_return_t<R>>
            operator()(R && r) const
            {
                return r.end();
            }

            template<typename R>
            constexpr std::enable_if_t<
                !has_member_end_v<R> && has_adl_end_v<R>,
                adl_return_t<R>>
            operator()(R && r) const
            {
                return end(r);
            }
        };
    }
#line 147 "boost/parser/detail/text/detail/begin_end.hpp"
    namespace {
        constexpr auto & end = static_const<end_impl::impl>::value;
    }


}}}
#line 43 "boost/parser/detail/text/config.hpp"
#    define BOOST_PARSER_DETAIL_TEXT_SUBRANGE boost::parser::subrange


namespace boost::parser::detail { namespace text {



    using char8_type = char;

}}
#line 63 "boost/parser/detail/text/config.hpp"
#    define BOOST_PARSER_DETAIL_TEXT_NAMESPACE_V1 inline namespace v1
#    define BOOST_PARSER_DETAIL_TEXT_NAMESPACE_V2 namespace v2
#line 7 "boost/parser/detail/text/detail/algorithm.hpp"
#define BOOST_PARSER_DETAIL_TEXT_DETAIL_ALGORITHM_HPP
#line 7 "boost/parser/detail/text/concepts.hpp"
#define BOOST_PARSER_DETAIL_TEXT_CONCEPTS_HPP
#line 7 "boost/parser/detail/text/utf.hpp"
#define BOOST_PARSER_DETAIL_TEXT_UTF_HPP



#include <cstdint>
#include <type_traits>
#include <cstdint>


namespace boost::parser::detail { namespace text {


    enum class format { none = 0, utf8 = 1, utf16 = 2, utf32 = 4 };

    namespace detail {
        template<typename T>
        constexpr format format_of()
        {
            if constexpr (
                std::is_same_v<T, char>



            ) {
                return format::utf8;
            } else if (
                std::is_same_v<T, char16_t>



            ) {
                return format::utf16;
            } else {
                return format::utf32;
            }
        }
    }

}}
#line 221 "boost/parser/detail/text/concepts.hpp"
namespace boost::parser::detail { namespace text { namespace detail {
#line 242 "boost/parser/detail/text/concepts.hpp"
    template<typename T>
    using iterator_t = decltype(detail::begin(std::declval<T &>()));
    template<typename T>
    using sentinel_t = decltype(detail::end(std::declval<T &>()));
    template<typename T>
    using iter_value_t = typename std::iterator_traits<T>::value_type;
    template<typename T>
    using iter_reference_t = decltype(*std::declval<T &>());
    template<typename T>
    using range_value_t = iter_value_t<iterator_t<T>>;
    template<typename T>
    using range_reference_t = iter_reference_t<iterator_t<T>>;
    template<typename T>
    using range_difference_t = std::ptrdiff_t;

    template<typename T>
    constexpr bool code_unit_v =



        std::is_same_v<T, char16_t> || std::is_same_v<T, char32_t> ||
        std::is_same_v<T, char> || std::is_same_v<T, wchar_t>;



}}}
#line 7 "boost/parser/detail/text/detail/iterator.hpp"
#define BOOST_PARSER_DETAIL_TEXT_DETAIL_ITERATOR_HPP
#line 7 "boost/parser/detail/stl_interfaces/reverse_iterator.hpp"
#define BOOST_PARSER_DETAIL_STL_INTERFACES_REVERSE_ITERATOR_HPP
#line 7 "boost/parser/detail/stl_interfaces/iterator_interface.hpp"
#define BOOST_PARSER_DETAIL_STL_INTERFACES_ITERATOR_INTERFACE_HPP
#line 7 "boost/parser/detail/stl_interfaces/fwd.hpp"
#define BOOST_PARSER_DETAIL_STL_INTERFACES_FWD_HPP
#line 7 "boost/parser/detail/stl_interfaces/config.hpp"
#define BOOST_PARSER_DETAIL_STL_INTERFACES_CONFIG_HPP
#line 13 "boost/parser/detail/stl_interfaces/config.hpp"
#    define BOOST_PARSER_DETAIL_STL_INTERFACES_USE_CONCEPTS 0
#line 25 "boost/parser/detail/stl_interfaces/config.hpp"
#define BOOST_PARSER_USE_DEDUCED_THIS 0
#line 35 "boost/parser/detail/stl_interfaces/config.hpp"
#    define BOOST_PARSER_DETAIL_STL_INTERFACES_NAMESPACE_V1 inline namespace v1
#    define BOOST_PARSER_DETAIL_STL_INTERFACES_NAMESPACE_V2 namespace v2
#    define BOOST_PARSER_DETAIL_STL_INTERFACES_NAMESPACE_V3 namespace v3
#line 19 "boost/parser/detail/stl_interfaces/fwd.hpp"
namespace boost::parser::detail { namespace stl_interfaces {




    enum class element_layout : bool {
        discontiguous = false,
        contiguous = true
    };

    inline namespace v1 {

        namespace v1_dtl {
            template<typename... T>
            using void_t = void;

            template<typename Iter>
            using iter_difference_t =
                typename std::iterator_traits<Iter>::difference_type;

            template<typename Range, typename = void>
            struct iterator;
            template<typename Range>
            struct iterator<
                Range,
                void_t<decltype(std::declval<Range &>().begin())>>
            {
                using type = decltype(std::declval<Range &>().begin());
            };
            template<typename Range>
            using iterator_t = typename iterator<Range>::type;

            template<typename Range, typename = void>
            struct sentinel;
            template<typename Range>
            struct sentinel<
                Range,
                void_t<decltype(std::declval<Range &>().end())>>
            {
                using type = decltype(std::declval<Range &>().end());
            };
            template<typename Range>
            using sentinel_t = typename sentinel<Range>::type;

            template<typename Range>
            using range_difference_t = iter_difference_t<iterator_t<Range>>;

            template<typename Range>
            using common_range =
                std::is_same<iterator_t<Range>, sentinel_t<Range>>;

            template<typename Range, typename = void>
            struct decrementable_sentinel : std::false_type
            {
            };
            template<typename Range>
            struct decrementable_sentinel<
                Range,
                void_t<decltype(--std::declval<sentinel_t<Range> &>())>>
                : std::true_type
            {
            };
        }

    }
}}
#line 11 "boost/parser/detail/stl_interfaces/iterator_interface.hpp"
#include <utility>
#include <type_traits>





namespace boost::parser::detail { namespace stl_interfaces {



    struct access
    {


        template<typename D>
        static constexpr auto base(D & d) noexcept
            -> decltype(d.base_reference())
        {
            return d.base_reference();
        }
        template<typename D>
        static constexpr auto base(D const & d) noexcept
            -> decltype(d.base_reference())
        {
            return d.base_reference();
        }


    };
#line 48 "boost/parser/detail/stl_interfaces/iterator_interface.hpp"
    template<typename T>




    struct proxy_arrow_result

    {
        constexpr proxy_arrow_result(T const & value) noexcept(
            noexcept(T(value))) :
            value_(value)
        {}
        constexpr proxy_arrow_result(T && value) noexcept(
            noexcept(T(std::move(value)))) :
            value_(std::move(value))
        {}

        constexpr T const * operator->() const noexcept { return &value_; }
        constexpr T * operator->() noexcept { return &value_; }

    private:
        T value_;
    };

    namespace detail {
        template<typename Pointer, typename Reference, typename T>
        auto make_pointer(
            T && value,
            std::enable_if_t<
                std::is_pointer<Pointer>::value &&
                    std::is_reference<Reference>::value,
                int> = 0) -> decltype(std::addressof(value))
        {
            return std::addressof(value);
        }

        template<typename Pointer, typename Reference, typename T>
        auto make_pointer(
            T && value,
            std::enable_if_t<
                !std::is_pointer<Pointer>::value &&
                    !std::is_same<Pointer, void>::value &&
                    std::is_reference<Reference>::value,
                int> = 0)
        {
            return Pointer(std::forward<T>(value));
        }

        template<typename Pointer, typename IteratorConcept>
        struct pointer
        {
            using type = Pointer;
        };
        template<typename Pointer>
        struct pointer<Pointer, std::output_iterator_tag>
        {
            using type = void;
        };
        template<typename Pointer, typename IteratorConcept>
        using pointer_t = typename pointer<Pointer, IteratorConcept>::type;

        template<typename T, typename U>
        using interoperable = std::integral_constant<
            bool,
            (std::is_convertible<T, U>::value ||
             std::is_convertible<U, T>::value)>;

        template<typename T, typename U>
        using common_t =
            std::conditional_t<std::is_convertible<T, U>::value, U, T>;

        template<typename T>
        using use_base = decltype(access::base(std::declval<T &>()));

        template<typename... T>
        using void_t = void;

        template<
            typename AlwaysVoid,
            template<class...> class Template,
            typename... Args>
        struct detector : std::false_type
        {
        };

        template<template<class...> class Template, typename... Args>
        struct detector<void_t<Template<Args...>>, Template, Args...>
            : std::true_type
        {
        };

        template<
            typename T,
            typename U,
            bool UseBase = detector<void, use_base, T>::value>
        struct common_eq
        {
            static constexpr auto call(T lhs, U rhs)
            {
                return static_cast<common_t<T, U>>(lhs).derived() ==
                       static_cast<common_t<T, U>>(rhs).derived();
            }
        };
        template<typename T, typename U>
        struct common_eq<T, U, true>
        {
            static constexpr auto call(T lhs, U rhs)
            {
                return access::base(lhs) == access::base(rhs);
            }
        };

        template<typename T, typename U>
        constexpr auto common_diff(T lhs, U rhs) noexcept(noexcept(
            static_cast<common_t<T, U>>(lhs) -
            static_cast<common_t<T, U>>(rhs)))
            -> decltype(
                static_cast<common_t<T, U>>(lhs) -
                static_cast<common_t<T, U>>(rhs))
        {
            return static_cast<common_t<T, U>>(lhs) -
                   static_cast<common_t<T, U>>(rhs);
        }
    }

}}

namespace boost::parser::detail { namespace stl_interfaces { inline namespace v1 {
#line 185 "boost/parser/detail/stl_interfaces/iterator_interface.hpp"
    template<
        typename Derived,
        typename IteratorConcept,
        typename ValueType,
        typename Reference = ValueType &,
        typename Pointer = ValueType *,
        typename DifferenceType = std::ptrdiff_t

        ,
        typename E = std::enable_if_t<
            std::is_class<Derived>::value &&
            std::is_same<Derived, std::remove_cv_t<Derived>>::value>

        >
    struct iterator_interface;

    namespace v1_dtl {
        template<typename Iterator, typename = void>
        struct ra_iter : std::false_type
        {
        };
        template<typename Iterator>
        struct ra_iter<Iterator, void_t<typename Iterator::iterator_concept>>
            : std::integral_constant<
                  bool,
                  std::is_base_of<
                      std::random_access_iterator_tag,
                      typename Iterator::iterator_concept>::value>
        {
        };

        template<typename Iterator, typename DifferenceType, typename = void>
        struct plus_eq : std::false_type
        {
        };
        template<typename Iterator, typename DifferenceType>
        struct plus_eq<
            Iterator,
            DifferenceType,
            void_t<decltype(
                std::declval<Iterator &>() += std::declval<DifferenceType>())>>
            : std::true_type
        {
        };

        template<
            typename D,
            typename IteratorConcept,
            typename ValueType,
            typename Reference,
            typename Pointer,
            typename DifferenceType>
        void derived_iterator(iterator_interface<
                              D,
                              IteratorConcept,
                              ValueType,
                              Reference,
                              Pointer,
                              DifferenceType> const &);
    }

    template<
        typename Derived,
        typename IteratorConcept,
        typename ValueType,
        typename Reference,
        typename Pointer,
        typename DifferenceType

        ,
        typename E

        >
    struct iterator_interface
    {

    private:
        constexpr Derived & derived() noexcept
        {
            return static_cast<Derived &>(*this);
        }
        constexpr Derived const & derived() const noexcept
        {
            return static_cast<Derived const &>(*this);
        }

        template<typename T, typename U, bool UseBase>
        friend struct detail::common_eq;


    public:
        using iterator_concept = IteratorConcept;
        using iterator_category = iterator_concept;
        using value_type = std::remove_const_t<ValueType>;
        using reference = Reference;
        using pointer = detail::pointer_t<Pointer, iterator_concept>;
        using difference_type = DifferenceType;

        template<typename D = Derived>
        constexpr auto operator*()
            noexcept(noexcept(*access::base(std::declval<D &>())))
                -> decltype(*access::base(std::declval<D &>()))
        {
            return *access::base(derived());
        }
        template<typename D = Derived>
        constexpr auto operator*() const
            noexcept(noexcept(*access::base(std::declval<D const &>())))
                -> decltype(*access::base(std::declval<D const &>()))
        {
            return *access::base(derived());
        }

        template<typename D = Derived>
        constexpr auto operator->() noexcept(noexcept(
            detail::make_pointer<pointer, reference>(*std::declval<D &>())))
            -> decltype(
                detail::make_pointer<pointer, reference>(*std::declval<D &>()))
        {
            return detail::make_pointer<pointer, reference>(*derived());
        }
        template<typename D = Derived>
        constexpr auto operator->() const noexcept(noexcept(
            detail::make_pointer<pointer, reference>(
                *std::declval<D const &>())))
            -> decltype(
                detail::make_pointer<pointer, reference>(
                    *std::declval<D const &>()))
        {
            return detail::make_pointer<pointer, reference>(*derived());
        }

        template<typename D = Derived>
        constexpr auto operator[](difference_type i) const noexcept(noexcept(
            D(std::declval<D const &>()),
            std::declval<D &>() += i,
            *std::declval<D &>()))
            -> decltype(std::declval<D &>() += i, *std::declval<D &>())
        {
            D retval = derived();
            retval += i;
            return *retval;
        }

        template<
            typename D = Derived,
            typename Enable =
                std::enable_if_t<!v1_dtl::plus_eq<D, difference_type>::value>>
        constexpr auto
        operator++() noexcept(noexcept(++access::base(std::declval<D &>())))
            -> decltype(
                ++access::base(std::declval<D &>()), std::declval<D &>())
        {
            ++access::base(derived());
            return derived();
        }

        template<typename D = Derived>
        constexpr auto operator++() noexcept(
            noexcept(std::declval<D &>() += difference_type(1)))
            -> decltype(
                std::declval<D &>() += difference_type(1), std::declval<D &>())
        {
            derived() += difference_type(1);
            return derived();
        }
        template<typename D = Derived>
        constexpr auto operator++(int)noexcept(
            noexcept(D(std::declval<D &>()), ++std::declval<D &>()))
            -> std::remove_reference_t<decltype(
                D(std::declval<D &>()),
                ++std::declval<D &>(),
                std::declval<D &>())>
        {
            D retval = derived();
            ++derived();
            return retval;
        }

        template<typename D = Derived>
        constexpr auto operator+=(difference_type n) noexcept(
            noexcept(access::base(std::declval<D &>()) += n))
            -> decltype(
                access::base(std::declval<D &>()) += n, std::declval<D &>())
        {
            access::base(derived()) += n;
            return derived();
        }

        template<typename D = Derived>
        constexpr auto operator+(difference_type i) const
            noexcept(noexcept(D(std::declval<D &>()), std::declval<D &>() += i))
                -> std::remove_reference_t<decltype(
                    D(std::declval<D &>()),
                    std::declval<D &>() += i,
                    std::declval<D &>())>
        {
            D retval = derived();
            retval += i;
            return retval;
        }
        friend constexpr Derived
        operator+(difference_type i, Derived it) noexcept
        {
            return it + i;
        }

        template<
            typename D = Derived,
            typename Enable =
                std::enable_if_t<!v1_dtl::plus_eq<D, difference_type>::value>>
        constexpr auto
        operator--() noexcept(noexcept(--access::base(std::declval<D &>())))
            -> decltype(--access::base(std::declval<D &>()), std::declval<D &>())
        {
            --access::base(derived());
            return derived();
        }

        template<typename D = Derived>
        constexpr auto operator--() noexcept(noexcept(
            D(std::declval<D &>()), std::declval<D &>() += -difference_type(1)))
            -> decltype(
                std::declval<D &>() += -difference_type(1), std::declval<D &>())
        {
            derived() += -difference_type(1);
            return derived();
        }
        template<typename D = Derived>
        constexpr auto operator--(int)noexcept(
            noexcept(D(std::declval<D &>()), --std::declval<D &>()))
            -> std::remove_reference_t<decltype(
                D(std::declval<D &>()),
                --std::declval<D &>(),
                std::declval<D &>())>
        {
            D retval = derived();
            --derived();
            return retval;
        }

        template<typename D = Derived>
        constexpr D & operator-=(difference_type i) noexcept
        {
            derived() += -i;
            return derived();
        }

        template<typename D = Derived>
        constexpr auto operator-(D other) const noexcept(noexcept(
            access::base(std::declval<D const &>()) - access::base(other)))
            -> decltype(
                access::base(std::declval<D const &>()) - access::base(other))
        {
            return access::base(derived()) - access::base(other);
        }

        friend constexpr Derived
        operator-(Derived it, difference_type i) noexcept
        {
            Derived retval = it;
            retval += -i;
            return retval;
        }
    };





    template<
        typename IteratorInterface1,
        typename IteratorInterface2,
        typename Enable =
            std::enable_if_t<!v1_dtl::ra_iter<IteratorInterface1>::value>>
    constexpr auto
    operator==(IteratorInterface1 lhs, IteratorInterface2 rhs) noexcept
        -> decltype(
            access::base(std::declval<IteratorInterface1 &>()) ==
            access::base(std::declval<IteratorInterface2 &>()))
    {
        return access::base(lhs) == access::base(rhs);
    }




    template<
        typename IteratorInterface1,
        typename IteratorInterface2,
        typename Enable =
            std::enable_if_t<v1_dtl::ra_iter<IteratorInterface1>::value>>
    constexpr auto
    operator==(IteratorInterface1 lhs, IteratorInterface2 rhs) noexcept(
        noexcept(detail::common_diff(lhs, rhs)))
        -> decltype(
            v1_dtl::derived_iterator(lhs), detail::common_diff(lhs, rhs) == 0)
    {
        return detail::common_diff(lhs, rhs) == 0;
    }



    template<typename IteratorInterface1, typename IteratorInterface2>
    constexpr auto operator!=(
        IteratorInterface1 lhs,
        IteratorInterface2 rhs) noexcept(noexcept(!(lhs == rhs)))
        -> decltype(v1_dtl::derived_iterator(lhs), !(lhs == rhs))
    {
        return !(lhs == rhs);
    }




    template<typename IteratorInterface1, typename IteratorInterface2>
    constexpr auto
    operator<(IteratorInterface1 lhs, IteratorInterface2 rhs) noexcept(
        noexcept(detail::common_diff(lhs, rhs)))
        -> decltype(
            v1_dtl::derived_iterator(lhs), detail::common_diff(lhs, rhs) < 0)
    {
        return detail::common_diff(lhs, rhs) < 0;
    }




    template<typename IteratorInterface1, typename IteratorInterface2>
    constexpr auto
    operator<=(IteratorInterface1 lhs, IteratorInterface2 rhs) noexcept(
        noexcept(detail::common_diff(lhs, rhs)))
        -> decltype(
            v1_dtl::derived_iterator(lhs), detail::common_diff(lhs, rhs) <= 0)
    {
        return detail::common_diff(lhs, rhs) <= 0;
    }




    template<typename IteratorInterface1, typename IteratorInterface2>
    constexpr auto
    operator>(IteratorInterface1 lhs, IteratorInterface2 rhs) noexcept(
        noexcept(detail::common_diff(lhs, rhs)))
        -> decltype(
            v1_dtl::derived_iterator(lhs), detail::common_diff(lhs, rhs) > 0)
    {
        return detail::common_diff(lhs, rhs) > 0;
    }




    template<typename IteratorInterface1, typename IteratorInterface2>
    constexpr auto
    operator>=(IteratorInterface1 lhs, IteratorInterface2 rhs) noexcept(
        noexcept(detail::common_diff(lhs, rhs)))
        -> decltype(
            v1_dtl::derived_iterator(lhs), detail::common_diff(lhs, rhs) >= 0)
    {
        return detail::common_diff(lhs, rhs) >= 0;
    }




    template<
        typename Derived,
        typename IteratorConcept,
        typename ValueType,
        typename Reference = ValueType,
        typename DifferenceType = std::ptrdiff_t>
    using proxy_iterator_interface = iterator_interface<
        Derived,
        IteratorConcept,
        ValueType,
        Reference,
        proxy_arrow_result<Reference>,
        DifferenceType>;

}}}
#line 12 "boost/parser/detail/stl_interfaces/reverse_iterator.hpp"
namespace boost::parser::detail { namespace stl_interfaces { inline namespace v1 {

    namespace v1_dtl {
        template<typename Iter>
        constexpr auto ce_dist(Iter f, Iter l, std::random_access_iterator_tag)
            -> decltype(l - f)
        {
            return l - f;
        }
        template<typename Iter, typename Tag>
        constexpr auto ce_dist(Iter f, Iter l, Tag)
            -> decltype(std::distance(f, l))
        {
            decltype(std::distance(f, l)) retval = 0;
            for (; f != l; ++f) {
                ++retval;
            }
            return retval;
        }

        template<typename Iter>
        constexpr Iter ce_prev(Iter it)
        {
            return --it;
        }

        template<typename Iter, typename Offset>
        constexpr void
        ce_adv(Iter & f, Offset n, std::random_access_iterator_tag)
        {
            f += n;
        }
        template<typename Iter, typename Offset, typename Tag>
        constexpr void ce_adv(Iter & f, Offset n, Tag)
        {
            if (0 < n) {
                for (Offset i = 0; i < n; ++i) {
                    ++f;
                }
            } else {
                for (Offset i = 0; i < -n; ++i) {
                    --f;
                }
            }
        }
    }




    template<typename BidiIter>
    struct reverse_iterator
        : iterator_interface<

              reverse_iterator<BidiIter>,





              typename std::iterator_traits<BidiIter>::iterator_category,

              typename std::iterator_traits<BidiIter>::value_type,
              typename std::iterator_traits<BidiIter>::reference,
              typename std::iterator_traits<BidiIter>::pointer,
              typename std::iterator_traits<BidiIter>::difference_type>
    {
        constexpr reverse_iterator() noexcept(noexcept(BidiIter())) : it_() {}
        constexpr reverse_iterator(BidiIter it) noexcept(
            noexcept(BidiIter(it))) :
            it_(it)
        {}
        template<
            typename BidiIter2,
            typename E = std::enable_if_t<
                std::is_convertible<BidiIter2, BidiIter>::value>>
        reverse_iterator(reverse_iterator<BidiIter2> const & it) : it_(it.it_)
        {}

        friend constexpr auto
        operator-(reverse_iterator lhs, reverse_iterator rhs) noexcept(
            noexcept(v1_dtl::ce_dist(
                lhs.it_,
                rhs.it_,
                typename std::iterator_traits<BidiIter>::iterator_category{})))
        {
            return -v1_dtl::ce_dist(
                rhs.it_,
                lhs.it_,
                typename std::iterator_traits<BidiIter>::iterator_category{});
        }

        constexpr typename std::iterator_traits<BidiIter>::reference
        operator*() const noexcept(
            noexcept(std::prev(v1_dtl::ce_prev(std::declval<BidiIter &>()))))
        {
            return *v1_dtl::ce_prev(it_);
        }

        constexpr reverse_iterator & operator+=(
            typename std::iterator_traits<BidiIter>::difference_type
                n) noexcept(noexcept(v1_dtl::
                                         ce_adv(
                                             std::declval<BidiIter &>(),
                                             -n,
                                             typename std::iterator_traits<
                                                 BidiIter>::
                                                 iterator_category{})))
        {
            v1_dtl::ce_adv(
                it_,
                -n,
                typename std::iterator_traits<BidiIter>::iterator_category{});
            return *this;
        }

        constexpr BidiIter base() const noexcept { return it_; }

    private:
        friend access;
        constexpr BidiIter & base_reference() noexcept { return it_; }
        constexpr BidiIter const & base_reference() const noexcept
        {
            return it_;
        }

        template<typename BidiIter2>
        friend struct reverse_iterator;

        BidiIter it_;
    };

    template<typename BidiIter>
    constexpr auto operator==(
        reverse_iterator<BidiIter> lhs,
        reverse_iterator<BidiIter>
            rhs) noexcept(noexcept(lhs.base() == rhs.base()))
        -> decltype(rhs.base() == lhs.base())
    {
        return lhs.base() == rhs.base();
    }

    template<typename BidiIter1, typename BidiIter2>
    constexpr auto operator==(
        reverse_iterator<BidiIter1> lhs,
        reverse_iterator<BidiIter2>
            rhs) noexcept(noexcept(lhs.base() == rhs.base()))
        -> decltype(rhs.base() == lhs.base())
    {
        return lhs.base() == rhs.base();
    }



    template<typename BidiIter>
    auto make_reverse_iterator(BidiIter it)
    {
        return reverse_iterator<BidiIter>(it);
    }

}}}
#line 12 "boost/parser/detail/text/detail/iterator.hpp"
#include <iterator>


namespace boost::parser::detail { namespace text { namespace detail {

    using reverse_char_iterator =
        parser::detail::stl_interfaces::reverse_iterator<char *>;
    using const_reverse_char_iterator =
        parser::detail::stl_interfaces::reverse_iterator<char const *>;

}}}
#line 13 "boost/parser/detail/text/detail/algorithm.hpp"
#include <numeric>
#include <type_traits>
#include <utility>




#include <cstdint>


namespace boost::parser::detail { namespace text { namespace detail {

    template<typename I>
    auto prev(I it)
    {



        return std::prev(it);

    }
    template<typename I>
    auto next(I it)
    {



        return std::next(it);

    }

    template<typename T>
    using remove_cv_ref_t =
        typename std::remove_cv<typename std::remove_reference<T>::type>::type;
#line 71 "boost/parser/detail/text/detail/algorithm.hpp"
    template<typename T>
    using has_base = decltype(std::declval<T>().base());
    template<typename T>
    using sentinel_comparable_to_iter_base =
        decltype(std::declval<T>().begin().base() == std::declval<T>().end());



    template<
        typename T,
        bool IsIt = is_detected_v<has_base, iterator_t<T>> &&
                    !is_detected_v<has_base, sentinel_t<T>> &&
                    is_detected_v<sentinel_comparable_to_iter_base, T>>
    constexpr bool is_cp_sentinel_gr_rng_v = false;
    template<typename T>
    constexpr bool is_cp_sentinel_gr_rng_v<T, true> = true;

    template<typename T>
    using gr_rng_cp_iter_t =
        decltype(detail::begin(std::declval<T &>()).base());
    template<typename T>
    using gr_rng_cp_sent_t = std::conditional_t<
        is_cp_sentinel_gr_rng_v<T>,
        sentinel_t<T>,
        gr_rng_cp_iter_t<T>>;



    template<typename T>
    using has_begin = decltype(*detail::begin(std::declval<T &>()));
    template<typename T>
    using has_end = decltype(*detail::end(std::declval<T &>()));

    template<typename T>
    using value_type_ = typename std::remove_cv<
        typename std::remove_reference<typename T::value_type>::type>::type;


    template<typename T>
    using nonpointer_iterator_category_ =
        typename T::iterator::iterator_category;

    template<typename T>
    using iterator_pointer_expr = std::is_pointer<typename T::iterator>;

    template<typename T>
    using iterator_category_ = typename std::conditional<
        detected_or_t<std::false_type, iterator_pointer_expr>::value,
        std::random_access_iterator_tag,
        detected_t<nonpointer_iterator_category_, T>>::type;



    template<typename T, typename U, int N>
    constexpr bool
        is_convertible_and_n_bytes_v = std::is_convertible<T, U>::value &&
                                       sizeof(T) == N;



    template<typename T>
    constexpr bool is_char_iter_v =
        std::is_same<char *, typename std::remove_cv<T>::type>::value ||
        std::is_same<char const *, typename std::remove_cv<T>::type>::value ||





        is_convertible_and_n_bytes_v<detected_t<value_type_, T>, char, 1>;


    template<typename T>
    using is_char_iter = std::integral_constant<bool, is_char_iter_v<T>>;

    template<typename T>
    constexpr bool is_char_range_v =
        (is_convertible_and_n_bytes_v<
             remove_cv_ref_t<detected_t<has_begin, T>>,
             char,
             1> &&
         is_convertible_and_n_bytes_v<
             remove_cv_ref_t<detected_t<has_end, T>>,
             char,
             1>);



    template<
        typename T,
        typename R1,
        typename Exclude,
        bool R1IsCharRange =
            is_char_range_v<R1> && !std::is_same<R1, Exclude>::value>
    struct rng_alg_ret
    {
    };

    template<typename T, typename R1, typename Exclude>
    struct rng_alg_ret<T, R1, Exclude, true>
    {
        using type = T;
    };

    template<typename T, typename R1, typename Exclude = void>
    using rng_alg_ret_t = typename rng_alg_ret<T, R1, Exclude>::type;

    template<
        typename T,
        typename R1,
        typename R2,
        bool R1IsCharRange = is_char_range_v<R1>,
        bool R2IsCharRange = is_char_range_v<R2>>
    struct rngs_alg_ret
    {
    };

    template<typename T, typename R1, typename R2>
    struct rngs_alg_ret<T, R1, R2, true, true>
    {
        using type = T;
    };

    template<typename T, typename R1, typename R2>
    using rngs_alg_ret_t = typename rngs_alg_ret<T, R1, R2>::type;



    template<typename T>
    using has_contig_begin = decltype(&*detail::begin(std::declval<T &>()));
    template<typename T>
    using has_contig_end = decltype(&*detail::end(std::declval<T &>()));

    template<typename T>
    constexpr bool is_contig_char_range_v =
        ((std::is_same<
              fixup_ptr_t<detected_t<has_contig_begin, T>>,
              char const *>::value &&
          std::is_same<
              fixup_ptr_t<detected_t<has_contig_end, T>>,
              char const *>::value)
#line 220 "boost/parser/detail/text/detail/algorithm.hpp"
             ) &&
        std::is_convertible<
            iterator_category_<T>,
            std::random_access_iterator_tag>::value;



    template<
        typename T,
        typename R1,
        bool R1IsContigCharRange = is_contig_char_range_v<R1>>
    struct contig_rng_alg_ret
    {
    };

    template<typename T, typename R1>
    struct contig_rng_alg_ret<T, R1, true>
    {
        using type = T;
    };

    template<typename T, typename R1>
    using contig_rng_alg_ret_t = typename contig_rng_alg_ret<T, R1>::type;

    template<
        typename T,
        typename R1,
        typename R2,
        bool R1IsContigCharRange = is_contig_char_range_v<R1>,
        bool R2IsContigCharRange = is_contig_char_range_v<R2>>
    struct contig_rngs_alg_ret
    {
    };

    template<typename T, typename R1, typename R2>
    struct contig_rngs_alg_ret<T, R1, R2, true, true>
    {
        using type = T;
    };

    template<typename T, typename R1, typename R2>
    using contig_rngs_alg_ret_t = typename contig_rngs_alg_ret<T, R1, R2>::type;



    template<typename T>
    constexpr bool is_char16_range_v =
        (is_convertible_and_n_bytes_v<
             remove_cv_ref_t<detected_t<has_begin, T>>,
             uint16_t,
             2> &&
         is_convertible_and_n_bytes_v<
             remove_cv_ref_t<detected_t<has_end, T>>,
             uint16_t,
             2>);



    template<
        typename T,
        typename R1,
        bool R1IsChar16Range = is_char16_range_v<R1>>
    struct rng16_alg_ret
    {
    };

    template<typename T, typename R1>
    struct rng16_alg_ret<T, R1, true>
    {
        using type = T;
    };

    template<typename T, typename R1>
    using rng16_alg_ret_t = typename rng16_alg_ret<T, R1>::type;



    template<typename T, typename R1, bool R1IsCharRange = is_char_iter_v<R1>>
    struct char_iter_ret
    {
    };

    template<typename T, typename R1>
    struct char_iter_ret<T, R1, true>
    {
        using type = T;
    };

    template<typename T, typename R1>
    using char_iter_ret_t = typename char_iter_ret<T, R1>::type;



    template<typename T>
    constexpr bool is_code_point_v = std::is_integral<T>::value &&
                                     sizeof(T) == 4;

    template<typename T>
    using has_deref_and_incr =
        std::pair<decltype(*std::declval<T>()), decltype(++std::declval<T>())>;

    template<typename T>
    constexpr bool is_cp_iter_v =
        ((std::is_pointer<T>::value &&
          is_code_point_v<typename std::remove_cv<
              typename std::remove_pointer<T>::type>::type>) ||
         (is_detected_v<has_deref_and_incr, T> &&
          is_code_point_v<
              typename std::remove_cv<detected_t<value_type_, T>>::type>));

    template<typename T, typename R1, bool R1IsCPRange = is_cp_iter_v<R1>>
    struct cp_iter_ret
    {
    };

    template<typename T, typename R1>
    struct cp_iter_ret<T, R1, true>
    {
        using type = T;
    };

    template<typename T, typename R1>
    using cp_iter_ret_t = typename cp_iter_ret<T, R1>::type;

    template<
        typename T,
        typename R1,
        typename R2,
        bool R1R2AreCPRanges = is_cp_iter_v<R1> && is_cp_iter_v<R2>>
    struct cp_iters_ret
    {
    };

    template<typename T, typename R1, typename R2>
    struct cp_iters_ret<T, R1, R2, true>
    {
        using type = T;
    };

    template<typename T, typename R1, typename R2>
    using cp_iters_ret_t = typename cp_iters_ret<T, R1, R2>::type;


    template<typename T>
    constexpr bool is_16_code_unit_v = std::is_integral<T>::value &&
                                       sizeof(T) == 2;

    template<typename T>
    constexpr bool is_16_iter_v =
        ((std::is_pointer<T>::value &&
          is_16_code_unit_v<typename std::remove_cv<
              typename std::remove_pointer<T>::type>::type>) ||
         (is_detected_v<has_deref_and_incr, T> &&
          is_16_code_unit_v<
              typename std::remove_cv<detected_t<value_type_, T>>::type>));

    template<typename T, typename R1, bool R1IsCPRange = is_16_iter_v<R1>>
    struct _16_iter_ret
    {
    };

    template<typename T, typename R1>
    struct _16_iter_ret<T, R1, true>
    {
        using type = T;
    };

    template<typename T, typename R1>
    using _16_iter_ret_t = typename _16_iter_ret<T, R1>::type;


    template<typename T>
    constexpr bool is_8_code_unit_v = std::is_integral<T>::value &&
                                      sizeof(T) == 1;

    template<typename T>
    constexpr bool is_8_iter_v =
        ((std::is_pointer<T>::value &&
          is_8_code_unit_v<typename std::remove_cv<
              typename std::remove_pointer<T>::type>::type>) ||
         (is_detected_v<has_deref_and_incr, T> &&
          is_8_code_unit_v<
              typename std::remove_cv<detected_t<value_type_, T>>::type>));

    template<typename T, typename R1, bool R1IsCPRange = is_8_iter_v<R1>>
    struct _8_iter_ret
    {
    };

    template<typename T, typename R1>
    struct _8_iter_ret<T, R1, true>
    {
        using type = T;
    };

    template<typename T, typename R1>
    using _8_iter_ret_t = typename _8_iter_ret<T, R1>::type;



    template<typename T, typename U>
    using comparable_ = decltype(std::declval<T>() == std::declval<U>());

    template<
        typename T,
        typename CPIter,
        typename Sentinel,
        bool FIsWordPropFunc = is_cp_iter_v<CPIter> &&
            is_detected_v<comparable_, CPIter, Sentinel>>
    struct cp_iter_sntl_ret
    {
    };

    template<typename T, typename CPIter, typename Sentinel>
    struct cp_iter_sntl_ret<T, CPIter, Sentinel, true>
    {
        using type = T;
    };

    template<typename T, typename CPIter, typename Sentinel>
    using cp_iter_sntl_ret_t =
        typename cp_iter_sntl_ret<T, CPIter, Sentinel>::type;

    template<typename T, typename R>
    using cp_rng_alg_ret_t =
        cp_iter_sntl_ret_t<T, iterator_t<R>, sentinel_t<R>>;

    template<
        typename T,
        typename CPIter1,
        typename Sentinel1,
        typename CPIter2,
        typename Sentinel2,
        bool AreCPRanges = is_cp_iter_v<CPIter1> && is_cp_iter_v<CPIter2> &&
            is_detected_v<comparable_, CPIter1, Sentinel1> &&
                is_detected_v<comparable_, CPIter2, Sentinel2>>
    struct cp_iters_sntls_ret
    {
    };

    template<
        typename T,
        typename CPIter1,
        typename Sentinel1,
        typename CPIter2,
        typename Sentinel2>
    struct cp_iters_sntls_ret<T, CPIter1, Sentinel1, CPIter2, Sentinel2, true>
    {
        using type = T;
    };

    template<
        typename T,
        typename CPIter1,
        typename Sentinel1,
        typename CPIter2,
        typename Sentinel2>
    using cp_iters_sntls_ret_t =
        typename cp_iters_sntls_ret<T, CPIter1, Sentinel1, CPIter2, Sentinel2>::
            type;

    template<typename T, typename R1, typename R2>
    using cp_rngs_alg_ret_t = cp_iters_sntls_ret_t<
        T,
        iterator_t<R1>,
        sentinel_t<R1>,
        iterator_t<R2>,
        sentinel_t<R2>>;


    template<int Size, typename T>
    constexpr bool is_cu_iter_v =
        Size == 1 ? is_char_iter_v<T> : is_16_iter_v<T>;

    template<
        int Size,
        typename T,
        typename U,
        bool UIsCUIter = is_cu_iter_v<Size, U>>
    struct cu_iter_ret
    {
    };

    template<int Size, typename T, typename U>
    struct cu_iter_ret<Size, T, U, true>
    {
        using type = T;
    };

    template<int Size, typename T, typename U>
    using cu_iter_ret_t = typename cu_iter_ret<Size, T, U>::type;

    template<int Size, typename T>
    constexpr bool is_cu_range_v =
        Size == 1 ? is_char_range_v<T> : is_char16_range_v<T>;

    template<
        int Size,
        typename T,
        typename U,
        typename Exclude,
        bool UIsCURange =
            is_cu_range_v<Size, U> && !std::is_same<U, Exclude>::value>
    struct cu_rng_alg_ret
    {
    };

    template<int Size, typename T, typename U, typename Exclude>
    struct cu_rng_alg_ret<Size, T, U, Exclude, true>
    {
        using type = T;
    };

    template<int Size, typename T, typename U, typename Exclude = void>
    using cu_rng_alg_ret_t = typename cu_rng_alg_ret<Size, T, U, Exclude>::type;


    template<typename T>
    using is_grapheme_iter_expr = std::integral_constant<
        bool,
        is_cp_iter_v<
            remove_cv_ref_t<decltype(std::declval<const T>().base())>>>;

    template<typename T>
    using is_grapheme_iter =
        detected_or_t<std::false_type, is_grapheme_iter_expr, T>;

    template<
        typename T,
        typename Iter,
        bool R1IsGraphemeIter = is_grapheme_iter<Iter>::value>
    struct graph_iter_alg_ret
    {};

    template<typename T, typename Iter>
    struct graph_iter_alg_ret<T, Iter, true>
    {
        using type = T;
    };

    template<typename T, typename Iter>
    using graph_iter_alg_ret_t = typename graph_iter_alg_ret<T, Iter>::type;

    template<
        typename T,
        typename Iter1,
        typename Iter2,
        bool Iter1Iter2AreGraphemeIter =
            is_grapheme_iter<Iter1>::value && is_grapheme_iter<Iter2>::value>
    struct graph_iters_alg_ret
    {};

    template<typename T, typename Iter1, typename Iter2>
    struct graph_iters_alg_ret<T, Iter1, Iter2, true>
    {
        using type = T;
    };

    template<typename T, typename Iter1, typename Iter2>
    using graph_iters_alg_ret_t =
        typename graph_iters_alg_ret<T, Iter1, Iter2>::type;


    template<typename Size, typename T>
    using is_grapheme_cu_iter_expr = std::integral_constant<
        bool,
        is_cp_iter_v<
            remove_cv_ref_t<decltype(std::declval<const T>().base())>> &&
            is_cu_iter_v<
                Size::value,
                remove_cv_ref_t<
                    decltype(std::declval<const T>().base().base())>>>;

    template<int Size, typename T>
    using is_grapheme_cu_iter = detected_or_t<
        std::false_type,
        is_grapheme_cu_iter_expr,
        std::integral_constant<int, Size>,
        T>;

    template<
        int Size,
        typename T,
        typename R1,
        bool R1IsGraphemeIter = is_grapheme_cu_iter<Size, R1>::value>
    struct graph_iter_cu_alg_ret
    {};

    template<int Size, typename T, typename R1>
    struct graph_iter_cu_alg_ret<Size, T, R1, true>
    {
        using type = T;
    };

    template<int Size, typename T, typename R1>
    using graph_iter_alg_cu_ret_t =
        typename graph_iter_cu_alg_ret<Size, T, R1>::type;

    template<typename T>
    using is_grapheme_range_expr = std::integral_constant<
        bool,
        is_cp_iter_v<remove_cv_ref_t<
            decltype(std::declval<const T>().begin().base())>> &&
            is_cp_iter_v<remove_cv_ref_t<
                decltype(std::declval<const T>().end().base())>> &&
            void_<
                decltype(std::declval<const T>().begin().base().base()),
                decltype(std::declval<const T>().end().base().base())>::value>;

    template<typename T>
    using is_grapheme_range =
        detected_or_t<std::false_type, is_grapheme_range_expr, T>;

    template<
        typename T,
        typename R1,
        bool R1IsGraphemeRange = is_grapheme_range<R1>::value>
    struct graph_rng_alg_ret
    {};

    template<typename T, typename R1>
    struct graph_rng_alg_ret<T, R1, true>
    {
        using type = T;
    };

    template<typename T, typename R1>
    using graph_rng_alg_ret_t = typename graph_rng_alg_ret<T, R1>::type;


    template<typename T>
    using is_cp_grapheme_range_expr = std::integral_constant<
        bool,
        is_cp_iter_v<remove_cv_ref_t<
            decltype(std::declval<const T>().begin().base())>> &&
            is_cp_iter_v<remove_cv_ref_t<
                decltype(std::declval<const T>().end().base())>>>;

    template<typename T>
    using is_cp_grapheme_range =
        detected_or_t<std::false_type, is_cp_grapheme_range_expr, T>;

    template<
        typename T,
        typename R1,
        bool R1IsGraphemeRange = is_grapheme_range<R1>::value>
    struct cp_graph_rng_alg_ret
    {};

    template<typename T, typename R1>
    struct cp_graph_rng_alg_ret<T, R1, true>
    {
        using type = T;
    };

    template<typename T, typename R1>
    using cp_graph_rng_alg_ret_t = typename cp_graph_rng_alg_ret<T, R1>::type;

    template<
        typename T,
        typename R1,
        typename R2,
        bool R1R2AreGraphemeRanges =
            is_cp_grapheme_range<R1>::value && is_cp_grapheme_range<R2>::value>
    struct graph_rngs_alg_ret
    {};

    template<typename T, typename R1, typename R2>
    struct graph_rngs_alg_ret<T, R1, R2, true>
    {
        using type = T;
    };

    template<typename T, typename R1, typename R2>
    using graph_rngs_alg_ret_t = typename graph_rngs_alg_ret<T, R1, R2>::type;

    template<
        typename T,
        typename R1,
        typename R2,
        bool R1IsGraphemeRangeButNotR2 =
            is_cp_grapheme_range<R1>::value && !is_cp_grapheme_range<R2>::value>
    struct lazy_graph_rng_and_non_alg_ret
    {};

    template<typename T, typename R1, typename R2>
    struct lazy_graph_rng_and_non_alg_ret<T, R1, R2, true>
    {
        using type = typename T::type;
    };

    template<typename T, typename R1, typename R2>
    using lazy_graph_rng_and_non_alg_ret_t =
        typename lazy_graph_rng_and_non_alg_ret<T, R1, R2>::type;

    template<
        typename T,
        typename R1,
        typename R2,
        bool R1IsGraphemeRangeButNotR2 =
            is_cp_grapheme_range<R1>::value && !is_cp_grapheme_range<R2>::value>
    struct graph_rng_and_non_alg_ret
    {};

    template<typename T, typename R1, typename R2>
    struct graph_rng_and_non_alg_ret<T, R1, R2, true>
    {
        using type = T;
    };

    template<typename T, typename R1, typename R2>
    using graph_rng_and_non_alg_ret_t =
        typename graph_rng_and_non_alg_ret<T, R1, R2>::type;

    template<
        typename T,
        typename R1,
        typename R2,
        bool R1R2AreNotGraphemeRanges = !is_cp_grapheme_range<R1>::value &&
                                        !is_cp_grapheme_range<R2>::value>
    struct non_graph_rngs_alg_ret
    {};

    template<typename T, typename R1, typename R2>
    struct non_graph_rngs_alg_ret<T, R1, R2, true>
    {
        using type = T;
    };

    template<typename T, typename R1, typename R2>
    using non_graph_rngs_alg_ret_t =
        typename non_graph_rngs_alg_ret<T, R1, R2>::type;

    template<
        typename T,
        typename R1,
        typename R2,
        bool R1R2AreNotGraphemeRanges = !is_cp_grapheme_range<R1>::value &&
                                        !is_cp_grapheme_range<R2>::value>
    struct lazy_non_graph_rngs_alg_ret
    {};

    template<typename T, typename R1, typename R2>
    struct lazy_non_graph_rngs_alg_ret<T, R1, R2, true>
    {
        using type = typename T::type;
    };

    template<typename T, typename R1, typename R2>
    using lazy_non_graph_rngs_alg_ret_t =
        typename lazy_non_graph_rngs_alg_ret<T, R1, R2>::type;


    template<typename T>
    using is_contig_grapheme_range_expr = std::integral_constant<
        bool,
        (std::is_same<
             decltype(std::declval<const T>().begin().base().base()),
             char const *>::value ||
         std::is_same<
             decltype(std::declval<const T>().begin().base().base()),
             char *>::value) &&
            (std::is_same<
                 decltype(std::declval<const T>().end().base().base()),
                 char const *>::value ||
             std::is_same<
                 decltype(std::declval<const T>().end().base().base()),
                 char *>::value)>;

    template<typename T>
    using is_contig_grapheme_range =
        detected_or_t<std::false_type, is_contig_grapheme_range_expr, T>;

    template<
        typename T,
        typename R1,
        bool R1IsContigGraphemeRange = is_contig_grapheme_range<R1>::value>
    struct contig_graph_rng_alg_ret
    {
    };

    template<typename T, typename R1>
    struct contig_graph_rng_alg_ret<T, R1, true>
    {
        using type = T;
    };

    template<typename T, typename R1>
    using contig_graph_rng_alg_ret_t =
        typename contig_graph_rng_alg_ret<T, R1>::type;



    inline std::size_t hash_combine_(std::size_t seed, std::size_t value)
    {
        return seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    template<int N>
    struct hash_4_more_chars
    {
        template<typename Iter>
        static std::size_t call(std::size_t curr, Iter it)
        {
            return curr;
        }
    };

    template<>
    struct hash_4_more_chars<8>
    {
        template<typename Iter>
        static std::size_t call(std::size_t curr, Iter it)
        {
            curr <<= 32;
            curr += (*(it + 4) << 24) + (*(it + 5) << 16) + (*(it + 2) << 6) +
                    (*(it + 7) << 0);
            return curr;
        }
    };

    template<typename CharRange>
    std::size_t hash_char_range(CharRange const & r)
    {
        auto first = r.begin();
        auto last = r.end();
        auto const size = last - first;
        auto const remainder = size % sizeof(std::size_t);
        last -= remainder;

        std::size_t retval = size;
        for (; first != last; first += sizeof(std::size_t)) {
            std::size_t curr = (*(first + 0) << 24) + (*(first + 1) << 16) +
                               (*(first + 2) << 8) + (*(first + 3) << 0);
            curr = hash_4_more_chars<sizeof(std::size_t)>::call(curr, first);
            retval = detail::hash_combine_(retval, curr);
        }

        first = last;
        last += remainder;
        for (; first != last; ++first) {
            retval = detail::hash_combine_(retval, *first);
        }

        return retval;
    }

    template<typename GraphemeRange>
    std::size_t hash_grapheme_range(GraphemeRange const & r)
    {
        std::size_t cps = 0;
        std::size_t retval = std::accumulate(
            r.begin().base(),
            r.end().base(),
            std::size_t(0),
            [&cps](std::size_t seed, std::size_t value) {
                ++cps;
                return detail::hash_combine_(seed, value);
            });
        return detail::hash_combine_(retval, cps);
    }

    template<typename Iter>
    using char_value_expr = std::integral_constant<
        bool,
        std::is_integral<
            typename std::iterator_traits<Iter>::value_type>::value &&
            sizeof(typename std::iterator_traits<Iter>::value_type) == 1>;

    template<typename Iter>
    constexpr bool is_char_ptr_v = std::is_pointer<Iter>::value &&
        detected_or_t<std::false_type, char_value_expr, Iter>::value;

    template<typename Iter>
    using _16_value_expr = std::integral_constant<
        bool,
        std::is_integral<
            typename std::iterator_traits<Iter>::value_type>::value &&
            sizeof(typename std::iterator_traits<Iter>::value_type) == 2>;

    template<typename Iter>
    constexpr bool is_16_ptr_v = std::is_pointer<Iter>::value &&
        detected_or_t<std::false_type, _16_value_expr, Iter>::value;

    template<typename Iter>
    using cp_value_expr = std::integral_constant<
        bool,
        std::is_integral<
            typename std::iterator_traits<Iter>::value_type>::value &&
            sizeof(typename std::iterator_traits<Iter>::value_type) == 4>;

    template<typename Iter>
    constexpr bool is_cp_ptr_v = std::is_pointer<Iter>::value &&
        detected_or_t<std::false_type, cp_value_expr, Iter>::value;

}}}
#line 7 "boost/parser/detail/stl_interfaces/view_interface.hpp"
#define BOOST_PARSER_DETAIL_STL_INTERFACES_VIEW_INTERFACE_HPP




namespace boost::parser::detail { namespace stl_interfaces { inline namespace v1 {
#line 24 "boost/parser/detail/stl_interfaces/view_interface.hpp"
    template<
        typename Derived,
        element_layout Contiguity = element_layout::discontiguous

        ,
        typename E = std::enable_if_t<
            std::is_class<Derived>::value &&
            std::is_same<Derived, std::remove_cv_t<Derived>>::value>

        >
    struct view_interface;

    namespace v1_dtl {
        template<typename D, element_layout Contiguity>
        void derived_view(view_interface<D, Contiguity> const &);
    }

    template<
        typename Derived,
        element_layout Contiguity

        ,
        typename E

        >
    struct view_interface
    {

    private:
        constexpr Derived & derived() noexcept
        {
            return static_cast<Derived &>(*this);
        }
        constexpr const Derived & derived() const noexcept
        {
            return static_cast<Derived const &>(*this);
        }


    public:
        template<typename D = Derived>
        constexpr auto empty() noexcept(
            noexcept(std::declval<D &>().begin() == std::declval<D &>().end()))
            -> decltype(
                std::declval<D &>().begin() == std::declval<D &>().end())
        {
            return derived().begin() == derived().end();
        }
        template<typename D = Derived>
        constexpr auto empty() const noexcept(noexcept(
            std::declval<D const &>().begin() ==
            std::declval<D const &>().end()))
            -> decltype(
                std::declval<D const &>().begin() ==
                std::declval<D const &>().end())
        {
            return derived().begin() == derived().end();
        }

        template<
            typename D = Derived,
            typename R = decltype(std::declval<D &>().empty())>
        constexpr explicit
        operator bool() noexcept(noexcept(std::declval<D &>().empty()))
        {
            return !derived().empty();
        }
        template<
            typename D = Derived,
            typename R = decltype(std::declval<D const &>().empty())>
        constexpr explicit operator bool() const
            noexcept(noexcept(std::declval<D const &>().empty()))
        {
            return !derived().empty();
        }

        template<
            typename D = Derived,
            element_layout C = Contiguity,
            typename Enable = std::enable_if_t<C == element_layout::contiguous>>
        constexpr auto data() noexcept(noexcept(std::declval<D &>().begin()))
            -> decltype(std::addressof(*std::declval<D &>().begin()))
        {
            return std::addressof(*derived().begin());
        }
        template<
            typename D = Derived,
            element_layout C = Contiguity,
            typename Enable = std::enable_if_t<C == element_layout::contiguous>>
        constexpr auto data() const
            noexcept(noexcept(std::declval<D const &>().begin()))
                -> decltype(std::addressof(*std::declval<D const &>().begin()))
        {
            return std::addressof(*derived().begin());
        }

        template<typename D = Derived>
        constexpr auto size() noexcept(
            noexcept(std::declval<D &>().end() - std::declval<D &>().begin()))
            -> decltype(std::declval<D &>().end() - std::declval<D &>().begin())
        {
            return derived().end() - derived().begin();
        }
        template<typename D = Derived>
        constexpr auto size() const noexcept(noexcept(
            std::declval<D const &>().end() -
            std::declval<D const &>().begin()))
            -> decltype(
                std::declval<D const &>().end() -
                std::declval<D const &>().begin())
        {
            return derived().end() - derived().begin();
        }

        template<typename D = Derived>
        constexpr auto front() noexcept(noexcept(*std::declval<D &>().begin()))
            -> decltype(*std::declval<D &>().begin())
        {
            return *derived().begin();
        }
        template<typename D = Derived>
        constexpr auto front() const
            noexcept(noexcept(*std::declval<D const &>().begin()))
                -> decltype(*std::declval<D const &>().begin())
        {
            return *derived().begin();
        }

        template<
            typename D = Derived,
            typename Enable = std::enable_if_t<
                v1_dtl::decrementable_sentinel<D>::value &&
                v1_dtl::common_range<D>::value>>
        constexpr auto
        back() noexcept(noexcept(*std::prev(std::declval<D &>().end())))
            -> decltype(*std::prev(std::declval<D &>().end()))
        {
            return *std::prev(derived().end());
        }
        template<
            typename D = Derived,
            typename Enable = std::enable_if_t<
                v1_dtl::decrementable_sentinel<D>::value &&
                v1_dtl::common_range<D>::value>>
        constexpr auto back() const
            noexcept(noexcept(*std::prev(std::declval<D const &>().end())))
                -> decltype(*std::prev(std::declval<D const &>().end()))
        {
            return *std::prev(derived().end());
        }

        template<typename D = Derived>
        constexpr auto operator[](v1_dtl::range_difference_t<D> n) noexcept(
            noexcept(std::declval<D &>().begin()[n]))
            -> decltype(std::declval<D &>().begin()[n])
        {
            return derived().begin()[n];
        }
        template<typename D = Derived>
        constexpr auto operator[](v1_dtl::range_difference_t<D> n) const
            noexcept(noexcept(std::declval<D const &>().begin()[n]))
                -> decltype(std::declval<D const &>().begin()[n])
        {
            return derived().begin()[n];
        }
    };



    template<typename ViewInterface>
    constexpr auto operator!=(ViewInterface lhs, ViewInterface rhs) noexcept(
        noexcept(lhs == rhs))
        -> decltype(v1_dtl::derived_view(lhs), !(lhs == rhs))
    {
        return !(lhs == rhs);
    }

}}}
#line 15 "boost/parser/subrange.hpp"
namespace boost::parser {






    template<typename I, typename S = I>

    struct subrange : detail::stl_interfaces::view_interface<subrange<I, S>>
    {
        constexpr subrange() = default;
        constexpr subrange(I first, S last) : first_(first), last_(last) {}
        template<typename R>
        constexpr explicit subrange(R const & r) :
            first_(detail::text::detail::begin(r)),
            last_(detail::text::detail::end(r))
        {}

        constexpr I begin() const { return first_; }
        constexpr S end() const { return last_; }

        [[nodiscard]] constexpr subrange next(std::ptrdiff_t n = 1) const
        {
            return subrange{detail::text::detail::next(first_), last_};
        }
        [[nodiscard]] constexpr subrange prev(std::ptrdiff_t n = 1) const
        {
            return subrange{detail::text::detail::prev(first_), last_};
        }

        constexpr subrange & advance(std::ptrdiff_t n)
        {
            std::advance(first_, n);
            return *this;
        }

        template<
            typename I2,
            typename S2,
            typename Enable = std::enable_if_t<
                std::is_convertible<I, I2>::value &&
                std::is_convertible<S, S2>::value>>
        constexpr operator subrange<I2, S2>() const
        {
            return {first_, last_};
        }

    private:
        I first_;
        [[no_unique_address]] S last_;
    };
#line 90 "boost/parser/subrange.hpp"
    template<typename I, typename S = I>

    constexpr subrange<I, S> make_subrange(I first, S last) noexcept
    {
        return subrange<I, S>(first, last);
    }

}
#line 81 "boost/parser/config.hpp"
#    define BOOST_PARSER_SUBRANGE boost::parser::subrange





#    define BOOST_PARSER_USE_STD_TUPLE 0



#define BOOST_PARSER_MAX_AGGREGATE_SIZE 25




#define BOOST_PARSER_CONSTEXPR constexpr
#line 2 "boost/parser/error_handling_fwd.hpp"
#define BOOST_PARSER_ERROR_HANDLING_FWD_HPP
#line 7 "boost/parser/detail/text/transcode_view.hpp"
#define BOOST_PARSER_DETAIL_TEXT_TRANSCODE_VIEW_HPP
#line 8 "boost/parser/detail/text/transcode_algorithm.hpp"
#define BOOST_PARSER_DETAIL_TEXT_TRANSCODE_ALGORITHM_HPP
#line 7 "boost/parser/detail/text/in_out_result.hpp"
#define BOOST_PARSER_DETAIL_TEXT_IN_OUT_RESULT_HPP




namespace boost::parser::detail { namespace text {



    template<typename I, typename O>
    struct in_out_result
    {
        [[no_unique_address]] I in;
        [[no_unique_address]] O out;
    };

}}
#line 7 "boost/parser/detail/text/transcode_iterator.hpp"
#define BOOST_PARSER_DETAIL_TEXT_TRANSCODE_ITERATOR_HPP
#line 7 "boost/parser/detail/text/transcode_iterator_fwd.hpp"
#define BOOST_PARSER_DETAIL_TEXT_TRANSCODE_ITERATOR_FWD_HPP




namespace boost::parser::detail { namespace text {

    struct use_replacement_character;

    namespace detail {
        template<
            typename RepackedIterator,
            typename I,
            typename S,
            typename Then>
        struct bidi_repacker;
    }
}}

namespace boost::parser::detail { namespace text {

    namespace detail {
        template<format Format>
        constexpr auto format_to_type();

        template<format Format>
        using format_to_type_t = decltype(format_to_type<Format>());
    }
#line 45 "boost/parser/detail/text/transcode_iterator_fwd.hpp"
    template<
        format FromFormat,
        format ToFormat,
        typename I,
        typename S = I,
        typename ErrorHandler = use_replacement_character>

    class utf_iterator;
#line 99 "boost/parser/detail/text/transcode_iterator_fwd.hpp"
}}
#line 17 "boost/parser/detail/text/transcode_iterator.hpp"
#include <array>
#include <iterator>
#include <type_traits>
#include <stdexcept>
#include <string_view>

namespace boost::parser::detail { namespace text {

    namespace {
        constexpr char16_t high_surrogate_base = 0xd7c0;
        constexpr char16_t low_surrogate_base = 0xdc00;
        constexpr char32_t high_surrogate_min = 0xd800;
        constexpr char32_t high_surrogate_max = 0xdbff;
        constexpr char32_t low_surrogate_min = 0xdc00;
        constexpr char32_t low_surrogate_max = 0xdfff;
        constexpr char32_t replacement_character = 0xfffd;
    }

    namespace detail {
        constexpr bool in(unsigned char lo, unsigned char c, unsigned char hi)
        {
            return lo <= c && c <= hi;
        }

        struct throw_on_encoding_error
        {};

        template<typename OutIter>
        inline constexpr OutIter read_into_buf(char32_t cp, OutIter buf)
        {
            if (cp < 0x80) {
                *buf = static_cast<char>(cp);
                ++buf;
            } else if (cp < 0x800) {
                *buf = static_cast<char>(0xC0 + (cp >> 6));
                ++buf;
                *buf = static_cast<char>(0x80 + (cp & 0x3f));
                ++buf;
            } else if (cp < 0x10000) {
                *buf = static_cast<char>(0xe0 + (cp >> 12));
                ++buf;
                *buf = static_cast<char>(0x80 + ((cp >> 6) & 0x3f));
                ++buf;
                *buf = static_cast<char>(0x80 + (cp & 0x3f));
                ++buf;
            } else {
                *buf = static_cast<char>(0xf0 + (cp >> 18));
                ++buf;
                *buf = static_cast<char>(0x80 + ((cp >> 12) & 0x3f));
                ++buf;
                *buf = static_cast<char>(0x80 + ((cp >> 6) & 0x3f));
                ++buf;
                *buf = static_cast<char>(0x80 + (cp & 0x3f));
                ++buf;
            }
            return buf;
        }

        template<typename OutIter>
        constexpr OutIter write_cp_utf8(char32_t cp, OutIter out)
        {
            return detail::read_into_buf(cp, out);
        }

        template<typename OutIter>
        constexpr OutIter write_cp_utf16(char32_t cp, OutIter out)
        {
            if (cp < 0x10000) {
                *out = static_cast<char16_t>(cp);
                ++out;
            } else {
                *out = static_cast<char16_t>(cp >> 10) + high_surrogate_base;
                ++out;
                *out = static_cast<char16_t>(cp & 0x3ff) + low_surrogate_base;
                ++out;
            }
            return out;
        }

        inline constexpr char32_t surrogates_to_cp(char16_t hi, char16_t lo)
        {
            return char32_t((hi - high_surrogate_base) << 10) +
                   (lo - low_surrogate_base);
        }

        template<typename T, typename U>
        using enable_utf8_cp = std::enable_if<is_char_iter_v<T>, U>;
        template<typename T, typename U = T>
        using enable_utf8_cp_t = typename enable_utf8_cp<T, U>::type;

        template<typename T, typename U>
        using enable_utf16_cp = std::enable_if<is_16_iter_v<T>, U>;
        template<typename T, typename U = T>
        using enable_utf16_cp_t = typename enable_utf16_cp<T, U>::type;

        template<typename I>
        auto bidirectional_at_most()
        {
#line 124 "boost/parser/detail/text/transcode_iterator.hpp"
            using category =
                typename std::iterator_traits<I>::iterator_category;
            if constexpr (std::is_base_of_v<
                              std::bidirectional_iterator_tag,
                              category>) {
                return std::bidirectional_iterator_tag{};
            } else {
                return category{};
            }

        }

        template<typename I>
        using bidirectional_at_most_t = decltype(bidirectional_at_most<I>());
    }


    inline constexpr bool surrogate(char32_t c)
    {
        return high_surrogate_min <= c && c <= low_surrogate_max;
    }


    inline constexpr bool high_surrogate(char32_t c)
    {
        return high_surrogate_min <= c && c <= high_surrogate_max;
    }


    inline constexpr bool low_surrogate(char32_t c)
    {
        return low_surrogate_min <= c && c <= low_surrogate_max;
    }




    inline constexpr bool reserved_noncharacter(char32_t c)
    {
        bool const byte01_reserved = (c & 0xffff) >= 0xfffe;
        bool const byte2_at_most_0x10 = ((c & 0xff0000u) >> 16) <= 0x10;
        return (byte01_reserved && byte2_at_most_0x10) ||
               (0xfdd0 <= c && c <= 0xfdef);
    }




    inline constexpr bool scalar_value(char32_t c)
    {
        return c <= 0x10ffff && !surrogate(c);
    }





    inline constexpr bool unreserved_scalar_value(char32_t c)
    {
        return scalar_value(c) && !reserved_noncharacter(c);
    }



    constexpr bool lead_code_unit(char8_type c)
    {
        return uint8_t((unsigned char)c - 0xc2) <= 0x32;
    }


    constexpr bool continuation(char8_type c) { return (int8_t)c < -0x40; }





    inline constexpr int utf8_code_units(char8_type first_unit_)
    {
        auto first_unit = (unsigned int)first_unit_;
        return first_unit <= 0x7f ? 1
               : boost::parser::detail::text::lead_code_unit(first_unit)
                   ? int(0xe0 <= first_unit) + int(0xf0 <= first_unit) + 2
                   : -1;
    }





    inline constexpr int utf16_code_units(char16_t first_unit)
    {
        if (boost::parser::detail::text::low_surrogate(first_unit))
            return -1;
        if (boost::parser::detail::text::high_surrogate(first_unit))
            return 2;
        return 1;
    }

    namespace detail {

        template<typename Iter>
        struct optional_iter
        {
            constexpr optional_iter() : it_(), valid_(false) {}
            constexpr optional_iter(Iter it) : it_(it), valid_(true) {}

            constexpr operator bool() const { return valid_; }
            constexpr Iter operator*() const
            {
                BOOST_ASSERT(valid_);
                return it_;
            }
            Iter & operator*()
            {
                BOOST_ASSERT(valid_);
                return it_;
            }

            friend constexpr bool
            operator==(optional_iter lhs, optional_iter rhs)
            {
                return lhs.valid_ == rhs.valid_ &&
                       (!lhs.valid_ || lhs.it_ == rhs.it_);
            }
            friend constexpr bool
            operator!=(optional_iter lhs, optional_iter rhs)
            {
                return !(lhs == rhs);
            }

        private:
            Iter it_;
            bool valid_;
        };


        template<typename Iter>
        constexpr optional_iter<Iter> end_of_invalid_utf8(Iter it)
        {
            BOOST_ASSERT(!boost::parser::detail::text::continuation(*it));

            if (detail::in(0, *it, 0x7f))
                return optional_iter<Iter>{};

            if (detail::in(0xc2, *it, 0xdf)) {
                auto next = it;
                if (!boost::parser::detail::text::continuation(*++next))
                    return next;
                return optional_iter<Iter>{};
            }

            if (detail::in(0xe0, *it, 0xe0)) {
                auto next = it;
                if (!detail::in(0xa0, *++next, 0xbf))
                    return next;
                if (!boost::parser::detail::text::continuation(*++next))
                    return next;
                return optional_iter<Iter>{};
            }
            if (detail::in(0xe1, *it, 0xec)) {
                auto next = it;
                if (!boost::parser::detail::text::continuation(*++next))
                    return next;
                if (!boost::parser::detail::text::continuation(*++next))
                    return next;
                return optional_iter<Iter>{};
            }
            if (detail::in(0xed, *it, 0xed)) {
                auto next = it;
                if (!detail::in(0x80, *++next, 0x9f))
                    return next;
                if (!boost::parser::detail::text::continuation(*++next))
                    return next;
                return optional_iter<Iter>{};
            }
            if (detail::in(0xee, *it, 0xef)) {
                auto next = it;
                if (!boost::parser::detail::text::continuation(*++next))
                    return next;
                if (!boost::parser::detail::text::continuation(*++next))
                    return next;
                return optional_iter<Iter>{};
            }

            if (detail::in(0xf0, *it, 0xf0)) {
                auto next = it;
                if (!detail::in(0x90, *++next, 0xbf))
                    return next;
                if (!boost::parser::detail::text::continuation(*++next))
                    return next;
                if (!boost::parser::detail::text::continuation(*++next))
                    return next;
                return optional_iter<Iter>{};
            }
            if (detail::in(0xf1, *it, 0xf3)) {
                auto next = it;
                if (!boost::parser::detail::text::continuation(*++next))
                    return next;
                if (!boost::parser::detail::text::continuation(*++next))
                    return next;
                if (!boost::parser::detail::text::continuation(*++next))
                    return next;
                return optional_iter<Iter>{};
            }
            if (detail::in(0xf4, *it, 0xf4)) {
                auto next = it;
                if (!detail::in(0x80, *++next, 0x8f))
                    return next;
                if (!boost::parser::detail::text::continuation(*++next))
                    return next;
                if (!boost::parser::detail::text::continuation(*++next))
                    return next;
                return optional_iter<Iter>{};
            }

            return it;
        }

        template<typename Iter>
        constexpr Iter decrement(Iter it)
        {
            Iter retval = it;

            int backup = 0;
            while (backup < 4 && boost::parser::detail::text::continuation(*--retval)) {
                ++backup;
            }
            backup = it - retval;

            if (boost::parser::detail::text::continuation(*retval))
                return it - 1;

            optional_iter<Iter> first_invalid = end_of_invalid_utf8(retval);
            if (first_invalid == retval)
                ++*first_invalid;
            while (first_invalid && (*first_invalid - retval) < backup) {
                backup -= *first_invalid - retval;
                retval = *first_invalid;
                first_invalid = end_of_invalid_utf8(retval);
                if (first_invalid == retval)
                    ++*first_invalid;
            }

            if (1 < backup) {
                int const cp_bytes = boost::parser::detail::text::utf8_code_units(*retval);
                if (cp_bytes < backup)
                    retval = it - 1;
            }

            return retval;
        }

        template<typename Iter>
        constexpr Iter decrement(Iter first, Iter it)
        {
            Iter retval = it;

            int backup = 0;
            while (backup < 4 && retval != first &&
                   boost::parser::detail::text::continuation(*--retval)) {
                ++backup;
            }
            backup = std::distance(retval, it);

            if (boost::parser::detail::text::continuation(*retval)) {
                if (it != first)
                    --it;
                return it;
            }

            optional_iter<Iter> first_invalid = end_of_invalid_utf8(retval);
            if (first_invalid == retval)
                ++*first_invalid;
            while (first_invalid &&
                   std::distance(retval, *first_invalid) < backup) {
                backup -= std::distance(retval, *first_invalid);
                retval = *first_invalid;
                first_invalid = end_of_invalid_utf8(retval);
                if (first_invalid == retval)
                    ++*first_invalid;
            }

            if (1 < backup) {
                int const cp_bytes = boost::parser::detail::text::utf8_code_units(*retval);
                if (cp_bytes < backup) {
                    if (it != first)
                        --it;
                    retval = it;
                }
            }

            return retval;
        }

        enum char_class : uint8_t {
            ill = 0,
            asc = 1,
            cr1 = 2,
            cr2 = 3,
            cr3 = 4,
            l2a = 5,
            l3a = 6,
            l3b = 7,
            l3c = 8,
            l4a = 9,
            l4b = 10,
            l4c = 11,
        };

        enum table_state : uint8_t {
            bgn = 0,
            e_d = bgn,
            err = 12,
            cs1 = 24,
            cs2 = 36,
            cs3 = 48,
            p3a = 60,
            p3b = 72,
            p4a = 84,
            p4b = 96,
            invalid_table_state = 200
        };

        struct first_cu
        {
            unsigned char initial_octet;
            table_state next;
        };

        namespace {
            constexpr first_cu first_cus[256] = {
                {0x00, bgn}, {0x01, bgn}, {0x02, bgn}, {0x03, bgn}, {0x04, bgn},
                {0x05, bgn}, {0x06, bgn}, {0x07, bgn}, {0x08, bgn}, {0x09, bgn},
                {0x0a, bgn}, {0x0b, bgn}, {0x0c, bgn}, {0x0d, bgn}, {0x0e, bgn},
                {0x0f, bgn}, {0x10, bgn}, {0x11, bgn}, {0x12, bgn}, {0x13, bgn},
                {0x14, bgn}, {0x15, bgn}, {0x16, bgn}, {0x17, bgn}, {0x18, bgn},
                {0x19, bgn}, {0x1a, bgn}, {0x1b, bgn}, {0x1c, bgn}, {0x1d, bgn},
                {0x1e, bgn}, {0x1f, bgn}, {0x20, bgn}, {0x21, bgn}, {0x22, bgn},
                {0x23, bgn}, {0x24, bgn}, {0x25, bgn}, {0x26, bgn}, {0x27, bgn},
                {0x28, bgn}, {0x29, bgn}, {0x2a, bgn}, {0x2b, bgn}, {0x2c, bgn},
                {0x2d, bgn}, {0x2e, bgn}, {0x2f, bgn}, {0x30, bgn}, {0x31, bgn},
                {0x32, bgn}, {0x33, bgn}, {0x34, bgn}, {0x35, bgn}, {0x36, bgn},
                {0x37, bgn}, {0x38, bgn}, {0x39, bgn}, {0x3a, bgn}, {0x3b, bgn},
                {0x3c, bgn}, {0x3d, bgn}, {0x3e, bgn}, {0x3f, bgn}, {0x40, bgn},
                {0x41, bgn}, {0x42, bgn}, {0x43, bgn}, {0x44, bgn}, {0x45, bgn},
                {0x46, bgn}, {0x47, bgn}, {0x48, bgn}, {0x49, bgn}, {0x4a, bgn},
                {0x4b, bgn}, {0x4c, bgn}, {0x4d, bgn}, {0x4e, bgn}, {0x4f, bgn},
                {0x50, bgn}, {0x51, bgn}, {0x52, bgn}, {0x53, bgn}, {0x54, bgn},
                {0x55, bgn}, {0x56, bgn}, {0x57, bgn}, {0x58, bgn}, {0x59, bgn},
                {0x5a, bgn}, {0x5b, bgn}, {0x5c, bgn}, {0x5d, bgn}, {0x5e, bgn},
                {0x5f, bgn}, {0x60, bgn}, {0x61, bgn}, {0x62, bgn}, {0x63, bgn},
                {0x64, bgn}, {0x65, bgn}, {0x66, bgn}, {0x67, bgn}, {0x68, bgn},
                {0x69, bgn}, {0x6a, bgn}, {0x6b, bgn}, {0x6c, bgn}, {0x6d, bgn},
                {0x6e, bgn}, {0x6f, bgn}, {0x70, bgn}, {0x71, bgn}, {0x72, bgn},
                {0x73, bgn}, {0x74, bgn}, {0x75, bgn}, {0x76, bgn}, {0x77, bgn},
                {0x78, bgn}, {0x79, bgn}, {0x7a, bgn}, {0x7b, bgn}, {0x7c, bgn},
                {0x7d, bgn}, {0x7e, bgn}, {0x7f, bgn}, {0x00, err}, {0x01, err},
                {0x02, err}, {0x03, err}, {0x04, err}, {0x05, err}, {0x06, err},
                {0x07, err}, {0x08, err}, {0x09, err}, {0x0a, err}, {0x0b, err},
                {0x0c, err}, {0x0d, err}, {0x0e, err}, {0x0f, err}, {0x10, err},
                {0x11, err}, {0x12, err}, {0x13, err}, {0x14, err}, {0x15, err},
                {0x16, err}, {0x17, err}, {0x18, err}, {0x19, err}, {0x1a, err},
                {0x1b, err}, {0x1c, err}, {0x1d, err}, {0x1e, err}, {0x1f, err},
                {0x20, err}, {0x21, err}, {0x22, err}, {0x23, err}, {0x24, err},
                {0x25, err}, {0x26, err}, {0x27, err}, {0x28, err}, {0x29, err},
                {0x2a, err}, {0x2b, err}, {0x2c, err}, {0x2d, err}, {0x2e, err},
                {0x2f, err}, {0x30, err}, {0x31, err}, {0x32, err}, {0x33, err},
                {0x34, err}, {0x35, err}, {0x36, err}, {0x37, err}, {0x38, err},
                {0x39, err}, {0x3a, err}, {0x3b, err}, {0x3c, err}, {0x3d, err},
                {0x3e, err}, {0x3f, err}, {0xc0, err}, {0xc1, err}, {0x02, cs1},
                {0x03, cs1}, {0x04, cs1}, {0x05, cs1}, {0x06, cs1}, {0x07, cs1},
                {0x08, cs1}, {0x09, cs1}, {0x0a, cs1}, {0x0b, cs1}, {0x0c, cs1},
                {0x0d, cs1}, {0x0e, cs1}, {0x0f, cs1}, {0x10, cs1}, {0x11, cs1},
                {0x12, cs1}, {0x13, cs1}, {0x14, cs1}, {0x15, cs1}, {0x16, cs1},
                {0x17, cs1}, {0x18, cs1}, {0x19, cs1}, {0x1a, cs1}, {0x1b, cs1},
                {0x1c, cs1}, {0x1d, cs1}, {0x1e, cs1}, {0x1f, cs1}, {0x00, p3a},
                {0x01, cs2}, {0x02, cs2}, {0x03, cs2}, {0x04, cs2}, {0x05, cs2},
                {0x06, cs2}, {0x07, cs2}, {0x08, cs2}, {0x09, cs2}, {0x0a, cs2},
                {0x0b, cs2}, {0x0c, cs2}, {0x0d, p3b}, {0x0e, cs2}, {0x0f, cs2},
                {0x00, p4a}, {0x01, cs3}, {0x02, cs3}, {0x03, cs3}, {0x04, p4b},
                {0xf5, err}, {0xf6, err}, {0xf7, err}, {0xf8, err}, {0xf9, err},
                {0xfa, err}, {0xfb, err}, {0xfc, err}, {0xfd, err}, {0xfe, err},
                {0xff, err},
            };

            constexpr char_class octet_classes[256] = {
                asc, asc, asc, asc, asc, asc, asc, asc, asc, asc, asc, asc, asc,
                asc, asc, asc, asc, asc, asc, asc, asc, asc, asc, asc, asc, asc,
                asc, asc, asc, asc, asc, asc, asc, asc, asc, asc, asc, asc, asc,
                asc, asc, asc, asc, asc, asc, asc, asc, asc, asc, asc, asc, asc,
                asc, asc, asc, asc, asc, asc, asc, asc, asc, asc, asc, asc, asc,
                asc, asc, asc, asc, asc, asc, asc, asc, asc, asc, asc, asc, asc,
                asc, asc, asc, asc, asc, asc, asc, asc, asc, asc, asc, asc, asc,
                asc, asc, asc, asc, asc, asc, asc, asc, asc, asc, asc, asc, asc,
                asc, asc, asc, asc, asc, asc, asc, asc, asc, asc, asc, asc, asc,
                asc, asc, asc, asc, asc, asc, asc, asc, asc, asc, asc, cr1, cr1,
                cr1, cr1, cr1, cr1, cr1, cr1, cr1, cr1, cr1, cr1, cr1, cr1, cr1,
                cr1, cr2, cr2, cr2, cr2, cr2, cr2, cr2, cr2, cr2, cr2, cr2, cr2,
                cr2, cr2, cr2, cr2, cr3, cr3, cr3, cr3, cr3, cr3, cr3, cr3, cr3,
                cr3, cr3, cr3, cr3, cr3, cr3, cr3, cr3, cr3, cr3, cr3, cr3, cr3,
                cr3, cr3, cr3, cr3, cr3, cr3, cr3, cr3, cr3, cr3, ill, ill, l2a,
                l2a, l2a, l2a, l2a, l2a, l2a, l2a, l2a, l2a, l2a, l2a, l2a, l2a,
                l2a, l2a, l2a, l2a, l2a, l2a, l2a, l2a, l2a, l2a, l2a, l2a, l2a,
                l2a, l2a, l2a, l3a, l3b, l3b, l3b, l3b, l3b, l3b, l3b, l3b, l3b,
                l3b, l3b, l3b, l3c, l3b, l3b, l4a, l4b, l4b, l4b, l4c, ill, ill,
                ill, ill, ill, ill, ill, ill, ill, ill, ill,
            };

            constexpr table_state transitions[108] = {
                err, e_d, err, err, err, cs1, p3a, cs2, p3b, p4a, cs3, p4b,
                err, err, err, err, err, err, err, err, err, err, err, err,
                err, err, e_d, e_d, e_d, err, err, err, err, err, err, err,
                err, err, cs1, cs1, cs1, err, err, err, err, err, err, err,
                err, err, cs2, cs2, cs2, err, err, err, err, err, err, err,
                err, err, err, err, cs1, err, err, err, err, err, err, err,
                err, err, cs1, cs1, err, err, err, err, err, err, err, err,
                err, err, err, cs2, cs2, err, err, err, err, err, err, err,
                err, err, cs2, err, err, err, err, err, err, err, err, err,
            };
        }

        template<typename InputIter, typename Sentinel>
        char32_t advance(InputIter & first, Sentinel last)
        {
            char32_t retval = 0;

            first_cu const info = first_cus[(unsigned char)*first];
            ++first;

            retval = info.initial_octet;
            int state = info.next;

            while (state != bgn) {
                if (first != last) {
                    unsigned char const cu = *first;
                    retval = (retval << 6) | (cu & 0x3f);
                    char_class const class_ = octet_classes[cu];
                    state = transitions[state + class_];
                    if (state == err)
                        return replacement_character;
                    ++first;
                } else {
                    return replacement_character;
                }
            }

            return retval;
        }

        template<typename Derived, typename Iter>
        struct trans_ins_iter
        {
            using value_type = void;
            using difference_type =



                void;

            using pointer = void;
            using reference = void;
            using iterator_category = std::output_iterator_tag;

            constexpr trans_ins_iter() {}
            constexpr trans_ins_iter(Iter it) : it_(it) {}
            constexpr Derived & operator*() { return derived(); }
            constexpr Derived & operator++() { return derived(); }
            constexpr Derived operator++(int) { return derived(); }
            constexpr Iter base() const { return it_; }

        protected:
            constexpr Iter & iter() { return it_; }

        private:
            constexpr Derived & derived()
            {
                return static_cast<Derived &>(*this);
            }
            Iter it_;
        };

        template<typename Derived, typename I, typename ValueType>
        using trans_iter = stl_interfaces::iterator_interface<
            Derived,
            bidirectional_at_most_t<I>,
            ValueType,
            ValueType>;
    }

}}

namespace boost::parser::detail { namespace text { inline namespace v1 {
#line 671 "boost/parser/detail/text/transcode_iterator.hpp"
    template<typename Iter>
    constexpr detail::enable_utf8_cp_t<Iter>
    find_invalid_encoding(Iter first, Iter last)
    {
        while (first != last) {
            int const cp_bytes = boost::parser::detail::text::utf8_code_units(*first);
            if (cp_bytes == -1 || last - first < cp_bytes)
                return first;

            if (detail::end_of_invalid_utf8(first))
                return first;

            first += cp_bytes;
        }

        return last;
    }

    template<typename Iter>
    constexpr detail::enable_utf16_cp_t<Iter>
    find_invalid_encoding(Iter first, Iter last)
    {
        while (first != last) {
            int const cp_units = boost::parser::detail::text::utf16_code_units(*first);
            if (cp_units == -1 || last - first < cp_units)
                return first;

            if (cp_units == 2 && !boost::parser::detail::text::low_surrogate(*(first + 1)))
                return first;

            first += cp_units;
        }

        return last;
    }

    template<typename Iter>
    constexpr detail::enable_utf8_cp_t<Iter, bool> encoded(
        Iter first, Iter last)
    {
        return v1::find_invalid_encoding(first, last) == last;
    }

    template<typename Iter>
    constexpr detail::enable_utf16_cp_t<Iter, bool> encoded(
        Iter first, Iter last)
    {
        return v1::find_invalid_encoding(first, last) == last;
    }

    template<typename Iter>
    constexpr detail::enable_utf8_cp_t<Iter, bool>
    starts_encoded(Iter first, Iter last)
    {
        if (first == last)
            return true;

        int const cp_bytes = boost::parser::detail::text::utf8_code_units(*first);
        if (cp_bytes == -1 || last - first < cp_bytes)
            return false;

        return !detail::end_of_invalid_utf8(first);
    }

    template<typename Iter>
    constexpr detail::enable_utf16_cp_t<Iter, bool>
    starts_encoded(Iter first, Iter last)
    {
        if (first == last)
            return true;

        int const cp_units = boost::parser::detail::text::utf16_code_units(*first);
        if (cp_units == -1 || last - first < cp_units)
            return false;

        return cp_units == 1 || boost::parser::detail::text::low_surrogate(*(first + 1));
    }

    template<typename Iter>
    constexpr detail::enable_utf8_cp_t<Iter, bool>
    ends_encoded(Iter first, Iter last)
    {
        if (first == last)
            return true;

        auto it = last;
        while (first != --it && boost::parser::detail::text::continuation(*it))
            ;

        return v1::starts_encoded(it, last);
    }

    template<typename Iter>
    constexpr detail::enable_utf16_cp_t<Iter, bool>
    ends_encoded(Iter first, Iter last)
    {
        if (first == last)
            return true;

        auto it = last;
        if (boost::parser::detail::text::low_surrogate(*--it))
            --it;

        return v1::starts_encoded(it, last);
    }

}}}
#line 923 "boost/parser/detail/text/transcode_iterator.hpp"
namespace boost::parser::detail { namespace text {



    struct use_replacement_character
    {
        constexpr char32_t operator()(std::string_view) const noexcept
        {
            return replacement_character;
        }
    };



    struct null_sentinel_t
    {





        template<typename I>

        friend constexpr bool operator==(I it, null_sentinel_t)
        {
            return *it == detail::iter_value_t<I>{};
        }

        template<typename I>
        friend constexpr bool operator==(null_sentinel_t, I it)
        {
            return *it == detail::iter_value_t<I>{};
        }
        template<typename I>
        friend constexpr bool operator!=(I it, null_sentinel_t)
        {
            return *it != detail::iter_value_t<I>{};
        }
        template<typename I>
        friend constexpr bool operator!=(null_sentinel_t, I it)
        {
            return *it != detail::iter_value_t<I>{};
        }

    };




    namespace {
        constexpr null_sentinel_t null_sentinel;
    }






    template<typename Iter>

    struct utf_32_to_8_out_iterator
        : detail::trans_ins_iter<utf_32_to_8_out_iterator<Iter>, Iter>
    {
        constexpr utf_32_to_8_out_iterator() {}
        explicit constexpr utf_32_to_8_out_iterator(Iter it) :
            detail::trans_ins_iter<utf_32_to_8_out_iterator<Iter>, Iter>(it)
        {}

        constexpr utf_32_to_8_out_iterator & operator=(char32_t cp)
        {
            auto & out = this->iter();
            out = detail::write_cp_utf8(cp, out);
            return *this;
        }
    };



    template<typename Cont>
    struct utf_32_to_8_insert_iterator : detail::trans_ins_iter<
                                             utf_32_to_8_insert_iterator<Cont>,
                                             std::insert_iterator<Cont>>
    {
        constexpr utf_32_to_8_insert_iterator() {}
        constexpr utf_32_to_8_insert_iterator(
            Cont & c, typename Cont::iterator it) :
            detail::trans_ins_iter<
                utf_32_to_8_insert_iterator<Cont>,
                std::insert_iterator<Cont>>(std::insert_iterator<Cont>(c, it))
        {}

        constexpr utf_32_to_8_insert_iterator & operator=(char32_t cp)
        {
            auto & out = this->iter();
            out = detail::write_cp_utf8(cp, out);
            return *this;
        }
    };



    template<typename Cont>
    struct utf_32_to_8_front_insert_iterator
        : detail::trans_ins_iter<
              utf_32_to_8_front_insert_iterator<Cont>,
              std::front_insert_iterator<Cont>>
    {
        constexpr utf_32_to_8_front_insert_iterator() {}
        explicit constexpr utf_32_to_8_front_insert_iterator(Cont & c) :
            detail::trans_ins_iter<
                utf_32_to_8_front_insert_iterator<Cont>,
                std::front_insert_iterator<Cont>>(
                std::front_insert_iterator<Cont>(c))
        {}

        constexpr utf_32_to_8_front_insert_iterator & operator=(char32_t cp)
        {
            auto & out = this->iter();
            out = detail::write_cp_utf8(cp, out);
            return *this;
        }
    };



    template<typename Cont>
    struct utf_32_to_8_back_insert_iterator
        : detail::trans_ins_iter<
              utf_32_to_8_back_insert_iterator<Cont>,
              std::back_insert_iterator<Cont>>
    {
        constexpr utf_32_to_8_back_insert_iterator() {}
        explicit constexpr utf_32_to_8_back_insert_iterator(Cont & c) :
            detail::trans_ins_iter<
                utf_32_to_8_back_insert_iterator<Cont>,
                std::back_insert_iterator<Cont>>(
                std::back_insert_iterator<Cont>(c))
        {}

        constexpr utf_32_to_8_back_insert_iterator & operator=(char32_t cp)
        {
            auto & out = this->iter();
            out = detail::write_cp_utf8(cp, out);
            return *this;
        }
    };


    namespace detail {
        template<typename OutIter>
        OutIter assign_8_to_32_insert(
            unsigned char cu, char32_t & cp, int & state, OutIter out)
        {
            auto write = [&] {
                *out = cp;
                ++out;
                state = invalid_table_state;
            };
            auto start_cp = [&] {
                first_cu const info = first_cus[cu];
                state = info.next;
                cp = info.initial_octet;
                if (state == bgn)
                    write();
            };
            if (state == invalid_table_state) {
                start_cp();
            } else {
                cp = (cp << 6) | (cu & 0x3f);
                char_class const class_ = octet_classes[cu];
                state = transitions[state + class_];
                if (state == bgn) {
                    write();
                } else if (state == err) {
                    *out = replacement_character;
                    ++out;
                    start_cp();
                }
            }
            return out;
        }
    }





    template<typename Iter>

    struct utf_8_to_32_out_iterator
        : detail::trans_ins_iter<utf_8_to_32_out_iterator<Iter>, Iter>
    {
        constexpr utf_8_to_32_out_iterator() {}
        explicit constexpr utf_8_to_32_out_iterator(Iter it) :
            detail::trans_ins_iter<utf_8_to_32_out_iterator<Iter>, Iter>(it),
            state_(detail::invalid_table_state)
        {}

        constexpr utf_8_to_32_out_iterator & operator=(char8_type cu)
        {
            auto & out = this->iter();
            out = detail::assign_8_to_32_insert(cu, cp_, state_, out);
            return *this;
        }


    private:
        int state_;
        char32_t cp_;

    };



    template<typename Cont>
    struct utf_8_to_32_insert_iterator : detail::trans_ins_iter<
                                             utf_8_to_32_insert_iterator<Cont>,
                                             std::insert_iterator<Cont>>
    {
        constexpr utf_8_to_32_insert_iterator() {}
        constexpr utf_8_to_32_insert_iterator(
            Cont & c, typename Cont::iterator it) :
            detail::trans_ins_iter<
                utf_8_to_32_insert_iterator<Cont>,
                std::insert_iterator<Cont>>(std::insert_iterator<Cont>(c, it)),
            state_(detail::invalid_table_state)
        {}

        constexpr utf_8_to_32_insert_iterator & operator=(char16_t cu)
        {
            auto & out = this->iter();
            out = detail::assign_8_to_32_insert(cu, cp_, state_, out);
            return *this;
        }


    private:
        int state_;
        char32_t cp_;

    };



    template<typename Cont>
    struct utf_8_to_32_front_insert_iterator
        : detail::trans_ins_iter<
              utf_8_to_32_front_insert_iterator<Cont>,
              std::front_insert_iterator<Cont>>
    {
        constexpr utf_8_to_32_front_insert_iterator() {}
        explicit constexpr utf_8_to_32_front_insert_iterator(Cont & c) :
            detail::trans_ins_iter<
                utf_8_to_32_front_insert_iterator<Cont>,
                std::front_insert_iterator<Cont>>(
                std::front_insert_iterator<Cont>(c)),
            state_(detail::invalid_table_state)
        {}

        constexpr utf_8_to_32_front_insert_iterator & operator=(char16_t cu)
        {
            auto & out = this->iter();
            out = detail::assign_8_to_32_insert(cu, cp_, state_, out);
            return *this;
        }


    private:
        int state_;
        char32_t cp_;

    };



    template<typename Cont>
    struct utf_8_to_32_back_insert_iterator
        : detail::trans_ins_iter<
              utf_8_to_32_back_insert_iterator<Cont>,
              std::back_insert_iterator<Cont>>
    {
        constexpr utf_8_to_32_back_insert_iterator() {}
        explicit constexpr utf_8_to_32_back_insert_iterator(Cont & c) :
            detail::trans_ins_iter<
                utf_8_to_32_back_insert_iterator<Cont>,
                std::back_insert_iterator<Cont>>(
                std::back_insert_iterator<Cont>(c)),
            state_(detail::invalid_table_state)
        {}

        constexpr utf_8_to_32_back_insert_iterator & operator=(char16_t cu)
        {
            auto & out = this->iter();
            out = detail::assign_8_to_32_insert(cu, cp_, state_, out);
            return *this;
        }


    private:
        int state_;
        char32_t cp_;

    };






    template<typename Iter>

    struct utf_32_to_16_out_iterator
        : detail::trans_ins_iter<utf_32_to_16_out_iterator<Iter>, Iter>
    {
        constexpr utf_32_to_16_out_iterator() {}
        explicit constexpr utf_32_to_16_out_iterator(Iter it) :
            detail::trans_ins_iter<utf_32_to_16_out_iterator<Iter>, Iter>(it)
        {}

        constexpr utf_32_to_16_out_iterator & operator=(char32_t cp)
        {
            auto & out = this->iter();
            out = detail::write_cp_utf16(cp, out);
            return *this;
        }
    };



    template<typename Cont>
    struct utf_32_to_16_insert_iterator
        : detail::trans_ins_iter<
              utf_32_to_16_insert_iterator<Cont>,
              std::insert_iterator<Cont>>
    {
        constexpr utf_32_to_16_insert_iterator() {}
        constexpr utf_32_to_16_insert_iterator(
            Cont & c, typename Cont::iterator it) :
            detail::trans_ins_iter<
                utf_32_to_16_insert_iterator<Cont>,
                std::insert_iterator<Cont>>(std::insert_iterator<Cont>(c, it))
        {}

        constexpr utf_32_to_16_insert_iterator & operator=(char32_t cp)
        {
            auto & out = this->iter();
            out = detail::write_cp_utf16(cp, out);
            return *this;
        }
    };



    template<typename Cont>
    struct utf_32_to_16_front_insert_iterator
        : detail::trans_ins_iter<
              utf_32_to_16_front_insert_iterator<Cont>,
              std::front_insert_iterator<Cont>>
    {
        constexpr utf_32_to_16_front_insert_iterator() {}
        explicit constexpr utf_32_to_16_front_insert_iterator(Cont & c) :
            detail::trans_ins_iter<
                utf_32_to_16_front_insert_iterator<Cont>,
                std::front_insert_iterator<Cont>>(
                std::front_insert_iterator<Cont>(c))
        {}

        constexpr utf_32_to_16_front_insert_iterator & operator=(char32_t cp)
        {
            auto & out = this->iter();
            out = detail::write_cp_utf16(cp, out);
            return *this;
        }
    };



    template<typename Cont>
    struct utf_32_to_16_back_insert_iterator
        : detail::trans_ins_iter<
              utf_32_to_16_back_insert_iterator<Cont>,
              std::back_insert_iterator<Cont>>
    {
        constexpr utf_32_to_16_back_insert_iterator() {}
        explicit constexpr utf_32_to_16_back_insert_iterator(Cont & c) :
            detail::trans_ins_iter<
                utf_32_to_16_back_insert_iterator<Cont>,
                std::back_insert_iterator<Cont>>(
                std::back_insert_iterator<Cont>(c))
        {}

        constexpr utf_32_to_16_back_insert_iterator & operator=(char32_t cp)
        {
            auto & out = this->iter();
            out = detail::write_cp_utf16(cp, out);
            return *this;
        }
    };


    namespace detail {
        template<typename OutIter>
        OutIter
        assign_16_to_32_insert(char16_t & prev_cu, char16_t cu, OutIter out)
        {
            if (high_surrogate(cu)) {
                if (prev_cu) {
                    *out = replacement_character;
                    ++out;
                }
                prev_cu = cu;
            } else if (low_surrogate(cu)) {
                if (prev_cu) {
                    *out = detail::surrogates_to_cp(prev_cu, cu);
                    ++out;
                } else {
                    *out = replacement_character;
                    ++out;
                }
                prev_cu = 0;
            } else {
                if (prev_cu) {
                    *out = replacement_character;
                    ++out;
                }
                *out = cu;
                ++out;
                prev_cu = 0;
            }
            return out;
        }
    }





    template<typename Iter>

    struct utf_16_to_32_out_iterator
        : detail::trans_ins_iter<utf_16_to_32_out_iterator<Iter>, Iter>
    {
        constexpr utf_16_to_32_out_iterator() {}
        explicit constexpr utf_16_to_32_out_iterator(Iter it) :
            detail::trans_ins_iter<utf_16_to_32_out_iterator<Iter>, Iter>(it),
            prev_cu_(0)
        {}

        constexpr utf_16_to_32_out_iterator & operator=(char16_t cu)
        {
            auto & out = this->iter();
            out = detail::assign_16_to_32_insert(prev_cu_, cu, out);
            return *this;
        }


    private:
        char16_t prev_cu_;

    };



    template<typename Cont>
    struct utf_16_to_32_insert_iterator
        : detail::trans_ins_iter<
              utf_16_to_32_insert_iterator<Cont>,
              std::insert_iterator<Cont>>
    {
        constexpr utf_16_to_32_insert_iterator() {}
        constexpr utf_16_to_32_insert_iterator(
            Cont & c, typename Cont::iterator it) :
            detail::trans_ins_iter<
                utf_16_to_32_insert_iterator<Cont>,
                std::insert_iterator<Cont>>(std::insert_iterator<Cont>(c, it)),
            prev_cu_(0)
        {}

        constexpr utf_16_to_32_insert_iterator & operator=(char16_t cu)
        {
            auto & out = this->iter();
            out = detail::assign_16_to_32_insert(prev_cu_, cu, out);
            return *this;
        }


    private:
        char16_t prev_cu_;

    };



    template<typename Cont>
    struct utf_16_to_32_front_insert_iterator
        : detail::trans_ins_iter<
              utf_16_to_32_front_insert_iterator<Cont>,
              std::front_insert_iterator<Cont>>
    {
        constexpr utf_16_to_32_front_insert_iterator() {}
        explicit constexpr utf_16_to_32_front_insert_iterator(Cont & c) :
            detail::trans_ins_iter<
                utf_16_to_32_front_insert_iterator<Cont>,
                std::front_insert_iterator<Cont>>(
                std::front_insert_iterator<Cont>(c)),
            prev_cu_(0)
        {}

        constexpr utf_16_to_32_front_insert_iterator & operator=(char16_t cu)
        {
            auto & out = this->iter();
            out = detail::assign_16_to_32_insert(prev_cu_, cu, out);
            return *this;
        }


    private:
        char16_t prev_cu_;

    };



    template<typename Cont>
    struct utf_16_to_32_back_insert_iterator
        : detail::trans_ins_iter<
              utf_16_to_32_back_insert_iterator<Cont>,
              std::back_insert_iterator<Cont>>
    {
        constexpr utf_16_to_32_back_insert_iterator() {}
        explicit constexpr utf_16_to_32_back_insert_iterator(Cont & c) :
            detail::trans_ins_iter<
                utf_16_to_32_back_insert_iterator<Cont>,
                std::back_insert_iterator<Cont>>(
                std::back_insert_iterator<Cont>(c)),
            prev_cu_(0)
        {}

        constexpr utf_16_to_32_back_insert_iterator & operator=(char16_t cu)
        {
            auto & out = this->iter();
            out = detail::assign_16_to_32_insert(prev_cu_, cu, out);
            return *this;
        }


    private:
        char16_t prev_cu_;

    };


    namespace detail {
        template<typename OutIter>
        OutIter
        assign_16_to_8_insert(char16_t & prev_cu, char16_t cu, OutIter out)
        {
            if (high_surrogate(cu)) {
                if (prev_cu)
                    out = detail::write_cp_utf8(replacement_character, out);
                prev_cu = cu;
            } else if (low_surrogate(cu)) {
                if (prev_cu) {
                    auto const cp = detail::surrogates_to_cp(prev_cu, cu);
                    out = detail::write_cp_utf8(cp, out);
                } else {
                    out = detail::write_cp_utf8(replacement_character, out);
                }
                prev_cu = 0;
            } else {
                if (prev_cu)
                    out = detail::write_cp_utf8(replacement_character, out);
                out = detail::write_cp_utf8(cu, out);
                prev_cu = 0;
            }
            return out;
        }
    }





    template<typename Iter>

    struct utf_16_to_8_out_iterator
        : detail::trans_ins_iter<utf_16_to_8_out_iterator<Iter>, Iter>
    {
        constexpr utf_16_to_8_out_iterator() {}
        explicit constexpr utf_16_to_8_out_iterator(Iter it) :
            detail::trans_ins_iter<utf_16_to_8_out_iterator<Iter>, Iter>(it),
            prev_cu_(0)
        {}

        constexpr utf_16_to_8_out_iterator & operator=(char16_t cu)
        {
            auto & out = this->iter();
            out = detail::assign_16_to_8_insert(prev_cu_, cu, out);
            return *this;
        }


    private:
        char16_t prev_cu_;

    };



    template<typename Cont>
    struct utf_16_to_8_insert_iterator : detail::trans_ins_iter<
                                             utf_16_to_8_insert_iterator<Cont>,
                                             std::insert_iterator<Cont>>
    {
        constexpr utf_16_to_8_insert_iterator() {}
        constexpr utf_16_to_8_insert_iterator(
            Cont & c, typename Cont::iterator it) :
            detail::trans_ins_iter<
                utf_16_to_8_insert_iterator<Cont>,
                std::insert_iterator<Cont>>(std::insert_iterator<Cont>(c, it)),
            prev_cu_(0)
        {}

        constexpr utf_16_to_8_insert_iterator & operator=(char16_t cu)
        {
            auto & out = this->iter();
            out = detail::assign_16_to_8_insert(prev_cu_, cu, out);
            return *this;
        }


    private:
        char16_t prev_cu_;

    };



    template<typename Cont>
    struct utf_16_to_8_front_insert_iterator
        : detail::trans_ins_iter<
              utf_16_to_8_front_insert_iterator<Cont>,
              std::front_insert_iterator<Cont>>
    {
        constexpr utf_16_to_8_front_insert_iterator() {}
        explicit constexpr utf_16_to_8_front_insert_iterator(Cont & c) :
            detail::trans_ins_iter<
                utf_16_to_8_front_insert_iterator<Cont>,
                std::front_insert_iterator<Cont>>(
                std::front_insert_iterator<Cont>(c)),
            prev_cu_(0)
        {}

        constexpr utf_16_to_8_front_insert_iterator & operator=(char16_t cu)
        {
            auto & out = this->iter();
            out = detail::assign_16_to_8_insert(prev_cu_, cu, out);
            return *this;
        }


    private:
        char16_t prev_cu_;

    };



    template<typename Cont>
    struct utf_16_to_8_back_insert_iterator
        : detail::trans_ins_iter<
              utf_16_to_8_back_insert_iterator<Cont>,
              std::back_insert_iterator<Cont>>
    {
        constexpr utf_16_to_8_back_insert_iterator() {}
        explicit constexpr utf_16_to_8_back_insert_iterator(Cont & c) :
            detail::trans_ins_iter<
                utf_16_to_8_back_insert_iterator<Cont>,
                std::back_insert_iterator<Cont>>(
                std::back_insert_iterator<Cont>(c)),
            prev_cu_(0)
        {}

        constexpr utf_16_to_8_back_insert_iterator & operator=(char16_t cu)
        {
            auto & out = this->iter();
            out = detail::assign_16_to_8_insert(prev_cu_, cu, out);
            return *this;
        }


    private:
        char16_t prev_cu_;

    };


    namespace detail {
        template<typename OutIter>
        OutIter assign_8_to_16_insert(
            unsigned char cu, char32_t & cp, int & state, OutIter out)
        {
            auto write = [&] {
                out = detail::write_cp_utf16(cp, out);
                state = invalid_table_state;
            };
            auto start_cp = [&] {
                first_cu const info = first_cus[cu];
                state = info.next;
                cp = info.initial_octet;
                if (state == bgn)
                    write();
            };
            if (state == invalid_table_state) {
                start_cp();
            } else {
                cp = (cp << 6) | (cu & 0x3f);
                char_class const class_ = octet_classes[cu];
                state = transitions[state + class_];
                if (state == bgn) {
                    write();
                } else if (state == err) {
                    out = detail::write_cp_utf16(replacement_character, out);
                    start_cp();
                }
            }
            return out;
        }
    }





    template<typename Iter>

    struct utf_8_to_16_out_iterator
        : detail::trans_ins_iter<utf_8_to_16_out_iterator<Iter>, Iter>
    {
        constexpr utf_8_to_16_out_iterator() {}
        explicit constexpr utf_8_to_16_out_iterator(Iter it) :
            detail::trans_ins_iter<utf_8_to_16_out_iterator<Iter>, Iter>(it),
            state_(detail::invalid_table_state)
        {}

        constexpr utf_8_to_16_out_iterator & operator=(char8_type cu)
        {
            auto & out = this->iter();
            out = detail::assign_8_to_16_insert(cu, cp_, state_, out);
            return *this;
        }


    private:
        int state_;
        char32_t cp_;

    };



    template<typename Cont>
    struct utf_8_to_16_insert_iterator : detail::trans_ins_iter<
                                             utf_8_to_16_insert_iterator<Cont>,
                                             std::insert_iterator<Cont>>
    {
        constexpr utf_8_to_16_insert_iterator() {}
        constexpr utf_8_to_16_insert_iterator(
            Cont & c, typename Cont::iterator it) :
            detail::trans_ins_iter<
                utf_8_to_16_insert_iterator<Cont>,
                std::insert_iterator<Cont>>(std::insert_iterator<Cont>(c, it)),
            state_(detail::invalid_table_state)
        {}

        constexpr utf_8_to_16_insert_iterator & operator=(char16_t cu)
        {
            auto & out = this->iter();
            out = detail::assign_8_to_16_insert(cu, cp_, state_, out);
            return *this;
        }


    private:
        int state_;
        char32_t cp_;

    };



    template<typename Cont>
    struct utf_8_to_16_front_insert_iterator
        : detail::trans_ins_iter<
              utf_8_to_16_front_insert_iterator<Cont>,
              std::front_insert_iterator<Cont>>
    {
        constexpr utf_8_to_16_front_insert_iterator() {}
        explicit constexpr utf_8_to_16_front_insert_iterator(Cont & c) :
            detail::trans_ins_iter<
                utf_8_to_16_front_insert_iterator<Cont>,
                std::front_insert_iterator<Cont>>(
                std::front_insert_iterator<Cont>(c)),
            state_(detail::invalid_table_state)
        {}

        constexpr utf_8_to_16_front_insert_iterator & operator=(char16_t cu)
        {
            auto & out = this->iter();
            out = detail::assign_8_to_16_insert(cu, cp_, state_, out);
            return *this;
        }


    private:
        int state_;
        char32_t cp_;

    };



    template<typename Cont>
    struct utf_8_to_16_back_insert_iterator
        : detail::trans_ins_iter<
              utf_8_to_16_back_insert_iterator<Cont>,
              std::back_insert_iterator<Cont>>
    {
        constexpr utf_8_to_16_back_insert_iterator() {}
        explicit constexpr utf_8_to_16_back_insert_iterator(Cont & c) :
            detail::trans_ins_iter<
                utf_8_to_16_back_insert_iterator<Cont>,
                std::back_insert_iterator<Cont>>(
                std::back_insert_iterator<Cont>(c)),
            state_(detail::invalid_table_state)
        {}

        constexpr utf_8_to_16_back_insert_iterator & operator=(char16_t cu)
        {
            auto & out = this->iter();
            out = detail::assign_8_to_16_insert(cu, cp_, state_, out);
            return *this;
        }


    private:
        int state_;
        char32_t cp_;

    };

}}
#line 7 "boost/parser/detail/text/unpack.hpp"
#define BOOST_PARSER_DETAIL_TEXT_UNPACK_HPP



#include <type_traits>
#include <optional>


namespace boost::parser::detail { namespace text {

    struct no_op_repacker
    {
        template<class T>
        T operator()(T x) const
        {
            return x;
        }
    };

    namespace detail {


        template<
            typename RepackedIterator,
            typename I,
            typename S,
            typename Then,
            bool Bidi>
        struct repacker
        {
            repacker() = default;

            template<bool Enable = Bidi, typename = std::enable_if_t<Enable>>

            repacker(I first, S last, Then then)



                : first{first},
                  last{last},
                  then{then}
            {}

            template<bool Enable = !Bidi, typename = std::enable_if_t<Enable>>

            repacker(S last, Then then)



                :
                last{last}, then{then}
            {}

            auto operator()(I it) const
            {
                if constexpr (Bidi) {
                    return then(RepackedIterator(*first, it, last));
                } else {
                    return then(RepackedIterator(it, last));
                }
            }

            std::optional<I> first;
            [[no_unique_address]] S last;
            [[no_unique_address]] Then then;
        };

        template<typename I, typename S, typename Repack>
        constexpr auto
        unpack_iterator_and_sentinel_impl(I first, S last, Repack repack);

        template<
            format FromFormat,
            format ToFormat,
            typename I,
            typename S,
            typename ErrorHandler,
            typename Repack>
        constexpr auto unpack_iterator_and_sentinel_impl(
            utf_iterator<FromFormat, ToFormat, I, S, ErrorHandler> first,
            utf_iterator<FromFormat, ToFormat, I, S, ErrorHandler> last,
            Repack repack);

        template<
            format FromFormat,
            format ToFormat,
            typename I,
            typename S,
            typename ErrorHandler,
            typename Repack>
        constexpr auto unpack_iterator_and_sentinel_impl(
            utf_iterator<FromFormat, ToFormat, I, S, ErrorHandler> first,
            S last,
            Repack repack);

        template<typename I, typename S, typename Repack>
        constexpr auto
        unpack_iterator_and_sentinel(I first, S last, Repack repack)
        {
            return detail::unpack_iterator_and_sentinel_impl(
                first, last, repack);
        }

        struct unpack_iterator_and_sentinel_cpo
        {
#line 119 "boost/parser/detail/text/unpack.hpp"
            template<typename I, typename S, typename Repack = no_op_repacker>

            constexpr auto
            operator()(I first, S last, Repack repack = Repack()) const
            {
                return unpack_iterator_and_sentinel(first, last, repack);
            }
        };
    }

    inline namespace cpo {
        inline constexpr detail::unpack_iterator_and_sentinel_cpo
            unpack_iterator_and_sentinel{};
    }




    template<format FormatTag, typename I, typename S, class Repack>

    struct unpack_result
    {
        static constexpr format format_tag = FormatTag;

        I first;
        [[no_unique_address]] S last;
        [[no_unique_address]] Repack repack;
    };

    namespace detail {
        struct no_such_type
        {};
        template<typename I, typename S, typename Repack>
        constexpr auto
        unpack_iterator_and_sentinel_impl(I first, S last, Repack repack)
        {
            using value_type = detail::iter_value_t<I>;
            if constexpr (
                std::is_same_v<value_type, char>



            ) {
                return unpack_result<format::utf8, I, S, Repack>{
                    first, last, repack};
            } else if constexpr (



                std::is_same_v<value_type, char16_t>) {
                return unpack_result<format::utf16, I, S, Repack>{
                    first, last, repack};
            } else if constexpr (

                std::is_same_v<value_type, wchar_t> ||

                std::is_same_v<value_type, char32_t>) {
                return unpack_result<format::utf32, I, S, Repack>{
                    first, last, repack};
            } else {
                static_assert(
                    std::is_same_v<Repack, no_such_type>,
                    "Unpacked iterator is not a utf_iter!");
                return 0;
            }
        }

    }
}}
#line 191 "boost/parser/detail/text/unpack.hpp"
namespace boost::parser::detail { namespace text { namespace detail {

    template<
        format FromFormat,
        format ToFormat,
        typename I,
        typename S,
        typename ErrorHandler,
        typename Repack>
    constexpr auto unpack_iterator_and_sentinel_impl(
        utf_iterator<FromFormat, ToFormat, I, S, ErrorHandler> first,
        utf_iterator<FromFormat, ToFormat, I, S, ErrorHandler> last,
        Repack repack)
    {
        using iterator = utf_iterator<FromFormat, ToFormat, I, S, ErrorHandler>;
        if constexpr (



            std::is_base_of_v<
                std::bidirectional_iterator_tag,
                typename std::iterator_traits<I>::iterator_category>

        ) {
            return boost::parser::detail::text::unpack_iterator_and_sentinel(
                first.base(),
                last.base(),
                repacker<
                    iterator,
                    decltype(first.begin()),
                    decltype(first.end()),
                    Repack,
                    true>(first.begin(), first.end(), repack));
        } else {
            return boost::parser::detail::text::unpack_iterator_and_sentinel(
                first.base(),
                last.base(),
                repacker<iterator, int, decltype(first.end()), Repack, false>(
                    first.end(), repack));
        }
    }

    template<
        format FromFormat,
        format ToFormat,
        typename I,
        typename S,
        typename ErrorHandler,
        typename Repack>
    constexpr auto unpack_iterator_and_sentinel_impl(
        utf_iterator<FromFormat, ToFormat, I, S, ErrorHandler> first,
        S last,
        Repack repack)
    {
        using iterator = utf_iterator<FromFormat, ToFormat, I, S, ErrorHandler>;
        if constexpr (



            std::is_base_of_v<
                std::bidirectional_iterator_tag,
                typename std::iterator_traits<I>::iterator_category>

        ) {
            return boost::parser::detail::text::unpack_iterator_and_sentinel(
                first.base(),
                last,
                repacker<
                    iterator,
                    decltype(first.begin()),
                    decltype(first.end()),
                    Repack,
                    true>(first.begin(), first.end(), repack));
        } else {
            return boost::parser::detail::text::unpack_iterator_and_sentinel(
                first.base(),
                last,
                repacker<iterator, int, S, Repack, false>(last, repack));
        }
    }

}}}
#line 1778 "boost/parser/detail/text/transcode_iterator.hpp"
namespace boost::parser::detail { namespace text { namespace detail {

    template<format Tag>
    struct make_utf8_dispatch;

    template<>
    struct make_utf8_dispatch<format::utf8>
    {
        template<typename Iter, typename Sentinel>
        static constexpr Iter call(Iter first, Iter it, Sentinel last)
        {
            return it;
        }
    };

    template<>
    struct make_utf8_dispatch<format::utf16>
    {
        template<typename Iter, typename Sentinel>
        static constexpr utf_iterator<
            format::utf16,
            format::utf8,
            Iter,
            Sentinel>
        call(Iter first, Iter it, Sentinel last)
        {
            return {first, it, last};
        }
    };

    template<>
    struct make_utf8_dispatch<format::utf32>
    {
        template<typename Iter, typename Sentinel>
        static constexpr utf_iterator<
            format::utf32,
            format::utf8,
            Iter,
            Sentinel>
        call(Iter first, Iter it, Sentinel last)
        {
            return {first, it, last};
        }
    };

    template<format Tag>
    struct make_utf16_dispatch;

    template<>
    struct make_utf16_dispatch<format::utf8>
    {
        template<typename Iter, typename Sentinel>
        static constexpr utf_iterator<
            format::utf8,
            format::utf16,
            Iter,
            Sentinel>
        call(Iter first, Iter it, Sentinel last)
        {
            return {first, it, last};
        }
    };

    template<>
    struct make_utf16_dispatch<format::utf16>
    {
        template<typename Iter, typename Sentinel>
        static constexpr Iter call(Iter first, Iter it, Sentinel last)
        {
            return it;
        }
    };

    template<>
    struct make_utf16_dispatch<format::utf32>
    {
        template<typename Iter, typename Sentinel>
        static constexpr utf_iterator<
            format::utf32,
            format::utf16,
            Iter,
            Sentinel>
        call(Iter first, Iter it, Sentinel last)
        {
            return {first, it, last};
        }
    };

    template<format Tag>
    struct make_utf32_dispatch;

    template<>
    struct make_utf32_dispatch<format::utf8>
    {
        template<typename Iter, typename Sentinel>
        static constexpr utf_iterator<
            format::utf8,
            format::utf32,
            Iter,
            Sentinel>
        call(Iter first, Iter it, Sentinel last)
        {
            return {first, it, last};
        }
    };

    template<>
    struct make_utf32_dispatch<format::utf16>
    {
        template<typename Iter, typename Sentinel>
        static constexpr utf_iterator<
            format::utf16,
            format::utf32,
            Iter,
            Sentinel>
        call(Iter first, Iter it, Sentinel last)
        {
            return {first, it, last};
        }
    };

    template<>
    struct make_utf32_dispatch<format::utf32>
    {
        template<typename Iter, typename Sentinel>
        static constexpr Iter call(Iter first, Iter it, Sentinel last)
        {
            return it;
        }
    };

    template<
        typename Cont,
        typename UTF8,
        typename UTF16,
        typename UTF32,
        int Bytes = sizeof(typename Cont::value_type)>
    struct from_utf8_dispatch
    {
        using type = UTF8;
    };

    template<typename Cont, typename UTF8, typename UTF16, typename UTF32>
    struct from_utf8_dispatch<Cont, UTF8, UTF16, UTF32, 2>
    {
        using type = UTF16;
    };

    template<typename Cont, typename UTF8, typename UTF16, typename UTF32>
    struct from_utf8_dispatch<Cont, UTF8, UTF16, UTF32, 4>
    {
        using type = UTF32;
    };

    template<typename Cont, typename UTF8, typename UTF16, typename UTF32>
    using from_utf8_dispatch_t =
        typename from_utf8_dispatch<Cont, UTF8, UTF16, UTF32>::type;

    template<
        typename Cont,
        typename UTF8,
        typename UTF16,
        typename UTF32,
        int Bytes = sizeof(typename Cont::value_type)>
    struct from_utf16_dispatch
    {
        using type = UTF16;
    };

    template<typename Cont, typename UTF8, typename UTF16, typename UTF32>
    struct from_utf16_dispatch<Cont, UTF8, UTF16, UTF32, 1>
    {
        using type = UTF8;
    };

    template<typename Cont, typename UTF8, typename UTF16, typename UTF32>
    struct from_utf16_dispatch<Cont, UTF8, UTF16, UTF32, 4>
    {
        using type = UTF32;
    };

    template<typename Cont, typename UTF8, typename UTF16, typename UTF32>
    using from_utf16_dispatch_t =
        typename from_utf16_dispatch<Cont, UTF8, UTF16, UTF32>::type;

    template<
        typename Cont,
        typename UTF8,
        typename UTF16,
        typename UTF32,
        int Bytes = sizeof(typename Cont::value_type)>
    struct from_utf32_dispatch
    {
        using type = UTF32;
    };

    template<typename Cont, typename UTF8, typename UTF16, typename UTF32>
    struct from_utf32_dispatch<Cont, UTF8, UTF16, UTF32, 1>
    {
        using type = UTF8;
    };

    template<typename Cont, typename UTF8, typename UTF16, typename UTF32>
    struct from_utf32_dispatch<Cont, UTF8, UTF16, UTF32, 2>
    {
        using type = UTF16;
    };

    template<typename Cont, typename UTF8, typename UTF16, typename UTF32>
    using from_utf32_dispatch_t =
        typename from_utf32_dispatch<Cont, UTF8, UTF16, UTF32>::type;

}}}

namespace boost::parser::detail { namespace text { inline namespace v1 {
#line 2124 "boost/parser/detail/text/transcode_iterator.hpp"
    template<typename Iter>
    utf_32_to_8_out_iterator<Iter> utf_32_to_8_out(Iter it)
    {
        return utf_32_to_8_out_iterator<Iter>(it);
    }

    template<typename Iter>
    utf_8_to_32_out_iterator<Iter> utf_8_to_32_out(Iter it)
    {
        return utf_8_to_32_out_iterator<Iter>(it);
    }

    template<typename Iter>
    utf_32_to_16_out_iterator<Iter> utf_32_to_16_out(Iter it)
    {
        return utf_32_to_16_out_iterator<Iter>(it);
    }

    template<typename Iter>
    utf_16_to_32_out_iterator<Iter> utf_16_to_32_out(Iter it)
    {
        return utf_16_to_32_out_iterator<Iter>(it);
    }

    template<typename Iter>
    utf_16_to_8_out_iterator<Iter> utf_16_to_8_out(Iter it)
    {
        return utf_16_to_8_out_iterator<Iter>(it);
    }

    template<typename Iter>
    utf_8_to_16_out_iterator<Iter> utf_8_to_16_out(Iter it)
    {
        return utf_8_to_16_out_iterator<Iter>(it);
    }

    template<typename Iter, typename Sentinel>
    auto utf8_iterator(Iter first, Iter it, Sentinel last)
    {
        auto const unpacked = text::unpack_iterator_and_sentinel(first, last);
        auto const unpacked_it =
            text::unpack_iterator_and_sentinel(it, last).first;
        constexpr format tag = unpacked.format_tag;
        return detail::make_utf8_dispatch<tag>::call(
            unpacked.first, unpacked_it, unpacked.last);
    }

    template<typename Iter, typename Sentinel>
    auto utf16_iterator(Iter first, Iter it, Sentinel last)
    {
        auto const unpacked = text::unpack_iterator_and_sentinel(first, last);
        auto const unpacked_it =
            text::unpack_iterator_and_sentinel(it, last).first;
        constexpr format tag = unpacked.format_tag;
        return detail::make_utf16_dispatch<tag>::call(
            unpacked.first, unpacked_it, unpacked.last);
    }

    template<typename Iter, typename Sentinel>
    auto utf32_iterator(Iter first, Iter it, Sentinel last)
    {
        auto const unpacked = text::unpack_iterator_and_sentinel(first, last);
        auto const unpacked_it =
            text::unpack_iterator_and_sentinel(it, last).first;
        constexpr format tag = unpacked.format_tag;
        return detail::make_utf32_dispatch<tag>::call(
            unpacked.first, unpacked_it, unpacked.last);
    }

    template<typename Cont>
    auto from_utf8_inserter(Cont & c, typename Cont::iterator it)
    {
        using result_type = detail::from_utf8_dispatch_t<
            Cont,
            std::insert_iterator<Cont>,
            utf_8_to_16_insert_iterator<Cont>,
            utf_8_to_32_insert_iterator<Cont>>;
        return result_type(c, it);
    }

    template<typename Cont>
    auto from_utf16_inserter(Cont & c, typename Cont::iterator it)
    {
        using result_type = detail::from_utf16_dispatch_t<
            Cont,
            utf_16_to_8_insert_iterator<Cont>,
            std::insert_iterator<Cont>,
            utf_16_to_32_insert_iterator<Cont>>;
        return result_type(c, it);
    }

    template<typename Cont>
    auto from_utf32_inserter(Cont & c, typename Cont::iterator it)
    {
        using result_type = detail::from_utf32_dispatch_t<
            Cont,
            utf_32_to_8_insert_iterator<Cont>,
            utf_32_to_16_insert_iterator<Cont>,
            std::insert_iterator<Cont>>;
        return result_type(c, it);
    }

    template<typename Cont>
    auto from_utf8_back_inserter(Cont & c)
    {
        using result_type = detail::from_utf8_dispatch_t<
            Cont,
            std::back_insert_iterator<Cont>,
            utf_8_to_16_back_insert_iterator<Cont>,
            utf_8_to_32_back_insert_iterator<Cont>>;
        return result_type(c);
    }

    template<typename Cont>
    auto from_utf16_back_inserter(Cont & c)
    {
        using result_type = detail::from_utf16_dispatch_t<
            Cont,
            utf_16_to_8_back_insert_iterator<Cont>,
            std::back_insert_iterator<Cont>,
            utf_16_to_32_back_insert_iterator<Cont>>;
        return result_type(c);
    }

    template<typename Cont>
    auto from_utf32_back_inserter(Cont & c)
    {
        using result_type = detail::from_utf32_dispatch_t<
            Cont,
            utf_32_to_8_back_insert_iterator<Cont>,
            utf_32_to_16_back_insert_iterator<Cont>,
            std::back_insert_iterator<Cont>>;
        return result_type(c);
    }

    template<typename Cont>
    auto from_utf8_front_inserter(Cont & c)
    {
        using result_type = detail::from_utf8_dispatch_t<
            Cont,
            std::front_insert_iterator<Cont>,
            utf_8_to_16_front_insert_iterator<Cont>,
            utf_8_to_32_front_insert_iterator<Cont>>;
        return result_type(c);
    }

    template<typename Cont>
    auto from_utf16_front_inserter(Cont & c)
    {
        using result_type = detail::from_utf16_dispatch_t<
            Cont,
            utf_16_to_8_front_insert_iterator<Cont>,
            std::front_insert_iterator<Cont>,
            utf_16_to_32_front_insert_iterator<Cont>>;
        return result_type(c);
    }

    template<typename Cont>
    auto from_utf32_front_inserter(Cont & c)
    {
        using result_type = detail::from_utf32_dispatch_t<
            Cont,
            utf_32_to_8_front_insert_iterator<Cont>,
            utf_32_to_16_front_insert_iterator<Cont>,
            std::front_insert_iterator<Cont>>;
        return result_type(c);
    }

}}}
#line 2500 "boost/parser/detail/text/transcode_iterator.hpp"
namespace boost::parser::detail { namespace text {
    namespace detail {
        template<format Format>
        constexpr auto format_to_type()
        {
            if constexpr (Format == format::utf8) {
                return char8_type{};
            } else if constexpr (Format == format::utf16) {
                return char16_t{};
            } else {
                return char32_t{};
            }
        }

        template<typename I>
        constexpr bool is_bidi =



            std::is_base_of_v<
                std::bidirectional_iterator_tag,
                typename std::iterator_traits<I>::iterator_category>

            ;

        template<typename I, bool SupportReverse = is_bidi<I>>
        struct first_and_curr
        {
            first_and_curr() = default;
            first_and_curr(I curr) : curr{curr} {}
            first_and_curr(const first_and_curr & other) = default;
            template<
                class I2

                ,
                typename Enable = std::enable_if_t<std::is_convertible_v<I2, I>>

                >



            first_and_curr(const first_and_curr<I2> & other) : curr{other.curr}
            {}

            I curr;
        };
        template<typename I>
        struct first_and_curr<I, true>
        {
            first_and_curr() = default;
            first_and_curr(I first, I curr) : first{first}, curr{curr} {}
            first_and_curr(const first_and_curr & other) = default;
            template<
                class I2

                ,
                typename Enable = std::enable_if_t<std::is_convertible_v<I2, I>>

                >



            first_and_curr(const first_and_curr<I2> & other) :
                first{other.first}, curr{other.curr}
            {}

            I first;
            I curr;
        };
    }
#line 2580 "boost/parser/detail/text/transcode_iterator.hpp"
    template<
        format FromFormat,
        format ToFormat,
        typename I,
        typename S,
        typename ErrorHandler>

    class utf_iterator
        : public stl_interfaces::iterator_interface<
              utf_iterator<FromFormat, ToFormat, I, S, ErrorHandler>,
              detail::bidirectional_at_most_t<I>,
              detail::format_to_type_t<ToFormat>,
              detail::format_to_type_t<ToFormat>>
    {
        static_assert(
            FromFormat == format::utf8 || FromFormat == format::utf16 ||
            FromFormat == format::utf32);
        static_assert(
            ToFormat == format::utf8 || ToFormat == format::utf16 ||
            ToFormat == format::utf32);


        template<typename T>
        constexpr static bool is_bidirectional = std::is_base_of_v<
            std::bidirectional_iterator_tag,
            detail::bidirectional_at_most_t<T>>;
        template<typename T>
        constexpr static bool is_forward = std::is_base_of_v<
            std::forward_iterator_tag,
            detail::bidirectional_at_most_t<T>>;
        template<typename T>
        constexpr static bool is_input = !is_bidirectional<T> && !is_forward<T>;


        static_assert(



            is_forward<I>

            || noexcept(ErrorHandler{}("")));

    public:
        using value_type = detail::format_to_type_t<ToFormat>;

        constexpr utf_iterator() = default;


        template<
            typename J = I,
            typename Enable = std::enable_if_t<is_bidirectional<J>>>

        constexpr utf_iterator(I first, I it, S last)



            : first_and_curr_{first, it}, last_(last)
        {
            if (curr() != last_)
                read();
        }

        template<
            typename J = I,
            typename Enable = std::enable_if_t<is_bidirectional<J>>>

        constexpr utf_iterator(I it, S last)



            :
            first_and_curr_{it}, last_(last)
        {
            if (curr() != last_)
                read();
        }

        template<
            class I2,
            class S2

            ,
            typename Enable = std::enable_if_t<
                std::is_convertible_v<I2, I> && std::is_convertible_v<S2, S>>

            >



        constexpr utf_iterator(
            utf_iterator<FromFormat, ToFormat, I2, S2, ErrorHandler> const &
                other) :
            buf_(other.buf_),
            first_and_curr_(other.first_and_curr_),
            buf_index_(other.buf_index_),
            buf_last_(other.buf_last_),
            last_(other.last_)
        {}


        template<
            typename J = I,
            typename Enable = std::enable_if_t<is_bidirectional<J>>>

        constexpr I begin() const



        {
            return first();
        }
        constexpr S end() const { return last_; }


        template<
            typename J = I,
            typename Enable = std::enable_if_t<is_forward<J>>>

        constexpr I base() const



        {
            return curr();
        }

        constexpr value_type operator*() const
        {
            BOOST_ASSERT(buf_index_ < buf_last_);
            return buf_[buf_index_];
        }

        constexpr utf_iterator & operator++()
        {
            BOOST_ASSERT(buf_index_ != buf_last_ || curr() != last_);
            if (buf_index_ + 1 == buf_last_ && curr() != last_) {
                if constexpr (



                    is_forward<I>

                ) {
                    std::advance(curr(), to_increment_);
                }
                if (curr() == last_)
                    buf_index_ = 0;
                else
                    read();
            } else if (buf_index_ + 1 <= buf_last_) {
                ++buf_index_;
            }
            return *this;
        }


        template<
            typename J = I,
            typename Enable = std::enable_if_t<is_bidirectional<J>>>

        constexpr utf_iterator & operator--()



        {
            BOOST_ASSERT(buf_index_ || curr() != first());
            if (!buf_index_ && curr() != first())
                read_reverse();
            else if (buf_index_)
                --buf_index_;
            return *this;
        }

        friend constexpr bool operator==(



            std::enable_if_t<is_forward<I>, utf_iterator>

            lhs, utf_iterator rhs)



        {
            if constexpr (



                is_forward<I>

            ) {
                return lhs.curr() == rhs.curr() && lhs.buf_index_ == rhs.buf_index_;
            } else {
                if (lhs.curr() != rhs.curr())
                    return false;

                if (lhs.buf_index_ == rhs.buf_index_ &&
                    lhs.buf_last_ == rhs.buf_last_) {
                    return true;
                }

                return lhs.buf_index_ == lhs.buf_last_ &&
                       rhs.buf_index_ == rhs.buf_last_;
            }
        }


        friend constexpr bool operator!=(
            std::enable_if_t<is_forward<I>, utf_iterator> lhs, utf_iterator rhs)
        { return !(lhs == rhs); }


        friend constexpr bool operator==(utf_iterator lhs, S rhs)
        {
            if constexpr (



                is_forward<I>

            ) {
                return lhs.curr() == rhs;
            } else {
                return lhs.curr() == rhs && lhs.buf_index_ == lhs.buf_last_;
            }
        }


        friend constexpr bool operator!=(utf_iterator lhs, S rhs)
        { return !(lhs == rhs); }



        using base_type = stl_interfaces::iterator_interface<
            utf_iterator<FromFormat, ToFormat, I, S, ErrorHandler>,
            detail::bidirectional_at_most_t<I>,
            value_type,
            value_type>;
        using base_type::operator++;
        using base_type::operator--;

    private:
        constexpr char32_t decode_code_point()
        {
            if constexpr (FromFormat == format::utf8) {
                char32_t cp = *curr();
                ++curr();
                to_increment_ = 1;
                if (cp < 0x80)
                    return cp;
#line 2854 "boost/parser/detail/text/transcode_iterator.hpp"
                char8_type curr_c = char8_type(cp);

                auto error = [&]() {
                    return ErrorHandler{}("Ill-formed UTF-8.");
                };
                auto next = [&]() {
                    ++curr();
                    ++to_increment_;
                };




                if (detail::in(0xc2, curr_c, 0xdf)) {
                    cp = curr_c & 0b00011111;
                    if (curr() == last_)
                        return error();
                    curr_c = *curr();
                    if (!detail::in(0x80, curr_c, 0xbf))
                        return error();
                    cp = (cp << 6) + (curr_c & 0b00111111);
                    next();

                } else if (curr_c == 0xe0) {
                    cp = curr_c & 0b00001111;
                    if (curr() == last_)
                        return error();
                    curr_c = *curr();
                    if (!detail::in(0xa0, curr_c, 0xbf))
                        return error();
                    cp = (cp << 6) + (curr_c & 0b00111111);
                    next();
                    if (curr() == last_)
                        return error();
                    curr_c = *curr();
                    if (!detail::in(0x80, curr_c, 0xbf))
                        return error();
                    cp = (cp << 6) + (curr_c & 0b00111111);
                    next();
                } else if (detail::in(0xe1, curr_c, 0xec)) {
                    cp = curr_c & 0b00001111;
                    if (curr() == last_)
                        return error();
                    curr_c = *curr();
                    if (!detail::in(0x80, curr_c, 0xbf))
                        return error();
                    cp = (cp << 6) + (curr_c & 0b00111111);
                    next();
                    if (curr() == last_)
                        return error();
                    curr_c = *curr();
                    if (!detail::in(0x80, curr_c, 0xbf))
                        return error();
                    cp = (cp << 6) + (curr_c & 0b00111111);
                    next();
                } else if (curr_c == 0xed) {
                    cp = curr_c & 0b00001111;
                    if (curr() == last_)
                        return error();
                    curr_c = *curr();
                    if (!detail::in(0x80, curr_c, 0x9f))
                        return error();
                    cp = (cp << 6) + (curr_c & 0b00111111);
                    next();
                    if (curr() == last_)
                        return error();
                    curr_c = *curr();
                    if (!detail::in(0x80, curr_c, 0xbf))
                        return error();
                    cp = (cp << 6) + (curr_c & 0b00111111);
                    next();
                } else if (detail::in(0xee, curr_c, 0xef)) {
                    cp = curr_c & 0b00001111;
                    if (curr() == last_)
                        return error();
                    curr_c = *curr();
                    if (!detail::in(0x80, curr_c, 0xbf))
                        return error();
                    cp = (cp << 6) + (curr_c & 0b00111111);
                    next();
                    if (curr() == last_)
                        return error();
                    curr_c = *curr();
                    if (!detail::in(0x80, curr_c, 0xbf))
                        return error();
                    cp = (cp << 6) + (curr_c & 0b00111111);
                    next();

                } else if (curr_c == 0xf0) {
                    cp = curr_c & 0b00000111;
                    if (curr() == last_)
                        return error();
                    curr_c = *curr();
                    if (!detail::in(0x90, curr_c, 0xbf))
                        return error();
                    cp = (cp << 6) + (curr_c & 0b00111111);
                    next();
                    if (curr() == last_)
                        return error();
                    curr_c = *curr();
                    if (!detail::in(0x80, curr_c, 0xbf))
                        return error();
                    cp = (cp << 6) + (curr_c & 0b00111111);
                    next();
                    if (curr() == last_)
                        return error();
                    curr_c = *curr();
                    if (!detail::in(0x80, curr_c, 0xbf))
                        return error();
                    cp = (cp << 6) + (curr_c & 0b00111111);
                    next();
                } else if (detail::in(0xf1, curr_c, 0xf3)) {
                    cp = curr_c & 0b00000111;
                    if (curr() == last_)
                        return error();
                    curr_c = *curr();
                    if (!detail::in(0x80, curr_c, 0xbf))
                        return error();
                    cp = (cp << 6) + (curr_c & 0b00111111);
                    next();
                    if (curr() == last_)
                        return error();
                    curr_c = *curr();
                    if (!detail::in(0x80, curr_c, 0xbf))
                        return error();
                    cp = (cp << 6) + (curr_c & 0b00111111);
                    next();
                    if (curr() == last_)
                        return error();
                    curr_c = *curr();
                    if (!detail::in(0x80, curr_c, 0xbf))
                        return error();
                    cp = (cp << 6) + (curr_c & 0b00111111);
                    next();
                } else if (curr_c == 0xf4) {
                    cp = curr_c & 0b00000111;
                    if (curr() == last_)
                        return error();
                    curr_c = *curr();
                    if (!detail::in(0x80, curr_c, 0x8f))
                        return error();
                    cp = (cp << 6) + (curr_c & 0b00111111);
                    next();
                    if (curr() == last_)
                        return error();
                    curr_c = *curr();
                    if (!detail::in(0x80, curr_c, 0xbf))
                        return error();
                    cp = (cp << 6) + (curr_c & 0b00111111);
                    next();
                    if (curr() == last_)
                        return error();
                    curr_c = *curr();
                    if (!detail::in(0x80, curr_c, 0xbf))
                        return error();
                    cp = (cp << 6) + (curr_c & 0b00111111);
                    next();
                } else {
                    return error();
                }
                return cp;
            } else if constexpr (FromFormat == format::utf16) {
                char16_t hi = *curr();
                ++curr();
                to_increment_ = 1;
                if (!boost::parser::detail::text::surrogate(hi))
                    return hi;

                if (boost::parser::detail::text::low_surrogate(hi)) {
                    return ErrorHandler{}(
                        "Invalid UTF-16 sequence; lone trailing surrogate.");
                }


                if (curr() == last_) {
                    return ErrorHandler{}(
                        "Invalid UTF-16 sequence; lone leading surrogate.");
                }

                char16_t lo = *curr();
                ++curr();
                ++to_increment_;
                if (!boost::parser::detail::text::low_surrogate(lo)) {
                    return ErrorHandler{}(
                        "Invalid UTF-16 sequence; lone leading surrogate.");
                }

                return char32_t((hi - high_surrogate_base) << 10) +
                       (lo - low_surrogate_base);
            } else {
                char32_t retval = *curr();
                ++curr();
                to_increment_ = 1;
                return retval;
            }
        }


        template<
            typename J = I,
            typename Enable = std::enable_if_t<is_bidirectional<J>>>

        constexpr char32_t decode_code_point_reverse()



        {
            if constexpr (FromFormat == format::utf8) {
                curr() = detail::decrement(first(), curr());
                auto initial = curr();
                char32_t cp = decode_code_point();
                curr() = initial;
                return cp;
            } else if constexpr (FromFormat == format::utf16) {
                char16_t lo = *--curr();
                if (!boost::parser::detail::text::surrogate(lo))
                    return lo;

                if (boost::parser::detail::text::high_surrogate(lo)) {
                    return ErrorHandler{}(
                        "Invalid UTF-16 sequence; lone leading surrogate.");
                }


                if (curr() == first()) {
                    return ErrorHandler{}(
                        "Invalid UTF-16 sequence; lone trailing surrogate.");
                }

                char16_t hi = *detail::prev(curr());
                if (!boost::parser::detail::text::high_surrogate(hi)) {
                    return ErrorHandler{}(
                        "Invalid UTF-16 sequence; lone trailing surrogate.");
                }
                --curr();

                return char32_t((hi - high_surrogate_base) << 10) +
                       (lo - low_surrogate_base);
            } else {
                return *--curr();
            }
        }

        template<class Out>
        static constexpr Out encode_code_point(char32_t cp, Out out)
        {
            if constexpr (ToFormat == format::utf8) {
                if (cp < 0x80) {
                    *out++ = static_cast<char8_type>(cp);
                } else if (cp < 0x800) {
                    *out++ = static_cast<char8_type>(0xC0 + (cp >> 6));
                    *out++ = static_cast<char8_type>(0x80 + (cp & 0x3f));
                } else if (cp < 0x10000) {
                    *out++ = static_cast<char8_type>(0xe0 + (cp >> 12));
                    *out++ = static_cast<char8_type>(0x80 + ((cp >> 6) & 0x3f));
                    *out++ = static_cast<char8_type>(0x80 + (cp & 0x3f));
                } else {
                    *out++ = static_cast<char8_type>(0xf0 + (cp >> 18));
                    *out++ = static_cast<char8_type>(0x80 + ((cp >> 12) & 0x3f));
                    *out++ = static_cast<char8_type>(0x80 + ((cp >> 6) & 0x3f));
                    *out++ = static_cast<char8_type>(0x80 + (cp & 0x3f));
                }
            } else if constexpr (ToFormat == format::utf16) {
                if (cp < 0x10000) {
                    *out++ = static_cast<char16_t>(cp);
                } else {
                    *out++ =
                        static_cast<char16_t>(cp >> 10) + high_surrogate_base;
                    *out++ =
                        static_cast<char16_t>(cp & 0x3ff) + low_surrogate_base;
                }
            } else {
                *out++ = cp;
            }
            return out;
        }

        constexpr void read()
        {
            I initial;
            if constexpr (



                is_forward<I>

            ) {
                initial = curr();
            }
            if constexpr (noexcept(ErrorHandler{}(""))) {
                char32_t cp = decode_code_point();
                auto it = encode_code_point(cp, buf_.begin());
                buf_index_ = 0;
                buf_last_ = uint8_t(it - buf_.begin());
            } else {
                auto buf = buf_;



                    char32_t cp = decode_code_point();
                    auto it = encode_code_point(cp, buf_.begin());
                    buf_index_ = 0;
                    buf_last_ = it - buf_.begin();
#line 3164 "boost/parser/detail/text/transcode_iterator.hpp"
            }
            if constexpr (



            is_forward<I>

            ) {
                curr() = initial;
            }
        }

        constexpr void read_reverse()
        {
            auto initial = curr();
            if constexpr (noexcept(ErrorHandler{}(""))) {
                char32_t cp = decode_code_point_reverse();
                auto it = encode_code_point(cp, buf_.begin());
                buf_last_ = uint8_t(it - buf_.begin());
                buf_index_ = buf_last_ - 1;
                to_increment_ = std::distance(curr(), initial);
            } else {
                auto buf = buf_;



                    char32_t cp = decode_code_point_reverse();
                    auto it = encode_code_point(cp, buf_.begin());
                    buf_last_ = it - buf_.begin();
                    buf_index_ = buf_last_ - 1;
                    to_increment_ = std::distance(curr(), initial);
#line 3202 "boost/parser/detail/text/transcode_iterator.hpp"
            }
        }


        template<
            typename J = I,
            typename Enable = std::enable_if_t<is_bidirectional<J>>>

        constexpr I first() const



        {
            return first_and_curr_.first;
        }
        constexpr I & curr() { return first_and_curr_.curr; }
        constexpr I curr() const { return first_and_curr_.curr; }

        std::array<value_type, 4 / static_cast<int>(ToFormat)> buf_;

        detail::first_and_curr<I> first_and_curr_;

        uint8_t buf_index_ = 0;
        uint8_t buf_last_ = 0;
        uint8_t to_increment_ = 0;

        [[no_unique_address]] S last_;
#line 3239 "boost/parser/detail/text/transcode_iterator.hpp"
        template<
            format FromFormat2,
            format ToFormat2,
            typename I2,
            typename S2,
            typename ErrorHandler2>

        friend class utf_iterator;
    };

}}

namespace boost::parser::detail { namespace text { namespace detail {

    template<class T>
    constexpr bool is_utf_iter = false;
    template<
        format FromFormat,
        format ToFormat,
        class I,
        class S,
        class ErrorHandler>
    constexpr bool
        is_utf_iter<utf_iterator<FromFormat, ToFormat, I, S, ErrorHandler>> =
            true;
#line 3284 "boost/parser/detail/text/transcode_iterator.hpp"
    template<typename T>
    using range_expr =
        decltype(detail::begin(std::declval<T &>()) == detail::end(std::declval<T &>()));
    template<typename T>
    constexpr bool is_range_v = is_detected_v<range_expr, T>;

    template<typename V>
    constexpr bool common_range_v =
        is_range_v<V> && std::is_same_v<iterator_t<V>, sentinel_t<V>>;
    template<typename V>
    constexpr bool input_range_v = is_range_v<V> && std::is_base_of_v<
        std::input_iterator_tag,
        typename std::iterator_traits<iterator_t<V>>::iterator_category>;
    template<typename V>
    constexpr bool forward_range_v = is_range_v<V> && std::is_base_of_v<
        std::forward_iterator_tag,
        typename std::iterator_traits<iterator_t<V>>::iterator_category>;
    template<typename V>
    constexpr bool bidirectional_range_v = is_range_v<V> && std::is_base_of_v<
        std::bidirectional_iterator_tag,
        typename std::iterator_traits<iterator_t<V>>::iterator_category>;
    template<typename T>
    constexpr bool default_initializable_v = std::is_default_constructible_v<T>;

    template<typename V>
    constexpr bool utf_range_v = is_range_v<V> && code_unit_v<range_value_t<V>>;

    template<typename V>
    constexpr bool
        utf32_range_v = is_range_v<V> &&
                        (

                            std::is_same_v<range_value_t<V>, wchar_t> ||

                            std::is_same_v<range_value_t<V>, char32_t>);



    template<typename I>
    constexpr bool random_access_iterator_v = std::is_base_of_v<
        std::random_access_iterator_tag,
        typename std::iterator_traits<I>::iterator_category>;
    template<typename I>
    constexpr bool bidirectional_iterator_v = std::is_base_of_v<
        std::bidirectional_iterator_tag,
        typename std::iterator_traits<I>::iterator_category>;
    template<typename I>
    constexpr bool forward_iterator_v = std::is_base_of_v<
        std::forward_iterator_tag,
        typename std::iterator_traits<I>::iterator_category>;

    template<
        class V,
        bool StoreFirst = !is_utf_iter<iterator_t<V>> && common_range_v<V> &&
                          bidirectional_range_v<V>,
        bool StoreLast = !is_utf_iter<iterator_t<V>>>
    struct first_last_storage
    {

        template<
            typename Enable = std::enable_if_t<
                default_initializable_v<iterator_t<V>> &&
                default_initializable_v<sentinel_t<V>>>>

        constexpr first_last_storage()




        {}
        constexpr first_last_storage(V & base) :
            first_{detail::begin(base)}, last_{detail::end(base)}
        {}

        constexpr auto begin(iterator_t<V> & it) const { return first_; }
        constexpr auto end(iterator_t<V> & it) const { return last_; }

        iterator_t<V> first_;
        sentinel_t<V> last_;
    };

    template<typename I>
    using trinary_iter_ctor = decltype(I(
        std::declval<I>().begin(),
        std::declval<I>().end(),
        std::declval<I>().end()));

    template<class V>
    struct first_last_storage<V, true, false>
    {

        template<
            typename Enable =
                std::enable_if_t<default_initializable_v<iterator_t<V>>>>

        constexpr first_last_storage()



        {}
        constexpr first_last_storage(V & base) : first_{detail::begin(base)} {}

        constexpr auto begin(iterator_t<V> & it) const { return first_; }
        constexpr auto end(iterator_t<V> & it) const {
            if constexpr (



                is_detected_v<trinary_iter_ctor, iterator_t<V>>

            ) {
                return iterator_t<V>(it.begin(), it.end(), it.end());
            } else {
                return it.end();
            }
        }

        iterator_t<V> first_;
    };

    template<class V>
    struct first_last_storage<V, false, true>
    {

        template<
            typename Enable =
                std::enable_if_t<default_initializable_v<sentinel_t<V>>>>

        constexpr first_last_storage()



        {}
        constexpr first_last_storage(V & base) : last_{detail::end(base)} {}

        constexpr auto begin(iterator_t<V> & it) const {
            if constexpr (is_utf_iter<iterator_t<V>>) {
                return iterator_t<V>(it.begin(), it.begin(), it.end());
            } else {
                return;
            }
        }
        constexpr auto end(iterator_t<V> & it) const { return last_; }

        sentinel_t<V> last_;
    };

    template<class V>
    struct first_last_storage<V, false, false>
    {
        constexpr first_last_storage() = default;
        constexpr first_last_storage(V & base) {}

        constexpr auto begin(iterator_t<V> & it) const {
            if constexpr (is_utf_iter<iterator_t<V>>) {
                return iterator_t<V>(it.begin(), it.begin(), it.end());
            } else {
                return;
            }
        }
        constexpr auto end(iterator_t<V> & it) const {
            if constexpr (



                is_detected_v<trinary_iter_ctor, iterator_t<V>>

            ) {
                return iterator_t<V>(it.begin(), it.end(), it.end());
            } else {
                return it.end();
            }
        }
    };


    template<class V>
    constexpr auto uc_view_category() {
        if constexpr (common_range_v<V> && bidirectional_range_v<V>) {
            return std::bidirectional_iterator_tag{};
        } else {
            return std::forward_iterator_tag{};
        }
    }

    template<class V>
    using uc_view_category_t = decltype(uc_view_category<V>());

    template<bool Const, class T>
    using maybe_const = std::conditional_t<Const, const T, T>;

    template<class T>
    constexpr bool is_empty_view = false;





}}}
#line 20 "boost/parser/detail/text/transcode_algorithm.hpp"
namespace boost::parser::detail { namespace text {

    template<typename Range>
    struct utf_range_like_iterator
    {
        using type = decltype(std::declval<Range>().begin());
    };

    template<typename T>
    struct utf_range_like_iterator<T *>
    {
        using type = T *;
    };

    template<std::size_t N, typename T>
    struct utf_range_like_iterator<T[N]>
    {
        using type = T *;
    };

    template<std::size_t N, typename T>
    struct utf_range_like_iterator<T (&)[N]>
    {
        using type = T *;
    };

    template<typename Range>
    using utf_range_like_iterator_t =
        typename utf_range_like_iterator<Range>::type;



    template<typename Iter, typename OutIter>
    using transcode_result = in_out_result<Iter, OutIter>;

    namespace detail {
        template<typename OutIter>
        constexpr OutIter read_into_utf8_iter(uint32_t cp, OutIter out)
        {
            if (cp < 0x80) {
                *out = static_cast<char>(cp);
                ++out;
            } else if (cp < 0x800) {
                *out = static_cast<char>(0xC0 + (cp >> 6));
                ++out;
                *out = static_cast<char>(0x80 + (cp & 0x3f));
                ++out;
            } else if (cp < 0x10000) {
                *out = static_cast<char>(0xe0 + (cp >> 12));
                ++out;
                *out = static_cast<char>(0x80 + ((cp >> 6) & 0x3f));
                ++out;
                *out = static_cast<char>(0x80 + (cp & 0x3f));
                ++out;
            } else {
                *out = static_cast<char>(0xf0 + (cp >> 18));
                ++out;
                *out = static_cast<char>(0x80 + ((cp >> 12) & 0x3f));
                ++out;
                *out = static_cast<char>(0x80 + ((cp >> 6) & 0x3f));
                ++out;
                *out = static_cast<char>(0x80 + (cp & 0x3f));
                ++out;
            }
            return out;
        }

        template<typename OutIter>
        constexpr OutIter read_into_utf16_iter(uint32_t cp, OutIter out)
        {
            uint16_t const high_surrogate_base = 0xd7c0;
            uint16_t const low_surrogate_base = 0xdc00;

            if (cp < 0x10000) {
                *out = static_cast<uint16_t>(cp);
                ++out;
            } else {
                *out = static_cast<uint16_t>(cp >> 10) + high_surrogate_base;
                ++out;
                *out = static_cast<uint16_t>(cp & 0x3ff) + low_surrogate_base;
                ++out;
            }
            return out;
        }

        template<
            bool UseN,
            typename InputIter,
            typename Sentinel,
            typename OutIter>
        transcode_result<InputIter, OutIter> transcode_utf_8_to_16(
            InputIter first,
            Sentinel last,
            std::ptrdiff_t n,
            OutIter out,
            std::input_iterator_tag)
        {
            for (; first != last && (!UseN || n); --n) {
                unsigned char const c = *first;
                if (c < 0x80) {
                    *out = *first;
                    ++first;
                    ++out;
                } else {
                    auto const cp = detail::advance(first, last);
                    out = detail::read_into_utf16_iter(cp, out);
                }
            }
            return {first, out};
        }

        template<bool UseN, typename Iter, typename OutIter>
        transcode_result<Iter, OutIter> transcode_utf_8_to_16(
            Iter first,
            Iter last,
            std::ptrdiff_t n,
            OutIter out,
            std::random_access_iterator_tag)
        {
            return transcode_utf_8_to_16<UseN>(
                first, last, n, out, std::input_iterator_tag{});
        }

        template<
            bool UseN,
            typename InputIter,
            typename Sentinel,
            typename OutIter>
        transcode_result<InputIter, OutIter> transcode_utf_8_to_32(
            InputIter first,
            Sentinel last,
            std::ptrdiff_t n,
            OutIter out,
            std::input_iterator_tag)
        {
            for (; first != last && (!UseN || n); --n) {
                unsigned char const c = *first;
                if (c < 0x80) {
                    *out = *first;
                    ++first;
                    ++out;
                } else {
                    *out = detail::advance(first, last);
                    ++out;
                }
            }
            return {first, out};
        }

        template<bool UseN, typename Iter, typename OutIter>
        transcode_result<Iter, OutIter> transcode_utf_8_to_32(
            Iter first,
            Iter last,
            std::ptrdiff_t n,
            OutIter out,
            std::random_access_iterator_tag)
        {
            return transcode_utf_8_to_32<UseN>(
                first, last, n, out, std::input_iterator_tag{});
        }

        template<format Tag>
        struct tag_t
        {};

        template<bool UseN, typename Iter, typename Sentinel, typename OutIter>
        transcode_result<Iter, OutIter> transcode_to_8(
            tag_t<format::utf8>,
            Iter first,
            Sentinel last,
            std::ptrdiff_t n,
            OutIter out)
        {
            for (; first != last && (!UseN || n); ++first, ++out) {
                *out = *first;
                --n;
            }
            return {first, out};
        }

        template<bool UseN, typename Iter, typename Sentinel, typename OutIter>
        transcode_result<Iter, OutIter> transcode_to_16(
            tag_t<format::utf8>,
            Iter first,
            Sentinel last,
            std::ptrdiff_t n,
            OutIter out)
        {
            return detail::transcode_utf_8_to_16<UseN>(
                first,
                last,
                n,
                out,
                typename std::iterator_traits<Iter>::iterator_category{});
        }

        template<bool UseN, typename Iter, typename Sentinel, typename OutIter>
        transcode_result<Iter, OutIter> transcode_to_32(
            tag_t<format::utf8>,
            Iter first,
            Sentinel last,
            std::ptrdiff_t n,
            OutIter out)
        {
            return detail::transcode_utf_8_to_32<UseN>(
                first,
                last,
                n,
                out,
                typename std::iterator_traits<Iter>::iterator_category{});
        }

        template<bool UseN, typename Iter, typename Sentinel, typename OutIter>
        transcode_result<Iter, OutIter> transcode_to_8(
            tag_t<format::utf16>,
            Iter first,
            Sentinel last,
            std::ptrdiff_t n,
            OutIter out)
        {
            uint32_t const high_surrogate_max = 0xdbff;
            uint16_t const high_surrogate_base = 0xd7c0;
            uint16_t const low_surrogate_base = 0xdc00;

            for (; first != last && (!UseN || n); ++first, --n) {
                uint32_t const hi = *first;
                if (surrogate(hi)) {
                    if (hi <= high_surrogate_max) {
                        ++first;
                        if (first == last) {
                            uint32_t const cp = replacement_character;
                            out = detail::read_into_utf8_iter(cp, out);
                            ++out;
                            return {first, out};
                        }
                        uint32_t const lo = *first;
                        if (low_surrogate(lo)) {
                            uint32_t const cp =
                                ((hi - high_surrogate_base) << 10) +
                                (lo - low_surrogate_base);
                            out = detail::read_into_utf8_iter(cp, out);
                            continue;
                        }
                    }
                    out = detail::read_into_utf8_iter(
                        replacement_character, out);
                } else {
                    out = detail::read_into_utf8_iter(hi, out);
                }
            }

            return {first, out};
        }

        template<bool UseN, typename Iter, typename Sentinel, typename OutIter>
        transcode_result<Iter, OutIter> transcode_to_16(
            tag_t<format::utf16>,
            Iter first,
            Sentinel last,
            std::ptrdiff_t n,
            OutIter out)
        {
            for (; first != last && (!UseN || n); ++first, ++out, --n) {
                *out = *first;
            }
            return {first, out};
        }

        template<bool UseN, typename Iter, typename Sentinel, typename OutIter>
        transcode_result<Iter, OutIter> transcode_to_32(
            tag_t<format::utf16>,
            Iter first,
            Sentinel last,
            std::ptrdiff_t n,
            OutIter out)
        {
            uint32_t const high_surrogate_max = 0xdbff;
            uint16_t const high_surrogate_base = 0xd7c0;
            uint16_t const low_surrogate_base = 0xdc00;

            for (; first != last && (!UseN || n); ++first, --n) {
                uint32_t const hi = *first;
                if (surrogate(hi)) {
                    if (hi <= high_surrogate_max) {
                        ++first;
                        if (first == last) {
                            *out = replacement_character;
                            ++out;
                            return {first, out};
                        }
                        uint32_t const lo = *first;
                        if (low_surrogate(lo)) {
                            uint32_t const cp =
                                ((hi - high_surrogate_base) << 10) +
                                (lo - low_surrogate_base);
                            *out = cp;
                            ++out;
                            continue;
                        }
                    }
                    *out = replacement_character;
                    ++out;
                } else {
                    *out = hi;
                    ++out;
                }
            }

            return {first, out};
        }

        template<bool UseN, typename Iter, typename Sentinel, typename OutIter>
        transcode_result<Iter, OutIter> transcode_to_8(
            tag_t<format::utf32>,
            Iter first,
            Sentinel last,
            std::ptrdiff_t n,
            OutIter out)
        {
            for (; first != last && (!UseN || n); ++first, --n) {
                out = detail::read_into_utf8_iter(*first, out);
            }
            return {first, out};
        }

        template<bool UseN, typename Iter, typename Sentinel, typename OutIter>
        transcode_result<Iter, OutIter> transcode_to_16(
            tag_t<format::utf32>,
            Iter first,
            Sentinel last,
            std::ptrdiff_t n,
            OutIter out)
        {
            for (; first != last && (!UseN || n); ++first, --n) {
                out = detail::read_into_utf16_iter(*first, out);
            }
            return {first, out};
        }

        template<bool UseN, typename Iter, typename Sentinel, typename OutIter>
        transcode_result<Iter, OutIter> transcode_to_32(
            tag_t<format::utf32>,
            Iter first,
            Sentinel last,
            std::ptrdiff_t n,
            OutIter out)
        {
            for (; first != last && (!UseN || n); ++first, ++out, --n) {
                *out = *first;
            }
            return {first, out};
        }
    }
#line 409 "boost/parser/detail/text/transcode_algorithm.hpp"
}}

namespace boost::parser::detail { namespace text { inline namespace v1 {
#line 516 "boost/parser/detail/text/transcode_algorithm.hpp"
    namespace dtl {
        template<
            bool UseN,
            typename Range,
            typename OutIter,
            bool _16Ptr = detail::is_16_ptr_v<Range>,
            bool CPPtr = detail::is_cp_ptr_v<Range>>
        struct transcode_to_8_dispatch
        {
            static constexpr auto
            call(Range && r, std::ptrdiff_t n, OutIter out)
                -> transcode_result<decltype(detail::begin(r)), OutIter>
            {
                auto const u = text::unpack_iterator_and_sentinel(
                    detail::begin(r), detail::end(r));
                auto unpacked = detail::transcode_to_8<UseN>(
                    detail::tag_t<u.format_tag>{}, u.first, u.last, n, out);
                return {u.repack(unpacked.in), unpacked.out};
            }
        };

        template<bool UseN, typename Ptr, typename OutIter>
        struct transcode_to_8_dispatch<UseN, Ptr, OutIter, true, false>
        {
            static constexpr auto
            call(Ptr p, std::ptrdiff_t n, OutIter out)
            {
                return detail::transcode_to_8<UseN>(
                    detail::tag_t<format::utf16>{}, p, null_sentinel, n, out);
            }
        };

        template<bool UseN, typename Ptr, typename OutIter>
        struct transcode_to_8_dispatch<UseN, Ptr, OutIter, false, true>
        {
            static constexpr auto
            call(Ptr p, std::ptrdiff_t n, OutIter out)
            {
                return detail::transcode_to_8<UseN>(
                    detail::tag_t<format::utf32>{}, p, null_sentinel, n, out);
            }
        };

        template<
            bool UseN,
            typename Range,
            typename OutIter,
            bool CharPtr = detail::is_char_ptr_v<Range>,
            bool CPPtr = detail::is_cp_ptr_v<Range>>
        struct transcode_to_16_dispatch
        {
            static constexpr auto
            call(Range && r, std::ptrdiff_t n, OutIter out)
                -> transcode_result<decltype(detail::begin(r)), OutIter>
            {
                auto const u = text::unpack_iterator_and_sentinel(
                    detail::begin(r), detail::end(r));
                auto unpacked = detail::transcode_to_16<UseN>(
                    detail::tag_t<u.format_tag>{}, u.first, u.last, n, out);
                return {u.repack(unpacked.in), unpacked.out};
            }
        };

        template<bool UseN, typename Ptr, typename OutIter>
        struct transcode_to_16_dispatch<UseN, Ptr, OutIter, true, false>
        {
            static constexpr auto
            call(Ptr p, std::ptrdiff_t n, OutIter out)
            {
                return detail::transcode_to_16<UseN>(
                    detail::tag_t<format::utf8>{}, p, null_sentinel, n, out);
            }
        };

        template<bool UseN, typename Ptr, typename OutIter>
        struct transcode_to_16_dispatch<UseN, Ptr, OutIter, false, true>
        {
            static constexpr auto
            call(Ptr p, std::ptrdiff_t n, OutIter out)
            {
                return detail::transcode_to_16<UseN>(
                    detail::tag_t<format::utf32>{}, p, null_sentinel, n, out);
            }
        };

        template<
            bool UseN,
            typename Range,
            typename OutIter,
            bool CharPtr = detail::is_char_ptr_v<Range>,
            bool _16Ptr = detail::is_16_ptr_v<Range>>
        struct transcode_to_32_dispatch
        {
            static constexpr auto
            call(Range && r, std::ptrdiff_t n, OutIter out)
                -> transcode_result<decltype(detail::begin(r)), OutIter>
            {
                auto const u = text::unpack_iterator_and_sentinel(
                    detail::begin(r), detail::end(r));
                auto unpacked = detail::transcode_to_32<UseN>(
                    detail::tag_t<u.format_tag>{}, u.first, u.last, n, out);
                return {u.repack(unpacked.in), unpacked.out};
            }
        };

        template<bool UseN, typename Ptr, typename OutIter>
        struct transcode_to_32_dispatch<UseN, Ptr, OutIter, true, false>
        {
            static constexpr auto
            call(Ptr p, std::ptrdiff_t n, OutIter out)
            {
                return detail::transcode_to_32<UseN>(
                    detail::tag_t<format::utf8>{}, p, null_sentinel, n, out);
            }
        };

        template<bool UseN, typename Ptr, typename OutIter>
        struct transcode_to_32_dispatch<UseN, Ptr, OutIter, false, true>
        {
            static constexpr auto
            call(Ptr p, std::ptrdiff_t n, OutIter out)
            {
                return detail::transcode_to_32<UseN>(
                    detail::tag_t<format::utf16>{}, p, null_sentinel, n, out);
            }
        };
    }

    template<typename Iter, typename Sentinel, typename OutIter>
    transcode_result<Iter, OutIter> transcode_to_utf8(
        Iter first, Sentinel last, OutIter out)
    {
        auto const r = text::unpack_iterator_and_sentinel(first, last);
        auto unpacked = detail::transcode_to_8<false>(
            detail::tag_t<r.format_tag>{}, r.first, r.last, -1, out);
        return {r.repack(unpacked.in), unpacked.out};
    }

    template<typename Range, typename OutIter>
    transcode_result<utf_range_like_iterator_t<Range>, OutIter>
    transcode_to_utf8(Range && r, OutIter out)
    {
        return dtl::transcode_to_8_dispatch<false, Range, OutIter>::call(
            r, -1, out);
    }

    template<typename Iter, typename Sentinel, typename OutIter>
    transcode_result<Iter, OutIter> transcode_to_utf16(
        Iter first, Sentinel last, OutIter out)
    {
        auto const r = text::unpack_iterator_and_sentinel(first, last);
        auto unpacked = detail::transcode_to_16<false>(
            detail::tag_t<r.format_tag>{}, r.first, r.last, -1, out);
        return {r.repack(unpacked.in), unpacked.out};
    }

    template<typename Range, typename OutIter>
    transcode_result<utf_range_like_iterator_t<Range>, OutIter>
    transcode_to_utf16(Range && r, OutIter out)
    {
        return dtl::transcode_to_16_dispatch<false, Range, OutIter>::call(
            r, -1, out);
    }

    template<typename Iter, typename Sentinel, typename OutIter>
    transcode_result<Iter, OutIter> transcode_to_utf32(
        Iter first, Sentinel last, OutIter out)
    {
        auto const r = text::unpack_iterator_and_sentinel(first, last);
        auto unpacked = detail::transcode_to_32<false>(
            detail::tag_t<r.format_tag>{}, r.first, r.last, -1, out);
        return {r.repack(unpacked.in), unpacked.out};
    }

    template<typename Range, typename OutIter>
    transcode_result<utf_range_like_iterator_t<Range>, OutIter>
    transcode_to_utf32(Range && r, OutIter out)
    {
        return dtl::transcode_to_32_dispatch<false, Range, OutIter>::call(
            r, -1, out);
    }

}}}
#line 7 "boost/parser/detail/text/detail/all_t.hpp"
#define BOOST_PARSER_DETAIL_TEXT_DETAIL_ALL_T_HPP





#include <array>





namespace boost::parser::detail::text::detail {

    template<typename T>
    using iterator_ = decltype(text::detail::begin(std::declval<T &>()));
    template<typename T>
    using sentinel_ = decltype(text::detail::end(std::declval<T &>()));

    template<typename T>
    constexpr bool range_ =
        is_detected_v<iterator_, T> && is_detected_v<sentinel_, T>;

    template<typename T>
    using has_insert_ = decltype(std::declval<T &>().insert(
        std::declval<T>().begin(), *std::declval<T>().begin()));

    template<typename T>
    constexpr bool container_ = is_detected_v<has_insert_, T>;

    template<typename T>
    constexpr bool is_std_array_v = false;
    template<typename T, size_t N>
    constexpr bool is_std_array_v<std::array<T, N>> = false;

    template<typename R>
    constexpr bool view =




        range_<R> && !container_<R> &&
        !std::is_array_v<std::remove_reference_t<R>> &&
        !is_std_array_v<std::remove_reference_t<R>>

        ;

    template<
        typename R,
        typename Enable = std::enable_if_t<range_<R> && std::is_object_v<R>>>
    struct ref_view : stl_interfaces::view_interface<ref_view<R>>
    {
    private:
        static void rvalue_poison(R &);
        static void rvalue_poison(R &&) = delete;

    public:
        template<
            typename T,
            typename Enable2 = std::enable_if_t<
                !std::
                    is_same_v<remove_cv_ref_t<T>, remove_cv_ref_t<ref_view>> &&
                std::is_convertible_v<T, R &>>,
            typename Enable3 = decltype(rvalue_poison(std::declval<T>()))>
        constexpr ref_view(T && t) :
            r_(std::addressof(static_cast<R &>((T &&) t)))
        {}
        constexpr R & base() const { return *r_; }
        constexpr iterator_<R> begin() const
        {
            return text::detail::begin(*r_);
        }
        constexpr sentinel_<R> end() const { return text::detail::end(*r_); }

    private:
        R * r_;
    };

    template<typename R>
    ref_view(R &) -> ref_view<R>;

    template<typename R>
    struct owning_view : stl_interfaces::view_interface<owning_view<R>>
    {
        owning_view() = default;
        constexpr owning_view(R && t) : r_(std::move(t)) {}

        owning_view(owning_view &&) = default;
        owning_view & operator=(owning_view &&) = default;

        constexpr R & base() & noexcept { return r_; }
        constexpr const R & base() const & noexcept { return r_; }
        constexpr R && base() && noexcept { return std::move(r_); }
        constexpr const R && base() const && noexcept { return std::move(r_); }

        constexpr iterator_<R> begin() { return text::detail::begin(r_); }
        constexpr sentinel_<R> end() { return text::detail::end(r_); }

        constexpr auto begin() const { return text::detail::begin(r_); }
        constexpr auto end() const { return text::detail::end(r_); }

    private:
        R r_ = R();
    };

    template<typename T>
    using can_ref_view_expr = decltype(ref_view(std::declval<T>()));
    template<typename T>
    constexpr bool can_ref_view = is_detected_v<can_ref_view_expr, T>;

    struct all_impl
    {
        template<typename R, typename Enable = std::enable_if_t<range_<R>>>
        [[nodiscard]] constexpr auto operator()(R && r) const
        {
            using T = remove_cv_ref_t<R>;
            if constexpr (view<T>)
                return (R &&) r;
            else if constexpr (can_ref_view<R>)
                return ref_view(r);
            else
                return owning_view<T>(std::move(r));
        }
    };

    constexpr all_impl all;





    template<typename R>
    using all_t = decltype(all(std::declval<R>()));


}
#line 7 "boost/parser/detail/stl_interfaces/view_adaptor.hpp"
#define BOOST_PARSER_DETAIL_STL_INTERFACES_VIEW_ADAPTOR_HPP
#line 7 "boost/parser/detail/stl_interfaces/detail/view_closure.hpp"
#define BOOST_PARSER_DETAIL_STL_INTERFACES_DETAIL_VIEW_CLOSURE_HPP
#line 7 "boost/parser/detail/stl_interfaces/detail/pipeable_view.hpp"
#define BOOST_PARSER_DETAIL_STL_INTERFACES_DETAIL_PIPEABLE_VIEW_HPP



#include <type_traits>






namespace boost::parser::detail { namespace stl_interfaces { namespace detail {

    template<typename T>
    using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

    struct pipeable_base;






    template<typename T>
    constexpr bool pipeable_ = std::is_base_of<pipeable_base, T>::value &&
        std::is_object<T>::value && std::is_copy_constructible<T>::value;





    template<
        typename T,
        typename U,
        typename Enable = std::enable_if_t<pipeable_<T> && pipeable_<U>>>

    struct view_pipeline;

    struct pipeable_base
    {





        template<
            typename T,
            typename U,
            typename Enable = std::enable_if_t<
                pipeable_<T> && pipeable_<U> &&
                std::is_constructible<remove_cvref_t<T>, T>::value &&
                std::is_constructible<remove_cvref_t<U>, U>::value>>

        friend constexpr auto operator|(T && t, U && u)
        {
            return view_pipeline<T, U>{(T &&) t, (U &&) u};
        }
    };

    template<typename Derived>
    struct pipeable : pipeable_base
    {
        template<typename R>
        friend constexpr auto operator|(R && r, Derived & d)
            -> decltype(((Derived &&) d)((R &&) r))
        {
            return ((Derived &&) d)((R &&) r);
        }

        template<typename R>
        friend constexpr auto operator|(R && r, Derived const & d)
            -> decltype(((Derived &&) d)((R &&) r))
        {
            return ((Derived &&) d)((R &&) r);
        }

        template<typename R>
        friend constexpr auto operator|(R && r, Derived && d)
            -> decltype(((Derived &&) d)((R &&) r))
        {
            return ((Derived &&) d)((R &&) r);
        }
    };




    template<typename T, typename U, typename>

    struct view_pipeline : pipeable<view_pipeline<T, U>>
    {
        view_pipeline() = default;

        constexpr view_pipeline(T && t, U && u) :
            left_(std::move(t)), right_(std::move(u))
        {}
#line 110 "boost/parser/detail/stl_interfaces/detail/pipeable_view.hpp"
        template<typename R>
        constexpr auto
        operator()(R && r) & -> decltype(this->right_(this->left_((R &&) r)))

        {
            return right_(left_((R &&) r));
        }
#line 124 "boost/parser/detail/stl_interfaces/detail/pipeable_view.hpp"
        template<typename R>
        constexpr auto operator()(
            R && r) const & -> decltype(this->right_(this->left_((R &&) r)))

        {
            return right_(left_((R &&) r));
        }
#line 138 "boost/parser/detail/stl_interfaces/detail/pipeable_view.hpp"
        template<typename R>
        constexpr auto operator()(R && r) && -> decltype(std::move(
            this->right_)(std::move(this->left_)((R &&) r)))

        {
            return std::move(right_)(std::move(left_)((R &&) r));
        }

        T left_;
        U right_;
    };

}}}
#line 11 "boost/parser/detail/stl_interfaces/detail/view_closure.hpp"
#include <utility>


namespace boost::parser::detail { namespace stl_interfaces { namespace detail {

    template<std::size_t I, typename T>
    struct box
    {
        T value_;
    };

    template<typename Indices, typename Func, typename... T>
    struct view_closure_impl;

    template<std::size_t... I, typename Func, typename... T>
    struct view_closure_impl<std::index_sequence<I...>, Func, T...>
        : box<I, T>...
    {
        view_closure_impl() = default;
        constexpr explicit view_closure_impl(Func, T &&... x) :
            box<I, T>{std::move(x)}...
        {}
#line 41 "boost/parser/detail/stl_interfaces/detail/view_closure.hpp"
        template<typename R>
        constexpr auto operator()(R && r) & -> decltype(
            Func{}((R &&) r, std::declval<box<I, T> &>().value_...))

        {
            return Func{}((R &&) r, static_cast<box<I, T> &>(*this).value_...);
        }
#line 56 "boost/parser/detail/stl_interfaces/detail/view_closure.hpp"
        template<typename R>
        constexpr auto operator()(R && r) const & -> decltype(
            Func{}((R &&) r, std::declval<box<I, T> const &>().value_...))

        {
            return Func{}(
                (R &&) r, static_cast<box<I, T> const &>(*this).value_...);
        }
#line 72 "boost/parser/detail/stl_interfaces/detail/view_closure.hpp"
        template<typename R>
        constexpr auto operator()(R && r) && -> decltype(
            Func{}((R &&) r, std::declval<box<I, T> &&>().value_...))

        {
            return Func{}((R &&) r, static_cast<box<I, T> &&>(*this).value_...);
        }
    };




    template<typename Func, typename... T>

    struct view_closure
        : pipeable<view_closure<Func, T...>>,
          view_closure_impl<std::index_sequence_for<T...>, Func, T...>
    {
        using base_type =
            view_closure_impl<std::index_sequence_for<T...>, Func, T...>;

        view_closure() = default;

        constexpr explicit view_closure(Func func, T &&... x) :
            base_type{func, std::move(x)...}
        {}
    };






}}}
#line 14 "boost/parser/detail/stl_interfaces/view_adaptor.hpp"
#include <tuple>
#include <type_traits>
#line 23 "boost/parser/detail/stl_interfaces/view_adaptor.hpp"
#define BOOST_PARSER_USE_CPP23_STD_RANGE_ADAPTOR_CLOSURE 0






#define BOOST_PARSER_USE_LIBSTDCPP_GCC12_RANGE_ADAPTOR_CLOSURE 0






#define BOOST_PARSER_NEED_VS_COMPATIBLE_RANGE_ADAPTOR_CLOSURE 0





#define BOOST_PARSER_DEFINE_CUSTOM_RANGE_ADAPTOR_CLOSURE 1
#line 51 "boost/parser/detail/stl_interfaces/view_adaptor.hpp"
namespace boost::parser::detail { namespace stl_interfaces {
    namespace detail {
        template<typename F, typename... Args>
        using invocable_expr =
            decltype(std::declval<F>()(std::declval<Args>()...));
        template<typename F, typename... Args>
        constexpr bool is_invocable_v =
            is_detected_v<invocable_expr, F, Args...>;

        template<typename Func, typename... CapturedArgs>
        struct bind_back_t
        {
            static_assert(std::is_move_constructible<Func>::value, "");





            template<typename F, typename... Args>
            explicit constexpr bind_back_t(int, F && f, Args &&... args) :
                f_((F &&) f), bound_args_((Args &&) args...)
            {
                static_assert(sizeof...(Args) == sizeof...(CapturedArgs), "");
            }

            template<typename... Args>
            constexpr decltype(auto) operator()(Args &&... args) &
            {
                return call_impl(*this, indices(), (Args &&) args...);
            }

            template<typename... Args>
            constexpr decltype(auto) operator()(Args &&... args) const &
            {
                return call_impl(*this, indices(), (Args &&) args...);
            }

            template<typename... Args>
            constexpr decltype(auto) operator()(Args &&... args) &&
            {
                return call_impl(
                    std::move(*this), indices(), (Args &&) args...);
            }

            template<typename... Args>
            constexpr decltype(auto) operator()(Args &&... args) const &&
            {
                return call_impl(
                    std::move(*this), indices(), (Args &&) args...);
            }

        private:
            using indices = std::index_sequence_for<CapturedArgs...>;

            template<typename T, size_t... I, typename... Args>
            static constexpr decltype(auto)
            call_impl(T && this_, std::index_sequence<I...>, Args &&... args)
            {
                return ((T &&) this_)
                    .f_((Args &&) args...,
                        std::get<I>(((T &&) this_).bound_args_)...);
            }

            Func f_;
            std::tuple<CapturedArgs...> bound_args_;
        };

        template<typename Func, typename... Args>
        using bind_back_result =
            bind_back_t<std::decay_t<Func>, std::decay_t<Args>...>;
    }


    template<typename Func, typename... Args>
    constexpr auto bind_back(Func && f, Args &&... args)
    {
        return detail::bind_back_result<Func, Args...>(
            0, (Func &&) f, (Args &&) args...);
    }
#line 142 "boost/parser/detail/stl_interfaces/view_adaptor.hpp"
    template<
        typename D,
        typename Enable = std::enable_if_t<
            std::is_class<D>::value &&
            std::is_same<D, std::remove_cv_t<D>>::value>>

    struct range_adaptor_closure;

    namespace detail {






        template<typename T>
        using range_adaptor_closure_tag_expr = typename range_adaptor_closure<
            T>::inheritance_tag_with_an_unlikely_name_;
        template<typename T>
        constexpr bool range_adaptor_closure_ =
            is_detected_v<range_adaptor_closure_tag_expr, remove_cvref_t<T>>;

    }
#line 189 "boost/parser/detail/stl_interfaces/view_adaptor.hpp"
    template<typename D, typename>

    struct range_adaptor_closure
    {




        template<
            typename T,
            typename Enable = std::enable_if_t<detail::is_invocable_v<D, T>>>

        [[nodiscard]] friend constexpr decltype(auto) operator|(T && t, D && d)
        {
            return std::move(d)((T &&) t);
        }





        template<
            typename T,
            typename Enable =
                std::enable_if_t<detail::is_invocable_v<D const &, T>>>

        [[nodiscard]] friend constexpr decltype(auto)
        operator|(T && t, D const & d)
        {
            return d((T &&) t);
        }

        using inheritance_tag_with_an_unlikely_name_ = int;
    };
#line 231 "boost/parser/detail/stl_interfaces/view_adaptor.hpp"
    template<typename F>
    struct closure : range_adaptor_closure<closure<F>>
    {
        constexpr closure(F f) : f_(f) {}





        template<
            typename T,
            typename Enable =
                std::enable_if_t<detail::is_invocable_v<F const &, T>>>

        constexpr decltype(auto) operator()(T && t) const &
        {
            return f_((T &&) t);
        }





        template<
            typename T,
            typename Enable = std::enable_if_t<detail::is_invocable_v<F &&, T>>>

        constexpr decltype(auto) operator()(T && t) &&
        {
            return std::move(f_)((T &&) t);
        }

    private:
        F f_;
    };


    namespace detail {

        template<typename F, bool Invocable, typename... Args>
        struct adaptor_impl
        {
            static constexpr decltype(auto) call(F const & f, Args &&... args)
            {
                return f((Args &&) args...);
            }
        };

        template<typename F, typename... Args>
        struct adaptor_impl<F, false, Args...>
        {
            static constexpr auto call(F const & f, Args &&... args)
            {
                using closure_func =
                    std::decay_t<decltype(stl_interfaces::bind_back(
                        f, (Args &&) args...))>;
                return closure<closure_func>(
                    stl_interfaces::bind_back(f, (Args &&) args...));
            }
        };

    }






    template<typename F>
    struct adaptor
    {
        constexpr adaptor(F f) : f_(f) {}


        template<typename... Args>
        constexpr auto operator()(Args &&... args) const

        {
#line 317 "boost/parser/detail/stl_interfaces/view_adaptor.hpp"
            return detail::adaptor_impl<
                F const &,
                detail::is_invocable_v<F const &, Args...>,
                Args...>::call(f_, (Args &&) args...);

        }

    private:
        F f_;
    };


}}
#line 16 "boost/parser/detail/text/transcode_view.hpp"
#include <climits>


namespace boost::parser::detail { namespace text {

    namespace detail {
        template<class I>
        constexpr auto iterator_to_tag()
        {
#line 32 "boost/parser/detail/text/transcode_view.hpp"
            if constexpr (detail::random_access_iterator_v<I>) {
                return std::random_access_iterator_tag{};
            } else if constexpr (detail::bidirectional_iterator_v<I>) {
                return std::bidirectional_iterator_tag{};
            } else if constexpr (detail::forward_iterator_v<I>) {

                return std::forward_iterator_tag{};
            } else {
                return std::input_iterator_tag{};
            }
        }
        template<class I>
        using iterator_to_tag_t = decltype(iterator_to_tag<I>());
#line 59 "boost/parser/detail/text/transcode_view.hpp"
        struct cast_to_char8;
        struct cast_to_char16;
        struct cast_to_char32;
        template<typename Tag, typename Arg>
        auto function_for_tag(Arg arg)
        {





                if constexpr (std::is_same_v<Tag, cast_to_char16>) {
                return (char16_t)arg;
            } else if constexpr (std::is_same_v<Tag, cast_to_char32>) {
                return (char32_t)arg;
            }
        }

    }
#line 85 "boost/parser/detail/text/transcode_view.hpp"
    template<typename V, typename F>

    class project_view : public stl_interfaces::view_interface<project_view<V, F>>
    {
        V base_ = V();

        template<bool Const>
        class iterator;
        template<bool Const>
        class sentinel;

    public:
        constexpr project_view()



        = default;
        constexpr explicit project_view(V base) : base_(std::move(base)) {}

        constexpr V& base() & { return base_; }
        constexpr const V& base() const& { return base_; }
        constexpr V base() && { return std::move(base_); }

        constexpr iterator<false> begin() { return iterator<false>{detail::begin(base_)}; }
        constexpr iterator<true> begin() const



        { return iterator<true>{detail::begin(base_)}; }

        constexpr sentinel<false> end() { return sentinel<false>{detail::end(base_)}; }




        constexpr sentinel<true> end() const






        { return iterator<true>{detail::end(base_)}; }





    };
#line 141 "boost/parser/detail/text/transcode_view.hpp"
    template<typename V, typename F>

    template<bool Const>
    class project_view<V, F>::iterator
        : public boost::parser::detail::stl_interfaces::proxy_iterator_interface<
              iterator<Const>,
              detail::iterator_to_tag_t<detail::iterator_t<detail::maybe_const<Const, V>>>,



              decltype(detail::function_for_tag<F>(0))

        >
    {
        using iterator_type = detail::iterator_t<detail::maybe_const<Const, V>>;
        using sentinel_type = detail::sentinel_t<detail::maybe_const<Const, V>>;
        using reference_type =



            decltype(detail::function_for_tag<F>(0))

            ;
        using sentinel = project_view<V, F>::sentinel<Const>;

        friend boost::parser::detail::stl_interfaces::access;
        iterator_type & base_reference() noexcept { return it_; }
        iterator_type base_reference() const { return it_; }

        iterator_type it_ = iterator_type();

        friend project_view<V, F>::sentinel<Const>;

        template<bool OtherConst>



        friend constexpr bool operator==(const iterator<OtherConst> & x,
                                         const sentinel & y);

        template<bool OtherConst>



        friend constexpr detail::range_difference_t<detail::maybe_const<OtherConst, V>>
        operator-(const iterator<OtherConst> & x, const sentinel & y);

        template<bool OtherConst>



        friend constexpr detail::range_difference_t<detail::maybe_const<OtherConst, V>>
        operator-(const sentinel & y, const iterator<OtherConst> & x);

    public:
        constexpr iterator() = default;
        constexpr iterator(iterator_type it) : it_(std::move(it)) {}




        constexpr reference_type operator*() const
        {
            return detail::function_for_tag<F>(*it_);
        }

    };
#line 215 "boost/parser/detail/text/transcode_view.hpp"
    template<typename V, typename F>

    template<bool Const>
    class project_view<V, F>::sentinel
    {
        using Base = detail::maybe_const<Const, V>;
        using sentinel_type = detail::sentinel_t<Base>;

        sentinel_type end_ = sentinel_type();

    public:
        constexpr sentinel() = default;
        constexpr explicit sentinel(sentinel_type end) : end_(std::move(end)) {}

        template<bool Enable = Const, class = std::enable_if_t<Enable>>

        constexpr sentinel(sentinel<!Const> i)




            : end_(std::move(i.end_))
        {}

        constexpr sentinel_type base() const { return end_; }

        template<bool OtherConst>



        friend constexpr bool operator==(const iterator<OtherConst> & x,
                                         const sentinel & y)
            { return x.it_ == y.end_; }

        template<bool OtherConst>



        friend constexpr detail::range_difference_t<detail::maybe_const<OtherConst, V>>
        operator-(const iterator<OtherConst> & x, const sentinel & y)
            { return x.it_ - y.end_; }

        template<bool OtherConst>



        friend constexpr detail::range_difference_t<detail::maybe_const<OtherConst, V>>
        operator-(const sentinel & y, const iterator<OtherConst> & x)
            { return y.end_ - x.it_; }
    };






    namespace detail {



        template<typename F>

        struct project_impl : stl_interfaces::range_adaptor_closure<project_impl<F>>
        {
            template<class R>
            using project_view_type = project_view<R, F>;
#line 289 "boost/parser/detail/text/transcode_view.hpp"
            template<class R>

            [[nodiscard]] constexpr auto operator()(R && r) const
            {



                return project_view_type<R>(std::forward<R>(r));

            }
        };
    }




    template<typename F>

    constexpr detail::project_impl<F> project{};
#line 346 "boost/parser/detail/text/transcode_view.hpp"
    template<typename V>
    class char16_view : public project_view<V, detail::cast_to_char16>

    {
    public:
        constexpr char16_view()



        = default;
        constexpr char16_view(V base) :



            project_view<V, detail::cast_to_char16>{std::move(base)}

        {}
    };





    template<typename V>
    class char32_view : public project_view<V, detail::cast_to_char32>

    {
    public:
        constexpr char32_view()



        = default;
        constexpr char32_view(V base) :



            project_view<V, detail::cast_to_char32>{std::move(base)}

        {}
    };
#line 399 "boost/parser/detail/text/transcode_view.hpp"
    namespace detail {
        template<template<class> class View, format Format>
        struct as_charn_impl : stl_interfaces::range_adaptor_closure<as_charn_impl<View, Format>>
        {
#line 410 "boost/parser/detail/text/transcode_view.hpp"
            template<class R>

            [[nodiscard]] constexpr auto operator()(R && r) const
            {
                using T = remove_cv_ref_t<R>;
                if constexpr (detail::is_empty_view<T>) {



                    return 42;

                } else if constexpr (std::is_pointer_v<T>) {
                    return View(
                        boost::parser::subrange(r, null_sentinel));
                } else {
                    return View(std::forward<R>(r));
                }
            }
        };

        template<class T>
        constexpr bool is_charn_view = false;




        template<class V>
        constexpr bool is_charn_view<char16_view<V>> = true;
        template<class V>
        constexpr bool is_charn_view<char32_view<V>> = true;
    }




    inline constexpr detail::as_charn_impl<char16_view, format::utf16> as_char16_t;
    inline constexpr detail::as_charn_impl<char32_view, format::utf32> as_char32_t;






    template<typename V>

    class unpacking_view : public stl_interfaces::view_interface<unpacking_view<V>> {
      V base_ = V();

    public:
      constexpr unpacking_view()



      = default;
      constexpr unpacking_view(V base) : base_(std::move(base)) {}

      constexpr V base() const &



      { return base_; }
      constexpr V base() && { return std::move(base_); }

      constexpr auto code_units() const noexcept {
        auto unpacked = boost::parser::detail::text::unpack_iterator_and_sentinel(detail::begin(base_), detail::end(base_));
        return boost::parser::subrange(unpacked.first, unpacked.last);
      }

      constexpr auto begin() { return code_units().begin(); }
      constexpr auto begin() const { return code_units().begin(); }

      constexpr auto end() { return code_units().end(); }
      constexpr auto end() const { return code_units().end(); }
    };

    template<class R>
    unpacking_view(R &&) -> unpacking_view<detail::all_t<R>>;






    template<format Format, typename V>

    class utf_view : public stl_interfaces::view_interface<utf_view<Format, V>>
    {
        V base_ = V();

        template<format FromFormat, class I, class S>
        static constexpr auto make_begin(I first, S last)
        {
            if constexpr (detail::bidirectional_iterator_v<I>) {
                return utf_iterator<FromFormat, Format, I, S>{first, first, last};
            } else {
                return utf_iterator<FromFormat, Format, I, S>{first, last};
            }
        }
        template<format FromFormat, class I, class S>
        static constexpr auto make_end(I first, S last)
        {
            if constexpr (!std::is_same_v<I, S>) {
                return last;
            } else if constexpr (detail::bidirectional_iterator_v<I>) {
                return utf_iterator<FromFormat, Format, I, S>{first, last, last};
            } else {
                return utf_iterator<FromFormat, Format, I, S>{last, last};
            }
        }

    public:
        constexpr utf_view()



        = default;
        constexpr utf_view(V base) : base_{std::move(base)} {}

        constexpr V base() const &



        { return base_; }
        constexpr V base() && { return std::move(base_); }

        constexpr auto begin()
        {
            constexpr format from_format = detail::format_of<detail::range_value_t<V>>();
            if constexpr(detail::is_charn_view<V>) {
                return make_begin<from_format>(detail::begin(base_.base()), detail::end(base_.base()));
            } else {
                return make_begin<from_format>(detail::begin(base_), detail::end(base_));
            }
        }
        constexpr auto begin() const
        {
            constexpr format from_format = detail::format_of<detail::range_value_t<const V>>();
            if constexpr(detail::is_charn_view<V>) {
                return make_begin<from_format>(detail::begin(base_.base()), detail::end(base_.base()));
            } else {
                return make_begin<from_format>(detail::begin(base_), detail::end(base_));
            }
        }

        constexpr auto end()
        {
            constexpr format from_format = detail::format_of<detail::range_value_t<V>>();
            if constexpr(detail::is_charn_view<V>) {
                return make_end<from_format>(detail::begin(base_.base()), detail::end(base_.base()));
            } else {
                return make_end<from_format>(detail::begin(base_), detail::end(base_));
            }
        }
        constexpr auto end() const
        {
            constexpr format from_format = detail::format_of<detail::range_value_t<const V>>();
            if constexpr(detail::is_charn_view<V>) {
                return make_end<from_format>(detail::begin(base_.base()), detail::end(base_.base()));
            } else {
                return make_end<from_format>(detail::begin(base_), detail::end(base_));
            }
        }



        friend std::ostream & operator<<(std::ostream & os, utf_view v)
        {
            if constexpr (Format == format::utf8) {
                auto out = std::ostreambuf_iterator<char>(os);
                for (auto it = v.begin(); it != v.end(); ++it, ++out) {
                    *out = *it;
                }
            } else {
                boost::parser::detail::text::transcode_to_utf8(
                    v.begin(), v.end(), std::ostreambuf_iterator<char>(os));
            }
            return os;
        }
#line 605 "boost/parser/detail/text/transcode_view.hpp"
    };
#line 626 "boost/parser/detail/text/transcode_view.hpp"
    template<typename V>

    class utf8_view : public utf_view<format::utf8, V>
    {
    public:
        constexpr utf8_view()



        = default;
        constexpr utf8_view(V base) :
            utf_view<format::utf8, V>{std::move(base)}
        {}
    };




    template<typename V>

    class utf16_view : public utf_view<format::utf16, V>
    {
    public:
        constexpr utf16_view()



        = default;
        constexpr utf16_view(V base) :
            utf_view<format::utf16, V>{std::move(base)}
        {}
    };




    template<typename V>

    class utf32_view : public utf_view<format::utf32, V>
    {
    public:
        constexpr utf32_view()



        = default;
        constexpr utf32_view(V base) :
            utf_view<format::utf32, V>{std::move(base)}
        {}
    };


    template<class R>
    utf8_view(R &&) -> utf8_view<detail::all_t<R>>;
    template<class R>
    utf16_view(R &&) -> utf16_view<detail::all_t<R>>;
    template<class R>
    utf32_view(R &&) -> utf32_view<detail::all_t<R>>;
#line 701 "boost/parser/detail/text/transcode_view.hpp"
    namespace detail {




        template<class R, class View>
        using can_utf_view_expr = decltype(View(std::declval<R>()));
        template<class R, template<class> class View>
        constexpr bool can_utf_view =
            is_detected_v<can_utf_view_expr, R, View<R>>;


        template<class T>
        constexpr bool is_utf_view = false;
        template<class T>
        constexpr bool is_utf_view<utf8_view<T>> = true;
        template<class T>
        constexpr bool is_utf_view<utf16_view<T>> = true;
        template<class T>
        constexpr bool is_utf_view<utf32_view<T>> = true;
        template<format F, class T>
        constexpr bool is_utf_view<utf_view<F, T>> = true;

        template<typename T>
        constexpr bool is_bounded_array_v = false;
        template<typename T, int N>
        constexpr bool is_bounded_array_v<T[N]> = true;

        template<class R>
        constexpr decltype(auto) unpack_range(R && r)
        {
            using T = detail::remove_cv_ref_t<R>;
            if constexpr (forward_range_v<T>) {
                auto unpacked =
                    boost::parser::detail::text::unpack_iterator_and_sentinel(detail::begin(r), detail::end(r));
                if constexpr (is_bounded_array_v<T>) {
                    constexpr auto n = std::extent_v<T>;
                    if (n && !r[n - 1])
                        --unpacked.last;
                    return boost::parser::subrange(unpacked.first, unpacked.last);
                } else if constexpr (
                    !std::is_same_v<decltype(unpacked.first), iterator_t<R>> ||
                    !std::is_same_v<decltype(unpacked.last), sentinel_t<R>>) {
                    return unpacking_view(std::forward<R>(r));
                } else {
                    return std::forward<R>(r);
                }
            } else {
                return std::forward<R>(r);
            }
        }

        template<class R>
        using unpacked_range = decltype(detail::unpack_range(std::declval<R>()));

        template<template<class> class View, format Format>
        struct as_utf_impl : stl_interfaces::range_adaptor_closure<as_utf_impl<View, Format>>
        {
#line 766 "boost/parser/detail/text/transcode_view.hpp"
            template<typename R>

            [[nodiscard]] constexpr auto operator()(R && r) const
            {
                using T = detail::remove_cv_ref_t<R>;
                if constexpr (detail::is_empty_view<T>) {



                    return 42;

                } else if constexpr (is_utf_view<T>) {
                    return View(std::forward<R>(r).base());
                } else if constexpr (detail::is_charn_view<T>) {
                    return View(std::forward<R>(r));
                } else if constexpr (std::is_pointer_v<T>) {
                    return View(
                        boost::parser::subrange(r, null_sentinel));
                } else {
                    return View(detail::unpack_range(std::forward<R>(r)));
                }
            }
        };

        template<class T>
        constexpr bool is_utf32_view = false;
        template<class V>
        constexpr bool is_utf32_view<utf_view<format::utf32, V>> = true;
    }

    inline constexpr detail::as_utf_impl<utf8_view, format::utf8> as_utf8;
    inline constexpr detail::as_utf_impl<utf16_view, format::utf16> as_utf16;
    inline constexpr detail::as_utf_impl<utf32_view, format::utf32> as_utf32;

}}
#line 8 "boost/parser/error_handling_fwd.hpp"
#include <iostream>
#include <string_view>


namespace boost { namespace parser {



    enum class error_handler_result {
        fail,
        rethrow
    };




    template<typename Iter>
    struct parse_error : std::runtime_error
    {
        parse_error(Iter it, std::string const & msg) :
            runtime_error(msg), iter(it)
        {}

        Iter iter;
    };



    template<typename Iter>
    struct line_position
    {
        Iter line_start;
        int64_t line_number;
        int64_t column_number;
    };



    template<typename Iter, typename Sentinel>
    std::ostream & write_formatted_message(
        std::ostream & os,
        std::string_view filename,
        Iter first,
        Iter it,
        Sentinel last,
        std::string_view message,
        int64_t preferred_max_line_length = 80,
        int64_t max_after_caret = 40);
#line 74 "boost/parser/error_handling_fwd.hpp"
    template<typename Iter, typename Sentinel>
    std::ostream & write_formatted_expectation_failure_error_message(
        std::ostream & os,
        std::string_view filename,
        Iter first,
        Sentinel last,
        parse_error<Iter> const & e,
        int64_t preferred_max_line_length = 80,
        int64_t max_after_caret = 40);
#line 100 "boost/parser/error_handling_fwd.hpp"
    enum class diagnostic_kind {
        error,
        warning
    };




    struct default_error_handler
    {
        constexpr default_error_handler() = default;




        template<typename Iter, typename Sentinel>
        error_handler_result operator()(
            Iter first, Sentinel last, parse_error<Iter> const & e) const;






        template<typename Context, typename Iter>
        void diagnose(
            diagnostic_kind kind,
            std::string_view message,
            Context const & context,
            Iter it) const;






        template<typename Context>
        void diagnose(
            diagnostic_kind kind,
            std::string_view message,
            Context const & context) const;

    };




    struct stream_error_handler
    {
        stream_error_handler() : err_os_(&std::cerr), warn_os_(err_os_) {}
        stream_error_handler(std::string_view filename) :
            filename_(filename), err_os_(&std::cerr), warn_os_(err_os_)
        {}
        stream_error_handler(std::string_view filename, std::ostream & errors) :
            filename_(filename), err_os_(&errors), warn_os_(&errors)
        {}
        stream_error_handler(
            std::string_view filename,
            std::ostream & errors,
            std::ostream & warnings) :
            filename_(filename), err_os_(&errors), warn_os_(&warnings)
        {}
#line 194 "boost/parser/error_handling_fwd.hpp"
        template<typename Iter, typename Sentinel>
        error_handler_result
        operator()(Iter first, Sentinel last, parse_error<Iter> const & e) const;
#line 204 "boost/parser/error_handling_fwd.hpp"
        template<typename Context, typename Iter>
        void diagnose(
            diagnostic_kind kind,
            std::string_view message,
            Context const & context,
            Iter it) const;






        template<typename Context>
        void diagnose(
            diagnostic_kind kind,
            std::string_view message,
            Context const & context) const;

    private:
        std::string filename_;
        std::ostream * err_os_;
        std::ostream * warn_os_;
    };

}}
#line 12 "boost/parser/parser_fwd.hpp"
#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <variant>


namespace boost::parser::detail { namespace text {
    struct null_sentinel_t;
}}

namespace boost { namespace parser {



    using null_sentinel_t = boost::parser::detail::text::null_sentinel_t;



    template<typename T>
    constexpr bool enable_optional = false;



    template<typename T>
    constexpr bool enable_variant = false;


    template<typename T>
    constexpr bool enable_optional<std::optional<T>> = true;
    template<typename... Ts>
    constexpr bool enable_variant<std::variant<Ts...>> = true;


    namespace detail {
        template<typename T>
        constexpr bool is_optional_v = enable_optional<T>;

        struct nope;

        enum class flags : unsigned int {
            gen_attrs = 1 << 0,
            use_skip = 1 << 1,
            trace = 1 << 2,
            in_apply_parser = 1 << 3
        };

        struct any_copyable
        {
            template<
                typename T,
                typename Enable = std::enable_if_t<!std::is_reference_v<T>>>
            any_copyable(T && v) :
                impl_(new holder<std::decay_t<T>>(std::move(v)))
            {}
            template<typename T>
            any_copyable(T const & v) : impl_(new holder<T>(v))
            {}

            any_copyable() = default;
            any_copyable(any_copyable const & other)
            {
                if (other.impl_)
                    impl_ = other.impl_->clone();
            }
            any_copyable & operator=(any_copyable const & other)
            {
                any_copyable temp(other);
                swap(temp);
                return *this;
            }
            any_copyable(any_copyable &&) = default;
            any_copyable & operator=(any_copyable &&) = default;

            bool empty() const { return impl_.get() == nullptr; }

            template<typename T>
            T & cast() const
            {
                BOOST_ASSERT(impl_);
                BOOST_ASSERT(dynamic_cast<holder<T> *>(impl_.get()));
                return static_cast<holder<T> *>(impl_.get())->value_;
            }

            void swap(any_copyable & other) { std::swap(impl_, other.impl_); }

        private:
            struct holder_base
            {
                virtual ~holder_base() {}
                virtual std::unique_ptr<holder_base> clone() const = 0;
            };
            template<typename T>
            struct holder : holder_base
            {
                holder(T && v) : value_(std::move(v)) {}
                holder(T const & v) : value_(v) {}
                virtual ~holder() {}
                virtual std::unique_ptr<holder_base> clone() const
                {
                    return std::unique_ptr<holder_base>(new holder<T>{value_});
                }
                T value_;
            };

            std::unique_ptr<holder_base> impl_;
        };

        using symbol_table_tries_t =
            std::map<void *, any_copyable, std::less<void *>>;

        template<typename Iter, typename Sentinel, typename ErrorHandler>
        inline auto make_context(
            Iter first,
            Sentinel last,
            bool & success,
            int & indent,
            ErrorHandler const & error_handler,
            nope &,
            symbol_table_tries_t & symbol_table_tries) noexcept;

        struct skip_skipper;

        struct char_subrange
        {
            char32_t lo_;
            char32_t hi_;
        };

        template<typename Tag>
        struct char_subranges
        {};

        struct hex_digit_subranges
        {};
        struct control_subranges
        {};

        template<typename Tag>
        struct char_set
        {};

        struct punct_chars
        {};
        struct lower_case_chars
        {};
        struct upper_case_chars
        {};
    }
#line 168 "boost/parser/parser_fwd.hpp"
    template<
        typename Parser,
        typename DelimiterParser = detail::nope,
        typename MinType = int64_t,
        typename MaxType = int64_t>
    struct repeat_parser;




    template<typename Parser>
    struct zero_plus_parser;





    template<typename Parser>
    struct one_plus_parser;
#line 194 "boost/parser/parser_fwd.hpp"
    template<typename Parser, typename DelimiterParser>
    struct delimited_seq_parser;





    template<typename Parser>
    struct opt_parser;






    template<typename ParserTuple>
    struct or_parser;
#line 219 "boost/parser/parser_fwd.hpp"
    template<
        typename ParserTuple,
        typename BacktrackingTuple,
        typename CombiningGroups>
    struct seq_parser;





    template<typename Parser, typename Action>
    struct action_parser;





    template<typename Parser>
    struct omit_parser;





    template<typename Parser>
    struct raw_parser;
#line 263 "boost/parser/parser_fwd.hpp"
    template<typename Parser>
    struct lexeme_parser;





    template<typename Parser>
    struct no_case_parser;





    template<typename Parser, typename SkipParser = detail::nope>
    struct skip_parser;




    template<typename Parser, bool FailOnMatch>
    struct expect_parser;
#line 292 "boost/parser/parser_fwd.hpp"
    template<typename T>
    struct symbol_parser;
#line 306 "boost/parser/parser_fwd.hpp"
    template<
        bool CanUseCallbacks,
        typename TagType,
        typename Attribute,
        typename LocalState,
        typename ParamsTuple>
    struct rule_parser;





    template<typename Predicate>
    struct eps_parser;


    struct eoi_parser;



    template<typename Attribute>
    struct attr_parser;




    struct sorted_t
    {};

    inline constexpr sorted_t sorted;
#line 343 "boost/parser/parser_fwd.hpp"
    template<typename Expected, typename AttributeType = void>
    struct char_parser;






    template<typename Tag>
    struct char_set_parser;






    template<typename Tag>
    struct char_subrange_parser;




    struct digit_parser;



    template<typename StrIter, typename StrSentinel>
    struct string_parser;






    template<bool NewlinesOnly, bool NoNewlines>
    struct ws_parser;



    struct bool_parser;
#line 390 "boost/parser/parser_fwd.hpp"
    template<
        typename T,
        int Radix = 10,
        int MinDigits = 1,
        int MaxDigits = -1,
        typename Expected = detail::nope>
    struct uint_parser;
#line 404 "boost/parser/parser_fwd.hpp"
    template<
        typename T,
        int Radix = 10,
        int MinDigits = 1,
        int MaxDigits = -1,
        typename Expected = detail::nope>
    struct int_parser;



    template<typename T>
    struct float_parser;






    template<typename SwitchValue, typename OrParser = detail::nope>
    struct switch_parser;
#line 434 "boost/parser/parser_fwd.hpp"
    template<
        typename Parser,
        typename GlobalState = detail::nope,
        typename ErrorHandler = default_error_handler>
    struct parser_interface;

    using no_attribute = detail::nope;
    using no_local_state = detail::nope;
    using no_params = detail::nope;




    template<
        typename TagType,
        typename Attribute = no_attribute,
        typename LocalState = no_local_state,
        typename ParamsTuple = no_params>
    struct rule;





    template<
        typename TagType,
        typename Attribute = no_attribute,
        typename LocalState = no_local_state,
        typename ParamsTuple = no_params>
    struct callback_rule;






    template<typename Context>
    decltype(auto) _val(Context const & context);





    template<typename Context>
    decltype(auto) _attr(Context const & context);



    template<typename Context>
    decltype(auto) _where(Context const & context);




    template<typename Context>
    decltype(auto) _begin(Context const & context);


    template<typename Context>
    decltype(auto) _end(Context const & context);




    template<typename Context>
    decltype(auto) _pass(Context const & context);





    template<typename Context>
    decltype(auto) _locals(Context const & context);






    template<typename Context>
    decltype(auto) _params(Context const & context);




    template<typename Context>
    decltype(auto) _globals(Context const & context);




    template<typename Context>
    decltype(auto) _error_handler(Context const & context);



    template<typename Iter, typename Context>
    void _report_error(
        Context const & context, std::string_view message, Iter location);



    template<typename Context>
    void _report_error(Context const & context, std::string_view message);



    template<typename Iter, typename Context>
    void _report_warning(
        Context const & context, std::string_view message, Iter location);



    template<typename Context>
    void _report_warning(Context const & context, std::string_view message);

}}
#line 7 "boost/parser/concepts.hpp"
#define BOOST_PARSER_CONCEPTS_HPP
#line 2 "boost/parser/error_handling.hpp"
#define BOOST_PARSER_ERROR_HANDLING_HPP
#line 2 "boost/parser/detail/printing.hpp"
#define BOOST_PARSER_DETAIL_PRINTING_HPP
#line 8 "boost/parser/tuple.hpp"
#define BOOST_PARSER_TUPLE_HPP
#line 26 "boost/parser/tuple.hpp"
#include <boost/hana.hpp>
#line 34 "boost/parser/tuple.hpp"
namespace boost { namespace parser {

    namespace detail {

        constexpr int to_int(char c)
        {
            int result = 0;

            if (c >= 'A' && c <= 'F') {
                result = static_cast<int>(c) - static_cast<int>('A') + 10;
            } else if (c >= 'a' && c <= 'f') {
                result = static_cast<int>(c) - static_cast<int>('a') + 10;
            } else {
                result = static_cast<int>(c) - static_cast<int>('0');
            }

            return result;
        }

        template<std::size_t N>
        constexpr long long parse_llong(const char (&arr)[N])
        {
            long long base = 10;
            std::size_t offset = 0;

            if constexpr (N > 2) {
                bool starts_with_zero = arr[0] == '0';
                bool is_hex = starts_with_zero && arr[1] == 'x';
                bool is_binary = starts_with_zero && arr[1] == 'b';

                if (is_hex) {

                    base = 16;
                    offset = 2;
                } else if (is_binary) {

                    base = 2;
                    offset = 2;
                } else if (starts_with_zero) {

                    base = 8;
                    offset = 1;
                }
            }

            long long number = 0;
            long long multiplier = 1;

            for (std::size_t i = 0; i < N - offset; ++i) {
                char c = arr[N - 1 - i];
                if (c != '\'') {
                    number += to_int(c) * multiplier;
                    multiplier *= base;
                }
            }

            return number;
        }
    }
#line 101 "boost/parser/tuple.hpp"
    template<typename... Args>
    using tuple = hana::tuple<Args...>;
#line 112 "boost/parser/tuple.hpp"
    template<typename T, T I>
    using integral_constant = hana::integral_constant<T, I>;




    template<typename T, typename U, U I>
    constexpr decltype(auto) get(T && x, integral_constant<U, I> i);




    template<long long I>
    using llong = integral_constant<long long, I>;

    namespace literals {


        template<char... chars>
        constexpr auto operator"" _c()
        {
            constexpr long long n =
                detail::parse_llong<sizeof...(chars)>({chars...});
            return llong<n>{};
        }
    }

    namespace detail {

        template<typename T, T I, typename... Args>
        constexpr decltype(auto)
        tuple_get(tuple<Args...> const & t, integral_constant<T, I>)
        {



            return hana::at_c<I>(t);

        }


        template<typename T, T I, typename... Args>
        constexpr decltype(auto)
        tuple_get(tuple<Args...> & t, integral_constant<T, I>)
        {



            return hana::at_c<I>(t);

        }


        template<typename T, T I, typename... Args>
        constexpr decltype(auto)
        tuple_get(tuple<Args...> && t, integral_constant<T, I>)
        {



            return std::move(hana::at_c<I>(t));

        }

        template<int N>
        struct ce_int
        {
            constexpr static int value = N;
        };

        struct whatever
        {
            int _;
            template<typename T>
            operator T() const && noexcept
            {






                return std::declval<T>();

            }
        };

        template<typename T, int... Is>
        constexpr auto
            constructible_expr_impl(std::integer_sequence<int, Is...>)
                -> decltype(T{whatever{Is}...}, ce_int<1>{});

        template<typename T, typename N>
        using constructible_expr = decltype(detail::constructible_expr_impl<T>(
            std::make_integer_sequence<int, N::value>()));

        template<typename T, int... Is>
        constexpr int struct_arity_impl(std::integer_sequence<int, Is...>)
        {
            return (
                detected_or_t<ce_int<0>, constructible_expr, T, ce_int<Is>>::
                    value +
                ... + 0);
        }
#line 221 "boost/parser/tuple.hpp"
        template<typename T>
        constexpr int struct_arity_v =
            detail::struct_arity_impl<T>(std::make_integer_sequence<
                                         int,
                                         25>()) -
            1;

        template<typename T>
        constexpr int tuple_size_ = -1;

        template<typename... Elems>
        constexpr int tuple_size_<tuple<Elems...>> = sizeof...(Elems);

        template<typename T, typename Tuple, int... Is>
        auto assign_tuple_to_aggregate(
            T & x, Tuple tup, std::integer_sequence<int, Is...>)
            -> decltype(x = T{parser::get(std::move(tup), llong<Is>{})...});

        template<typename T, typename Tuple, int... Is>
        auto tuple_to_aggregate(Tuple && tup, std::integer_sequence<int, Is...>)
            -> decltype(T{std::move(parser::get(tup, llong<Is>{}))...})
        {
            return T{std::move(parser::get(tup, llong<Is>{}))...};
        }

        template<typename T, typename Tuple>
        using tuple_to_aggregate_expr =
            decltype(detail::assign_tuple_to_aggregate(
                std::declval<T &>(),
                std::declval<Tuple>(),
                std::make_integer_sequence<int, tuple_size_<Tuple>>()));

        template<typename Struct, typename Tuple>
        constexpr bool is_struct_assignable_v =
            struct_arity_v<Struct> == tuple_size_<Tuple>
                ? is_detected_v<tuple_to_aggregate_expr, Struct, Tuple>
                : false;

        template<int N>
        struct tie_aggregate_impl
        {
            template<typename T>
            static constexpr auto call(T & x)
            {
                static_assert(
                    sizeof(T) && false,
                    "It looks like you're trying to use a struct larger than "
                    "the limit.");
            }
        };

        template<typename T>
        constexpr auto tie_aggregate(T & x)
        {
            static_assert(!std::is_union_v<T>);
            return tie_aggregate_impl<struct_arity_v<T>>::call(x);
        }

        template<typename Tuple, typename Tie, int... Is>
        auto aggregate_to_tuple(
            Tuple & tup, Tie tie, std::integer_sequence<int, Is...>)
            -> decltype((
                (parser::get(tup, llong<Is>{}) =
                     std::move(parser::get(tie, llong<Is>{}))),
                ...,
                (void)0))
        {
            return (
                (parser::get(tup, llong<Is>{}) =
                     std::move(parser::get(tie, llong<Is>{}))),
                ...,
                (void)0);
        }

        template<typename Tuple, typename T>
        using aggregate_to_tuple_expr = decltype(detail::aggregate_to_tuple(
            std::declval<Tuple &>(),
            detail::tie_aggregate(std::declval<T &>()),
            std::make_integer_sequence<int, tuple_size_<Tuple>>()));

        template<typename Tuple, typename Struct>
        constexpr bool is_tuple_assignable_impl()
        {
            if constexpr (
                std::is_aggregate_v<Struct> &&
                struct_arity_v<Struct> == tuple_size_<Tuple>) {
                return is_detected_v<aggregate_to_tuple_expr, Tuple, Struct>;
            } else {
                return false;
            }
        }

        template<typename Tuple, typename Struct>
        constexpr bool
            is_tuple_assignable_v = is_tuple_assignable_impl<Tuple, Struct>();

        template<typename T>
        struct is_tuple : std::false_type
        {};
        template<typename... T>
        struct is_tuple<tuple<T...>> : std::true_type
        {};
    }

    template<typename T, typename U, U I>
    constexpr decltype(auto) get(T && x, integral_constant<U, I> i)
    {
        using just_t = std::decay_t<T>;
        if constexpr (detail::is_tuple<just_t>::value) {
            return detail::tuple_get((T &&) x, i);
        } else if constexpr (std::is_aggregate_v<just_t>) {
            auto tup = detail::tie_aggregate(x);
            return detail::tuple_get(tup, i);
        } else {
            static_assert(
                sizeof(T) != sizeof(T),
                "boost::parser::get() is only defined for boost::parser::tuple "
                "and aggregate structs.");
        }
    }

}}
#line 9 "boost/parser/detail/aggr_to_tuple_generated.hpp"
namespace boost::parser::detail {


template<> struct tie_aggregate_impl<1> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01
] = x;
return parser::tuple<
    decltype(_01) &
>(
    _01
);
}
};


template<> struct tie_aggregate_impl<2> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &
>(
    _01, _02
);
}
};


template<> struct tie_aggregate_impl<3> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &
>(
    _01, _02, _03
);
}
};


template<> struct tie_aggregate_impl<4> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &
>(
    _01, _02, _03, _04
);
}
};


template<> struct tie_aggregate_impl<5> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &
>(
    _01, _02, _03, _04, _05
);
}
};


template<> struct tie_aggregate_impl<6> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &
>(
    _01, _02, _03, _04, _05, _06
);
}
};


template<> struct tie_aggregate_impl<7> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &
>(
    _01, _02, _03, _04, _05, _06, _07
);
}
};


template<> struct tie_aggregate_impl<8> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08
);
}
};


template<> struct tie_aggregate_impl<9> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09
);
}
};


template<> struct tie_aggregate_impl<10> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a
);
}
};


template<> struct tie_aggregate_impl<11> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b
);
}
};


template<> struct tie_aggregate_impl<12> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c
);
}
};


template<> struct tie_aggregate_impl<13> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d
);
}
};


template<> struct tie_aggregate_impl<14> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e
);
}
};


template<> struct tie_aggregate_impl<15> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f
);
}
};


template<> struct tie_aggregate_impl<16> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10
);
}
};


template<> struct tie_aggregate_impl<17> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11
);
}
};


template<> struct tie_aggregate_impl<18> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12
);
}
};


template<> struct tie_aggregate_impl<19> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13
);
}
};


template<> struct tie_aggregate_impl<20> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14
);
}
};


template<> struct tie_aggregate_impl<21> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15
);
}
};


template<> struct tie_aggregate_impl<22> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16
);
}
};


template<> struct tie_aggregate_impl<23> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17
);
}
};


template<> struct tie_aggregate_impl<24> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18
);
}
};


template<> struct tie_aggregate_impl<25> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19
);
}
};


template<> struct tie_aggregate_impl<26> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a
);
}
};


template<> struct tie_aggregate_impl<27> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b
);
}
};


template<> struct tie_aggregate_impl<28> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c
);
}
};


template<> struct tie_aggregate_impl<29> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d
);
}
};


template<> struct tie_aggregate_impl<30> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e
);
}
};


template<> struct tie_aggregate_impl<31> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f
);
}
};


template<> struct tie_aggregate_impl<32> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20
);
}
};


template<> struct tie_aggregate_impl<33> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21
);
}
};


template<> struct tie_aggregate_impl<34> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22
);
}
};


template<> struct tie_aggregate_impl<35> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23
);
}
};


template<> struct tie_aggregate_impl<36> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24
);
}
};


template<> struct tie_aggregate_impl<37> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25
);
}
};


template<> struct tie_aggregate_impl<38> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26
);
}
};


template<> struct tie_aggregate_impl<39> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27
);
}
};


template<> struct tie_aggregate_impl<40> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28
);
}
};


template<> struct tie_aggregate_impl<41> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29
);
}
};


template<> struct tie_aggregate_impl<42> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a
);
}
};


template<> struct tie_aggregate_impl<43> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b
);
}
};


template<> struct tie_aggregate_impl<44> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c
);
}
};


template<> struct tie_aggregate_impl<45> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d
);
}
};


template<> struct tie_aggregate_impl<46> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e
);
}
};


template<> struct tie_aggregate_impl<47> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f
);
}
};


template<> struct tie_aggregate_impl<48> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30
);
}
};


template<> struct tie_aggregate_impl<49> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31
);
}
};


template<> struct tie_aggregate_impl<50> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32
);
}
};


template<> struct tie_aggregate_impl<51> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33
);
}
};


template<> struct tie_aggregate_impl<52> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34
);
}
};


template<> struct tie_aggregate_impl<53> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35
);
}
};


template<> struct tie_aggregate_impl<54> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36
);
}
};


template<> struct tie_aggregate_impl<55> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37
);
}
};


template<> struct tie_aggregate_impl<56> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38
);
}
};


template<> struct tie_aggregate_impl<57> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39
);
}
};


template<> struct tie_aggregate_impl<58> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a
);
}
};


template<> struct tie_aggregate_impl<59> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b
);
}
};


template<> struct tie_aggregate_impl<60> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c
);
}
};


template<> struct tie_aggregate_impl<61> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d
);
}
};


template<> struct tie_aggregate_impl<62> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e
);
}
};


template<> struct tie_aggregate_impl<63> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f
);
}
};


template<> struct tie_aggregate_impl<64> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40
);
}
};


template<> struct tie_aggregate_impl<65> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41
);
}
};


template<> struct tie_aggregate_impl<66> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42
);
}
};


template<> struct tie_aggregate_impl<67> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43
);
}
};


template<> struct tie_aggregate_impl<68> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44
);
}
};


template<> struct tie_aggregate_impl<69> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45
);
}
};


template<> struct tie_aggregate_impl<70> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46
);
}
};


template<> struct tie_aggregate_impl<71> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &,
    decltype(_47) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47
);
}
};


template<> struct tie_aggregate_impl<72> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &,
    decltype(_47) &, decltype(_48) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48
);
}
};


template<> struct tie_aggregate_impl<73> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &,
    decltype(_47) &, decltype(_48) &, decltype(_49) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49
);
}
};


template<> struct tie_aggregate_impl<74> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &,
    decltype(_47) &, decltype(_48) &, decltype(_49) &, decltype(_4a) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a
);
}
};


template<> struct tie_aggregate_impl<75> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &,
    decltype(_47) &, decltype(_48) &, decltype(_49) &, decltype(_4a) &, decltype(_4b) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b
);
}
};


template<> struct tie_aggregate_impl<76> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &,
    decltype(_47) &, decltype(_48) &, decltype(_49) &, decltype(_4a) &, decltype(_4b) &,
    decltype(_4c) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c
);
}
};


template<> struct tie_aggregate_impl<77> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &,
    decltype(_47) &, decltype(_48) &, decltype(_49) &, decltype(_4a) &, decltype(_4b) &,
    decltype(_4c) &, decltype(_4d) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d
);
}
};


template<> struct tie_aggregate_impl<78> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &,
    decltype(_47) &, decltype(_48) &, decltype(_49) &, decltype(_4a) &, decltype(_4b) &,
    decltype(_4c) &, decltype(_4d) &, decltype(_4e) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e
);
}
};


template<> struct tie_aggregate_impl<79> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &,
    decltype(_47) &, decltype(_48) &, decltype(_49) &, decltype(_4a) &, decltype(_4b) &,
    decltype(_4c) &, decltype(_4d) &, decltype(_4e) &, decltype(_4f) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f
);
}
};


template<> struct tie_aggregate_impl<80> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &,
    decltype(_47) &, decltype(_48) &, decltype(_49) &, decltype(_4a) &, decltype(_4b) &,
    decltype(_4c) &, decltype(_4d) &, decltype(_4e) &, decltype(_4f) &, decltype(_50) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50
);
}
};


template<> struct tie_aggregate_impl<81> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &,
    decltype(_47) &, decltype(_48) &, decltype(_49) &, decltype(_4a) &, decltype(_4b) &,
    decltype(_4c) &, decltype(_4d) &, decltype(_4e) &, decltype(_4f) &, decltype(_50) &,
    decltype(_51) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51
);
}
};


template<> struct tie_aggregate_impl<82> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &,
    decltype(_47) &, decltype(_48) &, decltype(_49) &, decltype(_4a) &, decltype(_4b) &,
    decltype(_4c) &, decltype(_4d) &, decltype(_4e) &, decltype(_4f) &, decltype(_50) &,
    decltype(_51) &, decltype(_52) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52
);
}
};


template<> struct tie_aggregate_impl<83> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &,
    decltype(_47) &, decltype(_48) &, decltype(_49) &, decltype(_4a) &, decltype(_4b) &,
    decltype(_4c) &, decltype(_4d) &, decltype(_4e) &, decltype(_4f) &, decltype(_50) &,
    decltype(_51) &, decltype(_52) &, decltype(_53) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53
);
}
};


template<> struct tie_aggregate_impl<84> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &,
    decltype(_47) &, decltype(_48) &, decltype(_49) &, decltype(_4a) &, decltype(_4b) &,
    decltype(_4c) &, decltype(_4d) &, decltype(_4e) &, decltype(_4f) &, decltype(_50) &,
    decltype(_51) &, decltype(_52) &, decltype(_53) &, decltype(_54) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54
);
}
};


template<> struct tie_aggregate_impl<85> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &,
    decltype(_47) &, decltype(_48) &, decltype(_49) &, decltype(_4a) &, decltype(_4b) &,
    decltype(_4c) &, decltype(_4d) &, decltype(_4e) &, decltype(_4f) &, decltype(_50) &,
    decltype(_51) &, decltype(_52) &, decltype(_53) &, decltype(_54) &, decltype(_55) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55
);
}
};


template<> struct tie_aggregate_impl<86> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55, _56
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &,
    decltype(_47) &, decltype(_48) &, decltype(_49) &, decltype(_4a) &, decltype(_4b) &,
    decltype(_4c) &, decltype(_4d) &, decltype(_4e) &, decltype(_4f) &, decltype(_50) &,
    decltype(_51) &, decltype(_52) &, decltype(_53) &, decltype(_54) &, decltype(_55) &,
    decltype(_56) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55, _56
);
}
};


template<> struct tie_aggregate_impl<87> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55, _56, _57
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &,
    decltype(_47) &, decltype(_48) &, decltype(_49) &, decltype(_4a) &, decltype(_4b) &,
    decltype(_4c) &, decltype(_4d) &, decltype(_4e) &, decltype(_4f) &, decltype(_50) &,
    decltype(_51) &, decltype(_52) &, decltype(_53) &, decltype(_54) &, decltype(_55) &,
    decltype(_56) &, decltype(_57) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55, _56, _57
);
}
};


template<> struct tie_aggregate_impl<88> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55, _56, _57, _58
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &,
    decltype(_47) &, decltype(_48) &, decltype(_49) &, decltype(_4a) &, decltype(_4b) &,
    decltype(_4c) &, decltype(_4d) &, decltype(_4e) &, decltype(_4f) &, decltype(_50) &,
    decltype(_51) &, decltype(_52) &, decltype(_53) &, decltype(_54) &, decltype(_55) &,
    decltype(_56) &, decltype(_57) &, decltype(_58) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55, _56, _57, _58
);
}
};


template<> struct tie_aggregate_impl<89> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &,
    decltype(_47) &, decltype(_48) &, decltype(_49) &, decltype(_4a) &, decltype(_4b) &,
    decltype(_4c) &, decltype(_4d) &, decltype(_4e) &, decltype(_4f) &, decltype(_50) &,
    decltype(_51) &, decltype(_52) &, decltype(_53) &, decltype(_54) &, decltype(_55) &,
    decltype(_56) &, decltype(_57) &, decltype(_58) &, decltype(_59) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59
);
}
};


template<> struct tie_aggregate_impl<90> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _5a
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &,
    decltype(_47) &, decltype(_48) &, decltype(_49) &, decltype(_4a) &, decltype(_4b) &,
    decltype(_4c) &, decltype(_4d) &, decltype(_4e) &, decltype(_4f) &, decltype(_50) &,
    decltype(_51) &, decltype(_52) &, decltype(_53) &, decltype(_54) &, decltype(_55) &,
    decltype(_56) &, decltype(_57) &, decltype(_58) &, decltype(_59) &, decltype(_5a) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _5a
);
}
};


template<> struct tie_aggregate_impl<91> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _5a,
    _5b
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &,
    decltype(_47) &, decltype(_48) &, decltype(_49) &, decltype(_4a) &, decltype(_4b) &,
    decltype(_4c) &, decltype(_4d) &, decltype(_4e) &, decltype(_4f) &, decltype(_50) &,
    decltype(_51) &, decltype(_52) &, decltype(_53) &, decltype(_54) &, decltype(_55) &,
    decltype(_56) &, decltype(_57) &, decltype(_58) &, decltype(_59) &, decltype(_5a) &,
    decltype(_5b) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _5a,
    _5b
);
}
};


template<> struct tie_aggregate_impl<92> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _5a,
    _5b, _5c
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &,
    decltype(_47) &, decltype(_48) &, decltype(_49) &, decltype(_4a) &, decltype(_4b) &,
    decltype(_4c) &, decltype(_4d) &, decltype(_4e) &, decltype(_4f) &, decltype(_50) &,
    decltype(_51) &, decltype(_52) &, decltype(_53) &, decltype(_54) &, decltype(_55) &,
    decltype(_56) &, decltype(_57) &, decltype(_58) &, decltype(_59) &, decltype(_5a) &,
    decltype(_5b) &, decltype(_5c) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _5a,
    _5b, _5c
);
}
};


template<> struct tie_aggregate_impl<93> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _5a,
    _5b, _5c, _5d
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &,
    decltype(_47) &, decltype(_48) &, decltype(_49) &, decltype(_4a) &, decltype(_4b) &,
    decltype(_4c) &, decltype(_4d) &, decltype(_4e) &, decltype(_4f) &, decltype(_50) &,
    decltype(_51) &, decltype(_52) &, decltype(_53) &, decltype(_54) &, decltype(_55) &,
    decltype(_56) &, decltype(_57) &, decltype(_58) &, decltype(_59) &, decltype(_5a) &,
    decltype(_5b) &, decltype(_5c) &, decltype(_5d) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _5a,
    _5b, _5c, _5d
);
}
};


template<> struct tie_aggregate_impl<94> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _5a,
    _5b, _5c, _5d, _5e
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &,
    decltype(_47) &, decltype(_48) &, decltype(_49) &, decltype(_4a) &, decltype(_4b) &,
    decltype(_4c) &, decltype(_4d) &, decltype(_4e) &, decltype(_4f) &, decltype(_50) &,
    decltype(_51) &, decltype(_52) &, decltype(_53) &, decltype(_54) &, decltype(_55) &,
    decltype(_56) &, decltype(_57) &, decltype(_58) &, decltype(_59) &, decltype(_5a) &,
    decltype(_5b) &, decltype(_5c) &, decltype(_5d) &, decltype(_5e) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _5a,
    _5b, _5c, _5d, _5e
);
}
};


template<> struct tie_aggregate_impl<95> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _5a,
    _5b, _5c, _5d, _5e, _5f
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &,
    decltype(_47) &, decltype(_48) &, decltype(_49) &, decltype(_4a) &, decltype(_4b) &,
    decltype(_4c) &, decltype(_4d) &, decltype(_4e) &, decltype(_4f) &, decltype(_50) &,
    decltype(_51) &, decltype(_52) &, decltype(_53) &, decltype(_54) &, decltype(_55) &,
    decltype(_56) &, decltype(_57) &, decltype(_58) &, decltype(_59) &, decltype(_5a) &,
    decltype(_5b) &, decltype(_5c) &, decltype(_5d) &, decltype(_5e) &, decltype(_5f) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _5a,
    _5b, _5c, _5d, _5e, _5f
);
}
};


template<> struct tie_aggregate_impl<96> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _5a,
    _5b, _5c, _5d, _5e, _5f, _60
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &,
    decltype(_47) &, decltype(_48) &, decltype(_49) &, decltype(_4a) &, decltype(_4b) &,
    decltype(_4c) &, decltype(_4d) &, decltype(_4e) &, decltype(_4f) &, decltype(_50) &,
    decltype(_51) &, decltype(_52) &, decltype(_53) &, decltype(_54) &, decltype(_55) &,
    decltype(_56) &, decltype(_57) &, decltype(_58) &, decltype(_59) &, decltype(_5a) &,
    decltype(_5b) &, decltype(_5c) &, decltype(_5d) &, decltype(_5e) &, decltype(_5f) &,
    decltype(_60) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _5a,
    _5b, _5c, _5d, _5e, _5f, _60
);
}
};


template<> struct tie_aggregate_impl<97> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _5a,
    _5b, _5c, _5d, _5e, _5f, _60, _61
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &,
    decltype(_47) &, decltype(_48) &, decltype(_49) &, decltype(_4a) &, decltype(_4b) &,
    decltype(_4c) &, decltype(_4d) &, decltype(_4e) &, decltype(_4f) &, decltype(_50) &,
    decltype(_51) &, decltype(_52) &, decltype(_53) &, decltype(_54) &, decltype(_55) &,
    decltype(_56) &, decltype(_57) &, decltype(_58) &, decltype(_59) &, decltype(_5a) &,
    decltype(_5b) &, decltype(_5c) &, decltype(_5d) &, decltype(_5e) &, decltype(_5f) &,
    decltype(_60) &, decltype(_61) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _5a,
    _5b, _5c, _5d, _5e, _5f, _60, _61
);
}
};


template<> struct tie_aggregate_impl<98> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _5a,
    _5b, _5c, _5d, _5e, _5f, _60, _61, _62
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &,
    decltype(_47) &, decltype(_48) &, decltype(_49) &, decltype(_4a) &, decltype(_4b) &,
    decltype(_4c) &, decltype(_4d) &, decltype(_4e) &, decltype(_4f) &, decltype(_50) &,
    decltype(_51) &, decltype(_52) &, decltype(_53) &, decltype(_54) &, decltype(_55) &,
    decltype(_56) &, decltype(_57) &, decltype(_58) &, decltype(_59) &, decltype(_5a) &,
    decltype(_5b) &, decltype(_5c) &, decltype(_5d) &, decltype(_5e) &, decltype(_5f) &,
    decltype(_60) &, decltype(_61) &, decltype(_62) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _5a,
    _5b, _5c, _5d, _5e, _5f, _60, _61, _62
);
}
};


template<> struct tie_aggregate_impl<99> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _5a,
    _5b, _5c, _5d, _5e, _5f, _60, _61, _62, _63
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &,
    decltype(_47) &, decltype(_48) &, decltype(_49) &, decltype(_4a) &, decltype(_4b) &,
    decltype(_4c) &, decltype(_4d) &, decltype(_4e) &, decltype(_4f) &, decltype(_50) &,
    decltype(_51) &, decltype(_52) &, decltype(_53) &, decltype(_54) &, decltype(_55) &,
    decltype(_56) &, decltype(_57) &, decltype(_58) &, decltype(_59) &, decltype(_5a) &,
    decltype(_5b) &, decltype(_5c) &, decltype(_5d) &, decltype(_5e) &, decltype(_5f) &,
    decltype(_60) &, decltype(_61) &, decltype(_62) &, decltype(_63) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _5a,
    _5b, _5c, _5d, _5e, _5f, _60, _61, _62, _63
);
}
};


template<> struct tie_aggregate_impl<100> {
template<typename T> static constexpr auto call(T & x) {
auto & [
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _5a,
    _5b, _5c, _5d, _5e, _5f, _60, _61, _62, _63, _64
] = x;
return parser::tuple<
    decltype(_01) &, decltype(_02) &, decltype(_03) &, decltype(_04) &, decltype(_05) &,
    decltype(_06) &, decltype(_07) &, decltype(_08) &, decltype(_09) &, decltype(_0a) &,
    decltype(_0b) &, decltype(_0c) &, decltype(_0d) &, decltype(_0e) &, decltype(_0f) &,
    decltype(_10) &, decltype(_11) &, decltype(_12) &, decltype(_13) &, decltype(_14) &,
    decltype(_15) &, decltype(_16) &, decltype(_17) &, decltype(_18) &, decltype(_19) &,
    decltype(_1a) &, decltype(_1b) &, decltype(_1c) &, decltype(_1d) &, decltype(_1e) &,
    decltype(_1f) &, decltype(_20) &, decltype(_21) &, decltype(_22) &, decltype(_23) &,
    decltype(_24) &, decltype(_25) &, decltype(_26) &, decltype(_27) &, decltype(_28) &,
    decltype(_29) &, decltype(_2a) &, decltype(_2b) &, decltype(_2c) &, decltype(_2d) &,
    decltype(_2e) &, decltype(_2f) &, decltype(_30) &, decltype(_31) &, decltype(_32) &,
    decltype(_33) &, decltype(_34) &, decltype(_35) &, decltype(_36) &, decltype(_37) &,
    decltype(_38) &, decltype(_39) &, decltype(_3a) &, decltype(_3b) &, decltype(_3c) &,
    decltype(_3d) &, decltype(_3e) &, decltype(_3f) &, decltype(_40) &, decltype(_41) &,
    decltype(_42) &, decltype(_43) &, decltype(_44) &, decltype(_45) &, decltype(_46) &,
    decltype(_47) &, decltype(_48) &, decltype(_49) &, decltype(_4a) &, decltype(_4b) &,
    decltype(_4c) &, decltype(_4d) &, decltype(_4e) &, decltype(_4f) &, decltype(_50) &,
    decltype(_51) &, decltype(_52) &, decltype(_53) &, decltype(_54) &, decltype(_55) &,
    decltype(_56) &, decltype(_57) &, decltype(_58) &, decltype(_59) &, decltype(_5a) &,
    decltype(_5b) &, decltype(_5c) &, decltype(_5d) &, decltype(_5e) &, decltype(_5f) &,
    decltype(_60) &, decltype(_61) &, decltype(_62) &, decltype(_63) &, decltype(_64) &
>(
    _01, _02, _03, _04, _05, _06, _07, _08, _09, _0a, _0b, _0c, _0d, _0e, _0f,
    _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _1a, _1b, _1c, _1d, _1e,
    _1f, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _2a, _2b, _2c, _2d,
    _2e, _2f, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _3a, _3b, _3c,
    _3d, _3e, _3f, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _4a, _4b,
    _4c, _4d, _4e, _4f, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _5a,
    _5b, _5c, _5d, _5e, _5f, _60, _61, _62, _63, _64
);
}
};


}
#line 7 "boost/parser/detail/hl.hpp"
#define BOOST_PARSER_DETAIL_HL_HPP





#include <type_traits>
#include <utility>


namespace boost { namespace parser { namespace detail::hl {




    struct forward
    {
        template<typename T>
        decltype(auto) operator()(T && t)
        {
            return (T &&) t;
        }
    };

    template<typename... Args>
    constexpr auto make_tuple(Args &&... args)
    {



        return hana::make_tuple((Args &&) args...);

    }

    template<typename T, typename U>
    constexpr auto make_pair(T && t, U && u)
    {
        return hl::make_tuple((T &&) t, (U &&) u);
    }

    template<typename Tuple1, typename Tuple2>
    constexpr auto concat(Tuple1 const & t1, Tuple2 const & t2)
    {
#line 56 "boost/parser/detail/hl.hpp"
        return hana::insert_range(t1, hana::size(t1), t2);

    }




    template<typename F, typename Tuple, std::size_t... I>
    constexpr void for_each_impl(
        Tuple const & t, F && f, std::integer_sequence<std::size_t, I...>)
    {
        int _[] = {0, (f(parser::get(t, llong<I>{})), 0)...};
        (void)_;
    }
    template<
        typename F,
        typename Tuple,
        std::size_t... I,
        typename Enable = std::enable_if_t<!std::is_reference_v<Tuple>>>
    constexpr void
    for_each_impl(Tuple && t, F && f, std::integer_sequence<std::size_t, I...>)
    {
        int _[] = {0, (f(std::move(parser::get(t, llong<I>{}))), 0)...};
        (void)_;
    }

    template<typename F, typename... Args>
    constexpr void for_each(tuple<Args...> && t, F && f)
    {
        hl::for_each_impl(
            std::move(t),
            (F &&) f,
            std::make_integer_sequence<std::size_t, sizeof...(Args)>());
    }
    template<typename F, typename... Args>
    constexpr void for_each(tuple<Args...> const & t, F && f)
    {
        hl::for_each_impl(
            t,
            (F &&) f,
            std::make_integer_sequence<std::size_t, sizeof...(Args)>());
    }




    template<int offset, typename F, typename Tuple, std::size_t... I>
    constexpr auto transform_impl(
        Tuple const & t, F && f, std::integer_sequence<std::size_t, I...>)
    {
        return tuple<
            std::decay_t<decltype(f(parser::get(t, llong<I + offset>{})))>...>{
            f(parser::get(t, llong<I + offset>{}))...};
    }
    template<
        int offset,
        typename F,
        typename Tuple,
        std::size_t... I,
        typename Enable = std::enable_if_t<!std::is_reference_v<Tuple>>>
    auto constexpr transform_impl(
        Tuple && t, F && f, std::integer_sequence<std::size_t, I...>)
    {
        return tuple<std::decay_t<decltype(
            f(std::move(parser::get(t, llong<I + offset>{}))))>...>{
            f(std::move(parser::get(t, llong<I + offset>{})))...};
    }

    template<typename F, typename... Args>
    constexpr auto transform(tuple<Args...> && t, F && f)
    {
        return hl::transform_impl<0>(
            std::move(t),
            (F &&) f,
            std::make_integer_sequence<std::size_t, sizeof...(Args)>());
    }
    template<typename F, typename... Args>
    constexpr auto transform(tuple<Args...> const & t, F && f)
    {
        return hl::transform_impl<0>(
            t,
            (F &&) f,
            std::make_integer_sequence<std::size_t, sizeof...(Args)>());
    }




    template<std::size_t I, std::size_t N>
    struct fold_left_dispatch
    {
        template<typename F, typename State, typename... Args>
        constexpr static auto
        call(tuple<Args...> const & t, State && s, F const & f)
        {
            return fold_left_dispatch<I + 1, N>::call(
                t, f((State &&) s, parser::get(t, llong<I>{})), f);
        }
    };
    template<std::size_t I>
    struct fold_left_dispatch<I, I>
    {
        template<typename F, typename State, typename... Args>
        constexpr static auto
        call(tuple<Args...> const & t, State && s, F const & f)
        {
            return (State &&) s;
        }
    };

    template<typename F, typename State, typename... Args>
    constexpr auto fold_left(tuple<Args...> const & t, State && s, F const & f)
    {
        return hl::fold_left_dispatch<0, sizeof...(Args)>::call(
            t, (State &&) s, (F &&) f);
    }




    template<typename... Args>
    constexpr auto size(tuple<Args...> const & t)
    {
        return llong<sizeof...(Args)>{};
    }

    template<typename... Args>
    constexpr auto size_minus_one(tuple<Args...> const & t)
    {
        return llong<sizeof...(Args) - 1>{};
    }




    template<typename T, typename U>
    using comparable = decltype(std::declval<T>() == std::declval<U>());

    struct typesafe_equals
    {
        template<typename T, typename U>
        constexpr bool operator()(T const & t, U const & u)
        {
            if constexpr (detail::is_detected_v<comparable, T, U>) {
                return t == u;
            } else {
                return false;
            }
        }
    };

    template<typename T, typename Tuple, std::size_t... I>
    constexpr bool contains_impl(
        Tuple const & t, T const & x, std::integer_sequence<std::size_t, I...>)
    {
        typesafe_equals eq;
        (void)eq;
        return (eq(parser::get(t, llong<I>{}), x) || ...);
    }

    template<typename T, typename... Args>
    constexpr bool contains(tuple<Args...> & t, T const & x)
    {
        return contains_impl(
            t, x, std::make_integer_sequence<std::size_t, sizeof...(Args)>());
    }




    template<typename Arg, typename... Args>
    constexpr decltype(auto) front(tuple<Arg, Args...> & t)
    {
        return parser::get(t, llong<0>{});
    }
    template<typename Arg, typename... Args>
    constexpr decltype(auto) front(tuple<Arg, Args...> const & t)
    {
        return parser::get(t, llong<0>{});
    }
    template<typename Arg, typename... Args>
    constexpr decltype(auto) back(tuple<Arg, Args...> & t)
    {
        return parser::get(t, llong<sizeof...(Args)>{});
    }
    template<typename Arg, typename... Args>
    constexpr decltype(auto) back(tuple<Arg, Args...> const & t)
    {
        return parser::get(t, llong<sizeof...(Args)>{});
    }




    template<typename Arg, typename... Args>
    constexpr auto drop_front(tuple<Arg, Args...> && t)
    {
        return hl::transform_impl<1>(
            std::move(t),
            forward{},
            std::make_integer_sequence<std::size_t, sizeof...(Args)>());
    }
    template<typename Arg, typename... Args>
    constexpr auto drop_front(tuple<Arg, Args...> const & t)
    {
        return hl::transform_impl<1>(
            t,
            forward{},
            std::make_integer_sequence<std::size_t, sizeof...(Args)>());
    }




    template<typename Arg, typename... Args>
    constexpr auto drop_back(tuple<Arg, Args...> && t)
    {
        return hl::transform_impl<0>(
            std::move(t),
            forward{},
            std::make_integer_sequence<std::size_t, sizeof...(Args)>());
    }
    template<typename Arg, typename... Args>
    constexpr auto drop_back(tuple<Arg, Args...> const & t)
    {
        return hl::transform_impl<0>(
            t,
            forward{},
            std::make_integer_sequence<std::size_t, sizeof...(Args)>());
    }




    template<typename T, typename U>
    constexpr decltype(auto) first(tuple<T, U> & t)
    {
        return parser::get(t, llong<0>{});
    }
    template<typename T, typename U>
    constexpr decltype(auto) first(tuple<T, U> const & t)
    {
        return parser::get(t, llong<0>{});
    }
    template<typename T, typename U>
    constexpr decltype(auto) second(tuple<T, U> & t)
    {
        return parser::get(t, llong<1>{});
    }
    template<typename T, typename U>
    constexpr decltype(auto) second(tuple<T, U> const & t)
    {
        return parser::get(t, llong<1>{});
    }




    template<std::size_t I, typename... Tuples>
    constexpr decltype(auto) make_zip_elem(Tuples const &... ts)
    {
        return hl::make_tuple(parser::get(ts, llong<I>{})...);
    }

    template<std::size_t... I, typename... Tuples>
    constexpr auto zip_impl(std::index_sequence<I...>, Tuples const &... ts)
    {
        return hl::make_tuple(hl::make_zip_elem<I>(ts...)...);
    }

    template<typename T>
    struct tuplesize;
    template<typename... Args>
    struct tuplesize<tuple<Args...>>
    {
        constexpr static std::size_t value = sizeof...(Args);
    };

    template<typename Tuple, typename... Tuples>
    constexpr auto zip(Tuple const & t, Tuples const &... ts)
    {
        return hl::zip_impl(
            std::make_integer_sequence<
                std::size_t,
                tuplesize<std::remove_reference_t<Tuple>>::value>(),
            t,
            ts...);
    }




    template<typename... Args, typename T>
    constexpr auto append(tuple<Args...> && t, T && x)
    {



        return hana::append(std::move(t), (T &&) x);

    }
    template<typename... Args, typename T>
    constexpr auto append(tuple<Args...> const & t, T && x)
    {



        return hana::append(t, (T &&) x);

    }




    template<typename... Args, typename T>
    constexpr auto prepend(tuple<Args...> && t, T && x)
    {



        return hana::prepend(std::move(t), (T &&) x);

    }
    template<typename... Args, typename T>
    constexpr auto prepend(tuple<Args...> const & t, T && x)
    {



        return hana::prepend(t, (T &&) x);

    }

}}}
#line 12 "boost/parser/detail/printing.hpp"
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <variant>

#include <cctype>


namespace boost { namespace parser { namespace detail {

    template<typename Context>
    decltype(auto) _indent(Context const & context);

    template<typename Char>
    std::ostream & print_char(std::ostream & os, Char c)
    {
        if constexpr (





            false

        ) {
            os << char(c);
        } else {
            os << c;
        }
        return os;
    }

    enum { parser_component_limit = 4 };

    template<
        typename Context,
        typename Parser,
        typename DelimiterParser,
        typename MinType,
        typename MaxType>
    void print_parser(
        Context const & context,
        repeat_parser<Parser, DelimiterParser, MinType, MaxType> const & parser,
        std::ostream & os,
        int components = 0);

    template<typename Context, typename Parser>
    void print_parser(
        Context const & context,
        opt_parser<Parser> const & parser,
        std::ostream & os,
        int components = 0);

    template<typename Context, typename ParserTuple>
    void print_parser(
        Context const & context,
        or_parser<ParserTuple> const & parser,
        std::ostream & os,
        int components = 0);

    template<
        typename Context,
        typename ParserTuple,
        typename BacktrackingTuple,
        typename CombiningGroups>
    void print_parser(
        Context const & context,
        seq_parser<ParserTuple, BacktrackingTuple, CombiningGroups> const &
            parser,
        std::ostream & os,
        int components = 0);

    template<typename Context, typename Parser, typename Action>
    void print_parser(
        Context const & context,
        action_parser<Parser, Action> const & parser,
        std::ostream & os,
        int components = 0);

    template<typename Context, typename Parser>
    void print_parser(
        Context const & context,
        omit_parser<Parser> const & parser,
        std::ostream & os,
        int components = 0);

    template<typename Context, typename Parser>
    void print_parser(
        Context const & context,
        raw_parser<Parser> const & parser,
        std::ostream & os,
        int components = 0);
#line 116 "boost/parser/detail/printing.hpp"
    template<typename Context, typename Parser>
    void print_parser(
        Context const & context,
        lexeme_parser<Parser> const & parser,
        std::ostream & os,
        int components = 0);

    template<typename Context, typename Parser>
    void print_parser(
        Context const & context,
        no_case_parser<Parser> const & parser,
        std::ostream & os,
        int components = 0);

    template<typename Context, typename Parser, typename SkipParser>
    void print_parser(
        Context const & context,
        skip_parser<Parser, SkipParser> const & parser,
        std::ostream & os,
        int components = 0);

    template<typename Context, typename Parser, bool FailOnMatch>
    void print_parser(
        Context const & context,
        expect_parser<Parser, FailOnMatch> const & parser,
        std::ostream & os,
        int components = 0);

    template<
        typename Context,
        bool UseCallbacks,
        typename Parser,
        typename Attribute,
        typename LocalState,
        typename ParamsTuple>
    void print_parser(
        Context const & context,
        rule_parser<
            UseCallbacks,
            Parser,
            Attribute,
            LocalState,
            ParamsTuple> const & parser,
        std::ostream & os,
        int components = 0);

    template<typename Context, typename T>
    void print_parser(
        Context const & context,
        symbol_parser<T> const & parser,
        std::ostream & os,
        int components = 0);

    template<typename Context, typename Predicate>
    void print_parser(
        Context const & context,
        eps_parser<Predicate> const & parser,
        std::ostream & os,
        int components = 0);

    template<typename Context>
    void print_parser(
        Context const & context,
        eps_parser<nope> const & parser,
        std::ostream & os,
        int components = 0);

    template<typename Context>
    void print_parser(
        Context const & context,
        eoi_parser const & parser,
        std::ostream & os,
        int components = 0);

    template<typename Context, typename Atribute>
    void print_parser(
        Context const & context,
        attr_parser<Atribute> const & parser,
        std::ostream & os,
        int components = 0);

    template<typename Context, typename Expected, typename AttributeType>
    void print_parser(
        Context const & context,
        char_parser<Expected, AttributeType> const & parser,
        std::ostream & os,
        int components = 0);

    template<typename Context>
    void print_parser(
        Context const & context,
        digit_parser const & parser,
        std::ostream & os,
        int components = 0);

    template<typename Context>
    void print_parser(
        Context const & context,
        char_subrange_parser<hex_digit_subranges> const & parser,
        std::ostream & os,
        int components = 0);

    template<typename Context>
    void print_parser(
        Context const & context,
        char_subrange_parser<control_subranges> const & parser,
        std::ostream & os,
        int components = 0);

    template<typename Context>
    void print_parser(
        Context const & context,
        char_set_parser<punct_chars> const & parser,
        std::ostream & os,
        int components = 0);

    template<typename Context>
    void print_parser(
        Context const & context,
        char_set_parser<lower_case_chars> const & parser,
        std::ostream & os,
        int components = 0);

    template<typename Context>
    void print_parser(
        Context const & context,
        char_set_parser<upper_case_chars> const & parser,
        std::ostream & os,
        int components = 0);

    template<typename Context, typename Expected, typename AttributeType>
    void print_parser(
        Context const & context,
        omit_parser<char_parser<Expected, AttributeType>> const & parser,
        std::ostream & os,
        int components = 0);

    template<typename Context, typename StrIter, typename StrSentinel>
    void print_parser(
        Context const & context,
        string_parser<StrIter, StrSentinel> const & parser,
        std::ostream & os,
        int components = 0);

    template<typename Context, typename StrIter, typename StrSentinel>
    void print_parser(
        Context const & context,
        omit_parser<string_parser<StrIter, StrSentinel>> const & parser,
        std::ostream & os,
        int components = 0);

    template<typename Context, bool NewlinesOnly, bool NoNewlines>
    void print_parser(
        Context const & context,
        ws_parser<NewlinesOnly, NoNewlines> const & parser,
        std::ostream & os,
        int components = 0);

    template<typename Context>
    void print_parser(
        Context const & context,
        bool_parser const & parser,
        std::ostream & os,
        int components = 0);

    template<
        typename Context,
        typename T,
        int Radix,
        int MinDigits,
        int MaxDigits,
        typename Expected>
    void print_parser(
        Context const & context,
        uint_parser<T, Radix, MinDigits, MaxDigits, Expected> const & parser,
        std::ostream & os,
        int components = 0);

    template<
        typename Context,
        typename T,
        int Radix,
        int MinDigits,
        int MaxDigits,
        typename Expected>
    void print_parser(
        Context const & context,
        int_parser<T, Radix, MinDigits, MaxDigits, Expected> const & parser,
        std::ostream & os,
        int components = 0);

    template<typename Context, typename T>
    void print_parser(
        Context const & context,
        float_parser<T> const & parser,
        std::ostream & os,
        int components = 0);

    template<typename Context>
    void print_parser(
        Context const & context,
        float_parser<float> const & parser,
        std::ostream & os,
        int components = 0);

    template<typename Context>
    void print_parser(
        Context const & context,
        float_parser<double> const & parser,
        std::ostream & os,
        int components = 0);

    template<typename Context, typename SwitchValue, typename OrParser>
    void print_parser(
        Context const & context,
        switch_parser<SwitchValue, OrParser> const & parser,
        std::ostream & os,
        int components = 0);

    enum { trace_indent_factor = 2 };

    inline void trace_indent(int indent)
    {
        for (int i = 0, end = trace_indent_factor * indent; i != end; ++i) {
            std::cout << ' ';
        }
    }

    template<typename Iter, typename Sentinel, int SizeofValueType>
    struct trace_input_impl
    {
        static void call(
            std::ostream & os,
            Iter first_,
            Sentinel last_,
            bool quote,
            int64_t trace_input_cps)
        {
            auto utf8 = boost::parser::subrange(first_, last_) | text::as_utf8;
            auto first = utf8.begin();
            auto last = utf8.end();
            if (quote)
                os << '"';
            for (int64_t i = 0; i < trace_input_cps && first != last;
                 ++i, ++first) {
                detail::print_char(os, *first);
            }
            if (quote)
                os << '"';
        }
    };

    template<typename Iter, typename Sentinel>
    struct trace_input_impl<Iter, Sentinel, 1>
    {
        static void call(
            std::ostream & os,
            Iter first_,
            Sentinel last_,
            bool quote,
            int64_t trace_input_cps)
        {
            auto r = boost::parser::subrange(first_, last_);
            auto r_unpacked =
                detail::text::unpack_iterator_and_sentinel(first_, last_);
            auto utf32 = r | text::as_utf32;
            auto first = utf32.begin();
            auto const last = utf32.end();
            for (int64_t i = 0; i < trace_input_cps && first != last; ++i) {
                ++first;
            }
            if (quote)
                os << '"';
            auto first_repacked = r_unpacked.repack(first.base());
            for (Iter it = first_, end = first_repacked; it != end; ++it) {
                detail::print_char(os, *it);
            }
            if (quote)
                os << '"';
        }
    };

    template<typename Iter, typename Sentinel>
    inline void trace_input(
        std::ostream & os,
        Iter first,
        Sentinel last,
        bool quote = true,
        int64_t trace_input_cps = 8)
    {
        trace_input_impl<Iter, Sentinel, sizeof(*first)>::call(
            os, first, last, quote, trace_input_cps);
    }

    template<typename Iter, typename Sentinel>
    inline void trace_begin_match(
        Iter first, Sentinel last, int indent, std::string_view name)
    {
        detail::trace_indent(indent);
        std::cout << "[begin " << name << "; input=";
        detail::trace_input(std::cout, first, last);
        std::cout << "]" << std::endl;
    }

    template<typename Iter, typename Sentinel>
    inline void trace_end_match(
        Iter first, Sentinel last, int indent, std::string_view name)
    {
        detail::trace_indent(indent);
        std::cout << "[end " << name << "; input=";
        detail::trace_input(std::cout, first, last);
        std::cout << "]" << std::endl;
    }

    template<typename Iter, typename Sentinel, typename Context>
    void trace_prefix(
        Iter first,
        Sentinel last,
        Context const & context,
        std::string_view name)
    {
        int & indent = detail::_indent(context);
        detail::trace_begin_match(first, last, indent, name);
        ++indent;
    }

    template<typename Iter, typename Sentinel, typename Context>
    void trace_suffix(
        Iter first,
        Sentinel last,
        Context const & context,
        std::string_view name)
    {
        int & indent = detail::_indent(context);
        --indent;
        detail::trace_end_match(first, last, indent, name);
    }

    template<typename T>
    using streamable =
        decltype(std::declval<std::ostream &>() << std::declval<T const &>());

    template<typename T, bool Streamable = is_detected_v<streamable, T>>
    struct printer
    {
        std::ostream & operator()(std::ostream & os, T const &)
        {
            return os << "<<unprintable-value>>";
        }
    };

    template<typename T>
    void print_printable(std::ostream & os, T const & x)
    {
        os << x;
    }

    inline void print_printable(std::ostream & os, char c)
    {
        if (std::isprint(c)) {
            os << "'" << c << "'";
        } else {
            os << "'\\x" << std::hex << std::setw(2) << std::setfill('0')
               << (uint32_t)c << "'";
        }
    }

    inline void print_printable(std::ostream & os, char32_t c)
    {
        if (c < 256) {
            os << "U";
            detail::print_printable(os, (char)c);
        } else {
            os << "U'\\U" << std::hex << std::setw(8) << std::setfill('0')
               << (uint32_t)c << "'";
        }
    }

    template<typename T>
    struct printer<T, true>
    {
        std::ostream & operator()(std::ostream & os, T const & x)
        {
            detail::print_printable(os, x);
            return os;
        }
    };

    template<typename T>
    constexpr bool is_variant_v = enable_variant<T>;

    template<typename Attribute>
    inline void print(std::ostream & os, Attribute const & attr)
    {
        using just_attribute =
            std::remove_cv_t<std::remove_reference_t<Attribute>>;
        if constexpr (is_tuple<just_attribute>{}) {
            os << "(";
            bool first = false;
            hl::for_each(attr, [&](auto const & a) {
                if (first)
                    os << ", ";
                detail::print(os, a);
                first = false;
            });
            os << ")\n";
        } else if constexpr (is_optional_v<just_attribute>) {
            if (!attr)
                os << "<<empty>>";
            else
                detail::print(os, *attr);
        } else if constexpr (is_variant_v<just_attribute>) {
            os << "<<variant>>";
        } else {
            printer<just_attribute>{}(os, attr);
        }
    }

    template<typename Attribute>
    inline void print_attribute(Attribute const & attr, int indent)
    {
        detail::trace_indent(indent);
        std::cout << "attribute: ";
        detail::print(std::cout, attr);
        std::cout << "\n";
    }

    inline void print_attribute(nope const &, int) {}

    constexpr inline bool do_trace(flags f)
    {
        return (uint32_t(f) & uint32_t(flags::trace)) == uint32_t(flags::trace);
    }

    template<typename Context, typename T>
    auto resolve(Context const & context, T const & x);

    template<typename Context>
    auto resolve(Context const &, nope n);

    template<
        typename Iter,
        typename Sentinel,
        typename Context,
        typename Attribute>
    struct scoped_trace_t
    {
        scoped_trace_t(
            Iter & first,
            Sentinel last,
            Context const & context,
            flags f,
            Attribute const & attr,
            std::string name) :
            initial_first_(first),
            first_(first),
            last_(last),
            context_(context),
            flags_(f),
            attr_(attr),
            name_(std::move(name))
        {
            if (!detail::do_trace(flags_))
                return;
            detail::trace_prefix(first_, last_, context_, name_);
        }

        ~scoped_trace_t()
        {
            if (!detail::do_trace(flags_))
                return;
            detail::trace_indent(detail::_indent(context_));
            if (*context_.pass_) {
                std::cout << "matched ";
                detail::trace_input(std::cout, initial_first_, first_);
                std::cout << "\n";
                detail::print_attribute(
                    detail::resolve(context_, attr_),
                    detail::_indent(context_));
            } else {
                std::cout << "no match\n";
            }
            detail::trace_suffix(first_, last_, context_, name_);
        }

        Iter initial_first_;
        Iter & first_;
        Sentinel last_;
        Context const & context_;
        flags flags_;
        Attribute const & attr_;
        std::string name_;
    };

    template<
        typename Parser,
        typename Iter,
        typename Sentinel,
        typename Context,
        typename Attribute>
    auto scoped_trace(
        Parser const & parser,
        Iter & first,
        Sentinel last,
        Context const & context,
        flags f,
        Attribute const & attr)
    {
        std::stringstream oss;
        if (detail::do_trace(f))
            detail::print_parser(context, parser, oss);
        return scoped_trace_t<Iter, Sentinel, Context, Attribute>(
            first, last, context, f, attr, oss.str());
    }

    template<typename Context, typename Attribute>
    auto final_trace(Context const & context, flags f, Attribute const & attr)
    {
        if (!detail::do_trace(f))
            return;

        std::cout << "--------------------\n";
        if (*context.pass_) {
            std::cout << "parse succeeded\n";
            detail::print_attribute(detail::resolve(context, attr), 0);
        } else {
            std::cout << "parse failed\n";
        }
        std::cout << "--------------------" << std::endl;
    }

}}}
#line 7 "boost/parser/detail/text/algorithm.hpp"
#define BOOST_PARSER_DETAIL_TEXT_ALGORITHM_HPP
#line 7 "boost/parser/detail/text/detail/sentinel_tag.hpp"
#define BOOST_PARSER_DETAIL_TEXT_DETAIL_SENTINEL_TAG_HPP




namespace boost::parser::detail { namespace text { namespace detail {
    struct sentinel_tag
    {};
    struct non_sentinel_tag
    {};
}}}
#line 15 "boost/parser/detail/text/algorithm.hpp"
#include <cstddef>
#include <iterator>
#include <utility>


namespace boost::parser::detail { namespace text {

    namespace detail {
        template<typename Iter>
        std::ptrdiff_t distance(Iter first, Iter last, non_sentinel_tag)
        {
            return std::distance(first, last);
        }

        template<typename Iter, typename Sentinel>
        std::ptrdiff_t distance(Iter first, Sentinel last, sentinel_tag)
        {
            std::ptrdiff_t retval = 0;
            while (first != last) {
                ++retval;
                ++first;
            }
            return retval;
        }
    }



    template<typename Iter, typename Sentinel>
    std::ptrdiff_t distance(Iter first, Sentinel last)
    {
        return detail::distance(
            first,
            last,
            typename std::conditional<
                std::is_same<Iter, Sentinel>::value,
                detail::non_sentinel_tag,
                detail::sentinel_tag>::type());
    }



    template<typename BidiIter, typename Sentinel, typename T>
    BidiIter find(BidiIter first, Sentinel last, T const & x)
    {
        while (first != last) {
            if (*first == x)
                return first;
            ++first;
        }
        return first;
    }



    template<typename BidiIter, typename Sentinel, typename T>
    BidiIter find_not(BidiIter first, Sentinel last, T const & x)
    {
        while (first != last) {
            if (*first != x)
                return first;
            ++first;
        }
        return first;
    }



    template<typename BidiIter, typename Sentinel, typename Pred>
    BidiIter find_if(BidiIter first, Sentinel last, Pred p)
    {
        while (first != last) {
            if (p(*first))
                return first;
            ++first;
        }
        return first;
    }



    template<typename BidiIter, typename Sentinel, typename Pred>
    BidiIter find_if_not(BidiIter first, Sentinel last, Pred p)
    {
        while (first != last) {
            if (!p(*first))
                return first;
            ++first;
        }
        return first;
    }



    template<typename BidiIter, typename T>
    BidiIter find_backward(BidiIter first, BidiIter last, T const & x)
    {
        auto it = last;
        while (it != first) {
            if (*--it == x)
                return it;
        }
        return last;
    }



    template<typename BidiIter, typename T>
    BidiIter find_not_backward(BidiIter first, BidiIter last, T const & x)
    {
        auto it = last;
        while (it != first) {
            if (*--it != x)
                return it;
        }
        return last;
    }



    template<typename BidiIter, typename Pred>
    BidiIter find_if_backward(BidiIter first, BidiIter last, Pred p)
    {
        auto it = last;
        while (it != first) {
            if (p(*--it))
                return it;
        }
        return last;
    }



    template<typename BidiIter, typename Pred>
    BidiIter find_if_not_backward(BidiIter first, BidiIter last, Pred p)
    {
        auto it = last;
        while (it != first) {
            if (!p(*--it))
                return it;
        }
        return last;
    }


    template<typename Iter, typename Sentinel = Iter>
    using foreach_subrange_range =
        boost::parser::subrange<Iter, Sentinel>;





    template<typename FwdIter, typename Sentinel, typename Func>
    Func foreach_subrange(FwdIter first, Sentinel last, Func f)
    {
        while (first != last) {
            auto const & x = *first;
            auto const next = boost::parser::detail::text::find_not(first, last, x);
            if (first != next) {
                f(boost::parser::detail::text::foreach_subrange_range<FwdIter, Sentinel>(
                    first, next));
            }
            first = next;
        }
        return f;
    }





    template<typename FwdIter, typename Sentinel, typename Func, typename Proj>
    Func foreach_subrange(FwdIter first, Sentinel last, Func f, Proj proj)
    {
        using value_type = typename std::iterator_traits<FwdIter>::value_type;
        while (first != last) {
            auto const & x = proj(*first);
            auto const next = boost::parser::detail::text::find_if_not(
                first, last, [&x, proj](const value_type & element) {
                    return proj(element) == x;
                });
            if (first != next) {
                f(boost::parser::detail::text::foreach_subrange_range<FwdIter, Sentinel>(
                    first, next));
            }
            first = next;
        }
        return f;
    }




    template<typename FwdIter, typename Sentinel, typename T, typename Func>
    Func foreach_subrange_of(FwdIter first, Sentinel last, T const & x, Func f)
    {
        while (first != last) {
            first = boost::parser::detail::text::find(first, last, x);
            auto const next = boost::parser::detail::text::find_not(first, last, x);
            if (first != next) {
                f(boost::parser::detail::text::foreach_subrange_range<FwdIter, Sentinel>(
                    first, next));
            }
            first = next;
        }
        return f;
    }




    template<typename FwdIter, typename Sentinel, typename Pred, typename Func>
    Func foreach_subrange_if(FwdIter first, Sentinel last, Pred p, Func f)
    {
        while (first != last) {
            first = boost::parser::detail::text::find_if(first, last, p);
            auto const next = boost::parser::detail::text::find_if_not(first, last, p);
            if (first != next) {
                f(boost::parser::detail::text::foreach_subrange_range<FwdIter, Sentinel>(
                    first, next));
            }
            first = next;
        }
        return f;
    }


    template<typename Iter, typename Sentinel, typename Pred>
    bool all_of(Iter first, Sentinel last, Pred p)
    {
        for (; first != last; ++first) {
            if (!p(*first))
                return false;
        }
        return true;
    }


    template<
        typename Iter1,
        typename Sentinel1,
        typename Iter2,
        typename Sentinel2>
    bool equal(Iter1 first1, Sentinel1 last1, Iter2 first2, Sentinel2 last2)
    {
        for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
            if (*first1 != *first2)
                return false;
        }
        return first1 == last1 && first2 == last2;
    }


    template<
        typename Iter1,
        typename Sentinel1,
        typename Iter2,
        typename Sentinel2>
    std::pair<Iter1, Iter2>
    mismatch(Iter1 first1, Sentinel1 last1, Iter2 first2, Sentinel2 last2)
    {
        for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
            if (*first1 != *first2)
                break;
        }
        return {first1, first2};
    }




    template<
        typename Iter1,
        typename Sentinel1,
        typename Iter2,
        typename Sentinel2>
    int lexicographical_compare_three_way(
        Iter1 first1, Sentinel1 last1, Iter2 first2, Sentinel2 last2)
    {
        auto const iters = boost::parser::detail::text::mismatch(first1, last1, first2, last2);
        if (iters.first == last1) {
            if (iters.second == last2)
                return 0;
            else
                return -1;
        } else if (iters.second == last2) {
            return 1;
        } else if (*iters.first < *iters.second) {
            return -1;
        } else {
            return 1;
        }
    }


    template<typename Iter>
    using search_result = boost::parser::subrange<Iter>;


    template<
        typename Iter1,
        typename Sentinel1,
        typename Iter2,
        typename Sentinel2>
    search_result<Iter1>
    search(Iter1 first1, Sentinel1 last1, Iter2 first2, Sentinel2 last2)
    {
        if (first1 == last1 || first2 == last2)
            return {first1, first1};

        if (detail::next(first2) == last2) {
            auto const it = parser::detail::text::find(first1, last1, *first2);
            if (it == last1)
                return {it, it};
            return {it, detail::next(it)};
        }

        auto it = first1;
        for (;;) {
            first1 = parser::detail::text::find(first1, last1, *first2);

            if (first1 == last1)
                return {first1, first1};

            auto it2 = detail::next(first2);
            it = first1;
            if (++it == last1)
                return {it, it};

            while (*it == *it2) {
                if (++it2 == last2)
                    return {first1, ++it};
                if (++it == last1)
                    return {it, it};
            }

            ++first1;
        }

        return {first1, first1};
    }


    template<
        typename Iter1,
        typename Sentinel1,
        typename Iter2,
        typename Sentinel2,
        typename Pred>
    Iter1 find_first_of(
        Iter1 first1, Sentinel1 last1, Iter2 first2, Sentinel2 last2, Pred pred)
    {
        for (; first1 != last1; ++first1) {
            for (auto it = first2; it != last2; ++it) {
                if (pred(*first1, *it))
                    return first1;
            }
        }
        return first1;
    }

}}
#line 10 "boost/parser/error_handling.hpp"
#include <array>
#include <functional>
#include <iostream>
#include <sstream>


namespace boost { namespace parser {

    namespace detail {



        inline constexpr std::array<int, 7> eol_cps = {
            {0x000a, 0x000b, 0x000c, 0x000d, 0x0085, 0x2028, 0x2029}};

        inline constexpr int eol_cp_mask =
            0x000a | 0x000b | 0x000c | 0x000d | 0x0085 | 0x2028 | 0x2029;
    }



    template<typename Iter>
    line_position<Iter> find_line_position(Iter first, Iter it)
    {
        bool prev_cr = false;
        auto retval = line_position<Iter>{first, 0, 0};
        for (Iter pos = first; pos != it; ++pos) {
            auto const c = *pos;
            bool const found =
                (c & detail::eol_cp_mask) == c &&
                std::find(detail::eol_cps.begin(), detail::eol_cps.end(), c) !=
                    detail::eol_cps.end();
            if (found) {
                retval.line_start = std::next(pos);
                retval.column_number = 0;
            } else {
                ++retval.column_number;
            }
            if (found && (!prev_cr || c != 0x000a))
                ++retval.line_number;
            prev_cr = c == 0x000d;
        }
        return retval;
    }



    template<typename Iter, typename Sentinel>
    Iter find_line_end(Iter it, Sentinel last)
    {
        return parser::detail::text::find_if(it, last, [](auto c) {
            return (c & detail::eol_cp_mask) == c &&
                   std::find(
                       detail::eol_cps.begin(), detail::eol_cps.end(), c) !=
                       detail::eol_cps.end();
        });
    }

    template<typename Iter, typename Sentinel>
    std::ostream & write_formatted_message(
        std::ostream & os,
        std::string_view filename,
        Iter first,
        Iter it,
        Sentinel last,
        std::string_view message,
        int64_t preferred_max_line_length,
        int64_t max_after_caret)
    {
        if (!filename.empty())
            os << filename << ':';
        auto const position = parser::find_line_position(first, it);
        os << (position.line_number + 1) << ':' << position.column_number
           << ": " << message << " here";
        if (it == last)
            os << " (end of input)";
        os << ":\n";

        std::string underlining(std::distance(position.line_start, it), ' ');
        detail::trace_input(os, position.line_start, it, false, 1u << 31);
        if (it == last) {
            os << '\n' << underlining << "^\n";
            return os;
        }

        underlining += '^';

        int64_t const limit = (std::max)(
            preferred_max_line_length,
            (int64_t)underlining.size() + max_after_caret);

        int64_t i = (int64_t)underlining.size();
        auto const line_end = parser::find_line_end(std::next(it), last);
        detail::trace_input(os, it, line_end, false, limit - i);

        os << '\n' << underlining << '\n';

        return os;
    }
#line 136 "boost/parser/error_handling.hpp"
    template<typename Iter, typename Sentinel>
    std::ostream & write_formatted_expectation_failure_error_message(
        std::ostream & os,
        std::string_view filename,
        Iter first,
        Sentinel last,
        parse_error<Iter> const & e,
        int64_t preferred_max_line_length,
        int64_t max_after_caret)
    {
        std::string message = "error: Expected ";
        message += e.what();
        return parser::write_formatted_message(
            os,
            filename,
            first,
            e.iter,
            last,
            message,
            preferred_max_line_length,
            max_after_caret);
    }
#line 181 "boost/parser/error_handling.hpp"
    struct callback_error_handler
    {
        using callback_type = std::function<void(std::string const &)>;

        callback_error_handler() {}
        callback_error_handler(
            callback_type error,
            callback_type warning = callback_type(),
            std::string_view filename = "") :
            error_(error), warning_(warning), filename_(filename)
        {}
#line 204 "boost/parser/error_handling.hpp"
        template<typename Iter, typename Sentinel>
        error_handler_result
        operator()(Iter first, Sentinel last, parse_error<Iter> const & e) const
        {
            if (error_) {
                std::stringstream ss;
                parser::write_formatted_expectation_failure_error_message(
                    ss, filename_, first, last, e);
                error_(ss.str());
            }
            return error_handler_result::fail;
        }

        template<typename Context, typename Iter>
        void diagnose(
            diagnostic_kind kind,
            std::string_view message,
            Context const & context,
            Iter it) const
        {
            callback_type const & cb =
                kind == diagnostic_kind::error ? error_ : warning_;
            if (!cb)
                return;
            std::stringstream ss;
            parser::write_formatted_message(
                ss,
                filename_,
                parser::_begin(context),
                it,
                parser::_end(context),
                message);
            cb(ss.str());
        }

        template<typename Context>
        void diagnose(
            diagnostic_kind kind,
            std::string_view message,
            Context const & context) const
        {
            diagnose(kind, message, context, parser::_where(context).begin());
        }

        callback_type error_;
        callback_type warning_;
        std::string filename_;
    };



    struct rethrow_error_handler
    {
        template<typename Iter, typename Sentinel>
        error_handler_result
        operator()(Iter first, Sentinel last, parse_error<Iter> const & e) const
        {
            return error_handler_result::rethrow;
        }

        template<typename Context, typename Iter>
        void diagnose(
            diagnostic_kind kind,
            std::string_view message,
            Context const & context,
            Iter it) const
        {}

        template<typename Context>
        void diagnose(
            diagnostic_kind kind,
            std::string_view message,
            Context const & context) const
        {}
    };




    template<typename Iter, typename Sentinel>
    error_handler_result default_error_handler::operator()(
        Iter first, Sentinel last, parse_error<Iter> const & e) const
    {
        parser::write_formatted_expectation_failure_error_message(
            std::cerr, "", first, last, e);
        return error_handler_result::fail;
    }

    template<typename Context, typename Iter>
    void default_error_handler::diagnose(
        diagnostic_kind kind,
        std::string_view message,
        Context const & context,
        Iter it) const
    {
        parser::write_formatted_message(
            std::cerr,
            "",
            parser::_begin(context),
            it,
            parser::_end(context),
            message);
    }

    template<typename Context>
    void default_error_handler::diagnose(
        diagnostic_kind kind,
        std::string_view message,
        Context const & context) const
    {
        diagnose(kind, message, context, parser::_where(context).begin());
    }

    template<typename Iter, typename Sentinel>
    error_handler_result stream_error_handler::operator()(
        Iter first, Sentinel last, parse_error<Iter> const & e) const
    {
        std::ostream * os = err_os_;
        if (!os)
            os = &std::cerr;
        parser::write_formatted_expectation_failure_error_message(
            *os, filename_, first, last, e);
        return error_handler_result::fail;
    }

    template<typename Context, typename Iter>
    void stream_error_handler::diagnose(
        diagnostic_kind kind,
        std::string_view message,
        Context const & context,
        Iter it) const
    {
        std::ostream * os = kind == diagnostic_kind::error ? err_os_ : warn_os_;
        if (!os)
            os = &std::cerr;
        parser::write_formatted_message(
            *os,
            filename_,
            parser::_begin(context),
            it,
            parser::_end(context),
            message);
    }

    template<typename Context>
    void stream_error_handler::diagnose(
        diagnostic_kind kind,
        std::string_view message,
        Context const & context) const
    {
        diagnose(kind, message, context, parser::_where(context).begin());
    }

}}
#line 13 "boost/parser/detail/numeric.hpp"
#define BOOST_PARSER_DETAIL_NUMERIC_HPP

#include <cmath>


namespace boost { namespace parser { namespace detail_spirit_x3 {

    struct unused_type{};




    template <typename TargetChar, typename SourceChar>
    TargetChar cast_char(SourceChar ch)
    {
        if (std::is_signed_v<TargetChar> != std::is_signed_v<SourceChar>)
        {
            if (std::is_signed<SourceChar>::value)
            {

                typedef std::make_unsigned_t<SourceChar> USourceChar;
                return TargetChar(USourceChar(ch));
            }
            else
            {

                typedef std::make_signed_t<SourceChar> SSourceChar;
                return TargetChar(SSourceChar(ch));
            }
        }
        else
        {

            return TargetChar(ch);
        }
    }
#line 54 "boost/parser/detail/numeric.hpp"
    inline constexpr int log2_table[] = {
        0,       0,       1000000, 1584960, 2000000, 2321920, 2584960, 2807350,
        3000000, 3169920, 3321920, 3459430, 3584960, 3700430, 3807350, 3906890,
        4000000, 4087460, 4169920, 4247920, 4321920, 4392310, 4459430, 4523560,
        4584960, 4643850, 4700430, 4754880, 4807350, 4857980, 4906890, 4954190,
        5000000, 5044390, 5087460, 5129280, 5169925};

    template<typename T, unsigned Radix>
    struct digits_traits
    {
        static_assert(std::numeric_limits<T>::radix == 2, "");
        constexpr static int value =
            int((std::numeric_limits<T>::digits * 1000000) / log2_table[Radix]);
    };

    template <typename T>
    struct digits_traits<T, 10>
    {
        static int constexpr value = std::numeric_limits<T>::digits10;
    };

    template<unsigned Radix>
    struct radix_traits
    {
        template <typename Char>
        inline static bool is_valid(Char ch)
        {
            if (Radix <= 10)
                return (ch >= '0' && ch <= static_cast<Char>('0' + Radix -1));
            return (ch >= '0' && ch <= '9')
                || (ch >= 'a' && ch <= static_cast<Char>('a' + Radix -10 -1))
                || (ch >= 'A' && ch <= static_cast<Char>('A' + Radix -10 -1));
        }

        template <typename Char>
        inline static unsigned digit(Char ch)
        {
            if (Radix <= 10 || (ch >= '0' && ch <= '9'))
                return ch - '0';
            return std::tolower(detail_spirit_x3::cast_char<char>(ch)) - 'a' + 10;
        }
    };


    template <unsigned Radix>
    struct positive_accumulator
    {
        template <typename T, typename Char>
        inline static void add(T& n, Char ch, std::false_type)
        {
            const int digit = radix_traits<Radix>::digit(ch);
            n = n * T(Radix) + T(digit);
        }

        template <typename T, typename Char>
        inline static bool add(T& n, Char ch, std::true_type)
        {

            T const max = (std::numeric_limits<T>::max)();
            T const val = max / Radix;
            if (n > val)
                return false;

            T tmp = n * Radix;


            const int digit = radix_traits<Radix>::digit(ch);
            if (tmp > max - digit)
                return false;

            n = tmp + static_cast<T>(digit);
            return true;
        }
    };

    template <unsigned Radix>
    struct negative_accumulator
    {
        template <typename T, typename Char>
        inline static void add(T& n, Char ch, std::false_type)
        {
            const int digit = radix_traits<Radix>::digit(ch);
            n = n * T(Radix) - T(digit);
        }

        template <typename T, typename Char>
        inline static bool add(T& n, Char ch, std::true_type)
        {

            T const min = (std::numeric_limits<T>::min)();
            T const val = min / T(Radix);
            if (n < val)
                return false;

            T tmp = n * Radix;


            int const digit = radix_traits<Radix>::digit(ch);
            if (tmp < min + digit)
                return false;

            n = tmp - static_cast<T>(digit);
            return true;
        }
    };

    template <unsigned Radix, typename Accumulator, int MaxDigits>
    struct int_extractor
    {
        template <typename Char, typename T>
        inline static bool
        call(Char ch, std::size_t count, T& n, std::true_type)
        {
            std::size_t constexpr
                overflow_free = digits_traits<T, Radix>::value - 1;

            if (count < overflow_free)
            {
                Accumulator::add(n, ch, std::false_type{});
            }
            else
            {
                if (!Accumulator::add(n, ch, std::true_type{}))
                    return false;
            }
            return true;
        }

        template <typename Char, typename T>
        inline static bool
        call(Char ch, std::size_t , T& n, std::false_type)
        {

            Accumulator::add(n, ch, std::false_type{});
            return true;
        }

        template <typename Char>
        inline static bool
        call(Char , std::size_t , unused_type, std::false_type)
        {
            return true;
        }

        template <typename Char, typename T>
        inline static bool
        call(Char ch, std::size_t count, T& n)
        {
            return call(ch, count, n
              , std::integral_constant<bool,
                    (   (MaxDigits < 0)
                    ||  (MaxDigits > digits_traits<T, Radix>::value)
                    )
                  && std::numeric_limits<T>::is_bounded
                >()
            );
        }
    };

    template <int MaxDigits>
    struct check_max_digits
    {
        inline static bool
        call(std::size_t count)
        {
            return count < MaxDigits;
        }
    };

    template <>
    struct check_max_digits<-1>
    {
        inline static bool
        call(std::size_t )
        {
            return true;
        }
    };

    template <
        typename T, unsigned Radix, unsigned MinDigits, int MaxDigits
      , typename Accumulator = positive_accumulator<Radix>
      , bool Accumulate = false
    >
    struct extract_int_impl
    {
        template <typename Iterator, typename Sentinel, typename Attribute>
        inline static bool
        parse_main(
            Iterator& first
          , Sentinel last
          , Attribute& attr)
        {
            typedef radix_traits<Radix> radix_check;
            typedef int_extractor<Radix, Accumulator, MaxDigits> extractor;
            typedef
                typename std::iterator_traits<Iterator>::value_type char_type;

            Iterator it = first;
            std::size_t leading_zeros = 0;
            if (!Accumulate)
            {

                while (it != last && *it == '0' && leading_zeros < MaxDigits)
                {
                    ++it;
                    ++leading_zeros;
                }
            }

            typedef Attribute attribute_type;

            attribute_type val = Accumulate ? attr : attribute_type(0);
            std::size_t count = 0;
            char_type ch;

            while (true)
            {
                if (!check_max_digits<MaxDigits>::call(count + leading_zeros) ||
                    it == last)
                    break;
                ch = *it;
                if (!radix_check::is_valid(ch) ||
                    !extractor::call(ch, count, val))
                    break;
                ++it;
                ++count;
                if (!check_max_digits<MaxDigits>::call(count + leading_zeros) ||
                    it == last)
                    break;
                ch = *it;
                if (!radix_check::is_valid(ch) ||
                    !extractor::call(ch, count, val))
                    break;
                ++it;
                ++count;
                if (!check_max_digits<MaxDigits>::call(count + leading_zeros) ||
                    it == last)
                    break;
                ch = *it;
                if (!radix_check::is_valid(ch) ||
                    !extractor::call(ch, count, val))
                    break;
                ++it;
                ++count;
            }

            if (count + leading_zeros >= MinDigits)
            {
                attr = val;
                first = it;
                return true;
            }
            return false;
        }

        template <typename Iterator, typename Sentinel>
        inline static bool
        parse(
            Iterator& first
          , Sentinel last
          , unused_type)
        {
            T n = 0;
            return parse_main(first, last, n);
        }

        template <typename Iterator, typename Sentinel, typename Attribute>
        inline static bool
        parse(
            Iterator& first
          , Sentinel last
          , Attribute& attr)
        {
            return parse_main(first, last, attr);
        }
    };

    template <typename T, unsigned Radix, typename Accumulator, bool Accumulate>
    struct extract_int_impl<T, Radix, 1, -1, Accumulator, Accumulate>
    {
        template <typename Iterator, typename Sentinel, typename Attribute>
        inline static bool
        parse_main(
            Iterator& first
          , Sentinel last
          , Attribute& attr)
        {
            typedef radix_traits<Radix> radix_check;
            typedef int_extractor<Radix, Accumulator, -1> extractor;
            typedef
                typename std::iterator_traits<Iterator>::value_type char_type;

            Iterator it = first;
            std::size_t count = 0;
            if (!Accumulate)
            {

                while (it != last && *it == '0')
                {
                    ++it;
                    ++count;
                }

                if (it == last)
                {
                    if (count == 0)
                        return false;
                    attr = 0;
                    first = it;
                    return true;
                }
            }

            typedef Attribute attribute_type;

            attribute_type val = Accumulate ? attr : attribute_type(0);
            char_type ch = *it;

            if (!radix_check::is_valid(ch) || !extractor::call(ch, 0, val))
            {
                if (count == 0)
                    return false;
                attr = val;
                first = it;
                return true;
            }

            count = 0;
            ++it;
            while (true)
            {
                if (it == last)
                    break;
                ch = *it;
                if (!radix_check::is_valid(ch))
                    break;
                if (!extractor::call(ch, count, val))
                    return false;
                ++it;
                ++count;
                if (it == last)
                    break;
                ch = *it;
                if (!radix_check::is_valid(ch))
                    break;
                if (!extractor::call(ch, count, val))
                    return false;
                ++it;
                ++count;
                if (it == last)
                    break;
                ch = *it;
                if (!radix_check::is_valid(ch))
                    break;
                if (!extractor::call(ch, count, val))
                    return false;
                ++it;
                ++count;
            }

            attr = val;
            first = it;
            return true;
        }

        template <typename Iterator, typename Sentinel>
        inline static bool
        parse(
            Iterator& first
          , Sentinel last
          , unused_type)
        {
            T n = 0;
            return parse_main(first, last, n);
        }

        template <typename Iterator, typename Sentinel, typename Attribute>
        inline static bool
        parse(
            Iterator& first
          , Sentinel last
          , Attribute& attr)
        {
            return parse_main(first, last, attr);
        }
    };
#line 449 "boost/parser/detail/numeric.hpp"
    template<typename Iterator, typename Sentinel>
    inline bool extract_sign(Iterator & first, Sentinel last)
    {
        (void)last;
        BOOST_ASSERT(first != last);


        bool neg = *first == '-';
        if (neg || (*first == '+'))
        {
            ++first;
            return neg;
        }
        return false;
    }
#line 468 "boost/parser/detail/numeric.hpp"
    template <typename T, unsigned Radix, unsigned MinDigits, int MaxDigits
      , bool Accumulate = false>
    struct extract_uint
    {

        static_assert(
            (Radix >= 2 && Radix <= 36),
            "Error Unsupported Radix");

        template <typename Iterator, typename Sentinel>
        inline static bool call(Iterator& first, Sentinel last, T& attr)
        {
            if (first == last)
                return false;

            typedef extract_int_impl<
                T
              , Radix
              , MinDigits
              , MaxDigits
              , positive_accumulator<Radix>
              , Accumulate>
            extract_type;

            Iterator save = first;
            if (!extract_type::parse(first, last, attr))
            {
                first = save;
                return false;
            }
            return true;
        }
    };
#line 505 "boost/parser/detail/numeric.hpp"
    template <typename T, unsigned Radix, unsigned MinDigits, int MaxDigits>
    struct extract_int
    {

        static_assert(
            (Radix == 2 || Radix == 8 || Radix == 10 || Radix == 16),
            "Error Unsupported Radix");

        template <typename Iterator, typename Sentinel>
        inline static bool call(Iterator& first, Sentinel last, T& attr)
        {
            if (first == last)
                return false;

            typedef extract_int_impl<
                T, Radix, MinDigits, MaxDigits>
            extract_pos_type;

            typedef extract_int_impl<
                T, Radix, MinDigits, MaxDigits, negative_accumulator<Radix> >
            extract_neg_type;

            Iterator save = first;
            bool hit = detail_spirit_x3::extract_sign(first, last);
            if (hit)
                hit = extract_neg_type::parse(first, last, attr);
            else
                hit = extract_pos_type::parse(first, last, attr);

            if (!hit)
            {
                first = save;
                return false;
            }
            return true;
        }
    };




    template <typename T, typename Enable = void>
    struct pow10_helper
    {
        static T call(unsigned dim)
        {
            return std::pow(T(10), T(dim));
        }
    };

    template <typename T>
    struct pow10_table
    {
        constexpr static std::size_t size =
            std::numeric_limits<T>::max_exponent10;

        constexpr pow10_table()
         : exponents()
        {
            exponents[0] = T(1);
            for (auto i = 1; i != size; ++i)
                exponents[i] = exponents[i-1] * T(10);
        }

        T exponents[size];
    };

    template <typename T>
    struct native_pow10_helper
    {
        constexpr static auto table = pow10_table<T>();
        static T call(unsigned dim)
        {
            return table.exponents[dim];
        }
    };

    template <>
    struct pow10_helper<float>
      : native_pow10_helper<float> {};

    template <>
    struct pow10_helper<double>
      : native_pow10_helper<double> {};

    template <>
    struct pow10_helper<long double>
      : native_pow10_helper<long double> {};

    template <typename T>
    inline T pow10(unsigned dim)
    {
        return detail_spirit_x3::pow10_helper<T>::call(dim);
    }

    template<typename T>
    inline bool scale(int exp, T & n)
    {
        constexpr auto max_exp = std::numeric_limits<T>::max_exponent10;
        constexpr auto min_exp = std::numeric_limits<T>::min_exponent10;

        if (exp >= 0)
        {


            if (std::is_floating_point_v<T> && exp > max_exp)
                return false;
            n *= detail_spirit_x3::pow10<T>(exp);
        }
        else
        {
            if (exp < min_exp)
            {
                n /= detail_spirit_x3::pow10<T>(-min_exp);



                exp += -min_exp;
                if (std::is_floating_point_v<T> && exp < min_exp)
                    return false;

                n /= detail_spirit_x3::pow10<T>(-exp);
            }
            else
            {
                n /= detail_spirit_x3::pow10<T>(-exp);
            }
        }
        return true;
    }

    template<typename T>
    bool scale(int exp, int frac, T & n)
    {
        return detail_spirit_x3::scale(exp - frac, n);
    }

    template<typename T>
    T negate(bool neg, T n)
    {
        return neg ? -n : n;
    }

    template <typename T, typename RealPolicies>
    struct extract_real
    {
        template <typename Iterator, typename Sentinel, typename Attribute>
        static bool
        parse(Iterator& first, Sentinel last, Attribute& attr,
            RealPolicies const& p)
        {
            if (first == last)
                return false;
            Iterator save = first;



            bool neg = p.parse_sign(first, last);


            T n = 0;
            bool got_a_number = p.parse_n(first, last, n);



            if (!got_a_number)
            {

                if (p.parse_nan(first, last, n) ||
                    p.parse_inf(first, last, n))
                {

                    attr = detail_spirit_x3::negate(neg, n);
                    return true;
                }



                if (!p.allow_leading_dot)
                {
                    first = save;
                    return false;
                }
            }

            bool e_hit = false;
            Iterator e_pos;
            int frac_digits = 0;


            if (p.parse_dot(first, last))
            {



                Iterator savef = first;
                if (p.parse_frac_n(first, last, n))
                {


                    if (!std::is_same_v<T, unused_type>)
                        frac_digits =
                            static_cast<int>(std::distance(savef, first));
                    BOOST_ASSERT(frac_digits >= 0);
                }
                else if (!got_a_number || !p.allow_trailing_dot)
                {



                    first = save;
                    return false;
                }


                e_pos = first;
                e_hit = p.parse_exp(first, last);
            }
            else
            {

                if (!got_a_number)
                {
                    first = save;
                    return false;
                }



                e_pos = first;
                e_hit = p.parse_exp(first, last);
                if (p.expect_dot && !e_hit)
                {
                    first = save;
                    return false;
                }
            }

            if (e_hit)
            {


                int exp = 0;
                if (p.parse_exp_n(first, last, exp))
                {


                    if (!detail_spirit_x3::scale(exp, frac_digits, n))
                        return false;
                }
                else
                {


                    first = e_pos;


                    if (!detail_spirit_x3::scale(-frac_digits, n))
                        return false;
                }
            }
            else if (frac_digits)
            {

                if (!detail_spirit_x3::scale(-frac_digits, n))
                    return false;
            }


            attr = detail_spirit_x3::negate(neg, n);


            return true;
        }
    };
#line 785 "boost/parser/detail/numeric.hpp"
    struct common_type_equal
    {
        template<typename T, typename U>
        bool operator()(T x, U y)
        {
            using common_t = std::common_type_t<decltype(x), decltype(y)>;
            return (common_t)x == (common_t)y;
        }
    };

    template <typename Char, typename Iterator, typename Sentinel>
    inline bool string_parse(
        Char const* uc_i, Char const* lc_i
      , Iterator& first, Sentinel const& last)
    {
        Iterator i = first;

        common_type_equal eq;

        for (; *uc_i && *lc_i; ++uc_i, ++lc_i, ++i)
            if (i == last || (!eq(*uc_i, *i) && !eq(*lc_i, *i)))
                return false;
        first = i;
        return true;
    }
#line 818 "boost/parser/detail/numeric.hpp"
    template <typename T>
    struct ureal_policies
    {

        static bool const allow_leading_dot = true;
        static bool const allow_trailing_dot = true;
        static bool const expect_dot = false;

        template <typename Iterator, typename Sentinel>
        static bool
        parse_sign(Iterator& , Iterator const& )
        {
            return false;
        }

        template <typename Iterator, typename Sentinel, typename Attribute>
        static bool
        parse_n(Iterator& first, Sentinel const& last, Attribute& attr_)
        {
            return extract_uint<T, 10, 1, -1>::call(first, last, attr_);
        }

        template <typename Iterator, typename Sentinel>
        static bool
        parse_dot(Iterator& first, Sentinel const& last)
        {
            if (first == last || *first != '.')
                return false;
            ++first;
            return true;
        }

        template <typename Iterator, typename Sentinel, typename Attribute>
        static bool
        parse_frac_n(Iterator& first, Sentinel const& last, Attribute& attr_)
        {
            return extract_uint<T, 10, 1, -1, true>::call(first, last, attr_);
        }

        template <typename Iterator, typename Sentinel>
        static bool
        parse_exp(Iterator& first, Sentinel const& last)
        {
            if (first == last || (*first != 'e' && *first != 'E'))
                return false;
            ++first;
            return true;
        }

        template <typename Iterator, typename Sentinel>
        static bool
        parse_exp_n(Iterator& first, Sentinel const& last, int& attr_)
        {
            return extract_int<int, 10, 1, -1>::call(first, last, attr_);
        }
#line 888 "boost/parser/detail/numeric.hpp"
        template <typename Iterator, typename Sentinel, typename Attribute>
        static bool
        parse_nan(Iterator& first, Sentinel const& last, Attribute& attr_)
        {
            if (first == last)
                return false;

            if (*first != 'n' && *first != 'N')
                return false;


            if (detail_spirit_x3::string_parse("nan", "NAN", first, last))
            {
                if (first != last && *first == '(')
                {

                    Iterator i = first;

                    while (++i != last && *i != ')')
                        ;
                    if (i == last)
                        return false;

                    first = ++i;
                }
                attr_ = std::numeric_limits<T>::quiet_NaN();
                return true;
            }
            return false;
        }

        template <typename Iterator, typename Sentinel, typename Attribute>
        static bool
        parse_inf(Iterator& first, Sentinel const& last, Attribute& attr_)
        {
            if (first == last)
                return false;

            if (*first != 'i' && *first != 'I')
                return false;


            if (detail_spirit_x3::string_parse("inf", "INF", first, last))
            {

                detail_spirit_x3::string_parse("inity", "INITY", first, last);
                attr_ = std::numeric_limits<T>::infinity();
                return true;
            }
            return false;
        }
    };
#line 944 "boost/parser/detail/numeric.hpp"
    template <typename T>
    struct real_policies : ureal_policies<T>
    {
        template <typename Iterator, typename Sentinel>
        static bool
        parse_sign(Iterator& first, Sentinel const& last)
        {
            return detail_spirit_x3::extract_sign(first, last);
        }
    };

    template <typename T>
    struct strict_ureal_policies : ureal_policies<T>
    {
        static bool const expect_dot = true;
    };

    template <typename T>
    struct strict_real_policies : real_policies<T>
    {
        static bool const expect_dot = true;
    };

}}}
#line 2 "boost/parser/detail/case_fold.hpp"
#define BOOST_PARSER_DETAIL_CASE_FOLD_HPP
#line 9 "boost/parser/detail/case_fold_data_generated.hpp"
namespace boost::parser::detail {


struct short_mapping_range {
    char32_t cp_first_;
    char32_t cp_last_;
    uint16_t stride_;
    uint16_t first_idx_;
};
inline constexpr int longest_mapping = 3;
struct long_mapping {
    char32_t cp_;
    char32_t mapping_[longest_mapping + 1];
};
inline constexpr long_mapping  long_mappings[104] = {
    {0x00DF, {0x0073, 0x0073 , 0}},
    {0x0130, {0x0069, 0x0307 , 0}},
    {0x0149, {0x02BC, 0x006E , 0}},
    {0x01F0, {0x006A, 0x030C , 0}},
    {0x0390, {0x03B9, 0x0308, 0x0301 , 0}},
    {0x03B0, {0x03C5, 0x0308, 0x0301 , 0}},
    {0x0587, {0x0565, 0x0582 , 0}},
    {0x1E96, {0x0068, 0x0331 , 0}},
    {0x1E97, {0x0074, 0x0308 , 0}},
    {0x1E98, {0x0077, 0x030A , 0}},
    {0x1E99, {0x0079, 0x030A , 0}},
    {0x1E9A, {0x0061, 0x02BE , 0}},
    {0x1E9E, {0x0073, 0x0073 , 0}},
    {0x1F50, {0x03C5, 0x0313 , 0}},
    {0x1F52, {0x03C5, 0x0313, 0x0300 , 0}},
    {0x1F54, {0x03C5, 0x0313, 0x0301 , 0}},
    {0x1F56, {0x03C5, 0x0313, 0x0342 , 0}},
    {0x1F80, {0x1F00, 0x03B9 , 0}},
    {0x1F81, {0x1F01, 0x03B9 , 0}},
    {0x1F82, {0x1F02, 0x03B9 , 0}},
    {0x1F83, {0x1F03, 0x03B9 , 0}},
    {0x1F84, {0x1F04, 0x03B9 , 0}},
    {0x1F85, {0x1F05, 0x03B9 , 0}},
    {0x1F86, {0x1F06, 0x03B9 , 0}},
    {0x1F87, {0x1F07, 0x03B9 , 0}},
    {0x1F88, {0x1F00, 0x03B9 , 0}},
    {0x1F89, {0x1F01, 0x03B9 , 0}},
    {0x1F8A, {0x1F02, 0x03B9 , 0}},
    {0x1F8B, {0x1F03, 0x03B9 , 0}},
    {0x1F8C, {0x1F04, 0x03B9 , 0}},
    {0x1F8D, {0x1F05, 0x03B9 , 0}},
    {0x1F8E, {0x1F06, 0x03B9 , 0}},
    {0x1F8F, {0x1F07, 0x03B9 , 0}},
    {0x1F90, {0x1F20, 0x03B9 , 0}},
    {0x1F91, {0x1F21, 0x03B9 , 0}},
    {0x1F92, {0x1F22, 0x03B9 , 0}},
    {0x1F93, {0x1F23, 0x03B9 , 0}},
    {0x1F94, {0x1F24, 0x03B9 , 0}},
    {0x1F95, {0x1F25, 0x03B9 , 0}},
    {0x1F96, {0x1F26, 0x03B9 , 0}},
    {0x1F97, {0x1F27, 0x03B9 , 0}},
    {0x1F98, {0x1F20, 0x03B9 , 0}},
    {0x1F99, {0x1F21, 0x03B9 , 0}},
    {0x1F9A, {0x1F22, 0x03B9 , 0}},
    {0x1F9B, {0x1F23, 0x03B9 , 0}},
    {0x1F9C, {0x1F24, 0x03B9 , 0}},
    {0x1F9D, {0x1F25, 0x03B9 , 0}},
    {0x1F9E, {0x1F26, 0x03B9 , 0}},
    {0x1F9F, {0x1F27, 0x03B9 , 0}},
    {0x1FA0, {0x1F60, 0x03B9 , 0}},
    {0x1FA1, {0x1F61, 0x03B9 , 0}},
    {0x1FA2, {0x1F62, 0x03B9 , 0}},
    {0x1FA3, {0x1F63, 0x03B9 , 0}},
    {0x1FA4, {0x1F64, 0x03B9 , 0}},
    {0x1FA5, {0x1F65, 0x03B9 , 0}},
    {0x1FA6, {0x1F66, 0x03B9 , 0}},
    {0x1FA7, {0x1F67, 0x03B9 , 0}},
    {0x1FA8, {0x1F60, 0x03B9 , 0}},
    {0x1FA9, {0x1F61, 0x03B9 , 0}},
    {0x1FAA, {0x1F62, 0x03B9 , 0}},
    {0x1FAB, {0x1F63, 0x03B9 , 0}},
    {0x1FAC, {0x1F64, 0x03B9 , 0}},
    {0x1FAD, {0x1F65, 0x03B9 , 0}},
    {0x1FAE, {0x1F66, 0x03B9 , 0}},
    {0x1FAF, {0x1F67, 0x03B9 , 0}},
    {0x1FB2, {0x1F70, 0x03B9 , 0}},
    {0x1FB3, {0x03B1, 0x03B9 , 0}},
    {0x1FB4, {0x03AC, 0x03B9 , 0}},
    {0x1FB6, {0x03B1, 0x0342 , 0}},
    {0x1FB7, {0x03B1, 0x0342, 0x03B9 , 0}},
    {0x1FBC, {0x03B1, 0x03B9 , 0}},
    {0x1FC2, {0x1F74, 0x03B9 , 0}},
    {0x1FC3, {0x03B7, 0x03B9 , 0}},
    {0x1FC4, {0x03AE, 0x03B9 , 0}},
    {0x1FC6, {0x03B7, 0x0342 , 0}},
    {0x1FC7, {0x03B7, 0x0342, 0x03B9 , 0}},
    {0x1FCC, {0x03B7, 0x03B9 , 0}},
    {0x1FD2, {0x03B9, 0x0308, 0x0300 , 0}},
    {0x1FD3, {0x03B9, 0x0308, 0x0301 , 0}},
    {0x1FD6, {0x03B9, 0x0342 , 0}},
    {0x1FD7, {0x03B9, 0x0308, 0x0342 , 0}},
    {0x1FE2, {0x03C5, 0x0308, 0x0300 , 0}},
    {0x1FE3, {0x03C5, 0x0308, 0x0301 , 0}},
    {0x1FE4, {0x03C1, 0x0313 , 0}},
    {0x1FE6, {0x03C5, 0x0342 , 0}},
    {0x1FE7, {0x03C5, 0x0308, 0x0342 , 0}},
    {0x1FF2, {0x1F7C, 0x03B9 , 0}},
    {0x1FF3, {0x03C9, 0x03B9 , 0}},
    {0x1FF4, {0x03CE, 0x03B9 , 0}},
    {0x1FF6, {0x03C9, 0x0342 , 0}},
    {0x1FF7, {0x03C9, 0x0342, 0x03B9 , 0}},
    {0x1FFC, {0x03C9, 0x03B9 , 0}},
    {0xFB00, {0x0066, 0x0066 , 0}},
    {0xFB01, {0x0066, 0x0069 , 0}},
    {0xFB02, {0x0066, 0x006C , 0}},
    {0xFB03, {0x0066, 0x0066, 0x0069 , 0}},
    {0xFB04, {0x0066, 0x0066, 0x006C , 0}},
    {0xFB05, {0x0073, 0x0074 , 0}},
    {0xFB06, {0x0073, 0x0074 , 0}},
    {0xFB13, {0x0574, 0x0576 , 0}},
    {0xFB14, {0x0574, 0x0565 , 0}},
    {0xFB15, {0x0574, 0x056B , 0}},
    {0xFB16, {0x057E, 0x0576 , 0}},
    {0xFB17, {0x0574, 0x056D , 0}},
};
inline constexpr short_mapping_range mapping_ranges[220] = {
    {0x0041, 0x005A, 1, 0},
    {0x005A, 0x00B5, 91, 25},
    {0x00B5, 0x00C0, 11, 26},
    {0x00C0, 0x00D6, 1, 27},
    {0x00D6, 0x00D8, 2, 49},
    {0x00D8, 0x00DE, 1, 50},
    {0x00DE, 0x0100, 34, 56},
    {0x0100, 0x012E, 2, 57},
    {0x012E, 0x0132, 4, 80},
    {0x0132, 0x0136, 2, 81},
    {0x0136, 0x0139, 3, 83},
    {0x0139, 0x0147, 2, 84},
    {0x0147, 0x014A, 3, 91},
    {0x014A, 0x0178, 2, 92},
    {0x0178, 0x0179, 1, 115},
    {0x0179, 0x0181, 2, 116},
    {0x0181, 0x0182, 1, 120},
    {0x0182, 0x0186, 2, 121},
    {0x0186, 0x0187, 1, 123},
    {0x0187, 0x0189, 2, 124},
    {0x0189, 0x018B, 1, 125},
    {0x018B, 0x018E, 3, 127},
    {0x018E, 0x0191, 1, 128},
    {0x0191, 0x0193, 2, 131},
    {0x0193, 0x0194, 1, 132},
    {0x0194, 0x0196, 2, 133},
    {0x0196, 0x0198, 1, 134},
    {0x0198, 0x019C, 4, 136},
    {0x019C, 0x019D, 1, 137},
    {0x019D, 0x019F, 2, 138},
    {0x019F, 0x01A0, 1, 139},
    {0x01A0, 0x01A6, 2, 140},
    {0x01A6, 0x01A7, 1, 143},
    {0x01A7, 0x01A9, 2, 144},
    {0x01A9, 0x01AC, 3, 145},
    {0x01AC, 0x01AE, 2, 146},
    {0x01AE, 0x01AF, 1, 147},
    {0x01AF, 0x01B1, 2, 148},
    {0x01B1, 0x01B3, 1, 149},
    {0x01B3, 0x01B7, 2, 151},
    {0x01B7, 0x01B8, 1, 153},
    {0x01B8, 0x01BC, 4, 154},
    {0x01BC, 0x01C4, 8, 155},
    {0x01C4, 0x01C5, 1, 156},
    {0x01C5, 0x01C7, 2, 157},
    {0x01C7, 0x01C8, 1, 158},
    {0x01C8, 0x01CA, 2, 159},
    {0x01CA, 0x01CB, 1, 160},
    {0x01CB, 0x01DB, 2, 161},
    {0x01DB, 0x01DE, 3, 169},
    {0x01DE, 0x01EE, 2, 170},
    {0x01EE, 0x01F1, 3, 178},
    {0x01F1, 0x01F2, 1, 179},
    {0x01F2, 0x01F6, 2, 180},
    {0x01F6, 0x01F8, 1, 182},
    {0x01F8, 0x0232, 2, 184},
    {0x0232, 0x023A, 8, 213},
    {0x023A, 0x023B, 1, 214},
    {0x023B, 0x023D, 2, 215},
    {0x023D, 0x023E, 1, 216},
    {0x023E, 0x0241, 3, 217},
    {0x0241, 0x0243, 2, 218},
    {0x0243, 0x0246, 1, 219},
    {0x0246, 0x024E, 2, 222},
    {0x024E, 0x0345, 247, 226},
    {0x0345, 0x0370, 43, 227},
    {0x0370, 0x0372, 2, 228},
    {0x0372, 0x0376, 4, 229},
    {0x0376, 0x037F, 9, 230},
    {0x037F, 0x0386, 7, 231},
    {0x0386, 0x0388, 2, 232},
    {0x0388, 0x038A, 1, 233},
    {0x038A, 0x038E, 2, 235},
    {0x038E, 0x038F, 1, 237},
    {0x038F, 0x0391, 2, 238},
    {0x0391, 0x03A1, 1, 239},
    {0x03A1, 0x03A3, 2, 255},
    {0x03A3, 0x03AB, 1, 256},
    {0x03AB, 0x03C2, 23, 264},
    {0x03C2, 0x03CF, 13, 265},
    {0x03CF, 0x03D1, 1, 266},
    {0x03D1, 0x03D5, 4, 268},
    {0x03D5, 0x03D6, 1, 269},
    {0x03D6, 0x03F0, 2, 270},
    {0x03F0, 0x03F1, 1, 283},
    {0x03F1, 0x03F4, 3, 284},
    {0x03F4, 0x03F5, 1, 285},
    {0x03F5, 0x03F9, 2, 286},
    {0x03F9, 0x03FA, 1, 288},
    {0x03FA, 0x03FD, 3, 289},
    {0x03FD, 0x042F, 1, 290},
    {0x042F, 0x0460, 49, 340},
    {0x0460, 0x0480, 2, 341},
    {0x0480, 0x048A, 10, 357},
    {0x048A, 0x04C0, 2, 358},
    {0x04C0, 0x04C1, 1, 385},
    {0x04C1, 0x04CD, 2, 386},
    {0x04CD, 0x04D0, 3, 392},
    {0x04D0, 0x052E, 2, 393},
    {0x052E, 0x0531, 3, 440},
    {0x0531, 0x0556, 1, 441},
    {0x0556, 0x10A0, 2890, 478},
    {0x10A0, 0x10C5, 1, 479},
    {0x10C5, 0x10C7, 2, 516},
    {0x10C7, 0x10CD, 6, 517},
    {0x10CD, 0x13F8, 811, 518},
    {0x13F8, 0x13FD, 1, 519},
    {0x13FD, 0x1C80, 2179, 524},
    {0x1C80, 0x1C88, 1, 525},
    {0x1C88, 0x1C90, 8, 533},
    {0x1C90, 0x1CBA, 1, 534},
    {0x1CBA, 0x1CBD, 3, 576},
    {0x1CBD, 0x1CBF, 1, 577},
    {0x1CBF, 0x1E00, 321, 579},
    {0x1E00, 0x1E94, 2, 580},
    {0x1E94, 0x1E9B, 7, 654},
    {0x1E9B, 0x1EA0, 5, 655},
    {0x1EA0, 0x1EFE, 2, 656},
    {0x1EFE, 0x1F08, 10, 703},
    {0x1F08, 0x1F0F, 1, 704},
    {0x1F0F, 0x1F18, 9, 711},
    {0x1F18, 0x1F1D, 1, 712},
    {0x1F1D, 0x1F28, 11, 717},
    {0x1F28, 0x1F2F, 1, 718},
    {0x1F2F, 0x1F38, 9, 725},
    {0x1F38, 0x1F3F, 1, 726},
    {0x1F3F, 0x1F48, 9, 733},
    {0x1F48, 0x1F4D, 1, 734},
    {0x1F4D, 0x1F59, 12, 739},
    {0x1F59, 0x1F5F, 2, 740},
    {0x1F5F, 0x1F68, 9, 743},
    {0x1F68, 0x1F6F, 1, 744},
    {0x1F6F, 0x1FB8, 73, 751},
    {0x1FB8, 0x1FBB, 1, 752},
    {0x1FBB, 0x1FBE, 3, 755},
    {0x1FBE, 0x1FC8, 10, 756},
    {0x1FC8, 0x1FCB, 1, 757},
    {0x1FCB, 0x1FD8, 13, 760},
    {0x1FD8, 0x1FDB, 1, 761},
    {0x1FDB, 0x1FE8, 13, 764},
    {0x1FE8, 0x1FEC, 1, 765},
    {0x1FEC, 0x1FF8, 12, 769},
    {0x1FF8, 0x1FFB, 1, 770},
    {0x1FFB, 0x2126, 299, 773},
    {0x2126, 0x212A, 4, 774},
    {0x212A, 0x212B, 1, 775},
    {0x212B, 0x2132, 7, 776},
    {0x2132, 0x2160, 46, 777},
    {0x2160, 0x216F, 1, 778},
    {0x216F, 0x2183, 20, 793},
    {0x2183, 0x24B6, 819, 794},
    {0x24B6, 0x24CF, 1, 795},
    {0x24CF, 0x2C00, 1841, 820},
    {0x2C00, 0x2C2F, 1, 821},
    {0x2C2F, 0x2C60, 49, 868},
    {0x2C60, 0x2C62, 2, 869},
    {0x2C62, 0x2C64, 1, 870},
    {0x2C64, 0x2C67, 3, 872},
    {0x2C67, 0x2C6D, 2, 873},
    {0x2C6D, 0x2C70, 1, 876},
    {0x2C70, 0x2C72, 2, 879},
    {0x2C72, 0x2C75, 3, 880},
    {0x2C75, 0x2C7E, 9, 881},
    {0x2C7E, 0x2C80, 1, 882},
    {0x2C80, 0x2CE2, 2, 884},
    {0x2CE2, 0x2CEB, 9, 933},
    {0x2CEB, 0x2CED, 2, 934},
    {0x2CED, 0x2CF2, 5, 935},
    {0x2CF2, 0xA640, 31054, 936},
    {0xA640, 0xA66C, 2, 937},
    {0xA66C, 0xA680, 20, 959},
    {0xA680, 0xA69A, 2, 960},
    {0xA69A, 0xA722, 136, 973},
    {0xA722, 0xA72E, 2, 974},
    {0xA72E, 0xA732, 4, 980},
    {0xA732, 0xA76E, 2, 981},
    {0xA76E, 0xA779, 11, 1011},
    {0xA779, 0xA77D, 2, 1012},
    {0xA77D, 0xA77E, 1, 1014},
    {0xA77E, 0xA786, 2, 1015},
    {0xA786, 0xA78B, 5, 1019},
    {0xA78B, 0xA78D, 2, 1020},
    {0xA78D, 0xA790, 3, 1021},
    {0xA790, 0xA792, 2, 1022},
    {0xA792, 0xA796, 4, 1023},
    {0xA796, 0xA7AA, 2, 1024},
    {0xA7AA, 0xA7AE, 1, 1034},
    {0xA7AE, 0xA7B0, 2, 1038},
    {0xA7B0, 0xA7B4, 1, 1039},
    {0xA7B4, 0xA7C4, 2, 1043},
    {0xA7C4, 0xA7C7, 1, 1051},
    {0xA7C7, 0xA7C9, 2, 1054},
    {0xA7C9, 0xA7D0, 7, 1055},
    {0xA7D0, 0xA7D6, 6, 1056},
    {0xA7D6, 0xA7D8, 2, 1057},
    {0xA7D8, 0xA7F5, 29, 1058},
    {0xA7F5, 0xAB70, 891, 1059},
    {0xAB70, 0xABBF, 1, 1060},
    {0xABBF, 0xFF21, 21346, 1139},
    {0xFF21, 0xFF3A, 1, 1140},
    {0xFF3A, 0x10400, 1222, 1165},
    {0x10400, 0x10427, 1, 1166},
    {0x10427, 0x104B0, 137, 1205},
    {0x104B0, 0x104D3, 1, 1206},
    {0x104D3, 0x10570, 157, 1241},
    {0x10570, 0x1057A, 1, 1242},
    {0x1057A, 0x1057C, 2, 1252},
    {0x1057C, 0x1058A, 1, 1253},
    {0x1058A, 0x1058C, 2, 1267},
    {0x1058C, 0x10592, 1, 1268},
    {0x10592, 0x10594, 2, 1274},
    {0x10594, 0x10595, 1, 1275},
    {0x10595, 0x10C80, 1771, 1276},
    {0x10C80, 0x10CB2, 1, 1277},
    {0x10CB2, 0x118A0, 3054, 1327},
    {0x118A0, 0x118BF, 1, 1328},
    {0x118BF, 0x16E40, 21889, 1359},
    {0x16E40, 0x16E5F, 1, 1360},
    {0x16E5F, 0x1E900, 31393, 1391},
    {0x1E900, 0x1E921 + 1, 1, 1392},
};
inline constexpr char32_t single_mapping_cps[1426] = {
    0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068,
    0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F, 0x0070,
    0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077, 0x0078,
    0x0079, 0x007A, 0x03BC, 0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x00E4,
    0x00E5, 0x00E6, 0x00E7, 0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x00EC,
    0x00ED, 0x00EE, 0x00EF, 0x00F0, 0x00F1, 0x00F2, 0x00F3, 0x00F4,
    0x00F5, 0x00F6, 0x00F8, 0x00F9, 0x00FA, 0x00FB, 0x00FC, 0x00FD,
    0x00FE, 0x0101, 0x0103, 0x0105, 0x0107, 0x0109, 0x010B, 0x010D,
    0x010F, 0x0111, 0x0113, 0x0115, 0x0117, 0x0119, 0x011B, 0x011D,
    0x011F, 0x0121, 0x0123, 0x0125, 0x0127, 0x0129, 0x012B, 0x012D,
    0x012F, 0x0133, 0x0135, 0x0137, 0x013A, 0x013C, 0x013E, 0x0140,
    0x0142, 0x0144, 0x0146, 0x0148, 0x014B, 0x014D, 0x014F, 0x0151,
    0x0153, 0x0155, 0x0157, 0x0159, 0x015B, 0x015D, 0x015F, 0x0161,
    0x0163, 0x0165, 0x0167, 0x0169, 0x016B, 0x016D, 0x016F, 0x0171,
    0x0173, 0x0175, 0x0177, 0x00FF, 0x017A, 0x017C, 0x017E, 0x0073,
    0x0253, 0x0183, 0x0185, 0x0254, 0x0188, 0x0256, 0x0257, 0x018C,
    0x01DD, 0x0259, 0x025B, 0x0192, 0x0260, 0x0263, 0x0269, 0x0268,
    0x0199, 0x026F, 0x0272, 0x0275, 0x01A1, 0x01A3, 0x01A5, 0x0280,
    0x01A8, 0x0283, 0x01AD, 0x0288, 0x01B0, 0x028A, 0x028B, 0x01B4,
    0x01B6, 0x0292, 0x01B9, 0x01BD, 0x01C6, 0x01C6, 0x01C9, 0x01C9,
    0x01CC, 0x01CC, 0x01CE, 0x01D0, 0x01D2, 0x01D4, 0x01D6, 0x01D8,
    0x01DA, 0x01DC, 0x01DF, 0x01E1, 0x01E3, 0x01E5, 0x01E7, 0x01E9,
    0x01EB, 0x01ED, 0x01EF, 0x01F3, 0x01F3, 0x01F5, 0x0195, 0x01BF,
    0x01F9, 0x01FB, 0x01FD, 0x01FF, 0x0201, 0x0203, 0x0205, 0x0207,
    0x0209, 0x020B, 0x020D, 0x020F, 0x0211, 0x0213, 0x0215, 0x0217,
    0x0219, 0x021B, 0x021D, 0x021F, 0x019E, 0x0223, 0x0225, 0x0227,
    0x0229, 0x022B, 0x022D, 0x022F, 0x0231, 0x0233, 0x2C65, 0x023C,
    0x019A, 0x2C66, 0x0242, 0x0180, 0x0289, 0x028C, 0x0247, 0x0249,
    0x024B, 0x024D, 0x024F, 0x03B9, 0x0371, 0x0373, 0x0377, 0x03F3,
    0x03AC, 0x03AD, 0x03AE, 0x03AF, 0x03CC, 0x03CD, 0x03CE, 0x03B1,
    0x03B2, 0x03B3, 0x03B4, 0x03B5, 0x03B6, 0x03B7, 0x03B8, 0x03B9,
    0x03BA, 0x03BB, 0x03BC, 0x03BD, 0x03BE, 0x03BF, 0x03C0, 0x03C1,
    0x03C3, 0x03C4, 0x03C5, 0x03C6, 0x03C7, 0x03C8, 0x03C9, 0x03CA,
    0x03CB, 0x03C3, 0x03D7, 0x03B2, 0x03B8, 0x03C6, 0x03C0, 0x03D9,
    0x03DB, 0x03DD, 0x03DF, 0x03E1, 0x03E3, 0x03E5, 0x03E7, 0x03E9,
    0x03EB, 0x03ED, 0x03EF, 0x03BA, 0x03C1, 0x03B8, 0x03B5, 0x03F8,
    0x03F2, 0x03FB, 0x037B, 0x037C, 0x037D, 0x0450, 0x0451, 0x0452,
    0x0453, 0x0454, 0x0455, 0x0456, 0x0457, 0x0458, 0x0459, 0x045A,
    0x045B, 0x045C, 0x045D, 0x045E, 0x045F, 0x0430, 0x0431, 0x0432,
    0x0433, 0x0434, 0x0435, 0x0436, 0x0437, 0x0438, 0x0439, 0x043A,
    0x043B, 0x043C, 0x043D, 0x043E, 0x043F, 0x0440, 0x0441, 0x0442,
    0x0443, 0x0444, 0x0445, 0x0446, 0x0447, 0x0448, 0x0449, 0x044A,
    0x044B, 0x044C, 0x044D, 0x044E, 0x044F, 0x0461, 0x0463, 0x0465,
    0x0467, 0x0469, 0x046B, 0x046D, 0x046F, 0x0471, 0x0473, 0x0475,
    0x0477, 0x0479, 0x047B, 0x047D, 0x047F, 0x0481, 0x048B, 0x048D,
    0x048F, 0x0491, 0x0493, 0x0495, 0x0497, 0x0499, 0x049B, 0x049D,
    0x049F, 0x04A1, 0x04A3, 0x04A5, 0x04A7, 0x04A9, 0x04AB, 0x04AD,
    0x04AF, 0x04B1, 0x04B3, 0x04B5, 0x04B7, 0x04B9, 0x04BB, 0x04BD,
    0x04BF, 0x04CF, 0x04C2, 0x04C4, 0x04C6, 0x04C8, 0x04CA, 0x04CC,
    0x04CE, 0x04D1, 0x04D3, 0x04D5, 0x04D7, 0x04D9, 0x04DB, 0x04DD,
    0x04DF, 0x04E1, 0x04E3, 0x04E5, 0x04E7, 0x04E9, 0x04EB, 0x04ED,
    0x04EF, 0x04F1, 0x04F3, 0x04F5, 0x04F7, 0x04F9, 0x04FB, 0x04FD,
    0x04FF, 0x0501, 0x0503, 0x0505, 0x0507, 0x0509, 0x050B, 0x050D,
    0x050F, 0x0511, 0x0513, 0x0515, 0x0517, 0x0519, 0x051B, 0x051D,
    0x051F, 0x0521, 0x0523, 0x0525, 0x0527, 0x0529, 0x052B, 0x052D,
    0x052F, 0x0561, 0x0562, 0x0563, 0x0564, 0x0565, 0x0566, 0x0567,
    0x0568, 0x0569, 0x056A, 0x056B, 0x056C, 0x056D, 0x056E, 0x056F,
    0x0570, 0x0571, 0x0572, 0x0573, 0x0574, 0x0575, 0x0576, 0x0577,
    0x0578, 0x0579, 0x057A, 0x057B, 0x057C, 0x057D, 0x057E, 0x057F,
    0x0580, 0x0581, 0x0582, 0x0583, 0x0584, 0x0585, 0x0586, 0x2D00,
    0x2D01, 0x2D02, 0x2D03, 0x2D04, 0x2D05, 0x2D06, 0x2D07, 0x2D08,
    0x2D09, 0x2D0A, 0x2D0B, 0x2D0C, 0x2D0D, 0x2D0E, 0x2D0F, 0x2D10,
    0x2D11, 0x2D12, 0x2D13, 0x2D14, 0x2D15, 0x2D16, 0x2D17, 0x2D18,
    0x2D19, 0x2D1A, 0x2D1B, 0x2D1C, 0x2D1D, 0x2D1E, 0x2D1F, 0x2D20,
    0x2D21, 0x2D22, 0x2D23, 0x2D24, 0x2D25, 0x2D27, 0x2D2D, 0x13F0,
    0x13F1, 0x13F2, 0x13F3, 0x13F4, 0x13F5, 0x0432, 0x0434, 0x043E,
    0x0441, 0x0442, 0x0442, 0x044A, 0x0463, 0xA64B, 0x10D0, 0x10D1,
    0x10D2, 0x10D3, 0x10D4, 0x10D5, 0x10D6, 0x10D7, 0x10D8, 0x10D9,
    0x10DA, 0x10DB, 0x10DC, 0x10DD, 0x10DE, 0x10DF, 0x10E0, 0x10E1,
    0x10E2, 0x10E3, 0x10E4, 0x10E5, 0x10E6, 0x10E7, 0x10E8, 0x10E9,
    0x10EA, 0x10EB, 0x10EC, 0x10ED, 0x10EE, 0x10EF, 0x10F0, 0x10F1,
    0x10F2, 0x10F3, 0x10F4, 0x10F5, 0x10F6, 0x10F7, 0x10F8, 0x10F9,
    0x10FA, 0x10FD, 0x10FE, 0x10FF, 0x1E01, 0x1E03, 0x1E05, 0x1E07,
    0x1E09, 0x1E0B, 0x1E0D, 0x1E0F, 0x1E11, 0x1E13, 0x1E15, 0x1E17,
    0x1E19, 0x1E1B, 0x1E1D, 0x1E1F, 0x1E21, 0x1E23, 0x1E25, 0x1E27,
    0x1E29, 0x1E2B, 0x1E2D, 0x1E2F, 0x1E31, 0x1E33, 0x1E35, 0x1E37,
    0x1E39, 0x1E3B, 0x1E3D, 0x1E3F, 0x1E41, 0x1E43, 0x1E45, 0x1E47,
    0x1E49, 0x1E4B, 0x1E4D, 0x1E4F, 0x1E51, 0x1E53, 0x1E55, 0x1E57,
    0x1E59, 0x1E5B, 0x1E5D, 0x1E5F, 0x1E61, 0x1E63, 0x1E65, 0x1E67,
    0x1E69, 0x1E6B, 0x1E6D, 0x1E6F, 0x1E71, 0x1E73, 0x1E75, 0x1E77,
    0x1E79, 0x1E7B, 0x1E7D, 0x1E7F, 0x1E81, 0x1E83, 0x1E85, 0x1E87,
    0x1E89, 0x1E8B, 0x1E8D, 0x1E8F, 0x1E91, 0x1E93, 0x1E95, 0x1E61,
    0x1EA1, 0x1EA3, 0x1EA5, 0x1EA7, 0x1EA9, 0x1EAB, 0x1EAD, 0x1EAF,
    0x1EB1, 0x1EB3, 0x1EB5, 0x1EB7, 0x1EB9, 0x1EBB, 0x1EBD, 0x1EBF,
    0x1EC1, 0x1EC3, 0x1EC5, 0x1EC7, 0x1EC9, 0x1ECB, 0x1ECD, 0x1ECF,
    0x1ED1, 0x1ED3, 0x1ED5, 0x1ED7, 0x1ED9, 0x1EDB, 0x1EDD, 0x1EDF,
    0x1EE1, 0x1EE3, 0x1EE5, 0x1EE7, 0x1EE9, 0x1EEB, 0x1EED, 0x1EEF,
    0x1EF1, 0x1EF3, 0x1EF5, 0x1EF7, 0x1EF9, 0x1EFB, 0x1EFD, 0x1EFF,
    0x1F00, 0x1F01, 0x1F02, 0x1F03, 0x1F04, 0x1F05, 0x1F06, 0x1F07,
    0x1F10, 0x1F11, 0x1F12, 0x1F13, 0x1F14, 0x1F15, 0x1F20, 0x1F21,
    0x1F22, 0x1F23, 0x1F24, 0x1F25, 0x1F26, 0x1F27, 0x1F30, 0x1F31,
    0x1F32, 0x1F33, 0x1F34, 0x1F35, 0x1F36, 0x1F37, 0x1F40, 0x1F41,
    0x1F42, 0x1F43, 0x1F44, 0x1F45, 0x1F51, 0x1F53, 0x1F55, 0x1F57,
    0x1F60, 0x1F61, 0x1F62, 0x1F63, 0x1F64, 0x1F65, 0x1F66, 0x1F67,
    0x1FB0, 0x1FB1, 0x1F70, 0x1F71, 0x03B9, 0x1F72, 0x1F73, 0x1F74,
    0x1F75, 0x1FD0, 0x1FD1, 0x1F76, 0x1F77, 0x1FE0, 0x1FE1, 0x1F7A,
    0x1F7B, 0x1FE5, 0x1F78, 0x1F79, 0x1F7C, 0x1F7D, 0x03C9, 0x006B,
    0x00E5, 0x214E, 0x2170, 0x2171, 0x2172, 0x2173, 0x2174, 0x2175,
    0x2176, 0x2177, 0x2178, 0x2179, 0x217A, 0x217B, 0x217C, 0x217D,
    0x217E, 0x217F, 0x2184, 0x24D0, 0x24D1, 0x24D2, 0x24D3, 0x24D4,
    0x24D5, 0x24D6, 0x24D7, 0x24D8, 0x24D9, 0x24DA, 0x24DB, 0x24DC,
    0x24DD, 0x24DE, 0x24DF, 0x24E0, 0x24E1, 0x24E2, 0x24E3, 0x24E4,
    0x24E5, 0x24E6, 0x24E7, 0x24E8, 0x24E9, 0x2C30, 0x2C31, 0x2C32,
    0x2C33, 0x2C34, 0x2C35, 0x2C36, 0x2C37, 0x2C38, 0x2C39, 0x2C3A,
    0x2C3B, 0x2C3C, 0x2C3D, 0x2C3E, 0x2C3F, 0x2C40, 0x2C41, 0x2C42,
    0x2C43, 0x2C44, 0x2C45, 0x2C46, 0x2C47, 0x2C48, 0x2C49, 0x2C4A,
    0x2C4B, 0x2C4C, 0x2C4D, 0x2C4E, 0x2C4F, 0x2C50, 0x2C51, 0x2C52,
    0x2C53, 0x2C54, 0x2C55, 0x2C56, 0x2C57, 0x2C58, 0x2C59, 0x2C5A,
    0x2C5B, 0x2C5C, 0x2C5D, 0x2C5E, 0x2C5F, 0x2C61, 0x026B, 0x1D7D,
    0x027D, 0x2C68, 0x2C6A, 0x2C6C, 0x0251, 0x0271, 0x0250, 0x0252,
    0x2C73, 0x2C76, 0x023F, 0x0240, 0x2C81, 0x2C83, 0x2C85, 0x2C87,
    0x2C89, 0x2C8B, 0x2C8D, 0x2C8F, 0x2C91, 0x2C93, 0x2C95, 0x2C97,
    0x2C99, 0x2C9B, 0x2C9D, 0x2C9F, 0x2CA1, 0x2CA3, 0x2CA5, 0x2CA7,
    0x2CA9, 0x2CAB, 0x2CAD, 0x2CAF, 0x2CB1, 0x2CB3, 0x2CB5, 0x2CB7,
    0x2CB9, 0x2CBB, 0x2CBD, 0x2CBF, 0x2CC1, 0x2CC3, 0x2CC5, 0x2CC7,
    0x2CC9, 0x2CCB, 0x2CCD, 0x2CCF, 0x2CD1, 0x2CD3, 0x2CD5, 0x2CD7,
    0x2CD9, 0x2CDB, 0x2CDD, 0x2CDF, 0x2CE1, 0x2CE3, 0x2CEC, 0x2CEE,
    0x2CF3, 0xA641, 0xA643, 0xA645, 0xA647, 0xA649, 0xA64B, 0xA64D,
    0xA64F, 0xA651, 0xA653, 0xA655, 0xA657, 0xA659, 0xA65B, 0xA65D,
    0xA65F, 0xA661, 0xA663, 0xA665, 0xA667, 0xA669, 0xA66B, 0xA66D,
    0xA681, 0xA683, 0xA685, 0xA687, 0xA689, 0xA68B, 0xA68D, 0xA68F,
    0xA691, 0xA693, 0xA695, 0xA697, 0xA699, 0xA69B, 0xA723, 0xA725,
    0xA727, 0xA729, 0xA72B, 0xA72D, 0xA72F, 0xA733, 0xA735, 0xA737,
    0xA739, 0xA73B, 0xA73D, 0xA73F, 0xA741, 0xA743, 0xA745, 0xA747,
    0xA749, 0xA74B, 0xA74D, 0xA74F, 0xA751, 0xA753, 0xA755, 0xA757,
    0xA759, 0xA75B, 0xA75D, 0xA75F, 0xA761, 0xA763, 0xA765, 0xA767,
    0xA769, 0xA76B, 0xA76D, 0xA76F, 0xA77A, 0xA77C, 0x1D79, 0xA77F,
    0xA781, 0xA783, 0xA785, 0xA787, 0xA78C, 0x0265, 0xA791, 0xA793,
    0xA797, 0xA799, 0xA79B, 0xA79D, 0xA79F, 0xA7A1, 0xA7A3, 0xA7A5,
    0xA7A7, 0xA7A9, 0x0266, 0x025C, 0x0261, 0x026C, 0x026A, 0x029E,
    0x0287, 0x029D, 0xAB53, 0xA7B5, 0xA7B7, 0xA7B9, 0xA7BB, 0xA7BD,
    0xA7BF, 0xA7C1, 0xA7C3, 0xA794, 0x0282, 0x1D8E, 0xA7C8, 0xA7CA,
    0xA7D1, 0xA7D7, 0xA7D9, 0xA7F6, 0x13A0, 0x13A1, 0x13A2, 0x13A3,
    0x13A4, 0x13A5, 0x13A6, 0x13A7, 0x13A8, 0x13A9, 0x13AA, 0x13AB,
    0x13AC, 0x13AD, 0x13AE, 0x13AF, 0x13B0, 0x13B1, 0x13B2, 0x13B3,
    0x13B4, 0x13B5, 0x13B6, 0x13B7, 0x13B8, 0x13B9, 0x13BA, 0x13BB,
    0x13BC, 0x13BD, 0x13BE, 0x13BF, 0x13C0, 0x13C1, 0x13C2, 0x13C3,
    0x13C4, 0x13C5, 0x13C6, 0x13C7, 0x13C8, 0x13C9, 0x13CA, 0x13CB,
    0x13CC, 0x13CD, 0x13CE, 0x13CF, 0x13D0, 0x13D1, 0x13D2, 0x13D3,
    0x13D4, 0x13D5, 0x13D6, 0x13D7, 0x13D8, 0x13D9, 0x13DA, 0x13DB,
    0x13DC, 0x13DD, 0x13DE, 0x13DF, 0x13E0, 0x13E1, 0x13E2, 0x13E3,
    0x13E4, 0x13E5, 0x13E6, 0x13E7, 0x13E8, 0x13E9, 0x13EA, 0x13EB,
    0x13EC, 0x13ED, 0x13EE, 0x13EF, 0xFF41, 0xFF42, 0xFF43, 0xFF44,
    0xFF45, 0xFF46, 0xFF47, 0xFF48, 0xFF49, 0xFF4A, 0xFF4B, 0xFF4C,
    0xFF4D, 0xFF4E, 0xFF4F, 0xFF50, 0xFF51, 0xFF52, 0xFF53, 0xFF54,
    0xFF55, 0xFF56, 0xFF57, 0xFF58, 0xFF59, 0xFF5A, 0x10428, 0x10429,
    0x1042A, 0x1042B, 0x1042C, 0x1042D, 0x1042E, 0x1042F, 0x10430, 0x10431,
    0x10432, 0x10433, 0x10434, 0x10435, 0x10436, 0x10437, 0x10438, 0x10439,
    0x1043A, 0x1043B, 0x1043C, 0x1043D, 0x1043E, 0x1043F, 0x10440, 0x10441,
    0x10442, 0x10443, 0x10444, 0x10445, 0x10446, 0x10447, 0x10448, 0x10449,
    0x1044A, 0x1044B, 0x1044C, 0x1044D, 0x1044E, 0x1044F, 0x104D8, 0x104D9,
    0x104DA, 0x104DB, 0x104DC, 0x104DD, 0x104DE, 0x104DF, 0x104E0, 0x104E1,
    0x104E2, 0x104E3, 0x104E4, 0x104E5, 0x104E6, 0x104E7, 0x104E8, 0x104E9,
    0x104EA, 0x104EB, 0x104EC, 0x104ED, 0x104EE, 0x104EF, 0x104F0, 0x104F1,
    0x104F2, 0x104F3, 0x104F4, 0x104F5, 0x104F6, 0x104F7, 0x104F8, 0x104F9,
    0x104FA, 0x104FB, 0x10597, 0x10598, 0x10599, 0x1059A, 0x1059B, 0x1059C,
    0x1059D, 0x1059E, 0x1059F, 0x105A0, 0x105A1, 0x105A3, 0x105A4, 0x105A5,
    0x105A6, 0x105A7, 0x105A8, 0x105A9, 0x105AA, 0x105AB, 0x105AC, 0x105AD,
    0x105AE, 0x105AF, 0x105B0, 0x105B1, 0x105B3, 0x105B4, 0x105B5, 0x105B6,
    0x105B7, 0x105B8, 0x105B9, 0x105BB, 0x105BC, 0x10CC0, 0x10CC1, 0x10CC2,
    0x10CC3, 0x10CC4, 0x10CC5, 0x10CC6, 0x10CC7, 0x10CC8, 0x10CC9, 0x10CCA,
    0x10CCB, 0x10CCC, 0x10CCD, 0x10CCE, 0x10CCF, 0x10CD0, 0x10CD1, 0x10CD2,
    0x10CD3, 0x10CD4, 0x10CD5, 0x10CD6, 0x10CD7, 0x10CD8, 0x10CD9, 0x10CDA,
    0x10CDB, 0x10CDC, 0x10CDD, 0x10CDE, 0x10CDF, 0x10CE0, 0x10CE1, 0x10CE2,
    0x10CE3, 0x10CE4, 0x10CE5, 0x10CE6, 0x10CE7, 0x10CE8, 0x10CE9, 0x10CEA,
    0x10CEB, 0x10CEC, 0x10CED, 0x10CEE, 0x10CEF, 0x10CF0, 0x10CF1, 0x10CF2,
    0x118C0, 0x118C1, 0x118C2, 0x118C3, 0x118C4, 0x118C5, 0x118C6, 0x118C7,
    0x118C8, 0x118C9, 0x118CA, 0x118CB, 0x118CC, 0x118CD, 0x118CE, 0x118CF,
    0x118D0, 0x118D1, 0x118D2, 0x118D3, 0x118D4, 0x118D5, 0x118D6, 0x118D7,
    0x118D8, 0x118D9, 0x118DA, 0x118DB, 0x118DC, 0x118DD, 0x118DE, 0x118DF,
    0x16E60, 0x16E61, 0x16E62, 0x16E63, 0x16E64, 0x16E65, 0x16E66, 0x16E67,
    0x16E68, 0x16E69, 0x16E6A, 0x16E6B, 0x16E6C, 0x16E6D, 0x16E6E, 0x16E6F,
    0x16E70, 0x16E71, 0x16E72, 0x16E73, 0x16E74, 0x16E75, 0x16E76, 0x16E77,
    0x16E78, 0x16E79, 0x16E7A, 0x16E7B, 0x16E7C, 0x16E7D, 0x16E7E, 0x16E7F,
    0x1E922, 0x1E923, 0x1E924, 0x1E925, 0x1E926, 0x1E927, 0x1E928, 0x1E929,
    0x1E92A, 0x1E92B, 0x1E92C, 0x1E92D, 0x1E92E, 0x1E92F, 0x1E930, 0x1E931,
    0x1E932, 0x1E933, 0x1E934, 0x1E935, 0x1E936, 0x1E937, 0x1E938, 0x1E939,
    0x1E93A, 0x1E93B, 0x1E93C, 0x1E93D, 0x1E93E, 0x1E93F, 0x1E940, 0x1E941,
    0x1E942, 0x1E943,
};

}
#line 8 "boost/parser/detail/case_fold.hpp"
#include <algorithm>


namespace boost::parser::detail {

    template<typename I>
    std::optional<I> do_short_mapping(
        short_mapping_range const * first,
        short_mapping_range const * last,
        char32_t cp,
        I out)
    {
        auto it = std::lower_bound(
            first,
            last,
            cp,
            [](short_mapping_range const & range, char32_t cp) {
                return range.cp_first_ < cp;
            });
        if (it != first) {
            auto const prev = it - 1;
            if (prev->cp_first_ <= cp && cp < prev->cp_last_)
                it = prev;
        }
        if (it != last && it->cp_first_ <= cp && cp < it->cp_last_) {
            auto const offset = cp - it->cp_first_;
            if (offset % it->stride_ == 0) {
                *out++ =
                    single_mapping_cps[it->first_idx_ + offset / it->stride_];
                return out;
            }
        }

        return std::nullopt;
    }

    template<typename I>
    I case_fold(char32_t cp, I out)
    {

        if (cp < 0x100) {

            if (0x41 <= cp && cp < 0x5a) {
                *out++ = cp + 0x20;
                return out;
            } else if (cp == 0x00DF) {

                *out++ = 0x0073;
                *out++ = 0x0073;
                return out;
            } else {

                auto const first =
                    detail::text::detail::begin(mapping_ranges) + 1;

                auto const last =
                    detail::text::detail::begin(mapping_ranges) + 7;
                if (auto out_opt = do_short_mapping(first, last, cp, out))
                    return *out_opt;
            }
            *out++ = cp;
            return out;
        }


        {
            auto const first = detail::text::detail::begin(mapping_ranges);
            auto const last = detail::text::detail::end(mapping_ranges);
            if (auto out_opt = do_short_mapping(first, last, cp, out))
                return *out_opt;
        }


        {
            auto const last = detail::text::detail::end(long_mappings);
            auto const it = std::lower_bound(
                detail::text::detail::begin(long_mappings),
                last,
                cp,
                [](long_mapping const & mapping, char32_t cp) {
                    return mapping.cp_ < cp;
                });
            if (it != last && it->cp_ == cp) {




                return std::copy(
                    it->mapping_,
                    std::find(
                        detail::text::detail::begin(it->mapping_),
                        detail::text::detail::end(it->mapping_),
                        0),
                    out);

            }
        }

        *out++ = cp;
        return out;
    }

}
#line 7 "boost/parser/detail/unicode_char_sets.hpp"
#define BOOST_PARSER_UNICODE_CHAR_SETS_HPP




namespace boost::parser::detail {
#line 18 "boost/parser/detail/unicode_char_sets.hpp"
    template<>
    struct char_set<punct_chars>
    {
        static constexpr uint32_t chars[] = {
            0x21,    0x22,    0x23,    0x25,    0x26,    0x27,    0x28,
            0x29,    0x2A,    0x5B,    0x5C,    0x5D,    0x5F,    0x7B,
            0x7D,    0x2C,    0x2D,    0x2E,    0x2F,    0x3A,    0x3B,
            0x3F,    0x40,    0xA1,    0xA7,    0xAB,    0xB6,    0xB7,
            0xBB,    0xBF,    0x37E,   0x387,   0x55A,   0x55B,   0x55C,
            0x55D,   0x55E,   0x55F,   0x589,   0x58A,   0x5BE,   0x5C0,
            0x5C3,   0x5C6,   0x5F3,   0x5F4,   0x609,   0x60A,   0x60C,
            0x60D,   0x61B,   0x61D,   0x61E,   0x61F,   0x66A,   0x66B,
            0x66C,   0x66D,   0x6D4,   0x700,   0x701,   0x702,   0x703,
            0x704,   0x705,   0x706,   0x707,   0x708,   0x709,   0x70A,
            0x70B,   0x70C,   0x70D,   0x7F7,   0x7F8,   0x7F9,   0x830,
            0x831,   0x832,   0x833,   0x834,   0x835,   0x836,   0x837,
            0x838,   0x839,   0x83A,   0x83B,   0x83C,   0x83D,   0x83E,
            0x85E,   0x964,   0x965,   0x970,   0x9FD,   0xA76,   0xAF0,
            0xC77,   0xC84,   0xDF4,   0xE4F,   0xE5A,   0xE5B,   0xF04,
            0xF05,   0xF06,   0xF07,   0xFD3,   0xFD4,   0xF08,   0xF09,
            0xF0A,   0xF0B,   0xF0C,   0xF0D,   0xF0E,   0xF0F,   0xF10,
            0xF11,   0xF12,   0xF14,   0xF85,   0xFD0,   0xFD1,   0xFD2,
            0xF3A,   0xF3B,   0xF3C,   0xF3D,   0xFD9,   0xFDA,   0x104A,
            0x104B,  0x104C,  0x104D,  0x104E,  0x104F,  0x10FB,  0x1360,
            0x1361,  0x1362,  0x1363,  0x1364,  0x1365,  0x1366,  0x1367,
            0x1368,  0x1400,  0x166E,  0x169B,  0x169C,  0x16EB,  0x16EC,
            0x16ED,  0x1735,  0x1736,  0x17D4,  0x17D5,  0x17D6,  0x17D8,
            0x17D9,  0x17DA,  0x1800,  0x1801,  0x1802,  0x1803,  0x1804,
            0x1805,  0x1806,  0x1807,  0x1808,  0x1809,  0x180A,  0x1944,
            0x1945,  0x1A1E,  0x1A1F,  0x1AA0,  0x1AA1,  0x1AA2,  0x1AA3,
            0x1AA4,  0x1AA5,  0x1AA6,  0x1AA8,  0x1AA9,  0x1AAA,  0x1AAB,
            0x1AAC,  0x1AAD,  0x1B5A,  0x1B5B,  0x1B5C,  0x1B5D,  0x1B5E,
            0x1B5F,  0x1B60,  0x1B7D,  0x1B7E,  0x1BFC,  0x1BFD,  0x1BFE,
            0x1BFF,  0x1C3B,  0x1C3C,  0x1C3D,  0x1C3E,  0x1C3F,  0x1C7E,
            0x1C7F,  0x1CC0,  0x1CC1,  0x1CC2,  0x1CC3,  0x1CC4,  0x1CC5,
            0x1CC6,  0x1CC7,  0x1CD3,  0x2010,  0x2011,  0x2012,  0x2013,
            0x2014,  0x2015,  0x2016,  0x2017,  0x2020,  0x2021,  0x2022,
            0x2023,  0x2024,  0x2025,  0x2026,  0x2027,  0x2030,  0x2031,
            0x2032,  0x2033,  0x2034,  0x2035,  0x2036,  0x2037,  0x2038,
            0x203B,  0x203D,  0x203E,  0x203F,  0x2040,  0x2041,  0x2042,
            0x2043,  0x204A,  0x204B,  0x204C,  0x204D,  0x204E,  0x204F,
            0x2050,  0x2051,  0x2053,  0x2054,  0x2055,  0x2057,  0x2018,
            0x2019,  0x201A,  0x201B,  0x201C,  0x201D,  0x201E,  0x201F,
            0x2039,  0x203A,  0x203C,  0x2047,  0x2048,  0x2049,  0x2045,
            0x2046,  0x2056,  0x2058,  0x2059,  0x205A,  0x205B,  0x205C,
            0x205D,  0x205E,  0x207D,  0x207E,  0x208D,  0x208E,  0x2308,
            0x2309,  0x230A,  0x230B,  0x2329,  0x232A,  0x2768,  0x2769,
            0x276A,  0x276B,  0x276C,  0x276D,  0x276E,  0x276F,  0x2770,
            0x2771,  0x2772,  0x2773,  0x2774,  0x2775,  0x27C5,  0x27C6,
            0x27E6,  0x27E7,  0x27E8,  0x27E9,  0x27EA,  0x27EB,  0x27EC,
            0x27ED,  0x27EE,  0x27EF,  0x2983,  0x2984,  0x2985,  0x2986,
            0x2987,  0x2988,  0x2989,  0x298A,  0x298B,  0x298C,  0x2991,
            0x2992,  0x2993,  0x2994,  0x2995,  0x2996,  0x2997,  0x2998,
            0x29FC,  0x29FD,  0x298D,  0x298E,  0x298F,  0x2990,  0x29D8,
            0x29D9,  0x29DA,  0x29DB,  0x2CF9,  0x2CFA,  0x2CFB,  0x2CFC,
            0x2CFE,  0x2CFF,  0x2D70,  0x2E00,  0x2E01,  0x2E02,  0x2E03,
            0x2E04,  0x2E05,  0x2E06,  0x2E07,  0x2E08,  0x2E09,  0x2E0A,
            0x2E0B,  0x2E0C,  0x2E0D,  0x2E0E,  0x2E0F,  0x2E10,  0x2E11,
            0x2E12,  0x2E13,  0x2E14,  0x2E15,  0x2E16,  0x2E17,  0x2E18,
            0x2E19,  0x2E1A,  0x2E1B,  0x2E1E,  0x2E1F,  0x2E1C,  0x2E1D,
            0x2E20,  0x2E21,  0x2E26,  0x2E27,  0x2E28,  0x2E29,  0x2E55,
            0x2E56,  0x2E57,  0x2E58,  0x2E22,  0x2E23,  0x2E24,  0x2E25,
            0x2E2A,  0x2E2B,  0x2E2C,  0x2E2D,  0x2E2E,  0x2E30,  0x2E31,
            0x2E33,  0x2E34,  0x2E3F,  0x2E4A,  0x2E4B,  0x2E4C,  0x2E4D,
            0x2E4E,  0x2E4F,  0x2E52,  0x2E53,  0x2E54,  0x2E32,  0x2E35,
            0x2E36,  0x2E37,  0x2E38,  0x2E39,  0x2E3A,  0x2E3B,  0x2E3C,
            0x2E3D,  0x2E3E,  0x2E40,  0x2E41,  0x2E42,  0x2E43,  0x2E44,
            0x2E45,  0x2E46,  0x2E47,  0x2E48,  0x2E49,  0x2E59,  0x2E5A,
            0x2E5B,  0x2E5C,  0x2E5D,  0x3001,  0x3002,  0x3003,  0x3008,
            0x3009,  0x300A,  0x300B,  0x300C,  0x300D,  0x300E,  0x300F,
            0x3010,  0x3011,  0x3014,  0x3015,  0x3016,  0x3017,  0x3018,
            0x3019,  0x301A,  0x301B,  0x301C,  0x301D,  0x301E,  0x301F,
            0x3030,  0x303D,  0x30A0,  0x30FB,  0xA4FE,  0xA4FF,  0xA60D,
            0xA60E,  0xA60F,  0xA673,  0xA67E,  0xA6F2,  0xA6F3,  0xA6F4,
            0xA6F5,  0xA6F6,  0xA6F7,  0xA874,  0xA875,  0xA876,  0xA877,
            0xA8CE,  0xA8CF,  0xA8F8,  0xA8F9,  0xA8FA,  0xA8FC,  0xA92E,
            0xA92F,  0xA95F,  0xA9C1,  0xA9C2,  0xA9C3,  0xA9C4,  0xA9C5,
            0xA9C6,  0xA9C7,  0xA9C8,  0xA9C9,  0xA9CA,  0xA9CB,  0xA9CC,
            0xA9CD,  0xA9DE,  0xA9DF,  0xAA5C,  0xAA5D,  0xAA5E,  0xAA5F,
            0xAADE,  0xAADF,  0xAAF0,  0xAAF1,  0xABEB,  0xFD3E,  0xFD3F,
            0xFE10,  0xFE11,  0xFE12,  0xFE13,  0xFE14,  0xFE15,  0xFE16,
            0xFE17,  0xFE18,  0xFE19,  0xFE30,  0xFE31,  0xFE32,  0xFE33,
            0xFE34,  0xFE35,  0xFE36,  0xFE37,  0xFE38,  0xFE39,  0xFE3A,
            0xFE3B,  0xFE3C,  0xFE3D,  0xFE3E,  0xFE3F,  0xFE40,  0xFE41,
            0xFE42,  0xFE43,  0xFE44,  0xFE47,  0xFE48,  0xFE45,  0xFE46,
            0xFE49,  0xFE4A,  0xFE4B,  0xFE4C,  0xFE4D,  0xFE4E,  0xFE4F,
            0xFE50,  0xFE51,  0xFE52,  0xFE54,  0xFE55,  0xFE56,  0xFE57,
            0xFE58,  0xFE59,  0xFE5A,  0xFE5B,  0xFE5C,  0xFE5D,  0xFE5E,
            0xFE5F,  0xFE60,  0xFE61,  0xFE63,  0xFE68,  0xFE6A,  0xFE6B,
            0xFF01,  0xFF02,  0xFF03,  0xFF05,  0xFF06,  0xFF07,  0xFF08,
            0xFF09,  0xFF0A,  0xFF0C,  0xFF0D,  0xFF0E,  0xFF0F,  0xFF1A,
            0xFF1B,  0xFF1F,  0xFF20,  0xFF3B,  0xFF3C,  0xFF3D,  0xFF3F,
            0xFF5B,  0xFF5D,  0xFF5F,  0xFF60,  0xFF61,  0xFF62,  0xFF63,
            0xFF64,  0xFF65,  0x10100, 0x10101, 0x10102, 0x1039F, 0x103D0,
            0x1056F, 0x10857, 0x1091F, 0x1093F, 0x10A50, 0x10A51, 0x10A52,
            0x10A53, 0x10A54, 0x10A55, 0x10A56, 0x10A57, 0x10A58, 0x10A7F,
            0x10AF0, 0x10AF1, 0x10AF2, 0x10AF3, 0x10AF4, 0x10AF5, 0x10AF6,
            0x10B39, 0x10B3A, 0x10B3B, 0x10B3C, 0x10B3D, 0x10B3E, 0x10B3F,
            0x10B99, 0x10B9A, 0x10B9B, 0x10B9C, 0x10EAD, 0x10F55, 0x10F56,
            0x10F57, 0x10F58, 0x10F59, 0x10F86, 0x10F87, 0x10F88, 0x10F89,
            0x11047, 0x11048, 0x11049, 0x1104A, 0x1104B, 0x1104C, 0x1104D,
            0x110BB, 0x110BC, 0x110BE, 0x110BF, 0x110C0, 0x110C1, 0x11140,
            0x11141, 0x11142, 0x11143, 0x11174, 0x11175, 0x111C5, 0x111C6,
            0x111C7, 0x111C8, 0x111CD, 0x111DB, 0x111DD, 0x111DE, 0x111DF,
            0x11238, 0x11239, 0x1123A, 0x1123B, 0x1123C, 0x1123D, 0x112A9,
            0x1144B, 0x1144C, 0x1144D, 0x1144E, 0x1144F, 0x1145A, 0x1145B,
            0x1145D, 0x114C6, 0x115C1, 0x115C2, 0x115C3, 0x115C4, 0x115C5,
            0x115C6, 0x115C7, 0x115C8, 0x115C9, 0x115CA, 0x115CB, 0x115CC,
            0x115CD, 0x115CE, 0x115CF, 0x115D0, 0x115D1, 0x115D2, 0x115D3,
            0x115D4, 0x115D5, 0x115D6, 0x115D7, 0x11641, 0x11642, 0x11643,
            0x11660, 0x11661, 0x11662, 0x11663, 0x11664, 0x11665, 0x11666,
            0x11667, 0x11668, 0x11669, 0x1166A, 0x1166B, 0x1166C, 0x116B9,
            0x1173C, 0x1173D, 0x1173E, 0x1183B, 0x11944, 0x11945, 0x11946,
            0x119E2, 0x11A3F, 0x11A40, 0x11A45, 0x11A46, 0x11A41, 0x11A42,
            0x11A43, 0x11A44, 0x11A9A, 0x11A9B, 0x11A9C, 0x11A9E, 0x11A9F,
            0x11AA0, 0x11AA1, 0x11AA2, 0x11B00, 0x11B01, 0x11B02, 0x11B03,
            0x11B04, 0x11B05, 0x11B06, 0x11B07, 0x11B08, 0x11B09, 0x11C41,
            0x11C42, 0x11C43, 0x11C44, 0x11C45, 0x11C70, 0x11C71, 0x11EF7,
            0x11EF8, 0x11F43, 0x11F44, 0x11F45, 0x11F46, 0x11F47, 0x11F48,
            0x11F49, 0x11F4A, 0x11F4B, 0x11F4C, 0x11F4D, 0x11F4E, 0x11F4F,
            0x11FFF, 0x12470, 0x12471, 0x12472, 0x12473, 0x12474, 0x12FF1,
            0x12FF2, 0x16A6E, 0x16A6F, 0x16AF5, 0x16B37, 0x16B38, 0x16B39,
            0x16B3A, 0x16B3B, 0x16B44, 0x16E97, 0x16E98, 0x16E99, 0x16E9A,
            0x16FE2, 0x1BC9F, 0x1DA87, 0x1DA88, 0x1DA89, 0x1DA8A, 0x1DA8B,
            0x1E95E, 0x1E95F};
    };
#line 148 "boost/parser/detail/unicode_char_sets.hpp"
    template<>
    struct char_set<lower_case_chars>
    {
        static constexpr uint32_t chars[] = {
        0x61,    0x62,    0x63,    0x64,    0x65,    0x66,    0x67,    0x68,
        0x69,    0x6A,    0x6B,    0x6C,    0x6D,    0x6E,    0x6F,    0x70,
        0x71,    0x72,    0x73,    0x74,    0x75,    0x76,    0x77,    0x78,
        0x79,    0x7A,    0xB5,    0xDF,    0xE0,    0xE1,    0xE2,    0xE3,
        0xE4,    0xE5,    0xE6,    0xE7,    0xE8,    0xE9,    0xEA,    0xEB,
        0xEC,    0xED,    0xEE,    0xEF,    0xF0,    0xF1,    0xF2,    0xF3,
        0xF4,    0xF5,    0xF6,    0xF8,    0xF9,    0xFA,    0xFB,    0xFC,
        0xFD,    0xFE,    0xFF,    0x101,   0x103,   0x105,   0x107,   0x109,
        0x10B,   0x10D,   0x10F,   0x111,   0x113,   0x115,   0x117,   0x119,
        0x11B,   0x11D,   0x11F,   0x121,   0x123,   0x125,   0x127,   0x129,
        0x12B,   0x12D,   0x12F,   0x131,   0x133,   0x135,   0x137,   0x138,
        0x13A,   0x13C,   0x13E,   0x140,   0x142,   0x144,   0x146,   0x148,
        0x149,   0x14B,   0x14D,   0x14F,   0x151,   0x153,   0x155,   0x157,
        0x159,   0x15B,   0x15D,   0x15F,   0x161,   0x163,   0x165,   0x167,
        0x169,   0x16B,   0x16D,   0x16F,   0x171,   0x173,   0x175,   0x177,
        0x17A,   0x17C,   0x17E,   0x17F,   0x180,   0x183,   0x185,   0x188,
        0x18C,   0x18D,   0x192,   0x195,   0x199,   0x19A,   0x19B,   0x19E,
        0x1A1,   0x1A3,   0x1A5,   0x1A8,   0x1AA,   0x1AB,   0x1AD,   0x1B0,
        0x1B4,   0x1B6,   0x1B9,   0x1BA,   0x1BD,   0x1BE,   0x1BF,   0x1C6,
        0x1C9,   0x1CC,   0x1CE,   0x1D0,   0x1D2,   0x1D4,   0x1D6,   0x1D8,
        0x1DA,   0x1DC,   0x1DD,   0x1DF,   0x1E1,   0x1E3,   0x1E5,   0x1E7,
        0x1E9,   0x1EB,   0x1ED,   0x1EF,   0x1F0,   0x1F3,   0x1F5,   0x1F9,
        0x1FB,   0x1FD,   0x1FF,   0x201,   0x203,   0x205,   0x207,   0x209,
        0x20B,   0x20D,   0x20F,   0x211,   0x213,   0x215,   0x217,   0x219,
        0x21B,   0x21D,   0x21F,   0x221,   0x223,   0x225,   0x227,   0x229,
        0x22B,   0x22D,   0x22F,   0x231,   0x233,   0x234,   0x235,   0x236,
        0x237,   0x238,   0x239,   0x23C,   0x23F,   0x240,   0x242,   0x247,
        0x249,   0x24B,   0x24D,   0x24F,   0x250,   0x251,   0x252,   0x253,
        0x254,   0x255,   0x256,   0x257,   0x258,   0x259,   0x25A,   0x25B,
        0x25C,   0x25D,   0x25E,   0x25F,   0x260,   0x261,   0x262,   0x263,
        0x264,   0x265,   0x266,   0x267,   0x268,   0x269,   0x26A,   0x26B,
        0x26C,   0x26D,   0x26E,   0x26F,   0x270,   0x271,   0x272,   0x273,
        0x274,   0x275,   0x276,   0x277,   0x278,   0x279,   0x27A,   0x27B,
        0x27C,   0x27D,   0x27E,   0x27F,   0x280,   0x281,   0x282,   0x283,
        0x284,   0x285,   0x286,   0x287,   0x288,   0x289,   0x28A,   0x28B,
        0x28C,   0x28D,   0x28E,   0x28F,   0x290,   0x291,   0x292,   0x293,
        0x295,   0x296,   0x297,   0x298,   0x299,   0x29A,   0x29B,   0x29C,
        0x29D,   0x29E,   0x29F,   0x2A0,   0x2A1,   0x2A2,   0x2A3,   0x2A4,
        0x2A5,   0x2A6,   0x2A7,   0x2A8,   0x2A9,   0x2AA,   0x2AB,   0x2AC,
        0x2AD,   0x2AE,   0x2AF,   0x371,   0x373,   0x377,   0x37B,   0x37C,
        0x37D,   0x390,   0x3AC,   0x3AD,   0x3AE,   0x3AF,   0x3B0,   0x3B1,
        0x3B2,   0x3B3,   0x3B4,   0x3B5,   0x3B6,   0x3B7,   0x3B8,   0x3B9,
        0x3BA,   0x3BB,   0x3BC,   0x3BD,   0x3BE,   0x3BF,   0x3C0,   0x3C1,
        0x3C2,   0x3C3,   0x3C4,   0x3C5,   0x3C6,   0x3C7,   0x3C8,   0x3C9,
        0x3CA,   0x3CB,   0x3CC,   0x3CD,   0x3CE,   0x3D0,   0x3D1,   0x3D5,
        0x3D6,   0x3D7,   0x3D9,   0x3DB,   0x3DD,   0x3DF,   0x3E1,   0x3E3,
        0x3E5,   0x3E7,   0x3E9,   0x3EB,   0x3ED,   0x3EF,   0x3F0,   0x3F1,
        0x3F2,   0x3F3,   0x3F5,   0x3F8,   0x3FB,   0x3FC,   0x430,   0x431,
        0x432,   0x433,   0x434,   0x435,   0x436,   0x437,   0x438,   0x439,
        0x43A,   0x43B,   0x43C,   0x43D,   0x43E,   0x43F,   0x440,   0x441,
        0x442,   0x443,   0x444,   0x445,   0x446,   0x447,   0x448,   0x449,
        0x44A,   0x44B,   0x44C,   0x44D,   0x44E,   0x44F,   0x450,   0x451,
        0x452,   0x453,   0x454,   0x455,   0x456,   0x457,   0x458,   0x459,
        0x45A,   0x45B,   0x45C,   0x45D,   0x45E,   0x45F,   0x461,   0x463,
        0x465,   0x467,   0x469,   0x46B,   0x46D,   0x46F,   0x471,   0x473,
        0x475,   0x477,   0x479,   0x47B,   0x47D,   0x47F,   0x481,   0x48B,
        0x48D,   0x48F,   0x491,   0x493,   0x495,   0x497,   0x499,   0x49B,
        0x49D,   0x49F,   0x4A1,   0x4A3,   0x4A5,   0x4A7,   0x4A9,   0x4AB,
        0x4AD,   0x4AF,   0x4B1,   0x4B3,   0x4B5,   0x4B7,   0x4B9,   0x4BB,
        0x4BD,   0x4BF,   0x4C2,   0x4C4,   0x4C6,   0x4C8,   0x4CA,   0x4CC,
        0x4CE,   0x4CF,   0x4D1,   0x4D3,   0x4D5,   0x4D7,   0x4D9,   0x4DB,
        0x4DD,   0x4DF,   0x4E1,   0x4E3,   0x4E5,   0x4E7,   0x4E9,   0x4EB,
        0x4ED,   0x4EF,   0x4F1,   0x4F3,   0x4F5,   0x4F7,   0x4F9,   0x4FB,
        0x4FD,   0x4FF,   0x501,   0x503,   0x505,   0x507,   0x509,   0x50B,
        0x50D,   0x50F,   0x511,   0x513,   0x515,   0x517,   0x519,   0x51B,
        0x51D,   0x51F,   0x521,   0x523,   0x525,   0x527,   0x529,   0x52B,
        0x52D,   0x52F,   0x560,   0x561,   0x562,   0x563,   0x564,   0x565,
        0x566,   0x567,   0x568,   0x569,   0x56A,   0x56B,   0x56C,   0x56D,
        0x56E,   0x56F,   0x570,   0x571,   0x572,   0x573,   0x574,   0x575,
        0x576,   0x577,   0x578,   0x579,   0x57A,   0x57B,   0x57C,   0x57D,
        0x57E,   0x57F,   0x580,   0x581,   0x582,   0x583,   0x584,   0x585,
        0x586,   0x587,   0x588,   0x10D0,  0x10D1,  0x10D2,  0x10D3,  0x10D4,
        0x10D5,  0x10D6,  0x10D7,  0x10D8,  0x10D9,  0x10DA,  0x10DB,  0x10DC,
        0x10DD,  0x10DE,  0x10DF,  0x10E0,  0x10E1,  0x10E2,  0x10E3,  0x10E4,
        0x10E5,  0x10E6,  0x10E7,  0x10E8,  0x10E9,  0x10EA,  0x10EB,  0x10EC,
        0x10ED,  0x10EE,  0x10EF,  0x10F0,  0x10F1,  0x10F2,  0x10F3,  0x10F4,
        0x10F5,  0x10F6,  0x10F7,  0x10F8,  0x10F9,  0x10FA,  0x10FD,  0x10FE,
        0x10FF,  0x13F8,  0x13F9,  0x13FA,  0x13FB,  0x13FC,  0x13FD,  0x1C80,
        0x1C81,  0x1C82,  0x1C83,  0x1C84,  0x1C85,  0x1C86,  0x1C87,  0x1C88,
        0x1D00,  0x1D01,  0x1D02,  0x1D03,  0x1D04,  0x1D05,  0x1D06,  0x1D07,
        0x1D08,  0x1D09,  0x1D0A,  0x1D0B,  0x1D0C,  0x1D0D,  0x1D0E,  0x1D0F,
        0x1D10,  0x1D11,  0x1D12,  0x1D13,  0x1D14,  0x1D15,  0x1D16,  0x1D17,
        0x1D18,  0x1D19,  0x1D1A,  0x1D1B,  0x1D1C,  0x1D1D,  0x1D1E,  0x1D1F,
        0x1D20,  0x1D21,  0x1D22,  0x1D23,  0x1D24,  0x1D25,  0x1D26,  0x1D27,
        0x1D28,  0x1D29,  0x1D2A,  0x1D2B,  0x1D6B,  0x1D6C,  0x1D6D,  0x1D6E,
        0x1D6F,  0x1D70,  0x1D71,  0x1D72,  0x1D73,  0x1D74,  0x1D75,  0x1D76,
        0x1D77,  0x1D79,  0x1D7A,  0x1D7B,  0x1D7C,  0x1D7D,  0x1D7E,  0x1D7F,
        0x1D80,  0x1D81,  0x1D82,  0x1D83,  0x1D84,  0x1D85,  0x1D86,  0x1D87,
        0x1D88,  0x1D89,  0x1D8A,  0x1D8B,  0x1D8C,  0x1D8D,  0x1D8E,  0x1D8F,
        0x1D90,  0x1D91,  0x1D92,  0x1D93,  0x1D94,  0x1D95,  0x1D96,  0x1D97,
        0x1D98,  0x1D99,  0x1D9A,  0x1E01,  0x1E03,  0x1E05,  0x1E07,  0x1E09,
        0x1E0B,  0x1E0D,  0x1E0F,  0x1E11,  0x1E13,  0x1E15,  0x1E17,  0x1E19,
        0x1E1B,  0x1E1D,  0x1E1F,  0x1E21,  0x1E23,  0x1E25,  0x1E27,  0x1E29,
        0x1E2B,  0x1E2D,  0x1E2F,  0x1E31,  0x1E33,  0x1E35,  0x1E37,  0x1E39,
        0x1E3B,  0x1E3D,  0x1E3F,  0x1E41,  0x1E43,  0x1E45,  0x1E47,  0x1E49,
        0x1E4B,  0x1E4D,  0x1E4F,  0x1E51,  0x1E53,  0x1E55,  0x1E57,  0x1E59,
        0x1E5B,  0x1E5D,  0x1E5F,  0x1E61,  0x1E63,  0x1E65,  0x1E67,  0x1E69,
        0x1E6B,  0x1E6D,  0x1E6F,  0x1E71,  0x1E73,  0x1E75,  0x1E77,  0x1E79,
        0x1E7B,  0x1E7D,  0x1E7F,  0x1E81,  0x1E83,  0x1E85,  0x1E87,  0x1E89,
        0x1E8B,  0x1E8D,  0x1E8F,  0x1E91,  0x1E93,  0x1E95,  0x1E96,  0x1E97,
        0x1E98,  0x1E99,  0x1E9A,  0x1E9B,  0x1E9C,  0x1E9D,  0x1E9F,  0x1EA1,
        0x1EA3,  0x1EA5,  0x1EA7,  0x1EA9,  0x1EAB,  0x1EAD,  0x1EAF,  0x1EB1,
        0x1EB3,  0x1EB5,  0x1EB7,  0x1EB9,  0x1EBB,  0x1EBD,  0x1EBF,  0x1EC1,
        0x1EC3,  0x1EC5,  0x1EC7,  0x1EC9,  0x1ECB,  0x1ECD,  0x1ECF,  0x1ED1,
        0x1ED3,  0x1ED5,  0x1ED7,  0x1ED9,  0x1EDB,  0x1EDD,  0x1EDF,  0x1EE1,
        0x1EE3,  0x1EE5,  0x1EE7,  0x1EE9,  0x1EEB,  0x1EED,  0x1EEF,  0x1EF1,
        0x1EF3,  0x1EF5,  0x1EF7,  0x1EF9,  0x1EFB,  0x1EFD,  0x1EFF,  0x1F00,
        0x1F01,  0x1F02,  0x1F03,  0x1F04,  0x1F05,  0x1F06,  0x1F07,  0x1F10,
        0x1F11,  0x1F12,  0x1F13,  0x1F14,  0x1F15,  0x1F20,  0x1F21,  0x1F22,
        0x1F23,  0x1F24,  0x1F25,  0x1F26,  0x1F27,  0x1F30,  0x1F31,  0x1F32,
        0x1F33,  0x1F34,  0x1F35,  0x1F36,  0x1F37,  0x1F40,  0x1F41,  0x1F42,
        0x1F43,  0x1F44,  0x1F45,  0x1F50,  0x1F51,  0x1F52,  0x1F53,  0x1F54,
        0x1F55,  0x1F56,  0x1F57,  0x1F60,  0x1F61,  0x1F62,  0x1F63,  0x1F64,
        0x1F65,  0x1F66,  0x1F67,  0x1F70,  0x1F71,  0x1F72,  0x1F73,  0x1F74,
        0x1F75,  0x1F76,  0x1F77,  0x1F78,  0x1F79,  0x1F7A,  0x1F7B,  0x1F7C,
        0x1F7D,  0x1F80,  0x1F81,  0x1F82,  0x1F83,  0x1F84,  0x1F85,  0x1F86,
        0x1F87,  0x1F90,  0x1F91,  0x1F92,  0x1F93,  0x1F94,  0x1F95,  0x1F96,
        0x1F97,  0x1FA0,  0x1FA1,  0x1FA2,  0x1FA3,  0x1FA4,  0x1FA5,  0x1FA6,
        0x1FA7,  0x1FB0,  0x1FB1,  0x1FB2,  0x1FB3,  0x1FB4,  0x1FB6,  0x1FB7,
        0x1FBE,  0x1FC2,  0x1FC3,  0x1FC4,  0x1FC6,  0x1FC7,  0x1FD0,  0x1FD1,
        0x1FD2,  0x1FD3,  0x1FD6,  0x1FD7,  0x1FE0,  0x1FE1,  0x1FE2,  0x1FE3,
        0x1FE4,  0x1FE5,  0x1FE6,  0x1FE7,  0x1FF2,  0x1FF3,  0x1FF4,  0x1FF6,
        0x1FF7,  0x210A,  0x210E,  0x210F,  0x2113,  0x212F,  0x2134,  0x2139,
        0x213C,  0x213D,  0x2146,  0x2147,  0x2148,  0x2149,  0x214E,  0x2184,
        0x2C30,  0x2C31,  0x2C32,  0x2C33,  0x2C34,  0x2C35,  0x2C36,  0x2C37,
        0x2C38,  0x2C39,  0x2C3A,  0x2C3B,  0x2C3C,  0x2C3D,  0x2C3E,  0x2C3F,
        0x2C40,  0x2C41,  0x2C42,  0x2C43,  0x2C44,  0x2C45,  0x2C46,  0x2C47,
        0x2C48,  0x2C49,  0x2C4A,  0x2C4B,  0x2C4C,  0x2C4D,  0x2C4E,  0x2C4F,
        0x2C50,  0x2C51,  0x2C52,  0x2C53,  0x2C54,  0x2C55,  0x2C56,  0x2C57,
        0x2C58,  0x2C59,  0x2C5A,  0x2C5B,  0x2C5C,  0x2C5D,  0x2C5E,  0x2C5F,
        0x2C61,  0x2C65,  0x2C66,  0x2C68,  0x2C6A,  0x2C6C,  0x2C71,  0x2C73,
        0x2C74,  0x2C76,  0x2C77,  0x2C78,  0x2C79,  0x2C7A,  0x2C7B,  0x2C81,
        0x2C83,  0x2C85,  0x2C87,  0x2C89,  0x2C8B,  0x2C8D,  0x2C8F,  0x2C91,
        0x2C93,  0x2C95,  0x2C97,  0x2C99,  0x2C9B,  0x2C9D,  0x2C9F,  0x2CA1,
        0x2CA3,  0x2CA5,  0x2CA7,  0x2CA9,  0x2CAB,  0x2CAD,  0x2CAF,  0x2CB1,
        0x2CB3,  0x2CB5,  0x2CB7,  0x2CB9,  0x2CBB,  0x2CBD,  0x2CBF,  0x2CC1,
        0x2CC3,  0x2CC5,  0x2CC7,  0x2CC9,  0x2CCB,  0x2CCD,  0x2CCF,  0x2CD1,
        0x2CD3,  0x2CD5,  0x2CD7,  0x2CD9,  0x2CDB,  0x2CDD,  0x2CDF,  0x2CE1,
        0x2CE3,  0x2CE4,  0x2CEC,  0x2CEE,  0x2CF3,  0x2D00,  0x2D01,  0x2D02,
        0x2D03,  0x2D04,  0x2D05,  0x2D06,  0x2D07,  0x2D08,  0x2D09,  0x2D0A,
        0x2D0B,  0x2D0C,  0x2D0D,  0x2D0E,  0x2D0F,  0x2D10,  0x2D11,  0x2D12,
        0x2D13,  0x2D14,  0x2D15,  0x2D16,  0x2D17,  0x2D18,  0x2D19,  0x2D1A,
        0x2D1B,  0x2D1C,  0x2D1D,  0x2D1E,  0x2D1F,  0x2D20,  0x2D21,  0x2D22,
        0x2D23,  0x2D24,  0x2D25,  0x2D27,  0x2D2D,  0xA641,  0xA643,  0xA645,
        0xA647,  0xA649,  0xA64B,  0xA64D,  0xA64F,  0xA651,  0xA653,  0xA655,
        0xA657,  0xA659,  0xA65B,  0xA65D,  0xA65F,  0xA661,  0xA663,  0xA665,
        0xA667,  0xA669,  0xA66B,  0xA66D,  0xA681,  0xA683,  0xA685,  0xA687,
        0xA689,  0xA68B,  0xA68D,  0xA68F,  0xA691,  0xA693,  0xA695,  0xA697,
        0xA699,  0xA69B,  0xA723,  0xA725,  0xA727,  0xA729,  0xA72B,  0xA72D,
        0xA72F,  0xA730,  0xA731,  0xA733,  0xA735,  0xA737,  0xA739,  0xA73B,
        0xA73D,  0xA73F,  0xA741,  0xA743,  0xA745,  0xA747,  0xA749,  0xA74B,
        0xA74D,  0xA74F,  0xA751,  0xA753,  0xA755,  0xA757,  0xA759,  0xA75B,
        0xA75D,  0xA75F,  0xA761,  0xA763,  0xA765,  0xA767,  0xA769,  0xA76B,
        0xA76D,  0xA76F,  0xA771,  0xA772,  0xA773,  0xA774,  0xA775,  0xA776,
        0xA777,  0xA778,  0xA77A,  0xA77C,  0xA77F,  0xA781,  0xA783,  0xA785,
        0xA787,  0xA78C,  0xA78E,  0xA791,  0xA793,  0xA794,  0xA795,  0xA797,
        0xA799,  0xA79B,  0xA79D,  0xA79F,  0xA7A1,  0xA7A3,  0xA7A5,  0xA7A7,
        0xA7A9,  0xA7AF,  0xA7B5,  0xA7B7,  0xA7B9,  0xA7BB,  0xA7BD,  0xA7BF,
        0xA7C1,  0xA7C3,  0xA7C8,  0xA7CA,  0xA7D1,  0xA7D3,  0xA7D5,  0xA7D7,
        0xA7D9,  0xA7F6,  0xA7FA,  0xAB30,  0xAB31,  0xAB32,  0xAB33,  0xAB34,
        0xAB35,  0xAB36,  0xAB37,  0xAB38,  0xAB39,  0xAB3A,  0xAB3B,  0xAB3C,
        0xAB3D,  0xAB3E,  0xAB3F,  0xAB40,  0xAB41,  0xAB42,  0xAB43,  0xAB44,
        0xAB45,  0xAB46,  0xAB47,  0xAB48,  0xAB49,  0xAB4A,  0xAB4B,  0xAB4C,
        0xAB4D,  0xAB4E,  0xAB4F,  0xAB50,  0xAB51,  0xAB52,  0xAB53,  0xAB54,
        0xAB55,  0xAB56,  0xAB57,  0xAB58,  0xAB59,  0xAB5A,  0xAB60,  0xAB61,
        0xAB62,  0xAB63,  0xAB64,  0xAB65,  0xAB66,  0xAB67,  0xAB68,  0xAB70,
        0xAB71,  0xAB72,  0xAB73,  0xAB74,  0xAB75,  0xAB76,  0xAB77,  0xAB78,
        0xAB79,  0xAB7A,  0xAB7B,  0xAB7C,  0xAB7D,  0xAB7E,  0xAB7F,  0xAB80,
        0xAB81,  0xAB82,  0xAB83,  0xAB84,  0xAB85,  0xAB86,  0xAB87,  0xAB88,
        0xAB89,  0xAB8A,  0xAB8B,  0xAB8C,  0xAB8D,  0xAB8E,  0xAB8F,  0xAB90,
        0xAB91,  0xAB92,  0xAB93,  0xAB94,  0xAB95,  0xAB96,  0xAB97,  0xAB98,
        0xAB99,  0xAB9A,  0xAB9B,  0xAB9C,  0xAB9D,  0xAB9E,  0xAB9F,  0xABA0,
        0xABA1,  0xABA2,  0xABA3,  0xABA4,  0xABA5,  0xABA6,  0xABA7,  0xABA8,
        0xABA9,  0xABAA,  0xABAB,  0xABAC,  0xABAD,  0xABAE,  0xABAF,  0xABB0,
        0xABB1,  0xABB2,  0xABB3,  0xABB4,  0xABB5,  0xABB6,  0xABB7,  0xABB8,
        0xABB9,  0xABBA,  0xABBB,  0xABBC,  0xABBD,  0xABBE,  0xABBF,  0xFB00,
        0xFB01,  0xFB02,  0xFB03,  0xFB04,  0xFB05,  0xFB06,  0xFB13,  0xFB14,
        0xFB15,  0xFB16,  0xFB17,  0xFF41,  0xFF42,  0xFF43,  0xFF44,  0xFF45,
        0xFF46,  0xFF47,  0xFF48,  0xFF49,  0xFF4A,  0xFF4B,  0xFF4C,  0xFF4D,
        0xFF4E,  0xFF4F,  0xFF50,  0xFF51,  0xFF52,  0xFF53,  0xFF54,  0xFF55,
        0xFF56,  0xFF57,  0xFF58,  0xFF59,  0xFF5A,  0x10428, 0x10429, 0x1042A,
        0x1042B, 0x1042C, 0x1042D, 0x1042E, 0x1042F, 0x10430, 0x10431, 0x10432,
        0x10433, 0x10434, 0x10435, 0x10436, 0x10437, 0x10438, 0x10439, 0x1043A,
        0x1043B, 0x1043C, 0x1043D, 0x1043E, 0x1043F, 0x10440, 0x10441, 0x10442,
        0x10443, 0x10444, 0x10445, 0x10446, 0x10447, 0x10448, 0x10449, 0x1044A,
        0x1044B, 0x1044C, 0x1044D, 0x1044E, 0x1044F, 0x104D8, 0x104D9, 0x104DA,
        0x104DB, 0x104DC, 0x104DD, 0x104DE, 0x104DF, 0x104E0, 0x104E1, 0x104E2,
        0x104E3, 0x104E4, 0x104E5, 0x104E6, 0x104E7, 0x104E8, 0x104E9, 0x104EA,
        0x104EB, 0x104EC, 0x104ED, 0x104EE, 0x104EF, 0x104F0, 0x104F1, 0x104F2,
        0x104F3, 0x104F4, 0x104F5, 0x104F6, 0x104F7, 0x104F8, 0x104F9, 0x104FA,
        0x104FB, 0x10597, 0x10598, 0x10599, 0x1059A, 0x1059B, 0x1059C, 0x1059D,
        0x1059E, 0x1059F, 0x105A0, 0x105A1, 0x105A3, 0x105A4, 0x105A5, 0x105A6,
        0x105A7, 0x105A8, 0x105A9, 0x105AA, 0x105AB, 0x105AC, 0x105AD, 0x105AE,
        0x105AF, 0x105B0, 0x105B1, 0x105B3, 0x105B4, 0x105B5, 0x105B6, 0x105B7,
        0x105B8, 0x105B9, 0x105BB, 0x105BC, 0x10CC0, 0x10CC1, 0x10CC2, 0x10CC3,
        0x10CC4, 0x10CC5, 0x10CC6, 0x10CC7, 0x10CC8, 0x10CC9, 0x10CCA, 0x10CCB,
        0x10CCC, 0x10CCD, 0x10CCE, 0x10CCF, 0x10CD0, 0x10CD1, 0x10CD2, 0x10CD3,
        0x10CD4, 0x10CD5, 0x10CD6, 0x10CD7, 0x10CD8, 0x10CD9, 0x10CDA, 0x10CDB,
        0x10CDC, 0x10CDD, 0x10CDE, 0x10CDF, 0x10CE0, 0x10CE1, 0x10CE2, 0x10CE3,
        0x10CE4, 0x10CE5, 0x10CE6, 0x10CE7, 0x10CE8, 0x10CE9, 0x10CEA, 0x10CEB,
        0x10CEC, 0x10CED, 0x10CEE, 0x10CEF, 0x10CF0, 0x10CF1, 0x10CF2, 0x118C0,
        0x118C1, 0x118C2, 0x118C3, 0x118C4, 0x118C5, 0x118C6, 0x118C7, 0x118C8,
        0x118C9, 0x118CA, 0x118CB, 0x118CC, 0x118CD, 0x118CE, 0x118CF, 0x118D0,
        0x118D1, 0x118D2, 0x118D3, 0x118D4, 0x118D5, 0x118D6, 0x118D7, 0x118D8,
        0x118D9, 0x118DA, 0x118DB, 0x118DC, 0x118DD, 0x118DE, 0x118DF, 0x16E60,
        0x16E61, 0x16E62, 0x16E63, 0x16E64, 0x16E65, 0x16E66, 0x16E67, 0x16E68,
        0x16E69, 0x16E6A, 0x16E6B, 0x16E6C, 0x16E6D, 0x16E6E, 0x16E6F, 0x16E70,
        0x16E71, 0x16E72, 0x16E73, 0x16E74, 0x16E75, 0x16E76, 0x16E77, 0x16E78,
        0x16E79, 0x16E7A, 0x16E7B, 0x16E7C, 0x16E7D, 0x16E7E, 0x16E7F, 0x1D41A,
        0x1D41B, 0x1D41C, 0x1D41D, 0x1D41E, 0x1D41F, 0x1D420, 0x1D421, 0x1D422,
        0x1D423, 0x1D424, 0x1D425, 0x1D426, 0x1D427, 0x1D428, 0x1D429, 0x1D42A,
        0x1D42B, 0x1D42C, 0x1D42D, 0x1D42E, 0x1D42F, 0x1D430, 0x1D431, 0x1D432,
        0x1D433, 0x1D44E, 0x1D44F, 0x1D450, 0x1D451, 0x1D452, 0x1D453, 0x1D454,
        0x1D456, 0x1D457, 0x1D458, 0x1D459, 0x1D45A, 0x1D45B, 0x1D45C, 0x1D45D,
        0x1D45E, 0x1D45F, 0x1D460, 0x1D461, 0x1D462, 0x1D463, 0x1D464, 0x1D465,
        0x1D466, 0x1D467, 0x1D482, 0x1D483, 0x1D484, 0x1D485, 0x1D486, 0x1D487,
        0x1D488, 0x1D489, 0x1D48A, 0x1D48B, 0x1D48C, 0x1D48D, 0x1D48E, 0x1D48F,
        0x1D490, 0x1D491, 0x1D492, 0x1D493, 0x1D494, 0x1D495, 0x1D496, 0x1D497,
        0x1D498, 0x1D499, 0x1D49A, 0x1D49B, 0x1D4B6, 0x1D4B7, 0x1D4B8, 0x1D4B9,
        0x1D4BB, 0x1D4BD, 0x1D4BE, 0x1D4BF, 0x1D4C0, 0x1D4C1, 0x1D4C2, 0x1D4C3,
        0x1D4C5, 0x1D4C6, 0x1D4C7, 0x1D4C8, 0x1D4C9, 0x1D4CA, 0x1D4CB, 0x1D4CC,
        0x1D4CD, 0x1D4CE, 0x1D4CF, 0x1D4EA, 0x1D4EB, 0x1D4EC, 0x1D4ED, 0x1D4EE,
        0x1D4EF, 0x1D4F0, 0x1D4F1, 0x1D4F2, 0x1D4F3, 0x1D4F4, 0x1D4F5, 0x1D4F6,
        0x1D4F7, 0x1D4F8, 0x1D4F9, 0x1D4FA, 0x1D4FB, 0x1D4FC, 0x1D4FD, 0x1D4FE,
        0x1D4FF, 0x1D500, 0x1D501, 0x1D502, 0x1D503, 0x1D51E, 0x1D51F, 0x1D520,
        0x1D521, 0x1D522, 0x1D523, 0x1D524, 0x1D525, 0x1D526, 0x1D527, 0x1D528,
        0x1D529, 0x1D52A, 0x1D52B, 0x1D52C, 0x1D52D, 0x1D52E, 0x1D52F, 0x1D530,
        0x1D531, 0x1D532, 0x1D533, 0x1D534, 0x1D535, 0x1D536, 0x1D537, 0x1D552,
        0x1D553, 0x1D554, 0x1D555, 0x1D556, 0x1D557, 0x1D558, 0x1D559, 0x1D55A,
        0x1D55B, 0x1D55C, 0x1D55D, 0x1D55E, 0x1D55F, 0x1D560, 0x1D561, 0x1D562,
        0x1D563, 0x1D564, 0x1D565, 0x1D566, 0x1D567, 0x1D568, 0x1D569, 0x1D56A,
        0x1D56B, 0x1D586, 0x1D587, 0x1D588, 0x1D589, 0x1D58A, 0x1D58B, 0x1D58C,
        0x1D58D, 0x1D58E, 0x1D58F, 0x1D590, 0x1D591, 0x1D592, 0x1D593, 0x1D594,
        0x1D595, 0x1D596, 0x1D597, 0x1D598, 0x1D599, 0x1D59A, 0x1D59B, 0x1D59C,
        0x1D59D, 0x1D59E, 0x1D59F, 0x1D5BA, 0x1D5BB, 0x1D5BC, 0x1D5BD, 0x1D5BE,
        0x1D5BF, 0x1D5C0, 0x1D5C1, 0x1D5C2, 0x1D5C3, 0x1D5C4, 0x1D5C5, 0x1D5C6,
        0x1D5C7, 0x1D5C8, 0x1D5C9, 0x1D5CA, 0x1D5CB, 0x1D5CC, 0x1D5CD, 0x1D5CE,
        0x1D5CF, 0x1D5D0, 0x1D5D1, 0x1D5D2, 0x1D5D3, 0x1D5EE, 0x1D5EF, 0x1D5F0,
        0x1D5F1, 0x1D5F2, 0x1D5F3, 0x1D5F4, 0x1D5F5, 0x1D5F6, 0x1D5F7, 0x1D5F8,
        0x1D5F9, 0x1D5FA, 0x1D5FB, 0x1D5FC, 0x1D5FD, 0x1D5FE, 0x1D5FF, 0x1D600,
        0x1D601, 0x1D602, 0x1D603, 0x1D604, 0x1D605, 0x1D606, 0x1D607, 0x1D622,
        0x1D623, 0x1D624, 0x1D625, 0x1D626, 0x1D627, 0x1D628, 0x1D629, 0x1D62A,
        0x1D62B, 0x1D62C, 0x1D62D, 0x1D62E, 0x1D62F, 0x1D630, 0x1D631, 0x1D632,
        0x1D633, 0x1D634, 0x1D635, 0x1D636, 0x1D637, 0x1D638, 0x1D639, 0x1D63A,
        0x1D63B, 0x1D656, 0x1D657, 0x1D658, 0x1D659, 0x1D65A, 0x1D65B, 0x1D65C,
        0x1D65D, 0x1D65E, 0x1D65F, 0x1D660, 0x1D661, 0x1D662, 0x1D663, 0x1D664,
        0x1D665, 0x1D666, 0x1D667, 0x1D668, 0x1D669, 0x1D66A, 0x1D66B, 0x1D66C,
        0x1D66D, 0x1D66E, 0x1D66F, 0x1D68A, 0x1D68B, 0x1D68C, 0x1D68D, 0x1D68E,
        0x1D68F, 0x1D690, 0x1D691, 0x1D692, 0x1D693, 0x1D694, 0x1D695, 0x1D696,
        0x1D697, 0x1D698, 0x1D699, 0x1D69A, 0x1D69B, 0x1D69C, 0x1D69D, 0x1D69E,
        0x1D69F, 0x1D6A0, 0x1D6A1, 0x1D6A2, 0x1D6A3, 0x1D6A4, 0x1D6A5, 0x1D6C2,
        0x1D6C3, 0x1D6C4, 0x1D6C5, 0x1D6C6, 0x1D6C7, 0x1D6C8, 0x1D6C9, 0x1D6CA,
        0x1D6CB, 0x1D6CC, 0x1D6CD, 0x1D6CE, 0x1D6CF, 0x1D6D0, 0x1D6D1, 0x1D6D2,
        0x1D6D3, 0x1D6D4, 0x1D6D5, 0x1D6D6, 0x1D6D7, 0x1D6D8, 0x1D6D9, 0x1D6DA,
        0x1D6DC, 0x1D6DD, 0x1D6DE, 0x1D6DF, 0x1D6E0, 0x1D6E1, 0x1D6FC, 0x1D6FD,
        0x1D6FE, 0x1D6FF, 0x1D700, 0x1D701, 0x1D702, 0x1D703, 0x1D704, 0x1D705,
        0x1D706, 0x1D707, 0x1D708, 0x1D709, 0x1D70A, 0x1D70B, 0x1D70C, 0x1D70D,
        0x1D70E, 0x1D70F, 0x1D710, 0x1D711, 0x1D712, 0x1D713, 0x1D714, 0x1D716,
        0x1D717, 0x1D718, 0x1D719, 0x1D71A, 0x1D71B, 0x1D736, 0x1D737, 0x1D738,
        0x1D739, 0x1D73A, 0x1D73B, 0x1D73C, 0x1D73D, 0x1D73E, 0x1D73F, 0x1D740,
        0x1D741, 0x1D742, 0x1D743, 0x1D744, 0x1D745, 0x1D746, 0x1D747, 0x1D748,
        0x1D749, 0x1D74A, 0x1D74B, 0x1D74C, 0x1D74D, 0x1D74E, 0x1D750, 0x1D751,
        0x1D752, 0x1D753, 0x1D754, 0x1D755, 0x1D770, 0x1D771, 0x1D772, 0x1D773,
        0x1D774, 0x1D775, 0x1D776, 0x1D777, 0x1D778, 0x1D779, 0x1D77A, 0x1D77B,
        0x1D77C, 0x1D77D, 0x1D77E, 0x1D77F, 0x1D780, 0x1D781, 0x1D782, 0x1D783,
        0x1D784, 0x1D785, 0x1D786, 0x1D787, 0x1D788, 0x1D78A, 0x1D78B, 0x1D78C,
        0x1D78D, 0x1D78E, 0x1D78F, 0x1D7AA, 0x1D7AB, 0x1D7AC, 0x1D7AD, 0x1D7AE,
        0x1D7AF, 0x1D7B0, 0x1D7B1, 0x1D7B2, 0x1D7B3, 0x1D7B4, 0x1D7B5, 0x1D7B6,
        0x1D7B7, 0x1D7B8, 0x1D7B9, 0x1D7BA, 0x1D7BB, 0x1D7BC, 0x1D7BD, 0x1D7BE,
        0x1D7BF, 0x1D7C0, 0x1D7C1, 0x1D7C2, 0x1D7C4, 0x1D7C5, 0x1D7C6, 0x1D7C7,
        0x1D7C8, 0x1D7C9, 0x1D7CB, 0x1DF00, 0x1DF01, 0x1DF02, 0x1DF03, 0x1DF04,
        0x1DF05, 0x1DF06, 0x1DF07, 0x1DF08, 0x1DF09, 0x1DF0B, 0x1DF0C, 0x1DF0D,
        0x1DF0E, 0x1DF0F, 0x1DF10, 0x1DF11, 0x1DF12, 0x1DF13, 0x1DF14, 0x1DF15,
        0x1DF16, 0x1DF17, 0x1DF18, 0x1DF19, 0x1DF1A, 0x1DF1B, 0x1DF1C, 0x1DF1D,
        0x1DF1E, 0x1DF25, 0x1DF26, 0x1DF27, 0x1DF28, 0x1DF29, 0x1DF2A, 0x1E922,
        0x1E923, 0x1E924, 0x1E925, 0x1E926, 0x1E927, 0x1E928, 0x1E929, 0x1E92A,
        0x1E92B, 0x1E92C, 0x1E92D, 0x1E92E, 0x1E92F, 0x1E930, 0x1E931, 0x1E932,
        0x1E933, 0x1E934, 0x1E935, 0x1E936, 0x1E937, 0x1E938, 0x1E939, 0x1E93A,
        0x1E93B, 0x1E93C, 0x1E93D, 0x1E93E, 0x1E93F, 0x1E940, 0x1E941, 0x1E942,
        0x1E943};
    };
#line 437 "boost/parser/detail/unicode_char_sets.hpp"
    template<>
    struct char_set<upper_case_chars>
    {
        static constexpr uint32_t chars[] = {
            0x41,    0x42,    0x43,    0x44,    0x45,    0x46,    0x47,
            0x48,    0x49,    0x4A,    0x4B,    0x4C,    0x4D,    0x4E,
            0x4F,    0x50,    0x51,    0x52,    0x53,    0x54,    0x55,
            0x56,    0x57,    0x58,    0x59,    0x5A,    0xC0,    0xC1,
            0xC2,    0xC3,    0xC4,    0xC5,    0xC6,    0xC7,    0xC8,
            0xC9,    0xCA,    0xCB,    0xCC,    0xCD,    0xCE,    0xCF,
            0xD0,    0xD1,    0xD2,    0xD3,    0xD4,    0xD5,    0xD6,
            0xD8,    0xD9,    0xDA,    0xDB,    0xDC,    0xDD,    0xDE,
            0x100,   0x102,   0x104,   0x106,   0x108,   0x10A,   0x10C,
            0x10E,   0x110,   0x112,   0x114,   0x116,   0x118,   0x11A,
            0x11C,   0x11E,   0x120,   0x122,   0x124,   0x126,   0x128,
            0x12A,   0x12C,   0x12E,   0x130,   0x132,   0x134,   0x136,
            0x139,   0x13B,   0x13D,   0x13F,   0x141,   0x143,   0x145,
            0x147,   0x14A,   0x14C,   0x14E,   0x150,   0x152,   0x154,
            0x156,   0x158,   0x15A,   0x15C,   0x15E,   0x160,   0x162,
            0x164,   0x166,   0x168,   0x16A,   0x16C,   0x16E,   0x170,
            0x172,   0x174,   0x176,   0x178,   0x179,   0x17B,   0x17D,
            0x181,   0x182,   0x184,   0x186,   0x187,   0x189,   0x18A,
            0x18B,   0x18E,   0x18F,   0x190,   0x191,   0x193,   0x194,
            0x196,   0x197,   0x198,   0x19C,   0x19D,   0x19F,   0x1A0,
            0x1A2,   0x1A4,   0x1A6,   0x1A7,   0x1A9,   0x1AC,   0x1AE,
            0x1AF,   0x1B1,   0x1B2,   0x1B3,   0x1B5,   0x1B7,   0x1B8,
            0x1BC,   0x1C4,   0x1C7,   0x1CA,   0x1CD,   0x1CF,   0x1D1,
            0x1D3,   0x1D5,   0x1D7,   0x1D9,   0x1DB,   0x1DE,   0x1E0,
            0x1E2,   0x1E4,   0x1E6,   0x1E8,   0x1EA,   0x1EC,   0x1EE,
            0x1F1,   0x1F4,   0x1F6,   0x1F7,   0x1F8,   0x1FA,   0x1FC,
            0x1FE,   0x200,   0x202,   0x204,   0x206,   0x208,   0x20A,
            0x20C,   0x20E,   0x210,   0x212,   0x214,   0x216,   0x218,
            0x21A,   0x21C,   0x21E,   0x220,   0x222,   0x224,   0x226,
            0x228,   0x22A,   0x22C,   0x22E,   0x230,   0x232,   0x23A,
            0x23B,   0x23D,   0x23E,   0x241,   0x243,   0x244,   0x245,
            0x246,   0x248,   0x24A,   0x24C,   0x24E,   0x370,   0x372,
            0x376,   0x37F,   0x386,   0x388,   0x389,   0x38A,   0x38C,
            0x38E,   0x38F,   0x391,   0x392,   0x393,   0x394,   0x395,
            0x396,   0x397,   0x398,   0x399,   0x39A,   0x39B,   0x39C,
            0x39D,   0x39E,   0x39F,   0x3A0,   0x3A1,   0x3A3,   0x3A4,
            0x3A5,   0x3A6,   0x3A7,   0x3A8,   0x3A9,   0x3AA,   0x3AB,
            0x3CF,   0x3D2,   0x3D3,   0x3D4,   0x3D8,   0x3DA,   0x3DC,
            0x3DE,   0x3E0,   0x3E2,   0x3E4,   0x3E6,   0x3E8,   0x3EA,
            0x3EC,   0x3EE,   0x3F4,   0x3F7,   0x3F9,   0x3FA,   0x3FD,
            0x3FE,   0x3FF,   0x400,   0x401,   0x402,   0x403,   0x404,
            0x405,   0x406,   0x407,   0x408,   0x409,   0x40A,   0x40B,
            0x40C,   0x40D,   0x40E,   0x40F,   0x410,   0x411,   0x412,
            0x413,   0x414,   0x415,   0x416,   0x417,   0x418,   0x419,
            0x41A,   0x41B,   0x41C,   0x41D,   0x41E,   0x41F,   0x420,
            0x421,   0x422,   0x423,   0x424,   0x425,   0x426,   0x427,
            0x428,   0x429,   0x42A,   0x42B,   0x42C,   0x42D,   0x42E,
            0x42F,   0x460,   0x462,   0x464,   0x466,   0x468,   0x46A,
            0x46C,   0x46E,   0x470,   0x472,   0x474,   0x476,   0x478,
            0x47A,   0x47C,   0x47E,   0x480,   0x48A,   0x48C,   0x48E,
            0x490,   0x492,   0x494,   0x496,   0x498,   0x49A,   0x49C,
            0x49E,   0x4A0,   0x4A2,   0x4A4,   0x4A6,   0x4A8,   0x4AA,
            0x4AC,   0x4AE,   0x4B0,   0x4B2,   0x4B4,   0x4B6,   0x4B8,
            0x4BA,   0x4BC,   0x4BE,   0x4C0,   0x4C1,   0x4C3,   0x4C5,
            0x4C7,   0x4C9,   0x4CB,   0x4CD,   0x4D0,   0x4D2,   0x4D4,
            0x4D6,   0x4D8,   0x4DA,   0x4DC,   0x4DE,   0x4E0,   0x4E2,
            0x4E4,   0x4E6,   0x4E8,   0x4EA,   0x4EC,   0x4EE,   0x4F0,
            0x4F2,   0x4F4,   0x4F6,   0x4F8,   0x4FA,   0x4FC,   0x4FE,
            0x500,   0x502,   0x504,   0x506,   0x508,   0x50A,   0x50C,
            0x50E,   0x510,   0x512,   0x514,   0x516,   0x518,   0x51A,
            0x51C,   0x51E,   0x520,   0x522,   0x524,   0x526,   0x528,
            0x52A,   0x52C,   0x52E,   0x531,   0x532,   0x533,   0x534,
            0x535,   0x536,   0x537,   0x538,   0x539,   0x53A,   0x53B,
            0x53C,   0x53D,   0x53E,   0x53F,   0x540,   0x541,   0x542,
            0x543,   0x544,   0x545,   0x546,   0x547,   0x548,   0x549,
            0x54A,   0x54B,   0x54C,   0x54D,   0x54E,   0x54F,   0x550,
            0x551,   0x552,   0x553,   0x554,   0x555,   0x556,   0x10A0,
            0x10A1,  0x10A2,  0x10A3,  0x10A4,  0x10A5,  0x10A6,  0x10A7,
            0x10A8,  0x10A9,  0x10AA,  0x10AB,  0x10AC,  0x10AD,  0x10AE,
            0x10AF,  0x10B0,  0x10B1,  0x10B2,  0x10B3,  0x10B4,  0x10B5,
            0x10B6,  0x10B7,  0x10B8,  0x10B9,  0x10BA,  0x10BB,  0x10BC,
            0x10BD,  0x10BE,  0x10BF,  0x10C0,  0x10C1,  0x10C2,  0x10C3,
            0x10C4,  0x10C5,  0x10C7,  0x10CD,  0x13A0,  0x13A1,  0x13A2,
            0x13A3,  0x13A4,  0x13A5,  0x13A6,  0x13A7,  0x13A8,  0x13A9,
            0x13AA,  0x13AB,  0x13AC,  0x13AD,  0x13AE,  0x13AF,  0x13B0,
            0x13B1,  0x13B2,  0x13B3,  0x13B4,  0x13B5,  0x13B6,  0x13B7,
            0x13B8,  0x13B9,  0x13BA,  0x13BB,  0x13BC,  0x13BD,  0x13BE,
            0x13BF,  0x13C0,  0x13C1,  0x13C2,  0x13C3,  0x13C4,  0x13C5,
            0x13C6,  0x13C7,  0x13C8,  0x13C9,  0x13CA,  0x13CB,  0x13CC,
            0x13CD,  0x13CE,  0x13CF,  0x13D0,  0x13D1,  0x13D2,  0x13D3,
            0x13D4,  0x13D5,  0x13D6,  0x13D7,  0x13D8,  0x13D9,  0x13DA,
            0x13DB,  0x13DC,  0x13DD,  0x13DE,  0x13DF,  0x13E0,  0x13E1,
            0x13E2,  0x13E3,  0x13E4,  0x13E5,  0x13E6,  0x13E7,  0x13E8,
            0x13E9,  0x13EA,  0x13EB,  0x13EC,  0x13ED,  0x13EE,  0x13EF,
            0x13F0,  0x13F1,  0x13F2,  0x13F3,  0x13F4,  0x13F5,  0x1C90,
            0x1C91,  0x1C92,  0x1C93,  0x1C94,  0x1C95,  0x1C96,  0x1C97,
            0x1C98,  0x1C99,  0x1C9A,  0x1C9B,  0x1C9C,  0x1C9D,  0x1C9E,
            0x1C9F,  0x1CA0,  0x1CA1,  0x1CA2,  0x1CA3,  0x1CA4,  0x1CA5,
            0x1CA6,  0x1CA7,  0x1CA8,  0x1CA9,  0x1CAA,  0x1CAB,  0x1CAC,
            0x1CAD,  0x1CAE,  0x1CAF,  0x1CB0,  0x1CB1,  0x1CB2,  0x1CB3,
            0x1CB4,  0x1CB5,  0x1CB6,  0x1CB7,  0x1CB8,  0x1CB9,  0x1CBA,
            0x1CBD,  0x1CBE,  0x1CBF,  0x1E00,  0x1E02,  0x1E04,  0x1E06,
            0x1E08,  0x1E0A,  0x1E0C,  0x1E0E,  0x1E10,  0x1E12,  0x1E14,
            0x1E16,  0x1E18,  0x1E1A,  0x1E1C,  0x1E1E,  0x1E20,  0x1E22,
            0x1E24,  0x1E26,  0x1E28,  0x1E2A,  0x1E2C,  0x1E2E,  0x1E30,
            0x1E32,  0x1E34,  0x1E36,  0x1E38,  0x1E3A,  0x1E3C,  0x1E3E,
            0x1E40,  0x1E42,  0x1E44,  0x1E46,  0x1E48,  0x1E4A,  0x1E4C,
            0x1E4E,  0x1E50,  0x1E52,  0x1E54,  0x1E56,  0x1E58,  0x1E5A,
            0x1E5C,  0x1E5E,  0x1E60,  0x1E62,  0x1E64,  0x1E66,  0x1E68,
            0x1E6A,  0x1E6C,  0x1E6E,  0x1E70,  0x1E72,  0x1E74,  0x1E76,
            0x1E78,  0x1E7A,  0x1E7C,  0x1E7E,  0x1E80,  0x1E82,  0x1E84,
            0x1E86,  0x1E88,  0x1E8A,  0x1E8C,  0x1E8E,  0x1E90,  0x1E92,
            0x1E94,  0x1E9E,  0x1EA0,  0x1EA2,  0x1EA4,  0x1EA6,  0x1EA8,
            0x1EAA,  0x1EAC,  0x1EAE,  0x1EB0,  0x1EB2,  0x1EB4,  0x1EB6,
            0x1EB8,  0x1EBA,  0x1EBC,  0x1EBE,  0x1EC0,  0x1EC2,  0x1EC4,
            0x1EC6,  0x1EC8,  0x1ECA,  0x1ECC,  0x1ECE,  0x1ED0,  0x1ED2,
            0x1ED4,  0x1ED6,  0x1ED8,  0x1EDA,  0x1EDC,  0x1EDE,  0x1EE0,
            0x1EE2,  0x1EE4,  0x1EE6,  0x1EE8,  0x1EEA,  0x1EEC,  0x1EEE,
            0x1EF0,  0x1EF2,  0x1EF4,  0x1EF6,  0x1EF8,  0x1EFA,  0x1EFC,
            0x1EFE,  0x1F08,  0x1F09,  0x1F0A,  0x1F0B,  0x1F0C,  0x1F0D,
            0x1F0E,  0x1F0F,  0x1F18,  0x1F19,  0x1F1A,  0x1F1B,  0x1F1C,
            0x1F1D,  0x1F28,  0x1F29,  0x1F2A,  0x1F2B,  0x1F2C,  0x1F2D,
            0x1F2E,  0x1F2F,  0x1F38,  0x1F39,  0x1F3A,  0x1F3B,  0x1F3C,
            0x1F3D,  0x1F3E,  0x1F3F,  0x1F48,  0x1F49,  0x1F4A,  0x1F4B,
            0x1F4C,  0x1F4D,  0x1F59,  0x1F5B,  0x1F5D,  0x1F5F,  0x1F68,
            0x1F69,  0x1F6A,  0x1F6B,  0x1F6C,  0x1F6D,  0x1F6E,  0x1F6F,
            0x1FB8,  0x1FB9,  0x1FBA,  0x1FBB,  0x1FC8,  0x1FC9,  0x1FCA,
            0x1FCB,  0x1FD8,  0x1FD9,  0x1FDA,  0x1FDB,  0x1FE8,  0x1FE9,
            0x1FEA,  0x1FEB,  0x1FEC,  0x1FF8,  0x1FF9,  0x1FFA,  0x1FFB,
            0x2102,  0x2107,  0x210B,  0x210C,  0x210D,  0x2110,  0x2111,
            0x2112,  0x2115,  0x2119,  0x211A,  0x211B,  0x211C,  0x211D,
            0x2124,  0x2126,  0x2128,  0x212A,  0x212B,  0x212C,  0x212D,
            0x2130,  0x2131,  0x2132,  0x2133,  0x213E,  0x213F,  0x2145,
            0x2183,  0x2C00,  0x2C01,  0x2C02,  0x2C03,  0x2C04,  0x2C05,
            0x2C06,  0x2C07,  0x2C08,  0x2C09,  0x2C0A,  0x2C0B,  0x2C0C,
            0x2C0D,  0x2C0E,  0x2C0F,  0x2C10,  0x2C11,  0x2C12,  0x2C13,
            0x2C14,  0x2C15,  0x2C16,  0x2C17,  0x2C18,  0x2C19,  0x2C1A,
            0x2C1B,  0x2C1C,  0x2C1D,  0x2C1E,  0x2C1F,  0x2C20,  0x2C21,
            0x2C22,  0x2C23,  0x2C24,  0x2C25,  0x2C26,  0x2C27,  0x2C28,
            0x2C29,  0x2C2A,  0x2C2B,  0x2C2C,  0x2C2D,  0x2C2E,  0x2C2F,
            0x2C60,  0x2C62,  0x2C63,  0x2C64,  0x2C67,  0x2C69,  0x2C6B,
            0x2C6D,  0x2C6E,  0x2C6F,  0x2C70,  0x2C72,  0x2C75,  0x2C7E,
            0x2C7F,  0x2C80,  0x2C82,  0x2C84,  0x2C86,  0x2C88,  0x2C8A,
            0x2C8C,  0x2C8E,  0x2C90,  0x2C92,  0x2C94,  0x2C96,  0x2C98,
            0x2C9A,  0x2C9C,  0x2C9E,  0x2CA0,  0x2CA2,  0x2CA4,  0x2CA6,
            0x2CA8,  0x2CAA,  0x2CAC,  0x2CAE,  0x2CB0,  0x2CB2,  0x2CB4,
            0x2CB6,  0x2CB8,  0x2CBA,  0x2CBC,  0x2CBE,  0x2CC0,  0x2CC2,
            0x2CC4,  0x2CC6,  0x2CC8,  0x2CCA,  0x2CCC,  0x2CCE,  0x2CD0,
            0x2CD2,  0x2CD4,  0x2CD6,  0x2CD8,  0x2CDA,  0x2CDC,  0x2CDE,
            0x2CE0,  0x2CE2,  0x2CEB,  0x2CED,  0x2CF2,  0xA640,  0xA642,
            0xA644,  0xA646,  0xA648,  0xA64A,  0xA64C,  0xA64E,  0xA650,
            0xA652,  0xA654,  0xA656,  0xA658,  0xA65A,  0xA65C,  0xA65E,
            0xA660,  0xA662,  0xA664,  0xA666,  0xA668,  0xA66A,  0xA66C,
            0xA680,  0xA682,  0xA684,  0xA686,  0xA688,  0xA68A,  0xA68C,
            0xA68E,  0xA690,  0xA692,  0xA694,  0xA696,  0xA698,  0xA69A,
            0xA722,  0xA724,  0xA726,  0xA728,  0xA72A,  0xA72C,  0xA72E,
            0xA732,  0xA734,  0xA736,  0xA738,  0xA73A,  0xA73C,  0xA73E,
            0xA740,  0xA742,  0xA744,  0xA746,  0xA748,  0xA74A,  0xA74C,
            0xA74E,  0xA750,  0xA752,  0xA754,  0xA756,  0xA758,  0xA75A,
            0xA75C,  0xA75E,  0xA760,  0xA762,  0xA764,  0xA766,  0xA768,
            0xA76A,  0xA76C,  0xA76E,  0xA779,  0xA77B,  0xA77D,  0xA77E,
            0xA780,  0xA782,  0xA784,  0xA786,  0xA78B,  0xA78D,  0xA790,
            0xA792,  0xA796,  0xA798,  0xA79A,  0xA79C,  0xA79E,  0xA7A0,
            0xA7A2,  0xA7A4,  0xA7A6,  0xA7A8,  0xA7AA,  0xA7AB,  0xA7AC,
            0xA7AD,  0xA7AE,  0xA7B0,  0xA7B1,  0xA7B2,  0xA7B3,  0xA7B4,
            0xA7B6,  0xA7B8,  0xA7BA,  0xA7BC,  0xA7BE,  0xA7C0,  0xA7C2,
            0xA7C4,  0xA7C5,  0xA7C6,  0xA7C7,  0xA7C9,  0xA7D0,  0xA7D6,
            0xA7D8,  0xA7F5,  0xFF21,  0xFF22,  0xFF23,  0xFF24,  0xFF25,
            0xFF26,  0xFF27,  0xFF28,  0xFF29,  0xFF2A,  0xFF2B,  0xFF2C,
            0xFF2D,  0xFF2E,  0xFF2F,  0xFF30,  0xFF31,  0xFF32,  0xFF33,
            0xFF34,  0xFF35,  0xFF36,  0xFF37,  0xFF38,  0xFF39,  0xFF3A,
            0x10400, 0x10401, 0x10402, 0x10403, 0x10404, 0x10405, 0x10406,
            0x10407, 0x10408, 0x10409, 0x1040A, 0x1040B, 0x1040C, 0x1040D,
            0x1040E, 0x1040F, 0x10410, 0x10411, 0x10412, 0x10413, 0x10414,
            0x10415, 0x10416, 0x10417, 0x10418, 0x10419, 0x1041A, 0x1041B,
            0x1041C, 0x1041D, 0x1041E, 0x1041F, 0x10420, 0x10421, 0x10422,
            0x10423, 0x10424, 0x10425, 0x10426, 0x10427, 0x104B0, 0x104B1,
            0x104B2, 0x104B3, 0x104B4, 0x104B5, 0x104B6, 0x104B7, 0x104B8,
            0x104B9, 0x104BA, 0x104BB, 0x104BC, 0x104BD, 0x104BE, 0x104BF,
            0x104C0, 0x104C1, 0x104C2, 0x104C3, 0x104C4, 0x104C5, 0x104C6,
            0x104C7, 0x104C8, 0x104C9, 0x104CA, 0x104CB, 0x104CC, 0x104CD,
            0x104CE, 0x104CF, 0x104D0, 0x104D1, 0x104D2, 0x104D3, 0x10570,
            0x10571, 0x10572, 0x10573, 0x10574, 0x10575, 0x10576, 0x10577,
            0x10578, 0x10579, 0x1057A, 0x1057C, 0x1057D, 0x1057E, 0x1057F,
            0x10580, 0x10581, 0x10582, 0x10583, 0x10584, 0x10585, 0x10586,
            0x10587, 0x10588, 0x10589, 0x1058A, 0x1058C, 0x1058D, 0x1058E,
            0x1058F, 0x10590, 0x10591, 0x10592, 0x10594, 0x10595, 0x10C80,
            0x10C81, 0x10C82, 0x10C83, 0x10C84, 0x10C85, 0x10C86, 0x10C87,
            0x10C88, 0x10C89, 0x10C8A, 0x10C8B, 0x10C8C, 0x10C8D, 0x10C8E,
            0x10C8F, 0x10C90, 0x10C91, 0x10C92, 0x10C93, 0x10C94, 0x10C95,
            0x10C96, 0x10C97, 0x10C98, 0x10C99, 0x10C9A, 0x10C9B, 0x10C9C,
            0x10C9D, 0x10C9E, 0x10C9F, 0x10CA0, 0x10CA1, 0x10CA2, 0x10CA3,
            0x10CA4, 0x10CA5, 0x10CA6, 0x10CA7, 0x10CA8, 0x10CA9, 0x10CAA,
            0x10CAB, 0x10CAC, 0x10CAD, 0x10CAE, 0x10CAF, 0x10CB0, 0x10CB1,
            0x10CB2, 0x118A0, 0x118A1, 0x118A2, 0x118A3, 0x118A4, 0x118A5,
            0x118A6, 0x118A7, 0x118A8, 0x118A9, 0x118AA, 0x118AB, 0x118AC,
            0x118AD, 0x118AE, 0x118AF, 0x118B0, 0x118B1, 0x118B2, 0x118B3,
            0x118B4, 0x118B5, 0x118B6, 0x118B7, 0x118B8, 0x118B9, 0x118BA,
            0x118BB, 0x118BC, 0x118BD, 0x118BE, 0x118BF, 0x16E40, 0x16E41,
            0x16E42, 0x16E43, 0x16E44, 0x16E45, 0x16E46, 0x16E47, 0x16E48,
            0x16E49, 0x16E4A, 0x16E4B, 0x16E4C, 0x16E4D, 0x16E4E, 0x16E4F,
            0x16E50, 0x16E51, 0x16E52, 0x16E53, 0x16E54, 0x16E55, 0x16E56,
            0x16E57, 0x16E58, 0x16E59, 0x16E5A, 0x16E5B, 0x16E5C, 0x16E5D,
            0x16E5E, 0x16E5F, 0x1D400, 0x1D401, 0x1D402, 0x1D403, 0x1D404,
            0x1D405, 0x1D406, 0x1D407, 0x1D408, 0x1D409, 0x1D40A, 0x1D40B,
            0x1D40C, 0x1D40D, 0x1D40E, 0x1D40F, 0x1D410, 0x1D411, 0x1D412,
            0x1D413, 0x1D414, 0x1D415, 0x1D416, 0x1D417, 0x1D418, 0x1D419,
            0x1D434, 0x1D435, 0x1D436, 0x1D437, 0x1D438, 0x1D439, 0x1D43A,
            0x1D43B, 0x1D43C, 0x1D43D, 0x1D43E, 0x1D43F, 0x1D440, 0x1D441,
            0x1D442, 0x1D443, 0x1D444, 0x1D445, 0x1D446, 0x1D447, 0x1D448,
            0x1D449, 0x1D44A, 0x1D44B, 0x1D44C, 0x1D44D, 0x1D468, 0x1D469,
            0x1D46A, 0x1D46B, 0x1D46C, 0x1D46D, 0x1D46E, 0x1D46F, 0x1D470,
            0x1D471, 0x1D472, 0x1D473, 0x1D474, 0x1D475, 0x1D476, 0x1D477,
            0x1D478, 0x1D479, 0x1D47A, 0x1D47B, 0x1D47C, 0x1D47D, 0x1D47E,
            0x1D47F, 0x1D480, 0x1D481, 0x1D49C, 0x1D49E, 0x1D49F, 0x1D4A2,
            0x1D4A5, 0x1D4A6, 0x1D4A9, 0x1D4AA, 0x1D4AB, 0x1D4AC, 0x1D4AE,
            0x1D4AF, 0x1D4B0, 0x1D4B1, 0x1D4B2, 0x1D4B3, 0x1D4B4, 0x1D4B5,
            0x1D4D0, 0x1D4D1, 0x1D4D2, 0x1D4D3, 0x1D4D4, 0x1D4D5, 0x1D4D6,
            0x1D4D7, 0x1D4D8, 0x1D4D9, 0x1D4DA, 0x1D4DB, 0x1D4DC, 0x1D4DD,
            0x1D4DE, 0x1D4DF, 0x1D4E0, 0x1D4E1, 0x1D4E2, 0x1D4E3, 0x1D4E4,
            0x1D4E5, 0x1D4E6, 0x1D4E7, 0x1D4E8, 0x1D4E9, 0x1D504, 0x1D505,
            0x1D507, 0x1D508, 0x1D509, 0x1D50A, 0x1D50D, 0x1D50E, 0x1D50F,
            0x1D510, 0x1D511, 0x1D512, 0x1D513, 0x1D514, 0x1D516, 0x1D517,
            0x1D518, 0x1D519, 0x1D51A, 0x1D51B, 0x1D51C, 0x1D538, 0x1D539,
            0x1D53B, 0x1D53C, 0x1D53D, 0x1D53E, 0x1D540, 0x1D541, 0x1D542,
            0x1D543, 0x1D544, 0x1D546, 0x1D54A, 0x1D54B, 0x1D54C, 0x1D54D,
            0x1D54E, 0x1D54F, 0x1D550, 0x1D56C, 0x1D56D, 0x1D56E, 0x1D56F,
            0x1D570, 0x1D571, 0x1D572, 0x1D573, 0x1D574, 0x1D575, 0x1D576,
            0x1D577, 0x1D578, 0x1D579, 0x1D57A, 0x1D57B, 0x1D57C, 0x1D57D,
            0x1D57E, 0x1D57F, 0x1D580, 0x1D581, 0x1D582, 0x1D583, 0x1D584,
            0x1D585, 0x1D5A0, 0x1D5A1, 0x1D5A2, 0x1D5A3, 0x1D5A4, 0x1D5A5,
            0x1D5A6, 0x1D5A7, 0x1D5A8, 0x1D5A9, 0x1D5AA, 0x1D5AB, 0x1D5AC,
            0x1D5AD, 0x1D5AE, 0x1D5AF, 0x1D5B0, 0x1D5B1, 0x1D5B2, 0x1D5B3,
            0x1D5B4, 0x1D5B5, 0x1D5B6, 0x1D5B7, 0x1D5B8, 0x1D5B9, 0x1D5D4,
            0x1D5D5, 0x1D5D6, 0x1D5D7, 0x1D5D8, 0x1D5D9, 0x1D5DA, 0x1D5DB,
            0x1D5DC, 0x1D5DD, 0x1D5DE, 0x1D5DF, 0x1D5E0, 0x1D5E1, 0x1D5E2,
            0x1D5E3, 0x1D5E4, 0x1D5E5, 0x1D5E6, 0x1D5E7, 0x1D5E8, 0x1D5E9,
            0x1D5EA, 0x1D5EB, 0x1D5EC, 0x1D5ED, 0x1D608, 0x1D609, 0x1D60A,
            0x1D60B, 0x1D60C, 0x1D60D, 0x1D60E, 0x1D60F, 0x1D610, 0x1D611,
            0x1D612, 0x1D613, 0x1D614, 0x1D615, 0x1D616, 0x1D617, 0x1D618,
            0x1D619, 0x1D61A, 0x1D61B, 0x1D61C, 0x1D61D, 0x1D61E, 0x1D61F,
            0x1D620, 0x1D621, 0x1D63C, 0x1D63D, 0x1D63E, 0x1D63F, 0x1D640,
            0x1D641, 0x1D642, 0x1D643, 0x1D644, 0x1D645, 0x1D646, 0x1D647,
            0x1D648, 0x1D649, 0x1D64A, 0x1D64B, 0x1D64C, 0x1D64D, 0x1D64E,
            0x1D64F, 0x1D650, 0x1D651, 0x1D652, 0x1D653, 0x1D654, 0x1D655,
            0x1D670, 0x1D671, 0x1D672, 0x1D673, 0x1D674, 0x1D675, 0x1D676,
            0x1D677, 0x1D678, 0x1D679, 0x1D67A, 0x1D67B, 0x1D67C, 0x1D67D,
            0x1D67E, 0x1D67F, 0x1D680, 0x1D681, 0x1D682, 0x1D683, 0x1D684,
            0x1D685, 0x1D686, 0x1D687, 0x1D688, 0x1D689, 0x1D6A8, 0x1D6A9,
            0x1D6AA, 0x1D6AB, 0x1D6AC, 0x1D6AD, 0x1D6AE, 0x1D6AF, 0x1D6B0,
            0x1D6B1, 0x1D6B2, 0x1D6B3, 0x1D6B4, 0x1D6B5, 0x1D6B6, 0x1D6B7,
            0x1D6B8, 0x1D6B9, 0x1D6BA, 0x1D6BB, 0x1D6BC, 0x1D6BD, 0x1D6BE,
            0x1D6BF, 0x1D6C0, 0x1D6E2, 0x1D6E3, 0x1D6E4, 0x1D6E5, 0x1D6E6,
            0x1D6E7, 0x1D6E8, 0x1D6E9, 0x1D6EA, 0x1D6EB, 0x1D6EC, 0x1D6ED,
            0x1D6EE, 0x1D6EF, 0x1D6F0, 0x1D6F1, 0x1D6F2, 0x1D6F3, 0x1D6F4,
            0x1D6F5, 0x1D6F6, 0x1D6F7, 0x1D6F8, 0x1D6F9, 0x1D6FA, 0x1D71C,
            0x1D71D, 0x1D71E, 0x1D71F, 0x1D720, 0x1D721, 0x1D722, 0x1D723,
            0x1D724, 0x1D725, 0x1D726, 0x1D727, 0x1D728, 0x1D729, 0x1D72A,
            0x1D72B, 0x1D72C, 0x1D72D, 0x1D72E, 0x1D72F, 0x1D730, 0x1D731,
            0x1D732, 0x1D733, 0x1D734, 0x1D756, 0x1D757, 0x1D758, 0x1D759,
            0x1D75A, 0x1D75B, 0x1D75C, 0x1D75D, 0x1D75E, 0x1D75F, 0x1D760,
            0x1D761, 0x1D762, 0x1D763, 0x1D764, 0x1D765, 0x1D766, 0x1D767,
            0x1D768, 0x1D769, 0x1D76A, 0x1D76B, 0x1D76C, 0x1D76D, 0x1D76E,
            0x1D790, 0x1D791, 0x1D792, 0x1D793, 0x1D794, 0x1D795, 0x1D796,
            0x1D797, 0x1D798, 0x1D799, 0x1D79A, 0x1D79B, 0x1D79C, 0x1D79D,
            0x1D79E, 0x1D79F, 0x1D7A0, 0x1D7A1, 0x1D7A2, 0x1D7A3, 0x1D7A4,
            0x1D7A5, 0x1D7A6, 0x1D7A7, 0x1D7A8, 0x1D7CA, 0x1E900, 0x1E901,
            0x1E902, 0x1E903, 0x1E904, 0x1E905, 0x1E906, 0x1E907, 0x1E908,
            0x1E909, 0x1E90A, 0x1E90B, 0x1E90C, 0x1E90D, 0x1E90E, 0x1E90F,
            0x1E910, 0x1E911, 0x1E912, 0x1E913, 0x1E914, 0x1E915, 0x1E916,
            0x1E917, 0x1E918, 0x1E919, 0x1E91A, 0x1E91B, 0x1E91C, 0x1E91D,
            0x1E91E, 0x1E91F, 0x1E920, 0x1E921};
    };
}
#line 2 "boost/parser/detail/pp_for_each.hpp"
#define BOOST_PARSER_DETAIL_PP_FOR_EACH_HPP_INCLUDED
#line 2 "boost/parser/detail/pp_utilities.hpp"
#define BOOST_PARSER_DETAIL_PP_UTILITIES_HPP_INCLUDED
#line 9 "boost/parser/detail/pp_utilities.hpp"
#define BOOST_PARSER_PP_EXPAND(x) x

#define BOOST_PARSER_PP_CAT(x, y) BOOST_PARSER_PP_CAT_I(x, y)
#define BOOST_PARSER_PP_CAT_I(x, ...) x ## __VA_ARGS__
#line 22 "boost/parser/detail/pp_utilities.hpp"
#define BOOST_PARSER_PP_FIRST(x) BOOST_PARSER_PP_FIRST_I(x)
#define BOOST_PARSER_PP_FIRST_I(x, ...) x



#define BOOST_PARSER_PP_IS_PAREN_I(x) BOOST_PARSER_PP_CAT(BOOST_PARSER_PP_IS_PAREN_I_, BOOST_PARSER_PP_IS_PAREN_II x)
#define BOOST_PARSER_PP_IS_PAREN_II(...) 0
#define BOOST_PARSER_PP_IS_PAREN_I_0 1,
#define BOOST_PARSER_PP_IS_PAREN_I_BOOST_PARSER_PP_IS_PAREN_II 0,

#define BOOST_PARSER_PP_IS_PAREN(x) BOOST_PARSER_PP_FIRST(BOOST_PARSER_PP_IS_PAREN_I(x))

#define BOOST_PARSER_PP_EMPTY

#define BOOST_PARSER_PP_IS_EMPTY(x) BOOST_PARSER_PP_IS_EMPTY_I(BOOST_PARSER_PP_IS_PAREN(x), BOOST_PARSER_PP_IS_PAREN(x BOOST_PARSER_PP_EMPTY ()))
#define BOOST_PARSER_PP_IS_EMPTY_I(x, y) BOOST_PARSER_PP_IS_EMPTY_II(x, y)
#define BOOST_PARSER_PP_IS_EMPTY_II(x, y) BOOST_PARSER_PP_IS_EMPTY_III(x, y)
#define BOOST_PARSER_PP_IS_EMPTY_III(x, y) BOOST_PARSER_PP_IS_EMPTY_III_ ## x ## y
#define BOOST_PARSER_PP_IS_EMPTY_III_00 0
#define BOOST_PARSER_PP_IS_EMPTY_III_01 1
#define BOOST_PARSER_PP_IS_EMPTY_III_10 0
#define BOOST_PARSER_PP_IS_EMPTY_III_11 0

#define BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_CAT(BOOST_PARSER_PP_CALL_I_, BOOST_PARSER_PP_IS_EMPTY(x))(F, a, x)
#define BOOST_PARSER_PP_CALL_I_0(F, a, x) F(a, x)
#define BOOST_PARSER_PP_CALL_I_1(F, a, x)

#define BOOST_PARSER_PP_PARSE(x) BOOST_PARSER_PP_CAT(BOOST_PARSER_PP_PARSE_I_, BOOST_PARSER_PP_PARSE_II x)
#define BOOST_PARSER_PP_PARSE_II(...) 0, (__VA_ARGS__),
#define BOOST_PARSER_PP_PARSE_I_BOOST_PARSER_PP_PARSE_II 0, ~,
#define BOOST_PARSER_PP_PARSE_I_0 1
#line 63 "boost/parser/detail/pp_utilities.hpp"
#define BOOST_PARSER_PP_NAME(x) BOOST_PARSER_PP_NAME_I(BOOST_PARSER_PP_PARSE(x))
#define BOOST_PARSER_PP_NAME_I(x) BOOST_PARSER_PP_NAME_II(x)
#define BOOST_PARSER_PP_NAME_II(x, y, z) #z
#line 72 "boost/parser/detail/pp_utilities.hpp"
#define BOOST_PARSER_PP_POINTER(C, x) BOOST_PARSER_PP_POINTER_I(C, BOOST_PARSER_PP_PARSE(x))

#define BOOST_PARSER_PP_EXPAND_V(...) __VA_ARGS__
#line 86 "boost/parser/detail/pp_utilities.hpp"
#define BOOST_PARSER_PP_POINTER_I(C, x) BOOST_PARSER_PP_POINTER_II(C, x)
#define BOOST_PARSER_PP_POINTER_II(C, x, y, z) BOOST_PARSER_PP_POINTER_III_##x(C, y, z)
#define BOOST_PARSER_PP_POINTER_III_0(C, y, z) &C::z
#define BOOST_PARSER_PP_POINTER_III_1(C, y, z) ::boost::describe::detail::mfn<C, BOOST_PARSER_PP_EXPAND_V y>(&C::z)
#line 11 "boost/parser/detail/pp_for_each.hpp"
#define BOOST_PARSER_PP_FOR_EACH_0(F, a)
#define BOOST_PARSER_PP_FOR_EACH_1(F, a, x) BOOST_PARSER_PP_CALL(F, a, x)
#define BOOST_PARSER_PP_FOR_EACH_2(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_1(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_3(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_2(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_4(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_3(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_5(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_4(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_6(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_5(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_7(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_6(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_8(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_7(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_9(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_8(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_10(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_9(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_11(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_10(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_12(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_11(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_13(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_12(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_14(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_13(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_15(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_14(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_16(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_15(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_17(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_16(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_18(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_17(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_19(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_18(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_20(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_19(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_21(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_20(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_22(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_21(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_23(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_22(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_24(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_23(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_25(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_24(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_26(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_25(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_27(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_26(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_28(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_27(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_29(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_28(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_30(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_29(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_31(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_30(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_32(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_31(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_33(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_32(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_34(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_33(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_35(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_34(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_36(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_35(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_37(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_36(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_38(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_37(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_39(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_38(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_40(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_39(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_41(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_40(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_42(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_41(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_43(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_42(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_44(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_43(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_45(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_44(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_46(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_45(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_47(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_46(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_48(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_47(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_49(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_48(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_50(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_49(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_51(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_50(F, a, __VA_ARGS__))
#define BOOST_PARSER_PP_FOR_EACH_52(F, a, x, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_CALL(F, a, x) BOOST_PARSER_PP_FOR_EACH_51(F, a, __VA_ARGS__))

#define BOOST_PARSER_PP_FE_EXTRACT(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, V, ...) V

#define BOOST_PARSER_PP_FOR_EACH(F, ...) BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_EXPAND(BOOST_PARSER_PP_FE_EXTRACT(__VA_ARGS__, BOOST_PARSER_PP_FOR_EACH_52, BOOST_PARSER_PP_FOR_EACH_51, BOOST_PARSER_PP_FOR_EACH_50, BOOST_PARSER_PP_FOR_EACH_49, BOOST_PARSER_PP_FOR_EACH_48, BOOST_PARSER_PP_FOR_EACH_47, BOOST_PARSER_PP_FOR_EACH_46, BOOST_PARSER_PP_FOR_EACH_45, BOOST_PARSER_PP_FOR_EACH_44, BOOST_PARSER_PP_FOR_EACH_43, BOOST_PARSER_PP_FOR_EACH_42, BOOST_PARSER_PP_FOR_EACH_41, BOOST_PARSER_PP_FOR_EACH_40, BOOST_PARSER_PP_FOR_EACH_39, BOOST_PARSER_PP_FOR_EACH_38, BOOST_PARSER_PP_FOR_EACH_37, BOOST_PARSER_PP_FOR_EACH_36, BOOST_PARSER_PP_FOR_EACH_35, BOOST_PARSER_PP_FOR_EACH_34, BOOST_PARSER_PP_FOR_EACH_33, BOOST_PARSER_PP_FOR_EACH_32, BOOST_PARSER_PP_FOR_EACH_31, BOOST_PARSER_PP_FOR_EACH_30, BOOST_PARSER_PP_FOR_EACH_29, BOOST_PARSER_PP_FOR_EACH_28, BOOST_PARSER_PP_FOR_EACH_27, BOOST_PARSER_PP_FOR_EACH_26, BOOST_PARSER_PP_FOR_EACH_25, BOOST_PARSER_PP_FOR_EACH_24, BOOST_PARSER_PP_FOR_EACH_23, BOOST_PARSER_PP_FOR_EACH_22, BOOST_PARSER_PP_FOR_EACH_21, BOOST_PARSER_PP_FOR_EACH_20, BOOST_PARSER_PP_FOR_EACH_19, BOOST_PARSER_PP_FOR_EACH_18, BOOST_PARSER_PP_FOR_EACH_17, BOOST_PARSER_PP_FOR_EACH_16, BOOST_PARSER_PP_FOR_EACH_15, BOOST_PARSER_PP_FOR_EACH_14, BOOST_PARSER_PP_FOR_EACH_13, BOOST_PARSER_PP_FOR_EACH_12, BOOST_PARSER_PP_FOR_EACH_11, BOOST_PARSER_PP_FOR_EACH_10, BOOST_PARSER_PP_FOR_EACH_9, BOOST_PARSER_PP_FOR_EACH_8, BOOST_PARSER_PP_FOR_EACH_7, BOOST_PARSER_PP_FOR_EACH_6, BOOST_PARSER_PP_FOR_EACH_5, BOOST_PARSER_PP_FOR_EACH_4, BOOST_PARSER_PP_FOR_EACH_3, BOOST_PARSER_PP_FOR_EACH_2, BOOST_PARSER_PP_FOR_EACH_1, BOOST_PARSER_PP_FOR_EACH_0))(F, __VA_ARGS__))
#line 7 "boost/parser/detail/text/trie.hpp"
#define BOOST_PARSER_DETAIL_TEXT_TRIE_HPP
#line 7 "boost/parser/detail/text/trie_fwd.hpp"
#define BOOST_PARSER_DETAIL_TEXT_TRIE_FWD_HPP


namespace boost::parser::detail { namespace text {



    struct less
    {
        template<typename T>
        bool operator()(T const & lhs, T const & rhs) const
        {
            return std::less<T>{}(lhs, rhs);
        }
    };

    template<
        typename Key,
        typename Value,
        typename Compare = less,
        std::size_t KeySize = 0>
    struct trie;

    template<typename Key, typename Value, typename Compare = less>
    struct trie_map;

    template<typename Key, typename Compare = less>
    struct trie_set;

}}
#line 12 "boost/parser/detail/text/trie.hpp"
#include <algorithm>
#include <memory>
#include <type_traits>
#include <vector>


namespace boost::parser::detail { namespace text {




    template<typename T, bool Const = std::is_const<T>::value>
    struct optional_ref
    {
    private:
        T * t_;

    public:
        optional_ref() : t_(nullptr) {}
        optional_ref(T & t) : t_(&t) {}

        template<typename U>
        auto operator=(U && u)
            -> decltype(*this->t_ = static_cast<U &&>(u), *this)
        {
            BOOST_ASSERT(t_);
            *t_ = static_cast<U &&>(u);
            return *this;
        }

        explicit operator bool() const & { return t_ != nullptr; }
        explicit operator bool() & { return t_ != nullptr; }
        explicit operator bool() && { return t_ != nullptr; }

        T const & operator*() const
        {
            BOOST_ASSERT(t_);
            return *t_;
        }
        T const * operator->() const
        {
            BOOST_ASSERT(t_);
            return t_;
        }

        operator T const &() const &
        {
            BOOST_ASSERT(t_);
            return *t_;
        }

        operator T const &() const &&
        {
            BOOST_ASSERT(t_);
            return *t_;
        }

        T & operator*()
        {
            BOOST_ASSERT(t_);
            return *t_;
        }

        T * operator->()
        {
            BOOST_ASSERT(t_);
            return t_;
        }

        operator T &() &
        {
            BOOST_ASSERT(t_);
            return *t_;
        }

        operator T &() &&
        {
            BOOST_ASSERT(t_);
            return *t_;
        }
    };

    template<typename T>
    struct optional_ref<T, true>
    {
    private:
        T * t_;

    public:
        optional_ref() : t_(nullptr) {}
        optional_ref(T & t) : t_(&t) {}

        explicit operator bool() const & { return t_ != nullptr; }
        explicit operator bool() && { return t_ != nullptr; }

        T & operator*() const
        {
            BOOST_ASSERT(t_);
            return *t_;
        }
        T * operator->() const
        {
            BOOST_ASSERT(t_);
            return t_;
        }

        operator T &() const &
        {
            BOOST_ASSERT(t_);
            return *t_;
        }

        operator T &() const &&
        {
            BOOST_ASSERT(t_);
            return *t_;
        }
    };

    template<>
    struct optional_ref<bool, false>
    {
    private:
        bool * t_;

    public:
        optional_ref() : t_(nullptr) {}
        optional_ref(bool & t) : t_(&t) {}

        template<typename U>
        auto operator=(U && u)
            -> decltype(*this->t_ = static_cast<U &&>(u), *this)
        {
            BOOST_ASSERT(t_);
            *t_ = static_cast<U &&>(u);
            return *this;
        }

        explicit operator bool() const & { return t_ != nullptr; }
        explicit operator bool() && { return t_ != nullptr; }

        bool const & operator*() const
        {
            BOOST_ASSERT(t_);
            return *t_;
        }
        bool const * operator->() const
        {
            BOOST_ASSERT(t_);
            return t_;
        }

        bool & operator*()
        {
            BOOST_ASSERT(t_);
            return *t_;
        }

        bool * operator->()
        {
            BOOST_ASSERT(t_);
            return t_;
        }
    };

    template<>
    struct optional_ref<bool const, true>
    {
    private:
        bool const * t_;

    public:
        optional_ref() : t_(nullptr) {}
        optional_ref(bool const & t) : t_(&t) {}

        explicit operator bool() const & { return t_ != nullptr; }
        explicit operator bool() && { return t_ != nullptr; }

        bool const & operator*() const
        {
            BOOST_ASSERT(t_);
            return *t_;
        }
        bool const * operator->() const
        {
            BOOST_ASSERT(t_);
            return t_;
        }
    };

    namespace detail {

        template<
            typename ParentIndexing,
            typename Key,
            typename Value,
            std::size_t KeySize = 0>
        struct trie_node_t;

        struct no_index_within_parent_t
        {
            std::size_t value() const
            {
                BOOST_ASSERT(!"This should never be called.");
                return 0;
            }

            template<
                typename Key,
                typename Value,
                typename Iter,
                std::size_t KeySize>
            void insert_at(
                std::unique_ptr<trie_node_t<
                    no_index_within_parent_t,
                    Key,
                    Value,
                    KeySize>> const & child,
                std::ptrdiff_t offset,
                Iter it,
                Iter end)
            {}

            template<typename Key, typename Value, std::size_t KeySize>
            void insert_ptr(std::unique_ptr<trie_node_t<
                                no_index_within_parent_t,
                                Key,
                                Value,
                                KeySize>> const & child)
            {}

            template<typename Iter>
            void erase(Iter it, Iter end)
            {}
        };

        template<typename Char>
        struct char_range
        {
            Char * first_;
            Char * last_;

            Char * begin() const { return first_; }
            Char * end() const { return last_; }
        };

        struct void_type
        {};
    }


    template<typename Key, typename Value>
    struct trie_element
    {
        trie_element() {}
        trie_element(Key k, Value v) : key(k), value(v) {}

        template<typename KeyT, typename ValueT>
        trie_element(trie_element<KeyT, ValueT> const & rhs) :
            key(rhs.key), value(rhs.value)
        {}

        template<typename KeyT, typename ValueT>
        trie_element(trie_element<KeyT, ValueT> && rhs) :
            key(std::move(rhs.key)), value(std::move(rhs.value))
        {}

        template<typename KeyT, typename ValueT>
        trie_element & operator=(trie_element<KeyT, ValueT> const & rhs)
        {
            key = rhs.key;
            value = rhs.value;
            return *this;
        }

        template<typename KeyT, typename ValueT>
        trie_element & operator=(trie_element<KeyT, ValueT> && rhs)
        {
            key = std::move(rhs.key);
            value = std::move(rhs.value);
            return *this;
        }

        Key key;
        Value value;

        friend bool
        operator==(trie_element const & lhs, trie_element const & rhs)
        {
            return lhs.key == rhs.key && lhs.value == rhs.value;
        }
        friend bool
        operator!=(trie_element const & lhs, trie_element const & rhs)
        {
            return !(lhs == rhs);
        }
    };



    struct trie_match_result
    {
        trie_match_result() : node(nullptr), size(0), match(false), leaf(false)
        {}
        trie_match_result(void const * n, std::ptrdiff_t s, bool m, bool l) :
            node(n),
            size(s),
            match(m),
            leaf(l)
        {}


        void const * node;



        std::ptrdiff_t size;




        bool match;



        bool leaf;

        friend bool
        operator==(trie_match_result const & lhs, trie_match_result const & rhs)
        {
            return lhs.node == rhs.node && lhs.size == rhs.size &&
                   lhs.match == rhs.match && lhs.leaf == rhs.leaf;
        }
        friend bool
        operator!=(trie_match_result const & lhs, trie_match_result const & rhs)
        {
            return !(lhs == rhs);
        }
    };
#line 373 "boost/parser/detail/text/trie.hpp"
    template<
        typename Key,
        typename Value,
        typename Compare,
        std::size_t KeySize>
    struct trie
    {
    private:
        using node_t = detail::
            trie_node_t<detail::no_index_within_parent_t, Key, Value, KeySize>;

    public:
        using key_type = Key;
        using value_type = Value;
        using key_compare = Compare;
        using key_element_type = typename Key::value_type;

        using reference = value_type &;
        using const_reference = value_type const &;
        using size_type = std::ptrdiff_t;
        using difference_type = std::ptrdiff_t;

        using match_result = trie_match_result;

        trie() : size_(0) {}

        trie(Compare const & comp) : size_(0), comp_(comp) {}

        template<typename Iter, typename Sentinel>
        trie(Iter first, Sentinel last, Compare const & comp = Compare()) :
            size_(0), comp_(comp)
        {
            insert(first, last);
        }
        template<typename Range>
        explicit trie(Range r, Compare const & comp = Compare()) :
            size_(0), comp_(comp)
        {
            insert(detail::begin(r), detail::end(r));
        }
        trie(std::initializer_list<trie_element<key_type, value_type>> il) :
            size_(0)
        {
            insert(il);
        }

        trie &
        operator=(std::initializer_list<trie_element<key_type, value_type>> il)
        {
            clear();
            for (auto const & x : il) {
                insert(x);
            }
            return *this;
        }

        bool empty() const { return size_ == 0; }
        size_type size() const { return size_; }



#define BOOST_TRIE_C_STR_OVERLOAD(rtype, func) template<typename Char, std::size_t N> rtype func(Char const(&chars)[N]) { static_assert( std::is_same<Char, key_element_type>::value, "Only well-formed when Char is Key::value_type."); return func(detail::char_range<Char const>{chars, chars + N - 1}); }
#line 444 "boost/parser/detail/text/trie.hpp"
#define BOOST_TRIE_C_STR_OVERLOAD_QUALS(rtype, func, quals) template<typename Char, std::size_t N> rtype func(Char const(&chars)[N]) quals { static_assert( std::is_same<Char, key_element_type>::value, "Only well-formed when Char is Key::value_type."); return func(detail::char_range<Char const>{chars, chars + N - 1}); }
#line 457 "boost/parser/detail/text/trie.hpp"
        template<typename KeyRange>
        bool contains(KeyRange const & key) const
        {
            auto first = detail::begin(key);
            auto const last = detail::end(key);
            auto match = longest_match_impl<false>(first, last);
            return first == last && match.match;
        }


        template<typename Char, std::size_t N> bool contains(Char const(&chars)[N]) const { static_assert( std::is_same<Char, key_element_type>::value, "Only well-formed when Char is Key::value_type."); return contains(detail::char_range<Char const>{chars, chars + N - 1}); }




        template<typename KeyIter, typename Sentinel>
        match_result longest_subsequence(KeyIter first, Sentinel last) const
        {
            return longest_match_impl<false>(first, last);
        }



        template<typename KeyRange>
        match_result longest_subsequence(KeyRange const & key) const
        {
            return longest_subsequence(detail::begin(key), detail::end(key));
        }


        template<typename Char, std::size_t N> match_result longest_subsequence(Char const(&chars)[N]) const { static_assert( std::is_same<Char, key_element_type>::value, "Only well-formed when Char is Key::value_type."); return longest_subsequence(detail::char_range<Char const>{chars, chars + N - 1}); }




        template<typename KeyIter, typename Sentinel>
        match_result longest_match(KeyIter first, Sentinel last) const
        {
            return longest_match_impl<true>(first, last);
        }



        template<typename KeyRange>
        match_result longest_match(KeyRange const & key) const
        {
            return longest_match(detail::begin(key), detail::end(key));
        }


        template<typename Char, std::size_t N> match_result longest_match(Char const(&chars)[N]) const { static_assert( std::is_same<Char, key_element_type>::value, "Only well-formed when Char is Key::value_type."); return longest_match(detail::char_range<Char const>{chars, chars + N - 1}); }



        template<typename KeyElementT>
        match_result extend_subsequence(match_result prev, KeyElementT e) const
        {
            auto e_ptr = &e;
            return extend_subsequence_impl<false>(
                match_result{prev}, e_ptr, e_ptr + 1);
        }



        template<typename KeyIter, typename Sentinel>
        match_result extend_subsequence(
            match_result prev, KeyIter first, Sentinel last) const
        {
            return extend_subsequence_impl<false>(
                match_result{prev}, first, last);
        }




        template<typename KeyElementT>
        match_result extend_match(match_result prev, KeyElementT e) const
        {
            BOOST_ASSERT(prev.match);
            auto e_ptr = &e;
            return extend_subsequence_impl<true>(prev, e_ptr, e_ptr + 1);
        }




        template<typename KeyIter, typename Sentinel>
        match_result
        extend_match(match_result prev, KeyIter first, Sentinel last) const
        {
            BOOST_ASSERT(prev.match);
            return extend_subsequence_impl<true>(prev, first, last);
        }




        template<typename OutIter>
        OutIter copy_next_key_elements(match_result prev, OutIter out) const
        {
            auto node = to_node_ptr(prev.node);
            return node->copy_next_key_elements(out);
        }



        template<typename KeyRange>
        optional_ref<value_type const> operator[](KeyRange const & key) const
        {
            auto first = detail::begin(key);
            auto const last = detail::end(key);
            auto match = longest_match_impl<false>(first, last);
            if (first != last || !match.match)
                return {};
            return *to_node_ptr(match.node)->value();
        }


        template<typename Char, std::size_t N> optional_ref<value_type const> operator[](Char const(&chars)[N]) const { static_assert( std::is_same<Char, key_element_type>::value, "Only well-formed when Char is Key::value_type."); return operator[](detail::char_range<Char const>{chars, chars + N - 1}); }





        optional_ref<value_type const> operator[](match_result match) const
        {
            if (!match.match)
                return {};
            return *to_node_ptr(match.node)->value();
        }

        void clear()
        {
            header_ = node_t();
            size_ = 0;
        }



        template<typename KeyRange>
        optional_ref<value_type> operator[](KeyRange const & key)
        {
            auto first = detail::begin(key);
            auto const last = detail::end(key);
            auto match = longest_match_impl<false>(first, last);
            if (first != last || !match.match)
                return {};
            return *const_cast<node_t *>(to_node_ptr(match.node))->value();
        }


        template<typename Char, std::size_t N> optional_ref<value_type> operator[](Char const(&chars)[N]) { static_assert( std::is_same<Char, key_element_type>::value, "Only well-formed when Char is Key::value_type."); return operator[](detail::char_range<Char const>{chars, chars + N - 1}); }




        optional_ref<value_type> operator[](match_result match)
        {
            if (!match.match)
                return {};
            return *const_cast<node_t *>(to_node_ptr(match.node))->value();
        }




        template<typename KeyIter, typename Sentinel>
        bool insert(KeyIter first, Sentinel last, Value value)
        {
            if (empty()) {
                std::unique_ptr<node_t> new_node(new node_t(&header_));
                header_.insert(std::move(new_node));
            }

            auto match = longest_match_impl<false>(first, last);
            if (first == last && match.match)
                return false;

            auto node = create_children(
                const_cast<node_t *>(to_node_ptr(match.node)), first, last);
            node->value() = std::move(value);
            ++size_;

            return true;
        }



        template<typename KeyRange>
        bool insert(KeyRange const & key, Value value)
        {
            return insert(
                detail::begin(key), detail::end(key), std::move(value));
        }

        template<typename Char, std::size_t N>
        bool insert(Char const (&chars)[N], Value value)
        {
            static_assert(
                std::is_same<Char, key_element_type>::value,
                "Only well-formed when Char is Key::value_type.");
            return insert(
                detail::char_range<Char const>{chars, chars + N - 1},
                std::move(value));
        }



        template<typename Iter, typename Sentinel>
        void insert(Iter first, Sentinel last)
        {
            for (; first != last; ++first) {
                insert(first->key, first->value);
            }
        }


        bool insert(trie_element<key_type, value_type> const & e)
        {
            return insert(e.key, e.value);
        }



        template<typename Range>
        bool insert(Range const & r)
        {
            return insert(detail::begin(r), detail::end(r));
        }


        void
        insert(std::initializer_list<trie_element<key_type, value_type>> il)
        {
            for (auto const & x : il) {
                insert(x);
            }
        }






        template<typename KeyIter, typename Sentinel>
        void insert_or_assign(KeyIter first, Sentinel last, Value value)
        {
            auto it = first;
            auto match = longest_match_impl<false>(it, last);
            if (it == last && match.match) {
                const_cast<Value &>(*to_node_ptr(match.node)->value()) =
                    std::move(value);
            }
            insert(first, last, std::move(value));
        }






        template<typename KeyRange>
        void insert_or_assign(KeyRange const & key, Value value)
        {
            insert_or_assign(
                detail::begin(key), detail::end(key), std::move(value));
        }



        template<typename KeyRange>
        bool erase(KeyRange const & key)
        {
            auto first = detail::begin(key);
            auto const last = detail::end(key);
            auto match = longest_match_impl<false>(first, last);
            if (first != last || !match.match)
                return false;
            return erase(match);
        }



        bool erase(match_result match)
        {
            auto node = const_cast<node_t *>(to_node_ptr(match.node));
            if (node == &header_)
                return false;

            --size_;

            if (!node->empty()) {


                node->value() = std::optional<Value>();
                return true;
            }



            auto parent = const_cast<node_t *>(node->parent());
            parent->erase(node);
            while (parent->parent() && parent->empty() && !parent->value()) {
                node = parent;
                parent = const_cast<node_t *>(node->parent());
                parent->erase(node);
            }

            return true;
        }


        template<typename Char, std::size_t N> bool erase(Char const(&chars)[N]) { static_assert( std::is_same<Char, key_element_type>::value, "Only well-formed when Char is Key::value_type."); return erase(detail::char_range<Char const>{chars, chars + N - 1}); }


        void swap(trie & other)
        {
            header_.swap(other.header_);
            std::swap(size_, other.size_);
            std::swap(comp_, other.comp_);
        }

        friend bool operator==(trie const & lhs, trie const & rhs)
        {
            if (lhs.size_ != rhs.size_)
                return false;
            return lhs.header_ == rhs.header_;
        }
        friend bool operator!=(trie const & lhs, trie const & rhs)
        {
            return !(lhs == rhs);
        }



    private:
        trie const * const_this() { return const_cast<trie const *>(this); }
        static node_t const * to_node_ptr(void const * ptr)
        {
            return static_cast<node_t const *>(ptr);
        }

        template<bool StopAtMatch, typename KeyIter, typename Sentinel>
        match_result
        longest_match_impl(KeyIter & first, Sentinel last) const
        {
            return extend_subsequence_impl<StopAtMatch>(
                match_result{&header_, 0, false, true}, first, last);
        }

        template<bool StopAtMatch, typename KeyIter, typename Sentinel>
        match_result extend_subsequence_impl(
            match_result prev, KeyIter & first, Sentinel last) const
        {
            if (to_node_ptr(prev.node) == &header_) {
                if (header_.empty())
                    return prev;
                prev.node = header_.child(0);
            }

            if (first == last) {
                prev.match = !!to_node_ptr(prev.node)->value();
                prev.leaf = to_node_ptr(prev.node)->empty();
                return prev;
            }

            node_t const * node = to_node_ptr(prev.node);
            size_type size = prev.size;
            node_t const * last_match_node = nullptr;
            size_type last_match_size = 0;
            while (first != last) {
                auto const it = node->find(*first, comp_);
                if (it == node->end())
                    break;
                ++first;
                ++size;
                node = it->get();
                if (StopAtMatch && !!node->value()) {
                    last_match_node = node;
                    last_match_size = size;
                }
            }
            if (StopAtMatch && last_match_node) {
                node = last_match_node;
                size = last_match_size;
            }

            return match_result{node, size, !!node->value(), node->empty()};
        }

        template<typename KeyIter, typename Sentinel>
        node_t * create_children(node_t * node, KeyIter first, Sentinel last)
        {
            auto retval = node;
            for (; first != last; ++first) {
                std::unique_ptr<node_t> new_node(new node_t(retval));
                retval =
                    retval->insert(*first, comp_, std::move(new_node))->get();
            }
            return retval;
        }

        node_t header_;
        size_type size_;
        key_compare comp_;

        template<typename Key2, typename Value2, typename Compare2>
        friend struct trie_map;

    };

    namespace detail {

        template<
            typename ParentIndexing,
            typename Key,
            typename Value,
            std::size_t KeySize>
        struct trie_node_t
        {
            using children_t = std::vector<std::unique_ptr<trie_node_t>>;
            using iterator = typename children_t::iterator;
            using const_iterator = typename children_t::const_iterator;
            using key_element = typename Key::value_type;

            static_assert(std::is_unsigned<key_element>::value, "");

            trie_node_t() : parent_(nullptr) {}
            trie_node_t(trie_node_t * parent) : parent_(parent) {}
            trie_node_t(trie_node_t const & other) :
                children_(other.children_.size()),
                value_(other.value_),
                parent_(other.parent_),
                index_within_parent_(other.index_within_parent_)
            {
                for (int i = 0, end = (int)children_.size(); i < end; ++i) {
                    if (other.children_[i]) {
                        children_[i].reset(
                            new trie_node_t(*other.children_[i]));
                    }
                }
            }
            trie_node_t(trie_node_t && other) : parent_(nullptr)
            {
                swap(other);
            }
            trie_node_t & operator=(trie_node_t const & rhs)
            {
                BOOST_ASSERT(parent_ == nullptr &&
                    "Assignment of trie_node_ts are defined only for the "
                    "header node.");



                trie_node_t temp(rhs);
                temp.swap(*this);
                return *this;
            }
            trie_node_t & operator=(trie_node_t && rhs)
            {
                BOOST_ASSERT(parent_ == nullptr &&
                    "Move assignments of trie_node_ts are defined only for the "
                    "header node.");



                trie_node_t temp(std::move(rhs));
                temp.swap(*this);
                return *this;
            }

            auto max_size() const { return KeySize; }

            std::optional<Value> const & value() const { return value_; }

            Value & child_value(std::size_t i) const
            {
                return *children_[i]->value_;
            }

            trie_node_t * parent() const { return parent_; }

            bool empty() const { return children_.size() == 0; }

            const_iterator end() const { return children_.end(); }

            std::size_t index_within_parent() const
            {
                return index_within_parent_.value();
            }

            bool before_child_subtree(key_element const & e) const
            {
                return e < key_element(0);
            }

            template<typename Compare>
            const_iterator
            lower_bound(key_element const & e, Compare const &) const
            {
                return children_.empty() ? children_.end() : children_.begin() + e;
            }
            template<typename Compare>
            const_iterator
            find(key_element const & e, Compare const & comp) const
            {
                auto const it = lower_bound(e, comp);
                if (children_.empty() || !*it)
                    return children_.end();
                return it;
            }

            template<typename Compare>
            trie_node_t const *
            child(key_element const & e, Compare const &) const
            {
                return children_[e].get();
            }
            trie_node_t const * child(std::size_t i) const
            {
                return children_[i].get();
            }

            key_element const & key(std::size_t i) const
            {
                BOOST_ASSERT(key_element(i) == i);
                return key_element(i);
            }

            template<typename OutIter>
            OutIter copy_next_key_elements(OutIter out) const
            {
                for (key_element i = 0, end = (key_element)children_.size();
                     i < end;
                     ++i) {
                    if (children_[i]) {
                        *out = i;
                        ++out;
                    }
                }
                return out;
            }

            void swap(trie_node_t & other)
            {
                BOOST_ASSERT(parent_ == nullptr &&
                    "Swaps of trie_node_ts are defined only for the header "
                    "node.");



                children_.swap(other.children_);
                value_.swap(other.value_);
                for (auto const & node : children_) {
                    if (node)
                        node->parent_ = this;
                }
                for (auto const & node : other.children_) {
                    if (node)
                        node->parent_ = &other;
                }
                std::swap(index_within_parent_, other.index_within_parent_);
            }

            std::optional<Value> & value() { return value_; }

            template<typename Compare>
            iterator insert(
                key_element const & e,
                Compare const & comp,
                std::unique_ptr<trie_node_t> && child)
            {
                if (children_.empty())
                    children_.resize(max_size());
                auto child_it = children_.begin() + e;
                index_within_parent_.insert_at(
                    child, e, child_it, children_.end());
                children_[e] = std::move(child);
                return child_it;
            }
            iterator insert(std::unique_ptr<trie_node_t> && child)
            {
                if (children_.empty())
                    children_.resize(max_size());
                index_within_parent_.insert_ptr(child);
                return children_.insert(children_.begin(), std::move(child));
            }
            void erase(std::size_t i)
            {
                auto it = children_.begin() + i;
                it->reset(nullptr);
                index_within_parent_.erase(it, children_.end());
            }
            void erase(trie_node_t const * child)
            {
                auto const it = std::find_if(
                    children_.begin(),
                    children_.end(),
                    [child](std::unique_ptr<trie_node_t> const & ptr) {
                        return child == ptr.get();
                    });
                BOOST_ASSERT(it != children_.end());
                erase(it - children_.begin());
            }

            template<typename Compare>
            iterator
            lower_bound(key_element const & e, Compare const &)
            {
                return children_.begin() + e;
            }
            template<typename Compare>
            iterator find(key_element const & e, Compare const & comp)
            {
                if (children_.empty())
                    return children_.end();
                auto const it = lower_bound(e, comp);
                if (!*it)
                    return children_.end();
                return it;
            }

            template<typename Compare>
            trie_node_t * child(key_element const & e, Compare const &)
            {
                return children_[e].get();
            }
            trie_node_t * child(std::size_t i)
            {
                return children_[i].get();
            }

            friend bool
            operator==(trie_node_t const & lhs, trie_node_t const & rhs)
            {
                if (lhs.value_ != rhs.value_)
                    return false;
                return std::equal(
                    lhs.children_.begin(),
                    lhs.children_.end(),
                    rhs.children_.begin(),
                    rhs.children_.end(),
                    [](auto const & l_ptr, auto const & r_ptr) {
                        if (!l_ptr && !r_ptr)
                            return true;
                        if (!l_ptr || !r_ptr)
                            return false;
                        return *l_ptr == *r_ptr;
                    });
            }
            friend bool
            operator!=(trie_node_t const & lhs, trie_node_t const & rhs)
            {
                return !(lhs == rhs);
            }

        private:
            trie_node_t const * const_this()
            {
                return const_cast<trie_node_t const *>(this);
            }
            key_element const & key(const_iterator it) const
            {
                return key_element(it - children_.begin());
            }

            children_t children_;
            std::optional<Value> value_;
            trie_node_t * parent_;
            ParentIndexing index_within_parent_;

            friend struct index_within_parent_t;
        };

        template<typename ParentIndexing, typename Key, typename Value>
        struct trie_node_t<ParentIndexing, Key, Value, 0>
        {
            using children_t = std::vector<std::unique_ptr<trie_node_t>>;
            using iterator = typename children_t::iterator;
            using const_iterator = typename children_t::const_iterator;
            using key_element = typename Key::value_type;
            using keys_t = std::vector<key_element>;
            using key_iterator = typename keys_t::const_iterator;

            trie_node_t() : parent_(nullptr) {}
            trie_node_t(trie_node_t * parent) : parent_(parent) {}
            trie_node_t(trie_node_t const & other) :
                keys_(other.keys_),
                value_(other.value_),
                parent_(other.parent_),
                index_within_parent_(other.index_within_parent_)
            {
                children_.reserve(other.children_.size());
                for (auto const & node : other.children_) {
                    std::unique_ptr<trie_node_t> new_node(
                        new trie_node_t(*node));
                    children_.push_back(std::move(new_node));
                }
            }
            trie_node_t(trie_node_t && other) : parent_(nullptr)
            {
                swap(other);
            }
            trie_node_t & operator=(trie_node_t const & rhs)
            {
                BOOST_ASSERT(parent_ == nullptr &&
                    "Assignment of trie_node_ts are defined only for the "
                    "header node.");



                trie_node_t temp(rhs);
                temp.swap(*this);
                return *this;
            }
            trie_node_t & operator=(trie_node_t && rhs)
            {
                BOOST_ASSERT(parent_ == nullptr &&
                    "Move assignments of trie_node_ts are defined only for the "
                    "header node.");



                trie_node_t temp(std::move(rhs));
                temp.swap(*this);
                return *this;
            }

            std::optional<Value> const & value() const { return value_; }

            Value & child_value(std::size_t i) const
            {
                return *children_[i]->value_;
            }

            trie_node_t * parent() const { return parent_; }
            trie_node_t * min_child() const
            {
                return children_.front().get();
            }
            trie_node_t * max_child() const
            {
                return children_.back().get();
            }

            bool empty() const { return children_.size() == 0; }
            std::size_t size() const { return children_.size(); }

            bool min_value() const
            {
                return !!children_.front()->value_;
            }
            bool max_value() const
            {
                return !!children_.back()->value_;
            }

            const_iterator begin() const { return children_.begin(); }
            const_iterator end() const { return children_.end(); }

            key_iterator key_begin() const { return keys_.begin(); }
            key_iterator key_end() const { return keys_.end(); }

            std::size_t index_within_parent() const
            {
                return index_within_parent_.value();
            }

            bool before_child_subtree(key_element const & e) const
            {
                return keys_.empty() || e < keys_.front();
            }

            template<typename Compare>
            const_iterator
            lower_bound(key_element const & e, Compare const & comp) const
            {
                auto const it =
                    std::lower_bound(keys_.begin(), keys_.end(), e, comp);
                return children_.begin() + (it - keys_.begin());
            }
            template<typename Compare>
            const_iterator
            find(key_element const & e, Compare const & comp) const
            {
                auto const it = lower_bound(e, comp);
                auto const end_ = end();
                if (it != end_ && comp(e, key(it)))
                    return end_;
                return it;
            }

            template<typename Compare>
            trie_node_t const *
            child(key_element const & e, Compare const & comp) const
            {
                auto const it = find(e, comp);
                if (it == children_.end())
                    return nullptr;
                return it->get();
            }
            trie_node_t const * child(std::size_t i) const
            {
                return children_[i].get();
            }

            key_element const & key(std::size_t i) const
            {
                return keys_[i];
            }

            template<typename OutIter>
            OutIter copy_next_key_elements(OutIter out) const
            {
                return std::copy(key_begin(), key_end(), out);
            }

            void swap(trie_node_t & other)
            {
                BOOST_ASSERT(parent_ == nullptr &&
                    "Swaps of trie_node_ts are defined only for the header "
                    "node.");



                keys_.swap(other.keys_);
                children_.swap(other.children_);
                value_.swap(other.value_);
                for (auto const & node : children_) {
                    node->parent_ = this;
                }
                for (auto const & node : other.children_) {
                    node->parent_ = &other;
                }
                std::swap(index_within_parent_, other.index_within_parent_);
            }

            std::optional<Value> & value() { return value_; }

            iterator begin() { return children_.begin(); }
            iterator end() { return children_.end(); }

            template<typename Compare>
            iterator insert(
                key_element const & e,
                Compare const & comp,
                std::unique_ptr<trie_node_t> && child)
            {
                BOOST_ASSERT(child->empty());
                auto it = std::lower_bound(keys_.begin(), keys_.end(), e, comp);
                it = keys_.insert(it, e);
                auto const offset = it - keys_.begin();
                auto child_it = children_.begin() + offset;
                index_within_parent_.insert_at(
                    child, offset, child_it, children_.end());
                return children_.insert(child_it, std::move(child));
            }
            iterator insert(std::unique_ptr<trie_node_t> && child)
            {
                BOOST_ASSERT(empty());
                index_within_parent_.insert_ptr(child);
                return children_.insert(children_.begin(), std::move(child));
            }
            void erase(std::size_t i)
            {

                if (!keys_.empty())
                    keys_.erase(keys_.begin() + i);
                auto it = children_.erase(children_.begin() + i);
                index_within_parent_.erase(it, children_.end());
            }
            void erase(trie_node_t const * child)
            {
                auto const it = std::find_if(
                    children_.begin(),
                    children_.end(),
                    [child](std::unique_ptr<trie_node_t> const & ptr) {
                        return child == ptr.get();
                    });
                BOOST_ASSERT(it != children_.end());
                erase(it - children_.begin());
            }

            template<typename Compare>
            iterator
            lower_bound(key_element const & e, Compare const & comp)
            {
                auto const it = const_this()->lower_bound(e, comp);
                return children_.begin() +
                       (it - const_iterator(children_.begin()));
            }
            template<typename Compare>
            iterator find(key_element const & e, Compare const & comp)
            {
                auto const it = const_this()->find(e, comp);
                return children_.begin() +
                       (it - const_iterator(children_.begin()));
            }

            template<typename Compare>
            trie_node_t *
            child(key_element const & e, Compare const & comp)
            {
                return const_cast<trie_node_t *>(const_this()->child(e, comp));
            }
            trie_node_t * child(std::size_t i)
            {
                return const_cast<trie_node_t *>(const_this()->child(i));
            }

            friend bool
            operator==(trie_node_t const & lhs, trie_node_t const & rhs)
            {
                if (lhs.keys_ != rhs.keys_ || lhs.value_ != rhs.value_)
                    return false;
                return std::equal(
                    lhs.children_.begin(),
                    lhs.children_.end(),
                    rhs.children_.begin(),
                    rhs.children_.end(),
                    [](auto const & l_ptr, auto const & r_ptr) {
                        if (!l_ptr && !r_ptr)
                            return true;
                        if (!l_ptr || !r_ptr)
                            return false;
                        return *l_ptr == *r_ptr;
                    });
            }
            friend bool
            operator!=(trie_node_t const & lhs, trie_node_t const & rhs)
            {
                return !(lhs == rhs);
            }

        private:
            trie_node_t const * const_this()
            {
                return const_cast<trie_node_t const *>(this);
            }
            key_element const & key(const_iterator it) const
            {
                return keys_[it - children_.begin()];
            }

            keys_t keys_;
            children_t children_;
            std::optional<Value> value_;
            trie_node_t * parent_;
            ParentIndexing index_within_parent_;

            friend struct index_within_parent_t;
        };
    }

}}
#line 24 "boost/parser/parser.hpp"
#include <type_traits>
#include <variant>
#include <vector>


namespace boost { namespace parser {





    struct none;





    struct none
    {
        none() = default;



        template<typename T>
        none(T const &)
        {
            fail();
        }
        template<typename T>
        none & operator=(T const &)
        {
            fail();
            return *this;
        }
        template<typename T>
        operator T() const
        {
            fail();
            return T{};
        }


        none operator+() const
        {
            fail();
            return none{};
        }
        none operator-() const
        {
            fail();
            return none{};
        }
        none operator*() const
        {
            fail();
            return none{};
        }
        none operator~() const
        {
            fail();
            return none{};
        }
        none operator&() const
        {
            fail();
            return none{};
        }
        none operator!() const
        {
            fail();
            return none{};
        }
        none operator++()
        {
            fail();
            return none{};
        }
        none & operator++(int)
        {
            fail();
            return *this;
        }
        none operator--()
        {
            fail();
            return none{};
        }
        none operator--(int)
        {
            fail();
            return *this;
        }


        template<typename T>
        none operator<<(T const &) const
        {
            fail();
            return none{};
        }
        template<typename T>
        none operator>>(T const &) const
        {
            fail();
            return none{};
        }
        template<typename T>
        none operator*(T const &) const
        {
            fail();
            return none{};
        }
        template<typename T>
        none operator/(T const &) const
        {
            fail();
            return none{};
        }
        template<typename T>
        none operator%(T const &) const
        {
            fail();
            return none{};
        }
        template<typename T>
        none operator+(T const &) const
        {
            fail();
            return none{};
        }
        template<typename T>
        none operator-(T const &) const
        {
            fail();
            return none{};
        }
        template<typename T>
        none operator<(T const &) const
        {
            fail();
            return none{};
        }
        template<typename T>
        none operator>(T const &) const
        {
            fail();
            return none{};
        }
        template<typename T>
        none operator<=(T const &) const
        {
            fail();
            return none{};
        }
        template<typename T>
        none operator>=(T const &) const
        {
            fail();
            return none{};
        }
        template<typename T>
        none operator==(T const &) const
        {
            fail();
            return none{};
        }
        template<typename T>
        none operator!=(T const &) const
        {
            fail();
            return none{};
        }
        template<typename T>
        none operator||(T const &) const
        {
            fail();
            return none{};
        }
        template<typename T>
        none operator&&(T const &) const
        {
            fail();
            return none{};
        }
        template<typename T>
        none operator&(T const &) const
        {
            fail();
            return none{};
        }
        template<typename T>
        none operator|(T const &) const
        {
            fail();
            return none{};
        }
        template<typename T>
        none operator^(T const &) const
        {
            fail();
            return none{};
        }
        template<typename T>
        none operator,(T const &) const
        {
            fail();
            return none{};
        }
        template<typename T>
        none operator->*(T const &) const
        {
            fail();
            return none{};
        }
        template<typename T>
        none operator<<=(T const &)
        {
            fail();
            return none{};
        }
        template<typename T>
        none operator>>=(T const &)
        {
            fail();
            return none{};
        }
        template<typename T>
        none operator*=(T const &)
        {
            fail();
            return none{};
        }
        template<typename T>
        none operator/=(T const &)
        {
            fail();
            return none{};
        }
        template<typename T>
        none operator%=(T const &)
        {
            fail();
            return none{};
        }
        template<typename T>
        none operator+=(T const &)
        {
            fail();
            return none{};
        }
        template<typename T>
        none operator-=(T const &)
        {
            fail();
            return none{};
        }
        template<typename T>
        none operator&=(T const &)
        {
            fail();
            return none{};
        }
        template<typename T>
        none operator|=(T const &)
        {
            fail();
            return none{};
        }
        template<typename T>
        none operator^=(T const &)
        {
            fail();
            return none{};
        }
        template<typename T>
        none operator[](T const &) const
        {
            fail();
            return none{};
        }


        template<typename... Args>
        none operator()(Args const &...) const
        {
            fail();
            return none{};
        }

        void fail() const
        {
#line 322 "boost/parser/parser.hpp"
            BOOST_ASSERT(false);
        }
    };


    namespace detail {
#line 342 "boost/parser/parser.hpp"
        template<typename T>
        struct identity_t
        {
            using type = T;
        };
        template<typename T>
        struct print_t : identity_t<T>
        {
#line 360 "boost/parser/parser.hpp"
            enum {
                n =



                    sizeof(T) > -1

            };

        };



        template<typename T>
        using print_type = typename print_t<T>::type;




        struct nope
        {
            template<typename T>
            constexpr nope & operator=(T const &)
            {
                return *this;
            }

            operator std::nullopt_t() const noexcept { return std::nullopt; }

            template<typename Context>
            constexpr bool operator()(Context const &) const noexcept
            {
                return true;
            }

            constexpr nope operator*() const noexcept { return nope{}; }

            friend constexpr bool operator==(nope, nope) { return true; }
            friend constexpr bool operator!=(nope, nope) { return false; }

            template<typename T>
            friend constexpr bool operator==(T, nope)
            {
                return false;
            }
            template<typename T>
            friend constexpr bool operator!=(T, nope)
            {
                return false;
            }
        };

        inline nope global_nope;

        template<typename T, bool AlwaysConst = false>
        using nope_or_pointer_t = std::conditional_t<
            std::is_same_v<std::remove_const_t<T>, nope>,
            nope,
            std::conditional_t<AlwaysConst, T const *, T *>>;

        template<
            typename I,
            typename S,
            typename ErrorHandler,
            typename GlobalState = nope,
            typename Callbacks = nope,
            typename Attr = nope,
            typename Val = nope,
            typename RuleTag = void,
            typename RuleLocals = nope,
            typename RuleParams = nope,
            typename Where = nope>
        struct parse_context
        {
            parse_context() = default;
            parse_context(parse_context const &) = default;
            parse_context & operator=(parse_context const &) = default;

            using rule_tag = RuleTag;

            I first_;
            S last_;
            bool * pass_ = nullptr;
            int * trace_indent_ = nullptr;
            symbol_table_tries_t * symbol_table_tries_ = nullptr;
            ErrorHandler const * error_handler_ = nullptr;
            nope_or_pointer_t<GlobalState> globals_{};
            nope_or_pointer_t<Callbacks, true> callbacks_{};
            nope_or_pointer_t<Attr> attr_{};
            nope_or_pointer_t<Val> val_{};
            nope_or_pointer_t<RuleLocals> locals_{};
            nope_or_pointer_t<RuleParams, true> params_{};
            nope_or_pointer_t<Where, true> where_{};
            int no_case_depth_ = 0;

            template<typename T>
            static auto nope_or_address(T & x)
            {
                if constexpr (std::is_same_v<std::remove_const_t<T>, nope>)
                    return nope{};
                else
                    return std::addressof(x);
            }

            template<typename T, typename U>
            static auto other_or_address(T other, U & x)
            {
                if constexpr (std::is_same_v<std::remove_const_t<U>, nope>)
                    return other;
                else
                    return std::addressof(x);
            }

            parse_context(
                I & first,
                S last,
                bool & success,
                int & indent,
                ErrorHandler const & error_handler,
                GlobalState & globals,
                symbol_table_tries_t & symbol_table_tries) :
                first_(first),
                last_(last),
                pass_(std::addressof(success)),
                trace_indent_(std::addressof(indent)),
                symbol_table_tries_(std::addressof(symbol_table_tries)),
                error_handler_(std::addressof(error_handler)),
                globals_(nope_or_address(globals))
            {}


            parse_context(
                I & first,
                S last,
                bool & success,
                int & indent,
                ErrorHandler const & error_handler,
                Callbacks const & callbacks,
                GlobalState & globals,
                symbol_table_tries_t & symbol_table_tries) :
                first_(first),
                last_(last),
                pass_(std::addressof(success)),
                trace_indent_(std::addressof(indent)),
                symbol_table_tries_(std::addressof(symbol_table_tries)),
                error_handler_(std::addressof(error_handler)),
                globals_(nope_or_address(globals)),
                callbacks_(std::addressof(callbacks))
            {}


            template<
                typename OldVal,
                typename OldRuleTag,
                typename OldRuleLocals,
                typename OldRuleParams,
                typename NewVal,
                typename NewRuleTag,
                typename NewRuleLocals,
                typename NewRuleParams>
            parse_context(
                parse_context<
                    I,
                    S,
                    ErrorHandler,
                    GlobalState,
                    Callbacks,
                    Attr,
                    OldVal,
                    OldRuleTag,
                    OldRuleLocals,
                    OldRuleParams> const & other,
                NewRuleTag * tag_ptr,
                NewVal & value,
                NewRuleLocals & locals,
                NewRuleParams const & params) :
                first_(other.first_),
                last_(other.last_),
                pass_(other.pass_),
                trace_indent_(other.trace_indent_),
                symbol_table_tries_(other.symbol_table_tries_),
                error_handler_(other.error_handler_),
                globals_(other.globals_),
                callbacks_(other.callbacks_),
                attr_(other.attr_),
                no_case_depth_(other.no_case_depth_)
            {
                if constexpr (
                    std::is_same_v<OldRuleTag, NewRuleTag> &&
                    !std::is_same_v<OldRuleTag, void>) {
                    val_ = other.val_;
                    locals_ = other.locals_;
                    params_ = other.params_;
                } else {
                    val_ = other_or_address(other.val_, value);
                    locals_ = other_or_address(other.locals_, locals);
                    params_ = other_or_address(other.params_, params);
                }
            }


            template<typename OldAttr, typename OldWhere>
            parse_context(
                parse_context<
                    I,
                    S,
                    ErrorHandler,
                    GlobalState,
                    Callbacks,
                    OldAttr,
                    Val,
                    RuleTag,
                    RuleLocals,
                    RuleParams,
                    OldWhere> const & other,
                Attr & attr,
                Where const & where) :
                first_(other.first_),
                last_(other.last_),
                pass_(other.pass_),
                trace_indent_(other.trace_indent_),
                symbol_table_tries_(other.symbol_table_tries_),
                error_handler_(other.error_handler_),
                globals_(other.globals_),
                callbacks_(other.callbacks_),
                attr_(nope_or_address(attr)),
                val_(other.val_),
                locals_(other.locals_),
                params_(other.params_),
                where_(nope_or_address(where)),
                no_case_depth_(other.no_case_depth_)
            {}
        };

        template<
            typename I,
            typename S,
            typename ErrorHandler,
            typename GlobalState,
            typename Callbacks,
            typename Val,
            typename RuleTag,
            typename RuleLocals,
            typename RuleParams,
            typename Attr,
            typename Where,
            typename OldAttr>
        auto make_action_context(
            parse_context<
                I,
                S,
                ErrorHandler,
                GlobalState,
                Callbacks,
                OldAttr,
                Val,
                RuleTag,
                RuleLocals,
                RuleParams> const & context,
            Attr & attr,
            Where const & where)
        {
            using result_type = parse_context<
                I,
                S,
                ErrorHandler,
                GlobalState,
                Callbacks,
                Attr,
                Val,
                RuleTag,
                RuleLocals,
                RuleParams,
                Where>;
            return result_type(context, attr, where);
        }

        template<
            typename I,
            typename S,
            typename ErrorHandler,
            typename GlobalState,
            typename Callbacks,
            typename Attr,
            typename Val,
            typename RuleTag,
            typename RuleLocals,
            typename RuleParams,
            typename NewVal,
            typename NewRuleTag,
            typename NewRuleLocals,
            typename NewRuleParams>
        auto make_rule_context(
            parse_context<
                I,
                S,
                ErrorHandler,
                GlobalState,
                Callbacks,
                Attr,
                Val,
                RuleTag,
                RuleLocals,
                RuleParams> const & context,
            NewRuleTag * tag_ptr,
            NewVal & value,
            NewRuleLocals & locals,
            NewRuleParams const & params)
        {
            using result_type = parse_context<
                I,
                S,
                ErrorHandler,
                GlobalState,
                Callbacks,
                Attr,
                std::conditional_t<std::is_same_v<NewVal, nope>, Val, NewVal>,
                NewRuleTag,
                std::conditional_t<
                    std::is_same_v<NewRuleLocals, nope>,
                    RuleLocals,
                    NewRuleLocals>,
                std::conditional_t<
                    std::is_same_v<NewRuleParams, nope>,
                    RuleParams,
                    NewRuleParams>>;
            return result_type(context, tag_ptr, value, locals, params);
        }

        template<typename Iter, typename Sentinel, typename ErrorHandler>
        auto make_context(
            Iter first,
            Sentinel last,
            bool & success,
            int & indent,
            ErrorHandler const & error_handler,
            nope & n,
            symbol_table_tries_t & symbol_table_tries) noexcept
        {
            return parse_context(
                first,
                last,
                success,
                indent,
                error_handler,
                n,
                symbol_table_tries);
        }

        template<
            typename Iter,
            typename Sentinel,
            typename ErrorHandler,
            typename GlobalState>
        auto make_context(
            Iter first,
            Sentinel last,
            bool & success,
            int & indent,
            ErrorHandler const & error_handler,
            GlobalState & globals,
            symbol_table_tries_t & symbol_table_tries) noexcept
        {
            return parse_context(
                first,
                last,
                success,
                indent,
                error_handler,
                globals,
                symbol_table_tries);
        }

        template<
            typename Iter,
            typename Sentinel,
            typename ErrorHandler,
            typename Callbacks>
        auto make_context(
            Iter first,
            Sentinel last,
            bool & success,
            int & indent,
            ErrorHandler const & error_handler,
            Callbacks const & callbacks,
            nope & n,
            symbol_table_tries_t & symbol_table_tries) noexcept
        {
            return parse_context(
                first,
                last,
                success,
                indent,
                error_handler,
                callbacks,
                n,
                symbol_table_tries);
        }

        template<
            typename Iter,
            typename Sentinel,
            typename ErrorHandler,
            typename Callbacks,
            typename GlobalState>
        auto make_context(
            Iter first,
            Sentinel last,
            bool & success,
            int & indent,
            ErrorHandler const & error_handler,
            Callbacks const & callbacks,
            GlobalState & globals,
            symbol_table_tries_t & symbol_table_tries) noexcept
        {
            return parse_context(
                first,
                last,
                success,
                indent,
                error_handler,
                callbacks,
                globals,
                symbol_table_tries);
        }


        template<unsigned int I>
        struct param_t
        {
            template<typename Context>
            decltype(auto) operator()(Context const & context) const
            {
                return parser::_params(context)[llong<I>{}];
            }
        };


        template<typename T, typename... Args>
        using callable = decltype(std::declval<T>()(std::declval<Args>()...));

        template<
            typename Context,
            typename T,
            bool Callable = is_detected_v<callable, T const &, Context const &>>
        struct resolve_impl
        {
            static auto call(Context const &, T const & x) { return x; }
        };

        template<typename Context, typename T>
        struct resolve_impl<Context, T, true>
        {
            static auto call(Context const & context, T const & x)
            {
                return x(context);
            }
        };

        template<typename Context, typename T>
        auto resolve(Context const & context, T const & x)
        {
            return resolve_impl<Context, T>::call(context, x);
        }

        template<typename Context>
        auto resolve(Context const &, nope n)
        {
            return n;
        }


        template<typename Context, typename ParamsTuple>
        auto
        resolve_rule_params(Context const & context, ParamsTuple const & params)
        {
            return detail::hl::transform(params, [&](auto const & x) {
                return detail::resolve(context, x);
            });
        }

        template<typename Context>
        nope resolve_rule_params(Context const & context, nope)
        {
            return {};
        }

        template<typename LocalsType, typename Context>
        LocalsType make_locals_impl(Context const & context, std::true_type)
        {
            return LocalsType(context);
        }

        template<typename LocalsType, typename Context>
        LocalsType make_locals_impl(Context const & context, std::false_type)
        {
            return LocalsType();
        }

        template<typename LocalsType, typename Context>
        LocalsType make_locals(Context const & context)
        {
            return detail::make_locals_impl<LocalsType>(
                context,
                typename std::is_convertible<Context const &, LocalsType>::
                    type{});
        }


        template<typename Context>
        decltype(auto) _indent(Context const & context)
        {
            return *context.trace_indent_;
        }

        template<typename Context>
        decltype(auto) _callbacks(Context const & context)
        {
            return *context.callbacks_;
        }

        template<typename Context, typename T>
        decltype(auto) get_trie(
            Context const & context, symbol_parser<T> const & symbol_parser)
        {
            using trie_t = text::trie<std::vector<char32_t>, T>;
            symbol_table_tries_t & symbol_table_tries =
                *context.symbol_table_tries_;
            any_copyable & a = symbol_table_tries[(void *)&symbol_parser];
            if (a.empty()) {
                a = trie_t{};
                trie_t & trie = a.cast<trie_t>();
                for (auto const & e : symbol_parser.initial_elements()) {
                    trie.insert(e.first | text::as_utf32, e.second);
                }
                return trie;
            } else {
                return a.cast<trie_t>();
            }
        }




        template<typename T>
        using remove_cv_ref_t = typename std::remove_cv<
            typename std::remove_reference<T>::type>::type;

        template<typename T, typename U>
        using comparison = decltype(std::declval<T>() == std::declval<U>());

        template<typename T, typename U>
        constexpr bool is_equality_comparable_with_v =
            is_detected_v<comparison, T, U>;

        template<typename T>
        struct is_nope : std::false_type
        {};
        template<>
        struct is_nope<nope> : std::true_type
        {};
        template<typename T>
        constexpr bool is_nope_v = is_nope<remove_cv_ref_t<T>>::value;

        template<typename T>
        struct is_unconditional_eps : std::false_type
        {};
        template<>
        struct is_unconditional_eps<eps_parser<nope>> : std::true_type
        {};
        template<typename T>
        constexpr bool is_unconditional_eps_v =
            is_unconditional_eps<remove_cv_ref_t<T>>::value;

        template<typename T>
        struct is_zero_plus_p : std::false_type
        {};
        template<typename T>
        struct is_zero_plus_p<zero_plus_parser<T>> : std::true_type
        {};

        template<typename T>
        struct is_or_p : std::false_type
        {};
        template<typename T>
        struct is_or_p<or_parser<T>> : std::true_type
        {};

        template<typename T>
        struct is_seq_p : std::false_type
        {};
        template<typename T, typename U, typename V>
        struct is_seq_p<seq_parser<T, U, V>> : std::true_type
        {};

        template<typename T>
        struct is_one_plus_p : std::false_type
        {};
        template<typename T>
        struct is_one_plus_p<one_plus_parser<T>> : std::true_type
        {};

        template<typename T>
        struct is_utf8_view : std::false_type
        {};
        template<typename V>
        struct is_utf8_view<text::utf8_view<V>> : std::true_type
        {};

        template<typename T>
        using optional_type = remove_cv_ref_t<decltype(*std::declval<T &>())>;

        template<typename F, typename... Args>
        constexpr bool is_invocable_v = is_detected_v<callable, F, Args...>;

        template<typename T>
        using has_begin =
            decltype(*detail::text::detail::begin(std::declval<T &>()));
        template<typename T>
        using has_end =
            decltype(detail::text::detail::end(std::declval<T &>()));

        template<typename T>
        constexpr bool is_range =
            is_detected_v<has_begin, T> && is_detected_v<has_end, T>;

        template<typename T>
        using has_push_back =
            decltype(std::declval<T &>().push_back(*std::declval<T>().begin()));
#line 1013 "boost/parser/parser.hpp"
        template<typename T>
        using iterator_t =
            decltype(detail::text::detail::begin(std::declval<T &>()));
        template<typename Range>
        using sentinel_t =
            decltype(detail::text::detail::end(std::declval<Range &>()));
        template<typename T>
        using iter_value_t = typename std::iterator_traits<T>::value_type;
        template<typename T>
        using iter_reference_t = decltype(*std::declval<T &>());
        template<typename T>
        using iter_rvalue_reference_t =
            decltype(std::move(*std::declval<T &>()));
        template<typename T>
        using range_value_t = iter_value_t<iterator_t<T>>;
        template<typename T>
        using range_reference_t = iter_reference_t<iterator_t<T>>;
        template<typename T>
        using range_rvalue_reference_t = iter_rvalue_reference_t<iterator_t<T>>;

        template<typename T>
        using has_insert = decltype(std::declval<T &>().insert(
            std::declval<T>().begin(), *std::declval<T>().begin()));
        template<typename T>
        using has_range_insert = decltype(std::declval<T &>().insert(
            std::declval<T>().begin(),
            std::declval<T>().begin(),
            std::declval<T>().end()));

        template<typename T>
        constexpr bool is_container_v = is_detected_v<has_insert, T>;

        template<typename T, typename U>
        constexpr bool container_and_value_type =
            is_container_v<T> &&
            (std::is_same_v<detected_t<range_value_t, T>, U> ||
             (std::is_same_v<T, std::string> && std::is_same_v<U, char32_t>));

        template<typename T>
        constexpr bool is_parsable_code_unit_impl =
            std::is_same_v<T, char> || std::is_same_v<T, wchar_t> ||



            std::is_same_v<T, char16_t> || std::is_same_v<T, char32_t>;

        template<typename T>
        constexpr bool is_parsable_code_unit_v =
            is_parsable_code_unit_impl<std::remove_cv_t<T>>;

        template<typename T>
        constexpr bool is_parsable_iter_v = is_parsable_code_unit_v<
            remove_cv_ref_t<detected_t<iter_value_t, T>>>;

        template<typename T>
        constexpr bool is_parsable_range_v = is_parsable_code_unit_v<
            remove_cv_ref_t<detected_t<has_begin, T>>> &&
            is_detected_v<has_end, T>;

        template<typename T>
        constexpr bool is_parsable_pointer_v =
            std::is_pointer_v<remove_cv_ref_t<T>> && is_parsable_code_unit_v<
                std::remove_pointer_t<remove_cv_ref_t<T>>>;

        template<typename T>
        constexpr bool is_parsable_range_like_v =
            is_parsable_range_v<T> || is_parsable_pointer_v<T>;
    }

    template<typename T>
    constexpr bool container = detail::is_container_v<T>;

    namespace detail {


        template<typename T, bool = std::is_pointer_v<T>>
        constexpr bool is_code_unit_pointer_v = false;
        template<typename T>
        constexpr bool is_code_unit_pointer_v<T, true> =
            is_parsable_code_unit_v<std::remove_pointer_t<T>>;

        template<typename T>
        constexpr bool is_range_like = is_range<T> || is_code_unit_pointer_v<T>;

        template<typename I>
        constexpr bool is_char8_iter_v =



            false

            ;



        template<bool WrapInOptional, typename Tuple>
        struct to_hana_tuple_or_type_impl;

        template<typename... T>
        struct to_hana_tuple_or_type_impl<true, tuple<T...>>
        {
            using type = std::optional<std::variant<T...>>;
        };

        template<typename... T>
        struct to_hana_tuple_or_type_impl<false, tuple<T...>>
        {
            using type = std::variant<T...>;
        };

        template<typename T>
        struct to_hana_tuple_or_type_impl<true, tuple<T>>
        {



            using type =
                std::conditional_t<is_optional_v<T>, T, std::optional<T>>;
        };

        template<typename T>
        struct to_hana_tuple_or_type_impl<false, tuple<T>>
        {
            using type = T;
        };

        template<>
        struct to_hana_tuple_or_type_impl<true, tuple<>>
        {
            using type = nope;
        };

        template<>
        struct to_hana_tuple_or_type_impl<false, tuple<>>
        {
            using type = nope;
        };

        template<typename Pair>
        struct to_hana_tuple_or_type;

        template<typename Tuple, typename TrueFalse>
        struct to_hana_tuple_or_type<tuple<Tuple, TrueFalse>>
        {

            using type =
                typename to_hana_tuple_or_type_impl<TrueFalse::value, Tuple>::
                    type;
        };

        template<typename T>
        using to_hana_tuple_or_type_t = typename to_hana_tuple_or_type<T>::type;

        template<typename T>
        auto make_sequence_of()
        {
            if constexpr (
                std::is_same_v<T, char> || std::is_same_v<T, char32_t>) {
                return std::string{};
            } else if constexpr (std::is_same_v<T, nope>) {
                return nope{};
            } else {
                return std::vector<T>{};
            }
        }

        template<typename T>
        constexpr bool is_char_type_v =
            std::is_same_v<T, char> || std::is_same_v<T, char32_t>;

        template<typename T>
        struct optional_of_impl
        {
            using type = std::optional<T>;
        };

        template<typename T>
        struct optional_of_impl<std::optional<T>>
        {
            using type = std::optional<T>;
        };

        template<>
        struct optional_of_impl<nope>
        {
            using type = nope;
        };

        template<typename T>
        using optional_of = typename optional_of_impl<T>::type;

        template<typename T>
        struct unwrapped_optional
        {
            using type = T;
        };
        template<typename T>
        struct unwrapped_optional<std::optional<T>>
        {
            using type = T;
        };
        template<typename T>
        using unwrapped_optional_t = typename unwrapped_optional<T>::type;





        template<typename T>
        struct wrapper
        {
            using type = T;

            constexpr bool operator==(wrapper) const { return true; }
        };

        struct wrap
        {
            template<typename T>
            constexpr auto operator()(T type) const
            {
                return wrapper<T>{};
            }
        };

        struct unwrap
        {
            template<typename T>
            constexpr auto operator()(T wrapped_type) const
            {
                return typename T::type{};
            }
        };

        template<typename Container, typename T>
        void insert(Container & c, T && x)
        {
            if constexpr (is_detected_v<has_push_back, Container>) {
                c.push_back((T &&) x);
            } else {
                c.insert((T &&) x);
            }
        }

        template<typename Container, typename I>
        void insert(Container & c, I first, I last)
        {
            std::for_each(first, last, [&](auto && x) {
                using type = decltype(x);
                insert(c, (type &&) x);
            });
        }

        template<typename Container, typename T>
        constexpr bool needs_transcoding_to_utf8 =
            (std::is_same_v<range_value_t<remove_cv_ref_t<Container>>, char>



                ) && (std::is_same_v<remove_cv_ref_t<T>, char32_t>

             || std::is_same_v<remove_cv_ref_t<T>, wchar_t>

             );

        template<typename Container, typename T>
        void append(Container & c, T && x, bool gen_attrs)
        {
            if (!gen_attrs)
                return;
            if constexpr (needs_transcoding_to_utf8<Container, T>) {
                char32_t cps[1] = {(char32_t)x};
                auto const r = cps | text::as_utf8;
                c.insert(c.end(), r.begin(), r.end());
            } else {
                detail::insert(c, std::move(x));
            }
        }

        template<typename Container>
        void append(Container & c, nope &&, bool)
        {}

        template<typename T>
        void append(nope &, T &&, bool)
        {}

        inline void append(nope &, nope &&, bool) {}

        template<typename Container, typename Iter, typename Sentinel>
        void append(Container & c, Iter first, Sentinel last, bool gen_attrs)
        {
            if (!gen_attrs)
                return;
            if constexpr (needs_transcoding_to_utf8<
                              Container,
                              iter_value_t<Iter>>) {
                auto const r =
                    boost::parser::subrange(first, last) | text::as_utf8;
                c.insert(c.end(), r.begin(), r.end());
            } else {
                detail::insert(c, first, last);
            }
        }

        template<typename Iter, typename Sentinel>
        void append(nope &, Iter first, Sentinel last, bool gen_attrs)
        {}

        constexpr inline flags default_flags()
        {
            return flags(
                uint32_t(flags::gen_attrs) | uint32_t(flags::use_skip));
        }
        constexpr inline flags enable_skip(flags f)
        {
            return flags(uint32_t(f) | uint32_t(flags::use_skip));
        }
        constexpr inline flags disable_skip(flags f)
        {
            return flags(uint32_t(f) & ~uint32_t(flags::use_skip));
        }
        constexpr inline flags enable_attrs(flags f)
        {
            return flags(uint32_t(f) | uint32_t(flags::gen_attrs));
        }
        constexpr inline flags disable_attrs(flags f)
        {
            return flags(uint32_t(f) & ~uint32_t(flags::gen_attrs));
        }
        constexpr inline flags enable_trace(flags f)
        {
            return flags(uint32_t(f) | uint32_t(flags::trace));
        }
        constexpr inline flags disable_trace(flags f)
        {
            return flags(uint32_t(f) & ~uint32_t(flags::trace));
        }
        constexpr inline flags set_in_apply_parser(flags f)
        {
            return flags(uint32_t(f) | uint32_t(flags::in_apply_parser));
        }
        constexpr inline bool gen_attrs(flags f)
        {
            return (uint32_t(f) & uint32_t(flags::gen_attrs)) ==
                   uint32_t(flags::gen_attrs);
        }
        constexpr inline bool use_skip(flags f)
        {
            return (uint32_t(f) & uint32_t(flags::use_skip)) ==
                   uint32_t(flags::use_skip);
        }
        constexpr inline bool in_apply_parser(flags f)
        {
            return (uint32_t(f) & uint32_t(flags::in_apply_parser)) ==
                   uint32_t(flags::in_apply_parser);
        }

        struct null_parser
        {};

        struct skip_skipper
        {
            template<
                bool UseCallbacks,
                typename Iter,
                typename Sentinel,
                typename Context,
                typename SkipParser>
            nope operator()(
                std::bool_constant<UseCallbacks> use_cbs,
                Iter & first,
                Sentinel last,
                Context const & context,
                SkipParser const & skip,
                flags flags,
                bool & success) const noexcept
            {
                return {};
            }

            template<
                bool UseCallbacks,
                typename Iter,
                typename Sentinel,
                typename Context,
                typename SkipParser,
                typename Attribute>
            void operator()(
                std::bool_constant<UseCallbacks> use_cbs,
                Iter & first,
                Sentinel last,
                Context const & context,
                SkipParser const & skip,
                flags flags,
                bool & success,
                Attribute & retval) const
            {}
        };

        template<typename Iter, typename Sentinel>
        void
        skip(Iter & first, Sentinel last, null_parser const & skip_, flags f)
        {}

        template<typename Iter, typename Sentinel, typename SkipParser>
        void
        skip(Iter & first, Sentinel last, SkipParser const & skip_, flags f)
        {
            if (!detail::use_skip(f))
                return;
            bool success = true;
            int indent = 0;
            rethrow_error_handler eh;
            nope n;
            symbol_table_tries_t symbol_table_tries;
            auto const context = detail::make_context(
                first, last, success, indent, eh, n, symbol_table_tries);
            while (success) {
                skip_(
                    std::false_type{},
                    first,
                    last,
                    context,
                    skip_skipper{},
                    detail::disable_trace(detail::disable_skip(f)),
                    success);
            }
        }

        enum : int64_t { unbounded = -1 };

        template<typename T>
        std::optional<T> make_parse_result(T & x, bool success)
        {
            std::optional<T> retval;
            if (success)
                retval = x;
            return retval;
        }

        inline bool make_parse_result(nope &, bool success) { return success; }

        template<typename LoType, typename HiType>
        struct char_pair
        {
            LoType lo_;
            HiType hi_;
        };

        using case_fold_array_t = std::array<char32_t, detail::longest_mapping>;

        template<typename I, typename S>
        struct no_case_iter : stl_interfaces::iterator_interface<
                                  no_case_iter<I, S>,
                                  std::forward_iterator_tag,
                                  char32_t,
                                  char32_t>
        {
            no_case_iter() : it_(), last_(), idx_(0), last_idx_() {}
            no_case_iter(I it, S last) :
                it_(it), last_(last), idx_(0), last_idx_(0)
            {
                fold();
            }

            char32_t operator*() const { return folded_[idx_]; }
            no_case_iter & operator++()
            {
                ++idx_;
                if (last_idx_ <= idx_) {
                    ++it_;
                    fold();
                }
                return *this;
            }
            I base() const { return it_; }
            friend bool operator==(no_case_iter lhs, S rhs) noexcept
            {
                return lhs.it_ == rhs;
            }
            friend bool operator==(no_case_iter lhs, no_case_iter rhs) noexcept
            {
                return lhs.it_ == rhs.it_ && lhs.idx_ == rhs.idx_;
            }

            using base_type = stl_interfaces::iterator_interface<
                no_case_iter<I, S>,
                std::forward_iterator_tag,
                char32_t,
                char32_t>;
            using base_type::operator++;

        private:
            void fold()
            {
                idx_ = 0;
                if (it_ == last_) {
                    folded_[0] = 0;
                    last_idx_ = 1;
                    return;
                }
                auto const folded_last =
                    detail::case_fold(*it_, folded_.begin());
                last_idx_ = folded_last - folded_.begin();
            }

            case_fold_array_t folded_;
            I it_;
            [[no_unique_address]] S last_;
            int idx_;
            int last_idx_;
        };

        template<>
        struct char_subranges<hex_digit_subranges>
        {
            static constexpr char_subrange ranges[] = {
                {U'0', U'9'},
                {U'A', U'F'},
                {U'a', U'f'},
                {U'\uff10', U'\uff19'},
                {U'\uff21', U'\uff26'},
                {U'\uff41', U'\uff46'}};
        };

        template<>
        struct char_subranges<control_subranges>
        {
            static constexpr char_subrange ranges[] = {
                {U'\u0000', U'\u001f'}, {U'\u007f', U'\u009f'}};
        };

        template<typename Iter, typename Sentinel, bool SortedUTF32>
        struct char_range
        {
            template<typename T, typename Context>
            bool contains(T c_, Context const & context) const
            {
                if constexpr (SortedUTF32) {
                    return std::binary_search(chars_.begin(), chars_.end(), c_);
                }

                if (context.no_case_depth_) {
                    case_fold_array_t folded;
                    auto folded_last = detail::case_fold(c_, folded.begin());
                    if constexpr (std::is_same_v<T, char32_t>) {
                        auto const cps = chars_ | text::as_utf32;
                        auto chars_first = no_case_iter(cps.begin(), cps.end());
                        auto chars_last = cps.end();
                        auto result = text::search(
                            chars_first,
                            chars_last,
                            folded.begin(),
                            folded_last);
                        return !result.empty();
                    } else {
                        auto chars_first =
                            no_case_iter(chars_.begin(), chars_.end());
                        auto chars_last = chars_.end();
                        auto result = text::search(
                            chars_first,
                            chars_last,
                            folded.begin(),
                            folded_last);
                        return !result.empty();
                    }
                } else {
                    if constexpr (std::is_same_v<T, char32_t>) {
                        auto const cps = chars_ | text::as_utf32;
                        return text::find(cps.begin(), cps.end(), c_) !=
                               cps.end();
                    } else {
                        using element_type = decltype(*chars_.begin());
                        element_type const c = c_;
                        return text::find(chars_.begin(), chars_.end(), c) !=
                               chars_.end();
                    }
                }
            }

            boost::parser::subrange<Iter, Sentinel> chars_;
        };

        template<bool SortedUTF32, typename Iter, typename Sentinel>
        constexpr auto make_char_range(Iter first, Sentinel last)
        {
            return char_range<Iter, Sentinel, SortedUTF32>{
                boost::parser::subrange<Iter, Sentinel>{first, last}};
        }

        template<bool SortedUTF32, typename R>
        constexpr auto make_char_range(R && r) noexcept
        {
            if constexpr (std::is_pointer_v<std::decay_t<R>>) {
                return detail::make_char_range<SortedUTF32>(
                    r, text::null_sentinel);
            } else {
                return detail::make_char_range<SortedUTF32>(r.begin(), r.end());
            }
        }

        template<bool Equal, typename Context>
        auto no_case_aware_compare(Context const & context)
        {
            return [no_case = context.no_case_depth_](char32_t a, char32_t b) {
                if (no_case) {
                    case_fold_array_t folded_a = {0, 0, 0};
                    detail::case_fold(a, folded_a.begin());
                    case_fold_array_t folded_b = {0, 0, 0};
                    detail::case_fold(b, folded_b.begin());
                    return Equal ? folded_a == folded_b : folded_a < folded_b;
                } else {
                    return Equal ? a == b : a < b;
                }
            };
        }

        template<typename T, typename U>
        constexpr bool both_character_types =
            is_parsable_code_unit_v<T> && is_parsable_code_unit_v<U>;

        template<typename T, typename U>
        using eq_comparable =
            decltype(std::declval<T &>() == std::declval<U &>());

        template<
            typename Context,
            typename CharType,
            typename Expected,
            bool BothCharacters = both_character_types<CharType, Expected>>
        struct unequal_impl
        {
            static bool
            call(Context const & context, CharType c, Expected const & expected)
            {
                auto resolved = detail::resolve(context, expected);
                if constexpr (is_detected_v<
                                  eq_comparable,
                                  CharType,
                                  decltype(resolved)>) {
                    auto const compare =
                        detail::no_case_aware_compare<true>(context);
                    return !compare(c, resolved);
                } else {
                    return !resolved.contains(c, context);
                }
            }
        };

        template<typename Context, typename CharType, typename Expected>
        struct unequal_impl<Context, CharType, Expected, true>
        {
            static bool
            call(Context const & context, CharType c, Expected expected)
            {

                return !detail::no_case_aware_compare<true>(context)(
                    c, expected);
            }
        };

        template<typename Context, typename CharType, typename Expected>
        bool unequal(Context const & context, CharType c, Expected expected)
        {
            return unequal_impl<Context, CharType, Expected>::call(
                context, c, expected);
        }

        template<
            typename Context,
            typename CharType,
            typename LoType,
            typename HiType>
        bool unequal(
            Context const & context,
            CharType c,
            char_pair<LoType, HiType> const & expected)
        {
            auto const less = detail::no_case_aware_compare<false>(context);
            {
                auto lo = detail::resolve(context, expected.lo_);
                if (less(c, lo))
                    return true;
            }
            {
                auto hi = detail::resolve(context, expected.hi_);
                if (less(hi, c))
                    return true;
            }
            return false;
        }

        template<typename Context, typename CharType>
        bool unequal(Context const &, CharType, nope)
        {
            return false;
        }

        template<typename Container, typename T>
        using insertable = decltype(std::declval<Container &>().insert(
            std::declval<Container &>().end(), std::declval<T>()));

        template<typename T, typename Tuple, int... Is>
        auto
        make_from_tuple_impl(Tuple && tup, std::integer_sequence<int, Is...>)
            -> decltype(T(parser::get(std::move(tup), llong<Is>{})...))
        {
            return T(parser::get(std::move(tup), llong<Is>{})...);
        }

        template<typename T, typename... Args>
        auto make_from_tuple(tuple<Args...> && tup)
            -> decltype(detail::make_from_tuple_impl<T>(
                std::move(tup),
                std::make_integer_sequence<int, tuple_size_<tuple<Args...>>>()))
        {
            return detail::make_from_tuple_impl<T>(
                std::move(tup),
                std::make_integer_sequence<int, tuple_size_<tuple<Args...>>>());
        }

        template<typename T, typename Tuple>
        using constructible_from_tuple_expr =
            decltype(detail::make_from_tuple<T>(std::declval<Tuple>()));

        template<typename T, typename Tuple, bool = is_tuple<Tuple>{}>
        constexpr bool is_constructible_from_tuple_v = false;
        template<typename T, typename Tuple>
        constexpr bool is_constructible_from_tuple_v<T, Tuple, true> =
            is_detected_v<constructible_from_tuple_expr, T, Tuple>;

        template<typename Container, typename U>
        constexpr void move_back_impl(Container & c, U && x)
        {
            using just_t = range_value_t<Container>;
            using just_u = remove_cv_ref_t<U>;
            if constexpr (needs_transcoding_to_utf8<Container, U>) {
                char32_t cps[1] = {(char32_t)x};
                auto const r = cps | text::as_utf8;
                c.insert(c.end(), r.begin(), r.end());
            } else if constexpr (std::is_convertible_v<just_u &&, just_t>) {
                detail::insert(c, std::move(x));
            } else if constexpr (
                !is_tuple<just_t>::value && is_tuple<just_u>::value &&
                std::is_aggregate_v<just_t> &&
                !is_detected_v<insertable, Container, just_u &&> &&
                is_struct_assignable_v<just_t, just_u>) {
                auto int_seq =
                    std::make_integer_sequence<int, tuple_size_<just_u>>();
                detail::insert(
                    c,
                    detail::tuple_to_aggregate<just_t>(std::move(x), int_seq));
            } else if constexpr (
                is_tuple<just_t>::value && !is_tuple<just_u>::value &&
                std::is_aggregate_v<just_u> &&
                !is_detected_v<insertable, Container, just_u &&> &&
                is_tuple_assignable_v<just_t, just_u>) {
                just_t t;
                auto tie = detail::tie_aggregate(x);
                detail::aggregate_to_tuple(
                    t,
                    tie,
                    std::make_integer_sequence<int, tuple_size_<just_t>>());
                detail::insert(c, std::move(t));
            } else if constexpr (is_constructible_from_tuple_v<
                                     just_t,
                                     just_u>) {
                detail::insert(
                    c, detail::make_from_tuple<just_t>(std::move(x)));
            } else {
                static_assert(
                    sizeof(U) && false,
                    "Could not insert value into container, by: just inserting "
                    "it; doing tuple -> aggregate or aggregate -> tuple "
                    "conversions; or tuple -> class type construction.");
            }
        }

        template<typename Container, typename T>
        constexpr void move_back(Container & c, T && x, bool gen_attrs)
        {
            if (!gen_attrs)
                return;
            detail::move_back_impl(c, std::move(x));
        }

        template<typename Container>
        constexpr void
        move_back(Container & c, Container & x, bool gen_attrs)
        {
            if (!gen_attrs)
                return;
            c.insert(c.end(), x.begin(), x.end());
        }

        template<typename Container>
        constexpr void move_back(
            Container & c, std::optional<Container> & x, bool gen_attrs)
        {
            if (!gen_attrs || !x)
                return;
            c.insert(c.end(), x->begin(), x->end());
        }

        template<typename Container, typename T>
        constexpr void
        move_back(Container & c, std::optional<T> & x, bool gen_attrs)
        {
            if (!gen_attrs || !x)
                return;
            detail::move_back_impl(c, std::move(*x));
        }

        template<typename Container, typename T>
        constexpr void
        move_back(Container & c, std::optional<T> && x, bool gen_attrs)
        {
            if (!gen_attrs || !x)
                return;
            detail::move_back_impl(c, std::move(*x));
        }

        constexpr void move_back(nope, nope, bool gen_attrs) {}

        template<typename Container>
        constexpr void move_back(Container & c, nope, bool gen_attrs)
        {}

        template<typename From, typename To>
        using move_assignable_expr =
            decltype(std::declval<To &>() = std::declval<From &&>());
        template<typename From, typename To>
        constexpr bool is_move_assignable_v =
            is_detected_v<move_assignable_expr, From, To>;

        template<typename T, typename U>
        constexpr void assign(T & t, U && u)
        {
            using just_t = remove_cv_ref_t<T>;
            using just_u = remove_cv_ref_t<U>;
            if constexpr (is_move_assignable_v<just_u, just_t>) {
                static_assert(
                    (!std::is_same_v<just_t, std::string> ||
                     !std::is_arithmetic_v<just_u>),
                    "std::string is assignable from a char.  Due to implicit "
                    "conversions among arithmetic types, any arithmetic type "
                    "(like int or double) is assignable to std::string.  This "
                    "is almost certainly not what you meant to write, so "
                    "Boost.Parser disallows it.  If you want to do this, write "
                    "a semantic action and do it explicitly.");
                t = std::move(u);
            } else if constexpr (
                !is_tuple<just_t>::value && is_tuple<just_u>::value &&
                std::is_aggregate_v<just_t> &&
                !std::is_convertible_v<just_u &&, just_t> &&
                is_struct_assignable_v<just_t, just_u>) {
                auto int_seq =
                    std::make_integer_sequence<int, tuple_size_<just_u>>();
                t = detail::tuple_to_aggregate<just_t>(std::move(u), int_seq);
            } else if constexpr (
                is_tuple<just_t>::value && !is_tuple<just_u>::value &&
                std::is_aggregate_v<just_u> &&
                !std::is_convertible_v<just_u &&, just_t> &&
                is_tuple_assignable_v<just_t, just_u>) {
                auto tie = detail::tie_aggregate(u);
                detail::aggregate_to_tuple(
                    t,
                    tie,
                    std::make_integer_sequence<int, tuple_size_<just_t>>());
            } else if constexpr (is_constructible_from_tuple_v<
                                     just_t,
                                     just_u>) {
                t = detail::make_from_tuple<just_t>(std::move(u));
            } else {
                static_assert(
                    sizeof(T) && false,
                    "Could not assign value, by: just assigning it; doing tuple "
                    "-> aggregate or aggregate -> tuple conversions; or tuple "
                    "-> class type construction.");
            }
        }

        template<typename T>
        constexpr void assign(T &, nope)
        {}

        template<typename T, typename U>
        constexpr void assign_copy(T & t, U const & u)
        {
            t = u;
        }

        template<typename T>
        constexpr void assign_copy(T &, nope)
        {}

        template<
            typename Parser,
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename... T>
        void apply_parser(
            Parser const & parser,
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            flags flags,
            bool & success,
            std::optional<std::variant<T...>> & retval)
        {
            using attr_t = decltype(parser.call(
                use_cbs, first, last, context, skip, flags, success));
            if constexpr (std::is_same<
                              attr_t,
                              std::optional<std::variant<T...>>>{}) {
                parser.call(
                    use_cbs,
                    first,
                    last,
                    context,
                    skip,
                    flags,
                    success,
                    retval);
            } else if constexpr (is_nope_v<attr_t>) {
                parser.call(
                    use_cbs, first, last, context, skip, flags, success);
            } else {
                auto attr = parser.call(
                    use_cbs, first, last, context, skip, flags, success);
                if (success)
                    detail::assign(retval, std::variant<T...>(std::move(attr)));
            }
        }

        template<
            typename Parser,
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename... T>
        void apply_parser(
            Parser const & parser,
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            flags flags,
            bool & success,
            std::variant<T...> & retval)
        {
            auto attr = parser.call(
                use_cbs, first, last, context, skip, flags, success);
            if (success)
                detail::assign(retval, std::move(attr));
        }

        template<
            typename Parser,
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename T>
        void apply_parser(
            Parser const & parser,
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            flags flags,
            bool & success,
            std::optional<T> & retval)
        {
            auto attr = parser.call(
                use_cbs, first, last, context, skip, flags, success);
            if (success)
                detail::assign(retval, std::move(attr));
        }

        template<
            typename Parser,
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename Attribute>
        void apply_parser(
            Parser const & parser,
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            flags flags,
            bool & success,
            Attribute & retval)
        {
            parser.call(
                use_cbs, first, last, context, skip, flags, success, retval);
        }





        template<typename Iter, typename Sentinel, typename Parser>
        auto has_attribute(Iter first, Sentinel last, Parser parser)
        {
            constexpr auto flags = detail::flags::gen_attrs;
            using context = decltype(detail::make_context(
                first,
                last,
                std::declval<bool &>(),
                std::declval<int &>(),
                parser.error_handler_,
                parser.globals_,
                std::declval<detail::symbol_table_tries_t &>()));
            using attr_t = decltype(parser(
                std::false_type{},
                first,
                last,
                std::declval<context>(),
                detail::null_parser{},
                flags,
                std::declval<bool &>()));
            return std::integral_constant<bool, !is_nope_v<attr_t>>{};
        }

        template<typename BaseIter, typename Iter>
        struct scoped_base_assign
        {
            scoped_base_assign(BaseIter & base, Iter & it) :
                base_(base), it_(it)
            {}
            ~scoped_base_assign() { base_ = it_.base(); }

            BaseIter & base_;
            Iter & it_;
        };

        template<
            bool Debug,
            typename Iter,
            typename Sentinel,
            typename Parser,
            typename Attr,
            typename ErrorHandler>
        bool parse_impl(
            Iter & first,
            Sentinel last,
            Parser const & parser,
            ErrorHandler const & error_handler,
            Attr & attr)
        {
            auto const initial_first = first;
            bool success = true;
            int trace_indent = 0;
            detail::symbol_table_tries_t symbol_table_tries;
            auto context = detail::make_context(
                first,
                last,
                success,
                trace_indent,
                error_handler,
                parser.globals_,
                symbol_table_tries);
            auto const flags =
                Debug ? detail::enable_trace(detail::flags::gen_attrs)
                      : detail::flags::gen_attrs;
            try {
                parser(
                    std::false_type{},
                    first,
                    last,
                    context,
                    detail::null_parser{},
                    flags,
                    success,
                    attr);
                if (Debug)
                    detail::final_trace(context, flags, attr);
                return success;
            } catch (parse_error<Iter> const & e) {
                if (error_handler(initial_first, last, e) ==
                    error_handler_result::rethrow) {
                    throw;
                }
                return false;
            }
        }

        template<
            bool Debug,
            typename Iter,
            typename Sentinel,
            typename Parser,
            typename ErrorHandler>
        auto parse_impl(
            Iter & first,
            Sentinel last,
            Parser const & parser,
            ErrorHandler const & error_handler)
        {
            auto const initial_first = first;
            bool success = true;
            int trace_indent = 0;
            detail::symbol_table_tries_t symbol_table_tries;
            auto context = detail::make_context(
                first,
                last,
                success,
                trace_indent,
                error_handler,
                parser.globals_,
                symbol_table_tries);
            auto const flags =
                Debug ? detail::enable_trace(detail::flags::gen_attrs)
                      : detail::flags::gen_attrs;
            using attr_t = decltype(parser(
                std::false_type{},
                first,
                last,
                context,
                detail::null_parser{},
                flags,
                success));
            try {
                attr_t attr_ = parser(
                    std::false_type{},
                    first,
                    last,
                    context,
                    detail::null_parser{},
                    flags,
                    success);
                if (Debug)
                    detail::final_trace(context, flags, nope{});
                return detail::make_parse_result(attr_, success);
            } catch (parse_error<Iter> const & e) {
                if (error_handler(initial_first, last, e) ==
                    error_handler_result::rethrow) {
                    throw;
                }
                attr_t attr_{};
                return detail::make_parse_result(attr_, false);
            }
        }

        template<
            bool Debug,
            typename Iter,
            typename Sentinel,
            typename Parser,
            typename ErrorHandler,
            typename Callbacks>
        bool callback_parse_impl(
            Iter & first,
            Sentinel last,
            Parser const & parser,
            ErrorHandler const & error_handler,
            Callbacks const & callbacks)
        {
            auto const initial_first = first;
            bool success = true;
            int trace_indent = 0;
            detail::symbol_table_tries_t symbol_table_tries;
            auto context = detail::make_context(
                first,
                last,
                success,
                trace_indent,
                error_handler,
                callbacks,
                parser.globals_,
                symbol_table_tries);
            auto const flags =
                Debug ? detail::enable_trace(detail::flags::gen_attrs)
                      : detail::flags::gen_attrs;
            try {
                parser(
                    std::true_type{},
                    first,
                    last,
                    context,
                    detail::null_parser{},
                    flags,
                    success);
                if (Debug)
                    detail::final_trace(context, flags, nope{});
                return success;
            } catch (parse_error<Iter> const & e) {
                if (error_handler(initial_first, last, e) ==
                    error_handler_result::rethrow) {
                    throw;
                }
                return false;
            }
        }

        template<
            bool Debug,
            typename Iter,
            typename Sentinel,
            typename Parser,
            typename SkipParser,
            typename Attr,
            typename ErrorHandler>
        bool skip_parse_impl(
            Iter & first,
            Sentinel last,
            Parser const & parser,
            SkipParser const & skip,
            ErrorHandler const & error_handler,
            Attr & attr)
        {
            auto const initial_first = first;
            bool success = true;
            int trace_indent = 0;
            detail::symbol_table_tries_t symbol_table_tries;
            auto context = detail::make_context(
                first,
                last,
                success,
                trace_indent,
                error_handler,
                parser.globals_,
                symbol_table_tries);
            auto const flags =
                Debug ? detail::enable_trace(detail::default_flags())
                      : detail::default_flags();
            detail::skip(first, last, skip, flags);
            try {
                parser(
                    std::false_type{},
                    first,
                    last,
                    context,
                    skip,
                    flags,
                    success,
                    attr);
                detail::skip(first, last, skip, flags);
                if (Debug)
                    detail::final_trace(context, flags, attr);
                return success;
            } catch (parse_error<Iter> const & e) {
                if (error_handler(initial_first, last, e) ==
                    error_handler_result::rethrow) {
                    throw;
                }
                return false;
            }
        }

        template<
            bool Debug,
            typename Iter,
            typename Sentinel,
            typename Parser,
            typename SkipParser,
            typename ErrorHandler>
        auto skip_parse_impl(
            Iter & first,
            Sentinel last,
            Parser const & parser,
            SkipParser const & skip,
            ErrorHandler const & error_handler)
        {
            auto const initial_first = first;
            bool success = true;
            int trace_indent = 0;
            detail::symbol_table_tries_t symbol_table_tries;
            auto context = detail::make_context(
                first,
                last,
                success,
                trace_indent,
                error_handler,
                parser.globals_,
                symbol_table_tries);
            auto const flags =
                Debug ? detail::enable_trace(detail::default_flags())
                      : detail::default_flags();
            detail::skip(first, last, skip, flags);
            using attr_t = decltype(parser(
                std::false_type{}, first, last, context, skip, flags, success));
            try {
                attr_t attr_ = parser(
                    std::false_type{},
                    first,
                    last,
                    context,
                    skip,
                    flags,
                    success);
                detail::skip(first, last, skip, flags);
                if (Debug)
                    detail::final_trace(context, flags, nope{});
                return detail::make_parse_result(attr_, success);
            } catch (parse_error<Iter> const & e) {
                if (error_handler(initial_first, last, e) ==
                    error_handler_result::rethrow) {
                    throw;
                }
                attr_t attr_{};
                return detail::make_parse_result(attr_, false);
            }
        }

        template<
            bool Debug,
            typename Iter,
            typename Sentinel,
            typename Parser,
            typename SkipParser,
            typename ErrorHandler,
            typename Callbacks>
        bool callback_skip_parse_impl(
            Iter & first,
            Sentinel last,
            Parser const & parser,
            SkipParser const & skip,
            ErrorHandler const & error_handler,
            Callbacks const & callbacks)
        {
            auto const initial_first = first;
            bool success = true;
            int trace_indent = 0;
            detail::symbol_table_tries_t symbol_table_tries;
            auto context = detail::make_context(
                first,
                last,
                success,
                trace_indent,
                error_handler,
                callbacks,
                parser.globals_,
                symbol_table_tries);
            auto const flags =
                Debug ? detail::enable_trace(detail::default_flags())
                      : detail::default_flags();
            detail::skip(first, last, skip, flags);
            try {
                parser(
                    std::true_type{},
                    first,
                    last,
                    context,
                    skip,
                    flags,
                    success);
                detail::skip(first, last, skip, flags);
                if (Debug)
                    detail::final_trace(context, flags, nope{});
                return success;
            } catch (parse_error<Iter> const & e) {
                if (error_handler(initial_first, last, e) ==
                    error_handler_result::rethrow) {
                    throw;
                }
                return false;
            }
        }

        template<typename R>
        constexpr auto make_input_subrange(R && r) noexcept
        {
            using r_t = remove_cv_ref_t<R>;
            if constexpr (std::is_pointer_v<r_t>) {
                using value_type = iter_value_t<r_t>;
                if constexpr (std::is_same_v<value_type, char>) {
                    return boost::parser::subrange(r, text::null_sentinel);
                } else {
                    return r | text::as_utf32;
                }
            } else {
                using value_type = range_value_t<r_t>;
                if constexpr (text::detail::is_bounded_array_v<r_t>) {
                    if constexpr (std::is_same_v<value_type, char>) {
                        auto first = detail::text::detail::begin(r);
                        auto last = detail::text::detail::end(r);
                        if (first != last && !*std::prev(last))
                            --last;
                        return boost::parser::subrange(first, last);
                    } else {
                        return r | text::as_utf32;
                    }
                } else {
                    if constexpr (
                        std::is_same_v<value_type, char> &&
                        !is_utf8_view<r_t>::value) {
                        return boost::parser::subrange(
                            detail::text::detail::begin(r),
                            detail::text::detail::end(r));
                    } else {
                        return r | text::as_utf32;
                    }
                }
            }
        }

        template<typename R>
        constexpr auto make_view_begin(R & r) noexcept
        {
            if constexpr (std::is_pointer_v<std::decay_t<R>>) {
                return r;
            } else {
                return detail::text::detail::begin(r);
            }
        }

        template<typename R>
        constexpr auto make_view_end(R & r) noexcept
        {
            if constexpr (std::is_pointer_v<std::decay_t<R>>) {
                return text::null_sentinel;
            } else {
                return detail::text::detail::end(r);
            }
        }

        template<
            typename Iter1,
            typename Sentinel1,
            typename Iter2,
            typename Sentinel2,
            typename Pred>
        std::pair<Iter1, Iter2> mismatch(
            Iter1 first1,
            Sentinel1 last1,
            Iter2 first2,
            Sentinel2 last2,
            Pred pred)
        {
            std::pair<Iter1, Iter2> retval{first1, first2};
            while (retval.first != last1 && retval.second != last2 &&
                   pred(*retval.first, *retval.second)) {
                ++retval.first;
                ++retval.second;
            }
            return retval;
        }

        template<
            typename Iter1,
            typename Sentinel1,
            typename Iter2,
            typename Sentinel2>
        std::pair<Iter1, Iter2> no_case_aware_string_mismatch(
            Iter1 first1,
            Sentinel1 last1,
            Iter2 first2,
            Sentinel2 last2,
            bool no_case)
        {
            if (no_case) {
                auto it1 = no_case_iter(first1, last1);
                auto it2 = no_case_iter(first2, last2);
                auto const mismatch = detail::mismatch(
                    it1, last1, it2, last2, std::equal_to<char32_t>{});
                return std::pair<Iter1, Iter2>{
                    mismatch.first.base(), mismatch.second.base()};
            } else {
                return detail::mismatch(
                    first1, last1, first2, last2, std::equal_to<char32_t>{});
            }
        }

        template<typename I, typename S, typename T>
        std::optional<T>
        if_full_parse(I & first, S last, std::optional<T> retval)
        {
            if (first != last)
                retval = std::nullopt;
            return retval;
        }
        template<typename I, typename S>
        bool if_full_parse(I & first, S last, bool retval)
        {
            if (first != last)
                retval = false;
            return retval;
        }



        template<typename Struct, typename Tuple>
        constexpr auto is_struct_compatible();

        struct element_compatibility
        {
            template<typename T, typename U>
            constexpr auto operator()(T result, U x) const
            {
                using struct_elem =
                    remove_cv_ref_t<decltype(parser::get(x, llong<0>{}))>;
                using tuple_elem =
                    remove_cv_ref_t<decltype(parser::get(x, llong<1>{}))>;
                if constexpr (!T::value) {
                    return std::false_type{};
                } else if constexpr (
                    is_optional_v<struct_elem> && is_optional_v<tuple_elem>) {
                    using struct_opt_type = optional_type<struct_elem>;
                    using tuple_opt_type = optional_type<tuple_elem>;
                    using retval_t = decltype((*this)(
                        result,
                        detail::hl::make_tuple(
                            std::declval<struct_opt_type &>(),
                            std::declval<tuple_opt_type &>())));
                    return retval_t{};
                } else if constexpr (std::is_convertible_v<
                                         tuple_elem &&,
                                         struct_elem>) {
                    return std::true_type{};
                } else if constexpr (
                    container<struct_elem> && container<tuple_elem>) {
                    return detail::is_struct_compatible<
                        range_value_t<struct_elem>,
                        range_value_t<tuple_elem>>();
                } else {
                    return std::bool_constant<detail::is_struct_compatible<
                        struct_elem,
                        tuple_elem>()>{};
                }
            }
        };

        template<typename T, typename U>
        constexpr auto is_struct_compatible()
        {
            if constexpr (
                !std::is_aggregate_v<T> ||
                struct_arity_v<T> != tuple_size_<U>) {
                return std::false_type{};
            } else {
                using result_t = decltype(detail::hl::fold_left(
                    detail::hl::zip(
                        detail::tie_aggregate(std::declval<T &>()),
                        std::declval<U &>()),
                    std::true_type{},
                    element_compatibility{}));
                return result_t{};
            }
        }

        template<typename Struct, typename Tuple>
        constexpr bool is_struct_compatible_v =
            detail::is_struct_compatible<Struct, Tuple>();

        template<typename ParserAttr, typename GivenContainerAttr>
        constexpr auto parser_attr_or_container_value_type()
        {
            if constexpr (is_nope_v<ParserAttr>) {
                return nope{};
            } else {
                using value_type = range_value_t<GivenContainerAttr>;
                return std::conditional_t<
                    std::is_convertible_v<ParserAttr, value_type>,
                    ParserAttr,
                    value_type>{};
            }
        }
        template<typename ParserAttr, typename GivenContainerAttr>
        using parser_attr_or_container_value_type_v =
            decltype(parser_attr_or_container_value_type<
                     ParserAttr,
                     GivenContainerAttr>());

        template<typename T>
        constexpr auto tuple_or_struct_size(T && x)
        {
            if constexpr (is_tuple<remove_cv_ref_t<T>>{}) {
                return hl::size(x);
            } else {
                return llong<struct_arity_v<remove_cv_ref_t<T>>>{};
            }
        }

        template<typename T>
        struct attr_reset
        {
            attr_reset(T & x) : x_(std::addressof(x)) {}
            attr_reset(attr_reset const &) = delete;
            attr_reset(attr_reset &&) = delete;
            attr_reset & operator=(attr_reset const &) = delete;
            attr_reset & operator=(attr_reset &&) = delete;
            ~attr_reset()
            {
                if (x_)
                    *x_ = T();
            }

            bool operator=(bool b)
            {
                if (b)
                    x_ = nullptr;
                return b;
            }

        private:
            T * x_;
        };
    }


    template<typename Context>
    decltype(auto) _val(Context const & context)
    {
        if constexpr (detail::is_nope_v<decltype(*context.val_)>)
            return none{};
        else
            return *context.val_;
    }

    template<typename Context>
    decltype(auto) _attr(Context const & context)
    {
        if constexpr (detail::is_nope_v<decltype(*context.attr_)>)
            return none{};
        else
            return *context.attr_;
    }

    template<typename Context>
    decltype(auto) _where(Context const & context)
    {
        return *context.where_;
    }

    template<typename Context>
    decltype(auto) _begin(Context const & context)
    {
        return context.first_;
    }

    template<typename Context>
    decltype(auto) _end(Context const & context)
    {
        return context.last_;
    }

    template<typename Context>
    decltype(auto) _pass(Context const & context)
    {
        return *context.pass_;
    }

    template<typename Context>
    decltype(auto) _locals(Context const & context)
    {
        if constexpr (detail::is_nope_v<decltype(*context.locals_)>)
            return none{};
        else
            return *context.locals_;
    }

    template<typename Context>
    decltype(auto) _params(Context const & context)
    {
        if constexpr (detail::is_nope_v<decltype(*context.params_)>)
            return none{};
        else
            return *context.params_;
    }

    template<typename Context>
    decltype(auto) _globals(Context const & context)
    {
        if constexpr (detail::is_nope_v<decltype(*context.globals_)>)
            return none{};
        else
            return *context.globals_;
    }

    template<typename Context>
    decltype(auto) _no_case(Context const & context)
    {
        return context.no_case_depth_;
    }

    template<typename Context>
    decltype(auto) _error_handler(Context const & context)
    {
        return *context.error_handler_;
    }




    template<typename I, typename Context>

    void
    _report_error(Context const & context, std::string_view message, I location)
    {
        return context.error_handler_->diagnose(
            diagnostic_kind::error, message, context, location);
    }

    template<typename Context>
    void _report_error(Context const & context, std::string_view message)
    {
        return context.error_handler_->diagnose(
            diagnostic_kind::error, message, context);
    }




    template<typename I, typename Context>

    void _report_warning(
        Context const & context, std::string_view message, I location)
    {
        return context.error_handler_->diagnose(
            diagnostic_kind::warning, message, context, location);
    }

    template<typename Context>
    void _report_warning(Context const & context, std::string_view message)
    {
        return context.error_handler_->diagnose(
            diagnostic_kind::warning, message, context);
    }



    template<unsigned int I>
    inline constexpr detail::param_t<I> _p = {};
#line 2760 "boost/parser/parser.hpp"
    int64_t const Inf = detail::unbounded;

    template<
        typename Parser,
        typename DelimiterParser,
        typename MinType,
        typename MaxType>
    struct repeat_parser
    {
        constexpr repeat_parser(
            Parser parser,
            MinType _min,
            MaxType _max,
            DelimiterParser delimiter_parser = DelimiterParser{}) :
            parser_(parser),
            delimiter_parser_(delimiter_parser),
            min_(_min),
            max_(_max)
        {}

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser>
        auto call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success) const
        {
            using attr_t = decltype(parser_.call(
                use_cbs, first, last, context, skip, flags, success));
            auto retval = detail::make_sequence_of<attr_t>();
            call(use_cbs, first, last, context, skip, flags, success, retval);
            return retval;
        }

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename Attribute>
        void call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success,
            Attribute & retval) const
        {
            auto _ = detail::scoped_trace(
                *this,
                first,
                last,
                context,
                detail::in_apply_parser(flags) ? detail::disable_trace(flags)
                                               : flags,
                retval);

            if constexpr (detail::is_optional_v<Attribute>) {
                detail::optional_type<Attribute> attr;
                detail::apply_parser(
                    *this,
                    use_cbs,
                    first,
                    last,
                    context,
                    skip,
                    detail::set_in_apply_parser(flags),
                    success,
                    attr);
                if (success)
                    retval = std::move(attr);
            } else {
                using attr_t = detail::parser_attr_or_container_value_type_v<
                    decltype(parser_.call(
                        use_cbs, first, last, context, skip, flags, success)),
                    Attribute>;

                int64_t count = 0;

                for (int64_t end = detail::resolve(context, min_); count != end;
                     ++count) {
                    detail::skip(first, last, skip, flags);
                    attr_t attr;
                    parser_.call(
                        use_cbs,
                        first,
                        last,
                        context,
                        skip,
                        flags,
                        success,
                        attr);
                    if (!success) {
                        detail::assign(retval, Attribute());
                        return;
                    }
                    detail::move_back(
                        retval, std::move(attr), detail::gen_attrs(flags));
                }

                int64_t const end = detail::resolve(context, max_);



                BOOST_ASSERT(!detail::is_unconditional_eps<Parser>{} || end < Inf);


                for (; count != end; ++count) {
                    auto const prev_first = first;



                    if constexpr (!detail::is_nope_v<DelimiterParser>) {
                        detail::skip(first, last, skip, flags);
                        delimiter_parser_.call(
                            use_cbs,
                            first,
                            last,
                            context,
                            skip,
                            detail::disable_attrs(flags),
                            success);
                        if (!success) {
                            success = true;
                            break;
                        }
                    }

                    detail::skip(first, last, skip, flags);
                    attr_t attr;
                    parser_.call(
                        use_cbs,
                        first,
                        last,
                        context,
                        skip,
                        flags,
                        success,
                        attr);
                    if (!success) {
                        success = true;
                        first = prev_first;
                        break;
                    }
                    detail::move_back(
                        retval, std::move(attr), detail::gen_attrs(flags));
                }
            }
        }

        Parser parser_;
        DelimiterParser delimiter_parser_;
        MinType min_;
        MaxType max_;
    };



    template<typename Parser>
    struct zero_plus_parser : repeat_parser<Parser>
    {
        constexpr zero_plus_parser(Parser parser) :
            repeat_parser<Parser>(parser, 0, Inf)
        {}
    };



    template<typename Parser>
    struct one_plus_parser : repeat_parser<Parser>
    {
        constexpr one_plus_parser(Parser parser) :
            repeat_parser<Parser>(parser, 1, Inf)
        {}
    };




    template<typename Parser, typename DelimiterParser>
    struct delimited_seq_parser : repeat_parser<Parser, DelimiterParser>
    {
        constexpr delimited_seq_parser(
            Parser parser, DelimiterParser delimiter_parser) :
            repeat_parser<Parser, DelimiterParser>(
                parser, 1, Inf, delimiter_parser)
        {}
    };


    template<typename Parser>
    struct opt_parser
    {


        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser>
        auto call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success) const
        {
            using attr_t = decltype(parser_.call(
                use_cbs, first, last, context, skip, flags, success));
            detail::optional_of<attr_t> retval;
            call(use_cbs, first, last, context, skip, flags, success, retval);
            return retval;
        }



        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename Attribute>
        void call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success,
            Attribute & retval) const
        {

            auto _ = detail::scoped_trace(
                *this, first, last, context, flags, retval);



            detail::skip(first, last, skip, flags);



            if (!detail::gen_attrs(flags)) {
                parser_.call(
                    use_cbs, first, last, context, skip, flags, success);
                success = true;
                return;
            }



            parser_.call(
                use_cbs, first, last, context, skip, flags, success, retval);
            success = true;

        }



        Parser parser_;
    };


    template<typename ParserTuple>
    struct or_parser
    {
        constexpr or_parser(ParserTuple parsers) : parsers_(parsers) {}



        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser>
        struct use_parser_t
        {
            template<typename Parser>
            auto operator()(Parser const & parser) const
            {
                detail::skip(first_, last_, skip_, flags_);
                success_ = true;
                return parser.call(
                    std::bool_constant<UseCallbacks>{},
                    first_,
                    last_,
                    context_,
                    skip_,
                    flags_,
                    success_);
            }

            template<typename Parser, typename Attribute>
            void operator()(Parser const & parser, Attribute & retval) const
            {
                detail::skip(first_, last_, skip_, flags_);
                success_ = true;

                detail::apply_parser(
                    parser,
                    std::bool_constant<UseCallbacks>{},
                    first_,
                    last_,
                    context_,
                    skip_,
                    flags_,
                    success_,
                    retval);
            }

            Iter & first_;
            Sentinel last_;
            Context const & context_;
            SkipParser const & skip_;
            detail::flags flags_;
            bool & success_;
        };



        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser>
        auto call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success) const
        {
            use_parser_t<
                UseCallbacks,
                Iter,
                Sentinel,
                Context,
                SkipParser> const use_parser{
                first, last, context, skip, flags, success};


            using all_types =
                decltype(detail::hl::transform(parsers_, use_parser));


            using all_types_wrapped =
                decltype(detail::hl::transform(all_types{}, detail::wrap{}));
#line 3131 "boost/parser/parser.hpp"
            auto append_unique = [](auto result, auto x) {
                using x_type = typename decltype(x)::type;
                if constexpr (detail::is_nope_v<x_type>) {
                    return detail::hl::make_pair(
                        detail::hl::first(result), std::true_type{});
                } else if constexpr (detail::hl::contains(
                                         detail::hl::first(result), x)) {
                    return result;
                } else {
                    return detail::hl::make_pair(
                        detail::hl::append(detail::hl::first(result), x),
                        detail::hl::second(result));
                }
            };
            using wrapped_unique_types = decltype(detail::hl::fold_left(
                all_types_wrapped{},
                detail::hl::make_pair(tuple<>{}, std::false_type{}),
                append_unique));


            using unwrapped_types = decltype(detail::hl::make_pair(
                detail::hl::transform(
                    detail::hl::first(wrapped_unique_types{}),
                    detail::unwrap{}),
                detail::hl::second(wrapped_unique_types{})));
#line 3160 "boost/parser/parser.hpp"
            using result_t = detail::to_hana_tuple_or_type_t<unwrapped_types>;

            result_t retval;
            call(use_cbs, first, last, context, skip, flags, success, retval);
            return retval;
        }

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename Attribute>
        void call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success,
            Attribute & retval) const
        {
            auto _ = detail::scoped_trace(
                *this, first, last, context, flags, retval);

            use_parser_t<
                UseCallbacks,
                Iter,
                Sentinel,
                Context,
                SkipParser> const use_parser{
                first, last, context, skip, flags, success};

            bool done = false;
            auto try_parser = [prev_first = first,
                               use_parser,
                               &success,
                               flags,
                               &retval,
                               &done](auto const & parser) {
                if (done)
                    return;
                if (detail::gen_attrs(flags)) {
                    use_parser(parser, retval);
                    if (!success)
                        detail::assign(retval, Attribute());
                } else {
                    use_parser(parser);
                }
                if (success)
                    done = true;
                else
                    use_parser.first_ = prev_first;
            };
            detail::hl::for_each(parsers_, try_parser);

            if (!done)
                success = false;
        }



        template<typename Parser>
        constexpr auto prepend(parser_interface<Parser> parser) const noexcept;
        template<typename Parser>
        constexpr auto append(parser_interface<Parser> parser) const noexcept;



        ParserTuple parsers_;
    };

    namespace detail {
        template<int N, int... I>
        constexpr auto
        make_default_combining_impl(std::integer_sequence<int, I...>)
        {
            return hl::make_tuple(((void)I, llong<N>{})...);
        }
        template<template<class...> class Tuple, typename... Args>
        constexpr auto make_default_combining(Tuple<Args...>)
        {
            return detail::make_default_combining_impl<0>(
                std::make_integer_sequence<int, sizeof...(Args)>());
        }
        template<typename ParserTuple>
        using default_combining_t = decltype(detail::make_default_combining(
            std::declval<ParserTuple>()));

        struct default_combine_t
        {};
        struct merge_t
        {};
        struct separate_t
        {};

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser>
        struct dummy_use_parser_t
        {
            dummy_use_parser_t(
                Iter & first,
                Sentinel last,
                Context const & context,
                SkipParser const & skip,
                detail::flags flags,
                bool & success) :
                first_(first),
                last_(last),
                context_(context),
                skip_(skip),
                flags_(flags),
                success_(success)
            {}
            template<typename Parser>
            auto operator()(Parser const & parser) const
            {
                return parser.call(
                    std::bool_constant<UseCallbacks>{},
                    first_,
                    last_,
                    context_,
                    skip_,
                    flags_,
                    success_);
            }
            Iter & first_;
            Sentinel last_;
            Context const & context_;
            SkipParser const & skip_;
            detail::flags flags_;
            bool & success_;
        };

        template<typename... Args>
        constexpr void static_assert_merge_attributes(tuple<Args...> parsers);
#line 3306 "boost/parser/parser.hpp"
        template<typename CombiningGroups, typename... Args>
        constexpr auto make_combining(tuple<Args...> parsers)
        {
            if constexpr (std::is_same_v<CombiningGroups, default_combine_t>) {
                return detail::make_default_combining_impl<0>(
                    std::make_integer_sequence<int, sizeof...(Args)>());
            } else if constexpr (std::is_same_v<CombiningGroups, merge_t>) {
                detail::static_assert_merge_attributes(parsers);
                return detail::make_default_combining_impl<1>(
                    std::make_integer_sequence<int, sizeof...(Args)>());
            } else if constexpr (std::is_same_v<CombiningGroups, separate_t>) {
                return detail::make_default_combining_impl<-1>(
                    std::make_integer_sequence<int, sizeof...(Args)>());
            } else {
                return CombiningGroups{};
            }
        }
        template<typename ParserTuple, typename CombiningGroups>
        using combining_t = decltype(detail::make_combining<CombiningGroups>(
            std::declval<ParserTuple>()));

        struct max_
        {
            template<typename T, typename U>
            constexpr auto operator()(T x, U y) const
            {
                if constexpr (T::value < U::value)
                    return y;
                else
                    return x;
            }
        };
        template<int MaxGroupIdx>
        struct adjust_combining_groups
        {
            template<typename T, typename U>
            constexpr auto operator()(T result, U x) const
            {
                if constexpr (U::value <= 0)
                    return hl::append(result, x);
                else
                    return hl::append(result, llong<MaxGroupIdx + U::value>{});
            }
        };
        template<typename Tuple1, typename Tuple2>
        constexpr auto make_combined_combining(Tuple1 lhs, Tuple2 rhs)
        {
            auto max_group_idx = detail::hl::fold_left(lhs, llong<0>{}, max_{});
            auto rhs_adjusted = detail::hl::fold_left(
                rhs,
                tuple<>{},
                adjust_combining_groups<decltype(max_group_idx)::value>{});
            return hl::concat(lhs, rhs_adjusted);
        }
        template<typename CombiningGroups1, typename CombiningGroups2>
        using combined_combining_t = decltype(detail::make_combined_combining(
            std::declval<CombiningGroups1>(),
            std::declval<CombiningGroups2>()));

        enum class merge_kind { second_pass_detect, singleton, merged, group };

        template<merge_kind Kind>
        struct merge_kind_t
        {
            static constexpr merge_kind kind = Kind;
        };

        template<merge_kind Kind>
        static constexpr auto merge_wrap = merge_kind_t<Kind>{};
    }

    template<
        typename ParserTuple,
        typename BacktrackingTuple,
        typename CombiningGroups>
    struct seq_parser
    {
        using backtracking = BacktrackingTuple;
        using combining_groups = CombiningGroups;

        constexpr seq_parser(ParserTuple parsers) : parsers_(parsers) {}



        static constexpr auto true_ = std::true_type{};
        static constexpr auto false_ = std::false_type{};

        struct combine
        {
            template<typename T, typename U>
            auto operator()(
                T result_merging_indices_and_prev_group, U x_and_group) const
            {
                using namespace literals;
                using detail::merge_wrap;
                using detail::merge_kind;

                auto x = parser::get(x_and_group, 0_c);
                auto group = parser::get(x_and_group, 1_c);

                auto result =
                    parser::get(result_merging_indices_and_prev_group, 0_c);
                using result_back_type =
                    typename std::decay_t<decltype(detail::hl::back(
                        result))>::type;
                using unwrapped_optional_result_back_type =
                    detail::unwrapped_optional_t<result_back_type>;

                auto merging =
                    parser::get(result_merging_indices_and_prev_group, 1_c);
                auto indices =
                    parser::get(result_merging_indices_and_prev_group, 2_c);
                auto prev_group =
                    parser::get(result_merging_indices_and_prev_group, 3_c);

                using x_type = typename decltype(x)::type;
                using unwrapped_optional_x_type =
                    detail::unwrapped_optional_t<x_type>;

                if constexpr (detail::is_nope_v<x_type>) {
                    if constexpr (
                        !detail::is_nope_v<result_back_type> &&
                        0 < decltype(group)::value &&
                        decltype(group)::value != decltype(prev_group)::value) {
#line 3436 "boost/parser/parser.hpp"
                        return detail::hl::make_tuple(
                            detail::hl::append(result, x),
                            detail::hl::append(
                                merging,
                                merge_wrap<merge_kind::second_pass_detect>),
                            detail::hl::append(
                                indices, detail::hl::size(result)),
                            group);
                    } else {

                        return detail::hl::make_tuple(
                            result,
                            detail::hl::append(
                                merging,
                                merge_wrap<merge_kind::second_pass_detect>),
                            detail::hl::append(
                                indices, detail::hl::size_minus_one(result)),
                            prev_group);
                    }
                } else if constexpr (detail::is_nope_v<result_back_type>) {

                    constexpr auto merge =
                        0 < decltype(group)::value
                            ? merge_kind::group
                            : (decltype(group)::value == -1
                                   ? merge_kind::singleton
                                   : merge_kind::second_pass_detect);
                    return detail::hl::make_tuple(
                        detail::hl::append(detail::hl::drop_back(result), x),
                        detail::hl::append(merging, merge_wrap<merge>),
                        detail::hl::append(
                            indices, detail::hl::size_minus_one(result)),
                        group);
                } else if constexpr (0 < decltype(group)::value) {
                    if constexpr (
                        decltype(prev_group)::value == decltype(group)::value) {
                        return detail::hl::make_tuple(
                            result,
                            detail::hl::append(
                                merging, merge_wrap<merge_kind::group>),
                            detail::hl::append(
                                indices, detail::hl::size_minus_one(result)),
                            group);
                    } else {
                        return detail::hl::make_tuple(
                            detail::hl::append(result, x),
                            detail::hl::append(
                                merging, merge_wrap<merge_kind::group>),
                            detail::hl::append(
                                indices, detail::hl::size(result)),
                            group);
                    }
                } else if constexpr (
                    decltype(group)::value == -1 ||
                    decltype(group)::value != decltype(prev_group)::value) {
                    constexpr auto merge = decltype(group)::value == -1
                                               ? merge_kind::singleton
                                               : merge_kind::second_pass_detect;
                    return detail::hl::make_tuple(
                        detail::hl::append(result, x),
                        detail::hl::append(merging, merge_wrap<merge>),
                        detail::hl::append(indices, detail::hl::size(result)),
                        group);
                } else if constexpr (
                    detail::is_char_type_v<result_back_type> &&
                    (detail::is_char_type_v<x_type> ||
                     detail::is_char_type_v<unwrapped_optional_x_type>)) {

                    return detail::hl::make_tuple(
                        detail::hl::append(
                            detail::hl::drop_back(result),
                            detail::wrapper<std::string>{}),
                        detail::hl::append(
                            detail::hl::append(
                                detail::hl::drop_front(merging),
                                merge_wrap<merge_kind::second_pass_detect>),
                            merge_wrap<merge_kind::second_pass_detect>),
                        detail::hl::append(
                            indices, detail::hl::size_minus_one(result)),
                        group);
                } else if constexpr (
                    detail::
                        container_and_value_type<result_back_type, x_type> ||
                    detail::container_and_value_type<
                        result_back_type,
                        unwrapped_optional_x_type>) {


                    return detail::hl::make_tuple(
                        result,
                        detail::hl::append(
                            merging,
                            merge_wrap<merge_kind::second_pass_detect>),
                        detail::hl::append(
                            indices, detail::hl::size_minus_one(result)),
                        group);
                } else if constexpr (
                    detail::
                        container_and_value_type<x_type, result_back_type> ||
                    detail::container_and_value_type<
                        x_type,
                        unwrapped_optional_result_back_type>) {


                    return detail::hl::make_tuple(
                        detail::hl::append(detail::hl::drop_back(result), x),
                        detail::hl::append(
                            detail::hl::append(
                                detail::hl::drop_front(merging),
                                merge_wrap<merge_kind::second_pass_detect>),
                            merge_wrap<merge_kind::second_pass_detect>),
                        detail::hl::append(
                            indices, detail::hl::size_minus_one(result)),
                        group);
                } else {

                    return detail::hl::make_tuple(
                        detail::hl::append(result, x),
                        detail::hl::append(
                            merging, merge_wrap<merge_kind::second_pass_detect>),
                        detail::hl::append(indices, detail::hl::size(result)),
                        group);
                }
            }
        };

        struct find_merged
        {
            template<typename T, typename U>
            auto operator()(
                T final_types_and_result, U x_index_and_prev_merged) const
            {
                using namespace literals;
                using detail::merge_wrap;
                using detail::merge_kind;

                auto final_types = parser::get(final_types_and_result, 0_c);
                auto result = parser::get(final_types_and_result, 1_c);

                auto x_type_wrapper = parser::get(x_index_and_prev_merged, 0_c);
                auto index = parser::get(x_index_and_prev_merged, 1_c);
                auto prev_merged = parser::get(x_index_and_prev_merged, 2_c);

                auto type_at_index_wrapper = parser::get(final_types, index);
                using x_type = typename decltype(x_type_wrapper)::type;
                using type_at_index =
                    typename decltype(type_at_index_wrapper)::type;
                if constexpr (
                    decltype(prev_merged)::kind ==
                    merge_kind::second_pass_detect) {
                    if constexpr (
                        !std::is_same_v<x_type, type_at_index> &&
                        container<type_at_index>) {
                        return detail::hl::make_tuple(
                            final_types,
                            detail::hl::append(
                                result, merge_wrap<merge_kind::merged>));
                    } else {
                        return detail::hl::make_tuple(
                            final_types,
                            detail::hl::append(
                                result, merge_wrap<merge_kind::singleton>));
                    }
                } else {
                    return detail::hl::make_tuple(
                        final_types, detail::hl::append(result, prev_merged));
                }
            }
        };

        template<long long I>
        static constexpr auto
        merging_from_group(integral_constant<long long, I>)
        {
            using detail::merge_wrap;
            using detail::merge_kind;
            if constexpr (0 < I)
                return merge_wrap<merge_kind::group>;
            else if constexpr (I == -1)
                return merge_wrap<merge_kind::singleton>;
            else
                return merge_wrap<merge_kind::second_pass_detect>;
        }
#line 3623 "boost/parser/parser.hpp"
        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser>
        auto make_temp_result(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success) const
        {
            using namespace literals;

            detail::dummy_use_parser_t<
                UseCallbacks,
                Iter,
                Sentinel,
                Context,
                SkipParser> const
                dummy_use_parser(first, last, context, skip, flags, success);


            using all_types =
                decltype(detail::hl::transform(parsers_, dummy_use_parser));


            using all_types_wrapped =
                decltype(detail::hl::transform(all_types{}, detail::wrap{}));

            using combining_groups =
                detail::combining_t<ParserTuple, CombiningGroups>;
            constexpr auto first_group = detail::hl::front(combining_groups{});
#line 3663 "boost/parser/parser.hpp"
            constexpr auto combine_start = detail::hl::make_tuple(
                detail::hl::make_tuple(detail::hl::front(all_types_wrapped{})),
                detail::hl::make_tuple(merging_from_group(first_group)),
                tuple<llong<0>>{},
                first_group);
            using combined_types = decltype(detail::hl::fold_left(
                detail::hl::zip(
                    detail::hl::drop_front(all_types_wrapped{}),
                    detail::hl::drop_front(combining_groups{})),
                combine_start,
                combine{}));


            constexpr auto result_type_wrapped =
                parser::get(combined_types{}, 0_c);
            using result_type = decltype(detail::hl::transform(
                result_type_wrapped, detail::unwrap{}));

            using indices = decltype(parser::get(combined_types{}, 2_c));
            using first_pass_merged =
                decltype(parser::get(combined_types{}, 1_c));

            constexpr auto find_merged_start =
                detail::hl::make_tuple(result_type_wrapped, tuple<>{});
            using merged = decltype(detail::hl::fold_left(
                detail::hl::zip(
                    all_types_wrapped{}, indices{}, first_pass_merged{}),
                find_merged_start,
                find_merged{}));

            return detail::hl::make_tuple(
                result_type{}, indices{}, parser::get(merged{}, 1_c));
        }



        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser>
        auto call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first_,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success) const
        {
            Iter first = first_;

            auto temp_result = make_temp_result(
                use_cbs, first, last, context, skip, flags, success);

            std::decay_t<decltype(parser::get(temp_result, llong<0>{}))> retval;

            auto _ = detail::scoped_trace(
                *this,
                first_,
                last,
                context,
                detail::in_apply_parser(flags) ? detail::disable_trace(flags)
                                               : flags,
                retval);

            std::decay_t<decltype(parser::get(temp_result, llong<1>{}))>
                indices;
            std::decay_t<decltype(parser::get(temp_result, llong<2>{}))>
                merged;
            call_impl(
                use_cbs,
                first,
                last,
                context,
                skip,
                flags,
                success,
                retval,
                indices,
                merged);

            if (success)
                first_ = first;


            if constexpr (detail::hl::size(retval) == llong<1>{}) {
                using namespace literals;
                return parser::get(retval, 0_c);
            } else {
                return retval;
            }
        }

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename Attribute>
        void call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first_,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success,
            Attribute & retval) const
        {
            auto _ = detail::scoped_trace(
                *this,
                first_,
                last,
                context,
                detail::in_apply_parser(flags) ? detail::disable_trace(flags)
                                               : flags,
                retval);

            Iter first = first_;

            auto temp_result = make_temp_result(
                use_cbs, first, last, context, skip, flags, success);
            using temp_result_attr_t =
                std::decay_t<decltype(parser::get(temp_result, llong<0>{}))>;
            std::decay_t<decltype(parser::get(temp_result, llong<1>{}))>
                indices;
            std::decay_t<decltype(parser::get(temp_result, llong<2>{}))>
                merged;

            auto max_ = [](auto result, auto x) {
                if constexpr (decltype(result)::value < decltype(x)::value) {
                    return x;
                } else {
                    return result;
                }
            };
            using max_index_t =
                decltype(detail::hl::fold_left(indices, llong<0>{}, max_));

            if constexpr (detail::is_optional_v<Attribute>) {
                typename Attribute::value_type attr;
                call(use_cbs, first, last, context, skip, flags, success, attr);
                if (success)
                    detail::assign(retval, std::move(attr));
            } else if constexpr (
                detail::is_tuple<Attribute>{} ||
                detail::is_struct_compatible_v<Attribute, temp_result_attr_t>) {
                call_impl(
                    use_cbs,
                    first,
                    last,
                    context,
                    skip,
                    flags,
                    success,
                    retval,
                    indices,
                    merged);

                if (!success || !detail::gen_attrs(flags))
                    detail::assign(retval, Attribute());
            } else if constexpr (
                0 < max_index_t::value && detail::is_constructible_from_tuple_v<
                                              Attribute,
                                              temp_result_attr_t>) {
                temp_result_attr_t temp_retval;
                call_impl(
                    use_cbs,
                    first,
                    last,
                    context,
                    skip,
                    flags,
                    success,
                    temp_retval,
                    indices,
                    merged);

                if (success && detail::gen_attrs(flags)) {
                    detail::assign(
                        retval,
                        detail::make_from_tuple<Attribute>(
                            std::move(temp_retval)));
                }
            } else {

                tuple<Attribute> temp_retval;
                call_impl(
                    use_cbs,
                    first,
                    last,
                    context,
                    skip,
                    flags,
                    success,
                    temp_retval,
                    indices,
                    merged);

                if (success && detail::gen_attrs(flags)) {
                    detail::assign(
                        retval, std::move(detail::hl::front(temp_retval)));
                }
            }

            if (success)
                first_ = first;
        }
#line 3880 "boost/parser/parser.hpp"
        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename Attribute,
            typename Indices,
            typename Merged>
        void call_impl(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success,
            Attribute & retval,
            Indices const & indices,
            Merged const & merged) const
        {
            using detail::merge_wrap;
            using detail::merge_kind;

            static_assert(
                detail::is_tuple<Attribute>{} || std::is_aggregate_v<Attribute>,
                "");

            auto use_parser = [use_cbs,
                               &first,
                               last,
                               &context,
                               &skip,
                               flags,
                               &success,
                               &retval](auto const &
                                            parser_index_merged_and_backtrack) {
                using namespace literals;
                detail::skip(first, last, skip, flags);
                if (!success)
                    return;

                auto const & parser =
                    parser::get(parser_index_merged_and_backtrack, 0_c);
                auto merge_kind_t_ =
                    parser::get(parser_index_merged_and_backtrack, 2_c);
                constexpr bool was_merged_into_adjacent_container =
                    decltype(merge_kind_t_)::kind == merge_kind::merged;
                constexpr bool is_in_a_group =
                    decltype(merge_kind_t_)::kind == merge_kind::group;
                bool const can_backtrack =
                    parser::get(parser_index_merged_and_backtrack, 3_c);

                if (!detail::gen_attrs(flags)) {
                    parser.call(
                        use_cbs, first, last, context, skip, flags, success);
                    if (!success && !can_backtrack) {
                        std::stringstream oss;
                        detail::print_parser(context, parser, oss);
                        throw parse_error<Iter>(first, oss.str());
                    }
                    return;
                }

                auto const tuple_idx =
                    parser::get(parser_index_merged_and_backtrack, 1_c);
                auto const tuple_size = detail::tuple_or_struct_size(retval);
                static_assert(
                    decltype(tuple_idx)::value < decltype(tuple_size)::value,
                    "Looks like you're trying to write some attribute into an "
                    "out-of-bounds position in a tuple/struct.  In other "
                    "words, the attribute you're parsing into does not match "
                    "the default attribute used by this parser.  This may be "
                    "because you passed an out-param to parse() at the top "
                    "level that is not compatible with the attribute type "
                    "generated by the parser you passed to parse().");
                if constexpr (!(decltype(tuple_idx)::value <
                                decltype(tuple_size)::value)) {
                    [[maybe_unused]] detail::print_type<Attribute> _;
                }
                auto & out = parser::get(retval, tuple_idx);

                using attr_t = decltype(parser.call(
                    use_cbs, first, last, context, skip, flags, success));
                constexpr bool out_container =
                    container<std::decay_t<decltype(out)>>;
                constexpr bool attr_container = container<attr_t>;

                if constexpr (
                    (out_container == attr_container &&
                     !was_merged_into_adjacent_container) ||
                    is_in_a_group) {
                    parser.call(
                        use_cbs,
                        first,
                        last,
                        context,
                        skip,
                        flags,
                        success,
                        out);
                    if (!success) {
                        if (!can_backtrack) {
                            std::stringstream oss;
                            detail::print_parser(context, parser, oss);
                            throw parse_error<Iter>(first, oss.str());
                        }
                        out = std::decay_t<decltype(out)>();
                        return;
                    }
                } else {
                    attr_t x = parser.call(
                        use_cbs, first, last, context, skip, flags, success);
                    if (!success) {
                        if (!can_backtrack) {
                            std::stringstream oss;
                            detail::print_parser(context, parser, oss);
                            throw parse_error<Iter>(first, oss.str());
                        }
                        return;
                    }
                    if constexpr (out_container) {
                        detail::move_back(
                            out, std::move(x), detail::gen_attrs(flags));
                    } else {
                        detail::assign(out, std::move(x));
                    }
                }
            };

            auto const parsers_and_indices =
                detail::hl::zip(parsers_, indices, merged, backtracking{});
            detail::hl::for_each(parsers_and_indices, use_parser);
        }

        template<bool AllowBacktracking, typename Parser>
        constexpr auto prepend(parser_interface<Parser> parser) const noexcept;
        template<bool AllowBacktracking, typename Parser>
        constexpr auto append(parser_interface<Parser> parser) const noexcept;



        ParserTuple parsers_;
    };

    template<typename Parser, typename Action>
    struct action_parser
    {
        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser>
        detail::nope call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success) const
        {
            detail::nope retval;
            call(use_cbs, first, last, context, skip, flags, success, retval);
            return retval;
        }

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename Attribute>
        void call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success,
            Attribute & retval) const
        {
            auto _ = detail::scoped_trace(
                *this, first, last, context, flags, retval);

            auto const initial_first = first;
            auto attr = parser_.call(
                use_cbs,
                first,
                last,
                context,
                skip,
                detail::enable_attrs(flags),
                success);
            boost::parser::subrange const where(initial_first, first);
            if (success) {
                auto const action_context =
                    detail::make_action_context(context, attr, where);
                action_(action_context);
            }
        }

        Parser parser_;
        Action action_;
    };

    template<typename Parser>
    struct omit_parser
    {
        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser>
        detail::nope call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success) const
        {
            auto _ = detail::scoped_trace(
                *this, first, last, context, flags, detail::global_nope);

            parser_.call(
                use_cbs,
                first,
                last,
                context,
                skip,
                detail::disable_attrs(flags),
                success);
            return {};
        }

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename Attribute>
        void call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success,
            Attribute & retval) const
        {
            auto _ = detail::scoped_trace(
                *this, first, last, context, flags, retval);

            parser_.call(
                use_cbs,
                first,
                last,
                context,
                skip,
                detail::disable_attrs(flags),
                success);
        }

        Parser parser_;
    };

    template<typename Parser>
    struct raw_parser
    {
        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser>
        boost::parser::subrange<Iter> call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success) const
        {
            boost::parser::subrange<Iter> retval;
            call(use_cbs, first, last, context, skip, flags, success, retval);
            return retval;
        }

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename Attribute>
        void call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success,
            Attribute & retval) const
        {
            auto _ = detail::scoped_trace(
                *this, first, last, context, flags, retval);

            auto const initial_first = first;
            parser_.call(
                use_cbs,
                first,
                last,
                context,
                skip,
                detail::disable_attrs(flags),
                success);
            if (success && detail::gen_attrs(flags))
                detail::assign(
                    retval, boost::parser::subrange<Iter>(initial_first, first));
        }

        Parser parser_;
    };
#line 4303 "boost/parser/parser.hpp"
    template<typename Parser>
    struct lexeme_parser
    {
        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser>
        auto call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success) const
        {
            using attr_t = decltype(parser_.call(
                use_cbs, first, last, context, skip, flags, success));
            attr_t retval{};
            call(use_cbs, first, last, context, skip, flags, success, retval);
            return retval;
        }

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename Attribute>
        void call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success,
            Attribute & retval) const
        {
            auto _ = detail::scoped_trace(
                *this, first, last, context, flags, retval);

            parser_.call(
                use_cbs,
                first,
                last,
                context,
                skip,
                detail::disable_skip(flags),
                success,
                retval);
        }

        Parser parser_;
    };

    template<typename Parser>
    struct no_case_parser
    {
        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser>
        auto call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context_,
            SkipParser const & skip,
            detail::flags flags,
            bool & success) const
        {
            auto context = context_;
            ++context.no_case_depth_;

            using attr_t = decltype(parser_.call(
                use_cbs, first, last, context, skip, flags, success));
            attr_t retval{};
            call(use_cbs, first, last, context, skip, flags, success, retval);
            return retval;
        }

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename Attribute>
        void call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context_,
            SkipParser const & skip,
            detail::flags flags,
            bool & success,
            Attribute & retval) const
        {
            auto context = context_;
            ++context.no_case_depth_;

            auto _ = detail::scoped_trace(
                *this, first, last, context, flags, retval);

            parser_.call(
                use_cbs, first, last, context, skip, flags, success, retval);
        }

        Parser parser_;
    };

    template<typename Parser, typename SkipParser>
    struct skip_parser
    {
        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser_>
        auto call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser_ const & skip,
            detail::flags flags,
            bool & success) const
        {
            using attr_t = decltype(parser_.call(
                use_cbs, first, last, context, skip, flags, success));
            attr_t retval{};
            call(use_cbs, first, last, context, skip, flags, success, retval);
            return retval;
        }

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser_,
            typename Attribute>
        void call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser_ const & skip,
            detail::flags flags,
            bool & success,
            Attribute & retval) const
        {
            auto _ = detail::scoped_trace(
                *this, first, last, context, flags, retval);

            if constexpr (detail::is_nope_v<SkipParser>) {
                parser_.call(
                    use_cbs,
                    first,
                    last,
                    context,
                    skip,
                    detail::enable_skip(flags),
                    success,
                    retval);
            } else {
                parser_.call(
                    use_cbs,
                    first,
                    last,
                    context,
                    skip_parser_,
                    detail::enable_skip(flags),
                    success,
                    retval);
            }
        }

        Parser parser_;
        SkipParser skip_parser_;
    };

    template<typename Parser, bool FailOnMatch>
    struct expect_parser
    {
        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser>
        detail::nope call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success) const
        {
            detail::nope retval;
            call(use_cbs, first, last, context, skip, flags, success, retval);
            return retval;
        }

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename Attribute>
        void call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success,
            Attribute & retval) const
        {
            auto _ = detail::scoped_trace(
                *this, first, last, context, flags, retval);

            auto first_copy = first;
            parser_.call(
                use_cbs,
                first_copy,
                last,
                context,
                skip,
                detail::disable_attrs(flags),
                success);
            if (FailOnMatch)
                success = !success;
        }

        Parser parser_;
    };

    template<typename T>
    struct symbol_parser
    {
        symbol_parser() : copied_from_(nullptr) {}
        symbol_parser(symbol_parser const & other) :
            initial_elements_(other.initial_elements_),
            copied_from_(other.copied_from_ ? other.copied_from_ : &other)
        {}





        template<typename Context>
        parser::detail::text::optional_ref<T>
        find(Context const & context, std::string_view str) const
        {
            parser::detail::text::trie<std::vector<char32_t>, T> & trie_ =
                detail::get_trie(context, ref());
            return trie_[str | detail::text::as_utf32];
        }




        template<typename Context>
        void insert(Context const & context, std::string_view str, T && x) const
        {
            parser::detail::text::trie<std::vector<char32_t>, T> & trie_ =
                detail::get_trie(context, ref());
            trie_.insert(str | detail::text::as_utf32, std::move(x));
        }



        template<typename Context>
        void erase(Context const & context, std::string_view str) const
        {
            parser::detail::text::trie<std::vector<char32_t>, T> & trie_ =
                detail::get_trie(context, ref());
            trie_.erase(str | detail::text::as_utf32);
        }

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser>
        T call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success) const
        {
            T retval;
            call(use_cbs, first, last, context, skip, flags, success, retval);
            return retval;
        }

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename Attribute>
        void call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success,
            Attribute & retval) const
        {
            auto _ = detail::scoped_trace(
                *this, first, last, context, flags, retval);

            parser::detail::text::trie<std::vector<char32_t>, T> const & trie_ =
                detail::get_trie(context, ref());
            auto const lookup = trie_.longest_match(first, last);
            if (lookup.match) {
                std::advance(first, lookup.size);
                detail::assign(retval, T{*trie_[lookup]});
            } else {
                success = false;
            }
        }

        std::vector<std::pair<std::string_view, T>> initial_elements_;
        symbol_parser const * copied_from_;

        symbol_parser const & ref() const noexcept
        {
            if (copied_from_)
                return *copied_from_;
            return *this;
        }
        std::vector<std::pair<std::string_view, T>> const &
        initial_elements() const noexcept
        {
            return ref().initial_elements_;
        }
    };

    template<
        bool CanUseCallbacks,
        typename TagType,
        typename Attribute,
        typename LocalState,
        typename ParamsTuple>
    struct rule_parser
    {
        using tag_type = TagType;
        using attr_type = Attribute;
        using locals_type = LocalState;

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser>
        auto call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success) const
        {
            constexpr bool recursive_rule =
                std::is_same_v<typename Context::rule_tag, tag_type> &&
                !std::is_same_v<typename Context::rule_tag, void>;

            if constexpr (recursive_rule)
                flags = detail::disable_attrs(flags);

            attr_type retval{};
            locals_type locals = detail::make_locals<locals_type>(context);
            auto params = detail::resolve_rule_params(context, params_);
            tag_type * const tag_ptr = nullptr;
            auto const rule_context = detail::make_rule_context(
                context, tag_ptr, retval, locals, params);
            auto _ = detail::scoped_trace(
                *this, first, last, rule_context, flags, retval);

            parse_rule(
                tag_ptr,
                use_cbs,
                first,
                last,
                rule_context,
                skip,
                flags,
                success,
                retval);

            if constexpr (CanUseCallbacks && UseCallbacks && !recursive_rule) {
                if (!success)
                    return attr_type{};

                auto const & callbacks = _callbacks(context);

                if constexpr (detail::is_nope_v<attr_type>) {
                    static_assert(
                        detail::is_invocable_v<decltype(callbacks), tag_type>,
                        "For rules without attributes, Callbacks must be a "
                        "struct with overloads of the form void(tag_type).  If "
                        "you're seeing an error here, you probably have not "
                        "met this contract.");
                    callbacks(tag_type{});
                } else {
                    static_assert(
                        detail::is_invocable_v<
                            decltype(callbacks),
                            tag_type,
                            decltype(std::move(retval))>,
                        "For rules with attributes, Callbacks must be a struct "
                        "with overloads of the form void(tag_type, attr_type). "
                        "If you're seeing an error here, you probably have not "
                        "met this contract.");
                    callbacks(tag_type{}, std::move(retval));
                }

                return attr_type{};
            } else {
                if (!success && !recursive_rule)
                    detail::assign(retval, attr_type());
                if constexpr (recursive_rule)
                    return detail::nope{};
                else
                    return retval;
            }
        }

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename Attribute_>
        void call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success,
            Attribute_ & retval) const
        {
            if constexpr (CanUseCallbacks && UseCallbacks) {
                call(use_cbs, first, last, context, skip, flags, success);
            } else {
                auto attr =
                    call(use_cbs, first, last, context, skip, flags, success);
                if (success)
                    detail::assign(retval, std::move(attr));
            }
        }

        std::string_view diagnostic_text_;
        ParamsTuple params_;
    };




    template<typename Parser, typename GlobalState, typename ErrorHandler>
    struct parser_interface
    {
        using parser_type = Parser;
        using global_state_type = GlobalState;
        using error_handler_type = ErrorHandler;

        constexpr parser_interface() {}
        constexpr parser_interface(parser_type p) : parser_(p) {}
        constexpr parser_interface(
            parser_type p, global_state_type gs, error_handler_type eh) :
            parser_(p), globals_(gs), error_handler_(eh)
        {}




        constexpr auto operator!() const noexcept
        {
            return parser::parser_interface{
                expect_parser<parser_type, true>{parser_}};
        }




        constexpr auto operator&() const noexcept
        {
            return parser::parser_interface{
                expect_parser<parser_type, false>{parser_}};
        }



        constexpr auto operator*() const noexcept
        {
            if constexpr (detail::is_zero_plus_p<parser_type>{}) {
                return *this;
            } else if constexpr (detail::is_one_plus_p<parser_type>{}) {
                using inner_parser = decltype(parser_type::parser_);
                return parser::parser_interface{
                    zero_plus_parser<inner_parser>(parser_.parser_)};
            } else {
                return parser::parser_interface{
                    zero_plus_parser<parser_type>(parser_)};
            }
        }



        constexpr auto operator+() const noexcept
        {
            if constexpr (detail::is_zero_plus_p<parser_type>{}) {
                using inner_parser = decltype(parser_type::parser_);
                return parser::parser_interface{
                    zero_plus_parser<inner_parser>(parser_.parser_)};
            } else if constexpr (detail::is_one_plus_p<parser_type>{}) {
                return *this;
            } else {
                return parser::parser_interface{
                    one_plus_parser<parser_type>(parser_)};
            }
        }



        constexpr auto operator-() const noexcept
        {
            return parser::parser_interface{opt_parser<parser_type>{parser_}};
        }



        template<typename ParserType2>
        constexpr auto
        operator>>(parser_interface<ParserType2> rhs) const noexcept
        {
            if constexpr (detail::is_seq_p<parser_type>{}) {
                return parser_.template append<true>(rhs);
            } else if constexpr (detail::is_seq_p<ParserType2>{}) {
                return rhs.parser_.template prepend<true>(*this);
            } else {
                using parser_t = seq_parser<
                    tuple<parser_type, ParserType2>,
                    tuple<std::true_type, std::true_type>,
                    tuple<llong<0>, llong<0>>>;
                return parser::parser_interface{parser_t{
                    tuple<parser_type, ParserType2>{parser_, rhs.parser_}}};
            }
        }



        constexpr auto operator>>(char rhs) const noexcept;



        constexpr auto operator>>(char32_t rhs) const noexcept;






        template<
            typename R,
            typename Enable =
                std::enable_if_t<detail::is_parsable_range_like_v<R>>>

        constexpr auto operator>>(R && r) const noexcept;






        template<typename ParserType2>
        constexpr auto
        operator>(parser_interface<ParserType2> rhs) const noexcept
        {
            if constexpr (detail::is_seq_p<parser_type>{}) {
                return parser_.template append<false>(rhs);
            } else if constexpr (detail::is_seq_p<ParserType2>{}) {
                return rhs.parser_.template prepend<false>(*this);
            } else {
                using parser_t = seq_parser<
                    tuple<parser_type, ParserType2>,
                    tuple<std::true_type, std::false_type>,
                    tuple<llong<0>, llong<0>>>;
                return parser::parser_interface{parser_t{
                    tuple<parser_type, ParserType2>{parser_, rhs.parser_}}};
            }
        }





        constexpr auto operator>(char rhs) const noexcept;





        constexpr auto operator>(char32_t rhs) const noexcept;
#line 4940 "boost/parser/parser.hpp"
        template<
            typename R,
            typename Enable =
                std::enable_if_t<detail::is_parsable_range_like_v<R>>>

        constexpr auto operator>(R && r) const noexcept;



        template<typename ParserType2>
        constexpr auto
        operator|(parser_interface<ParserType2> rhs) const noexcept
        {
            if constexpr (detail::is_or_p<parser_type>{}) {
                return parser_.append(rhs);
            } else if constexpr (detail::is_or_p<ParserType2>{}) {
                return rhs.parser_.prepend(*this);
            } else {
#line 4967 "boost/parser/parser.hpp"
                BOOST_ASSERT(!detail::is_unconditional_eps<parser_type>{});

                return parser::parser_interface{
                    or_parser<tuple<parser_type, ParserType2>>{
                        tuple<parser_type, ParserType2>{parser_, rhs.parser_}}};
            }
        }



        constexpr auto operator|(char rhs) const noexcept;



        constexpr auto operator|(char32_t rhs) const noexcept;






        template<
            typename R,
            typename Enable =
                std::enable_if_t<detail::is_parsable_range_like_v<R>>>

        constexpr auto operator|(R && r) const noexcept;



        template<typename ParserType2>
        constexpr auto
        operator-(parser_interface<ParserType2> rhs) const noexcept
        {
            return !rhs >> *this;
        }



        constexpr auto operator-(char rhs) const noexcept;



        constexpr auto operator-(char32_t rhs) const noexcept;






        template<
            typename R,
            typename Enable =
                std::enable_if_t<detail::is_parsable_range_like_v<R>>>

        constexpr auto operator-(R && r) const noexcept;



        template<typename ParserType2>
        constexpr auto
        operator%(parser_interface<ParserType2> rhs) const noexcept
        {
            return parser::parser_interface{
                delimited_seq_parser<parser_type, ParserType2>(
                    parser_, rhs.parser_)};
        }



        constexpr auto operator%(char rhs) const noexcept;



        constexpr auto operator%(char32_t rhs) const noexcept;






        template<
            typename R,
            typename Enable =
                std::enable_if_t<detail::is_parsable_range_like_v<R>>>

        constexpr auto operator%(R && r) const noexcept;




        template<typename Action>
        constexpr auto operator[](Action action) const
        {
            using action_parser_t = action_parser<parser_type, Action>;
            return parser::parser_interface{action_parser_t{parser_, action}};
        }
#line 5072 "boost/parser/parser.hpp"
        template<typename Arg, typename... Args>
        constexpr auto operator()(Arg && arg, Args &&... args) const noexcept
            -> decltype(std::declval<parser_type const &>()(
                (Arg &&) arg, (Args &&) args...))
        {
            return parser_((Arg &&) arg, (Args &&) args...);
        }





        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParserType>
        auto operator()(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParserType const & skip,
            detail::flags flags,
            bool & success) const
        {
            return parser_.call(
                use_cbs, first, last, context, skip, flags, success);
        }



        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParserType,
            typename Attribute>
        void operator()(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParserType const & skip,
            detail::flags flags,
            bool & success,
            Attribute & attr) const
        {
            parser_.call(
                use_cbs, first, last, context, skip, flags, success, attr);
        }

        parser_type parser_;
        global_state_type globals_;
        error_handler_type error_handler_;


    };





    template<typename Parser, typename GlobalState, typename ErrorHandler>
    auto with_globals(
        parser_interface<Parser, detail::nope, ErrorHandler> const & parser,
        GlobalState & globals)
    {
        return parser_interface<Parser, GlobalState &, ErrorHandler>{
            parser.parser_, globals, parser.error_handler_};
    }




    template<typename Parser, typename GlobalState, typename ErrorHandler>
    auto with_error_handler(
        parser_interface<Parser, GlobalState, default_error_handler> const &
            parser,
        ErrorHandler & error_handler)
    {
        return parser_interface<Parser, GlobalState, ErrorHandler &>{
            parser.parser_, parser.globals_, error_handler};
    }
#line 5167 "boost/parser/parser.hpp"
    template<typename T>
    struct symbols : parser_interface<symbol_parser<T>>
    {
        symbols() {}
        symbols(std::initializer_list<std::pair<std::string_view, T>> il)
        {
            this->parser_.initial_elements_ = il;
        }

        using parser_interface<symbol_parser<T>>::operator();





        symbols & insert_for_next_parse(std::string_view str, T x)
        {
            this->parser_.initial_elements_.push_back(
                std::pair<std::string_view, T>(str, std::move(x)));
            return *this;
        }


        symbols & operator()(std::string_view str, T x)
        {
            return insert_for_next_parse(str, std::move(x));
        }





        template<typename Context>
        parser::detail::text::optional_ref<T>
        find(Context const & context, std::string_view str) const
        {
            return this->parser_.find(context, str);
        }




        template<typename Context>
        void insert(Context const & context, std::string_view str, T x) const
        {
            this->parser_.insert(context, str, std::move(x));
        }



        template<typename Context>
        void erase(Context const & context, std::string_view str) const
        {
            this->parser_.erase(context, str);
        }
    };

    template<
        typename TagType,
        typename Attribute,
        typename LocalState,
        typename ParamsTuple>
    struct rule
        : parser_interface<
              rule_parser<false, TagType, Attribute, LocalState, ParamsTuple>>
    {
        static_assert(
            !std::is_same_v<TagType, void>,
            "void is not a valid tag type for a rule.");

        constexpr rule(char const * diagnostic_text)
        {
            this->parser_.diagnostic_text_ = diagnostic_text;
        }

        template<typename T, typename... Ts>
        constexpr auto with(T && x, Ts &&... xs) const
        {
            BOOST_ASSERT((detail::is_nope_v<ParamsTuple> &&
                 "If you're seeing this, you tried to chain calls on a rule, "
                 "like 'rule.with(foo).with(bar)'.  Quit it!'"));



            using params_tuple_type = decltype(detail::hl::make_tuple(
                static_cast<T &&>(x), static_cast<Ts &&>(xs)...));
            using rule_parser_type = rule_parser<
                false,
                TagType,
                Attribute,
                LocalState,
                params_tuple_type>;
            using result_type = parser_interface<rule_parser_type>;
            return result_type{rule_parser_type{
                this->parser_.diagnostic_text_,
                detail::hl::make_tuple(
                    static_cast<T &&>(x), static_cast<Ts &&>(xs)...)}};
        }
    };

    template<
        typename TagType,
        typename Attribute,
        typename LocalState,
        typename ParamsTuple>
    struct callback_rule
        : parser_interface<
              rule_parser<true, TagType, Attribute, LocalState, ParamsTuple>>
    {
        constexpr callback_rule(char const * diagnostic_text)
        {
            this->parser_.diagnostic_text_ = diagnostic_text;
        }

        template<typename T, typename... Ts>
        constexpr auto with(T && x, Ts &&... xs) const
        {
            BOOST_ASSERT((detail::is_nope_v<ParamsTuple> &&
                 "If you're seeing this, you tried to chain calls on a "
                 "callback_rule, like 'rule.with(foo).with(bar)'.  Quit it!'"));



            using params_tuple_type = decltype(detail::hl::make_tuple(
                static_cast<T &&>(x), static_cast<Ts &&>(xs)...));
            using rule_parser_type = rule_parser<
                true,
                TagType,
                Attribute,
                LocalState,
                params_tuple_type>;
            using result_type = parser_interface<rule_parser_type>;
            return result_type{rule_parser_type{
                this->parser_.diagnostic_text_,
                detail::hl::make_tuple(
                    static_cast<T &&>(x), static_cast<Ts &&>(xs)...)}};
        }
    };



#define BOOST_PARSER_DEFINE_IMPL(_, diagnostic_text_) template< bool UseCallbacks, typename Iter, typename Sentinel, typename Context, typename SkipParser> auto parse_rule( decltype(diagnostic_text_)::parser_type::tag_type *, std::bool_constant<UseCallbacks> use_cbs, Iter & first, Sentinel last, Context const & context, SkipParser const & skip, boost::parser::detail::flags flags, bool & success) { auto const & parser = BOOST_PARSER_PP_CAT(diagnostic_text_, _def); return parser(use_cbs, first, last, context, skip, flags, success); } template< bool UseCallbacks, typename Iter, typename Sentinel, typename Context, typename SkipParser, typename Attribute> void parse_rule( decltype(diagnostic_text_)::parser_type::tag_type *, std::bool_constant<UseCallbacks> use_cbs, Iter & first, Sentinel last, Context const & context, SkipParser const & skip, boost::parser::detail::flags flags, bool & success, Attribute & retval) { auto const & parser = BOOST_PARSER_PP_CAT(diagnostic_text_, _def); using attr_t = decltype(parser( use_cbs, first, last, context, skip, flags, success)); if constexpr (boost::parser::detail::is_nope_v<attr_t>) { parser(use_cbs, first, last, context, skip, flags, success); } else { parser( use_cbs, first, last, context, skip, flags, success, retval); } }
#line 5360 "boost/parser/parser.hpp"
#define BOOST_PARSER_DEFINE_RULES(...) BOOST_PARSER_PP_FOR_EACH(BOOST_PARSER_DEFINE_IMPL, _, __VA_ARGS__)





    template<typename ParserTuple>
    template<typename Parser>
    constexpr auto or_parser<ParserTuple>::prepend(
        parser_interface<Parser> parser) const noexcept
    {
#line 5380 "boost/parser/parser.hpp"
        BOOST_ASSERT(!detail::is_unconditional_eps<Parser>{});
        return parser_interface{
            or_parser<decltype(detail::hl::prepend(parsers_, parser.parser_))>{
                detail::hl::prepend(parsers_, parser.parser_)}};
    }

    template<typename ParserTuple>
    template<typename Parser>
    constexpr auto or_parser<ParserTuple>::append(
        parser_interface<Parser> parser) const noexcept
    {
#line 5400 "boost/parser/parser.hpp"
        BOOST_ASSERT(!detail::is_unconditional_eps_v<decltype(
                                detail::hl::back(parsers_))>);

        if constexpr (detail::is_or_p<Parser>{}) {
            return parser_interface{or_parser<decltype(
                detail::hl::concat(parsers_, parser.parser_.parsers_))>{
                detail::hl::concat(parsers_, parser.parser_.parsers_)}};
        } else {
            return parser_interface{or_parser<decltype(
                detail::hl::append(parsers_, parser.parser_))>{
                detail::hl::append(parsers_, parser.parser_)}};
        }
    }

    template<
        typename ParserTuple,
        typename BacktrackingTuple,
        typename CombiningGroups>
    template<bool AllowBacktracking, typename Parser>
    constexpr auto
    seq_parser<ParserTuple, BacktrackingTuple, CombiningGroups>::prepend(
        parser_interface<Parser> parser) const noexcept
    {
        using combining_groups =
            detail::combining_t<ParserTuple, CombiningGroups>;
        using final_combining_groups =
            decltype(detail::hl::prepend(combining_groups{}, llong<0>{}));
        using backtracking = decltype(detail::hl::prepend(
            detail::hl::prepend(
                detail::hl::drop_front(BacktrackingTuple{}),
                std::bool_constant<AllowBacktracking>{}),
            std::true_type{}));
        using parser_t = seq_parser<
            decltype(detail::hl::prepend(parsers_, parser.parser_)),
            backtracking,
            final_combining_groups>;
        return parser_interface{
            parser_t{detail::hl::prepend(parsers_, parser.parser_)}};
    }

    template<
        typename ParserTuple,
        typename BacktrackingTuple,
        typename CombiningGroups>
    template<bool AllowBacktracking, typename Parser>
    constexpr auto
    seq_parser<ParserTuple, BacktrackingTuple, CombiningGroups>::append(
        parser_interface<Parser> parser) const noexcept
    {
        using combining_groups =
            detail::combining_t<ParserTuple, CombiningGroups>;
        if constexpr (detail::is_seq_p<Parser>{}) {
            using parser_combining_groups = detail::combining_t<
                decltype(parser.parser_.parsers_),
                typename Parser::combining_groups>;
            using final_combining_groups = detail::
                combined_combining_t<combining_groups, parser_combining_groups>;
            using backtracking = decltype(detail::hl::concat(
                BacktrackingTuple{}, typename Parser::backtracking{}));
            using parser_t = seq_parser<
                decltype(detail::hl::concat(parsers_, parser.parser_.parsers_)),
                backtracking,
                final_combining_groups>;
            return parser_interface{parser_t{
                detail::hl::concat(parsers_, parser.parser_.parsers_)}};
        } else {
            using final_combining_groups =
                decltype(detail::hl::append(combining_groups{}, llong<0>{}));
            using backtracking = decltype(detail::hl::append(
                BacktrackingTuple{}, std::bool_constant<AllowBacktracking>{}));
            using parser_t = seq_parser<
                decltype(detail::hl::append(parsers_, parser.parser_)),
                backtracking,
                final_combining_groups>;
            return parser_interface{
                parser_t{detail::hl::append(parsers_, parser.parser_)}};
        }
    }
#line 5486 "boost/parser/parser.hpp"
    template<template<class> class Parser>
    struct directive
    {
        template<typename Parser2>
        constexpr auto operator[](parser_interface<Parser2> rhs) const noexcept
        {
            return parser_interface{Parser<Parser2>{rhs.parser_}};
        }
    };




    inline constexpr directive<omit_parser> omit;




    inline constexpr directive<raw_parser> raw;
#line 5516 "boost/parser/parser.hpp"
    inline constexpr directive<lexeme_parser> lexeme;




    inline constexpr directive<no_case_parser> no_case;



    template<typename MinType, typename MaxType>
    struct repeat_directive
    {
        template<typename Parser2>
        constexpr auto operator[](parser_interface<Parser2> rhs) const noexcept
        {
            using repeat_parser_type =
                repeat_parser<Parser2, detail::nope, MinType, MaxType>;
            return parser_interface{
                repeat_parser_type{rhs.parser_, min_, max_}};
        }

        MinType min_;
        MaxType max_;
    };




    template<typename T>
    constexpr repeat_directive<T, T> repeat(T n) noexcept
    {
        return repeat_directive<T, T>{n, n};
    }





    template<typename MinType, typename MaxType>
    constexpr repeat_directive<MinType, MaxType>
    repeat(MinType min_, MaxType max_) noexcept
    {
        return repeat_directive<MinType, MaxType>{min_, max_};
    }






    template<typename SkipParser = detail::nope>
    struct skip_directive
    {
        template<typename Parser>
        constexpr auto operator[](parser_interface<Parser> rhs) const noexcept
        {
            return parser_interface{
                skip_parser<Parser, SkipParser>{rhs.parser_, skip_parser_}};
        }


        template<typename SkipParser2>
        constexpr auto
        operator()(parser_interface<SkipParser2> skip_parser) const noexcept
        {
            BOOST_ASSERT((detail::is_nope_v<SkipParser> &&
                 "If you're seeing this, you tried to chain calls on skip, "
                 "like 'skip(foo)(bar)'.  Quit it!'"));



            return skip_directive<parser_interface<SkipParser2>>{skip_parser};
        }

        SkipParser skip_parser_;
    };




    inline constexpr skip_directive<> skip;




    struct merge_directive
    {
        template<
            typename ParserTuple,
            typename BacktrackingTuple,
            typename CombiningGroups>
        constexpr auto
        operator[](parser_interface<
                   seq_parser<ParserTuple, BacktrackingTuple, CombiningGroups>>
                       rhs) const noexcept
        {
            return parser_interface{
                seq_parser<ParserTuple, BacktrackingTuple, detail::merge_t>{
                    rhs.parser_.parsers_}};
        }
    };
#line 5622 "boost/parser/parser.hpp"
    inline constexpr merge_directive merge;




    struct separate_directive
    {
        template<
            typename ParserTuple,
            typename BacktrackingTuple,
            typename CombiningGroups>
        constexpr auto
        operator[](parser_interface<
                   seq_parser<ParserTuple, BacktrackingTuple, CombiningGroups>>
                       rhs) const noexcept
        {
            return parser_interface{
                seq_parser<ParserTuple, BacktrackingTuple, detail::separate_t>{
                    rhs.parser_.parsers_}};
        }
    };
#line 5650 "boost/parser/parser.hpp"
    inline constexpr separate_directive separate;




    template<typename Predicate>
    struct eps_parser
    {
        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser>
        detail::nope call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success) const noexcept
        {
            auto _ = detail::scoped_trace(
                *this, first, last, context, flags, detail::global_nope);
            boost::parser::subrange const where(first, first);
            auto const predicate_context = detail::make_action_context(
                context, detail::global_nope, where);
#line 5682 "boost/parser/parser.hpp"
            success = pred_(predicate_context);
            return {};
        }

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename Attribute>
        void call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success,
            Attribute & retval) const
        {
            auto _ = detail::scoped_trace(
                *this, first, last, context, flags, retval);
            boost::parser::subrange const where(first, first);
            auto const predicate_context = detail::make_action_context(
                context, detail::global_nope, where);
#line 5712 "boost/parser/parser.hpp"
            success = pred_(predicate_context);
        }



        template<typename Predicate2>
        constexpr auto operator()(Predicate2 pred) const noexcept
        {
            BOOST_ASSERT((detail::is_nope_v<Predicate> &&
                 "If you're seeing this, you tried to chain calls on eps, "
                 "like 'eps(foo)(bar)'.  Quit it!'"));



            return parser_interface{eps_parser<Predicate2>{std::move(pred)}};
        }

        Predicate pred_;
    };




    inline constexpr parser_interface<eps_parser<detail::nope>> eps;

    struct eoi_parser
    {
        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser>
        detail::nope call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success) const
        {
            auto _ = detail::scoped_trace(
                *this, first, last, context, flags, detail::global_nope);
            if (first != last)
                success = false;
            return {};
        }

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename Attribute>
        void call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success,
            Attribute & retval) const
        {
            auto _ = detail::scoped_trace(
                *this, first, last, context, flags, retval);
            if (first != last)
                success = false;
        }
    };


    inline constexpr parser_interface<eoi_parser> eoi;

    template<typename Attribute>
    struct attr_parser
    {
        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser>
        auto call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const &,
            detail::flags flags,
            bool &) const
        {
            auto _ = detail::scoped_trace(
                *this, first, last, context, flags, detail::global_nope);
            return detail::resolve(context, attr_);
        }

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename Attribute_>
        void call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success,
            Attribute_ & retval) const
        {
            auto _ = detail::scoped_trace(
                *this, first, last, context, flags, retval);
            if (detail::gen_attrs(flags))
                detail::assign_copy(retval, detail::resolve(context, attr_));
        }

        Attribute attr_;
    };



    template<typename Attribute>
    constexpr auto attr(Attribute a) noexcept
    {
        return parser_interface{attr_parser<Attribute>{std::move(a)}};
    }

    template<typename Expected, typename AttributeType>
    struct char_parser
    {
        constexpr char_parser() {}
        constexpr char_parser(Expected expected) : expected_(expected) {}

        template<typename T>
        using attribute_t = std::conditional_t<
            std::is_same_v<AttributeType, void>,
            std::decay_t<T>,
            AttributeType>;

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser>
        auto call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success) const -> attribute_t<decltype(*first)>
        {
            attribute_t<decltype(*first)> retval;
            call(use_cbs, first, last, context, skip, flags, success, retval);
            return retval;
        }

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename Attribute>
        void call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success,
            Attribute & retval) const
        {
            auto _ = detail::scoped_trace(
                *this, first, last, context, flags, retval);

            if (first == last) {
                success = false;
                return;
            }
            attribute_t<decltype(*first)> const x = *first;
            if (detail::unequal(context, x, expected_)) {
                success = false;
                return;
            }
            detail::assign(retval, x);
            ++first;
        }
#line 5915 "boost/parser/parser.hpp"
        template<
            typename T,
            typename Enable =
                std::enable_if_t<!detail::is_parsable_range_like_v<T>>>

        constexpr auto operator()(T x) const noexcept

        {
            BOOST_ASSERT((detail::is_nope_v<Expected> &&
                 "If you're seeing this, you tried to chain calls on char_, "
                 "like 'char_('a')('b')'.  Quit it!'"));



            return parser_interface{
                char_parser<T, AttributeType>{std::move(x)}};
        }



        template<typename LoType, typename HiType>
        constexpr auto operator()(LoType lo, HiType hi) const noexcept
        {
            BOOST_ASSERT((detail::is_nope_v<Expected> &&
                 "If you're seeing this, you tried to chain calls on char_, "
                 "like 'char_('a', 'b')('c', 'd')'.  Quit it!'"));



            using char_pair_t = detail::char_pair<LoType, HiType>;
            using char_parser_t = char_parser<char_pair_t, AttributeType>;
            return parser_interface(
                char_parser_t(char_pair_t{std::move(lo), std::move(hi)}));
        }
#line 5955 "boost/parser/parser.hpp"
        template<
            typename R,
            typename Enable =
                std::enable_if_t<detail::is_parsable_range_like_v<R>>>

        constexpr auto operator()(R && r) const noexcept
        {
            BOOST_ASSERT(((!std::is_rvalue_reference_v<R &&> ||
                  !detail::is_range<detail::remove_cv_ref_t<R>>) &&
                     "It looks like you tried to pass an rvalue range to "
                     "char_().  Don't do that, or you'll end up with dangling "
                     "references."));





            BOOST_ASSERT((detail::is_nope_v<Expected> &&
                 "If you're seeing this, you tried to chain calls on char_, "
                 "like 'char_(char-set)(char-set)'.  Quit it!'"));



            auto chars = detail::make_char_range<false>(r);
            using char_range_t = decltype(chars);
            using char_parser_t = char_parser<char_range_t, AttributeType>;
            return parser_interface(char_parser_t(chars));
        }
#line 5988 "boost/parser/parser.hpp"
        template<
            typename R,
            typename Enable = std::enable_if_t<
                detail::is_parsable_range_like_v<R> &&
                std::is_same_v<detail::range_value_t<R>, char32_t>>>

        constexpr auto operator()(sorted_t, R && r) const noexcept

        {
            BOOST_ASSERT(((!std::is_rvalue_reference_v<R &&> ||
                  !detail::is_range<detail::remove_cv_ref_t<R>>) &&
                     "It looks like you tried to pass an rvalue range to "
                     "char_().  Don't do that, or you'll end up with dangling "
                     "references."));





            BOOST_ASSERT((detail::is_nope_v<Expected> &&
                 "If you're seeing this, you tried to chain calls on char_, "
                 "like 'char_(char-set)(char-set)'.  Quit it!'"));



            auto chars = detail::make_char_range<true>(r);
            using char_range_t = decltype(chars);
            using char_parser_t = char_parser<char_range_t, AttributeType>;
            return parser_interface(char_parser_t(chars));
        }

        Expected expected_;
    };

    struct digit_parser
    {
        constexpr digit_parser() {}

        template<typename T>
        using attribute_t = std::decay_t<T>;

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser>
        auto call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success) const -> attribute_t<decltype(*first)>
        {
            attribute_t<decltype(*first)> retval;
            call(use_cbs, first, last, context, skip, flags, success, retval);
            return retval;
        }

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename Attribute>
        void call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success,
            Attribute & retval) const
        {
            auto _ = detail::scoped_trace(
                *this, first, last, context, flags, retval);

            if (first == last) {
                success = false;
                return;
            }
            attribute_t<decltype(*first)> const x = *first;
            char32_t const x_cmp = x;
            if (x_cmp < U'\x0100' && (x_cmp < U'0' || U'9' < x_cmp)) {
                success = false;
                return;
            }
            char32_t const * it = std::upper_bound(
                std::begin(zero_cps) + 1, std::end(zero_cps), x);
            if (it == std::begin(zero_cps) || x_cmp < *(it - 1) ||
                *(it - 1) + 9 < x_cmp) {
                success = false;
                return;
            }
            detail::assign(retval, x);
            ++first;
        }
#line 6087 "boost/parser/parser.hpp"
        static constexpr char32_t zero_cps[] = {
            U'\u0030',
            U'\u0660',
            U'\u06F0',
            U'\u07C0',
            U'\u0966',
            U'\u09E6',
            U'\u0A66',
            U'\u0AE6',
            U'\u0B66',
            U'\u0BE6',
            U'\u0C66',
            U'\u0CE6',
            U'\u0D66',
            U'\u0DE6',
            U'\u0E50',
            U'\u0ED0',
            U'\u0F20',
            U'\u1040',
            U'\u1090',
            U'\u17E0',
            U'\u1810',
            U'\u1946',
            U'\u19D0',
            U'\u1A80',
            U'\u1A90',
            U'\u1B50',
            U'\u1BB0',
            U'\u1C40',
            U'\u1C50',
            U'\uA620',
            U'\uA8D0',
            U'\uA900',
            U'\uA9D0',
            U'\uA9F0',
            U'\uAA50',
            U'\uABF0',
            U'\uFF10',
            U'\U000104A0',
            U'\U00010D30',
            U'\U00011066',
            U'\U000110F0',
            U'\U00011136',
            U'\U000111D0',
            U'\U000112F0',
            U'\U00011450',
            U'\U000114D0',
            U'\U00011650',
            U'\U000116C0',
            U'\U00011730',
            U'\U000118E0',
            U'\U00011950',
            U'\U00011C50',
            U'\U00011D50',
            U'\U00011DA0',
            U'\U00011F50',
            U'\U00016A60',
            U'\U00016AC0',
            U'\U00016B50',
            U'\U0001D7CE',
            U'\U0001D7D8',
            U'\U0001D7E2',
            U'\U0001D7EC',
            U'\U0001D7F6',
            U'\U0001E140',
            U'\U0001E2F0',
            U'\U0001E4F0',
            U'\U0001E950',
            U'\U0001FBF0'
        };
    };

    template<typename Tag>
    struct char_set_parser
    {
         char_set_parser()
        {
            auto const & chars = detail::char_set<Tag>::chars;
            auto const first = std::begin(chars);
            auto const last = std::end(chars);
            auto it = std::upper_bound(first, last, 0x100);
            if (it != last)
                one_byte_offset_ = it - first;
        }

        template<typename T>
        using attribute_t = std::decay_t<T>;

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser>
        auto call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success) const -> attribute_t<decltype(*first)>
        {
            attribute_t<decltype(*first)> retval;
            call(use_cbs, first, last, context, skip, flags, success, retval);
            return retval;
        }

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename Attribute>
        void call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success,
            Attribute & retval) const
        {
            auto _ = detail::scoped_trace(
                *this, first, last, context, flags, retval);

            if (first == last) {
                success = false;
                return;
            }

            auto const & chars = detail::char_set<Tag>::chars;
            attribute_t<decltype(*first)> const x = *first;
            uint32_t const x_cmp = x;
            if (x_cmp < U'\x0100') {
                uint32_t const * it = std::lower_bound(
                    std::begin(chars),
                    std::begin(chars) + one_byte_offset_,
                    x_cmp);
                if (it != std::end(chars) && *it == x_cmp) {
                    detail::assign(retval, x_cmp);
                    ++first;
                } else {
                    success = false;
                }
                return;
            }

            uint32_t const * it = std::lower_bound(
                std::begin(chars) + one_byte_offset_, std::end(chars), x_cmp);
            if (it != std::end(chars) && *it == x_cmp) {
                detail::assign(retval, x_cmp);
                ++first;
                return;
            }

            success = false;
        }

        int one_byte_offset_ = 0;
    };

    template<typename Tag>
    struct char_subrange_parser
    {
        constexpr char_subrange_parser() {}

        template<typename T>
        using attribute_t = std::decay_t<T>;

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser>
        auto call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success) const -> attribute_t<decltype(*first)>
        {
            attribute_t<decltype(*first)> retval;
            call(use_cbs, first, last, context, skip, flags, success, retval);
            return retval;
        }

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename Attribute>
        void call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success,
            Attribute & retval) const
        {
            auto _ = detail::scoped_trace(
                *this, first, last, context, flags, retval);

            if (first == last) {
                success = false;
                return;
            }
            attribute_t<decltype(*first)> const x = *first;
            char32_t const x_cmp = x;
            success = false;
            for (auto subrange : detail::char_subranges<Tag>::ranges) {
                if (subrange.lo_ <= x_cmp && x_cmp <= subrange.hi_) {
                    success = true;
                    detail::assign(retval, x);
                    ++first;
                    return;
                }
            }
        }
    };
#line 6323 "boost/parser/parser.hpp"
    inline constexpr parser_interface<char_parser<detail::nope>> char_;
#line 6331 "boost/parser/parser.hpp"
    inline constexpr parser_interface<char_parser<detail::nope, char32_t>> cp;
#line 6339 "boost/parser/parser.hpp"
    inline constexpr parser_interface<char_parser<detail::nope, char>> cu;


    inline constexpr auto lit(char c) noexcept { return omit[char_(c)]; }
#line 6350 "boost/parser/parser.hpp"
    inline constexpr auto lit(char32_t c) noexcept { return omit[char_(c)]; }

    template<typename StrIter, typename StrSentinel>
    struct string_parser
    {
        constexpr string_parser() : expected_first_(), expected_last_() {}




        template<
            typename R,
            typename Enable =
                std::enable_if_t<detail::is_parsable_range_like_v<R>>>

        constexpr string_parser(R && r) :
            expected_first_(detail::make_view_begin(r)),
            expected_last_(detail::make_view_end(r))
        {}

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser>
        std::string call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success) const
        {
            std::string retval;
            call(use_cbs, first, last, context, skip, flags, success, retval);
            return retval;
        }

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename Attribute>
        void call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success,
            Attribute & retval) const
        {
            auto _ = detail::scoped_trace(
                *this, first, last, context, flags, retval);

            if (first == last) {
                success = false;
                return;
            }

            if constexpr (std::is_same_v<
                              detail::remove_cv_ref_t<decltype(*first)>,
                              char32_t>) {
                auto const cps =
                    boost::parser::subrange(expected_first_, expected_last_) |
                    detail::text::as_utf32;

                auto const mismatch = detail::no_case_aware_string_mismatch(
                    first,
                    last,
                    cps.begin(),
                    cps.end(),
                    context.no_case_depth_);
                if (mismatch.second != cps.end()) {
                    success = false;
                    return;
                }

                detail::append(
                    retval, first, mismatch.first, detail::gen_attrs(flags));

                first = mismatch.first;
            } else {
                auto const mismatch = detail::no_case_aware_string_mismatch(
                    first,
                    last,
                    expected_first_,
                    expected_last_,
                    context.no_case_depth_);
                if (mismatch.second != expected_last_) {
                    success = false;
                    return;
                }

                detail::append(
                    retval, first, mismatch.first, detail::gen_attrs(flags));

                first = mismatch.first;
            }
        }

        StrIter expected_first_;
        StrSentinel expected_last_;
    };




    template<typename R>

    string_parser(R r) -> string_parser<
        decltype(detail::make_view_begin(r)),
        decltype(detail::make_view_end(r))>;






    template<typename R>

    constexpr auto string(R && str) noexcept
    {
        return parser_interface{string_parser(str)};
    }





    template<typename R>

    constexpr auto lit(R && str) noexcept
    {
        return omit[parser::string(str)];
    }

    template<bool NewlinesOnly, bool NoNewlines>
    struct ws_parser
    {
        constexpr ws_parser() {}

        static_assert(!NewlinesOnly || !NoNewlines);

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser>
        detail::nope call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success) const
        {
            detail::nope nope;
            call(use_cbs, first, last, context, skip, flags, success, nope);
            return {};
        }

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename Attribute>
        void call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success,
            Attribute & retval) const
        {
            auto _ = detail::scoped_trace(
                *this, first, last, context, flags, retval);

            if (first == last) {
                success = false;
                return;
            }
            int const x = *first;
            if constexpr (NewlinesOnly) {
                if (x == 0x000a) {
                    ++first;
                    return;
                }
                if (x == 0x000d) {
                    ++first;
                    if (first != last && *first == 0x000a)
                        ++first;
                    return;
                }
                if (0x000b == x || x == 0x000c || x == 0x0085 || x == 0x2028 ||
                    x == 0x2029) {
                    ++first;
                    return;
                }
                success = false;
            } else if constexpr (NoNewlines) {
                if (x == 0x0020) {
                    ++first;
                    return;
                }
                if (x == 0x0009) {
                    ++first;
                    return;
                }
                if (x == 0x00a0 || x == 0x1680 ||
                    (0x2000 <= x && x <= 0x200a) || x == 0x202F ||
                    x == 0x205F || x == 0x3000) {
                    ++first;
                    return;
                }
                success = false;
            } else {
                if (x == 0x0020 || x == 0x000a) {
                    ++first;
                    return;
                }
                if (x == 0x000d) {
                    ++first;
                    if (first != last && *first == 0x000a)
                        ++first;
                    return;
                }
                if (0x0009 <= x && x <= 0x000c) {
                    ++first;
                    return;
                }
                if (x == 0x0085 || x == 0x00a0 || x == 0x1680 ||
                    (0x2000 <= x && x <= 0x200a) || x == 0x2028 ||
                    x == 0x2029 || x == 0x202F || x == 0x205F || x == 0x3000) {
                    ++first;
                    return;
                }
                success = false;
            }
        }
    };




    inline constexpr parser_interface<ws_parser<true, false>> eol;





    inline constexpr parser_interface<ws_parser<false, false>> ws;





    inline constexpr parser_interface<ws_parser<false, true>> blank;





    inline constexpr parser_interface<digit_parser> digit;




    inline constexpr parser_interface<
        char_subrange_parser<detail::hex_digit_subranges>>
        hex_digit;



    inline constexpr parser_interface<
        char_subrange_parser<detail::control_subranges>>
        control;




    inline
        parser_interface<char_set_parser<detail::punct_chars>>
            punct;



    inline
        parser_interface<char_set_parser<detail::lower_case_chars>>
            lower;



    inline
        parser_interface<char_set_parser<detail::upper_case_chars>>
            upper;

    struct bool_parser
    {
        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser>
        bool call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success) const
        {
            bool retval;
            call(use_cbs, first, last, context, skip, flags, success, retval);
            return retval;
        }

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename Attribute>
        void call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success,
            Attribute & retval) const
        {
            auto _ = detail::scoped_trace(
                *this, first, last, context, flags, retval);

            auto compare =
                [no_case = context.no_case_depth_](char32_t a, char32_t b) {
                    if (no_case && 0x41 <= b && b < 0x5b)
                        b += 0x20;
                    return a == b;
                };



            char const t[] = "true";
            if (detail::mismatch(t, t + 4, first, last, compare).first ==
                t + 4) {
                std::advance(first, 4);
                detail::assign(retval, true);
                return;
            }
            char const f[] = "false";
            if (detail::mismatch(f, f + 5, first, last, compare).first ==
                f + 5) {
                std::advance(first, 5);
                detail::assign(retval, false);
                return;
            }
            success = false;
        }
    };



    inline constexpr parser_interface<bool_parser> bool_;

    template<
        typename T,
        int Radix,
        int MinDigits,
        int MaxDigits,
        typename Expected>
    struct uint_parser
    {
        static_assert(2 <= Radix && Radix <= 36, "Unsupported radix.");

        constexpr uint_parser() {}
        explicit constexpr uint_parser(Expected expected) : expected_(expected)
        {}

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser>
        T call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success) const
        {
            T retval;
            call(use_cbs, first, last, context, skip, flags, success, retval);
            return retval;
        }

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename Attribute>
        void call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success,
            Attribute & retval) const
        {
            auto _ = detail::scoped_trace(
                *this, first, last, context, flags, retval);
            using extract =
                detail_spirit_x3::extract_uint<T, Radix, MinDigits, MaxDigits>;
            T attr = 0;
            success = extract::call(first, last, attr);
            if (attr != detail::resolve(context, expected_))
                success = false;
            if (success)
                detail::assign(retval, attr);
        }



        template<typename Expected2>
        constexpr auto operator()(Expected2 expected) const noexcept
        {
            BOOST_ASSERT((detail::is_nope_v<Expected> &&
                 "If you're seeing this, you tried to chain calls on this "
                 "parser, like 'uint_(2)(3)'.  Quit it!'"));



            using parser_t =
                uint_parser<T, Radix, MinDigits, MaxDigits, Expected2>;
            return parser_interface{parser_t{expected}};
        }

        Expected expected_;
    };



    inline constexpr parser_interface<uint_parser<unsigned int, 2>> bin;



    inline constexpr parser_interface<uint_parser<unsigned int, 8>> oct;



    inline constexpr parser_interface<uint_parser<unsigned int, 16>> hex;



    inline constexpr parser_interface<uint_parser<unsigned short>> ushort_;



    inline constexpr parser_interface<uint_parser<unsigned int>> uint_;



    inline constexpr parser_interface<uint_parser<unsigned long>> ulong_;



    inline constexpr parser_interface<uint_parser<unsigned long long>>
        ulong_long;

    template<
        typename T,
        int Radix,
        int MinDigits,
        int MaxDigits,
        typename Expected>
    struct int_parser
    {
        static_assert(
            Radix == 2 || Radix == 8 || Radix == 10 || Radix == 16,
            "Unsupported radix.");

        constexpr int_parser() {}
        explicit constexpr int_parser(Expected expected) : expected_(expected)
        {}

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser>
        T call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success) const
        {
            T retval;
            call(use_cbs, first, last, context, skip, flags, success, retval);
            return retval;
        }

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename Attribute>
        void call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success,
            Attribute & retval) const
        {
            auto _ = detail::scoped_trace(
                *this, first, last, context, flags, retval);
            using extract =
                detail_spirit_x3::extract_int<T, Radix, MinDigits, MaxDigits>;
            T attr = 0;
            auto const initial = first;
            success = extract::call(first, last, attr);
            if (first == initial || attr != detail::resolve(context, expected_))
                success = false;
            if (success)
                detail::assign(retval, attr);
        }



        template<typename Expected2>
        constexpr auto operator()(Expected2 expected) const noexcept
        {
            BOOST_ASSERT((detail::is_nope_v<Expected> &&
                 "If you're seeing this, you tried to chain calls on this "
                 "parser, like 'int_(2)(3)'.  Quit it!'"));



            using parser_t =
                int_parser<T, Radix, MinDigits, MaxDigits, Expected2>;
            return parser_interface{parser_t{expected}};
        }

        Expected expected_;
    };



    inline constexpr parser_interface<int_parser<short>> short_;



    inline constexpr parser_interface<int_parser<int>> int_;



    inline constexpr parser_interface<int_parser<long>> long_;



    inline constexpr parser_interface<int_parser<long long>> long_long;

    template<typename T>
    struct float_parser
    {
        constexpr float_parser() {}

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser>
        T call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success) const
        {
            T retval;
            call(use_cbs, first, last, context, skip, flags, success, retval);
            return retval;
        }

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename Attribute>
        void call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success,
            Attribute & retval) const
        {
            auto _ = detail::scoped_trace(
                *this, first, last, context, flags, retval);
            detail_spirit_x3::real_policies<T> policies;
            using extract = detail_spirit_x3::
                extract_real<T, detail_spirit_x3::real_policies<T>>;
            T attr = 0;
            auto const initial = first;
            success = extract::parse(first, last, attr, policies);
            if (first == initial)
                success = false;
            if (success)
                detail::assign(retval, attr);
        }
    };


    inline constexpr parser_interface<float_parser<float>> float_;


    inline constexpr parser_interface<float_parser<double>> double_;





    template<typename Predicate>
    struct if_directive
    {
        template<typename Parser2>
        constexpr auto operator[](parser_interface<Parser2> rhs) const noexcept
        {
            return eps(pred_) >> rhs;
        }

        Predicate pred_;
    };




    template<typename Predicate>
    constexpr auto if_(Predicate pred) noexcept
    {
        return if_directive<Predicate>{pred};
    }

    namespace detail {
        template<typename SwitchValue, typename Value>
        struct switch_parser_equal
        {
            template<typename Context>
            bool operator()(Context & context) const
            {
                auto const switch_value =
                    detail::resolve(context, switch_value_);
                auto const value = detail::resolve(context, value_);
                return value == switch_value;
            }
            SwitchValue switch_value_;
            Value value_;
        };
    }

    template<typename SwitchValue, typename OrParser>
    struct switch_parser
    {
        switch_parser() {}
        switch_parser(SwitchValue switch_value) : switch_value_(switch_value) {}
        switch_parser(SwitchValue switch_value, OrParser or_parser) :
            switch_value_(switch_value), or_parser_(or_parser)
        {}

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser>
        auto call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success) const
        {
            BOOST_ASSERT((!detail::is_nope_v<OrParser> &&
                 "It looks like you tried to write switch_(val).  You need at "
                 "least one alternative, like: switch_(val)(value_1, "
                 "parser_1)(value_2, parser_2)..."));




            using attr_t = decltype(or_parser_.call(
                use_cbs, first, last, context, skip, flags, success));
            attr_t attr{};
            auto _ =
                detail::scoped_trace(*this, first, last, context, flags, attr);
            attr = or_parser_.call(
                use_cbs, first, last, context, skip, flags, success);
            return attr;
        }

        template<
            bool UseCallbacks,
            typename Iter,
            typename Sentinel,
            typename Context,
            typename SkipParser,
            typename Attribute>
        void call(
            std::bool_constant<UseCallbacks> use_cbs,
            Iter & first,
            Sentinel last,
            Context const & context,
            SkipParser const & skip,
            detail::flags flags,
            bool & success,
            Attribute & retval) const
        {
            BOOST_ASSERT((!detail::is_nope_v<OrParser>,
                 "It looks like you tried to write switch_(val).  You need at "
                 "least one alternative, like: switch_(val)(value_1, "
                 "parser_1)(value_2, parser_2)..."));




            auto _ = detail::scoped_trace(
                *this, first, last, context, flags, retval);
            or_parser_.call(
                use_cbs, first, last, context, skip, flags, success, retval);
        }



        template<typename Value, typename Parser2>
        constexpr auto
        operator()(Value value_, parser_interface<Parser2> rhs) const noexcept
        {
            auto const match = detail::switch_parser_equal<SwitchValue, Value>{
                switch_value_, value_};
            auto or_parser = make_or_parser(or_parser_, eps(match) >> rhs);
            using switch_parser_type =
                switch_parser<SwitchValue, decltype(or_parser)>;
            return parser_interface{
                switch_parser_type{switch_value_, or_parser}};
        }



        template<typename Parser1, typename Parser2>
        static constexpr auto
        make_or_parser(Parser1 parser1, parser_interface<Parser2> parser2)
        {
            return (parser_interface{parser1} | parser2).parser_;
        }

        template<typename Parser>
        static constexpr auto
        make_or_parser(detail::nope, parser_interface<Parser> parser)
        {
            return parser.parser_;
        }



        SwitchValue switch_value_;
        OrParser or_parser_;
    };
#line 7156 "boost/parser/parser.hpp"
    template<typename T>
    constexpr auto switch_(T x) noexcept
    {
        return switch_parser<T>{x};
    }




    template<typename Parser, typename GlobalState, typename ErrorHandler>
    constexpr auto
    parser_interface<Parser, GlobalState, ErrorHandler>::operator>>(
        char rhs) const noexcept
    {
        return *this >> parser::lit(rhs);
    }

    template<typename Parser, typename GlobalState, typename ErrorHandler>
    constexpr auto
    parser_interface<Parser, GlobalState, ErrorHandler>::operator>>(
        char32_t rhs) const noexcept
    {
        return *this >> parser::lit(rhs);
    }

    template<typename Parser, typename GlobalState, typename ErrorHandler>



    template<typename R, typename>

    constexpr auto
    parser_interface<Parser, GlobalState, ErrorHandler>::operator>>(
        R && r) const noexcept
    {
        return *this >> parser::lit(r);
    }




    template<typename Parser>
    constexpr auto operator>>(char c, parser_interface<Parser> rhs) noexcept
    {
        if constexpr (detail::is_seq_p<Parser>{}) {
            return rhs.parser_.template prepend<true>(parser::lit(c));
        } else {
            return parser::lit(c) >> rhs;
        }
    }


    template<typename Parser>
    constexpr auto operator>>(char32_t c, parser_interface<Parser> rhs) noexcept
    {
        if constexpr (detail::is_seq_p<Parser>{}) {
            return rhs.parser_.template prepend<true>(parser::lit(c));
        } else {
            return parser::lit(c) >> rhs;
        }
    }





    template<
        typename R,
        typename Parser,
        typename Enable = std::enable_if_t<detail::is_parsable_range_like_v<R>>>

    constexpr auto operator>>(R && r, parser_interface<Parser> rhs) noexcept
    {
        if constexpr (detail::is_seq_p<Parser>{}) {
            return rhs.parser_.template prepend<true>(parser::lit(r));
        } else {
            return parser::lit(r) >> rhs;
        }
    }



    template<typename Parser, typename GlobalState, typename ErrorHandler>
    constexpr auto
    parser_interface<Parser, GlobalState, ErrorHandler>::operator>(
        char rhs) const noexcept
    {
        return *this > parser::lit(rhs);
    }

    template<typename Parser, typename GlobalState, typename ErrorHandler>
    constexpr auto
    parser_interface<Parser, GlobalState, ErrorHandler>::operator>(
        char32_t rhs) const noexcept
    {
        return *this > parser::lit(rhs);
    }

    template<typename Parser, typename GlobalState, typename ErrorHandler>



    template<typename R, typename>

    constexpr auto
    parser_interface<Parser, GlobalState, ErrorHandler>::operator>(
        R && r) const noexcept
    {
        return *this > parser::lit(r);
    }




    template<typename Parser>
    constexpr auto operator>(char c, parser_interface<Parser> rhs) noexcept
    {
        if constexpr (detail::is_seq_p<Parser>{}) {
            return rhs.parser_.template prepend<false>(parser::lit(c));
        } else {
            return parser::lit(c) > rhs;
        }
    }


    template<typename Parser>
    constexpr auto operator>(char32_t c, parser_interface<Parser> rhs) noexcept
    {
        if constexpr (detail::is_seq_p<Parser>{}) {
            return rhs.parser_.template prepend<false>(parser::lit(c));
        } else {
            return parser::lit(c) > rhs;
        }
    }





    template<
        typename R,
        typename Parser,
        typename Enable = std::enable_if_t<detail::is_parsable_range_like_v<R>>>

    constexpr auto operator>(R && r, parser_interface<Parser> rhs) noexcept
    {
        if constexpr (detail::is_seq_p<Parser>{}) {
            return rhs.parser_.template prepend<false>(parser::lit(r));
        } else {
            return parser::lit(r) > rhs;
        }
    }



    template<typename Parser, typename GlobalState, typename ErrorHandler>
    constexpr auto
    parser_interface<Parser, GlobalState, ErrorHandler>::operator|(
        char rhs) const noexcept
    {
        return *this | parser::lit(rhs);
    }

    template<typename Parser, typename GlobalState, typename ErrorHandler>
    constexpr auto
    parser_interface<Parser, GlobalState, ErrorHandler>::operator|(
        char32_t rhs) const noexcept
    {
        return *this | parser::lit(rhs);
    }

    template<typename Parser, typename GlobalState, typename ErrorHandler>



    template<typename R, typename>

    constexpr auto
    parser_interface<Parser, GlobalState, ErrorHandler>::operator|(
        R && r) const noexcept
    {
        return *this | parser::lit(r);
    }




    template<typename Parser>
    constexpr auto operator|(char c, parser_interface<Parser> rhs) noexcept
    {
        if constexpr (detail::is_or_p<Parser>{}) {
            return rhs.parser_.prepend(parser::lit(c));
        } else {
            return parser::lit(c) | rhs;
        }
    }


    template<typename Parser>
    constexpr auto operator|(char32_t c, parser_interface<Parser> rhs) noexcept
    {
        if constexpr (detail::is_or_p<Parser>{}) {
            return rhs.parser_.prepend(parser::lit(c));
        } else {
            return parser::lit(c) | rhs;
        }
    }





    template<
        typename R,
        typename Parser,
        typename Enable = std::enable_if_t<detail::is_parsable_range_like_v<R>>>

    constexpr auto operator|(R && r, parser_interface<Parser> rhs) noexcept
    {
        if constexpr (detail::is_or_p<Parser>{}) {
            return rhs.parser_.prepend(parser::lit(r));
        } else {
            return parser::lit(r) | rhs;
        }
    }



    template<typename Parser, typename GlobalState, typename ErrorHandler>
    constexpr auto
    parser_interface<Parser, GlobalState, ErrorHandler>::operator-(
        char rhs) const noexcept
    {
        return !parser::lit(rhs) >> *this;
    }

    template<typename Parser, typename GlobalState, typename ErrorHandler>
    constexpr auto
    parser_interface<Parser, GlobalState, ErrorHandler>::operator-(
        char32_t rhs) const noexcept
    {
        return !parser::lit(rhs) >> *this;
    }

    template<typename Parser, typename GlobalState, typename ErrorHandler>



    template<typename R, typename>

    constexpr auto
    parser_interface<Parser, GlobalState, ErrorHandler>::operator-(
        R && r) const noexcept
    {
        return !parser::lit(r) >> *this;
    }




    template<typename Parser>
    constexpr auto operator-(char c, parser_interface<Parser> rhs) noexcept
    {
        return !rhs >> parser::lit(c);
    }


    template<typename Parser>
    constexpr auto operator-(char32_t c, parser_interface<Parser> rhs) noexcept
    {
        return !rhs >> parser::lit(c);
    }





    template<
        typename R,
        typename Parser,
        typename Enable = std::enable_if_t<detail::is_parsable_range_like_v<R>>>

    constexpr auto operator-(R && r, parser_interface<Parser> rhs) noexcept
    {
        return !rhs >> parser::lit(r);
    }



    template<typename Parser, typename GlobalState, typename ErrorHandler>
    constexpr auto
    parser_interface<Parser, GlobalState, ErrorHandler>::operator%(
        char rhs) const noexcept
    {
        return *this % parser::lit(rhs);
    }

    template<typename Parser, typename GlobalState, typename ErrorHandler>
    constexpr auto
    parser_interface<Parser, GlobalState, ErrorHandler>::operator%(
        char32_t rhs) const noexcept
    {
        return *this % parser::lit(rhs);
    }

    template<typename Parser, typename GlobalState, typename ErrorHandler>



    template<typename R, typename>

    constexpr auto
    parser_interface<Parser, GlobalState, ErrorHandler>::operator%(
        R && r) const noexcept
    {
        return *this % parser::lit(r);
    }




    template<typename Parser>
    constexpr auto operator%(char c, parser_interface<Parser> rhs) noexcept
    {
        return parser::lit(c) % rhs;
    }


    template<typename Parser>
    constexpr auto operator%(char32_t c, parser_interface<Parser> rhs) noexcept
    {
        return parser::lit(c) % rhs;
    }





    template<
        typename R,
        typename Parser,
        typename Enable = std::enable_if_t<detail::is_parsable_range_like_v<R>>>

    constexpr auto operator%(R && r, parser_interface<Parser> rhs) noexcept
    {
        return parser::lit(r) % rhs;
    }

}}
#line 2 "boost/parser/detail/printing_impl.hpp"
#define BOOST_PARSER_DETAIL_PRINTING_IMPL_HPP



#include <typeinfo>
#define BOOST_PARSER_HAVE_BOOST_TYPEINDEX 0


namespace boost { namespace parser { namespace detail {

    template<typename T>
    auto type_name()
    {



        return typeid(T).name();

    }

    template<typename Parser>
    struct n_aray_parser : std::false_type
    {};

    template<
        typename Parser,
        typename DelimiterParser,
        typename MinType,
        typename MaxType>
    struct n_aray_parser<
        repeat_parser<Parser, DelimiterParser, MinType, MaxType>>
        : std::true_type
    {};

    template<typename Parser, typename MinType, typename MaxType>
    struct n_aray_parser<repeat_parser<Parser, detail::nope, MinType, MaxType>>
        : std::false_type
    {};

    template<typename Parser, typename DelimiterParser>
    struct n_aray_parser<delimited_seq_parser<Parser, DelimiterParser>>
        : std::true_type
    {};

    template<typename ParserTuple>
    struct n_aray_parser<or_parser<ParserTuple>> : std::true_type
    {};

    template<
        typename ParserTuple,
        typename BacktrackingTuple,
        typename CombiningGroups>
    struct n_aray_parser<
        seq_parser<ParserTuple, BacktrackingTuple, CombiningGroups>>
        : std::true_type
    {};


    template<typename Parser>
    constexpr bool n_aray_parser_v = n_aray_parser<Parser>::value;

    template<typename Context, typename Expected>
    void print_expected(
        Context const & context,
        std::ostream & os,
        Expected expected,
        bool no_parens = false)
    {
        if (is_nope_v<Expected>)
            return;
        if (!no_parens)
            os << "(";
        detail::print(os, detail::resolve(context, expected));
        if (!no_parens)
            os << ")";
    }

    template<
        typename Context,
        typename Parser,
        typename DelimiterParser,
        typename MinType,
        typename MaxType>
    void print_parser(
        Context const & context,
        repeat_parser<Parser, DelimiterParser, MinType, MaxType> const & parser,
        std::ostream & os,
        int components)
    {
        if constexpr (is_nope_v<DelimiterParser>) {
            auto const min_ = detail::resolve(context, parser.min_);
            auto const max_ = detail::resolve(context, parser.max_);
            constexpr bool n_ary_child = n_aray_parser_v<Parser>;
            if (min_ == 0 && max_ == Inf) {
                os << "*";
                if (n_ary_child)
                    os << "(";
                detail::print_parser(
                    context, parser.parser_, os, components + 1);
                if (n_ary_child)
                    os << ")";
            } else if (min_ == 1 && max_ == Inf) {
                os << "+";
                if (n_ary_child)
                    os << "(";
                detail::print_parser(
                    context, parser.parser_, os, components + 1);
                if (n_ary_child)
                    os << ")";
            } else {
                os << "repeat(";
                detail::print(os, min_);
                if (min_ == max_) {
                    os << ")[";
                } else {
                    os << ", ";
                    if (max_ == unbounded)
                        os << "Inf";
                    else
                        detail::print(os, max_);
                    os << ")[";
                }
                detail::print_parser(
                    context, parser.parser_, os, components + 1);
                os << "]";
            }
        } else {
            detail::print_parser(context, parser.parser_, os, components + 1);
            os << " % ";
            detail::print_parser(
                context, parser.delimiter_parser_, os, components + 2);
        }
    }

    template<typename Context, typename Parser>
    void print_parser(
        Context const & context,
        opt_parser<Parser> const & parser,
        std::ostream & os,
        int components)
    {
        os << "-";
        constexpr bool n_ary_child = n_aray_parser_v<Parser>;
        if (n_ary_child)
            os << "(";
        detail::print_parser(context, parser.parser_, os, components + 1);
        if (n_ary_child)
            os << ")";
    }

    template<typename Context, typename ParserTuple>
    void print_parser(
        Context const & context,
        or_parser<ParserTuple> const & parser,
        std::ostream & os,
        int components)
    {
        int i = 0;
        bool printed_ellipsis = false;
        hl::for_each(parser.parsers_, [&](auto const & parser) {
            if (components == parser_component_limit) {
                if (!printed_ellipsis)
                    os << " | ...";
                printed_ellipsis = true;
                return;
            }
            if (i)
                os << " | ";
            detail::print_parser(context, parser, os, components);
            ++components;
            ++i;
        });
    }

    template<
        typename Context,
        typename ParserTuple,
        typename BacktrackingTuple,
        typename CombiningGroups>
    void print_parser(
        Context const & context,
        seq_parser<ParserTuple, BacktrackingTuple, CombiningGroups> const &
            parser,
        std::ostream & os,
        int components)
    {
        int prev_group = 0;
        int i = 0;
        bool printed_ellipsis = false;
        using combining_groups =
            detail::combining_t<ParserTuple, CombiningGroups>;
        hl::for_each(
            hl::zip(parser.parsers_, BacktrackingTuple{}, combining_groups{}),
            [&](auto const & parser_and_backtrack) {
                using namespace literals;
                auto const & parser = parser::get(parser_and_backtrack, 0_c);
                auto const backtrack = parser::get(parser_and_backtrack, 1_c);
                auto const group = parser::get(parser_and_backtrack, 2_c);

                if (components == parser_component_limit) {
                    if (!printed_ellipsis) {
                        os << (backtrack ? " >> ..." : " > ...");
                    }
                    printed_ellipsis = true;
                    return;
                }
                if (group != prev_group && prev_group)
                    os << ']';
                if (i)
                    os << (backtrack ? " >> " : " > ");
                if (group != prev_group && group)
                    os << (group == -1 ? "separate[" : "merge[");
                detail::print_parser(context, parser, os, components);
                ++components;
                ++i;
                prev_group = group;
            });
        if (prev_group && !printed_ellipsis)
            os << ']';
    }

    template<typename Context, typename Parser, typename Action>
    void print_parser(
        Context const & context,
        action_parser<Parser, Action> const & parser,
        std::ostream & os,
        int components)
    {
        detail::print_parser(context, parser.parser_, os, components);
        os << "[<<action>>]";
    }

    template<typename Context, typename Parser>
    void print_directive(
        Context const & context,
        std::string_view name,
        Parser const & parser,
        std::ostream & os,
        int components)
    {
        os << name << "[";
        if (++components == parser_component_limit)
            os << "...";
        else
            detail::print_parser(context, parser, os, components + 1);
        os << "]";
    }

    template<typename Context, typename Parser>
    void print_parser(
        Context const & context,
        omit_parser<Parser> const & parser,
        std::ostream & os,
        int components)
    {
        detail::print_directive(
            context, "omit", parser.parser_, os, components);
    }

    template<typename Context, typename Parser>
    void print_parser(
        Context const & context,
        raw_parser<Parser> const & parser,
        std::ostream & os,
        int components)
    {
        detail::print_directive(context, "raw", parser.parser_, os, components);
    }
#line 284 "boost/parser/detail/printing_impl.hpp"
    template<typename Context, typename Parser>
    void print_parser(
        Context const & context,
        lexeme_parser<Parser> const & parser,
        std::ostream & os,
        int components)
    {
        detail::print_directive(
            context, "lexeme", parser.parser_, os, components);
    }

    template<typename Context, typename Parser>
    void print_parser(
        Context const & context,
        no_case_parser<Parser> const & parser,
        std::ostream & os,
        int components)
    {
        detail::print_directive(
            context, "no_case", parser.parser_, os, components);
    }

    template<typename Context, typename Parser, typename SkipParser>
    void print_parser(
        Context const & context,
        skip_parser<Parser, SkipParser> const & parser,
        std::ostream & os,
        int components)
    {
        if constexpr (is_nope_v<SkipParser>) {
            detail::print_directive(
                context, "skip", parser.parser_, os, components);
        } else {
            os << "skip(";
            detail::print_parser(
                context, parser.skip_parser_.parser_, os, components);
            os << ")";
            detail::print_directive(
                context, "", parser.parser_, os, components + 1);
        }
    }

    template<typename Context, typename Parser, bool FailOnMatch>
    void print_parser(
        Context const & context,
        expect_parser<Parser, FailOnMatch> const & parser,
        std::ostream & os,
        int components)
    {
        if (FailOnMatch)
            os << "!";
        else
            os << "&";
        constexpr bool n_ary_child = n_aray_parser_v<Parser>;
        if (n_ary_child)
            os << "(";
        detail::print_parser(context, parser.parser_, os, components + 1);
        if (n_ary_child)
            os << ")";
    }

    template<
        typename Context,
        bool UseCallbacks,
        typename Parser,
        typename Attribute,
        typename LocalState,
        typename ParamsTuple>
    void print_parser(
        Context const & context,
        rule_parser<
            UseCallbacks,
            Parser,
            Attribute,
            LocalState,
            ParamsTuple> const & parser,
        std::ostream & os,
        int components)
    {
        os << parser.diagnostic_text_;
        if constexpr (!is_nope_v<ParamsTuple>) {
            os << ".with(";
            int i = 0;
            hl::for_each(parser.params_, [&](auto const & param) {
                if (i++)
                    os << ", ";
                detail::print_expected(context, os, param, true);
            });
            os << ")";
        }
    }

    template<typename Context, typename T>
    void print_parser(
        Context const & context,
        symbol_parser<T> const & parser,
        std::ostream & os,
        int components)
    {
        os << "symbols<" << detail::type_name<T>() << ">";
    }

    template<typename Context, typename Predicate>
    void print_parser(
        Context const & context,
        eps_parser<Predicate> const & parser,
        std::ostream & os,
        int components)
    {
        os << "eps(<<pred>>)";
    }

    template<typename Context>
    void print_parser(
        Context const & context,
        eps_parser<nope> const & parser,
        std::ostream & os,
        int components)
    {
        os << "eps";
    }

    template<typename Context>
    void print_parser(
        Context const & context,
        eoi_parser const & parser,
        std::ostream & os,
        int components)
    {
        os << "eoi";
    }

    template<typename Context, typename Atribute>
    void print_parser(
        Context const & context,
        attr_parser<Atribute> const & parser,
        std::ostream & os,
        int components)
    {
        os << "attr";
        detail::print_expected(context, os, parser.attr_);
    }

    template<
        typename Context,
        typename ResolvedExpected,
        bool Integral = std::is_integral<ResolvedExpected>{},
        int SizeofExpected = sizeof(ResolvedExpected)>
    struct print_expected_char_impl
    {
        static void call(
            Context const & context,
            std::ostream & os,
            ResolvedExpected expected)
        {
            detail::print(os, expected);
        }
    };

    template<typename Context, typename Expected>
    struct print_expected_char_impl<Context, Expected, true, 4>
    {
        static void
        call(Context const & context, std::ostream & os, Expected expected)
        {
            std::array<char32_t, 1> cps = {{(char32_t)expected}};
            auto const r = cps | text::as_utf8;
            os << "'";
            for (auto c : r) {
                detail::print_char(os, c);
            }
            os << "'";
        }
    };

    template<typename Context, typename Expected>
    void print_expected_char(
        Context const & context, std::ostream & os, Expected expected)
    {
        auto resolved_expected = detail::resolve(context, expected);
        detail::print_expected_char_impl<Context, decltype(resolved_expected)>::
            call(context, os, resolved_expected);
    }

    template<typename Context, typename T>
    struct char_print_parser_impl
    {
        static void call(Context const & context, std::ostream & os, T expected)
        {
            detail::print_expected_char(context, os, expected);
        }
    };

    template<typename Context, typename T, typename U>
    struct char_print_parser_impl<Context, char_pair<T, U>>
    {
        static void call(
            Context const & context,
            std::ostream & os,
            char_pair<T, U> expected)
        {
            detail::print_expected_char(context, os, expected.lo_);
            os << ", ";
            detail::print_expected_char(context, os, expected.hi_);
        }
    };

    template<typename Context, typename Iter, typename Sentinel, bool B>
    struct char_print_parser_impl<Context, char_range<Iter, Sentinel, B>>
    {
        static void call(
            Context const & context,
            std::ostream & os,
            char_range<Iter, Sentinel, B> expected)
        {
            os << "\"";
            auto const r = expected.chars_ | text::as_utf8;
            for (auto c : r) {
                detail::print_char(os, c);
            }
            os << "\"";
        }
    };

    template<typename Context, typename Expected, typename AttributeType>
    void print_parser(
        Context const & context,
        char_parser<Expected, AttributeType> const & parser,
        std::ostream & os,
        int components)
    {
        if (std::is_same_v<AttributeType, uint32_t>)
            os << "cp";
        else if (std::is_same_v<AttributeType, char>)
            os << "cu";
        else
            os << "char_";
        if constexpr (!is_nope_v<Expected>) {
            os << "(";
            char_print_parser_impl<Context, Expected>::call(
                context, os, parser.expected_);
            os << ")";
        }
    }

    template<typename Context>
    void print_parser(
        Context const & context,
        digit_parser const & parser,
        std::ostream & os,
        int components)
    {
        os << "digit";
    }

    template<typename Context>
    void print_parser(
        Context const & context,
        char_subrange_parser<hex_digit_subranges> const & parser,
        std::ostream & os,
        int components)
    {
        os << "hex_digit";
    }

    template<typename Context>
    void print_parser(
        Context const & context,
        char_subrange_parser<control_subranges> const & parser,
        std::ostream & os,
        int components)
    {
        os << "control";
    }

    template<typename Context>
    void print_parser(
        Context const & context,
        char_set_parser<punct_chars> const & parser,
        std::ostream & os,
        int components)
    {
        os << "punct";
    }

    template<typename Context>
    void print_parser(
        Context const & context,
        char_set_parser<lower_case_chars> const & parser,
        std::ostream & os,
        int components)
    {
        os << "lower";
    }

    template<typename Context>
    void print_parser(
        Context const & context,
        char_set_parser<upper_case_chars> const & parser,
        std::ostream & os,
        int components)
    {
        os << "upper";
    }

    template<typename Context, typename Expected, typename AttributeType>
    void print_parser(
        Context const & context,
        omit_parser<char_parser<Expected, AttributeType>> const & parser,
        std::ostream & os,
        int components)
    {
        if constexpr (is_nope_v<Expected>) {
            os << "omit[char_]";
        } else {
            char_print_parser_impl<Context, Expected>::call(
                context, os, parser.parser_.expected_);
        }
    }

    template<typename Context, typename StrIter, typename StrSentinel>
    void print_parser(
        Context const & context,
        string_parser<StrIter, StrSentinel> const & parser,
        std::ostream & os,
        int components)
    {
        os << "string(\"";
        for (auto c : boost::parser::subrange(
                          parser.expected_first_, parser.expected_last_) |
                          text::as_utf8) {
            detail::print_char(os, c);
        }
        os << "\")";
    }

    template<typename Context, typename StrIter, typename StrSentinel>
    void print_parser(
        Context const & context,
        omit_parser<string_parser<StrIter, StrSentinel>> const & parser,
        std::ostream & os,
        int components)
    {
        os << "\"";
        for (auto c : boost::parser::subrange(
                          parser.parser_.expected_first_,
                          parser.parser_.expected_last_) |
                          text::as_utf8) {
            detail::print_char(os, c);
        }
        os << "\"";
    }

    template<typename Context, bool NewlinesOnly, bool NoNewlines>
    void print_parser(
        Context const & context,
        ws_parser<NewlinesOnly, NoNewlines> const & parser,
        std::ostream & os,
        int components)
    {
        if constexpr (NoNewlines)
            os << "blank";
        else if constexpr (NewlinesOnly)
            os << "eol";
        else
            os << "ws";
    }

    template<typename Context>
    void print_parser(
        Context const & context,
        bool_parser const & parser,
        std::ostream & os,
        int components)
    {
        os << "bool_";
    }

    template<
        typename Context,
        typename T,
        int Radix,
        int MinDigits,
        int MaxDigits,
        typename Expected>
    void print_parser(
        Context const & context,
        uint_parser<T, Radix, MinDigits, MaxDigits, Expected> const & parser,
        std::ostream & os,
        int components)
    {
        if (MinDigits == 1 && MaxDigits == -1) {
            if (std::is_same_v<T, unsigned short>) {
                os << "ushort_";
                detail::print_expected(context, os, parser.expected_);
                return;
            } else if (std::is_same_v<T, unsigned int>) {
                if (Radix == 2)
                    os << "bin";
                else if (Radix == 8)
                    os << "oct";
                else if (Radix == 16)
                    os << "hex";
                else if (Radix == 10)
                    os << "uint_";
                detail::print_expected(context, os, parser.expected_);
                return;
            } else if (Radix == 10 && std::is_same_v<T, unsigned long>) {
                os << "ulong_";
                detail::print_expected(context, os, parser.expected_);
                return;
            } else if (Radix == 10 && std::is_same_v<T, unsigned long long>) {
                os << "ulong_long";
                detail::print_expected(context, os, parser.expected_);
                return;
            }
        }
        os << "uint<" << detail::type_name<T>() << ", " << Radix << ", "
           << MinDigits << ", " << MaxDigits << ">";
        detail::print_expected(context, os, parser.expected_);
    }

    template<
        typename Context,
        typename T,
        int Radix,
        int MinDigits,
        int MaxDigits,
        typename Expected>
    void print_parser(
        Context const & context,
        int_parser<T, Radix, MinDigits, MaxDigits, Expected> const & parser,
        std::ostream & os,
        int components)
    {
        if (Radix == 10 && MinDigits == 1 && MaxDigits == -1) {
            if (std::is_same_v<T, short>) {
                os << "short_";
                detail::print_expected(context, os, parser.expected_);
                return;
            } else if (std::is_same_v<T, int>) {
                os << "int_";
                detail::print_expected(context, os, parser.expected_);
                return;
            } else if (std::is_same_v<T, long>) {
                os << "long_";
                detail::print_expected(context, os, parser.expected_);
                return;
            } else if (std::is_same_v<T, long long>) {
                os << "long_long";
                detail::print_expected(context, os, parser.expected_);
                return;
            }
        }
        os << "int<" << detail::type_name<T>() << ", " << Radix << ", "
           << MinDigits << ", " << MaxDigits << ">";
        detail::print_expected(context, os, parser.expected_);
    }

    template<typename Context>
    void print_parser(
        Context const & context,
        int_parser<short> const & parser,
        std::ostream & os,
        int components)
    {
        os << "short_";
    }

    template<typename Context>
    void print_parser(
        Context const & context,
        int_parser<long> const & parser,
        std::ostream & os,
        int components)
    {
        os << "long_";
    }

    template<typename Context>
    void print_parser(
        Context const & context,
        int_parser<long long> const & parser,
        std::ostream & os,
        int components)
    {
        os << "long_long";
    }

    template<typename Context, typename T>
    void print_parser(
        Context const & context,
        float_parser<T> const & parser,
        std::ostream & os,
        int components)
    {
        os << "float<" << detail::type_name<T>() << ">";
    }

    template<typename Context>
    void print_parser(
        Context const & context,
        float_parser<float> const & parser,
        std::ostream & os,
        int components)
    {
        os << "float_";
    }

    template<typename Context>
    void print_parser(
        Context const & context,
        float_parser<double> const & parser,
        std::ostream & os,
        int components)
    {
        os << "double_";
    }

    template<
        typename Context,
        typename ParserTuple,
        typename BacktrackingTuple,
        typename CombiningGroups>
    void print_switch_matchers(
        Context const & context,
        seq_parser<ParserTuple, BacktrackingTuple, CombiningGroups> const &
            parser,
        std::ostream & os,
        int components)
    {
        using namespace literals;

        os << "("
           << detail::resolve(
                  context, parser::get(parser.parsers_, 0_c).pred_.value_)
           << ", ";
        detail::print_parser(
            context, parser::get(parser.parsers_, 1_c), os, components);
        os << ")";
    }

    template<typename Context, typename ParserTuple>
    void print_switch_matchers(
        Context const & context,
        or_parser<ParserTuple> const & parser,
        std::ostream & os,
        int components)
    {
        using namespace literals;

        bool printed_ellipsis = false;
        hl::for_each(parser.parsers_, [&](auto const & parser) {
            if (components == parser_component_limit) {
                if (!printed_ellipsis)
                    os << "...";
                printed_ellipsis = true;
                return;
            }
            detail::print_switch_matchers(context, parser, os, components);
            ++components;
        });
    }

    template<typename Context, typename SwitchValue, typename OrParser>
    void print_parser(
        Context const & context,
        switch_parser<SwitchValue, OrParser> const & parser,
        std::ostream & os,
        int components)
    {
        os << "switch_(";
        detail::print(os, detail::resolve(context, parser.switch_value_));
        os << ")";
        detail::print_switch_matchers(
            context, parser.or_parser_, os, components);
    }

}}}
#line 7508 "boost/parser/parser.hpp"
namespace boost { namespace parser {



    enum class trace { off, on };
#line 7529 "boost/parser/parser.hpp"
    template<
        typename I,
        typename S,
        typename Parser,
        typename GlobalState,
        typename ErrorHandler,
        typename Attr,
        typename Enable = std::enable_if_t<
            detail::is_parsable_iter_v<I> &&
            detail::is_equality_comparable_with_v<I, S>>>

    bool prefix_parse(
        I & first,
        S last,
        parser_interface<Parser, GlobalState, ErrorHandler> const & parser,
        Attr & attr,
        trace trace_mode = trace::off)
    {
        detail::attr_reset reset(attr);
        if constexpr (!detail::is_char8_iter_v<I>) {
            static_assert(
                decltype(detail::has_attribute(first, last, parser)){},
                "If you're seeing this error, you're trying to get parse() to "
                "fill in attr above, using the attribute generated by parser. "
                "However, parser does not generate an attribute.");
            if (trace_mode == trace::on) {
                return reset = detail::parse_impl<true>(
                           first, last, parser, parser.error_handler_, attr);
            } else {
                return reset = detail::parse_impl<false>(
                           first, last, parser, parser.error_handler_, attr);
            }
        } else {
            auto r =
                boost::parser::subrange(first, last) | detail::text::as_utf32;
            auto f = r.begin();
            auto const l = r.end();
            auto _ = detail::scoped_base_assign(first, f);
            static_assert(
                decltype(detail::has_attribute(f, l, parser)){},
                "If you're seeing this error, you're trying to get parse() to "
                "fill in attr above, using the attribute generated by parser. "
                "However, parser does not generate an attribute.");
            if (trace_mode == trace::on) {
                return reset = detail::parse_impl<true>(
                           f, l, parser, parser.error_handler_, attr);
            } else {
                return reset = detail::parse_impl<false>(
                           f, l, parser, parser.error_handler_, attr);
            }
        }
    }
#line 7596 "boost/parser/parser.hpp"
    template<
        typename R,
        typename Parser,
        typename GlobalState,
        typename ErrorHandler,
        typename Attr,
        typename Enable =
            std::enable_if_t<detail::is_parsable_range_like_v<R>>>

    bool parse(
        R const & r,
        parser_interface<Parser, GlobalState, ErrorHandler> const & parser,
        Attr & attr,
        trace trace_mode = trace::off)
#line 7619 "boost/parser/parser.hpp"
    {
        detail::attr_reset reset(attr);
        auto r_ = detail::make_input_subrange(r);
        auto first = r_.begin();
        auto const last = r_.end();
        return reset = detail::if_full_parse(
                   first,
                   last,
                   parser::prefix_parse(first, last, parser, attr, trace_mode));
    }
#line 7642 "boost/parser/parser.hpp"
    template<
        typename I,
        typename S,
        typename Parser,
        typename GlobalState,
        typename ErrorHandler,
        typename Enable = std::enable_if_t<
            detail::is_parsable_iter_v<I> &&
            detail::is_equality_comparable_with_v<I, S>>>

    auto prefix_parse(
        I & first,
        S last,
        parser_interface<Parser, GlobalState, ErrorHandler> const & parser,
        trace trace_mode = trace::off)
    {
        if constexpr (!detail::is_char8_iter_v<I>) {
            if (trace_mode == trace::on) {
                return detail::parse_impl<true>(
                    first, last, parser, parser.error_handler_);
            } else {
                return detail::parse_impl<false>(
                    first, last, parser, parser.error_handler_);
            }
        } else {
            auto r =
                boost::parser::subrange(first, last) | detail::text::as_utf32;
            auto f = r.begin();
            auto const l = r.end();
            auto _ = detail::scoped_base_assign(first, f);
            if (trace_mode == trace::on) {
                return detail::parse_impl<true>(
                    f, l, parser, parser.error_handler_);
            } else {
                return detail::parse_impl<false>(
                    f, l, parser, parser.error_handler_);
            }
        }
    }
#line 7694 "boost/parser/parser.hpp"
    template<
        typename R,
        typename Parser,
        typename GlobalState,
        typename ErrorHandler,
        typename Enable = std::enable_if_t<detail::is_parsable_range_like_v<R>>>

    auto parse(
        R const & r,
        parser_interface<Parser, GlobalState, ErrorHandler> const & parser,
        trace trace_mode = trace::off)
#line 7714 "boost/parser/parser.hpp"
    {
        auto r_ = detail::make_input_subrange(r);
        auto first = r_.begin();
        auto const last = r_.end();
        return detail::if_full_parse(
            first, last, parser::prefix_parse(first, last, parser, trace_mode));
    }
#line 7738 "boost/parser/parser.hpp"
    template<
        typename I,
        typename S,
        typename Parser,
        typename GlobalState,
        typename ErrorHandler,
        typename SkipParser,
        typename Attr,
        typename Enable = std::enable_if_t<
            detail::is_parsable_iter_v<I> &&
            detail::is_equality_comparable_with_v<I, S>>>

    bool prefix_parse(
        I & first,
        S last,
        parser_interface<Parser, GlobalState, ErrorHandler> const & parser,
        parser_interface<SkipParser> const & skip,
        Attr & attr,
        trace trace_mode = trace::off)
    {
        detail::attr_reset reset(attr);
        if constexpr (!detail::is_char8_iter_v<I>) {
            static_assert(
                decltype(detail::has_attribute(first, last, parser)){},
                "If you're seeing this error, you're trying to get parse() to "
                "fill in attr above, using the attribute generated by parser. "
                "However, parser does not generate an attribute.");
            if (trace_mode == trace::on) {
                return reset = detail::skip_parse_impl<true>(
                           first,
                           last,
                           parser,
                           skip,
                           parser.error_handler_,
                           attr);
            } else {
                return reset = detail::skip_parse_impl<false>(
                           first,
                           last,
                           parser,
                           skip,
                           parser.error_handler_,
                           attr);
            }
        } else {
            auto r =
                boost::parser::subrange(first, last) | detail::text::as_utf32;
            auto f = r.begin();
            auto const l = r.end();
            auto _ = detail::scoped_base_assign(first, f);
            static_assert(
                decltype(detail::has_attribute(f, l, parser)){},
                "If you're seeing this error, you're trying to get parse() to "
                "fill in attr above, using the attribute generated by parser. "
                "However, parser does not generate an attribute.");
            if (trace_mode == trace::on) {
                return reset = detail::skip_parse_impl<true>(
                           f, l, parser, skip, parser.error_handler_, attr);
            } else {
                return reset = detail::skip_parse_impl<false>(
                           f, l, parser, skip, parser.error_handler_, attr);
            }
        }
    }
#line 7819 "boost/parser/parser.hpp"
    template<
        typename R,
        typename Parser,
        typename GlobalState,
        typename ErrorHandler,
        typename SkipParser,
        typename Attr,
        typename Enable = std::enable_if_t<detail::is_parsable_range_like_v<R>>>

    bool parse(
        R const & r,
        parser_interface<Parser, GlobalState, ErrorHandler> const & parser,
        parser_interface<SkipParser> const & skip,
        Attr & attr,
        trace trace_mode = trace::off)
#line 7843 "boost/parser/parser.hpp"
    {
        detail::attr_reset reset(attr);
        auto r_ = detail::make_input_subrange(r);
        auto first = r_.begin();
        auto const last = r_.end();
        return reset = detail::if_full_parse(
                   first,
                   last,
                   parser::prefix_parse(
                       first, last, parser, skip, attr, trace_mode));
    }
#line 7870 "boost/parser/parser.hpp"
    template<
        typename I,
        typename S,
        typename Parser,
        typename GlobalState,
        typename ErrorHandler,
        typename SkipParser,
        typename Enable = std::enable_if_t<
            detail::is_parsable_iter_v<I> &&
            detail::is_equality_comparable_with_v<I, S>>>

    auto prefix_parse(
        I & first,
        S last,
        parser_interface<Parser, GlobalState, ErrorHandler> const & parser,
        parser_interface<SkipParser> const & skip,
        trace trace_mode = trace::off)
    {
        if constexpr (!detail::is_char8_iter_v<I>) {
            if (trace_mode == trace::on) {
                return detail::skip_parse_impl<true>(
                    first, last, parser, skip, parser.error_handler_);
            } else {
                return detail::skip_parse_impl<false>(
                    first, last, parser, skip, parser.error_handler_);
            }
        } else {
            auto r =
                boost::parser::subrange(first, last) | detail::text::as_utf32;
            auto f = r.begin();
            auto const l = r.end();
            auto _ = detail::scoped_base_assign(first, f);
            if (trace_mode == trace::on) {
                return detail::skip_parse_impl<true>(
                    f, l, parser, skip, parser.error_handler_);
            } else {
                return detail::skip_parse_impl<false>(
                    f, l, parser, skip, parser.error_handler_);
            }
        }
    }
#line 7926 "boost/parser/parser.hpp"
    template<
        typename I,
        typename S,
        typename Parser,
        typename GlobalState,
        typename ErrorHandler,
        typename TagType,
        typename Attribute,
        typename LocalState,
        typename ParamsTuple,
        typename Enable = std::enable_if_t<
            detail::is_parsable_iter_v<I> &&
            detail::is_equality_comparable_with_v<I, S>>>

    auto prefix_parse(
        I & first,
        S last,
        parser_interface<Parser, GlobalState, ErrorHandler> const & parser,
        rule<TagType, Attribute, LocalState, ParamsTuple> const & skip,
        trace trace_mode = trace::off)
    {
        if constexpr (!detail::is_char8_iter_v<I>) {
            if (trace_mode == trace::on) {
                return detail::skip_parse_impl<true>(
                    first, last, parser, skip, parser.error_handler_);
            } else {
                return detail::skip_parse_impl<false>(
                    first, last, parser, skip, parser.error_handler_);
            }
        } else {
            auto r =
                boost::parser::subrange(first, last) | detail::text::as_utf32;
            auto f = r.begin();
            auto const l = r.end();
            auto _ = detail::scoped_base_assign(first, f);
            if (trace_mode == trace::on) {
                return detail::skip_parse_impl<true>(
                    f, l, parser, skip, parser.error_handler_);
            } else {
                return detail::skip_parse_impl<false>(
                    f, l, parser, skip, parser.error_handler_);
            }
        }
    }
#line 7983 "boost/parser/parser.hpp"
    template<
        typename I,
        typename S,
        typename Parser,
        typename GlobalState,
        typename ErrorHandler,
        typename TagType,
        typename Attribute,
        typename LocalState,
        typename ParamsTuple,
        typename Enable = std::enable_if_t<
            detail::is_parsable_iter_v<I> &&
            detail::is_equality_comparable_with_v<I, S>>>

    auto prefix_parse(
        I & first,
        S last,
        parser_interface<Parser, GlobalState, ErrorHandler> const & parser,
        callback_rule<TagType, Attribute, LocalState, ParamsTuple> const & skip,
        trace trace_mode = trace::off)
    {
        if constexpr (!detail::is_char8_iter_v<I>) {
            if (trace_mode == trace::on) {
                return detail::skip_parse_impl<true>(
                    first, last, parser, skip, parser.error_handler_);
            } else {
                return detail::skip_parse_impl<false>(
                    first, last, parser, skip, parser.error_handler_);
            }
        } else {
            auto r =
                boost::parser::subrange(first, last) | detail::text::as_utf32;
            auto f = r.begin();
            auto const l = r.end();
            auto _ = detail::scoped_base_assign(first, f);
            if (trace_mode == trace::on) {
                return detail::skip_parse_impl<true>(
                    f, l, parser, skip, parser.error_handler_);
            } else {
                return detail::skip_parse_impl<false>(
                    f, l, parser, skip, parser.error_handler_);
            }
        }
    }
#line 8045 "boost/parser/parser.hpp"
    template<
        typename R,
        typename Parser,
        typename GlobalState,
        typename ErrorHandler,
        typename SkipParser,
        typename Enable = std::enable_if_t<detail::is_parsable_range_like_v<R>>>

    auto parse(
        R const & r,
        parser_interface<Parser, GlobalState, ErrorHandler> const & parser,
        parser_interface<SkipParser> const & skip,
        trace trace_mode = trace::off)
#line 8067 "boost/parser/parser.hpp"
    {
        auto r_ = detail::make_input_subrange(r);
        auto first = r_.begin();
        auto const last = r_.end();
        return detail::if_full_parse(
            first,
            last,
            parser::prefix_parse(first, last, parser, skip, trace_mode));
    }
#line 8090 "boost/parser/parser.hpp"
    template<
        typename R,
        typename Parser,
        typename GlobalState,
        typename ErrorHandler,
        typename TagType,
        typename Attribute,
        typename LocalState,
        typename ParamsTuple,
        typename Enable = std::enable_if_t<detail::is_parsable_range_like_v<R>>>

    auto parse(
        R const & r,
        parser_interface<Parser, GlobalState, ErrorHandler> const & parser,
        rule<TagType, Attribute, LocalState, ParamsTuple> const & skip,
        trace trace_mode = trace::off)
#line 8115 "boost/parser/parser.hpp"
    {
        auto r_ = detail::make_input_subrange(r);
        auto first = r_.begin();
        auto const last = r_.end();
        return detail::if_full_parse(
            first,
            last,
            parser::prefix_parse(first, last, parser, skip, trace_mode));
    }
#line 8136 "boost/parser/parser.hpp"
    template<
        typename R,
        typename Parser,
        typename GlobalState,
        typename ErrorHandler,
        typename TagType,
        typename Attribute,
        typename LocalState,
        typename ParamsTuple,
        typename Enable = std::enable_if_t<detail::is_parsable_range_like_v<R>>>

    auto parse(
        R const & r,
        parser_interface<Parser, GlobalState, ErrorHandler> const & parser,
        callback_rule<TagType, Attribute, LocalState, ParamsTuple> const & skip,
        trace trace_mode = trace::off)
#line 8161 "boost/parser/parser.hpp"
    {
        auto r_ = detail::make_input_subrange(r);
        auto first = r_.begin();
        auto const last = r_.end();
        return detail::if_full_parse(
            first,
            last,
            parser::prefix_parse(first, last, parser, skip, trace_mode));
    }
#line 8192 "boost/parser/parser.hpp"
    template<
        typename I,
        typename S,
        typename Parser,
        typename GlobalState,
        typename ErrorHandler,
        typename Callbacks,
        typename Enable = std::enable_if_t<
            detail::is_parsable_iter_v<I> &&
            detail::is_equality_comparable_with_v<I, S>>>

    bool callback_prefix_parse(
        I & first,
        S last,
        parser_interface<Parser, GlobalState, ErrorHandler> const & parser,
        Callbacks const & callbacks,
        trace trace_mode = trace::off)
    {
        if constexpr (!detail::is_char8_iter_v<I>) {
            if (trace_mode == trace::on) {
                return detail::callback_parse_impl<true>(
                    first, last, parser, parser.error_handler_, callbacks);
            } else {
                return detail::callback_parse_impl<false>(
                    first, last, parser, parser.error_handler_, callbacks);
            }
        } else {
            auto r =
                boost::parser::subrange(first, last) | detail::text::as_utf32;
            auto f = r.begin();
            auto const l = r.end();
            auto _ = detail::scoped_base_assign(first, f);
            if (trace_mode == trace::on) {
                return detail::callback_parse_impl<true>(
                    f, l, parser, parser.error_handler_, callbacks);
            } else {
                return detail::callback_parse_impl<false>(
                    f, l, parser, parser.error_handler_, callbacks);
            }
        }
    }
#line 8253 "boost/parser/parser.hpp"
    template<
        typename R,
        typename Parser,
        typename GlobalState,
        typename ErrorHandler,
        typename Callbacks,
        typename Enable = std::enable_if_t<detail::is_parsable_range_like_v<R>>>

    bool callback_parse(
        R const & r,
        parser_interface<Parser, GlobalState, ErrorHandler> const & parser,
        Callbacks const & callbacks,
        trace trace_mode = trace::off)
#line 8275 "boost/parser/parser.hpp"
    {
        auto r_ = detail::make_input_subrange(r);
        auto first = r_.begin();
        auto const last = r_.end();
        return detail::if_full_parse(
            first,
            last,
            parser::callback_prefix_parse(first, last, parser, callbacks));
    }
#line 8306 "boost/parser/parser.hpp"
    template<
        typename I,
        typename S,
        typename Parser,
        typename GlobalState,
        typename ErrorHandler,
        typename SkipParser,
        typename Callbacks,
        typename Enable = std::enable_if_t<
            detail::is_parsable_iter_v<I> &&
            detail::is_equality_comparable_with_v<I, S>>>

    bool callback_prefix_parse(
        I & first,
        S last,
        parser_interface<Parser, GlobalState, ErrorHandler> const & parser,
        parser_interface<SkipParser> const & skip,
        Callbacks const & callbacks,
        trace trace_mode = trace::off)
    {
        if constexpr (!detail::is_char8_iter_v<I>) {
            if (trace_mode == trace::on) {
                return detail::callback_skip_parse_impl<true>(
                    first,
                    last,
                    parser,
                    skip,
                    parser.error_handler_,
                    callbacks);
            } else {
                return detail::callback_skip_parse_impl<false>(
                    first,
                    last,
                    parser,
                    skip,
                    parser.error_handler_,
                    callbacks);
            }
        } else {
            auto r =
                boost::parser::subrange(first, last) | detail::text::as_utf32;
            auto f = r.begin();
            auto const l = r.end();
            auto _ = detail::scoped_base_assign(first, f);
            if (trace_mode == trace::on) {
                return detail::callback_skip_parse_impl<true>(
                    f, l, parser, skip, parser.error_handler_, callbacks);
            } else {
                return detail::callback_skip_parse_impl<false>(
                    f, l, parser, skip, parser.error_handler_, callbacks);
            }
        }
    }
#line 8381 "boost/parser/parser.hpp"
    template<
        typename R,
        typename Parser,
        typename GlobalState,
        typename ErrorHandler,
        typename SkipParser,
        typename Callbacks,
        typename Enable = std::enable_if_t<detail::is_parsable_range_like_v<R>>>

    bool callback_parse(
        R const & r,
        parser_interface<Parser, GlobalState, ErrorHandler> const & parser,
        parser_interface<SkipParser> const & skip,
        Callbacks const & callbacks,
        trace trace_mode = trace::off)
#line 8405 "boost/parser/parser.hpp"
    {
        auto r_ = detail::make_input_subrange(r);
        auto first = r_.begin();
        auto const last = r_.end();
        return detail::if_full_parse(
            first,
            last,
            parser::callback_prefix_parse(
                first, last, parser, skip, callbacks, trace_mode));
    }

    namespace literals {

        constexpr auto operator""_l(char c) { return parser::lit(c); }





        constexpr auto operator""_l(char32_t c) { return parser::lit(c); }

        constexpr auto operator""_l(char const * str, std::size_t)
        {
            return parser::lit(str);
        }
#line 8438 "boost/parser/parser.hpp"
        constexpr auto operator""_l(char32_t const * str, std::size_t)
        {
            return parser::lit(str);
        }


        constexpr auto operator""_p(char c) { return char_(c); }





        constexpr auto operator""_p(char32_t c) { return char_(c); }

        constexpr auto operator""_p(char const * str, std::size_t)
        {
            return parser::string(str);
        }
#line 8464 "boost/parser/parser.hpp"
        constexpr auto operator""_p(char32_t const * str, std::size_t)
        {
            return parser::string(str);
        }
    }

    namespace detail {
        template<typename... Args>
        constexpr void static_assert_merge_attributes(tuple<Args...> parsers)
        {



            using context_t = parse_context<
                char const *,
                char const *,
                default_error_handler>;
            using skipper_t = parser_interface<ws_parser<false, false>>;
            using use_parser_t = dummy_use_parser_t<
                false,
                char const *,
                char const *,
                context_t,
                skipper_t> const;
            using all_types =
                decltype(hl::transform(parsers, std::declval<use_parser_t>()));
            auto all_types_wrapped = hl::transform(all_types{}, detail::wrap{});
            auto first_non_nope = hl::fold_left(
                all_types_wrapped,
                wrapper<nope>{},
                [=](auto result, auto type) {
                    if constexpr (is_nope_v<typename decltype(result)::type>) {
                        return type;
                    } else {
                        return result;
                    }
                });
            using first_t = typename decltype(first_non_nope)::type;
            static_assert(
                !is_nope_v<first_t>,
                "It looks like you wrote merge[p1 >> p2 >> ... pn], and none "
                "of the parsers p1, p2, ... pn produces an attribute.  Please "
                "fix.");
            if constexpr (is_nope_v<first_t>) {
                [[maybe_unused]] detail::print_type<tuple<Args...>> tuple_types;
                [[maybe_unused]] detail::print_type<all_types> attribute_types;
            }
            hl::for_each(all_types_wrapped, [=](auto type) {
                using t = typename decltype(type)::type;
                if constexpr (!is_nope_v<t>) {
                    static_assert(
                        std::is_same_v<t, first_t>,
                        "If you see an error here, you wrote merge[p1 >> "
                        "p2 >> ... pn] where at least one of the types in "
                        "ATTR(p1), ATTR(p2), ... ATTR(pn) is not the same "
                        "type as one of the others.");
                    if constexpr (!std::is_same_v<t, first_t>) {
                        [[maybe_unused]] detail::print_type<tuple<Args...>>
                            tuple_types;
                        [[maybe_unused]] detail::print_type<all_types>
                            attribute_types;
                        [[maybe_unused]] detail::print_type<first_t> first_type;
                        [[maybe_unused]] detail::print_type<t> this_type;
                    }
                }
            });

        }
    }
}}
