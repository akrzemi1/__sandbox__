//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_SRC_HPP
#define BOOST_URL_SRC_HPP

/*

This file is meant to be included once,
in a translation unit of the program.

*/

// MUST COME FIRST
#ifndef BOOST_URL_SOURCE
#define BOOST_URL_SOURCE
#endif

// We include this in case someone is
// using src.hpp as their main header file
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_HPP
#define BOOST_URL_HPP

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_GRAMMAR_HPP
#define BOOST_URL_GRAMMAR_HPP

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_CONFIG_HPP
#define BOOST_URL_DETAIL_CONFIG_HPP

#include <boost/config.hpp>
#include <boost/config/workaround.hpp>
#include <limits.h>
#include <stdint.h>

#if CHAR_BIT != 8
# error unsupported platform
#endif

#if defined(BOOST_URL_DOCS)
# define BOOST_URL_DECL
#else
# if (defined(BOOST_URL_DYN_LINK) || defined(BOOST_ALL_DYN_LINK)) && !defined(BOOST_URL_STATIC_LINK)
#  if defined(BOOST_URL_SOURCE)
#   define BOOST_URL_DECL  BOOST_SYMBOL_EXPORT
#   define BOOST_URL_BUILD_DLL
#  else
#   define BOOST_URL_DECL  BOOST_SYMBOL_IMPORT
#  endif
# endif // shared lib
# ifndef  BOOST_URL_DECL
#  define BOOST_URL_DECL
# endif
# if !defined(BOOST_URL_SOURCE) && !defined(BOOST_ALL_NO_LIB) && !defined(BOOST_URL_NO_LIB)
#  define BOOST_LIB_NAME boost_url
#  if defined(BOOST_ALL_DYN_LINK) || defined(BOOST_URL_DYN_LINK)
#   define BOOST_DYN_LINK
#  endif
#  include <boost/config/auto_link.hpp>
# endif
#endif

#if ! defined(BOOST_URL_NO_SSE2) && \
    ! defined(BOOST_URL_USE_SSE2)
# if (defined(_M_IX86) && _M_IX86_FP == 2) || \
      defined(_M_X64) || defined(__SSE2__)
#  define BOOST_URL_USE_SSE2
# endif
#endif

// This macro is used for the limits
// test which sets the value lower,
// to exercise code coverage.
//
#ifndef BOOST_URL_MAX_SIZE
// we leave room for a null,
// and still fit in signed-32
#define BOOST_URL_MAX_SIZE 0x7ffffffe
#endif

#if BOOST_WORKAROUND( BOOST_GCC_VERSION, <= 72000 ) || \
    BOOST_WORKAROUND( BOOST_CLANG_VERSION, <= 35000 )
# define BOOST_URL_CONSTEXPR
#else
# define BOOST_URL_CONSTEXPR constexpr
#endif

// Add source location to error codes
#ifdef BOOST_URL_NO_SOURCE_LOCATION
# define BOOST_URL_ERR(ev) (ev)
#else
# define BOOST_URL_ERR(ev) (::boost::system::error_code( (ev), [] { \
         static constexpr auto loc(BOOST_CURRENT_LOCATION); \
         return &loc; }()))
#endif

// detect 32/64 bit
#if UINTPTR_MAX == UINT64_MAX
# define BOOST_URL_ARCH 64
#elif UINTPTR_MAX == UINT32_MAX
# define BOOST_URL_ARCH 32
#else
# error Unknown or unsupported architecture, please open an issue
#endif

#ifndef BOOST_URL_DOCS
using pos_t = size_t;
#endif

#endif

//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
// Copyright (c) 2022 Alan Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_GRAMMAR_ASCII_HPP
#define BOOST_URL_GRAMMAR_ASCII_HPP

//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_GRAMMAR_DETAIL_CHARSET_HPP
#define BOOST_URL_GRAMMAR_DETAIL_CHARSET_HPP

#include <boost/core/bit.hpp>
#include <boost/type_traits/make_void.hpp>
#include <type_traits>

#ifdef BOOST_URL_USE_SSE2
# include <emmintrin.h>
# include <xmmintrin.h>
# ifdef _MSC_VER
#  include <intrin.h>
# endif
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4127) // conditional expression is constant
#endif

namespace boost {
namespace urls {
namespace grammar {
namespace detail {

template<class T, class = void>
struct has_find_if : std::false_type {};

template<class T>
struct has_find_if<T, boost::void_t<
    decltype(
    std::declval<char const*&>() =
        std::declval<T const&>().find_if(
            std::declval<char const*>(),
            std::declval<char const*>())
            )>> : std::true_type
{
};

template<class T, class = void>
struct has_find_if_not : std::false_type {};

template<class T>
struct has_find_if_not<T, boost::void_t<
    decltype(
    std::declval<char const*&>() =
        std::declval<T const&>().find_if_not(
            std::declval<char const*>(),
            std::declval<char const*>())
            )>> : std::true_type
{
};

template<class Pred>
char const*
find_if(
    char const* first,
    char const* const last,
    Pred const& pred,
    std::false_type) noexcept
{
    while(first != last)
    {
        if(pred(*first))
            break;
        ++first;
    }
    return first;
}

template<class Pred>
char const*
find_if(
    char const* first,
    char const* const last,
    Pred const& pred,
    std::true_type) noexcept
{
    return pred.find_if(
        first, last);
}

template<class Pred>
char const*
find_if_not(
    char const* first,
    char const* const last,
    Pred const& pred,
    std::false_type) noexcept
{
    while(first != last)
    {
        if(! pred(*first))
            break;
        ++first;
    }
    return first;
}

template<class Pred>
char const*
find_if_not(
    char const* first,
    char const* const last,
    Pred const& pred,
    std::true_type) noexcept
{
    return pred.find_if_not(
        first, last);
}

#ifdef BOOST_URL_USE_SSE2

// by Peter Dimov
template<class Pred>
char const*
find_if_pred(
    Pred const& pred,
    char const* first,
    char const* last ) noexcept
{
    while( last - first >= 16 )
    {
        unsigned char r[ 16 ] = {};
        for( int i = 0; i < 16; ++i )
            r[ i ] = pred( first[ i ] )? 0xFF: 0x00;
        __m128i r2 = _mm_loadu_si128( (__m128i const*)r );
        unsigned r3 = _mm_movemask_epi8( r2 );
        if( r3 )
            return first + boost::core::countr_zero( r3 );
        first += 16;
    }
    while(
        first != last &&
        ! pred(*first))
    {
        ++first;
    }
    return first;
}

// by Peter Dimov
template<class Pred>
char const*
find_if_not_pred(
    Pred const& pred,
    char const* first,
    char const* last ) noexcept
{
    while( last - first >= 16 )
    {
        unsigned char r[ 16 ] = {};
        for( int i = 0; i < 16; ++i )
            r[ i ] = pred( first[ i ] )? 0x00: 0xFF;
        __m128i r2 = _mm_loadu_si128( (__m128i const*)r );
        unsigned r3 = _mm_movemask_epi8( r2 );
        if( r3 )
            return first + boost::core::countr_zero( r3 );
        first += 16;
    }
    while(
        first != last &&
        pred(*first))
    {
        ++first;
    }
    return first;
}

#endif

} // detail
} // grammar
} // urls
} // boost

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
#include <boost/static_assert.hpp>

namespace boost {
namespace urls {
namespace grammar {

// Algorithms for interacting with ASCII
// characters and strings, for implementing
// semantics in RFCs. These routines do
// not use locale.

/** Return c converted to lower case

    This function returns the lowercase equivalent
    if a character is an upper-case letter, otherwise
    returns the same character.

    @par Exception Safety
    Throws nothing.

    @return The converted character

    @param c The ascii character to convert
*/
constexpr
char
ascii_tolower(char c) noexcept
{
    return
      (c >= 'A' &&
       c <= 'Z')
        ? c + 'a' - 'A'
        : c;
}

/** Return c converted to upper case

    This function returns the lowercase equivalent
    if a character is an upper-case letter, otherwise
    returns the same character.

    @par Exception Safety
    Throws nothing.

    @return The converted character

    @param c The ascii character to convert
*/
constexpr
char
ascii_toupper(char c) noexcept
{
    return
      (c >= 'a' &&
       c <= 'z')
        ? c - ('a' - 'A')
        : c;
}

} // grammar
} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_GRAMMAR_CHAR_LITERAL_HPP
#define BOOST_URL_GRAMMAR_CHAR_LITERAL_HPP

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_GRAMMAR_ERROR_HPP
#define BOOST_URL_GRAMMAR_ERROR_HPP

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_ERROR_CODE_HPP
#define BOOST_URL_ERROR_CODE_HPP

#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>

namespace boost {
namespace urls {

/// The type of error category used by the library
using error_category = boost::system::error_category;

/// The type of error code used by the library
using error_code = boost::system::error_code;

/// The type of error condition used by the library
using error_condition = boost::system::error_condition;

/// The type of system error thrown by the library
using system_error = boost::system::system_error;

} // urls
} // boost

#endif

namespace boost {
namespace urls {
namespace grammar {

/** Error codes returned by BNF algorithms
*/
enum class error
{
    // VFALCO 3 space indent or
    // else Doxygen malfunctions

    /**
     * The operation completed successfully.
    */
    success = 0,

    /**
     * A list parser reached the end.
    */
    end,

    /**
     * Incomplete input for grammar.

       This happens when the end of the input
       string is reached without fully matching
       the grammar. Parsing elements which
       support streaming will return this error
       to indicate additional input is necessary
       for completely matching the grammar.
    */
    incomplete,

    /**
     * Leftover input remaining after match.
    */
    leftover,

    /** An integer overflowed during parsing.
    */
    overflow,

    /**
     * An unspecified syntax error was found.
    */
    syntax
};

//------------------------------------------------

/** Error conditions returned by the library.
*/
enum class condition
{
    /**
     * A fatal error in syntax was encountered.

       This indicates that parsing cannot continue.
    */
    fatal = 1
};

} // grammar
} // urls
} // boost

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_GRAMMAR_IMPL_ERROR_HPP
#define BOOST_URL_GRAMMAR_IMPL_ERROR_HPP

#include <type_traits>

namespace boost {
namespace system {
template<>
struct is_error_code_enum<
    ::boost::urls::grammar::error>
{
    static bool const value = true;
};
template<>
struct is_error_condition_enum<
    ::boost::urls::grammar::condition>
{
    static bool const value = true;
};
} // system
} // boost

namespace boost {
namespace urls {
namespace grammar {

BOOST_URL_DECL
error_code
make_error_code(
    error e) noexcept;

BOOST_URL_DECL
error_condition
make_error_condition(
    condition c) noexcept;

} // grammar
} // urls
} // boost

#endif

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_GRAMMAR_PARSE_TAG_HPP
#define BOOST_URL_GRAMMAR_PARSE_TAG_HPP


namespace boost {
namespace urls {
namespace grammar {

/** Type tag for tag_invoke customization
*/
struct parse_tag
{
};

} // grammar
} // urls
} // boost

#endif

namespace boost {
namespace urls {
namespace grammar {

/** Rule for a literal character
*/
template<char Ch>
struct char_literal
{
    char const* p;

    friend
    void
    tag_invoke(
        parse_tag const&,
        char const*& it,
        char const* end,
        error_code& ec,
        char_literal& t) noexcept
    {
        if(it == end)
        {
            ec = error::incomplete;
            return;
        }
        if(*it != Ch)
        {
            ec = error::syntax;
            return;
        }
        t.p = it++;
    }
};

} // grammar
} // urls
} // boost

#endif
//
// Copyright (c) 2021 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_GRAMMAR_CHARSET_HPP
#define BOOST_URL_GRAMMAR_CHARSET_HPP

#include <boost/type_traits/make_void.hpp>
#include <boost/static_assert.hpp>
#include <cstdint>
#include <type_traits>

// Credit to Peter Dimov for ideas regarding
// SIMD constexpr, and character set masks.

namespace boost {
namespace urls {
namespace grammar {

/** Alias for `std::true_type` if T satisfies <em>CharSet</em>.

    This metafunction determines if the
    type `T` meets these requirements of
    <em>CharSet</em>:

    @li An instance of `T` is invocable
    with this equivalent function signature:
    @code
    bool T::operator()( char ) const;
    @endcode

    @par Example
    Use with `enable_if` on the return value:
    @code
    template< class CharSet >
    typename std::enable_if< is_charset<T>::value >::type
    func( CharSet const& cs );
    @endcode

    @tparam T the type to check.
*/
#ifdef BOOST_URL_DOCS
template<class T>
using is_charset = __see_below__;
#else
template<class T, class = void>
struct is_charset : std::false_type {};

template<class T>
struct is_charset<T, boost::void_t<
    decltype(
    std::declval<bool&>() =
        std::declval<T const&>().operator()(
            std::declval<char>())
            ) > > : std::true_type
{
};
#endif

//------------------------------------------------

/** A character set containing all characters.

    Objects of this type are invocable with
    this equivalent signature:

    @code
    bool( char ch ) const noexcept;
    @endcode

    The function object returns `true` when
    `ch` is a member of the character set,
    and `false` otherwise. The type of the
    function satisfies the <em>CharSet</em>
    requirements.

    @par BNF
    @code
    ALL         = %x00-FF
                ; all ASCII and high-ASCII
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc5234#appendix-B.1"
        >B.1. Core Rules (rfc5234)</a>

    @see
        @ref all_chars,
        @ref is_charset,
        @ref find_if,
        @ref find_if_not.
*/
#ifdef BOOST_URL_DOCS
using all_chars_t = __see_below__;
#else
struct all_chars_t
{
    constexpr
    bool
    operator()(char) const noexcept
    {
        return true;
    }

    constexpr
    char const*
    find_if(
        char const* first,
        char const*) const noexcept
    {
        return first;
    }

    constexpr
    char const*
    find_if_not(
        char const*,
        char const* last) const noexcept
    {
        return last;
    }
};
#endif

/** A character set constant representing all characters

    @see
        @ref all_chars_t
*/
constexpr all_chars_t all_chars{};

//------------------------------------------------

/** A character set containing all letters and digits.

    Objects of this type are invocable
    with this equivalent signature:

    @code
    bool( char ch ) const noexcept;
    @endcode

    The function object returns `true` when
    `ch` is a member of the character set,
    and `false` otherwise. The type of the
    function satisfies the <em>CharSet</em>
    requirements.

    @par BNF
    @code
    ALNUM       = ALPHA / DIGIT

    ALPHA       =  %x41-5A / %x61-7A
                ; A-Z / a-z

    DIGIT       =  %x30-39
                ; 0-9
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc5234#appendix-B.1"
        >B.1. Core Rules (rfc5234)</a>

    @see
        @ref is_charset,
        @ref find_if,
        @ref find_if_not.
*/
#ifdef BOOST_URL_DOCS
using alnum_chars_t = __see_below__;
#else
struct alnum_chars_t
{
    constexpr
    bool
    operator()(char c) const noexcept
    {
        return
            (c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z');
    }

#ifdef BOOST_URL_USE_SSE2
    char const*
    find_if(
        char const* first,
        char const* last) const noexcept
    {
        return detail::find_if_pred(
            *this, first, last);
    }

    char const*
    find_if_not(
        char const* first,
        char const* last) const noexcept
    {
        return detail::find_if_not_pred(
            *this, first, last);
    }
#endif
};
#endif

/** A character set containing the alphanumeric characters.

    @see
        @ref alnum_chars_t
*/
constexpr alnum_chars_t alnum_chars{};

//------------------------------------------------

/** A character set containing the alphabetical characters.

    Objects of this type are invocable
    with this equivalent signature:

    @code
    bool( char ch ) const noexcept;
    @endcode

    The function object returns `true` when
    `ch` is a member of the character set,
    and `false` otherwise. The type of the
    function satisfies the <em>CharSet</em>
    requirements.

    @par BNF
    @code
    ALPHA       =  %x41-5A / %x61-7A
                ; A-Z / a-z
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc5234#appendix-B.1"
        >B.1. Core Rules (rfc5234)</a>

    @see
        @ref alpha_chars,
        @ref is_charset,
        @ref find_if,
        @ref find_if_not.
*/
#ifdef BOOST_URL_DOCS
using alpha_chars_t = __see_below__;
#else
struct alpha_chars_t
{
    constexpr
    bool
    operator()(char c) const noexcept
    {
        return
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z');
    }

#ifdef BOOST_URL_USE_SSE2
    char const*
    find_if(
        char const* first,
        char const* last) const noexcept
    {
        return detail::find_if_pred(
            *this, first, last);
    }

    char const*
    find_if_not(
        char const* first,
        char const* last) const noexcept
    {
        return detail::find_if_not_pred(
            *this, first, last);
    }
#endif
};
#endif

/** A character set containing the alphabetical characters.

    @see
        @ref alpha_chars_t
*/
constexpr alpha_chars_t alpha_chars{};

//------------------------------------------------

/** A character set containing the decimal digits.

    This is an object invocable with
    the equivalent signature:

    @code
    bool( char ch ) const noexcept;
    @endcode

    The function object returns `true` when
    `ch` is a member of the character set,
    and `false` otherwise. The type of the
    function satisfies the <em>CharSet</em>
    requirements.

    @par BNF
    @code
    DIGIT       =  %x30-39
                ; 0-9
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc5234#appendix-B.1"
        >B.1. Core Rules (rfc5234)</a>

    @see
        @ref digit_chars,
        @ref is_charset,
        @ref find_if,
        @ref find_if_not.
*/
#ifdef BOOST_URL_DOCS
using digit_chars_t = __see_below__;
#else
struct digit_chars_t
{
    constexpr
    bool
    operator()(char c) const noexcept
    {
        return c >= '0' && c <= '9';
    }

#ifdef BOOST_URL_USE_SSE2
    char const*
    find_if(
        char const* first,
        char const* last) const noexcept
    {
        return detail::find_if_pred(
            *this, first, last);
    }

    char const*
    find_if_not(
        char const* first,
        char const* last) const noexcept
    {
        return detail::find_if_not_pred(
            *this, first, last);
    }
#endif
};
#endif

/** A character set containing the decimal digits

    @see
        @ref digit_chars_t
*/
constexpr digit_chars_t digit_chars{};

//------------------------------------------------

/** A character set containing the hexadecimal digits.

    This is an object invocable with
    the equivalent signature:

    @code
    bool( char ch ) const noexcept;
    @endcode

    The function object returns `true` when
    `ch` is a member of the character set,
    and `false` otherwise. The type of the
    function satisfies the <em>CharSet</em>
    requirements.

    @par BNF
    @code
    HEXDIG      = DIGIT
                / "A" / "B" / "C" / "D" / "E" / "F"
                / "a" / "b" / "c" / "d" / "e" / "f"
    @endcode

    @note The RFCs are inconsistent on the case
    sensitivity of hexadecimal digits. Existing
    uses suggest case-insensitivity is a de-facto
    standard.

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc5234#appendix-B.1"
        >B.1. Core Rules (rfc5234)</a>
    @li <a href="https://datatracker.ietf.org/doc/html/rfc7230#section-1.2"
        >1.2. Syntax Notation (rfc7230)</a>
    @li <a href="https://datatracker.ietf.org/doc/html/rfc5952#section-2.3"
        >2.3. Uppercase or Lowercase (rfc5952)</a>
    @li <a href="https://datatracker.ietf.org/doc/html/rfc5952#section-4.3"
        >4.3. Lowercase (rfc5952)</a>

    @see
        @ref hexdig_chars,
        @ref is_charset,
        @ref find_if,
        @ref find_if_not.
*/
#ifdef BOOST_URL_DOCS
using hexdig_chars_t = __see_below__;
#else
struct hexdig_chars_t
{
    /** Return true if c is in the character set.
    */
    constexpr
    bool
    operator()(char c) const noexcept
    {
        return
            (c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'F') ||
            (c >= 'a' && c <= 'f');
    }

#ifdef BOOST_URL_USE_SSE2
    char const*
    find_if(
        char const* first,
        char const* last) const noexcept
    {
        return detail::find_if_pred(
            *this, first, last);
    }

    char const*
    find_if_not(
        char const* first,
        char const* last) const noexcept
    {
        return detail::find_if_not_pred(
            *this, first, last);
    }
#endif
};
#endif

/** A character set containing the hexadecimal digits

    @see
        @ref hexdig_chars_t
*/
constexpr hexdig_chars_t hexdig_chars{};

// VFALCO We can declare
// these later if needed
//
//struct hexdig_upper_chars;
//struct hexdig_lower_chars;

/** Return the numeric value of a HEXDIG, or -1 if invalid

    This function returns the numeric value
    of a single hexadecimal digit, or -1 if
    `ch` is not a hexadecimal digit.
*/
inline
bool
hexdig_value(char c, char& res) noexcept
{
    if (c >= '0' && c <= '9')
    {
        res = c - '0';
        return true;
    }
    if (c >= 'A' && c <= 'F')
    {
        res = 10 + c - 'A';
        return true;
    }
    if (c >= 'a' && c <= 'f')
    {
        res = 10 + c - 'a';
        return true;
    }
    res = 0;
    return false;
}

//------------------------------------------------

/** Find the first character in the string that is in the set.

    @par Exception Safety
    Throws nothing.

    @return A pointer to the character if found,
    otherwise a pointer to `last`.

    @param first A pointer to the first character
    in the string to search.

    @param last A pointer to one past the last
    character in the string to search.

    @param cs The character set to use.

    @see
        @ref find_if_not.
*/
template<class CharSet>
char const*
find_if(
    char const* const first,
    char const* const last,
    CharSet const& cs) noexcept
{
    BOOST_STATIC_ASSERT(
        is_charset<CharSet>::value);
    return detail::find_if(first, last, cs,
        detail::has_find_if<CharSet>{});
}

/** Find the first character in the string that is not in CharSet

    @par Exception Safety
    Throws nothing.

    @return A pointer to the character if found,
    otherwise a pointer to `last`.

    @param first A pointer to the first character
    in the string to search.

    @param last A pointer to one past the last
    character in the string to search.

    @param cs The character set to use.

    @see
        @ref find_if_not.
*/
template<class CharSet>
char const*
find_if_not(
    char const* const first,
    char const* const last,
    CharSet const& cs) noexcept
{
    BOOST_STATIC_ASSERT(
        is_charset<CharSet>::value);
    return detail::find_if_not(first, last, cs,
        detail::has_find_if_not<CharSet>{});
}

} // grammar
} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//                         Damian Jarek (damian dot jarek93 at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/http_proto
//

#ifndef BOOST_URL_GRAMMAR_ELEMENTS_HPP
#define BOOST_URL_GRAMMAR_ELEMENTS_HPP

//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_GRAMMAR_PARSE_HPP
#define BOOST_URL_GRAMMAR_PARSE_HPP

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_STRING_VIEW_HPP
#define BOOST_URL_STRING_VIEW_HPP

#include <boost/core/detail/string_view.hpp>

namespace boost {
namespace urls {

/** The type of string_view used by the library

    String views are used to pass character
    buffers into or out of functions. Ownership
    of the underlying character buffer is not
    transferred; the caller is responsible for
    ensuring that the lifetime of the object
    owning the character buffer extends until
    the string view is no longer referenced.
*/
typedef boost::core::string_view string_view;

} // urls
} // boost

#endif
#include <boost/type_traits/make_void.hpp>
#include <type_traits>

namespace boost {
namespace urls {
namespace grammar {

/** Parse a literal character

    This function parses the first char in a
    substring as a literal character.

    @par Example

    @code
    std::string str = "Hello.cpp";
    char const* it = str.data();
    char const* end = it + str.size();
    if (parse(it, end, ec, 'H'))
    {
        std::cout << "str begins with 'H'\n";
    }
    @endcode

    @par Exception Safety
    Throws nothing.

    @param it An iterator to the first element in the range.
    At the end of this function, `it` points to one past the last
    element parsed in the range.

    @param end An iterator to the end of the range

    @param ec Set to the error, if any occurred

    @param ch A literal character to match
*/
void
tag_invoke(
    parse_tag const&,
    char const*& it,
    char const* end,
    error_code& ec,
    char ch) noexcept;

/** Parse a range of characters into two or more sequential grammar rules

    This function parses a range of characters into
    one or more grammar rules, where each rule
    defines its own syntax and storage for parsed
    results.

    The algorithm works by invoking a function called
    `parse` for each element using an unqualified call
    so that argument-dependent lookup applies, using
    the equivalent signature:

    @code
    template< class Rule >
    bool parse( char const*&, char const*, error_code&, Rule&& r );
    @endcode

    If this call succeeds, the algorithm proceeds by
    performing the same operation on the remaining
    arguments including the parameter pack.

    If an error occurs for any of the rules in the
    parameter pack, the entire parse operation fails
    but the chars from the substring are still consumed.

    @par Example
    @code
    if (parse(it, end, ec, r1, r2, r3))
    {
        std::cout << "Range [it, end) parsed successfully\n";
    }
    @endcode

    @par Exception Safety
    Defined by the types of the rule objects.

    @return `true` if the range initial chars match the elements successfully

    @param it An iterator to the first element in the range.
    At the end of this function, `it` points to one past the last
    element parsed in the range.

    @param end An iterator to the end of the range

    @param ec Set to the error, if any occurred

    @param rn Grammar rule objects

*/
template<class... Rn>
bool
parse(
    char const*& it,
    char const* end,
    error_code& ec,
    Rn&&... rn);

/** Parse all rules in sequence

    If the parse fails, `it` is unchanged.
*/
template<class... Rn>
bool
parse_all(
    char const*& it,
    char const* end,
    error_code& ec,
    Rn&&... rn);

/** Parse a sequence of grammar rules

   This function parses a complete string into the sequence of
   specified grammar rules. The function returns an error if
   some characters are left unparsed.

   @par Example

   @code
   if (parse(str, ec, r1, r2, r3)) {
     std::cout << "String " << str << " parsed successfully\n";
   }
   @endcode

   @par Exception Safety

     Defined by the types of the rule objects.

   @return `true` if the string matches the rules successfully and all
   chars are consumed

   @param s The input string

   @param ec Set to the error, if any occurred. If the string is not completely consumed,
   but the beginning of the string matches the elements, `ec` is set to @ref error::leftover.

   @param rn Grammar rule objects

*/
template<class... Rn>
bool
parse_string(
    string_view s,
    error_code& ec,
    Rn&&... rn);

/** Parse a sequence of grammar rules and throw on failure

   This function parses a complete string into the specified sequence
   of grammar rules. If the string is not completely consumed, an
   error is thrown.

   @par Example

   @code
   try {
      parse(str, ec, r1, r2, r3);
   } catch (boost::urls::system_error& e) {
      std::cout << e.what() << '\n';
   }
   @endcode

   @par Exception Safety

     Exceptions thrown on invalid input.

   @param s The input string

   @param rn Grammar rule objects

   @throws boost::system::system_error Thrown on failure.

 */
template<class... Rn>
void
parse_string(
    string_view s,
    Rn&&... rn);

} // grammar
} // urls
} // boost

//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_GRAMMAR_IMPL_PARSE_HPP
#define BOOST_URL_GRAMMAR_IMPL_PARSE_HPP

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_EXCEPT_HPP
#define BOOST_URL_DETAIL_EXCEPT_HPP

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_ERROR_HPP
#define BOOST_URL_ERROR_HPP

#include <boost/system/result.hpp>
#include <stdexcept>

namespace boost {
namespace urls {

/** The type of result returned by library functions

    This is an alias template used as the return type
    for functions that can either return a container,
    or fail with an error code. This is a brief
    synopsis of the type:

    @par Declaration
    @code
    template< class T >
    class result
    {
    public:
        // Return true if the result contains an error
        constexpr bool has_error() const noexcept;

        // These return true if the result contains a value
        constexpr bool has_value() const noexcept;
        constexpr explicit operator bool() const noexcept;

        // Return the value or throw an exception if has_value()==false
        constexpr T& value();
        constexpr T& operator*();
        constexpr T const& value() const;
        constexpr T const& operator*() const;

        // Return the error, which is default constructed if has_error()==false
        constexpr error_code error() const noexcept;

        ...more
    @endcode

    @par Usage
    Given the function @ref parse_uri with this signature:
    @code
    result< url_view > parse_uri( string_view s ) noexcept;
    @endcode

    The following statement captures the value in a
    variable upon success, otherwise throws:
    @code
    url_view u = parse_uri( "http://example.com/path/to/file.txt" ).value();
    @endcode

    This statement captures the result in a local
    variable and inspects the error condition:
    @code
    result< url_view > r = parse_uri( "http://example.com/path/to/file.txt" );

    if( r.has_error() )
        std::cout << r.error();
    else
        std::cout << r.value();
    @endcode

    @note For a full synopsis of the type, please see
    the corresponding documentation in Boost.System.

    @tparam T The type of value held by the result.
*/
template<class T>
using result = boost::system::result<T, error_code>;

//----------------------------------------------------------

/** Error codes returned the library
*/
enum class error
{
    // VFALCO 3 space indent or
    // else Doxygen malfunctions

    /**
     * The operation completed successfully.
    */
    success = 0,

    /**
     * Bad alphabetic character.
    */
    bad_alpha,

    /**
     * Character is not a digit.
    */
    bad_digit,

    /**
     * A required element was empty.
    */
    bad_empty_element,

    /**
     * Bad HEXDIG
    */
    bad_hexdig,

    /**
     * Syntax error in IPv6 address.
    */
    bad_ipv6,

    /**
     * Bad leading zero in number.
    */
    bad_leading_zero,

    /**
     * The octet is out of range.
    */
    bad_octet,

    /**
     * Bad schemeless path segment.
    */
    bad_schemeless_path_segment,

    /** Bad empty element.
    */
    empty,

    /**
     * Illegal empty path segment.
    */
    empty_path_segment,

    /**
     * Null encountered in pct-encoded.
    */
    illegal_null,

    /**
     * Illegal reserved character in encoded string.
    */
    illegal_reserved_char,

    /**
     * Validation failed
    */
    invalid,

    /**
     * Missing character literal.
    */
    missing_char_literal,

    /**
     * Missing path segment.
    */
    missing_path_segment,

    /**
     * A slash '/' was expected in the path segment.
    */
    missing_path_separator,

    /**
     * Missing words in IPv6 address.
    */
    missing_words,

    /**
     * A grammar element was not in canonical form.
    */
    non_canonical,

    /**
     * Wrong character literal.
    */
    wrong_char_literal,

    //--------------------------------------------

    /**
     * Bad hexadecimal digit.

       This error condition is fatal.
    */
    bad_pct_hexdig,

    /**
     * The percent-encoded sequence is incomplete.

       This error condition is fatal.
    */
    incomplete_pct_encoding,

    /**
     * Missing hexadecimal digit.

       This error condition is fatal.
    */
    missing_pct_hexdig,

    /**
     * No space in output buffer

       This error is returned when a provided
       output buffer was too small to hold
       the complete result of an algorithm.
    */
    no_space,

    /**
     * The URL is not a base URL
    */
    not_a_base
};

} // urls
} // boost

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_ERROR_HPP
#define BOOST_URL_IMPL_ERROR_HPP

#include <type_traits>

namespace boost {
namespace system {
template<>
struct is_error_code_enum<::boost::urls::error>
{
    static bool const value = true;
};
} // system
} // boost

namespace boost {
namespace urls {

BOOST_URL_DECL
error_code
make_error_code(error e);

} // urls
} // boost

#endif

#endif
#include <boost/assert/source_location.hpp>

namespace boost {
namespace urls {
namespace detail {

BOOST_URL_DECL void BOOST_NORETURN throw_bad_alloc(source_location const& loc);
BOOST_URL_DECL void BOOST_NORETURN throw_invalid_argument(source_location const& loc);
BOOST_URL_DECL void BOOST_NORETURN throw_invalid_argument(char const* what, source_location const& loc);
BOOST_URL_DECL void BOOST_NORETURN throw_length_error(char const* what, source_location const& loc);
BOOST_URL_DECL void BOOST_NORETURN throw_out_of_range(source_location const& loc);
BOOST_URL_DECL void BOOST_NORETURN throw_system_error(error_code const& ec, source_location const& loc);
//BOOST_URL_DECL void BOOST_NORETURN throw_system_error(error e, source_location const& loc);

inline
void
maybe_throw(
    error_code const& ec,
    source_location const& loc)
{
    if(ec.failed())
        throw_system_error(ec, loc);
}

} // detail
} // urls
} // boost

#endif
#include <utility>

namespace boost {
namespace urls {
namespace grammar {

inline
void
tag_invoke(
    parse_tag const&,
    char const*& it,
    char const* const end,
    error_code& ec,
    char ch) noexcept
{
    if(it == end)
    {
        // end of input
        ec = error::incomplete;
        return;
    }
    if(*it != ch)
    {
        // expected ch
        ec = error::syntax;
        return;
    }
    ++it;
}

//------------------------------------------------

namespace detail {

inline
void
parse_impl(
    char const*&,
    char const*,
    error_code&) noexcept
{
}

// this is here to avoid
// having to clear `ec`
// for every call
template<
    class R0,
    class... Rn>
void
parse_impl(
    char const*& it,
    char const* const end,
    error_code& ec,
    R0&& r0,
    Rn&&... rn)
{
    using grammar::tag_invoke;
    tag_invoke(
        parse_tag{},
        it, end, ec,
        std::forward<R0>(r0));
    if(ec.failed())
        return;
    parse_impl(
        it, end, ec,
        std::forward<Rn>(rn)...);
}

} // detail

template<class... Rn>
bool
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    Rn&&... rn)
{
    ec = {};
    detail::parse_impl(
        it, end, ec,
        std::forward<Rn>(rn)...);
    return ! ec.failed();
}

//------------------------------------------------

template<class... Rn>
bool
parse_all(
    char const*& it,
    char const* const end,
    error_code& ec,
    Rn&&... rn)
{
    auto const it0 = it;
    if(! (parse)(it, end, ec,
        std::forward<Rn>(rn)...))
    {
        it = it0;
        return false;
    }
    return true;
}

//------------------------------------------------

template<class... Rn>
bool
parse_string(
    string_view s,
    error_code& ec,
    Rn&&... rn)
{
    auto it = s.data();
    auto const end = it + s.size();
    if(! (parse)(it, end, ec,
        std::forward<Rn>(rn)...))
        return false;
    if(it != end)
    {
        // input not consumed fully
        ec = error::leftover;
        return false;
    }
    return true;
}

//------------------------------------------------

template<class... Rn>
void
parse_string(
    string_view s,
    Rn&&... rn)
{
    error_code ec;
    if(! parse_string(s, ec,
        std::forward<Rn>(rn)...))
    {
        urls::detail::throw_system_error(
            ec, BOOST_CURRENT_LOCATION);
    }
    BOOST_ASSERT(! ec.failed());
}

} // grammar
} // urls
} // boost

#endif

#endif
#include <boost/static_assert.hpp>

#include <boost/mp11/integer_sequence.hpp>
#include <boost/mp11/algorithm.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/copy_cv.hpp>
#include <cstdlib>
#include <utility>

namespace boost {
namespace urls {
namespace grammar {

namespace detail {

// lightweight tuple by Damian Jarek.
// this one is default-constructible,

template<
    std::size_t I, class R>
struct element_impl
{
    R t;
};

template<class... Rn>
struct elements_impl;

template<
    class... Rn,
    std::size_t... In>
struct elements_impl<
    boost::mp11::index_sequence<
        In...>, Rn...>
  : element_impl<In, Rn>...
{
};

template<class... Rn>
struct tuple : elements_impl<
    boost::mp11::index_sequence_for<
        Rn...>, Rn...>
{
};

} // detail

//------------------------------------------------

template<
    std::size_t I, class R>
R&
get(detail::element_impl<
    I, R>& te)
{
    return te.t;
}

template<
    std::size_t I, class R>
R const&
get(detail::element_impl<
    I, R> const& te)
{
    return te.t;
}

template<
    std::size_t I, class R>
R&&
get(detail::element_impl<
    I, R>&& te)
{
    return std::move(te.t);
}

template<
    std::size_t I, class R>
R&
get(detail::element_impl<
    I, R&>&& te)
{
    return te.t;
}

//------------------------------------------------

/** Rule for a series of elements parsed in sequence.
*/
template<class... Rn>
struct elements
#ifndef BOOST_URL_DOCS
    : detail::elements_impl<
        boost::mp11::index_sequence_for<
            Rn...>, Rn...>
#endif
{
    BOOST_STATIC_ASSERT(
        sizeof...(Rn) > 0);
    friend
    void
    tag_invoke(
        parse_tag const&,
        char const*& it,
        char const* end,
        error_code& ec,
        elements& t) noexcept
    {
        constexpr auto I = sizeof...(Rn);
        parse_impl(
            it, end, ec, t,
            std::integral_constant<
                std::size_t, I-1>{});
    }

private:
    static
    void
    parse_impl(
        char const*& it,
        char const* end,
        error_code& ec,
        elements& t,
        std::integral_constant<
            std::size_t, 0>)
    {
        parse(it, end, ec, get<0>(t));
    }

    template<std::size_t I>
    static
    void
    parse_impl(
        char const*& it,
        char const* end,
        error_code& ec,
        elements& t,
        std::integral_constant<
            std::size_t, I>)
    {
        parse_impl(
            it, end, ec, t,
            std::integral_constant<
                std::size_t, I-1>{});
        if(ec.failed())
            return;
        parse(it, end, ec, get<I>(t));
    }
};

} // grammar
} // urls
} // boost

#endif
//
// Copyright (c) 2021 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_GRAMMAR_LUT_CHARS_HPP
#define BOOST_URL_GRAMMAR_LUT_CHARS_HPP

#include <boost/type_traits/make_void.hpp>
#include <cstdint>
#include <type_traits>

// Credit to Peter Dimov for ideas regarding
// SIMD constexpr, and character set masks.

namespace boost {
namespace urls {
namespace grammar {

#ifndef BOOST_URL_DOCS
namespace detail {
template<class T, class = void>
struct is_pred : std::false_type {};

template<class T>
struct is_pred<T, boost::void_t<
    decltype(
    std::declval<bool&>() =
        std::declval<T const&>().operator()(
            std::declval<char>())
            ) > > : std::true_type
{
};
} // detail
#endif

/** A character set based on a constexpr lookup table.

    Objects of this type are invocable with
    the equivalent signature:

    @code
    bool( char ch ) const noexcept;
    @endcode

    The function object returns `true` when
    `ch` is a member of the character set,
    and `false` otherwise. The type of the
    function satisfies the <em>CharSet</em>
    requirements.

    @see
        @ref is_charset,
        @ref find_if,
        @ref find_if_not.
*/
class lut_chars
{
    std::uint64_t mask_[4] = {};

    constexpr
    static
    std::uint64_t
    lo(char c) noexcept
    {
        return static_cast<
            unsigned char>(c) & 3;
    }

    constexpr
    static
    std::uint64_t
    hi(char c) noexcept
    {
        return 1ULL << (static_cast<
            unsigned char>(c) >> 2);
    }

    constexpr
    static
    lut_chars
    construct(
        char const* s) noexcept
    {
        return *s
            ? lut_chars(*s) +
                construct(s+1)
            : lut_chars();
    }

    constexpr
    static
    lut_chars
    construct(
        unsigned char ch,
        bool b) noexcept
    {
        return b
            ? lut_chars(ch)
            : lut_chars();
    }

    template<class Pred>
    constexpr
    static
    lut_chars
    construct(
        Pred pred,
        unsigned char ch) noexcept
    {
        return ch == 255
            ? construct(ch, pred(ch))
            : construct(ch, pred(ch)) +
                construct(pred, ch + 1);
    }

    constexpr
    lut_chars() = default;

    constexpr
    lut_chars(
        std::uint64_t m0,
        std::uint64_t m1,
        std::uint64_t m2,
        std::uint64_t m3) noexcept
        : mask_{ m0, m1, m2, m3 }
    {
    }

public:
    /** Constructor.

        This function constructs a character
        set which has as a single member,
        the character `ch`.

        @par Example
        @code
        constexpr lut_chars asterisk( '*' );
        @endcode

        @par Complexity
        Constant.

        @par Exception Safety
        Throws nothing.

        @param ch A character.
    */
    constexpr
    lut_chars(char ch) noexcept
        : mask_ {
            lo(ch) == 0 ? hi(ch) : 0,
            lo(ch) == 1 ? hi(ch) : 0,
            lo(ch) == 2 ? hi(ch) : 0,
            lo(ch) == 3 ? hi(ch) : 0 }
    {
    }

    /** Constructor.

        This function constructs a character
        set which has as members, all of the
        characters present in the null-terminated
        string `s`.

        @par Example
        @code
        constexpr lut_chars digits = "0123456789";
        @endcode

        @par Complexity
        Linear in `::strlen(s)`, or constant
        if `s` is a constant expression.

        @par Exception Safety
        Throws nothing.

        @param s A null-terminated string.
    */
    constexpr
    lut_chars(char const* s) noexcept
        : lut_chars(construct(s))
    {
    }

    /** Constructor.

        This function constructs a character
        set which has as members, every value
        of `char ch` for which the expression
        `pred(ch)` returns `true`.

        @par Example
        @code
        struct is_digit
        {
            constexpr bool
            operator()(char c ) const noexcept
            {
                return c >= '0' && c <= '9';
            }
        };

        constexpr lut_chars digits( is_digit{} );
        @endcode

        @par Complexity
        Linear in `pred`, or constant if
        `pred(ch)` is a constant expression.

        @par Exception Safety
        Throws nothing.

        @param pred The function object to
        use for determining membership in
        the character set.
    */
    template<class Pred
#ifndef BOOST_URL_DOCS
        ,class = typename std::enable_if<
            detail::is_pred<Pred>::value &&
        ! std::is_base_of<
            lut_chars, Pred>::value>::type
#endif
    >
    constexpr
    lut_chars(Pred const& pred) noexcept
        : lut_chars(
            construct(pred, 0))
    {
    }

    /** Return true if ch is in the character set.

        This function returns true if the
        character `ch` is in the set, otherwise
        it returns false.

        @par Complexity
        Constant.

        @par Exception Safety
        Throws nothing.

        @param ch The character to test.
    */
    constexpr
    bool
    operator()(
        unsigned char ch) const noexcept
    {
        return mask_[lo(ch)] & hi(ch);
    }

    /** Return the union of two character sets.

        This function returns a new character
        set which contains all of the characters
        in `cs0` as well as all of the characters
        in `cs`.

        @par Example
        This creates a character set which
        includes all letters and numbers
        @code
        constexpr lut_chars alpha_chars(
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz");

        constexpr lut_chars alnum_chars = alpha_chars + "0123456789";
        @endcode

        @par Complexity
        Constant.

        @return The new character set.

        @param cs0 A character to join
        
        @param cs1 A character to join
    */
    friend
    constexpr
    lut_chars
    operator+(
        lut_chars const& cs0,
        lut_chars const& cs1) noexcept
    {
        return lut_chars(
            cs0.mask_[0] | cs1.mask_[0],
            cs0.mask_[1] | cs1.mask_[1],
            cs0.mask_[2] | cs1.mask_[2],
            cs0.mask_[3] | cs1.mask_[3]);
    }

    /** Return a new character set by subtracting

        This function returns a new character
        set which is formed from all of the
        characters in `cs0` which are not in `cs`.

        @par Example
        This statement declares a character set
        containing all the lowercase letters
        which are not vowels:
        @code
        constexpr lut_chars consonants = lut_chars("abcdefghijklmnopqrstuvwxyz") - "aeiou";
        @endcode

        @par Complexity
        Constant.

        @return The new character set.

        @param cs0 A character set to join.
        
        @param cs1 A character set to join.
    */
    friend
    constexpr
    lut_chars
    operator-(
        lut_chars const& cs0,
        lut_chars const& cs1) noexcept
    {
        return lut_chars(
            cs0.mask_[0] & ~cs1.mask_[0],
            cs0.mask_[1] & ~cs1.mask_[1],
            cs0.mask_[2] & ~cs1.mask_[2],
            cs0.mask_[3] & ~cs1.mask_[3]);
    }

    /** Return a new character set which is the complement of another character set.

        This function returns a new character
        set which contains all of the characters
        that are not in `*this`.

        @par Example
        This statement declares a character set
        containing everything but vowels:
        @code
        constexpr lut_chars not_vowels = ~lut_chars( "aAeEiIoOuU" );
        @endcode

        @par Complexity
        Constant.

        @par Exception Safety
        Throws nothing.

        @return The new character set.
    */
    constexpr
    lut_chars
    operator~() const noexcept
    {
        return lut_chars(
            ~mask_[0],
            ~mask_[1],
            ~mask_[2],
            ~mask_[3]
        );
    }

#ifndef BOOST_URL_DOCS
#ifdef BOOST_URL_USE_SSE2
    char const*
    find_if(
        char const* first,
        char const* last) const noexcept
    {
        return detail::find_if_pred(
            *this, first, last);
    }

    char const*
    find_if_not(
        char const* first,
        char const* last) const noexcept
    {
        return detail::find_if_not_pred(
            *this, first, last);
    }
#endif
#endif
};

} // grammar
} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_GRAMMAR_OPTIONAL_HPP
#define BOOST_URL_GRAMMAR_OPTIONAL_HPP

#include <boost/assert.hpp>

namespace boost {
namespace urls {
namespace grammar {

/** Rule for an optional element.
*/
template<class R>
struct optional
{
    bool has_value = false;
    R v;

    explicit
    operator bool() const noexcept
    {
        return has_value;
    }

    R const&
    operator*() const noexcept
    {
        BOOST_ASSERT(has_value);
        return v;
    }

    friend
    void
    tag_invoke(
        parse_tag const&,
        char const*& it,
        char const* end,
        error_code& ec,
        optional& t)
    {
        auto const start = it;
        if(! (parse)(
            it, end, ec, t.v))
        {
            ec = {};
            it = start;
            t.has_value = false;
            return;
        }
        t.has_value = true;
    }
};

} // bnf
} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_GRAMMAR_RANGE_HPP
#define BOOST_URL_GRAMMAR_RANGE_HPP

#include <boost/static_assert.hpp>
#include <boost/type_traits/make_void.hpp>
#include <cstddef>
#include <iterator>
#include <type_traits>

namespace boost {
namespace urls {
namespace grammar {

/** Alias for `std::true_type` if T satisfies __Range__
*/
#ifdef BOOST_URL_DOCS
template<class T>
using is_range = __see_below__;
#else
template<class T, class = void>
struct is_range : std::false_type {};

template<class T>
struct is_range<T, boost::void_t<
    decltype(typename T::value_type()), // default ctor
    decltype(
    std::declval<bool&>() =
        T::begin(
            std::declval<char const*&>(),
            std::declval<char const*>(),
            std::declval<error_code&>(),
            std::declval<typename
                std::add_lvalue_reference<
                    typename T::value_type>::type>()),
    std::declval<bool&>() =
        T::increment(
            std::declval<char const*&>(),
            std::declval<char const*>(),
            std::declval<error_code&>(),
            std::declval<typename
                std::add_lvalue_reference<
                    typename T::value_type>::type>())
        ) > >
    : std::true_type
{
};
#endif

//------------------------------------------------

class range_
{
    void(*fp_)(
        char const*&,
        char const*,
        error_code&,
        range_&);

    template<class T>
    static
    void
    parse_impl(
        char const*& it,
        char const* end,
        error_code& ec,
        range_& t);

protected:
    template<class T>
    explicit
    range_(T const*) noexcept;

public:
    string_view str;
    std::size_t count;

    template<class T>
    class iterator;

    range_() noexcept
        : fp_(nullptr)
    {
    }

    friend
    void
    tag_invoke(
        parse_tag const&,
        char const*& it,
        char const* end,
        error_code& ec,
        range_& t)
    {
        t.fp_(it, end, ec, t);
    }
};

//------------------------------------------------

/** Rule for a repeating number of rules.
*/
template<class R>
class range_base
{
    string_view s_;
    std::size_t n_ = 0;

protected:
    /** Parse a range
    */
    void
    parse(
        char const*& it,
        char const* end,
        error_code& ec,
        std::size_t N,
        std::size_t M);

public:
#ifdef BOOST_URL_DOCS
    /** A read-only forward iterator
    */
    using iterator = __see_below__;
#else
    class iterator;
#endif

    /** The type of value returned when dereferencing an iterator.
    */
    using value_type = typename R::value_type;

    /** The type of value returned when dereferencing an iterator.
    */
    using reference = typename R::reference;

    /** The type of value returned when dereferencing an iterator.
    */
    using const_reference = reference;

    /** An unsigned integer type used to represent size.
    */
    using size_type = std::size_t;

    /** A signed integer type used to represent differences.
    */
    using difference_type = std::ptrdiff_t;

    /** Copy constructor.
    */
    range_base(range_base const&) = default;

    /** Copy assignment.
    */
    range_base& operator=(range_base const&) = default;

    /** Constructor

        Default constructed ranges are empty.
    */
    range_base() = default;

    /** Return the underlying string.
    */
    string_view
    str() const noexcept
    {
        return s_;
    }

    /** Return an iterator to the beginning.
    */
    iterator
    begin() const;

    /** Return an iterator to the end.
    */
    iterator
    end() const;

    /** Return true if the range contains no elements
    */
    bool
    empty() const noexcept
    {
        return n_ == 0;
    }

    /** Return the number of elements in the range
    */
    std::size_t
    size() const noexcept
    {
        return n_;
    }
};

//------------------------------------------------

/** Rule for a repeating number of rules.

    This rule defines a sequence containing
    at least N and at most M of R.

    @par BNF
    @code
    range           =  <n>*<m>element

    *<m>element     => <0>*<m>element
    <n>*element     => <n>*<inf.>element
    *element        => <0>*<inf.>element
    <n>element      => <n>*<n>element
    [ element ]     => *1( element )
    @endcode

    @tparam R The rule to repeat
    @tparam N The minimum number of repetitions, which may be zero
    @tparam M The maximum number of repetitions.

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc5234#section-3.6"
        >3.6. Variable Repetition: *Rule</a>
*/
template<
    class R,
    std::size_t N = 0,
    std::size_t M = std::size_t(-1)>
class range : public range_base<R>
{
    BOOST_STATIC_ASSERT(
        M >= N ||
        (N == M && N > 0));

public:
    /** Parse a range
    */
    friend
    void
    tag_invoke(
        parse_tag const&,
        char const*& it,
        char const* end,
        error_code& ec,
        range& t)
    {
        t.parse(it, end, ec, N, M);
    }
};

} // grammar
} // urls
} // boost

//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_GRAMMAR_IMPL_RANGE_HPP
#define BOOST_URL_GRAMMAR_IMPL_RANGE_HPP

#include <boost/assert.hpp>
#include <iterator>

namespace boost {
namespace urls {
namespace grammar {

template<class T>
void
range_::
parse_impl(
    char const*& it,
    char const* end,
    error_code& ec,
    range_& t)
{
    typename T::value_type t0;
    auto start = it;
    std::size_t n = 0;
    T::begin(it, end, ec, t0);
    if(ec.failed())
    {
        if(ec == error::end)
        {
            ec = {};
            goto finish;
        }
        return;
    }
    for(;;)
    {
        ++n;
        T::increment(
            it, end, ec, t0);
        if(ec.failed())
        {
            if(ec == error::end)
            {
                ec = {};
                break;
            }
            return;
        }
    }
finish:
    t.str = string_view(
        start, it - start);
    t.count = n;
}

template<class T>
range_::
range_(T const*) noexcept
    : fp_(&range_::parse_impl<T>)
{
    // Type requirements not met!
    BOOST_STATIC_ASSERT(
        is_range<T>::value);
}

//------------------------------------------------

template<class R>
class range_base<R>::iterator
{
    friend class range_base<R>;

    char const* next_ = nullptr;
    char const* end_ = nullptr;
    typename R::reference v_;

    explicit
    iterator(string_view s)
        : next_(s.data())
        , end_(s.data() + s.size())
    {
        error_code ec;
        R::begin(next_, end_, ec, v_);
        BOOST_ASSERT(! ec.failed());
    }

    iterator(
        string_view s,
        int) noexcept
        : next_(nullptr)
        , end_(s.data() + s.size())
    {
    }

public:
    using value_type = typename R::value_type;
    using reference = typename R::reference;
    using pointer = void const*;
    using difference_type = std::ptrdiff_t;
    using iterator_category =
        std::forward_iterator_tag;

    iterator() = default;
    iterator(
        iterator const&) noexcept = default;
    iterator& operator=(
        iterator const&) noexcept = default;

    reference
    operator*() const noexcept
    {
        return v_;
    }

    bool
    operator==(
        iterator other) const noexcept
    {
        return
            next_ == other.next_ &&
            end_ == other.end_;
    }

    bool
    operator!=(
        iterator other) const noexcept
    {
        return
            next_ != other.next_ ||
            end_ != other.end_;
    }

    iterator&
    operator++() noexcept
    {
        BOOST_ASSERT(next_ != nullptr);
        error_code ec;
        R::increment(next_, end_, ec, v_);
        if(ec == error::end)
            next_ = nullptr;
        else
            BOOST_ASSERT(! ec.failed());
        return *this;
    }

    iterator
    operator++(int) noexcept
    {
        auto tmp = *this;
        ++*this;
        return tmp;
    }
};

//------------------------------------------------

template<class R>
auto
range_base<R>::
begin() const ->
    iterator
{
    return iterator(s_);
}

template<class R>
auto
range_base<R>::
end() const ->
    iterator
{
    return iterator(s_, 1);
}

template<class R>
void
range_base<R>::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    std::size_t N,
    std::size_t M)
{
    std::size_t n = 0;
    auto const start = it;
    typename range<R>::reference v;

    R::begin(it, end, ec, v);
    if(ec.failed())
    {
        if(ec != error::end)
            return;
        if(n < N)
        {
            // too few
            ec = error::syntax;
            return;
        }

        // good
        ec = {};
        n_ = n;
        s_ = string_view(
            start, it - start);
        return;
    }

    for(;;)
    {
        ++n;
        R::increment(it, end, ec, v);
        if(ec.failed())
        {
            if(ec != error::end)
                return;
            ec = {};
            break;
        }
        if(n > M)
        {
            // too many
            ec = error::syntax;
            return;
        }
    }

    // good
    n_ = n;
    s_ = string_view(start, it - start);
}

} // grammar
} // urls
} // boost

#endif

#endif
//
// Copyright (c) 2016-2019 Damian Jarek (damian dot jarek93 at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/http_proto
//

#ifndef BOOST_URL_GRAMMAR_TOKEN_HPP
#define BOOST_URL_GRAMMAR_TOKEN_HPP

#include <boost/static_assert.hpp>

namespace boost {
namespace urls {
namespace grammar {

/** Rule for 1 or more characters from a character set

    @par BNF
    @code
    token     = 1*( ch )
    @endcode
*/
template<class CharSet>
struct token
{
    using value_type = std::string;
    using reference = string_view;

    BOOST_STATIC_ASSERT(
        is_charset<CharSet>::value);

    string_view s;

    reference
    operator*() noexcept
    {
        return s;
    }

    friend
    void
    tag_invoke(
        parse_tag const&,
        char const*& it,
        char const* end,
        error_code& ec,
        token& t) noexcept
    {
        auto const start = it;
        static constexpr CharSet cs{};
        if(it == end)
        {
            ec = grammar::error::incomplete;
            return;
        }
        it = (find_if_not)(it, end, cs);
        if(it == start)
        {
            ec = grammar::error::syntax;
            return;
        }
        t.s = string_view(start, it - start);
    }
};

} // grammar
} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_GRAMMAR_UNSIGNED_DECIMAL_HPP
#define BOOST_URL_GRAMMAR_UNSIGNED_DECIMAL_HPP

#include <type_traits>

namespace boost {
namespace urls {
namespace grammar {

/** Rule for an unsigned decimal

    @par BNF
    @code
    unsigned      = "0" / ( ["1"..."9"] *DIGIT )
    @endcode
*/
template<class T>
struct unsigned_decimal
{
    BOOST_STATIC_ASSERT(
        std::is_unsigned<T>::value);

    T u;
    string_view s;

    friend
    void
    tag_invoke(
        parse_tag const&,
        char const*& it,
        char const* end,
        error_code& ec,
        unsigned_decimal& t) noexcept
    {
        parse(it, end, ec, t);
    }

private:
    static
    void
    parse(
        char const*& it,
        char const* end,
        error_code& ec,
        unsigned_decimal& t) noexcept
    {
        if(it == end)
        {
            ec = grammar::error::syntax;
            return;
        }
        if(*it == '0')
        {
            t.s = string_view(
                it, 1);
            ++it;
            t.u = 0;
            return;
        }
        T u = 0;
        auto const start = it;
        constexpr auto div = (T(-1) / 10);
        constexpr auto rem = (T(-1) % 10);
        while(it != end)
        {
            if(! digit_chars(*it))
                break;
            T const dig = *it - '0';
            if( u > div ||
                (u == div && dig > rem))
            {
                ec = grammar::error::overflow;
                return;
            }
            u = static_cast<T>(10 * u + dig);
            ++it;
        }
        t.u = u;
        t.s = string_view(start, it);
    }
};

//------------------------------------------------

} // grammar
} // urls
} // boost

#endif

#endif

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_AUTHORITY_VIEW_HPP
#define BOOST_URL_AUTHORITY_VIEW_HPP

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_HOST_TYPE_HPP
#define BOOST_URL_HOST_TYPE_HPP


namespace boost {
namespace urls {

/** Identifies the type of host in a URL.

    Values of this type are returned by URL views
    and containers to indicate the type of host
    present in a URL.

    @see
        @ref url_view::host.
*/
enum class host_type
{
    // VFALCO 3 space indent or
    // else Doxygen malfunctions

    /**
     * No host is specified.
    */
    none,

    /**
     * A host is specified by reg-name.
    */
    name,

    /**
     * A host is specified by IPv4address.
       See @ref ipv4_address
    */
    ipv4,

    /**
     * A host is specified by IPv6address.

       @see @ref ipv6_address
    */
    ipv6,

    /**
     * A host is specified by IPvFuture.
    */
    ipvfuture
};

} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IPV4_ADDRESS_HPP
#define BOOST_URL_IPV4_ADDRESS_HPP

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2019 Alan Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_CONST_STRING_HPP
#define BOOST_URL_CONST_STRING_HPP


namespace boost {
namespace urls {

/** A constant string with shared ownership

    Objects of this type represent read-only
    strings with ownership of the character
    buffer.

    They are reference counted to allow cheap copies.

    The type is derived from `string_view`,
    which provides compatibility with strings
    in terms of comparisons and conversions.

    However, care must be exercised; undefined
    behavior results if the string_view
    portion of the object is modified
    directly, for example by calling
    `remove_suffix` or `operator=`.

    Slicing, however, is supported, as
    copies of the `string_view` portion
    of the object are valid and remain
    valid for the lifetime of the original
    object.
*/
class const_string : public string_view
{
    static constexpr
    std::size_t
    builtin_capacity = 32;

    struct base;
    struct result;

    union data_t
    {
        base* p_;
        char buf_[builtin_capacity];

        data_t() = default;

        explicit
        data_t(base* p)
            : p_(p)
        {
        }
    };

    data_t data_;

    static constexpr
    bool
    is_small(std::size_t n) noexcept
    {
        return n <= builtin_capacity;
    }

    BOOST_URL_DECL
    explicit
    const_string(
        result const& r) noexcept;

public:
    class factory;

    /** Destructor

        Releases ownership of the string buffer.
        If this is the last owner, the storage
        is deallocated.
    */
    BOOST_URL_DECL
    ~const_string();

    /** Constructor

        Default-constructed objects represent
        the empty string.
    */
    BOOST_URL_DECL
    const_string() noexcept;

    /** Constructor

        Copies do not allocate. The newly
        constructed string acquires shared
        ownership of the underlying character
        buffer.

        @par Exception Safety
        Throws nothing.
    */
    BOOST_URL_DECL
    const_string(
        const_string const& other) noexcept;

    /** Assignment

        Assignment does not allocate. The newly
        assigned string acquires shared
        ownership of the underlying character
        buffer.
    */
    BOOST_URL_DECL
    const_string&
    operator=(
        const_string const& other) noexcept;

    /** Constructor

        Constructs a string with the specified
        size. The function object `init` is
        used to initialize the contents of
        the string, and will be invoked with
        this equivalent signature:

        @code
        void (std::size_t size, char* dest );
        @endcode

        @param size The size of the resulting string

        @param a The allocator to use.

        @param init The function object to invoke.
    */
    template<
        class Allocator,
        class InitFn>
    const_string(
        std::size_t size,
        Allocator const& a,
        InitFn const& init);

    /** Constructor

        Constructs a copy of the string `s`.
        If memory allocation is required, the
        specified allocator `a` will be used.

        @param s The string to copy

        @param a The allocator to use.
    */
    template<class Allocator>
    const_string(
        string_view s,
        Allocator const& a);
};

//------------------------------------------------

/** A producer for constant strings used a specified allocator.

    Objects of this type are used to produce
    constant strings which all use the specified
    allocator.
*/
class const_string::factory
{
    friend class const_string;

    class base;

    base* p_;

    template<class T>
    class impl;

public:
    /** Destructor
    */
    BOOST_URL_DECL
    ~factory();

    /** Constructor

        @par Exception Safety
        Throws nothing.
    */
    BOOST_URL_DECL
    factory(
        factory const& other) noexcept;

    /** Assignment

        @par Exception Safety
        Throws nothing.
    */
    BOOST_URL_DECL
    factory&
    operator=(
        factory const& other) noexcept;

    /** Constructor

        Default constructed factories use
        the standard allocator
        `std::allocator<char>`.

        @par Exception Safety
        Throws nothing.
    */
    BOOST_URL_DECL
    factory() noexcept;

    /** Constructor

        All factories constructed with this
        function will use the standard allocator
        `std::allocator<char>`.
    */
    template<class T>
    explicit
    factory(std::allocator<T> const&)
        : factory()
    {
    }

    /** Constructor

        Creates a factory using the specified
        allocator.
    */
    template<class Allocator>
    explicit
    factory(Allocator const& alloc);

    /** Return a newly constructed string

        Constructs a string with the specified
        size. The function object `init` is
        used to initialize the contents of
        the string, and will be invoked with
        this equivalent signature:

        @code
        void (std::size_t size, char* dest );
        @endcode

        @param n The size of the resulting string

        @param init The function object to invoke.
    */
    template<class InitFn>
    const_string
    operator()(
        std::size_t n,
        InitFn const& init) const;

    /** Return a newly constructed copy of a string

        Constructs a string with the specified
        size. The function object `init` is
        used to initialize the contents of
        the string, and will be invoked with
        this equivalent signature:

        @code
        void (std::size_t size, char* dest );
        @endcode

        @param s The string to copy
    */
    BOOST_URL_DECL
    const_string
    operator()(string_view s) const;
};

} // urls
} // boost

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_STRING_HPP
#define BOOST_URL_IMPL_STRING_HPP

 //
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_OVER_ALLOCATOR_HPP
#define BOOST_URL_DETAIL_OVER_ALLOCATOR_HPP

#include <boost/config.hpp>
#include <boost/assert.hpp>
#include <boost/core/empty_value.hpp>
#include <boost/type_traits/is_final.hpp>
#include <boost/type_traits/type_with_alignment.hpp>
#ifdef BOOST_NO_CXX11_ALLOCATOR
# include <boost/container/allocator_traits.hpp>
#endif
#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>

namespace boost {
namespace urls {
namespace detail {

// This is a workaround for allocator_traits
// implementations which falsely claim C++11
// compatibility.
#ifdef BOOST_NO_CXX11_ALLOCATOR
template<class Alloc>
using allocator_traits =
    boost::container::allocator_traits<Alloc>;
#else
template<class Alloc>
using allocator_traits = std::allocator_traits<Alloc>;
#endif

template<class T, class Allocator>
class over_allocator
    : private empty_value<Allocator>
{
    template<class U, class OtherAlloc>
    friend class over_allocator;

    std::size_t extra_;

public:
    using is_always_equal = std::false_type;
    using value_type = typename
        allocator_traits<typename allocator_traits<
            Allocator>::template rebind_alloc<T>>::value_type;
    using pointer = typename
        allocator_traits<typename allocator_traits<
            Allocator>::template rebind_alloc<T>>::pointer;
    using const_pointer = typename
        allocator_traits<typename allocator_traits<
            Allocator>::template rebind_alloc<T>>::const_pointer;
    using size_type = typename
        allocator_traits<typename allocator_traits<
            Allocator>::template rebind_alloc<T>>::size_type;
    using difference_type = typename
        allocator_traits<typename allocator_traits<
            Allocator>::template rebind_alloc<T>>::difference_type;

    template<class U>
    struct rebind
    {
        using other = over_allocator<U, Allocator>;
    };

    over_allocator(
        std::size_t extra,
        Allocator const& alloc)
        : empty_value<Allocator>(
            boost::empty_init_t{}, alloc)
        , extra_(extra)
    {
    }

    template<class U>
    over_allocator(over_allocator<U, Allocator> const& other) noexcept
        : empty_value<Allocator>(
            boost::empty_init_t{}, other.get())
        , extra_(other.extra_)
    {
    }

    pointer
    allocate(size_type n)
    {
        BOOST_ASSERT(n == 1);
        using U = typename boost::type_with_alignment<
            alignof(value_type)>::type;
        auto constexpr S = sizeof(U);
        using A = typename allocator_traits<
            Allocator>::template rebind_alloc<U>;
        A a(this->get());
        return reinterpret_cast<pointer>(
            std::allocator_traits<A>::allocate(a,
                (n * sizeof(value_type) + extra_ + S - 1) / S));
    }

    void
    deallocate(pointer p, size_type n)
    {
        BOOST_ASSERT(n == 1);
        using U = typename boost::type_with_alignment<
            alignof(value_type)>::type;
        auto constexpr S = sizeof(U);
        using A = typename allocator_traits<
            Allocator>::template rebind_alloc<U>;
        A a{this->get()};
        std::allocator_traits<A>::deallocate(a,
            reinterpret_cast<U*>(p),
                (n * sizeof(value_type) + extra_ + S - 1) / S);
    }

#if defined(BOOST_LIBSTDCXX_VERSION) && BOOST_LIBSTDCXX_VERSION < 60000
    template<class U, class... Args>
    void
    construct(U* ptr, Args&&... args)
    {
        ::new((void*)ptr) U(std::forward<Args>(args)...);
    }

    template<class U>
    void
    destroy(U* ptr)
    {
        ptr->~U();
    }
#endif

    template<class U>
    friend
    bool
    operator==(
        over_allocator const& lhs,
        over_allocator<U, Allocator> const& rhs)
    {
        return
            lhs.get() == rhs.get() &&
            lhs.extra_ == rhs.extra_;
    }

    template<class U>
    friend
    bool
    operator!=(
        over_allocator const& lhs,
        over_allocator<U, Allocator> const& rhs)
    {
        return ! (lhs == rhs);
    }
};

} // detail
} // urls
} // boost

#endif
#include <atomic>
#include <memory>
#include <new>

namespace boost {
namespace urls {

struct const_string::result
{
    base* p;
    char* data;
    std::size_t size;
};

//------------------------------------------------

struct const_string::base
{
    std::atomic<
        std::uint32_t> refs{1};

    void
    release(std::size_t size) noexcept
    {
        if(--refs > 0)
            return;
        destroy(size);
    }

    virtual
    void
    destroy(std::size_t size) noexcept = 0;
};

//------------------------------------------------

class const_string::factory::base
{
public:
    std::atomic<
        std::uint32_t> refs{1};

    void
    release() noexcept
    {
        if(--refs > 0)
            return;
        destroy();
    }

    virtual
    void
    destroy() noexcept = 0;

    virtual
    result
    construct(std::size_t size) const = 0;
};

//------------------------------------------------

template<class Allocator>
class const_string::factory::impl final
    : public base
{
    friend class const_string;

    Allocator a_;

    struct string final : const_string::base
    {
        Allocator a_;

        string(Allocator const& a) noexcept
            : a_(a)
        {
        }

        void
        destroy(std::size_t size) noexcept override
        {
            detail::over_allocator<
                string, Allocator> a(size, a_);
            this->~string();
            a.deallocate(this, 1);
        }
    };

public:
    explicit
    impl(Allocator const& a) noexcept
        : a_(a)
    {
    }

    void
    destroy() noexcept override
    {
        typename detail::allocator_traits<
            Allocator>::template rebind_alloc<
                impl> a(a_);
        this->~impl();
        a.deallocate(this, 1);
    }

    result
    construct(std::size_t size) const override
    {
        // VFALCO Should we do something special when size==0?
        detail::over_allocator<
            string, Allocator> a(size, a_);
        auto p = ::new(a.allocate(1)) string(a_);
        return result{ p, reinterpret_cast<
            char*>(p + 1), size };
    }
};

//------------------------------------------------

template<class Allocator>
const_string::
factory::
factory(Allocator const& a)
    : p_([&a]
    {
        using A = typename
            detail::allocator_traits<
                Allocator>::template rebind_alloc<
                    factory::impl<Allocator>>;
        return ::new(A(a).allocate(1)
            ) factory::impl<Allocator>(a);
    }())
{
}

template<class InitFn>
const_string
const_string::
factory::
operator()(
    std::size_t n,
    InitFn const& init) const
{
    // VFALCO Should we do something special when n==0?
    if (is_small(n))
        return const_string(n,
            std::allocator<char>{}, init);
    auto r = p_->construct(n);
    const_string s(r);
    init(n, r.data);
    return s;
}

//------------------------------------------------

template<class Allocator>
const_string::
const_string(
    string_view s,
    Allocator const& a)
{
    if (is_small(s.size()))
    {
        if (!s.empty())
            std::memcpy(data_.buf_,
                s.data(), s.size());
        static_cast<string_view&>(*this
            ) = { data_.buf_, s.size()};
        return;
    }
    auto r = factory::impl<Allocator>(
        a).construct(s.size());
    static_cast<string_view&>(
        *this) = { r.data, r.size };
    std::memcpy(r.data, s.data(), s.size());
    data_.p_ = r.p;
}

template<
    class Allocator,
    class InitFn>
const_string::
const_string(
    std::size_t size,
    Allocator const& a,
    InitFn const& init)
{
    if (is_small(size))
    {
        init(size, data_.buf_);
        static_cast<string_view&>(
            *this) = { data_.buf_, size };
        return;
    }
    auto r = factory::impl<
        Allocator>(a).construct(size);
    static_cast<string_view&>(
        *this) = { r.data, r.size };
    init(size, r.data);
    data_.p_ = r.p;
}

} // urls
} // boost

#endif

#endif
#include <array>
#include <cstdint>
#include <iosfwd>

namespace boost {
namespace urls {

/** An IP version 4 style address.

    Objects of this type are used to construct,
    parse, and manipulate IP version 6 addresses.

    @par BNF
    @code
    IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet

    dec-octet   = DIGIT                 ; 0-9
                / %x31-39 DIGIT         ; 10-99
                / "1" 2DIGIT            ; 100-199
                / "2" %x30-34 DIGIT     ; 200-249
                / "25" %x30-35          ; 250-255
    @endcode

    @par Specification
    @li <a href="https://en.wikipedia.org/wiki/IPv4"
        >IPv4 (Wikipedia)</a>
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.2"
        >3.2.2. Host (rfc3986)</a>

    @see
        @ref parse_ipv4_address,
        @ref ipv6_address.
*/
class ipv4_address
{
public:
    /** The number of characters in the longest possible IPv4 string.

        The longest ipv4 address string is "255.255.255.255".
    */
    static
    constexpr
    std::size_t max_str_len = 15;

    /** The type used to represent an address as an unsigned integer
    */
    using uint_type =
        std::uint_least32_t;

    /** The type used to represent an address as an array of bytes
    */
    using bytes_type =
        std::array<unsigned char, 4>;

    /** Constructor.
    */
    ipv4_address() = default;

    /** Constructor.
    */
    ipv4_address(
        ipv4_address const&) = default;

    /** Copy Assignment.
    */
    ipv4_address&
    operator=(
        ipv4_address const&) = default;

    //
    //---
    //

    /** Construct from an unsigned integer.

        This function constructs an address from
        the unsigned integer `u`, where the most
        significant byte forms the first octet
        of the resulting address.

        @param u The integer to construct from.
    */
    BOOST_URL_DECL
    explicit
    ipv4_address(
        uint_type u) noexcept;

    /** Construct from an array of bytes.

        This function constructs an address
        from the array in `bytes`, which will
        be interpreted in big-endian.

        @param bytes The value to construct from.
    */
    BOOST_URL_DECL
    ipv4_address(
        bytes_type const& bytes) noexcept;

    /** Construct from a string.

        This function constructs an address from
        the string `s`, which must contain a valid
        IPv4 address string or else an exception
        is thrown.

        @note For a non-throwing parse function,
        use @ref parse_ipv4_address.

        @throw std::invalid_argument parse error.

        @param s The string to parse.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.2"
            >3.2.2. Host (rfc3986)</a>

        @see
            @ref parse_ipv4_address.
    */
    BOOST_URL_DECL
    ipv4_address(
        string_view s);

    /** Return the address as bytes, in network byte order.
    */
    BOOST_URL_DECL
    bytes_type
    to_bytes() const noexcept;

    /** Return the address as an unsigned integer.
    */
    BOOST_URL_DECL
    uint_type
    to_uint() const noexcept;

    /** Return the address as a string in dotted decimal format

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.

        @param a An optional allocator the returned
        string will use. If this parameter is omitted,
        the default allocator is used.

        @return A @ref const_string using the
        specified allocator.
    */
    template<class Allocator =
        std::allocator<char>>
    const_string
    to_string(Allocator const& a = {}) const
    {
        char buf[max_str_len];
        auto const n = print_impl(buf);
        return const_string(
            string_view(buf, n), a);
    }

    /** Write a dotted decimal string representing the address to a buffer

        The resulting buffer is not null-terminated.

        @throw std::length_error `dest_size < ipv4_address::max_str_len`

        @return The formatted string

        @param dest The buffer in which to write,
        which must have at least `dest_size` space.

        @param dest_size The size of the output buffer.
    */
    BOOST_URL_DECL
    string_view
    to_buffer(
        char* dest,
        std::size_t dest_size) const;

    /** Return true if the address is a loopback address
    */
    BOOST_URL_DECL
    bool
    is_loopback() const noexcept;

    /** Return true if the address is unspecified
    */
    BOOST_URL_DECL
    bool
    is_unspecified() const noexcept;

    /** Return true if the address is a multicast address
    */
    BOOST_URL_DECL
    bool
    is_multicast() const noexcept;

    /** Return true if two addresses are equal
    */
    friend
    bool
    operator==(
        ipv4_address const& a1,
        ipv4_address const& a2) noexcept
    {
        return a1.addr_ == a2.addr_;
    }

    /** Return true if two addresses are not equal
    */
    friend
    bool
    operator!=(
        ipv4_address const& a1,
        ipv4_address const& a2) noexcept
    {
        return a1.addr_ != a2.addr_;
    }

    /** Return an address object that represents any address
    */
    static
    ipv4_address
    any() noexcept
    {
        return ipv4_address();
    }

    /** Return an address object that represents the loopback address
    */
    static
    ipv4_address
    loopback() noexcept
    {
        return ipv4_address(0x7F000001);
    }

    /** Return an address object that represents the broadcast address
    */
    static
    ipv4_address
    broadcast() noexcept
    {
        return ipv4_address(0xFFFFFFFF);
    }

    /** Customization point for parsing an IPv4 address.
    */
    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* const end,
        error_code& ec,
        ipv4_address& t) noexcept
    {
        parse(it, end, ec, t);
    }

private:
    BOOST_URL_DECL
    static
    void
    parse(
        char const*& it,
        char const* const end,
        error_code& ec,
        ipv4_address& t) noexcept;

    friend class ipv6_address;

    BOOST_URL_DECL
    std::size_t
    print_impl(
        char* dest) const noexcept;

    uint_type addr_ = 0;
};

//------------------------------------------------

/** Format the address to an output stream.

    IPv4 addresses written to output streams
    are written in their dotted decimal format.

    @param os The output stream.

    @param addr The address to format.
*/
BOOST_URL_DECL
std::ostream&
operator<<(
    std::ostream& os,
    ipv4_address const& addr);

/** Return an IPv4 address from an IP address string in dotted decimal form
*/
BOOST_URL_DECL
result<ipv4_address>
parse_ipv4_address(
    string_view s) noexcept;

} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IPV6_ADDRESS_HPP
#define BOOST_URL_IPV6_ADDRESS_HPP

#include <array>
#include <cstdint>
#include <iosfwd>

namespace boost {
namespace urls {

#ifndef BOOST_URL_DOCS
class ipv4_address;
#endif

/** An IP version 6 style address.

    Objects of this type are used to construct,
    parse, and manipulate IP version 6 addresses.

    @par BNF
    @code
    IPv6address =                            6( h16 ":" ) ls32
                /                       "::" 5( h16 ":" ) ls32
                / [               h16 ] "::" 4( h16 ":" ) ls32
                / [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
                / [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
                / [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32
                / [ *4( h16 ":" ) h16 ] "::"              ls32
                / [ *5( h16 ":" ) h16 ] "::"              h16
                / [ *6( h16 ":" ) h16 ] "::"

    ls32        = ( h16 ":" h16 ) / IPv4address
                ; least-significant 32 bits of address

    h16         = 1*4HEXDIG
                ; 16 bits of address represented in hexadecimal
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc4291"
        >IP Version 6 Addressing Architecture (rfc4291)</a>
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.2"
        >3.2.2. Host (rfc3986)</a>

    @see
        @ref ipv4_address,
        @ref parse_ipv6_address.
*/
class ipv6_address
{
public:
    /** The number of characters in the longest possible IPv6 string.

        The longest IPv6 address is:
        @code
        ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff
        @endcode

        @see
            @ref to_buffer.
    */
    // ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff
    // ::ffff:255.255.255.255
    // 12345678901234567890123456789012345678901234567890
    //          1         2         3        4
    static
    constexpr
    std::size_t max_str_len = 49;

    /** The type used to represent an address as an array of bytes.

        Octets are stored in network byte order.
    */
    using bytes_type = std::array<
        unsigned char, 16>;

    /** Constructor.

        Default constructed objects represent
        the unspecified address.

        @li <a href="https://datatracker.ietf.org/doc/html/rfc4291#section-2.5.2"
            >2.5.2. The Unspecified Address</a>

        @see
            @ref is_unspecified
    */
    ipv6_address() = default;

    /** Constructor.
    */
    ipv6_address(
        ipv6_address const&) = default;

    /** Copy Assignment
    */
    ipv6_address&
    operator=(
        ipv6_address const&) = default;

    /** Construct from an array of bytes.

        This function constructs an address
        from the array in `bytes`, which will
        be interpreted in big-endian.

        @param bytes The value to construct from.
    */
    BOOST_URL_DECL
    ipv6_address(
        bytes_type const& bytes) noexcept;

    /** Construct from an IPv4 address.

        This function constructs an IPv6 address
        from the IPv4 address `addr`. The resulting
        address is an IPv4-Mapped IPv6 Address.

        @param addr The address to construct from.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc4291#section-2.5.5.2"
            >2.5.5.2. IPv4-Mapped IPv6 Address (rfc4291)</a>
    */
    BOOST_URL_DECL
    ipv6_address(
        ipv4_address const& addr) noexcept;

    /** Construct from a string.

        This function constructs an address from
        the string `s`, which must contain a valid
        IPv6 address string or else an exception
        is thrown.

        @note For a non-throwing parse function,
        use @ref parse_ipv6_address.

        @throw std::invalid_argument parse error.

        @param s The string to parse.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.2"
            >3.2.2. Host (rfc3986)</a>

        @see
            @ref parse_ipv6_address.
    */
    BOOST_URL_DECL
    ipv6_address(
        string_view s);

    /** Return the address as bytes, in network byte order
    */
    bytes_type
    to_bytes() const noexcept
    {
        return addr_;
    }

    /** Return the address as a string.

        The returned string will not
        contain surrounding square brackets.

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.

        @param a An optional allocator the returned
        string will use. If this parameter is omitted,
        the default allocator is used.

        @return A @ref const_string using the
        specified allocator.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc4291#section-2.2">
            2.2. Text Representation of Addresses (rfc4291)</a>
    */
    template<class Allocator =
        std::allocator<char>>
    const_string
    to_string(Allocator const& a = {}) const
    {
        char buf[max_str_len];
        auto const n = print_impl(buf);
        return const_string(
            string_view(buf, n), a);
    }

    /** Write a dotted decimal string representing the address to a buffer

        The resulting buffer is not null-terminated.

        @throw std::length_error `dest_size < ipv6_address::max_str_len`

        @return The formatted string

        @param dest The buffer in which to write,
        which must have at least `dest_size` space.

        @param dest_size The size of the output buffer.
    */
    BOOST_URL_DECL
    string_view
    to_buffer(
        char* dest,
        std::size_t dest_size) const;

    /** Return true if the address is unspecified

        The address 0:0:0:0:0:0:0:0 is called the
        unspecified address. It indicates the
        absence of an address.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc4291#section-2.5.2">
            2.5.2. The Unspecified Address (rfc4291)</a>
    */
    BOOST_URL_DECL
    bool
    is_unspecified() const noexcept;

    /** Return true if the address is a loopback address

        The unicast address 0:0:0:0:0:0:0:1 is called
        the loopback address. It may be used by a node
        to send an IPv6 packet to itself.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc4291#section-2.5.3">
            2.5.3. The Loopback Address (rfc4291)</a>
    */
    BOOST_URL_DECL
    bool
    is_loopback() const noexcept;

    /** Return true if the address is a mapped IPv4 address

        This address type is used to represent the
        addresses of IPv4 nodes as IPv6 addresses.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc4291#section-2.5.5.2">
            2.5.5.2. IPv4-Mapped IPv6 Address (rfc4291)</a>
    */
    BOOST_URL_DECL
    bool
    is_v4_mapped() const noexcept;

    /** Return true if two addresses are equal
    */
    BOOST_URL_DECL
    friend
    bool
    operator==(
        ipv6_address const& a1,
        ipv6_address const& a2) noexcept;

    /** Return true if two addresses are not equal
    */
    friend
    bool
    operator!=(
        ipv6_address const& a1,
        ipv6_address const& a2) noexcept
    {
        return !( a1 == a2 );
    }

    /** Return an address object that represents the loopback address

        The unicast address 0:0:0:0:0:0:0:1 is called
        the loopback address. It may be used by a node
        to send an IPv6 packet to itself.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc4291#section-2.5.3">
            2.5.3. The Loopback Address (rfc4291)</a>
    */
    BOOST_URL_DECL
    static
    ipv6_address
    loopback() noexcept;

    /** Parse a string containing an IPv6 address.

        This function overload is used with
        @ref grammar::parse.

        @param it A pointer to the beginning of
        the string to be parsed. Upon return, this
        will point to one past the last character
        visited.

        @param end A pointer to one past the last
        character in the string to be parsed.

        @param ec Set to the error, if any occurred.

        @param t Set to the result upon success.
    */
    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* const end,
        error_code& ec,
        ipv6_address& t) noexcept
    {
        parse(it, end, ec, t);
    }

private:
    BOOST_URL_DECL
    static
    void
    parse(
        char const*& it,
        char const* const end,
        error_code& ec,
        ipv6_address& t) noexcept;

    BOOST_URL_DECL
    std::size_t
    print_impl(
        char* dest) const noexcept;

    bytes_type addr_{};
};

//------------------------------------------------

/** Format the address to an output stream

    This function writes the address to an
    output stream using standard notation.

    @return The output stream, for chaining.

    @param os The output stream to write to.

    @param addr The address to write.
*/
BOOST_URL_DECL
std::ostream&
operator<<(
    std::ostream& os,
    ipv6_address const& addr);

/** Parse a string containing an IPv6 address.

    This function attempts to parse the string
    as an IPv6 address and returns a result
    containing the address upon success, or
    an error code if the string does not contain
    a valid IPv6 address.

    @par Exception Safety
    Throws nothing.

    @return A result containing the address.

    @param s The string to parse.
*/
BOOST_URL_DECL
result<ipv6_address>
parse_ipv6_address(
    string_view s) noexcept;

} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_PCT_ENCODING_HPP
#define BOOST_URL_PCT_ENCODING_HPP

//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_CHARSETS_HPP
#define BOOST_URL_RFC_CHARSETS_HPP

#include <cstdint>

namespace boost {
namespace urls {

/** The unreserved character set type.

    Objects of this type are invocable
    with this equivalent signature:

    @code
    bool( char ch ) const noexcept;
    @endcode

    The function object returns `true` when
    `ch` is a member of the character set,
    and `false` otherwise. This type satisfies
    the <em>CharSet</em> requirements.

    @par BNF
    @code
    unreserved    = ALPHA / DIGIT / "-" / "." / "_" / "~"
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-2.3"
        >2.3. Unreserved Characters (rfc3986)</a>

    @see
        @ref unreserved_chars
*/
#ifdef BOOST_URL_DOCS
using unreserved_chars_t = see_below;
#else
struct unreserved_chars_t : grammar::lut_chars
{
    constexpr
    unreserved_chars_t() noexcept
        : grammar::lut_chars(
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "0123456789"
            "-._~")
    {
    }
};
#endif

/** The unreserved character set.

    The object is invocable with
    this equivalent signature:

    @code
    bool( char ch ) const noexcept;
    @endcode

    The function object returns `true` when
    `ch` is a member of the character set,
    and `false` otherwise. The type of this
    object satisfies the <em>CharSet</em>
    requirements.

    @par BNF
    @code
    unreserved    = ALPHA / DIGIT / "-" / "." / "_" / "~"
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-2.3"
        >2.3. Unreserved Characters (rfc3986)</a>

    @see
        @ref unreserved_chars_t
*/
constexpr unreserved_chars_t unreserved_chars{};

//------------------------------------------------

/** The reserved character set type.

    Objects of this type are invocable
    with this equivalent signature:

    @code
    bool( char ch ) const noexcept;
    @endcode

    The function object returns `true` when
    `ch` is a member of the character set,
    and `false` otherwise. This type satisfies
    the <em>CharSet</em> requirements.

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-2.3"
        >2.3. Unreserved Characters (rfc3986)</a>

    @see
        @ref reserved_chars,
        @ref unreserved_chars,
        @ref unreserved_chars_t.
*/
#ifdef BOOST_URL_DOCS
using reserved_chars_t = see_below;
#else
struct reserved_chars_t : grammar::lut_chars
{
    constexpr
    reserved_chars_t() noexcept
        : grammar::lut_chars(~unreserved_chars)
    {
    }
};
#endif

/** The reserved character set.

    The object is invocable with
    this equivalent signature:

    @code
    bool( char ch ) const noexcept;
    @endcode

    The function object returns `true` when
    `ch` is a member of the character set,
    and `false` otherwise. The type of this
    object satisfies the <em>CharSet</em>
    requirements.

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-2.3"
        >2.3. Unreserved Characters (rfc3986)</a>

    @see
        @ref reserved_chars_t,
        @ref unreserved_chars,
        @ref unreserved_chars_t.
*/
constexpr reserved_chars_t reserved_chars{};

//------------------------------------------------

/** The gen-delims character set type.

    Objects of this type are invocable
    with this equivalent signature:

    @code
    bool( char ch ) const noexcept;
    @endcode

    The function object returns `true` when
    `ch` is a member of the character set,
    and `false` otherwise. This type satisfies
    the <em>CharSet</em> requirements.

    @par BNF
    @code
    gen-delims    = ":" / "/" / "?" / "#" / "[" / "]" / "@"
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-2.2"
        >2.2. Reserved Characters (rfc3986)</a>

    @see
        @ref gen_delim_chars
*/
#ifdef BOOST_URL_DOCS
using gen_delim_chars_t = see_below;
#else
struct gen_delim_chars_t : grammar::lut_chars
{
    constexpr
    gen_delim_chars_t() noexcept
        : grammar::lut_chars(
            ":/?#[]@")
    {
    }
};
#endif

/** The gen-delims character set.

    The object is invocable with
    this equivalent signature:

    @code
    bool( char ch ) const noexcept;
    @endcode

    The function object returns `true` when
    `ch` is a member of the character set,
    and `false` otherwise. The type of this
    object satisfies the <em>CharSet</em>
    requirements.

    @par BNF
    @code
    gen-delims    = ":" / "/" / "?" / "#" / "[" / "]" / "@"
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-2.2"
        >2.2. Reserved Characters (rfc3986)</a>

    @see
        @ref gen_delim_chars_t
*/
constexpr gen_delim_chars_t gen_delim_chars{};

//------------------------------------------------

/** The sub-delims character set type.

    Objects of this type are invocable
    with this equivalent signature:

    @code
    bool( char ch ) const noexcept;
    @endcode

    The function object returns `true` when
    `ch` is a member of the character set,
    and `false` otherwise. This type satisfies
    the <em>CharSet</em> requirements.

    @par BNF
    @code
    sub-delims    = "!" / "$" / "&" / "'" / "(" / ")"
                  / "*" / "+" / "," / ";" / "="
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-2.2"
        >2.2. Reserved Characters (rfc3986)</a>

    @see
        @ref subdelim_chars
*/
#ifdef BOOST_URL_DOCS
using subdelim_chars_t = see_below;
#else
struct subdelim_chars_t : grammar::lut_chars
{
    constexpr
    subdelim_chars_t() noexcept
        : grammar::lut_chars(
            "!$&()*+,;=\x27")
    {
    }
};
#endif

/** The sub-delims character set.

    The object is invocable with
    this equivalent signature:

    @code
    bool( char ch ) const noexcept;
    @endcode

    The function object returns `true` when
    `ch` is a member of the character set,
    and `false` otherwise. The type of this
    object satisfies the <em>CharSet</em>
    requirements.

    @par BNF
    @code
    sub-delims    = "!" / "$" / "&" / "'" / "(" / ")"
                  / "*" / "+" / "," / ";" / "="
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-2.2"
        >2.2. Reserved Characters (rfc3986)</a>

    @see
        @ref subdelim_chars_t
*/
constexpr subdelim_chars_t subdelim_chars{};

//------------------------------------------------

/** The pchars character set type.

    Objects of this type are invocable
    with this equivalent signature:

    @code
    bool( char ch ) const noexcept;
    @endcode

    The function object returns `true` when
    `ch` is a member of the character set,
    and `false` otherwise. This type satisfies
    the <em>CharSet</em> requirements.

    @par BNF
    @code
    pchar         = unreserved / pct-encoded / sub-delims / ":" / "@"
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.3"
        >3.3. Path (rfc3986)</a>

    @see
        @ref pchars
*/
#ifdef BOOST_URL_DOCS
using pchars_t = see_below;
#else
struct pchars_t : grammar::lut_chars
{
    constexpr
    pchars_t() noexcept
        : grammar::lut_chars(
            unreserved_chars +
            subdelim_chars +
            ':' + '@')
    {
    }
};
#endif

/** The pchar character set.

    The object is invocable with
    this equivalent signature:

    @code
    bool( char ch ) const noexcept;
    @endcode

    The function object returns `true` when
    `ch` is a member of the character set,
    and `false` otherwise. The type of this
    object satisfies the <em>CharSet</em>
    requirements.

    @par BNF
    @code
    pchar         = unreserved / pct-encoded / sub-delims / ":" / "@"
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.3"
        >3.3. Path (rfc3986)</a>

    @see
        @ref pchars_t
*/
constexpr pchars_t pchars{};

//------------------------------------------------

/** The query character set type.

    Objects of this type are invocable
    with this equivalent signature:

    @code
    bool( char ch ) const noexcept;
    @endcode

    The function object returns `true` when
    `ch` is a member of the character set,
    and `false` otherwise. This type satisfies
    the <em>CharSet</em> requirements.

    @par BNF
    @code
    query         = *( pchar / "/" / "?" )
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.4"
        >3.4. Query (rfc3986)</a>

    @see
        @ref query_chars_t
*/
#ifdef BOOST_URL_DOCS
using query_chars_t = see_below;
#else
struct query_chars_t : grammar::lut_chars
{
    constexpr
    query_chars_t() noexcept
        : grammar::lut_chars(
            pchars + '/' + '?')
    {
    }
};
#endif

/** The query character set.

    The object is invocable with
    this equivalent signature:

    @code
    bool( char ch ) const noexcept;
    @endcode

    The function object returns `true` when
    `ch` is a member of the character set,
    and `false` otherwise. The type of this
    object satisfies the <em>CharSet</em>
    requirements.

    @par BNF
    @code
    query         = *( pchar / "/" / "?" )
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.4"
        >3.4. Query (rfc3986)</a>

    @see
        @ref query_chars_t
*/
constexpr query_chars_t query_chars{};

//------------------------------------------------

/** The fragment character set type.

    Objects of this type are invocable
    with this equivalent signature:

    @code
    bool( char ch ) const noexcept;
    @endcode

    The function object returns `true` when
    `ch` is a member of the character set,
    and `false` otherwise. This type satisfies
    the <em>CharSet</em> requirements.

    @par BNF
    @code
    fragment      = *( pchar / "/" / "?" )
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.5"
        >3.5. Fragment (rfc3986)</a>

    @see
        @ref fragment_chars
*/
#ifdef BOOST_URL_DOCS
using fragment_chars_t = see_below;
#else
struct fragment_chars_t : grammar::lut_chars
{
    constexpr
    fragment_chars_t() noexcept
        : grammar::lut_chars(
            pchars + '/' + '?')
    {
    }
};
#endif

/** The fragment character set.

    The object is invocable with
    this equivalent signature:

    @code
    bool( char ch ) const noexcept;
    @endcode

    The function object returns `true` when
    `ch` is a member of the character set,
    and `false` otherwise. The type of this
    object satisfies the <em>CharSet</em>
    requirements.

    @par BNF
    @code
    fragment      = *( pchar / "/" / "?" )
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.5"
        >3.5. Fragment (rfc3986)</a>

    @see
        @ref fragment_chars_t
*/
constexpr fragment_chars_t fragment_chars{};

} // urls
} // boost

#endif
#include <memory>

namespace boost {
namespace urls {

/** Options for removing percent-encoding from strings

    @see
        @ref pct_decode,
        @ref pct_decode_bytes_unchecked,
        @ref pct_decode_unchecked,
        @ref validate_pct_encoding.
*/
struct pct_decode_opts
{
    /** True if null characters are allowed in decoded output
    */
    bool allow_null = true;

    /** True if PLUS ('+') decodes into SP (space, ' ')

        @par Specification
        @li <a href="https://www.w3.org/TR/html401/interact/forms.html#h-17.13.4.1">
            application/x-www-form-urlencoded (w3.org)</a>
    */
    bool plus_to_space = true;

    /** True if decoding a non-normal string is an error
    */
    bool non_normal_is_error = false;
};

/** Validate a percent encoded string and return the number of decoded bytes

    This function examines the characters in
    the string to determine the number of bytes
    necessary if the string were to be percent
    decoded using the given options. No encoding
    is actually performed. Since not all encoded
    strings are valid, this function also performs
    checking to ensure that the encoding is valid
    for the character set, setting the error if
    the string is invalid.

    @par Example 1
    This validates and calculates the decoded length of a valid percent-encoded string.
    @code
    error_code ec;
    std::size_t decoded_size = validate_pct_encoding( "Program%20Files",
            ec, pct_decode_opts{}, pchars );
    assert( ! ec.failed() );
    assert( decoded_size == 13 );
    @endcode

    @par Example 2
    This shows how validation can fail using an error code.
    @code
    error_code ec;
    std::size_t decoded_size = validate_pct_encoding( "bad%escape",
            ec, pct_decode_opts{}, pchars );
    assert( ec.failed() );
    @endcode

    @par Exception Safety
    Throws nothing.

    @return The number of bytes needed, excluding
    any null terminator.

    @param s The percent-encoded string to analyze.

    @param ec Set to the error, if any occurred.

    @param opt The options for decoding. If this
    parameter is omitted, the default options
    will be used.

    @param cs An optional character set to use.
    This type must satisfy the requirements
    of <em>CharSet</em>. If this parameter is
    omitted, then no characters are considered
    special. The character set is ignored if
    `opt.non_normal_is_error == false`.

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-2.1">
        2.1. Percent-Encoding (rfc3986)</a>

    @see
        @ref pct_decode,
        @ref pct_decode_bytes_unchecked,
        @ref pct_decode_opts,
        @ref pct_decode_to_value,
        @ref pct_decode_unchecked,
        @ref reserved_chars_t.
*/
template<
    class CharSet = reserved_chars_t>
std::size_t
validate_pct_encoding(
    string_view s,
    error_code& ec,
    pct_decode_opts const& opt = {},
    CharSet const& cs = {}) noexcept;

/** Write a string with percent-decoding into a buffer.

    This function applies percent-decoding to
    the given percent-encoded string, by
    converting escape sequences into their
    character equivalent.
    The function returns the number of bytes
    written to the destination buffer, which
    may be less than the size of the output
    area.

    @par Example
    @code
    char *dest = new char[MAX_LENGTH];
    error_code ec;
    std::size_t decoded_size = pct_decode( dest, dest + MAX_LENGTH,
            "Program%20Files", ec, pct_decode_opts{}, pchars);

    assert( ! ec.failed() );
    assert( decoded_size == 13 );
    assert( strncmp( "Program Files", dest, decoded_size ) == 0 );
    @endcode

    @par Exception Safety
    Throws nothing.

    @return The number of bytes written to
    the destination buffer, which does not
    include any null termination.

    @param dest A pointer to the
    beginning of the output buffer.

    @param end A pointer to one past the end
    of the output buffer.

    @param s The string to decode.

    @param ec Set to the error, if any
    occurred. If the destination buffer
    is too small to hold the result, `ec`
    is set to @ref error::no_space.

    @param opt The options for decoding. If
    this parameter is omitted, the default
    options will be used.

    @param cs The character set to use.

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-2.1"
        >2.1. Percent-Encoding (rfc3986)</a>

    @see
        @ref pct_decode_bytes_unchecked,
        @ref pct_decode_opts,
        @ref pct_decode_to_value.
        @ref pct_decode_unchecked.
        @ref validate_pct_encoding.
*/
template<
    class CharSet = reserved_chars_t>
std::size_t
pct_decode(
    char* dest,
    char const* end,
    string_view s,
    error_code& ec,
    pct_decode_opts const& opt = {},
    CharSet const& cs = {}) noexcept;

/** Return a string with percent-decoding applied.

    This function applies percent-decoding to
    the given percent-encoded string, by
    converting escape sequences into their
    character equivalent.
    The result is returned as a string using
    the optionally specified allocator.

    @par Example
    @code
    std::string result = pct_decode( "Program%20Files", pct_decode_opts{}, pchars );
    assert( result == "Program Files" );
    @endcode

    @par Exception Safety
    Throws on invalid input.
    Calls to allocate may throw.

    @return A `std::basic_string` holding
    the decoded result. If the default
    allocator is used, the return type is
    `std::string`.

    @param s The string to decode.

    @param opt The options for decoding. If
    this parameter is omitted, the default
    options will be used.

    @param cs An optionally specified
    character set to use. If this parameter
    is omitted, all characters are considered
    unreserved.

    @param a An optional allocator the returned
    string will use. If this parameter is omitted,
    the default allocator is used.

    @throws std::invalid_argument invalid input.

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-2.1"
        >2.1. Percent-Encoding (rfc3986)</a>

    @see
        @ref pct_decode_bytes_unchecked,
        @ref pct_decode_opts,
        @ref pct_decode_to_value.
        @ref pct_decode_unchecked.
        @ref validate_pct_encoding.
*/
template<
    class CharSet = reserved_chars_t,
    class Allocator = std::allocator<char> >
std::basic_string<char,
    std::char_traits<char>,
        Allocator>
pct_decode(
    string_view s,
    pct_decode_opts const& opt = {},
    CharSet const& cs = {},
    Allocator const& a = {});

/** Return a string with percent-decoding applied.

    This function applies percent-decoding to
    the given percent-encoded string, by
    converting escape sequences into their
    character equivalent.
    The result is returned as a @ref const_string
    using the optionally specified allocator.

    @par Example
    @code
    const_string result = pct_decode_to_value( "Program%20Files", pct_decode_opts{}, pchars );
    assert( result.compare( "Program Files" ) == 0 );
    @endcode

    @par Exception Safety
    Throws on invalid input.
    Calls to allocate may throw.

    @return A `std::basic_string` holding
    the decoded result. If the default
    allocator is used, the return type is
    `std::string`.

    @param s The string to decode.

    @param opt The options for decoding. If
    this parameter is omitted, the default
    options will be used.

    @param cs An optionally specified
    character set to use. If this parameter
    is omitted, all characters are considered
    unreserved.

    @param a An optional allocator the returned
    string will use. If this parameter is omitted,
    the default allocator is used.

    @throws std::invalid_argument invalid input.

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-2.1"
        >2.1. Percent-Encoding (rfc3986)</a>

    @see
        @ref pct_decode_bytes_unchecked,
        @ref pct_decode_opts,
        @ref pct_decode_to_value.
        @ref pct_decode_unchecked.
        @ref validate_pct_encoding.
*/
template<
    class CharSet = reserved_chars_t,
    class Allocator = std::allocator<char> >
const_string
pct_decode_to_value(
    string_view s,
    pct_decode_opts const& opt = {},
    CharSet const& cs = {},
    Allocator const& a = {});

/** Return the number of bytes needed to hold the string with percent-decoding applied.

    This function calculates the number of
    bytes needed to apply percent-decoding to
    the encoded string `s`, excluding any
    null terminator. The caller is responsible
    for validating the input string before
    calling this function.

    @par Example
    @code
    std::size_t size = pct_decode_bytes_unchecked( "Program%20Files" );
    assert( size == 13 );
    @endcode

    @par Preconditions
    The string `s` must contain a valid
    percent-encoding.

    @par Exception Safety
    Throws nothing.

    @return The number of bytes needed to
    apply percent-decoding, excluding any
    null terminator.

    @param s The string to inspect.

    @see
        @ref validate_pct_encoding.
*/
BOOST_URL_DECL
std::size_t
pct_decode_bytes_unchecked(
    string_view s) noexcept;

/** Apply percent-decoding to a string

    This function applies percent-decoding to
    the input string, without performing any
    checking to ensure that the input string
    is valid. The contents of the output
    buffer will never be left undefined,
    regardless of input.

    @par Example
    @code
    char *dest = new char[MAX_LENGTH];
    std::size_t decoded_size = pct_decode_unchecked( dest, dest + MAX_LENGTH,
            "Program%20Files" );

    assert( decoded_size == 13 );
    assert( strncmp("Program Files", dest, decoded_size) == 0 );
    @endcode

    @par Exception Safety
    Throws nothing.

    @return The number of bytes written to
    the destination.

    @param dest A pointer to the beginning of
    the output buffer to hold the percent-decoded
    characters.

    @param end A pointer to one past the end
    of the output buffer.

    @param s The string to apply percent-decoding to.

    @param opt Optional settings for applying
    the decoding. If this parameter is omitted,
    default settings are used.
*/
BOOST_URL_DECL
std::size_t
pct_decode_unchecked(
    char* dest,
    char const* end,
    string_view s,
    pct_decode_opts const& opt = {}) noexcept;

/** Return a string with percent-decoding applied.

    This function applies percent-decoding
    to the specified string and returns a
    newly allocated string containing the
    result, using the optionally specified
    allocator. No checking is performed to
    ensure that the input is valid; however,
    the returned string is never undefined.

    @par Example
    @code
    const_string result = pct_decode_unchecked( "Program%20Files" );
    assert( result.compare( "Program Files" ) == 0 );
    @endcode

    @par Exception Safety
    Calls to allocate may throw.

    @return A string containing the decoded
    result.

    @param s The string to decode.

    @param opt Optional settings for applying
    the decoding. If this parameter is omitted,
    default settings are used.

    @param a An optional allocator the returned
    string will use. If this parameter is omitted,
    the default allocator is used.

    @param decoded_size An optional hint to
    the algorithm indicating the correct
    decoded size. If this parameter is omitted,
    the value will be calculated as-if by
    calling @ref pct_decode_bytes_unchecked.
*/
template<class Allocator =
    std::allocator<char> >
const_string
pct_decode_unchecked(
    string_view s,
    pct_decode_opts const& opt = {},
    Allocator const& a = {},
    std::size_t decoded_size =
        std::size_t(-1) );

//------------------------------------------------

/** Options for applying percent-encoding to strings.

    Instances of this type may be provided
    to percent-encoding algorithms to
    customize their behavior.

    @see
        @ref pct_encode,
        @ref pct_encode_bytes,
        @ref pct_encode_to_value.
*/
struct pct_encode_opts
{
    /** True if space (SP, ' ') encodes into PLUS ('+').

        @par Specification
        @li <a href="https://www.w3.org/TR/html401/interact/forms.html#h-17.13.4.1">
            application/x-www-form-urlencoded (w3.org)</a>
    */
    bool space_to_plus = false;
};

/** Return the number of bytes needed to store a string with percent-encoding applied

    This function examines the characters in
    the string to determine the number of bytes
    necessary if the string were to be percent
    encoded using the given options and character
    set. No encoding is actually performed.

    @par Example 1
    Find the number of bytes needed to encode a string without transforming
    ' ' to '+'.
    @code
    pct_encode_opts opt;
    opt.space_to_plus = false;
    std::size_t n = pct_encode_bytes( "My Stuff", pchars, opt );

    assert( n == 10 );
    @endcode

    @par Example 2
    Find the number of bytes needed to encode a string when transforming
    ' ' to '+'.
    @code
    pct_encode_opts opt;
    opt.space_to_plus = true;
    std::size_t n = pct_encode_bytes( "My Stuff", opt, pchars );

    assert( n == 8 );
    @endcode

    @par Exception Safety
    Throws nothing.

    @return The number of bytes needed,
    excluding any null terminator.

    @param s The string to encode.

    @param opt The options for encoding. If
    this parameter is omitted, the default
    options will be used.

    @param cs The character set to use.

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-2.1"
        >2.1. Percent-Encoding (rfc3986)</a>

    @see
        @ref pct_decode,
        @ref pct_encode,
        @ref pct_encode_opts,
        @ref pct_encode_to_value.
*/
template<
    class CharSet = reserved_chars_t>
std::size_t
pct_encode_bytes(
    string_view s,
    pct_encode_opts const& opt = {},
    CharSet const& cs = {}) noexcept;

/** Write a string with percent-encoding into a buffer.

    This function applies percent-encoding to
    the given plain string, by escaping all
    characters that are not in the specified
    <em>CharSet</em>.
    The output is written to the destination,
    and will be truncated if there is
    insufficient space.

    @par Example
    @code
    char *dest = new char[MAX_LENGTH];
    std::size_t encoded_size = pct_encode( dest, dest + MAX_LENGTH,
            "Program Files", pct_encode_opts{}, pchars );

    assert( encoded_size == 15 );
    assert( strncmp( "Program%20Files", dest, encoded_size ) == 0 );
    @endcode

    @par Exception Safety
    Throws nothing.

    @return `true` if the output was large
    enough to hold the entire result.

    @param[in, out] dest A pointer to the
    beginning of the output buffer. Upon
    return, the argument will be changed
    to one past the last character written.

    @param end A pointer to one past the end
    of the output buffer.

    @param s The string to encode.

    @param opt The options for encoding. If
    this parameter is omitted, the default
    options will be used.

    @param cs The character set to use.

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-2.1"
        >2.1. Percent-Encoding (rfc3986)</a>

    @see
        @ref pct_decode,
        @ref pct_encode,
        @ref pct_encode_bytes.
*/
template<
    class CharSet = reserved_chars_t>
std::size_t
pct_encode(
    char* dest,
    char const* end,
    string_view s,
    pct_encode_opts const& opt = {},
    CharSet const& cs = {});

/** Return a string with percent-encoding applied

    This function applies percent-encoding to
    the given plain string, by escaping all
    characters that are not in the specified
    <em>CharSet</em>.
    The result is returned as a
    `std::basic_string`, using the optionally
    specified allocator.

    @par Example
    @code
    pct_encode_opts opt;
    opt.space_to_plus = true;
    std::string s = pct_encode( "My Stuff", opt, pchars );

    assert( s == "My+Stuff" );
    @endcode

    @par Exception Safety
    Calls to allocate may throw.

    @return A `std::basic_string` holding the
    encoded string, using the specified
    allocator.

    @param s The string to encode.

    @param opt The options for encoding. If
    this parameter is omitted, the default
    options will be used.

    @param cs The character set to use.

    @param a An optional allocator the returned
    string will use. If this parameter is omitted,
    the default allocator is used. In this case
    the return type will be `std::string`.

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-2.1"
        >2.1. Percent-Encoding (rfc3986)</a>

    @see
        @ref pct_encode,
        @ref pct_encode_bytes,
        @ref pct_encode_opts,
        @ref pct_encode_to_value,
        @ref reserved_chars_t.
*/
template<
    class CharSet = reserved_chars_t,
    class Allocator =
        std::allocator<char> >
std::basic_string<char,
    std::char_traits<char>,
        Allocator>
pct_encode(
    string_view s,
    pct_encode_opts const& opt = {},
    CharSet const& cs = {},
    Allocator const& a = {});

/** Return a string with percent-encoding applied

    This function applies percent-encoding to
    the given plain string, by escaping all
    characters that are not in the specified
    <em>CharSet</em>.
    The result is returned as a @ref const_string
    using the optionally specified allocator.

    @par Example
    @code
    pct_encode_opts opt;
    opt.space_to_plus = true;
    const_string encoded = pct_encode_to_value( "My Stuff", opt, pchars );

    assert( encoded.compare("My+Stuff") == 0 );
    @endcode

    @par Exception Safety
    Calls to allocate may throw.

    @return A @ref const_string holding the
    encoded string.

    @param s The string to encode.

    @param opt The options for encoding. If
    this parameter is omitted, the default
    options will be used.

    @param cs The character set to use.

    @param a An optional allocator the returned
    string will use. If this parameter is omitted,
    the default allocator is used.

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-2.1"
        >2.1. Percent-Encoding (rfc3986)</a>

    @see
        @ref pct_encode,
        @ref pct_encode_bytes,
        @ref pct_encode_opts.
*/
template<
    class CharSet = reserved_chars_t,
    class Allocator = std::allocator<char> >
const_string
pct_encode_to_value(
    string_view s,
    pct_encode_opts const& opt = {},
    CharSet const& cs = {},
    Allocator const& a = {});

} // urls
} // boost

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_PCT_ENCODING_HPP
#define BOOST_URL_IMPL_PCT_ENCODING_HPP

#include <boost/assert.hpp>
#include <boost/static_assert.hpp>

namespace boost {
namespace urls {

template<class CharSet>
std::size_t
validate_pct_encoding(
    string_view s,
    error_code& ec,
    pct_decode_opts const& opt,
    CharSet const& cs) noexcept
{
    // CharSet must satisfy is_charset
    BOOST_STATIC_ASSERT(
        grammar::is_charset<CharSet>::value);

    // can't have % in charset
    BOOST_ASSERT(! cs('%'));
    std::size_t n = 0;
    auto it = s.data();
    auto const end = it + s.size();
    while(it != end)
    {
        if( opt.plus_to_space &&
            *it == '+')
        {
            // plus to space
            ++n;
            ++it;
            continue;
        }
        if( ! opt.allow_null &&
            *it == '\0')
        {
            // null in input
            ec = BOOST_URL_ERR(
                error::illegal_null);
            return n;
        }
        if(cs(*it))
        {
            // unreserved
            ++n;
            ++it;
            continue;
        }
        if(*it == '%')
        {
            // escaped
            ++it;
            if(end - it < 2)
            {
                // missing HEXDIG
                ec = BOOST_URL_ERR(
                    error::missing_pct_hexdig);
                return n;
            }
            char d0;
            bool r0 = grammar::hexdig_value(it[0], d0);
            char d1;
            bool r1 = grammar::hexdig_value(it[1], d1);
            if( !r0 || !r1 )
            {
                // expected HEXDIG
                ec = BOOST_URL_ERR(
                    error::bad_pct_hexdig);
                return n;
            }
            it += 2;
            char const c = static_cast<char>(
                ((static_cast<
                    unsigned char>(d0) << 4) +
                (static_cast<
                    unsigned char>(d1))));
            if( ! opt.allow_null &&
                c == '\0')
            {
                // escaped null
                ec = BOOST_URL_ERR(
                    error::illegal_null);
                return n;
            }
            if( opt.non_normal_is_error &&
                cs(c))
            {
                // escaped unreserved char
                ec = BOOST_URL_ERR(
                    error::non_canonical);
                return n;
            }
            ++n;
            continue;
        }
        // reserved character in input
        ec = BOOST_URL_ERR(
            error::illegal_reserved_char);
        return n;
    }
    BOOST_ASSERT(it == end);
    ec = {};
    return n;
}

//------------------------------------------------

template<class CharSet>
std::size_t
pct_decode(
    char* dest,
    char const* end,
    string_view s,
    error_code& ec,
    pct_decode_opts const& opt,
    CharSet const& cs) noexcept
{
    // CharSet must satisfy is_charset
    BOOST_STATIC_ASSERT(
        grammar::is_charset<CharSet>::value);

    auto const n =
        validate_pct_encoding(
            s, ec, opt, cs);
    if(ec.failed())
        return 0;
    auto const n1 =
        pct_decode_unchecked(
            dest, end, s, opt);
    if(n1 < n)
    {
        ec = error::no_space;
        return n1;
    }
    return n1;
}

//------------------------------------------------

template<
    class CharSet,
    class Allocator>
std::basic_string<char,
    std::char_traits<char>,
        Allocator>
pct_decode(
    string_view s,
    pct_decode_opts const& opt,
    CharSet const& cs,
    Allocator const& a)
{
    // CharSet must satisfy is_charset
    BOOST_STATIC_ASSERT(
        grammar::is_charset<CharSet>::value);

    std::basic_string<char,
        std::char_traits<char>,
            Allocator> r(a);
    if(s.empty())
        return r;
    error_code ec;
    auto const n =
        validate_pct_encoding(
            s, ec, opt, cs);
    if(ec.failed())
        detail::throw_invalid_argument(
            BOOST_CURRENT_LOCATION);
    r.resize(n);
    pct_decode_unchecked(
        &r[0], &r[0] + r.size(),
            s, opt);
    return r;
}

//------------------------------------------------

template<
    class CharSet,
    class Allocator>
const_string
pct_decode_to_value(
    string_view s,
    pct_decode_opts const& opt,
    CharSet const& cs,
    Allocator const& a)
{
    // CharSet must satisfy is_charset
    BOOST_STATIC_ASSERT(
        grammar::is_charset<CharSet>::value);

    if(s.empty())
        return const_string();
    error_code ec;
    auto const n =
        validate_pct_encoding(
            s, ec, opt, cs);
    if(ec.failed())
        detail::throw_invalid_argument(
            BOOST_CURRENT_LOCATION);
    return const_string(n, a,
        [&opt, &s]
        (std::size_t n, char* dest)
        {
            pct_decode_unchecked(
                dest, dest + n, s, opt);
        });
}

//------------------------------------------------

template<class Allocator>
const_string
pct_decode_unchecked(
    string_view s,
    pct_decode_opts const& opt,
    Allocator const& a,
    std::size_t decoded_size)
{
    if(decoded_size == std::size_t(-1))
        decoded_size =
            pct_decode_bytes_unchecked(s);
    return const_string(
        decoded_size, a,
        [&s, &opt](
            std::size_t n, char* dest)
        {
            pct_decode_unchecked(
                dest, dest + n, s, opt);
        });
}

//------------------------------------------------

template<class CharSet>
std::size_t
pct_encode_bytes(
    string_view s,
    pct_encode_opts const& opt,
    CharSet const& cs) noexcept
{
    // CharSet must satisfy is_charset
    BOOST_STATIC_ASSERT(
        grammar::is_charset<CharSet>::value);

    std::size_t n = 0;
    auto it = s.data();
    auto const end = it + s.size();
    if(! opt.space_to_plus)
    {
        while(it != end)
        {
            if(cs(*it))
                ++n;
            else
                n += 3;
            ++it;
        }
        return n;
    }
    // If you are converting space
    // to plus, then space should
    // be in the list of reserved
    // characters!
    BOOST_ASSERT(! cs(' '));
    while(it != end)
    {
        if(*it == ' ')
            ++n;
        else if(cs(*it))
            ++n;
        else
            n += 3;
        ++it;
    }
    return n;
}

//------------------------------------------------

template<class CharSet>
std::size_t
pct_encode(
    char* dest,
    char const* const end,
    string_view s,
    pct_encode_opts const& opt,
    CharSet const& cs)
{
    // CharSet must satisfy is_charset
    BOOST_STATIC_ASSERT(
        grammar::is_charset<CharSet>::value);

    // Can't have % in charset
    BOOST_ASSERT(! cs('%'));

    static constexpr char hex[] =
        "0123456789abcdef";
    auto const dest0 = dest;
    auto p = s.data();
    auto const last = p + s.size();
    auto const end3 = end - 3;
    if(! opt.space_to_plus)
    {
        while(p != last)
        {
            if(cs(*p))
            {
                if(dest == end)
                    return dest - dest0;
                *dest++ = *p++;
                continue;
            }
            if(dest > end3)
                return dest - dest0;
            *dest++ = '%';
            auto const u = static_cast<
                unsigned char>(*p);
            *dest++ = hex[u>>4];
            *dest++ = hex[u&0xf];
            ++p;
        }
        return dest - dest0;
    }
    // If you are converting space
    // to plus, then space should
    // be in the list of reserved
    // characters!
    BOOST_ASSERT(! cs(' '));
    while(p != last)
    {
        if(cs(*p))
        {
            if(dest == end)
                return dest - dest0;
            *dest++ = *p++;
            continue;
        }
        if(*p == ' ')
        {
            if(dest == end)
                return dest - dest0;
            *dest++ = '+';
            ++p;
            continue;
        }
        if(dest > end3)
            return dest - dest0;
        *dest++ = '%';
        auto const u = static_cast<
            unsigned char>(*p);
        *dest++ = hex[u>>4];
        *dest++ = hex[u&0xf];
        ++p;
    }
    return dest - dest0;
}

//------------------------------------------------

template<
    class CharSet,
    class Allocator>
std::basic_string<char,
    std::char_traits<char>,
        Allocator>
pct_encode(
    string_view s,
    pct_encode_opts const& opt,
    CharSet const& cs,
    Allocator const& a)
{
    // CharSet must satisfy is_charset
    BOOST_STATIC_ASSERT(
        grammar::is_charset<CharSet>::value);

    std::basic_string<
        char,
        std::char_traits<char>,
        Allocator> r(a);
    if(s.empty())
        return r;
    auto const n =
        pct_encode_bytes(s, opt, cs);
    r.resize(n);
    char* dest = &r[0];
    char const* end = dest + n;
    auto const n1 = pct_encode(
        dest, end, s, opt, cs);
    BOOST_ASSERT(n1 == n);
    (void)n1;
    return r;
}
//------------------------------------------------

template<
    class CharSet,
    class Allocator>
const_string
pct_encode_to_value(
    string_view s,
    pct_encode_opts const& opt,
    CharSet const& cs,
    Allocator const& a)
{
    // CharSet must satisfy is_charset
    BOOST_STATIC_ASSERT(
        grammar::is_charset<CharSet>::value);

    if(s.empty())
        return const_string();
    auto const n =
        pct_encode_bytes(s, opt, cs);
    return const_string(n, a,
        [&s, &opt, &cs](
            std::size_t n, char* dest)
        {
            auto const n1 = pct_encode(
                dest, dest+n, s, opt, cs);
            BOOST_ASSERT(n1 == n);
            (void)n1;
        });
}

} // urls
} // boost

#endif

#endif
#include <boost/assert.hpp>
#include <cstddef>
#include <iosfwd>
#include <utility>

namespace boost {
namespace urls {

#ifndef BOOST_URL_DOCS
struct authority_rule;
struct host_rule;
#endif

/** A read-only view to an authority.

    Objects of this type represent a valid
    authority whose storage is managed externally.
    That is, it acts like a `std::string_view` in
    terms of ownership. Callers are responsible
    for ensuring that the lifetime of the
    underlying string extends until the
    `authority_view` is no longer in use.
    To construct from an existing string it is
    necessary to use one of the parsing
    functions. Each function parses against
    a particular URL grammar:

    @par Example
    @code
    authority_view a;

    a = parse_authority( "www.example.com:443" );
    @endcode

    @par BNF
    @code
    authority     = [ userinfo "@" ] host [ ":" port ]

    userinfo      = user [ ":" [ password ] ]

    user          = *( unreserved / pct-encoded / sub-delims )
    password      = *( unreserved / pct-encoded / sub-delims / ":" )

    host          = IP-literal / IPv4address / reg-name

    port          = *DIGIT
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2"
        >3.2. Authority (rfc3986)</a>

    @see
        @ref parse_authority.
*/
class BOOST_SYMBOL_VISIBLE
    authority_view
{
    enum
    {
        id_user = 0,
        id_pass,        // leading ':', trailing '@'
        id_host,
        id_port,        // leading ':'
        id_end          // one past the end
    };

    static
    constexpr
    pos_t zero_ = 0;

    static
    constexpr
    char const* const empty_ = "";

    char const* cs_ = empty_;
    pos_t offset_[id_end + 1] = {};
    pos_t decoded_[id_end] = {};
    unsigned char ip_addr_[16] = {};
    // VFALCO don't we need a bool?
    std::uint16_t port_number_ = 0;
    urls::host_type host_type_ =
        urls::host_type::none;

    struct shared_impl;

    pos_t len(int first,
        int last) const noexcept;
    void set_size(
        int id, pos_t n) noexcept;
    explicit authority_view(
        char const* cs) noexcept;
    authority_view(
        authority_view const& u,
            char const* cs) noexcept;

    // return offset of id
    auto
    offset(int id) const noexcept ->
        pos_t
    {
        return
            id == id_user ?
            zero_ : offset_[id - 1];
    }

    // return length of part
    auto
    len(int id) const noexcept ->
        pos_t
    {
        return
            offset(id + 1) -
            offset(id);
    }

    // return id as string
    string_view
    get(int id) const noexcept
    {
        return {
            cs_ + offset(id), len(id) };
    }

    // return [first, last) as string
    string_view
    get(int first,
        int last) const noexcept
    {
        return { cs_ + offset(first),
            offset(last) - offset(first) };
    }

public:
    /** The type of elements.
    */
    using value_type        = char;

    /** The type of pointers to elements.
    */
    using pointer           = char const*;

    /** The type of const pointers to elements.
    */
    using const_pointer     = char const*;

    /** The type of reference to elements.
    */
    using reference         = char const&;

    /** The type of const references to elements.
    */
    using const_reference   = char const&;

    /** The type of const iterator to elements.
    */
    using const_iterator    = char const*;

    /** The type of iterator to elements.
    */
    using iterator          = char const*;

    /** An unsigned integer type to represent sizes.
    */
    using size_type         = std::size_t;

    /** A signed integer type to represent differences.
    */
    using difference_type   = std::ptrdiff_t;

    //--------------------------------------------
    //
    // Special Members
    //
    //--------------------------------------------

    /** Destructor
    */
    BOOST_URL_DECL
    virtual ~authority_view();

    /** Constructor

        Default constructed authorities
        refer to a string with zero length,
        which is always valid. This matches
        the grammar for a zero-length host.

        @par Exception Safety
        Throws nothing.

        @par Specification
    */
    BOOST_URL_DECL
    authority_view() noexcept;

    BOOST_URL_DECL
    authority_view(
        authority_view const&) noexcept;

    BOOST_URL_DECL
    authority_view&
    operator=(
        authority_view const&) noexcept;

    //--------------------------------------------
    //
    // Capacity
    // Element Access
    //
    //--------------------------------------------

    /** Return the maximum number of characters allowed in the authority.

        This includes any null terminator, if present

        @par Exception Safety
        Throws nothing.
    */
    static
    constexpr
    std::size_t
    max_size() noexcept
    {
        return BOOST_URL_MAX_SIZE;
    }

    /** Return the number of characters in the authority.

        This function returns the number
        of characters in the authority, not
        including any null terminator.

        @par Exception Safety
        Throws nothing.
    */
    std::size_t
    size() const noexcept
    {
        return offset(id_end);
    }

    /** Return true if the contents are empty.

        @par Exception Safety
        Throws nothing.
    */
    bool
    empty() const noexcept
    {
        return size() == 0;
    }

    /** Return a pointer to the first character

        This function returns a pointer to the
        beginning of the view, which is not
        guaranteed to be null-terminated.

        @par Exception Safety
        Throws nothing.
    */
    char const*
    data() const noexcept
    {
        return cs_;
    }

    /** Access the specified character

        This function returns a reference to
        the character at the specified zero-based
        position. If `pos` is out of range, an
        exception if thrown.

        @param pos The zero-based character
        position to access.

        @throw std::out_of_range pos >= size()
    */
    char const&
    at(std::size_t pos) const
    {
        if(pos >= size())
            detail::throw_out_of_range(
                BOOST_CURRENT_LOCATION);
        return cs_[pos];
    }

    /** Access the specified character

        This function returns a reference to
        the character at the specified zero-based
        position. The behavior is undefined if
        `pos` is out of range.

        @par Preconditions
        @code
        pos < this->size()
        @endcode
    */
    char const&
    operator[](
        std::size_t pos) const noexcept
    {
        BOOST_ASSERT(pos < size());
        return cs_[pos];
    }

    /** Return an iterator to the beginning

        This function returns a constant iterator
        to the first character of the view, or
        one past the last element if the view is
        empty.
    */
    char const*
    begin() const noexcept
    {
        return data();
    }

    /** Return an iterator to the end

        This function returns a constant iterator to
        the character following the last character of
        the view. This character acts as a placeholder,
        attempting to access it results in undefined
        behavior.
    */
    char const*
    end() const noexcept
    {
        return data() + size();
    }

    //--------------------------------------------
    //
    // Observers
    //
    //--------------------------------------------

    /** Return a read-only copy of the authority, with shared lifetime.

        This function makes a copy of the storage
        pointed to by this, and attaches it to a
        new constant @ref authority_view returned in a
        shared pointer. The lifetime of the storage
        for the characters will extend for the
        lifetime of the shared object. This allows
        the new view to be copied and passed around
        after the original string buffer is destroyed.

        @par Example
        @code
        std::shared_ptr<authority const> sp;
        {
            std::string s( "user:pass@example.com:443" );
            authority_view a = parse_authority( s ).value();    // a references characters in s

            assert( a.data() == s.data() );                     // same buffer

            sp = a.collect();

            assert( sp->data() != a.data() );                   // different buffer
            assert( sp->encoded_authority() == s);              // same contents

            // s is destroyed and thus a
            // becomes invalid, but sp remains valid.
        }
        std::cout << *sp; // works
        @endcode
    */
    BOOST_URL_DECL
    std::shared_ptr<
        authority_view const>
    collect() const;

    /** Return the complete authority

        This function returns the authority
        as a percent-encoded string.

        @par Example
        @code
        assert( parse_authority( "www.example.com" ).value().encoded_authority() == "www.example.com" );
        @endcode

        @par BNF
        @code
        authority   = [ userinfo "@" ] host [ ":" port ]
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2"
            >3.2. Authority (rfc3986)</a>
    */
    string_view
    encoded_authority() const noexcept
    {
        return string_view(
            data(), size());
    }

    //--------------------------------------------

    /** Return true if this contains a userinfo

        This function returns true if this
        contains a userinfo.

        @par Example
        @code
        authority_view a = parse_authority( "user@example.com" ).value();

        assert( a.has_userinfo() == true );
        @endcode

        @par BNF
        @code
        userinfo    = user [ ":" [ password ] ]

        authority   = [ userinfo "@" ] host [ ":" port ]
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.1"
            >3.2.1. User Information (rfc3986)</a>

        @see
            @ref encoded_userinfo,
            @ref userinfo.
    */
    BOOST_URL_DECL
    bool
    has_userinfo() const noexcept;

    /** Return the userinfo

        This function returns the userinfo
        as a percent-encoded string.

        @par Example
        @code
        assert( parse_authority( "user:pass@example.com" ).value().encoded_userinfo() == "user:pass" );
        @endcode

        @par BNF
        @code
        userinfo    = user [ ":" [ password ] ]

        authority   = [ userinfo "@" ] host [ ":" port ]
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.1"
            >3.2.1. User Information (rfc3986)</a>

        @see
            @ref has_userinfo,
            @ref userinfo.
    */
    BOOST_URL_DECL
    string_view
    encoded_userinfo() const noexcept;

    /** Return the userinfo

        This function returns the userinfo as a
        string with percent-decoding applied, using
        the optionally specified allocator.

        @par Example
        @code
        authority_view a = parse_authority( "user:pass@example.com" ).value();

        assert( a.userinfo() == "user:pass" );
        @endcode

        @par BNF
        @code
        userinfo    = user [ ":" [ password ] ]

        authority   = [ userinfo "@" ] host [ ":" port ]
        @endcode

        @par Exception Safety
        Calls to allocate may throw.

        @param a An optional allocator the returned
        string will use. If this parameter is omitted,
        the default allocator is used.

        @return A @ref const_string using the
        specified allocator.

        @par Specification
        <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.1"
            >3.2.1. User Information (rfc3986)</a>

        @see
            @ref has_userinfo,
            @ref encoded_userinfo.
    */
    template<
        class Allocator =
            std::allocator<char>>
    const_string
    userinfo(
        Allocator const& a = {}) const
    {
        pct_decode_opts opt;
        opt.plus_to_space = false;
        return pct_decode_unchecked(
            encoded_userinfo(), opt, a);
    }

    //--------------------------------------------

    /** Return the user

        This function returns the user portion of
        the userinfo as a percent-encoded string.

        @par Example
        @code
        authority_view a = parse_authority( "user:pass@example.com" ).value();

        assert( a.encoded_user() == "user" );
        @endcode

        @par BNF
        @code
        userinfo    = user [ ":" [ password ] ]

        user        = *( unreserved / pct-encoded / sub-delims )
        password    = *( unreserved / pct-encoded / sub-delims / ":" )
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.1"
            >3.2.1. User Information (rfc3986)</a>

        @see
            @ref encoded_password,
            @ref has_password,
            @ref password,
            @ref user.
    */
    string_view
    encoded_user() const noexcept
    {
        return get(id_user);
    }

    /** Return the user

        This function returns the user portion of
        the userinfo as a string with percent-decoding
        applied, using the optionally specified
        allocator.

        @par Example
        @code
        assert( parse_authority( "user:pass@example.com" ).value().user() == "user" );
        @endcode

        @par BNF
        @code
        userinfo    = user [ ":" [ password ] ]

        user        = *( unreserved / pct-encoded / sub-delims )
        password    = *( unreserved / pct-encoded / sub-delims / ":" )
        @endcode

        @par Exception Safety
        Calls to allocate may throw.

        @param a An optional allocator the returned
        string will use. If this parameter is omitted,
        the default allocator is used.

        @return A @ref const_string using the
        specified allocator.

        @par Specification
        <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.1"
            >3.2.1. User Information (rfc3986)</a>

        @see
            @ref encoded_password,
            @ref encoded_user,
            @ref has_password,
            @ref password.
    */
    template<
        class Allocator =
            std::allocator<char>>
    const_string
    user(
        Allocator const& a = {}) const
    {
        pct_decode_opts opt;
        opt.plus_to_space = false;
        return pct_decode_unchecked(
            encoded_user(), opt, a);
    }

    /** Return true if this contains a password

        This function returns true if the userinfo
        contains a password (which may be empty).

        @par Example
        @code
        assert( parse_authority( "user@example.com" ).value().has_password() == false );

        assert( parse_authority( "user:pass@example.com" ).value().has_password() == true );

        assert( parse_authority( ":@" ).value().has_password() == true );
        @endcode

        @par BNF
        @code
        userinfo    = user [ ":" [ password ] ]

        user        = *( unreserved / pct-encoded / sub-delims )
        password    = *( unreserved / pct-encoded / sub-delims / ":" )
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.1"
            >3.2.1. User Information (rfc3986)</a>

        @see
            @ref encoded_password,
            @ref encoded_user,
            @ref password,
            @ref user.
    */
    BOOST_URL_DECL
    bool
    has_password() const noexcept;

    /** Return the password

        This function returns the password portion
        of the userinfo as a percent-encoded string.

        @par Example
        @code
        authority_view a = parse_authority( "user:pass@example.com" ).value();

        assert( a.encoded_user() == "user" );
        @endcode

        @par BNF
        @code
        userinfo    = user [ ":" [ password ] ]

        user        = *( unreserved / pct-encoded / sub-delims )
        password    = *( unreserved / pct-encoded / sub-delims / ":" )
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.1"
            >3.2.1. User Information (rfc3986)</a>

        @see
            @ref encoded_password,
            @ref has_password,
            @ref password,
            @ref user.
    */
    BOOST_URL_DECL
    string_view
    encoded_password() const noexcept;

    /** Return the password

        This function returns the password from the
        userinfo with percent-decoding applied,
        using the optionally specified allocator.

        @par Exception Safety
        Calls to allocate may throw.

        @param a An allocator to use for the string.
        If this parameter is omitted, the default
        allocator is used, and the return type of
        the function becomes `std::string`.

        @return A @ref const_string using the
        specified allocator.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.1"
            >3.2.1. User Information (rfc3986)</a>

        @see
            @ref encoded_password,
            @ref encoded_user,
            @ref has_password,
            @ref password.
    */
    template<
        class Allocator =
            std::allocator<char>>
    const_string
    password(
        Allocator const& a = {}) const
    {
        pct_decode_opts opt;
        opt.plus_to_space = false;
        return pct_decode_unchecked(
            encoded_password(), opt, a);
    }

    //--------------------------------------------

    /** Return the type of host present, if any

        This function returns a
            @ref urls::host_type
        constant representing the type of
        host this contains, which may be
        @ref urls::host_type::none.

        @par Example
        @code
        assert( authority_view().host_type() == host_type::none );

        assert( parse_authority( "example.com" ).value().host_type() == host_type::name );

        assert( parse_authority( "192.168.0.1" ).value().host_type() == host_type::ipv4 );
        @endcode

        @par BNF
        @code
        host        = IP-literal / IPv4address / reg-name

        IP-literal  = "[" ( IPv6address / IPvFuture  ) "]"

        IPvFuture   = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.2"
            >3.2.2. Host (rfc3986)</a>

        @see
            @ref encoded_host,
            @ref has_port,
            @ref host,
            @ref port,
            @ref port_number.
    */
    urls::host_type
    host_type() const noexcept
    {
        return host_type_;
    }

    /** Return the host

        This function returns the host portion of
        the authority as a percent-encoded string,

        @par Example
        @code
        assert( parse_authority( "" ).value().encoded_host() == "" );

        assert( parse_authority( "http://example.com" ).value().encoded_host() == "example.com" );

        assert( parse_authority( "http://192.168.0.1" ).value().encoded_host() == "192.168.0.1" );
        @endcode

        @par BNF
        @code
        host        = IP-literal / IPv4address / reg-name

        IP-literal  = "[" ( IPv6address / IPvFuture  ) "]"

        IPvFuture   = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )

        reg-name    = *( unreserved / pct-encoded / "-" / ".")
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.2"
            >3.2.2. Host (rfc3986)</a>

        @see
            @ref has_port,
            @ref host,
            @ref host_type,
            @ref port,
            @ref port_number.
    */
    BOOST_URL_DECL
    string_view
    encoded_host() const noexcept;

    /** Return the host

        This function returns the host portion of
        the authority as a string with percent-decoding
        applied, using the optionally specified
        allocator.

        @par Example
        @code
        assert( parse_authority( "" ).value().host() == "" );

        assert( parse_authority( "example.com" ).value().host() == "example.com" );

        assert( parse_authority( "192.168.0.1" ).value().host() == "192.168.0.1" );
        @endcode

        @par BNF
        @code
        host        = IP-literal / IPv4address / reg-name

        IP-literal  = "[" ( IPv6address / IPvFuture  ) "]"

        IPvFuture   = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )

        reg-name    = *( unreserved / pct-encoded / "-" / ".")
        @endcode

        @par Exception Safety
        Calls to allocate may throw.

        @param a An optional allocator the returned
        string will use. If this parameter is omitted,
        the default allocator is used, and the return
        type of the function becomes `std::string`.

        @return A @ref const_string using the
        specified allocator.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.2"
            >3.2.2. Host (rfc3986)</a>

        @see
            @ref encoded_host,
            @ref encoded_host_and_port
            @ref has_port,
            @ref host_type,
            @ref port,
            @ref port_number.
    */
    template<
        class Allocator =
            std::allocator<char>>
    const_string
    host(
        Allocator const& a = {}) const
    {
        auto const s0 =
            encoded_host();
        if(host_type_ !=
            urls::host_type::name)
        {
            // no decoding
            return const_string(s0, a);
        }
        pct_decode_opts opt;
        opt.plus_to_space = false;
        return pct_decode_unchecked(
            s0, opt, a, decoded_[id_host]);
    }

    /** Return the host as an IPv4 address

        If @ref host_type equals @ref urls::host_type::ipv4,
        this function returns the corresponding
        @ref ipv4_address
        of the host if it exists, otherwise it
        returns the unspecified address which
        is equal to "0.0.0.0".

        @par BNF
        @code
        IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet

        dec-octet   = DIGIT                 ; 0-9
                    / %x31-39 DIGIT         ; 10-99
                    / "1" 2DIGIT            ; 100-199
                    / "2" %x30-34 DIGIT     ; 200-249
                    / "25" %x30-35          ; 250-255
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.2"
            >3.2.2. Host (rfc3986)</a>

        @see
            @ref ipv4_address.
    */
    BOOST_URL_DECL
    urls::ipv4_address
    ipv4_address() const noexcept;

    /** Return the host as an IPv6 address

        If @ref host_type equals
        @ref urls::host_type::ipv6, this
        function returns the corresponding
        @ref ipv6_address of the host if it
        exists, otherwise it returns the
        unspecified address which is equal
        to "0:0:0:0:0:0:0:0".

        @par BNF
        @code
        IPv6address =                            6( h16 ":" ) ls32
                    /                       "::" 5( h16 ":" ) ls32
                    / [               h16 ] "::" 4( h16 ":" ) ls32
                    / [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
                    / [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
                    / [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32
                    / [ *4( h16 ":" ) h16 ] "::"              ls32
                    / [ *5( h16 ":" ) h16 ] "::"              h16
                    / [ *6( h16 ":" ) h16 ] "::"

        ls32        = ( h16 ":" h16 ) / IPv4address
                    ; least-significant 32 bits of address

        h16         = 1*4HEXDIG
                    ; 16 bits of address represented in hexadecimal
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.2"
            >3.2.2. Host (rfc3986)</a>

        @see
            @ref ipv6_address.
    */
    BOOST_URL_DECL
    urls::ipv6_address
    ipv6_address() const noexcept;

    /** Return the host as an IPvFuture string

        If @ref host_type equals
        @ref urls::host_type::ipvfuture, this
        function returns a string representing
        the address. Otherwise it returns the
        empty string.

        @par BNF
        @code
        IPvFuture  = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.2"
            >3.2.2. Host (rfc3986)</a>
    */
    BOOST_URL_DECL
    string_view
    ipv_future() const noexcept;

    /** Return true if the URL contains a port

        This function returns true if the
        authority contains a port.

        @par BNF
        @code
        authority   = [ userinfo "@" ] host [ ":" port ]

        port        = *DIGIT
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.3"
            >3.2.3. Port (rfc3986)</a>

        @see
            @ref encoded_host,
            @ref encoded_host_and_port
            @ref host,
            @ref host_type,
            @ref port,
            @ref port_number.
    */
    BOOST_URL_DECL
    bool
    has_port() const noexcept;

    /** Return the port

        This function returns the port specified
        in the authority, or an empty string if
        there is no port.

        @par BNF
        @code
        port        = *DIGIT
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.3"
            >3.2.3. Port (rfc3986)</a>

        @see
            @ref encoded_host,
            @ref encoded_host_and_port
            @ref has_port,
            @ref host,
            @ref host_type,
            @ref port_number.
    */
    BOOST_URL_DECL
    string_view
    port() const noexcept;

    /** Return the port as an intege

        This function returns the port as an
        integer if the authority specifies
        a port and the number can be represented.
        Otherwise it returns zero.

        @par BNF
        @code
        port        = *DIGIT
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.3"
            >3.2.3. Port (rfc3986)</a>

        @see
            @ref encoded_host,
            @ref encoded_host_and_port
            @ref has_port,
            @ref host,
            @ref host_type,
            @ref port,
    */
    BOOST_URL_DECL
    std::uint16_t
    port_number() const noexcept;

    /** Return the host and port

        This function returns the host and
        port of the authority as a single
        percent-encoded string.

        @par BNF
        @code
        authority   = [ userinfo "@" ] host [ ":" port ]
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.3"
            >3.2.3. Port (rfc3986)</a>

        @see
            @ref encoded_host,
            @ref encoded_host_and_port
            @ref has_port,
            @ref host,
            @ref host_type,
            @ref port,
    */
    BOOST_URL_DECL
    string_view
    encoded_host_and_port() const noexcept;

    //--------------------------------------------
    //
    // Parsing
    //
    //--------------------------------------------

    BOOST_URL_DECL friend result<authority_view>
        parse_authority(string_view s) noexcept;

    friend class url_view;

private:
    void apply(host_rule const& h) noexcept;
    void apply(authority_rule const& t) noexcept;
};

//------------------------------------------------

/** Parse an authority

    This function parses a string according to
    the authority grammar below, and returns an
    @ref authority_view referencing the string.
    Ownership of the string is not transferred;
    the caller is responsible for ensuring that
    the lifetime of the string extends until the
    view is no longer being accessed.

    @par BNF
    @code
    authority     = [ userinfo "@" ] host [ ":" port ]

    userinfo      = user [ ":" [ password ] ]

    user          = *( unreserved / pct-encoded / sub-delims )
    password      = *( unreserved / pct-encoded / sub-delims / ":" )

    host          = IP-literal / IPv4address / reg-name

    port          = *DIGIT
    @endcode

    @par Exception Safety
    Throws nothing.

    @return A view to the parsed authority

    @param s The string to parse

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2"
        >3.2. Authority (rfc3986)</a>

    @see
        @ref authority_view.
*/
BOOST_URL_DECL
result<authority_view>
parse_authority(
    string_view s) noexcept;

//------------------------------------------------

/** Format the encoded authority to the output stream

    This function serializes the encoded URL
    to the output stream.

    @par Example
    @code
    url_view u = parse_uri( "http://www.example.com/index.htm" );

    std::cout << u << std::endl;
    @endcode

    @return A reference to the output stream, for chaining

    @param os The output stream to write to

    @param a The URL to write
*/
BOOST_URL_DECL
std::ostream&
operator<<(
    std::ostream& os,
    authority_view const& a);

} // urls
} // boost

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_AUTHORITY_VIEW_HPP
#define BOOST_URL_IMPL_AUTHORITY_VIEW_HPP

namespace boost {
namespace urls {

// return length of [first, last)
inline
auto
authority_view::
len(
    int first,
    int last) const noexcept ->
        pos_t
{
    BOOST_ASSERT(first <= last);
    BOOST_ASSERT(last <= id_end);
    return offset(last) - offset(first);
}

// change id to size n
inline
void
authority_view::
set_size(
    int id,
    pos_t n) noexcept
{
    auto d = n - len(id);
    for(auto i = id;
        i <= id_end; ++i)
        offset_[i] += d;
}

} // urls
} // boost

#endif
#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_PARAMS_HPP
#define BOOST_URL_PARAMS_HPP

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_QUERY_PARAM_HPP
#define BOOST_URL_QUERY_PARAM_HPP


namespace boost {
namespace urls {

#ifndef BOOST_URL_DOCS
struct query_param;
namespace detail
{
    struct params_iterator_impl;
}
#endif

/** A query parameter view.

    Objects of this type represent a single key/value
    pair in a query string.
    The presence of a value is indicated by
    `has_value == true`.
    A value that is present with an empty string
    is distinct from a value that is absent.
    Ownership of the underlying character buffers
    representing the key and value is not transferred.
    The caller is responsible for ensuring that the
    buffers assigned to these members remains valid while
    the object exists.
*/
struct query_param_view
{
    /** The percent-encoded key
    */
    string_view key;

    /** The percent-encoded value

        The presence of a value is indicated by
        `has_value == true`.
        A value that is present with an empty string
        is distinct from a value that is absent.
    */
    string_view value;

    /** True if the value is present

        The presence of a value is indicated by
        `has_value == true`.
        A value that is present with an empty string
        is distinct from a value that is absent.
    */
    bool has_value = false;

#ifndef BOOST_URL_DOCS
#ifdef BOOST_NO_CXX14_AGGREGATE_NSDMI
    constexpr
    query_param_view(
        string_view key_ = {},
        string_view value_ = {},
        bool has_value_ = false ) noexcept
        : key(key_)
        , value(value_)
        , has_value(has_value_)
    {
    }
#endif
#endif
};

//------------------------------------------------

/** A query parameter value.

    Objects of this type represent a single
    key/value pair, possibly percent-encoded,
    in a query string.
    The presence of a value is indicated by
    `has_value == true`.
    A value that is present with an empty string
    is distinct from a value that is absent.
    Whether the strings are percent-encoded
    is determined by the container from which
    the value is obtained.
*/
struct query_param
{
    /** The key.
    */
    const_string key;

    /** The value.

        The presence of a value is indicated by
        `has_value == true`.
        A value that is present with an empty string
        is distinct from a value that is absent.
    */
    const_string value;

    /** True if the value is present

        The presence of a value is indicated by
        `has_value == true`.
        A value that is present with an empty string
        is distinct from a value that is absent.
    */
    bool has_value = false;

    /** Constructor
    */
    query_param() = default;

    /** Constructor
    */
    template<class Allocator>
    query_param(
        query_param_view const& v,
        Allocator const& a)
        : key(v.key, a)
        , value(v.has_value ?
            v.value :
            string_view(), a)
        , has_value(v.has_value)
    {
    }

    /** Constructor
    */
    query_param(
        const_string const& key_,
        const_string const& value_,
        bool has_value_)
        : key(key_)
        , value(value_)
        , has_value(has_value_)
    {
    }

    /** Conversion
    */
    operator query_param_view() const noexcept
    {
        if(has_value)
            return { key, value, true };
        return { key, string_view(), false };
    }

private:
    friend class params_view;
    friend class params;
    friend struct detail::params_iterator_impl;

    BOOST_URL_DECL
    query_param(
        char const* s,
        std::size_t nk,
        std::size_t nv,
        const_string::factory const& a);
};

} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_PARTS_BASE_HPP
#define BOOST_URL_DETAIL_PARTS_BASE_HPP


namespace boost {
namespace urls {
namespace detail {

// mix-in to provide part
// constants and variables
struct parts_base
{
    enum
    {
        id_scheme = -1, // trailing ':'
        id_user,        // leading "//"
        id_pass,        // leading ':', trailing '@'
        id_host,
        id_port,        // leading ':'
        id_path,
        id_query,       // leading '?'
        id_frag,        // leading '#'
        id_end          // one past the end
    };

    static
    constexpr
    pos_t zero_ = 0;

    static
    constexpr
    char const* const empty_ = "";
};

} // detail
} // urls
} // boost

#endif
#include <initializer_list>
#include <iterator>
#include <type_traits>

namespace boost {
namespace urls {

#ifndef BOOST_URL_DOCS
class url;
class params_encoded;
#endif

/** A random-access view of percent-decoded query parameters in a url.

   This view represents the range of percent-decoded
   query parameters in a url.

   Its iterator dereferences to read-only decoded
   query parameters in the underlying url.

 */
class params
    : private detail::parts_base
{
    friend class url;
    friend class params_encoded;

    url* u_ = nullptr;

    const_string::factory a_;

    template<class Allocator>
    params(
        url& u,
        Allocator const& a);

public:
    /** A read-only forward iterator to a decoded query parameter.

        This is a read-only forward iterator to
        the decoded query parameters.

     */
#ifdef BOOST_URL_DOCS
    using iterator = __see_below__;
#else
    class iterator;
#endif

    /// @copydoc iterator
    using const_iterator = iterator;

    /** A type which can represent a parameter as a value

        This type allows for making a copy of
        a parameter where ownership is retained
        in the copy.

    */
    using value_type = query_param;

    /// @copydoc value_type
    using reference = query_param;

    /// @copydoc value_type
    using const_reference = reference;

    /** An unsigned integer type to represent sizes
    */
    using size_type = std::size_t;

    /** A signed integer type to represent differences
    */
    using difference_type = std::ptrdiff_t;

    /** Constructor

        Copy constructor

    */
    params(params const&) = default;

    /** View assignment

        After the assignment, both views will point to
        the same underlying object.

    */
    params&
    operator=(params const&) & = default;

    /** Assignment

        The query parameters are replaced by the
        contents of the initializer list.

        The key and value strings must not
        reference the underlying URL buffer, or
        else the behavior is undefined.

        @param init Initializer list with query parameters

     */
    params&
    operator=(std::initializer_list<query_param_view> init);

    /** Assignment

        The query parameters are replaced by the
        contents of the initializer list.

        The key and value strings must not
        reference the underlying URL buffer, or
        else the behavior is undefined.

        @param init Initializer list with query parameters

     */
    void
    assign(std::initializer_list<
        query_param_view> init);

    /** Assignment

        The query parameters are replaced by the
        contents of the range.

        The key and value strings must not
        reference the underlying URL buffer, or
        else the behavior is undefined.

        @param first An iterator to the beginning of the query parameter range

        @param last An iterator to the end of the query parameter range

     */
    template<class FwdIt>
    void
    assign(FwdIt first, FwdIt last);

private:
    template<class FwdIt>
    void
    assign(FwdIt first, FwdIt last,
        std::forward_iterator_tag);

public:
#ifndef BOOST_URL_DOCS
    // Doxygen cannot render ` = delete`
    template<class FwdIt>
    void
    assign(FwdIt first, FwdIt last,
        std::input_iterator_tag) = delete;
#endif

    //--------------------------------------------
    //
    // Iterators
    //
    //--------------------------------------------

    /** Returns an iterator to the beginning

        Returns an iterator to the beginning of
        the container

        @return Iterator to beginning of the container

     */
    const_iterator
    begin() const noexcept;

    /** Returns an iterator to the end

        @return Iterator to the end of the container

     */
    iterator
    end() const noexcept;

    //--------------------------------------------
    //
    // Capacity
    //
    //--------------------------------------------

    /** Checks whether the container is empty

        @return True if container is empty

     */
    bool
    empty() const noexcept;

    /** Return the number of elements

        @return Number of elements in the container

     */
    std::size_t
    size() const noexcept;

    /** Clears the contents of the container

        This function clears the contents of the
        container as if calling
        `erase(begin(), end())`.

     */
    void
    clear() noexcept;

    /** Insert element

        This function insert an element at the
        specified container position.

        @return Iterator pointing to the new inserted element

        @param before Position at which the element should be inserted

        @param v Element to be inserted

      */
    iterator
    insert(
        iterator before,
        query_param_view const& v);

    /** Insert elements

        This function inserts a list of elements
        at the specified container position

        The key and value strings must not
        reference the underlying URL buffer, or
        else the behavior is undefined.

        @return Iterator pointing to the first inserted element

        @param before Position at which the elements should be inserted

        @param init Elements to be inserted

     */
    iterator
    insert(
        iterator before,
        std::initializer_list<
            query_param_view> init);

    /** Insert elements

        This function inserts a range of elements
        at the specified container position.

        The key and value strings must not
        reference the underlying URL buffer, or
        else the behavior is undefined.

        @return Iterator pointing to the first inserted element

        @param before Position at which the elements should be inserted

        @param first An iterator to the beginning of the query parameter range

        @param last An iterator to the end of the query parameter range

     */
    template<class FwdIt>
    iterator
    insert(
        iterator before,
        FwdIt first,
        FwdIt last);

private:
    template<class FwdIt>
    iterator
    insert(
        iterator before,
        FwdIt first,
        FwdIt last,
        std::forward_iterator_tag);

public:
#ifndef BOOST_URL_DOCS
    // Doxygen cannot render ` = delete`
    template<class FwdIt>
    iterator
    insert(
            iterator before,
            FwdIt first,
            FwdIt last,
            std::input_iterator_tag) = delete;
#endif

     /** Replace element

        This function replaces an element at
        the specified container position.

        @return Iterator to position where element was inserted

        @param pos Position whose element should be replaced

        @param value New value for the position

     */
    iterator
    replace(
        iterator pos,
        query_param_view const& value);

     /** Replace elements

        This function replaces a range of elements
        at a range of container positions

        The key and value strings must not
        reference the underlying URL buffer, or
        else the behavior is undefined.

        @return Iterator to position where the first inserted element

        @param from Iterator to beginning of the range to be replaced

        @param to Iterator to end of the range to be replaced

        @param first An iterator to the beginning of range to be inserted

        @param last An iterator to the end of the range to be inserted

     */
    template<class FwdIt>
    iterator
    replace(
        iterator from,
        iterator to,
        FwdIt first,
        FwdIt last);

     /** Replace elements

        This function replaces a list of elements
        at a range of container positions

        The key and value strings must not
        reference the underlying URL buffer, or
        else the behavior is undefined.

        @return Iterator to position where the first inserted element

        @param from Iterator to beginning of the range to be replaced

        @param to Iterator to end of the range to be replaced

        @param init List of elements to replace the range

     */
    iterator
    replace(
        iterator from,
        iterator to,
        std::initializer_list<
            query_param_view> init);

     /** Remove a query parameter from the container

        @return Iterator to position where the element was removed

        @param pos Iterator to element which should be removed

     */
    BOOST_URL_DECL
    iterator
    remove_value(
        iterator pos);

     /** Replace element

        This function replaces a query parameter
        at the specified container position

        @return Iterator to position where the element was replaced

        @param pos Position whose element should be replaced

        @param value New value for the element

     */
    BOOST_URL_DECL
    iterator
    replace_value(
        iterator pos,
        string_view value);

    /** Constructs a value at container position

        @return Iterator to position where the element was constructed

        @param pos Position where the new element should be constructed

        @param key Key of the new element

        @param value Value of the new element

     */
    iterator
    emplace_at(
        iterator pos,
        string_view key,
        string_view value);

    /** Constructs a key-only value at container position

        This function constructs a value at the
        specified container position.

        The new value has a specified query key and no query mapped value.

        @return Iterator to position where the element was constructed

        @param pos Position where the new element should be constructed

        @param key The key of the new element

      */
    iterator
    emplace_at(
        iterator pos,
        string_view key);

    /** Constructs a key-only query parameter at container position

        @return Iterator to position where the element was constructed

        @param before Position where the new element should be constructed

        @param key Key of the query param

        @param value Value of the query param

      */
    iterator
    emplace_before(
        iterator before,
        string_view key,
        string_view value);

    /** Constructs a value at container position

        @return Iterator to position where the element was constructed

        @param before Position where the new element should be constructed

        @param key Key of the new query param

      */
    iterator
    emplace_before(
        iterator before,
        string_view key);

    /** Erases element from container at specified position

        @return Iterator to position where the element was erased

        @param pos Position whose element should be erased

      */
    iterator
    erase(iterator pos);

    /** Erases range of elements from container

        @return Iterator to position where the first element was erased

        @param first An iterator to the beginning of the query parameter range

        @param last An iterator to the end of the query parameter range

      */
    BOOST_URL_DECL
    iterator
    erase(
        iterator first,
        iterator last);

    /** Erases elements associated with a specified key from container

        @return Number of elements erased from the container

        @param key Key of the element to be erased

      */
    BOOST_URL_DECL
    std::size_t
    erase(string_view key) noexcept;

    /** Construct key-only element at the end of the container

        @return Position where the element was constructed

        @param key String-like element key

      */
    iterator
    emplace_back(
        string_view key);

    /** Construct element at the end of the container

        @return Position where the element was constructed

        @param key Element key

        @param value Element value

      */
    iterator
    emplace_back(
        string_view key,
        string_view value);

    /** Insert value at the end of the container

        @param value Value to be inserted

      */
    void
    push_back(
        query_param_view const& value);

    //--------------------------------------------
    //
    // Lookup
    //
    //--------------------------------------------

    /** Count number of elements with a specified key

        @return Number of elements with a specified key

        @param key Element key

      */
    BOOST_URL_DECL
    std::size_t
    count(string_view key) const noexcept;

    /** Find element with a specified key

        @return Iterator pointing to element with a specified key

        @param key Element key

      */
    iterator
    find(string_view key) const noexcept;

    /** Find element with a specified key after position

        This function searches the range [`from`, `end`).

        `from==end` is valid.

        @return Iterator pointing to element with the specified key

        @param from First position to consider in search

        @param key Element key

      */
    BOOST_URL_DECL
    iterator
    find(
        iterator from,
        string_view key) const noexcept;

    /** Check if container contains an element with the specified key

        @return True if container contains an element with the specified key

        @param key Element key

      */
    bool
    contains(string_view key) const noexcept;

};

} // urls
} // boost

// VFALCO This include is at the bottom of
// url.hpp because of a circular dependency
//#include <boost/url/impl/params.hpp>

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_PARAMS_ENCODED_HPP
#define BOOST_URL_PARAMS_ENCODED_HPP

#include <initializer_list>
#include <iterator>
#include <type_traits>

namespace boost {
namespace urls {

#ifndef BOOST_URL_DOCS
class url;
#endif

/** A random-access view of percent-encoded query parameters.

    The elements of this class dereference
    directly to the percent-encoded values as
    string views.

    This avoids the necessity allocating decoded
    strings.

 */
class params_encoded
    : private detail::parts_base
{
    friend class url;

    url* u_ = nullptr;

    explicit
    params_encoded(
        url& u) noexcept;

public:
    /** A read-only forward iterator to an encoded query parameter.

        This is a read-only forward iterator to
        the encoded query parameters.

     */
#ifdef BOOST_URL_DOCS
    using iterator = __see_below__;
#else
    class iterator;
#endif

    /// @copydoc iterator
    using const_iterator = iterator;

    /** A type which can represent a parameter as a value

        This type allows for making a copy of
        a parameter where ownership is retained
        in the copy.

    */
    using value_type = query_param;

    /** A type which can represent a parameter as a const reference

        This type does not make a copy of a parameter
        and ownership is retained by the container.

    */
    using reference = query_param_view;

    /// @copydoc reference
    using const_reference = query_param_view;

    /** An unsigned integer type to represent sizes
     */
    using size_type = std::size_t;

    /** A signed integer type to represent differences
     */
    using difference_type = std::ptrdiff_t;

    /** Constructor

        Copy constructor

    */
    params_encoded(params_encoded const&) = default;

    /** Assignment

        After the assignment, both views will point to
        the same underlying object.

    */
    params_encoded&
    operator=(params_encoded const&) & = default;

    /** Assignment from initializer list

        The query parameters are replaced by the
        contents of the initializer list.

        The behavior is undefined if the elements
        belong to the container.

        @return Pointer to this params object

        @param init Initializer list with query parameters

      */
    params_encoded&
    operator=(std::initializer_list<
        reference> init);

    /** Assignment from initializer list

        The query parameters are replaced by the
        contents of the initializer list.

        The behavior is undefined if the elements
        belong to the container.

        @param init Initializer list with query parameters

    */
    void
    assign(std::initializer_list<
        reference> init);

    /** Assignment from iterators

        The query parameters are replaced by the
        contents of the range.

        The behavior is undefined if the elements
        belong to the container.

        @param first Iterator to first query parameter

        @param last Iterator to one past the last query parameter

      */
    template<class FwdIt>
    void
    assign(FwdIt first, FwdIt last);

private:
    /** Assignment from forward iterators
      */
    template<class FwdIt>
    void
    assign(FwdIt first, FwdIt last,
        std::forward_iterator_tag);

public:
#ifndef BOOST_URL_DOCS
    /** Assignment from input iterators
     */
    template<class FwdIt>
    void
    assign(FwdIt first, FwdIt last,
           std::input_iterator_tag) = delete;
#endif

    /** Return a view of this container as percent-decoded query parameters

        @tparam Allocator An Allocator type

        @return A percent-decoded container of query parameters

        @param alloc Allocator to create read-only strings for decoded strings

    */
    template<class Allocator =
        std::allocator<char>>
    params
    decoded(Allocator const& alloc = {}) const;

    //--------------------------------------------
    //
    // Element Access
    //
    //--------------------------------------------

    /** Return indexed element with bounds checking

        @return Query param reference

        @param pos Position

      */
    reference
    at(std::size_t pos) const;

    /** Return first element matching key with bounds checking

        @return Query param mapped value

        @param key Element key

      */
    BOOST_URL_DECL
    string_view
    at(string_view key) const;

    /** Return indexed element

        @return Query params reference

        @param pos Element position

      */
    BOOST_URL_DECL
    reference
    operator[](
        std::size_t pos) const;

    /** Return first element

        @return Query params reference

      */
    reference
    front() const;

    /** Return last element

        @return Query params reference
      */
    reference
    back() const;

    //--------------------------------------------
    //
    // Iterators
    //
    //--------------------------------------------

    /** Returns an iterator to the beginning of container

        @return Iterator to first element
      */
    iterator
    begin() const noexcept;

    /** Returns an iterator to one past the last element

        @return Iterator to one past the last element

      */
    iterator
    end() const noexcept;

    //--------------------------------------------
    //
    // Capacity
    //
    //--------------------------------------------

    /// Return true if the container is empty
    bool
    empty() const noexcept;

    /// Returns the number of query parameters in the url
    std::size_t
    size() const noexcept;

    /** Clears the contents of the container

        This function clears the contents of the
        container as if calling
        `erase(begin(), end())`.

      */
    void
    clear() noexcept;

    //--------------------------------------------

    /** Insert element in container

        This function inserts an element at the
        specified container position.

        The behavior is undefined if the element
        belongs to the container.

        @return Iterator pointing to the inserted element

        @param before Position at which the element should be inserted

        @param v Element to be inserted

      */
    iterator
    insert(
        iterator before,
        reference const& v);

    /** Insert elements in container

        This function inserts a list of elements
        at the specified container position.

        The behavior is undefined if the elements
        belong to the container.

        @return Iterator pointing to the first inserted element

        @param before Position at which the elements should be inserted

        @param init Elements to be inserted

      */
    iterator
    insert(
        iterator before,
        std::initializer_list<
            reference> init);

    /** Insert elements in container

        This function inserts a range of elements
        at the specified container position.

        The behavior is undefined if any elements of
        the range belong to the container.

        @return Iterator pointing to the first inserted element

        @param before Position at which the elements should be inserted

        @param first Iterator pointing to first element to be inserted

        @param last Iterator pointing to one past the last element to be inserted

      */
    template<class FwdIt>
    iterator
    insert(
        iterator before,
        FwdIt first,
        FwdIt last);

private:
    /** Insert forward range of elements at container position
     */
    template<class FwdIt>
    iterator
    insert(
        iterator before,
        FwdIt first,
        FwdIt last,
        std::forward_iterator_tag);

public:
#ifndef BOOST_URL_DOCS
    // Doxygen cannot render ` = delete`
    template<class FwdIt>
    iterator
    insert(
        iterator before,
        FwdIt first,
        FwdIt last,
        std::input_iterator_tag) = delete;
#endif

    //--------------------------------------------

    /** Replace container element

        This function replaces an element at the
        specified container position.

        The behavior is undefined if the new element
        belongs to the existing container.

        @return Iterator to position where element was inserted

        @param pos Position whose element should be replaced

        @param value New value for the position

     */
    iterator
    replace(
        iterator pos,
        reference const& value);

    /** Replace container elements

        This function replaces a range of elements
        at a range of container positions.

        The behavior is undefined if any elements of
        the range belong to the container.

        @return Iterator to position where the first inserted element

        @param from Iterator to first element to be replaced

        @param to Iterator to one past last element to be replaced

        @param first Iterator to first element to be inserted

        @param last Iterator to one past last element to be inserted

      */
    template<class FwdIt>
    iterator
    replace(
        iterator from,
        iterator to,
        FwdIt first,
        FwdIt last);

    /** Replace container elements

        This function replaces a list of elements
        at a range of container positions.

        The behavior is undefined if any elements of
        the initializer_list belong to the
        container.

        @return Iterator to position where the first inserted element

        @param from Iterator to first element to be replaced

        @param to Iterator to one past last element to be replaced

        @param init List of elements to replace the range

      */
    iterator
    replace(
        iterator from,
        iterator to,
        std::initializer_list<
            reference> init);

    /** Remove a value from the container

        @return Iterator to position where the element was removed

        @param pos Iterator to element which should be removed

    */
    BOOST_URL_DECL
    iterator
    remove_value(
        iterator pos);

    /** Replace element value

        This function replaces a value at the
        specified container position while
        maintaining the original key.

        @return Iterator to position where the element was replaced

        @param pos Position whose element should be replaced

        @param value New value for the element

     */
    BOOST_URL_DECL
    iterator
    replace_value(
        iterator pos,
        string_view value);

    //--------------------------------------------

    /** Constructs a value at container position

        @return Iterator to position where the element was constructed

        @param pos Position where the new element should be constructed

        @param key Key of the new element

        @param value Value of the new element

      */
    iterator
    emplace_at(
        iterator pos,
        string_view key,
        string_view value);

    /** Constructs a key-only value at container position

        This function constructs a value at the
        specified container position.

        The new value has a specified query key
        and no query mapped value.

        @return Iterator to position where the element was constructed

        @param pos Position where the new element should be constructed

        @param key The key of the new element

      */
    iterator
    emplace_at(
        iterator pos,
        string_view key);

    /** Constructs a key-only query parameter at container position

        @return Iterator to position where the element was constructed

        @param before Position where the new element should be constructed

        @param key Key of the query param

        @param value Value of the query param

      */
    iterator
    emplace_before(
        iterator before,
        string_view key,
        string_view value);

    /** Constructs a value at container position

        @return Iterator to position where the element was constructed

        @param before Position where the new element should be constructed

        @param key Key of the new query param

      */
    iterator
    emplace_before(
        iterator before,
        string_view key);

    //--------------------------------------------

    /** Erases element from container at position

        @return Iterator to position where the element was erased

        @param pos Position whose element should be erased

      */
    iterator
    erase(iterator pos);

    /** Erases range of elements from container

        @return Iterator to position where the first element was erased

        @param first Iterator pointing to the first element to be erased

        @param last Iterator pointing to one past the last element to be erased

      */
    BOOST_URL_DECL
    iterator
    erase(
        iterator first,
        iterator last);

    /** Erases elements associated with a specified key from container

        @return Number of elements erased from the container

        @param key Key of the element to be erased

      */
    BOOST_URL_DECL
    std::size_t
    erase(string_view key) noexcept;

    //--------------------------------------------

    /** Construct element at the last position of the container

        @return Position where the element was constructed

        @param key Element key

      */
    iterator
    emplace_back(
        string_view key);

    /** Construct element at the last position of the container

        @return Position where the element was constructed

        @param key Element key

        @param value Element value

      */
    iterator
    emplace_back(
        string_view key,
        string_view value);

    /** Insert value at the last position of the container

        @param value Value to be inserted
      */
    void
    push_back(
        reference const& value);

    /** Remove element at the last position of the container
      */
    void
    pop_back() noexcept;

    //--------------------------------------------
    //
    // Lookup
    //
    //--------------------------------------------

    /** Count number of elements with a specified key

        @return Number of elements with a specified key

        @param key Element key

      */
    BOOST_URL_DECL
    std::size_t
    count(string_view key) const noexcept;

    /** Find element with a specified key

        @return Iterator pointing to element with a specified key

        @param key Element key

      */
    iterator
    find(string_view key) const noexcept;

    /** Find element with a specified key after specified position

        This function searches the range `[from, end)`.

        The range where `from==end` is also valid.

        @return Iterator pointing to element with the specified key

        @param from First position to consider in search

        @param key Element key

      */
    BOOST_URL_DECL
    iterator
    find(
        iterator from,
        string_view key) const noexcept;

    /** Check if container contains an element with the specified key

        @return True if container contains an element with the specified key

        @param key Element key

      */
    bool
    contains(string_view key) const noexcept;
};

} // urls
} // boost

// VFALCO This include is at the bottom of
// url.hpp because of a circular dependency
//#include <boost/url/impl/params_encoded.hpp>

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_PARAMS_ENCODED_VIEW_HPP
#define BOOST_URL_PARAMS_ENCODED_VIEW_HPP

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_PARAMS_VIEW_HPP
#define BOOST_URL_PARAMS_VIEW_HPP

#include <iterator>
#include <type_traits>

namespace boost {
namespace urls {

#ifndef BOOST_URL_DOCS
class url_view;
class params_encoded_view;
#endif

class params_view
    : private detail::parts_base
{
    friend class url_view;
    friend class params_encoded_view;

    string_view s_;
    std::size_t n_;
    const_string::factory a_;

    template<class Allocator>
    params_view(
        string_view s,
        std::size_t n,
        Allocator const& a);

public:
    /** A read-only forward iterator to a decoded query parameter.

        This is a read-only forward iterator to
        the decoded query parameters.

     */
#ifdef BOOST_URL_DOCS
    using iterator = __see_below__;
#else
    class iterator;
#endif

    /// @copydoc iterator
    using const_iterator = iterator;

    /** A type which can represent a parameter as a value

        This type allows for making a copy of
        a parameter where ownership is retained
        in the copy.

    */
    using value_type = query_param;

    /// @copydoc value_type
    using reference = query_param;

    /// @copydoc value_type
    using const_reference = value_type;

    /** An unsigned integer type used to represent size.
    */
    using size_type = std::size_t;

    /** A signed integer type used to represent differences.
    */
    using difference_type = std::ptrdiff_t;

    /** Constructor

        Copy constructor
    */
    params_view(params_view const&) = default;

    /** Assignment

        After the assignment, both views will point to
        the same underlying object.
    */
    params_view&
    operator=(params_view const&) & = default;

    //--------------------------------------------
    //
    // Element Access
    //
    //--------------------------------------------

    BOOST_URL_DECL
    const_string
    at(string_view key) const;

    //--------------------------------------------
    //
    // Iterators
    //
    //--------------------------------------------

    BOOST_URL_DECL
    iterator
    begin() const noexcept;

    BOOST_URL_DECL
    iterator
    end() const noexcept;

    //--------------------------------------------
    //
    // Capacity
    //
    //--------------------------------------------

    bool
    empty() const noexcept;

    std::size_t
    size() const noexcept;

    //--------------------------------------------
    //
    // Lookup
    //
    //--------------------------------------------

    BOOST_URL_DECL
    std::size_t
    count(string_view key) const noexcept;

    iterator
    find(string_view key) const noexcept;

    /** Search [from, end), from==end is valid
    */
    BOOST_URL_DECL
    iterator
    find(
        iterator from,
        string_view key) const noexcept;

    bool
    contains(string_view key) const noexcept;
};

} // urls
} // boost

// VFALCO This include is at the bottom of
// url_view.hpp because of a circular dependency
//#include <boost/url/impl/params_view.hpp>

#endif
#include <iterator>
#include <type_traits>

namespace boost {
namespace urls {

#ifndef BOOST_URL_DOCS
class url_view;
#endif

/** A forward range of read-only encoded query parameters.

    Objects of this type represent an iterable
    range of query parameters, where each parameter
    is represented by percent-encoded key and value
    strings.

    Dereferenced iterators return a structure of
    string views into the underlying character
    buffer.

    Ownership of the underlying characters is
    not transferred; the character buffer used
    to construct the container must remain
    valid for as long as the container exists.

    The container models a multi-map. That is,
    duplicate keys are possible.

    A view of encoded parameters in a URL's query
    can be obtained by calling
        @ref url_view::encoded_params.
    Alternatively, to obtain encoded parameters
    from a query stored in a string call the
    parsing function (see below).

    @par Example

    A query parameter string is parsed into
    encoded params, then each parameer is
    printed to standard output:

    @code
    params_encoded_view pev = parse_query_params( "cust=John&id=42&last_invoice=1001" ).value();

    for(auto e : pev)
        std::cout << "key = " << e.key << ", value = " << e.value << std::endl;
    @endcode

    @par Complexity
    Iterator increment runs in linear time on
    the size of the parameter.
    All other operations run in constant time.
    No operations allocate memory.

    @see
        @ref parse_query_params.
*/
class params_encoded_view
    : private detail::parts_base
{
    friend class url_view;

    string_view s_;
    std::size_t n_ = 0;

    params_encoded_view(
        string_view s,
        std::size_t n) noexcept;

public:
    /** A read-only forward iterator to an encoded query parameter.

        This is a read-only forward iterator to
        the encoded query parameters.

     */
#ifdef BOOST_URL_DOCS
    using iterator = __see_below__;
#else
    class iterator;
#endif

    /// @copydoc iterator
    using const_iterator = iterator;

    /** A type which can represent a parameter as a value

        This type allows for making a copy of
        a parameter where ownership is retained
        in the copy.

    */
    using value_type = query_param;

    /** A type which can represent a parameter as a const reference

        This type does not make a copy of a parameter
        and ownership is retained by the container.

    */
    using reference = query_param_view;

    /// @copydoc reference
    using const_reference = query_param_view;

    /** An unsigned integer type used to represent size.
    */
    using size_type = std::size_t;

    /** A signed integer type used to represent differences.
    */
    using difference_type = std::ptrdiff_t;

    /** Return a view of this container as percent-decoded query parameters

        This function returns a new view over the
        same underlying character buffer where each
        segment is returned as a read-only string
        with percent-decoding applied using the
        optionally specified allocator.

        The decoded view does not take ownership of
        the underlying character buffer; the caller
        is still responsible for ensuring that the
        buffer remains valid until all views which
        reference it are destroyed.

        @par Exceptions
        Calls to allocate may throw.

        @return A view to decoded path segments.

        @param alloc The allocator the returned
        view will use for all string storage. If
        this parameter is ommitted, the default
        allocator will be used.
    */
    template<class Allocator =
        std::allocator<char>>
    params_view
    decoded(Allocator const& alloc = {}) const;

    /** Constructor

        Copy constructor
    */
    params_encoded_view(params_encoded_view const&) = default;

    /** Assignment

        After the assignment, both views will point to
        the same underlying object.
    */
    params_encoded_view&
    operator=(params_encoded_view const&) & = default;

    //--------------------------------------------
    //
    // Element Access
    //
    //--------------------------------------------

    /** Return the first element matching the key.

        This function searches the container for
        the specified encoded key. If the key
        is found, the corresponding value is
        returned. If the value does not exist,
        an empty string is returned. Otherwise,
        if the key does not exist, an exception
        is thrown.

        If multiple parameters match the key,
        only the first match is returned.

        @par Example
        @code
        params_encoded_view pev = parse_query_params( "cust=John&id=42&last_invoice=1001" ).value();

        std::cout << pev.at( "cust" ); // prints "John"
        @endcode

        @return The value as an encoded string.

        @param key The encoded key.

        @throws std::out_of_range Key not found.
    */
    BOOST_URL_DECL
    string_view
    at(string_view key) const;

    //--------------------------------------------
    //
    // Iterators
    //
    //--------------------------------------------

    /** Return an iterator to the beginning.
    */
    BOOST_URL_DECL
    iterator
    begin() const noexcept;

    /** Return an iterator to the end.
    */
    BOOST_URL_DECL
    iterator
    end() const noexcept;

    //--------------------------------------------
    //
    // Capacity
    //
    //--------------------------------------------

    /** Return true if the range contains no elements.
    */
    bool
    empty() const noexcept;

    /** Return the number of elements in the range.
    */
    std::size_t
    size() const noexcept;

    //--------------------------------------------
    //
    // Lookup
    //
    //--------------------------------------------

    /** Return the number of matching elements.

        This function returns the total number
        of elements whose key matches the
        specified encoded string.

        @par Exception Safety
        Throws nothing.

        @return The number of elements.

        @param key The encoded key.
    */
    BOOST_URL_DECL
    std::size_t
    count(string_view key) const noexcept;

    /** Return the first element matching the key

        This function returns the first
        element which matches the specified
        percent-encoded key. If no element
        matches, then an iterator to the end
        is returned.

        @par Exception Safety
        Throws nothing.

        @return An iterator to the element.

        @param key The encoded key.
    */
    iterator
    find(string_view key) const noexcept;

    /** Return the next element matching the key

        This function returns the first
        element which matches the specified
        percent-encoded key, starting the
        search at `*from` inclusive.

        If no element matches the key, then an
        iterator to the end is returned.

        @par Exception Safety
        Throws nothing.

        @return An iterator to the element

        @param from An iterator to the element
        to start from. The range `[ from, end() )`
        is searched.

        @param key The encoded key.
    */
    BOOST_URL_DECL
    iterator
    find(
        iterator from,
        string_view key) const noexcept;

    /** Return true if at least one matching element exists.

        This function returns true if at least one
        element matches the specified percent-encoded
        key.

        @par Exception Safety
        Throws nothing.

        @return `true` if a matching element exists.

        @param key The encoded key.
    */
    bool
    contains(string_view key) const noexcept;

    //--------------------------------------------
    //
    // Parsing
    //
    //--------------------------------------------

    BOOST_URL_DECL friend
    result<params_encoded_view>
    parse_query_params(string_view s) noexcept;
};

//------------------------------------------------

/** Return a query params view from a parsed string, using query-params bnf

    This function parses the string and returns the
    corresponding query params object if the string
    is valid, otherwise sets the error and returns
    an empty range. The query string should not
    include the leading question mark.

    @par BNF
    @code
    query-params    = query-param *( "&" query-param )
    query-param     = key [ "=" value ]

    key             = *qpchar
    value           = *( qpchar / "=" )
    qpchar          = unreserved
                    / pct-encoded
                    / "!" / "$" / "'" / "(" / ")"
                    / "*" / "+" / "," / ";"
                    / ":" / "@" / "/" / "?"
    @endcode

    @par Exception Safety
    No-throw guarantee.

    @return The encoded parameter view, or an
    error if parsing failed.

    @param s The string to parse

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.4">
        3.4. Query (rfc3986)</a>

    @see
        @ref params_encoded_view,
        @ref result.
*/
BOOST_URL_DECL
result<params_encoded_view>
parse_query_params(
    string_view s) noexcept;

} // urls
} // boost

// VFALCO This include is at the bottom of
// url_view.hpp because of a circular dependency
//#include <boost/url/impl/params_encoded_view.hpp>

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_PCT_ENCODING_TYPES_HPP
#define BOOST_URL_PCT_ENCODING_TYPES_HPP

#include <cstddef>

namespace boost {
namespace urls {

/** A valid percent-encoded string
*/
struct pct_encoded_str
{
    /** A string holding the encoded characters
    */
    string_view str;

    /** The number of bytes needed to hold the decoded string
    */
    std::size_t decoded_size = 0;
};

} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_SCHEME_HPP
#define BOOST_URL_SCHEME_HPP


namespace boost {
namespace urls {

/* VFALCO NOTE The formatting of javadocs
               for enums is the way it is
   to work around an output bug in Doxygen!
*/

/** Identifies a known URL scheme

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.1"
        >3.1. Scheme (rfc3986)</a>
*/
enum class scheme : unsigned char
{
    /** Indicates that no scheme is present
    */
    none = 0,

    /** Indicates the scheme is not a well-known scheme
    */
    unknown,

    /**
     * File Transfer Protocol (FTP)

       FTP is a standard communication protocol
       used for the transfer of computer files
       from a server to a client on a computer
       network.

       @par Specification
       @li <a href="https://datatracker.ietf.org/doc/html/draft-yevstifeyev-ftp-uri-scheme">
           The 'ftp' URI Scheme</a>
    */
    ftp,

    /**
     * File URI Scheme

       The File URI Scheme is typically used
       to retrieve files from within one's
       own computer.

       @par Specification
       @li <a href="https://datatracker.ietf.org/doc/html/rfc8089">
           The "file" URI Scheme (rfc8089)</a>
    */
    file,

    /**
     * The Hypertext Transfer Protocol URI Scheme

       URLs of this type indicate a resource which
       is interacted with using the HTTP protocol.

       @par Specification
       @li <a href="https://datatracker.ietf.org/doc/html/rfc7230">
            Hypertext Transfer Protocol (HTTP/1.1) (rfc7230)</a>
    */
    http,

    /**
     * The Secure Hypertext Transfer Protocol URI Scheme

       URLs of this type indicate a resource which
       is interacted with using the Secure HTTP
       protocol.

       @par Specification
       @li <a href="https://datatracker.ietf.org/doc/html/rfc7230">
            Hypertext Transfer Protocol (HTTP/1.1) (rfc7230)</a>
    */
    https,

    /**
     * The WebSocket URI Scheme

       URLs of this type indicate a resource which
       is interacted with using the WebSocket protocol.

       @par Specification
       @li <a href="https://datatracker.ietf.org/doc/html/rfc6455">
            The WebSocket Protocol (rfc6455)</a>
    */
    ws,

    /**
     * The Secure WebSocket URI Scheme

       URLs of this type indicate a resource which
       is interacted with using the Secure WebSocket
       protocol.

       @par Specification
       @li <a href="https://datatracker.ietf.org/doc/html/rfc6455">
            The WebSocket Protocol (rfc6455)</a>
    */
    wss
};

/** Return the known scheme for a non-normalized string, if known

    If the string does not identify a known
    scheme, the value @ref scheme::unknown is
    returned.

    @par BNF
    @code
    scheme      = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
    @endcode

    @return The known scheme

    @param s The string holding the scheme

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.1"
        >3.1. Scheme (rfc3986)</a>
*/
BOOST_URL_DECL
scheme
string_to_scheme(string_view s) noexcept;

/** Return the normalized string for a known scheme

    @return A string representing the known scheme

    @param s The known scheme constant
*/
BOOST_URL_DECL
string_view
to_string(scheme s) noexcept;

} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_SEGMENTS_HPP
#define BOOST_URL_SEGMENTS_HPP

#include <initializer_list>
#include <iterator>

namespace boost {
namespace urls {

#ifndef BOOST_URL_DOCS
class url;
#endif

/** A container referencing a random-access range of modifiable, percent-decoded path segments.

    This class implements a <em>RandomAccessContainer</em>
    representing the path segments in a @ref url as
    percent-encoded strings. Ownership of the segments
    is not transferred; the container references the
    buffer in the url. Therefore, the lifetime of the
    url must remain valid until this container no
    longer exists.

    Objects of this type are not constructed directly;
    Instead, call the corresponding non-const member
    function of @ref url to obtain an instance of
    the container:

    @par Example
    @code
    url u = parse_relative_ref( "/path/to/file.txt" );

    segments se = u.segments();

    for( segments::value_type s : se )
        std::cout << s << std::endl;
    @endcode

    The @ref reference and @ref const_reference
    nested types are defined as publicly accessible
    nested classes. They proxy the behavior of a
    reference to a percent-encoded string in the
    underlying URL. The primary use of these
    references is to provide l-values that can be
    returned from element-accessing operations.
    Any reads or writes which happen through a
    @ref reference or @ref const_reference
    potentially read or write the underlying
    @ref url.

    @see
        @ref url.
*/
class segments
    : private detail::parts_base
{
    url* u_ = nullptr;
    const_string::factory a_;

    friend class url;

    template<class Allocator>
    explicit
    segments(
        url& u,
        Allocator const& a);

public:
    /** A read-only bidirectional iterator to a decoded segment.

        This is a read-only bidirectional iterator to
        the decoded segments.

     */
#ifdef BOOST_URL_DOCS
    using iterator = __see_below__;
#else
    class iterator;
#endif

    /// @copydoc iterator
    using const_iterator = iterator;

    /** A type which can represent a segment as a value

        This type allows for making a copy of
        a segment where ownership is retained
        in the copy.
    */
    using value_type = const_string;

    /// @copydoc value_type
    using reference = const_string;

    /// @copydoc value_type
    using const_reference = const_string;

    /** An unsigned integer type
    */
    using size_type = std::size_t;

    /** A signed integer type
    */
    using difference_type = std::ptrdiff_t;

    //--------------------------------------------
    //
    // Members
    //
    //--------------------------------------------

    /** Returns true if this contains an absolute path.

        Absolute paths always start with a
        forward slash ('/').
    */
    bool
    is_absolute() const noexcept;

    /** Constructor

        Copy constructor
    */
    segments(segments const&) = default;

    /** Assignment

        After the assignment, both views will point to
        the same underlying object.
    */
    segments&
    operator=(segments const&) & = default;

    /** Replace the contents of the container

        This function replaces the contents with
        an initializer list of unencoded strings.
        The behavior is undefined if any
        string refers to the contents of `*this`.
        All iterators and references to elements
        of the container are invalidated,
        including the @ref end iterator.
        @par Example
        @code
        url u = parse_relative_uri( "/path/to/file.txt" );
        u.segments() = { "etc", "init.rc" };
        assert( u.encoded_path() == "/etc/init.rc") );
        @endcode

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.
        Exceptions thrown on invalid input.

        @param init An initializer list of strings.
    */
    segments&
    operator=(std::initializer_list<string_view> init);

    /** Replace the contents of the container

        This function replaces the contents
        with a range of percent-encoded
        strings.
        Each string must contain a valid
        percent-encoding or else an
        exception is thrown.
        The behavior is undefined if either
        argument is an iterator into `*this`.
        All iterators and references to elements
        of the container are invalidated,
        including the @ref end iterator.

        @par Requires
        @code
        std::is_convertible< std::iterator_traits< FwdIt >::value_type, string_view >::value == true
        @endcode

        @par Example
        @code
        url u = parse_relative_uri( "/path/to/file.txt" );

        segments se = u.segments();

        std::vector< std::string > v = { "etc", "init.rc" };

        se.insert( u.end() - 1, v.begin(), v.end() );

        assert( u.encoded_path() == "/etc/init.rc") );
        @endcode

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.
        Exceptions thrown on invalid input.

        @param first An iterator to the first
        element in the range

        @param last An iterator to one past the
        last element in the range

        @throw std::invalid_argument invalid percent-encoding
    */
    template<class FwdIt>
#ifdef BOOST_URL_DOCS
    void
#else
    typename std::enable_if<
        std::is_convertible<typename
            std::iterator_traits<
                FwdIt>::value_type,
            string_view>::value>::type
#endif
    assign(FwdIt first, FwdIt last);

    //--------------------------------------------
    //
    // Element Access
    //
    //--------------------------------------------

    /** Return the first element
    */
    const_string
    front() const;

    /** Return the last element
    */
    const_string
    back() const;

    //--------------------------------------------
    //
    // Iterators
    //
    //--------------------------------------------

    /** Return an iterator to the beginning.
    */
    iterator
    begin() const noexcept;

    /** Return an iterator to the end.
    */
    iterator
    end() const noexcept;

    //--------------------------------------------
    //
    // Capacity
    //
    //--------------------------------------------

    /** Return true if the container is empty

        This function returns true if there are
        no elements in the container. That is, if
        the underlying path is the empty string.
    */
    bool
    empty() const noexcept;

    /** Return the number of elements in the container

        This function returns the number of
        elements in the underlying path. Empty
        segments count towards this total.

        @par Exception Safety
        Throws nothing.
    */
    std::size_t
    size() const noexcept;

    //--------------------------------------------
    //
    // Modifiers
    //
    //--------------------------------------------

    /** Remove the contents of the container

        This function removes all the segments
        from the container, leaving the
        underlying URL with an empty path.

        @par Postconditions
        @code
        empty() == true
        @endcode

        @par Exception Safety
        Throws nothing.
    */
    void
    clear() noexcept;

    //--------------------------------------------

    /** Insert an element

        This function inserts a segment specified
        by the percent-encoded string `s`, at the
        position preceding `before`.
        The string must contain a valid
        percent-encoding, or else an exception
        is thrown.
        All references and iterators starting
        from the newly inserted element and
        up to and including the last element
        and @ref end iterators are invalidated.

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.
        Exceptions thrown on invalid input.

        @return An iterator pointing to the
        inserted value.

        @param before An iterator before which the
        new element should be inserted.

        @param s A valid percent-encoded string
        to be inserted.

        @throw std::invalid_argument invalid percent-encoding
    */
    BOOST_URL_DECL
    iterator
    insert(
        iterator before,
        string_view s);

    /** Insert a range of segments

        This function inserts a range of unencoded
        strings passed as an initializer-list.
        All references and iterators starting
        from the newly inserted elements and
        up to and including the last element
        and @ref end iterators are invalidated.

        @par Example
        @code
        url u = parse_relative_uri( "/path/file.txt" );
        segments se = u.segments();
        se.insert( u.end() - 1, { "to", "the" } );
        assert( u.encoded_path() == "/path/to/the/file.txt") );
        @endcode

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.

        @note Behavior is undefined if any elements of
        initializer_list belong to the container

        @return An iterator to one past the last
        newly inserted element or `before` if
        the range is empty.

        @param before An iterator before which the
        new elements should be inserted.

        @param init The initializer list containing
        unencoded segments to insert.
    */
    iterator
    insert(
        iterator before,
        std::initializer_list<string_view> init);

    /** Insert a range of segments

        This function inserts a range
        of percent-encoded strings.
        Each string must contain a valid
        percent-encoding or else an
        exception is thrown.
        The behavior is undefined if either
        argument is an iterator into `this`.
        All references and iterators starting
        from the newly inserted elements and
        up to and including the last element
        and @ref end iterators are invalidated.

        @par Requires
        @code
        std::is_convertible< std::iterator_traits< FwdIt >::value_type, string_view >::value == true
        @endcode

        @par Example
        @code
        url u = parse_relative_uri( "/path/file.txt" );

        segments se = u.segments();

        std::vector< std::string > v = { "to", "the" };

        se.insert( u.end() - 1, v.begin(), v.end() );

        assert( u.encoded_path() == "/path/to/the/file.txt") );
        @endcode

        @note Behavior is undefined if any elements of the range
        belong to the container

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.
        Exceptions thrown on invalid input.

        @return An iterator to one past the last
        newly inserted element or `before` if
        the range is empty.

        @param before An iterator before which the
        new element should be inserted.

        @param first An iterator to the first
        element to insert.

        @param last An iterator to one past the
        last element to insert.

        @throw std::invalid_argument invalid percent-encoding
    */
    template<class FwdIt>
#ifdef BOOST_URL_DOCS
    iterator
#else
    typename std::enable_if<
        std::is_convertible<typename
            std::iterator_traits<
                FwdIt>::value_type,
            string_view>::value,
        iterator>::type
#endif
    insert(
        iterator before,
        FwdIt first,
        FwdIt last);

private:
    template<class FwdIt>
    iterator
    insert(
        iterator before,
        FwdIt first,
        FwdIt last,
        std::input_iterator_tag) = delete;

    template<class FwdIt>
    iterator
    insert(
        iterator before,
        FwdIt first,
        FwdIt last,
        std::forward_iterator_tag);
public:

    //--------------------------------------------

    iterator
    replace(
        iterator pos,
        string_view s);

    iterator
    replace(
        iterator from,
        iterator to,
        std::initializer_list<
            string_view> init);

    template<class FwdIt>
#ifdef BOOST_URL_DOCS
    iterator
#else
    typename std::enable_if<
        std::is_convertible<typename
            std::iterator_traits<
                FwdIt>::value_type,
            string_view>::value,
        iterator>::type
#endif
    replace(
        iterator from,
        iterator to,
        FwdIt first,
        FwdIt last);

    //--------------------------------------------

    /** Erase an element

        This function erases the element pointed
        to by `pos`, which must be a valid
        iterator for the container.
        All references and iterators starting
        from pos and up to and including
        the last element and @ref end iterators
        are invalidated.

        @par Preconditions
        `pos` points to a valid element in
        this container.

        @par Example
        @code
        url u = parse_relative_uri( "/path/to/file.txt" );

        segments se = u.segments();

        se.erase( se.begin() + 1 );

        assert( u.encoded_path() == "/path/file.txt" );
        @endcode

        @par Exception Safety
        Throws nothing.

        @return An iterator following
        the last element erased.

        @param pos An iterator to the
        element to erase.
    */
    iterator
    erase(
        iterator pos) noexcept;

    /** Erase a range of elements

        This function erases the elements
        in the range `[first, last)`, which
        must be a valid range in the container.
        All references and iterators starting
        from `first` and up to and including
        the last element and @ref end iterators
        are invalidated.

        @par Preconditions
        `[first, last)` is a valid range in
        this container.

        @par Example
        @code
        url u = parse_relative_uri( "/path/to/the/file.txt" );

        segments se = u.segments();

        se.erase( se.begin() + 1, se.begin() + 3 );

        assert( u.encoded_path() == "/path/file.txt" );
        @endcode

        @return An iterator following
        the last element erased.

        @param first The beginning of the
        range to erase.

        @param last The end of the range
        to erase.

        @throw std::invalid_argument invalid percent-encoding
    */
    BOOST_URL_DECL
    iterator
    erase(
        iterator first,
        iterator last) noexcept;

    //--------------------------------------------

    /** Add an element to the end

        This function appends a segment
        containing the percent-encoded string
        `s` to the end of the container.
        The percent-encoding must be valid or
        else an exception is thrown.
        All @ref end iterators are invalidated.

        @par Example
        @code
        url u = parse_relative_uri( "/path/to" );

        u.segments().push_back( "file.txt" );

        assert( u.encoded_path() == "/path/to/file.txt" );
        @endcode

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.
        Exceptions thrown on invalid input.

        @param s The string to add

        @throw std::invalid_argument invalid percent-encoding
    */
    void
    push_back(
        string_view s);

    /** Remove the last element

        This function removes the last element
        from the container, which must not be
        empty or else undefined behavior occurs.
        Iterators and references to
        the last element, as well as the
        @ref end iterator, are invalidated.

        @par Preconditions
        @code
        not empty()
        @endcode

        @par Example
        @code
        url u = parse_relative_uri( "/path/to/file.txt" );

        u.segments().pop_back();

        assert( u.encoded_path() == "/path/to" );
        @endcode

        @par Exception Safety
        Throws nothing.
    */
    void
    pop_back() noexcept;
};

} // urls
} // boost

// VFALCO This include is at the bottom of
// url.hpp because of a circular dependency
//#include <boost/url/impl/segments.hpp>

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_SEGMENTS_ENCODED_HPP
#define BOOST_URL_SEGMENTS_ENCODED_HPP

#include <initializer_list>
#include <iterator>

namespace boost {
namespace urls {

#ifndef BOOST_URL_DOCS
class url;
#endif

/** A reference-like container to modifiable URL segments

    This class implements a <em>RandomAccessContainer</em>
    representing the path segments in a @ref url as
    percent-encoded strings. Ownership of the segments
    is not transferred; the container references the
    buffer in the url. Therefore, the lifetime of the
    url must remain valid until this container no
    longer exists.

    Objects of this type are not constructed directly;
    Instead, call the corresponding non-const member
    function of @ref url to obtain an instance of
    the container:

    @par Example
    @code
    url u = parse_relative_ref( "/path/to/file.txt" );

    segments_encoded se = u.encoded_segments();

    for( segments_encoded::value_type s : se )
        std::cout << s << std::endl;
    @endcode

    The @ref reference and @ref const_reference
    nested types are defined as publicly accessible
    nested classes. They proxy the behavior of a
    reference to a percent-encoded string in the
    underlying URL. The primary use of these
    references is to provide l-values that can be
    returned from element-accessing operations.
    Any reads or writes which happen through a
    @ref reference or @ref const_reference
    potentially read or write the underlying
    @ref url.

    @see
        @ref url.
*/
class segments_encoded
    : private detail::parts_base
{
    url* u_ = nullptr;

    friend class url;

    explicit
    segments_encoded(
        url& u) noexcept;

public:
    /** A read-only bidirectional iterator to an encoded segment.

        This is a read-only bidirectional iterator to
        the encoded segments.

     */
#ifdef BOOST_URL_DOCS
    using iterator = __see_below__;
#else
    class iterator;
#endif

    /// @copydoc iterator
    using const_iterator = iterator;

    /** A type which can represent a segment as a value

        This type allows for making a copy of
        a segment where ownership is retained
        in the copy.
    */
    using value_type = const_string;

    /** A type which can represent a segment as a const reference

        This type does not make a copy of a segment
        and ownership is retained by the container.
    */
    using reference = string_view;

    /// @copydoc reference
    using const_reference = string_view;

    /** An unsigned integer type
    */
    using size_type = std::size_t;

    /** A signed integer type
    */
    using difference_type = std::ptrdiff_t;

    //--------------------------------------------
    //
    // Members
    //
    //--------------------------------------------

    /** Returns true if this contains an absolute path.

        Absolute paths always start with a
        forward slash ('/').
    */
    bool
    is_absolute() const noexcept;

    /** Return this container as percent-decoded segments
    */
    template<class Allocator = std::allocator<char> >
    segments
    decoded(Allocator const& alloc = {}) const;

    /** Constructor

        Copy constructor
    */
    segments_encoded(segments_encoded const&) = default;

    /** Assignment

        After the assignment, both views will point to
        the same underlying object.
    */
    segments_encoded&
    operator=(segments_encoded const&) & = default;

    /** Replace the contents of the container

        This function replaces the contents
        with an initializer list  of
        percent-encoded strings.
        Each string must contain a valid
        percent-encoding or else an
        exception is thrown.
        The behavior is undefined any string
        refers to the contents of `*this`.
        All iterators and references to elements
        of the container are invalidated,
        including the @ref end iterator.

        @par Example
        @code
        url u = parse_relative_uri( "/path/to/file.txt" );
        u.encoded_segments() = { "etc", "init.rc" };
        assert( u.encoded_path() == "/etc/init.rc") );
        @endcode

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.
        Exceptions thrown on invalid input.

        @param init An initializer list of strings.

        @throw std::invalid_argument invalid percent-encoding
    */
    segments_encoded&
    operator=(std::initializer_list<string_view> init);

    /** Replace the contents of the container

        This function replaces the contents
        with a range of percent-encoded
        strings.
        Each string must contain a valid
        percent-encoding or else an
        exception is thrown.
        The behavior is undefined if either
        argument is an iterator into `*this`.
        All iterators and references to elements
        of the container are invalidated,
        including the @ref end iterator.

        @par Requires
        @code
        std::is_convertible< std::iterator_traits< FwdIt >::value_type, string_view >::value == true
        @endcode

        @par Example
        @code
        url u = parse_relative_uri( "/path/to/file.txt" );

        segments_encoded se = u.encoded_segments();

        std::vector< std::string > v = { "etc", "init.rc" };

        se.insert( u.end() - 1, v.begin(), v.end() );

        assert( u.encoded_path() == "/etc/init.rc") );
        @endcode

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.
        Exceptions thrown on invalid input.

        @param first An iterator to the first
        element in the range

        @param last An iterator to one past the
        last element in the range

        @throw std::invalid_argument invalid percent-encoding
    */
    template<class FwdIt>
#ifdef BOOST_URL_DOCS
    void
#else
    typename std::enable_if<
        std::is_convertible<typename
            std::iterator_traits<
                FwdIt>::value_type,
            string_view>::value>::type
#endif
    assign(FwdIt first, FwdIt last);

    //--------------------------------------------
    //
    // Element Access
    //
    //--------------------------------------------

    /** Return the first element
    */
    string_view
    front() const noexcept;

    /** Return the last element
    */
    const_reference
    back() const noexcept;

    //--------------------------------------------
    //
    // Iterators
    //
    //--------------------------------------------

    /** Return an iterator to the beginning
    */
    BOOST_URL_DECL
    iterator
    begin() const noexcept;

    /** Return an iterator to the end
    */
    BOOST_URL_DECL
    iterator
    end() const noexcept;

    //--------------------------------------------
    //
    // Capacity
    //
    //--------------------------------------------

    /** Return true if the container is empty

        This function returns true if there are
        no elements in the container. That is, if
        the underlying path is the empty string.
    */
    bool
    empty() const noexcept;

    /** Return the number of elements in the container

        This function returns the number of
        elements in the underlying path. Empty
        segments count towards this total.

        @par Exception Safety
        Throws nothing.
    */
    std::size_t
    size() const noexcept;

    //--------------------------------------------
    //
    // Modifiers
    //
    //--------------------------------------------

    /** Remove the contents of the container

        This function removes all the segments
        from the container, leaving the
        underlying URL with an empty path.

        @par Postconditions
        @code
        empty() == true
        @endcode

        @par Exception Safety
        Throws nothing.
    */
    void
    clear() noexcept;

    //--------------------------------------------

    /** Insert an element

        This function inserts a segment specified
        by the percent-encoded string `s`, at the
        position preceding `before`.
        The string must contain a valid
        percent-encoding, or else an exception
        is thrown.
        All references and iterators starting
        from the newly inserted element and
        up to and including the last element
        and @ref end iterators are invalidated.

        @note Behavior is undefined if the element
        belongs to the container

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.
        Exceptions thrown on invalid input.

        @return An iterator pointing to the
        inserted value.

        @param before An iterator before which the
        new element should be inserted.

        @param s A valid percent-encoded string
        to be inserted.

        @throw std::invalid_argument invalid percent-encoding
    */
    BOOST_URL_DECL
    iterator
    insert(
        iterator before,
        string_view s);

    /** Insert a range of segments

        This function inserts a range of
        percent-encoded strings passed as
        an initializer-list.
        Each string must contain a valid
        percent-encoding or else an exception
        is thrown.
        All references and iterators starting
        from the newly inserted elements and
        up to and including the last element
        and @ref end iterators are invalidated.

        @par Example
        @code
        url u = parse_relative_uri( "/path/file.txt" );

        segments_encoded se = u.encoded_segments();

        se.insert( u.end() - 1, { "to", "the" } );

        assert( u.encoded_path() == "/path/to/the/file.txt") );
        @endcode

        @note Behavior is undefined if any elements of the
        initializer_list belong to the container

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.
        Exceptions thrown on invalid input.

        @return An iterator to one past the last
        newly inserted element or `before` if
        the range is empty.

        @param before An iterator before which the
        new elements should be inserted.

        @param init The initializer list containing
        percent-encoded segments to insert.

        @throw std::invalid_argument invalid percent-encoding
    */
    iterator
    insert(
        iterator before,
        std::initializer_list<string_view> init);

    /** Insert a range of segments

        This function inserts a range
        of percent-encoded strings.
        Each string must contain a valid
        percent-encoding or else an
        exception is thrown.
        The behavior is undefined if either
        argument is an iterator into `this`.
        All references and iterators starting
        from the newly inserted elements and
        up to and including the last element
        and @ref end iterators are invalidated.

        @par Requires
        @code
        std::is_convertible< std::iterator_traits< FwdIt >::value_type, string_view >::value == true
        @endcode

        @par Example
        @code
        url u = parse_relative_uri( "/path/file.txt" );

        segments_encoded se = u.encoded_segments();

        std::vector< std::string > v = { "to", "the" };

        se.insert( u.end() - 1, v.begin(), v.end() );

        assert( u.encoded_path() == "/path/to/the/file.txt") );
        @endcode

        @note Behavior is undefined if any elements of the range
        belong to the container

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.
        Exceptions thrown on invalid input.

        @return An iterator to one past the last
        newly inserted element or `before` if
        the range is empty.

        @param before An iterator before which the
        new element should be inserted.

        @param first An iterator to the first
        element to insert.

        @param last An iterator to one past the
        last element to insert.

        @throw std::invalid_argument invalid percent-encoding
    */
    template<class FwdIt>
#ifdef BOOST_URL_DOCS
    iterator
#else
    typename std::enable_if<
        std::is_convertible<typename
            std::iterator_traits<
                FwdIt>::value_type,
            string_view>::value,
        iterator>::type
#endif
    insert(
        iterator before,
        FwdIt first,
        FwdIt last);

private:
    template<class FwdIt>
    iterator
    insert(
        iterator before,
        FwdIt first,
        FwdIt last,
        std::input_iterator_tag) = delete;

    template<class FwdIt>
    iterator
    insert(
        iterator before,
        FwdIt first,
        FwdIt last,
        std::forward_iterator_tag);
public:

    //--------------------------------------------

    /** Erase an element

        This function erases the element pointed
        to by `pos`, which must be a valid
        iterator for the container.
        All references and iterators starting
        from pos and up to and including
        the last element and @ref end iterators
        are invalidated.

        @par Preconditions
        `pos` points to a valid element in
        this container.

        @par Example
        @code
        url u = parse_relative_uri( "/path/to/file.txt" );

        segments_encoded se = u.encoded_segments();

        se.erase( se.begin() + 1 );

        assert( u.encoded_path() == "/path/file.txt" );
        @endcode

        @par Exception Safety
        Throws nothing.

        @return An iterator following
        the last element erased.

        @param pos An iterator to the
        element to erase.
    */
    iterator
    erase(
        iterator pos) noexcept;

    /** Erase a range of elements

        This function erases the elements
        in the range `[first, last)`, which
        must be a valid range in the container.
        All references and iterators starting
        from `first` and up to and including
        the last element and @ref end iterators
        are invalidated.

        @par Preconditions
        `[first, last)` is a valid range in
        this container.

        @par Example
        @code
        url u = parse_relative_uri( "/path/to/the/file.txt" );

        segments_encoded se = u.encoded_segments();

        se.erase( se.begin() + 1, se.begin() + 3 );

        assert( u.encoded_path() == "/path/file.txt" );
        @endcode

        @return An iterator following
        the last element erased.

        @param first The beginning of the
        range to erase.

        @param last The end of the range
        to erase.

        @throw std::invalid_argument invalid percent-encoding
    */
    BOOST_URL_DECL
    iterator
    erase(
        iterator first,
        iterator last) noexcept;

    //--------------------------------------------

    iterator
    replace(
        iterator pos,
        string_view s);

    iterator
    replace(
        iterator from,
        iterator to,
        std::initializer_list<
            string_view> init);

    template<class FwdIt>
#ifdef BOOST_URL_DOCS
    iterator
#else
    typename std::enable_if<
        std::is_convertible<typename
            std::iterator_traits<
                FwdIt>::value_type,
            string_view>::value,
        iterator>::type
#endif
    replace(
        iterator from,
        iterator to,
        FwdIt first,
        FwdIt last);

    //--------------------------------------------

    /** Add an element to the end

        This function appends a segment
        containing the percent-encoded string
        `s` to the end of the container.
        The percent-encoding must be valid or
        else an exception is thrown.
        All @ref end iterators are invalidated.

        @par Example
        @code
        url u = parse_relative_uri( "/path/to" );

        u.segments_encoded().push_back( "file.txt" );

        assert( u.encoded_path() == "/path/to/file.txt" );
        @endcode

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.
        Exceptions thrown on invalid input.

        @param s The string to add

        @throw std::invalid_argument invalid percent-encoding
    */
    void
    push_back(
        string_view s);

    /** Remove the last element

        This function removes the last element
        from the container, which must not be
        empty or else undefined behavior occurs.
        Iterators and references to
        the last element, as well as the
        @ref end iterator, are invalidated.

        @par Preconditions
        @code
        not empty()
        @endcode

        @par Example
        @code
        url u = parse_relative_uri( "/path/to/file.txt" );

        u.segments_encoded().pop_back();

        assert( u.encoded_path() == "/path/to" );
        @endcode

        @par Exception Safety
        Throws nothing.
    */
    void
    pop_back() noexcept;
};

} // urls
} // boost

// VFALCO This include is at the bottom of
// url.hpp because of a circular dependency
//#include <boost/url/impl/segments_encoded.hpp>

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_SEGMENTS_ENCODED_VIEW_HPP
#define BOOST_URL_SEGMENTS_ENCODED_VIEW_HPP

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_SEGMENTS_VIEW_HPP
#define BOOST_URL_SEGMENTS_VIEW_HPP

#include <cstddef>
#include <iosfwd>

namespace boost {
namespace urls {

#ifndef BOOST_URL_DECL
class url_view;
class segments_encoded_view;
#endif

/** A bidirectional range of read-only path segment strings with percent-decoding applied.

    @see
        @ref segments_encoded_view.
*/
class segments_view
{
    string_view s_;
    std::size_t n_ = 0;
    const_string::factory a_;

    friend class url_view;
    friend class segments_encoded_view;

    template<class Allocator>
    segments_view(
        string_view s,
        std::size_t n,
        Allocator const& a);

public:
    /** A read-only bidirectional iterator to a decoded segment.

        This is a read-only bidirectional iterator to
        the decoded segments.

     */
#ifdef BOOST_URL_DOCS
    using iterator = __see_below__;
#else
    class iterator;
#endif

    /// @copydoc iterator
    using const_iterator = iterator;

    /** A type which can represent a segment as a value

        This type allows for making a copy of
        a segment where ownership is retained
        in the copy.
    */
    using value_type = const_string;

    /// @copydoc value_type
    using reference = const_string;

    /// @copydoc value_type
    using const_reference = const_string;

    /** The unsigned integer type used to represent size.
    */
    using size_type = std::size_t;

    /** The signed integer type used to represent differences.
    */
    using difference_type = std::ptrdiff_t;

    //--------------------------------------------
    //
    // Members
    //
    //--------------------------------------------

    /** Constructor

        Default constructed views represent an
        empty path.
    */
    segments_view() noexcept;

    /** Constructor

        Copy constructor
    */
    segments_view(segments_view const& other) = default;
    /** Assignment

        After the assignment, both views will point to
        the same underlying object.
    */
    segments_view&
    operator=(segments_view const& other) & = default;


    /** Returns true if this contains an absolute path.

        Absolute paths always start with a
        forward slash ('/').
    */
    bool
    is_absolute() const noexcept;

    //--------------------------------------------
    //
    // Element Access
    //
    //--------------------------------------------

    /** Return the first element.
    */
    const_string
    front() const noexcept;

    /** Return the last element.
    */
    const_string
    back() const noexcept;

    //--------------------------------------------
    //
    // Iterators
    //
    //--------------------------------------------

    /** Return an iterator to the beginning.
    */
    BOOST_URL_DECL
    iterator
    begin() const noexcept;

    /** Return an iterator to the end.
    */
    BOOST_URL_DECL
    iterator
    end() const noexcept;

    //--------------------------------------------
    //
    // Capacity
    //
    //--------------------------------------------

    /** Return true if the range contains no elements
    */
    bool
    empty() const noexcept;

    /** Return the number of elements in the range
    */
    std::size_t
    size() const noexcept;

    //--------------------------------------------
    //
    // Friends
    //
    //--------------------------------------------

    BOOST_URL_DECL friend std::ostream&
        operator<<(std::ostream& os,
            segments_view const& pv);
};

//----------------------------------------------------------

/** Format the object to an output stream
*/
BOOST_URL_DECL
std::ostream&
operator<<(
    std::ostream& os,
    segments_view const& vw);

} // urls
} // boost

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_SEGMENTS_VIEW_HPP
#define BOOST_URL_IMPL_SEGMENTS_VIEW_HPP

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_SEGMENTS_ITERATOR_IMPL_HPP
#define BOOST_URL_DETAIL_SEGMENTS_ITERATOR_IMPL_HPP

#include <string>

namespace boost {
namespace urls {
namespace detail {

struct segments_iterator_impl
{
    std::size_t i_ = 0;
    char const* begin_ = nullptr;
    char const* pos_ = nullptr;
    char const* next_ = nullptr;
    char const* end_ = nullptr;
    const_string::factory a_;
    pct_encoded_str t_;

    BOOST_URL_DECL
    segments_iterator_impl(
        string_view s,
        std::size_t nseg,
        const_string::factory const& a) noexcept;

    // end ctor
    BOOST_URL_DECL
    segments_iterator_impl(
        string_view s,
        std::size_t nseg,
        const_string::factory const& a,
        int) noexcept;

    segments_iterator_impl() = default;

    segments_iterator_impl(
        segments_iterator_impl const&) noexcept = default;

    segments_iterator_impl& operator=(
        segments_iterator_impl const&) noexcept = default;

    BOOST_URL_DECL
    const_string
    dereference() const noexcept;

    BOOST_URL_DECL
    void
    increment() noexcept;

    BOOST_URL_DECL
    void
    decrement() noexcept;

    bool
    equal(
        segments_iterator_impl const& other) const noexcept
    {
        return
            next_ == other.next_ &&
            end_ == other.end_;
    }
};

} // detail
} // urls
} // boost

#endif
#include <cstddef>

namespace boost {
namespace urls {

class segments_view::
    iterator
{
    detail::segments_iterator_impl impl_;

    friend segments_view;

    iterator(
        string_view s,
        std::size_t nseg,
        const_string::factory const& a) noexcept
        : impl_(s, nseg, a)
    {
    }

    // end ctor
    iterator(
        string_view s,
        std::size_t nseg,
        const_string::factory const& a,
        int) noexcept
        : impl_(s, nseg, a, 0)
    {
    }

public:
    using value_type = const_string;
    using reference = const_string;
    using pointer = void const*;
    using difference_type = std::ptrdiff_t;
    using iterator_category =
        std::bidirectional_iterator_tag;

    iterator() noexcept = default;

    iterator(iterator const&) noexcept = default;

    iterator&
    operator=(iterator const&) noexcept = default;

    const_string
    operator*() const noexcept
    {
        return impl_.dereference();
    }

    iterator&
    operator++() noexcept
    {
        impl_.increment();
        return *this;
    }

    iterator&
    operator--() noexcept
    {
        impl_.decrement();
        return *this;
    }

    iterator
    operator++(int) noexcept
    {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    iterator
    operator--(int) noexcept
    {
        auto tmp = *this;
        --*this;
        return tmp;
    }

    bool
    operator==(
        iterator const& other) const noexcept
    {
        return impl_.equal(other.impl_);
    }

    bool
    operator!=(
        iterator const& other) const noexcept
    {
        return !impl_.equal(other.impl_);
    }
};

//------------------------------------------------
//
// Members
//
//------------------------------------------------

template<class Allocator>
segments_view::
segments_view(
    string_view s,
    std::size_t n,
    Allocator const& a)
    : s_(s)
    , n_(n)
    , a_(a)
{
}

inline
segments_view::
segments_view() noexcept = default;

inline
bool
segments_view::
is_absolute() const noexcept
{
    return s_.starts_with('/');
}

//------------------------------------------------
//
// Element Access
//
//------------------------------------------------

inline
const_string
segments_view::
front() const noexcept
{
    BOOST_ASSERT(! empty());
    return *begin();
}

inline
const_string
segments_view::
back() const noexcept
{
    BOOST_ASSERT(! empty());
    return *--end();
}

//------------------------------------------------
//
// Capacity
//
//------------------------------------------------

inline
bool
segments_view::
empty() const noexcept
{
    return size() == 0;
}

inline
std::size_t
segments_view::
size() const noexcept
{
    return n_;
}

} // urls
} // boost

#endif

#endif
#include <iosfwd>
#include <utility>

namespace boost {
namespace urls {

#ifndef BOOST_URL_DECL
class url_view;
#endif

/** A bidirectional range of read-only encoded path segment strings.

    Objects of this type represent an iterable
    range of path segments, where each segment
    is represented by a percent-encoded string.

    Dereferenced iterators return string views
    into the underlying character buffer.

    Ownership of the underlying characters is
    not transferred; the character buffer used
    to construct the container must remain
    valid for as long as the container exists.

    A view of encoded segments in a URL's path
    can be obtained by calling
        @ref url_view::encoded_segments.
    Alternatively, to obtain encoded segments
    from a path stored in a string call one of
    the parsing functions (see below).

    @par Examples

    A path string is parsed into encoded
    segments, then each segment is printed to
    standard output:

    @code
    segments_encoded_view sev = parse_path( "/path/to/file.txt" ).value();

    for( auto it = sev.begin(); it != sev.end(); ++it )
        std::cout << *it << std::endl;
    @endcode

    A URL containing a path is parsed, then a
    view to the encoded segments is obtained
    and formatted to standard output:

    @code
    url_view u = parse_uri( "http://example.com/path/to/file.txt" ).value();

    segments_encoded_view sev = u.encoded_segments();

    std::cout << sev << std::endl;
    @endcode

    @par Complexity

    Iterator increment or decrement runs in
    linear time on the size of the segment.
    All other operations run in constant time.
    No operations allocate memory.

    @see
        @ref parse_path,
        @ref parse_path_abempty,
        @ref parse_path_absolute,
        @ref parse_path_noscheme,
        @ref parse_path_rootless,
        @ref segments_view.
*/
class segments_encoded_view
{
    string_view s_;
    std::size_t n_;

    friend class url_view;

    BOOST_URL_DECL
    segments_encoded_view(
        string_view s,
        std::size_t n) noexcept;

public:
    /** A read-only bidirectional iterator to an encoded segment.

        This is a read-only bidirectional iterator to
        the encoded segments.

     */
#ifdef BOOST_URL_DOCS
    using iterator = __see_below__;
#else
    class iterator;
#endif

    /// @copydoc iterator
    using const_iterator = iterator;

    /** A type which can represent a segment as a value

        This type allows for making a copy of
        a segment where ownership is retained
        in the copy.

    */
    using value_type = const_string;

    /** A type which can represent a segment as a const reference

        This type does not make a copy of a segment
        and ownership is retained by the container.

    */
    using reference = string_view;

    /// @copydoc reference
    using const_reference = string_view;

    /** An unsigned integer type used to represent size.
    */
    using size_type = std::size_t;

    /** A signed integer type used to represent differences.
    */
    using difference_type = std::ptrdiff_t;

    //--------------------------------------------
    //
    // Members
    //
    //--------------------------------------------

    /** Constructor

        A default-constructed instance will be
        an empty range.
    */
    segments_encoded_view() noexcept;

    /** Constructor

        Copy constructor
    */
    segments_encoded_view(segments_encoded_view const&) noexcept = default;

    /** Assignment

        After the assignment, both views will point to
        the same underlying object.
    */
    segments_encoded_view&
    operator=(segments_encoded_view const&) & = default;

    /** Return a view of this container as percent-decoded segments

        This function returns a new view over the
        same underlying character buffer where each
        segment is returned as a @ref const_string
        with percent-decoding applied using the
        optionally specified allocator.

        The decoded view does not take ownership of
        the underlying character buffer; the caller
        is still responsible for ensuring that the
        buffer remains valid until all views which
        reference it are destroyed.

        @par Example
        @code
        segments_encoded_view sev = parse_path( "/%70%61%74%68/%74%6f/%66%69%6c%65%2e%74%78%74" ).value();

        segments_view sv = sev.decoded();

        std::stringstream ss;

        ss << sv.front() << "/../" << sv.back();

        assert( ss.string() == "path/../file.txt" );
        @endcode

        @par Exceptions
        Calls to allocate may throw.

        @return A view to decoded path segments.

        @param alloc The allocator the returned
        view will use for all string storage. If
        this parameter is ommitted, the default
        allocator will be used.
    */
    template<class Allocator = std::allocator<char> >
    segments_view
    decoded(Allocator const& alloc = {}) const;

    /** Returns true if this contains an absolute path.

        Absolute paths always start with a
        forward slash ('/').
    */
    bool
    is_absolute() const noexcept;

    //--------------------------------------------
    //
    // Element Access
    //
    //--------------------------------------------

    /** Return the first element.
    */
    string_view
    front() const noexcept;

    /** Return the last element.
    */
    string_view
    back() const noexcept;

    //--------------------------------------------
    //
    // Iterators
    //
    //--------------------------------------------

    /** Return an iterator to the beginning.
    */
    BOOST_URL_DECL
    iterator
    begin() const noexcept;

    /** Return an iterator to the end.
    */
    BOOST_URL_DECL
    iterator
    end() const noexcept;

    //--------------------------------------------
    //
    // Capacity
    //
    //--------------------------------------------

    /** Return true if the range contains no elements
    */
    bool
    empty() const noexcept;

    /** Return the number of elements in the range
    */
    std::size_t
    size() const noexcept;

    //--------------------------------------------

    BOOST_URL_DECL friend std::ostream&
        operator<<(std::ostream& os,
            segments_encoded_view const& pv);

    BOOST_URL_DECL friend
        result<segments_encoded_view>
        parse_path(string_view s) noexcept;

    BOOST_URL_DECL friend
        result<segments_encoded_view>
        parse_path_abempty(string_view s) noexcept;

    BOOST_URL_DECL friend
        result<segments_encoded_view>
        parse_path_absolute(string_view s) noexcept;

    BOOST_URL_DECL friend
        result<segments_encoded_view>
        parse_path_noscheme(string_view s) noexcept;

    BOOST_URL_DECL friend
        result<segments_encoded_view>
        parse_path_rootless(string_view s) noexcept;
};

//----------------------------------------------------------

/** Format the object to an output stream
*/
BOOST_URL_DECL
std::ostream&
operator<<(
    std::ostream& os,
    segments_encoded_view const& vw);

//----------------------------------------------------------

/** Parse a string and return an encoded segment view

    This function parses the string and returns the
    corresponding path object if the string is valid,
    otherwise returns an error.

    @par BNF
    @code
    path          = [ "/" ] segment *( "/" segment )
    @endcode

    @par Exception Safety
    No-throw guarantee.

    @return A valid view on success, otherwise an
    error code.

    @param s The string to parse

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.3">
        3.3. Path (rfc3986)</a>

    @see
        @ref parse_path,
        @ref parse_path_absolute,
        @ref parse_path_noscheme,
        @ref parse_path_rootless.
*/
BOOST_URL_DECL
result<segments_encoded_view>
parse_path(string_view s) noexcept;

/** Parse a string and return an encoded segment view

    This function parses the string and returns the
    corresponding path object if the string is valid,
    otherwise sets the error and returns an empty range.

    @par BNF
    @code
    path-abempty  = *( "/" segment )
    @endcode

    @par Exception Safety
    No-throw guarantee.

    @return A valid view on success, otherwise an
    error code.

    @param s The string to parse

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.3">
        3.3. Path (rfc3986)</a>

    @see
        @ref parse_path,
        @ref parse_path_absolute,
        @ref parse_path_noscheme,
        @ref parse_path_rootless.
*/
BOOST_URL_DECL
result<segments_encoded_view>
parse_path_abempty(
    string_view s) noexcept;

/** Parse a string and return an encoded segment view

    This function parses the string and returns the
    corresponding path object if the string is valid,
    otherwise sets the error and returns an empty range.

    @par BNF
    @code
    path-absolute = "/" [ segment-nz *( "/" segment ) ]
    @endcode

    @par Exception Safety
    No-throw guarantee.

    @return A valid view on success, otherwise an
    error code.

    @param s The string to parse

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.3">
        3.3. Path (rfc3986)</a>

    @see
        @ref parse_path,
        @ref parse_path_abempty,
        @ref parse_path_noscheme,
        @ref parse_path_rootless.
*/
BOOST_URL_DECL
result<segments_encoded_view>
parse_path_absolute(
    string_view s) noexcept;

/** Parse a string and return an encoded segment view

    This function parses the string and returns the
    corresponding path object if the string is valid,
    otherwise sets the error and returns an empty range.

    @par BNF
    @code
    path-noscheme = segment-nz-nc *( "/" segment )

    segment-nz-nc = 1*( unreserved / pct-encoded / sub-delims / "@" )
                    ; non-zero-length segment without any colon ":"
    @endcode

    @par Exception Safety
    No-throw guarantee.

    @return A valid view on success, otherwise an
    error code.

    @param s The string to parse

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.3">
        3.3. Path (rfc3986)</a>

    @see
        @ref parse_path,
        @ref parse_path_abempty,
        @ref parse_path_absolute,
        @ref parse_path_rootless.
*/
BOOST_URL_DECL
result<segments_encoded_view>
parse_path_noscheme(
    string_view s) noexcept;

/** Parse a string and return an encoded segment view

    This function parses the string and returns the
    corresponding path object if the string is valid,
    otherwise sets the error and returns an empty range.

    @par BNF
    @code
    path-rootless = segment-nz *( "/" segment )

    segment-nz    = 1*pchar
    @endcode

    @par Exception Safety
    No-throw guarantee.

    @return A valid view on success, otherwise an
    error code.

    @param s The string to parse

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.3">
        3.3. Path (rfc3986)</a>

    @see
        @ref parse_path,
        @ref parse_path_abempty,
        @ref parse_path_absolute,
        @ref parse_path_noscheme.
*/
BOOST_URL_DECL
result<segments_encoded_view>
parse_path_rootless(
    string_view s) noexcept;

} // urls
} // boost

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_SEGMENTS_ENCODED_VIEW_HPP
#define BOOST_URL_IMPL_SEGMENTS_ENCODED_VIEW_HPP

//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
// Copyright (c) 2022 Alan Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_PCT_ENCODING_HPP
#define BOOST_URL_DETAIL_PCT_ENCODING_HPP

#include <boost/assert.hpp>
#include <memory>

namespace boost {
namespace urls {
namespace detail {

BOOST_URL_DECL
bool
key_equal_encoded(
    string_view plain_key,
    pct_encoded_str encoded) noexcept;

BOOST_URL_DECL
bool
key_equal_encoded(
    string_view plain_key,
    string_view encoded) noexcept;

} // detail
} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_SEGMENTS_ENCODED_ITERATOR_IMPL_HPP
#define BOOST_URL_DETAIL_SEGMENTS_ENCODED_ITERATOR_IMPL_HPP

#include <string>

namespace boost {
namespace urls {
namespace detail {

struct segments_encoded_iterator_impl
{
    std::size_t i_ = 0;
    string_view s_;
    char const* begin_ = nullptr;
    char const* pos_ = nullptr;
    char const* next_ = nullptr;
    char const* end_ = nullptr;

    BOOST_URL_DECL
    segments_encoded_iterator_impl(
        string_view s,
        std::size_t nseg) noexcept;

    // end ctor
    BOOST_URL_DECL
    segments_encoded_iterator_impl(
        string_view s,
        std::size_t nseg,
        int) noexcept;

    segments_encoded_iterator_impl() = default;

    segments_encoded_iterator_impl(
        segments_encoded_iterator_impl const&) noexcept = default;

    segments_encoded_iterator_impl& operator=(
        segments_encoded_iterator_impl const&) noexcept = default;

    BOOST_URL_DECL
    void
    increment() noexcept;

    BOOST_URL_DECL
    void
    decrement() noexcept;

    bool
    equal(
        segments_encoded_iterator_impl const& other) const noexcept
    {
        return
            next_ == other.next_ &&
            end_ == other.end_;
    }
};

} // detail
} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_PATHS_RULE_HPP
#define BOOST_URL_RFC_PATHS_RULE_HPP

#include <cstddef>

namespace boost {
namespace urls {

/** Information about a parsed path
*/
struct parsed_path
{
    /** The encoded string representing the path
    */
    string_view path;

    /** The number of segments in the path
    */
    std::size_t count = 0;
};

//------------------------------------------------

/** Rule for segment

    @par BNF
    @code
    segment       = *pchar
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.3"
        >3.3. Path (rfc3986)</a>

    @see
        @ref path_abempty_rule,
        @ref path_absolute_rule,
        @ref path_noscheme_rule,
        @ref path_rootless_rule,
        @ref pchars,
        @ref segment_nz_rule,
        @ref segment_nz_nc_rule
*/
#ifdef BOOST_URL_DOCS
using segment_rule = __see_below__;
#else
struct segment_rule
{
    pct_encoded_str& v;

    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* const end,
        error_code& ec,
        segment_rule const& t) noexcept
    {
        parse(it, end, ec, t);
    }

private:
    BOOST_URL_DECL
    static
    void
    parse(
        char const*& it,
        char const* const end,
        error_code& ec,
        segment_rule const& t) noexcept;

};
#endif

//------------------------------------------------

/** Rule for segment-nz

    @par BNF
    @code
    segment-nz    = 1*pchar
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.3"
        >3.3. Path (rfc3986)</a>

    @see
        @ref path_abempty_rule,
        @ref path_absolute_rule,
        @ref path_noscheme_rule,
        @ref path_rootless_rule,
        @ref pchars,
        @ref segment_rule,
        @ref segment_nz_nc_rule
*/
#ifdef BOOST_URL_DOCS
using segment_nz_rule = __see_below__;
#else
struct segment_nz_rule
{
    pct_encoded_str& v;

    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* const end,
        error_code& ec,
        segment_nz_rule const& t) noexcept
    {
        parse(it, end, ec, t);
    }

private:
    BOOST_URL_DECL
    static
    void
    parse(
        char const*& it,
        char const* const end,
        error_code& ec,
        segment_nz_rule const& t) noexcept;
};
#endif

//------------------------------------------------

/** Rule for segment_nz_nc

    @par BNF
    @code
    segment-nz-nc   = 1*( unreserved / pct-encoded / sub-delims / "@" )
                    ; non-zero-length segment without any colon ":"
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.3"
        >3.3. Path (rfc3986)</a>

    @see
        @ref path_abempty_rule,
        @ref path_absolute_rule,
        @ref path_noscheme_rule,
        @ref path_rootless_rule,
        @ref pchars,
        @ref segment_rule,
        @ref segment_nz_rule,
        @ref subdelim_chars,
        @ref unreserved_chars
*/
#ifdef BOOST_URL_DOCS
using segment_nz_nc_rule = __see_below__;
#else
struct segment_nz_nc_rule
{
    pct_encoded_str& v;

    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* const end,
        error_code& ec,
        segment_nz_nc_rule const& t) noexcept
    {
        parse(it, end, ec, t);
    }

private:
    BOOST_URL_DECL
    static
    void
    parse(
        char const*& it,
        char const* const end,
        error_code& ec,
        segment_nz_nc_rule const& t) noexcept;


};
#endif

//------------------------------------------------

/** Rule for path-abempty grammar

    @par BNF
    @code
    path-abempty  = *( "/" segment )
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.3"
        >3.3. Path (rfc3986)</a>

    @see
        @ref path_absolute_rule,
        @ref path_noscheme_rule,
        @ref path_rootless_rule,
        @ref segment_rule
*/
#ifdef BOOST_URL_DOCS
using path_abempty_rule = __see_below__;
#else
struct path_abempty_rule : grammar::range_
{
    using value_type =
        pct_encoded_str;

    path_abempty_rule()
        : grammar::range_(this)
    {
    }

    BOOST_URL_DECL
    static
    bool
    begin(
        char const*& it,
        char const* end,
        error_code& ec,
        pct_encoded_str& t) noexcept;

    BOOST_URL_DECL
    static
    bool
    increment(
        char const*& it,
        char const* end,
        error_code& ec,
        pct_encoded_str& t) noexcept;
};
#endif

//------------------------------------------------

/** Rule for path-absolute grammar.

    @par BNF
    @code
    path-absolute = "/" [ segment-nz *( "/" segment ) ]
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.3"
        >3.3. Path (rfc3986)</a>

    @see
        @ref path_abempty_rule,
        @ref path_noscheme_rule,
        @ref path_rootless_rule,
        @ref segment_rule,
        @ref segment_nz_rule
*/
#ifdef BOOST_URL_DOCS
using path_absolute_rule = __see_below__;
#else
struct path_absolute_rule : grammar::range_
{
    using value_type =
        pct_encoded_str;

    path_absolute_rule()
        : grammar::range_(this)
    {
    }

    BOOST_URL_DECL
    static
    bool
    begin(
        char const*& it,
        char const* const end,
        error_code& ec,
        pct_encoded_str& t) noexcept;

    BOOST_URL_DECL
    static
    bool
    increment(
        char const*& it,
        char const* const end,
        error_code& ec,
        pct_encoded_str& t) noexcept;
};
#endif

//------------------------------------------------

/** Rule for path-noscheme grammar.

    @par BNF
    @code
    path-noscheme = segment-nz-nc *( "/" segment )
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.3"
        >3.3. Path (rfc3986)</a>

    @see
        @ref path_abempty_rule,
        @ref path_absolute_rule,
        @ref path_rootless_rule,
        @ref segment_rule,
        @ref segment_nz_nc_rule
*/
#ifdef BOOST_URL_DOCS
using path_noscheme_rule = __see_below__;
#else
struct path_noscheme_rule : grammar::range_
{
    using value_type =
        pct_encoded_str;

    path_noscheme_rule()
        : grammar::range_(this)
    {
    }

    BOOST_URL_DECL
    static
    bool
    begin(
        char const*& it,
        char const* const end,
        error_code& ec,
        pct_encoded_str& t) noexcept;

    BOOST_URL_DECL
    static
    bool
    increment(
        char const*& it,
        char const* const end,
        error_code& ec,
        pct_encoded_str& t) noexcept;
};
#endif

//------------------------------------------------

/** Rule for path-rootless grammar.

    @par Example
    @code
    path_rootless_rule t;
    bool success = parse( it, end, ec, t);
    @endcode

    @par BNF
    @code
    path-rootless = segment-nz *( "/" segment )
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.3"
        >3.3. Path (rfc3986)</a>

    @see
        @ref path_abempty_rule,
        @ref path_absolute_rule,
        @ref path_noscheme_rule,
        @ref segment_rule,
        @ref segment_nz_nc_rule
*/
#ifdef BOOST_URL_DOCS
using path_rootless_rule = __see_below__;
#else
struct path_rootless_rule : grammar::range_
{
    using value_type =
        pct_encoded_str;

    path_rootless_rule()
        : grammar::range_(this)
    {
    }

    BOOST_URL_DECL
    static
    bool
    begin(
        char const*& it,
        char const* const end,
        error_code& ec,
        pct_encoded_str& t) noexcept;

    BOOST_URL_DECL
    static
    bool
    increment(
        char const*& it,
        char const* const end,
        error_code& ec,
        pct_encoded_str& t) noexcept;
};
#endif

} // urls
} // boost

#endif
#include <cstdint>
#include <iterator>

namespace boost {
namespace urls {

class segments_encoded_view::iterator
{
    friend segments_encoded_view;

    detail::segments_encoded_iterator_impl impl_;

    iterator(
        string_view s,
        std::size_t nseg) noexcept
        : impl_(s, nseg)
    {
    }

    // end ctor
    iterator(
        string_view s,
        std::size_t nseg,
        int) noexcept
        : impl_(s, nseg, 0)
    {
    }

public:
    using value_type = const_string;
    using reference = string_view;
    using pointer = void const*;
    using difference_type = std::ptrdiff_t;
    using iterator_category =
        std::bidirectional_iterator_tag;

    iterator() = default;

    iterator(iterator const&) noexcept = default;

    iterator& operator=(iterator const&) noexcept = default;

    reference
    operator*() const noexcept
    {
        return impl_.s_;
    }

    iterator&
    operator++() noexcept
    {
        impl_.increment();
        return *this;
    }

    iterator&
    operator--() noexcept
    {
        impl_.decrement();
        return *this;
    }

    iterator
    operator++(int) noexcept
    {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    iterator
    operator--(int) noexcept
    {
        auto tmp = *this;
        --*this;
        return tmp;
    }

    bool
    operator==(
        iterator const& other) const noexcept
    {
        return impl_.equal(other.impl_);
    }

    bool
    operator!=(
        iterator const& other) const noexcept
    {
        return !impl_.equal(other.impl_);
    }
};

//------------------------------------------------
//
// Members
//
//------------------------------------------------

inline
segments_encoded_view::
segments_encoded_view() noexcept
    : s_("")
    , n_(0)
{
}

template<class Allocator>
segments_view
segments_encoded_view::
decoded(Allocator const& alloc) const
{
    return segments_view(s_, n_, alloc);
}

inline
bool
segments_encoded_view::
is_absolute() const noexcept
{
    return s_.starts_with('/');
}

//------------------------------------------------
//
// Element Access
//
//------------------------------------------------

inline
string_view
segments_encoded_view::
front() const noexcept
{
    BOOST_ASSERT(! empty());
    return *begin();
}

inline
string_view
segments_encoded_view::
back() const noexcept
{
    BOOST_ASSERT(! empty());
    return *--end();
}

//------------------------------------------------
//
// Capacity
//
//------------------------------------------------

inline
bool
segments_encoded_view::
empty() const noexcept
{
    return size() == 0;
}

inline
std::size_t
segments_encoded_view::
size() const noexcept
{
    return n_;
}

} // urls
} // boost

#endif

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_STATIC_POOL_HPP
#define BOOST_URL_STATIC_POOL_HPP

#include <cstdlib>
#include <string>
#include <type_traits>

namespace boost {
namespace urls {

/** The type of allocator returned by static_pool
*/
#ifdef BOOST_URL_DOCS
template<class T>
using static_pool_allocator = __see_below__
#else
template<class T>
class static_pool_allocator;
#endif

/** Base class for fixed-storage pool
*/
class basic_static_pool
{
    char* begin_;
    char* end_;
    char* top_;
    std::size_t n_ = 0;

    template<class T>
    friend class static_pool_allocator;

    void** table() noexcept;
    void** find(void* p) noexcept;

    BOOST_URL_DECL
    void*
    allocate(
        std::size_t bytes,
        std::size_t align);

    BOOST_URL_DECL
    void
    deallocate(
        void* p,
        std::size_t bytes,
        std::size_t align) noexcept;

public:
    /** Constructor (deleted)
    */
    basic_static_pool(
        basic_static_pool const&) = delete;

    /** Assignment (deleted)
    */
    basic_static_pool&
        operator=(basic_static_pool const&) = delete;

    /** The type of strings using the pool's allocator.
    */
    using string_type = std::basic_string<
        char, std::char_traits<char>,
            static_pool_allocator<char>>;

    /** Destructor
    */
    BOOST_URL_DECL
    ~basic_static_pool();

    /** Constructor
    */
    basic_static_pool(
        char* buffer,
        std::size_t size)
        : begin_(buffer)
        , end_(buffer + size)
        , top_(end_)
    {
    }

    /** Return an allocator which uses the pool.
    */
    static_pool_allocator<char>
    allocator() noexcept;

    /** Construct a std::basic_string using the pool as its allocator.
    */
    template<class... Args>
    string_type
    make_string(Args&&... args);
};

//------------------------------------------------

/** A fixed-size storage pool for allocating memory
*/
template<std::size_t N>
class static_pool : public basic_static_pool
{
    char buf_[N];

public:
    /** Constructor
    */
    static_pool() noexcept
        : basic_static_pool(buf_, N)
    {
    }
};

} // urls
} // boost

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_STATIC_POOL_HPP
#define BOOST_URL_IMPL_STATIC_POOL_HPP

namespace boost {
namespace urls {

template<class T>
class static_pool_allocator
{
    basic_static_pool* pool_ = nullptr;

    template<class U>
    friend class static_pool_allocator;

public:
    using pointer = T*;
    using reference = T&;
    using value_type = T;
    using size_type = std::size_t;
    using const_pointer = T const*;
    using const_reference = T const&;
    using difference_type = std::ptrdiff_t;
    using is_always_equal = std::false_type;

    template<class U>
    struct rebind
    {
        using other = static_pool_allocator<U>;
    };

#ifndef BOOST_URL_NO_GCC_4_2_WORKAROUND
    // libg++ basic_string requires Allocator to be DefaultConstructible
    // https://code.woboq.org/firebird/include/c++/4.8.2/bits/basic_string.tcc.html#82
    static_pool_allocator() = default;
#endif

    template<class U>
    static_pool_allocator(
        static_pool_allocator<
            U> const& other) noexcept
        : pool_(other.pool_)
    {
    }

    explicit
    static_pool_allocator(
        basic_static_pool& pool)
        : pool_(&pool)
    {
    }

    pointer
    allocate(size_type n)
    {
        return reinterpret_cast<T*>(
            pool_->allocate(
                n * sizeof(T),
                    alignof(T)));
    }

    void
    deallocate(
        pointer p,
        size_type n) noexcept
    {
        pool_->deallocate(p,
            n * sizeof(T), alignof(T));
    }

    template<class U>
    bool
    operator==(static_pool_allocator<
        U> const& other) const noexcept
    {
        return pool_ == other.pool_;
    }

    template<class U>
    bool
    operator!=(static_pool_allocator<
        U> const& other) const noexcept
    {
        return pool_ != other.pool_;
    }
};

//------------------------------------------------

inline
auto
basic_static_pool::
allocator() noexcept ->
    static_pool_allocator<char>
{
    return static_pool_allocator<
        char>(*this);
}

template<class... Args>
auto
basic_static_pool::
make_string(Args&&... args) ->
    string_type
{
    return std::basic_string<
        char, std::char_traits<char>,
            static_pool_allocator<char>>(
        std::forward<Args>(args)...,
            allocator());
}

} // urls
} // boost

#endif

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_STATIC_URL_HPP
#define BOOST_URL_STATIC_URL_HPP

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_URL_HPP
#define BOOST_URL_URL_HPP

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_URL_VIEW_HPP
#define BOOST_URL_URL_VIEW_HPP

#include <boost/assert.hpp>
#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <memory>
#include <utility>

namespace boost {
namespace urls {

#ifndef BOOST_URL_DOCS
struct authority_rule;
struct fragment_part_rule;
struct host_rule;
struct parsed_path;
struct query_part_rule;
struct scheme_part_rule;
#endif

/** A read-only view to a URL

    Objects of this type represent valid URL
    strings whose storage is managed externally.
    That is, it acts like a `std::string_view` in
    terms of ownership. Callers are responsible
    for ensuring that the lifetime of the
    underlying string extends until the view
    is no longer in use.
    The constructor parses using the
    <em>URI-reference</em> grammar and throws
    an exception on error.
    The parsing free functions offer different
    choices of grammar and can indicate failure
    using an error code.

    @par Example
    @code
    url_view u( "http://www.example.com/index.html" );

    // Reassign, throw on error:
    u = parse_relative_ref( "/path/to/file.txt" ).value();

    result< url_view > r = parse_absolute_uri(
        "magnet:?xt=urn:btih:c12fe1c06bba254a9dc9f519b335aa7c1367a88a" );
    if( r.has_value() )
        std::cout << r.value();
    else
        std::cout << r.error().message();
    @endcode

    @par BNF
    @code
    URI-reference = URI / relative-ref

    URI           = scheme ":" hier-part [ "?" query ] [ "#" fragment ]

    relative-ref  = relative-part [ "?" query ] [ "#" fragment ]

    absolute-URI  = scheme ":" hier-part [ "?" query ]
    @endcode

    @par Specification
    @li <a href="https://tools.ietf.org/html/rfc3986"
        >Uniform Resource Identifier (URI): Generic Syntax (rfc3986)</a>

    @see
        @ref parse_absolute_uri,
        @ref parse_relative_ref,
        @ref parse_uri,
        @ref parse_uri_reference.
*/
class BOOST_SYMBOL_VISIBLE url_view
    : protected detail::parts_base
{
#ifndef BOOST_URL_DOCS
protected:
#endif
    char const* cs_ = empty_;
    pos_t offset_[id_end + 1] = {};
    pos_t decoded_[id_end] = {};
    pos_t nseg_ = 0;
    pos_t nparam_ = 0;
    unsigned char ip_addr_[16] = {};
    // VFALCO don't we need a bool?
    std::uint16_t port_number_ = 0;
    urls::host_type host_type_ =
        urls::host_type::none;
    urls::scheme scheme_ =
        urls::scheme::none;

    friend class url;
    friend class static_url_base;
    friend struct std::hash<url_view>;
    struct shared_impl;

    url_view& base() noexcept;
    url_view const& base() const noexcept;
    std::size_t table_bytes() const noexcept;
    pos_t len(int first, int last) const noexcept;
    void set_size(int id, pos_t n) noexcept;
    void split(int id, std::size_t n) noexcept;
    void adjust(int first, int last,
        std::size_t n) noexcept;
    void collapse(int first, int last,
        std::size_t n) noexcept;

    BOOST_URL_DECL
    url_view(int, char const* cs) noexcept;

    BOOST_URL_DECL
    url_view(
        url_view const& u,
        char const* cs) noexcept;

    // return offset of id
    auto
    offset(int id) const noexcept ->
        pos_t
    {
        return
            id == id_scheme ?
            zero_ : offset_[id];
    }

    // return length of part
    auto
    len(int id) const noexcept ->
        pos_t
    {
        return id == id_end ? 0 : (
            offset(id + 1) -
            offset(id) );
    }

    // return id as string
    string_view
    get(int id) const noexcept
    {
        return {
            cs_ + offset(id), len(id) };
    }

    // return [first, last) as string
    string_view
    get(int first,
        int last) const noexcept
    {
        return { cs_ + offset(first),
            offset(last) - offset(first) };
    }

public:
    /** The type of elements.
    */
    using value_type = char;

    /** The type of pointers to elements.
    */
    using pointer = char const*;

    /** The type of const pointers to elements.
    */
    using const_pointer = char const*;

    /** The type of const iterator to elements.
    */
    using const_iterator = char const*;

    /** The type of iterator to elements.
    */
    using iterator = char const*;

    /** An unsigned integer type to represent sizes.
    */
    using size_type = std::size_t;

    //--------------------------------------------
    //
    // Special Members
    //
    //--------------------------------------------

    /** Destructor

        Any params, segments, or iterators
        which reference this object are
        invalidated. The ownership and lifetime
        of the underlying character buffer
        remains unchanged.
    */
    BOOST_URL_DECL
    virtual ~url_view();

    /** Constructor

        Default constructed views refer to
        a string with zero length, which
        always remains valid. This matches
        the grammar for a relative-ref with
        an empty path and no query or
        fragment.

        @par BNF
        @code
        relative-ref  = relative-part [ "?" query ] [ "#" fragment ]
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-4.2"
            >4.2. Relative Reference (rfc3986)</a>
    */
    BOOST_URL_DECL
    url_view() noexcept;

    /** Constructor
    */
    BOOST_URL_DECL
    url_view(url_view const&) noexcept;

    /** Assignment
    */
    BOOST_URL_DECL
    url_view&
    operator=(url_view const&) noexcept;

    /** Construct from a string.

        This function constructs a URL from
        the string `s`, which must contain a
        valid URI or <em>relative-ref</em> or
        else an exception is thrown. Upon
        successful construction, the view
        refers to the characters in the
        buffer pointed to by `s`.
        Ownership is not transferred; The
        caller is responsible for ensuring
        that the lifetime of the buffer
        extends until the view is destroyed.

        @par BNF
        @code
        URI           = scheme ":" hier-part [ "?" query ] [ "#" fragment ]

        relative-ref  = relative-part [ "?" query ] [ "#" fragment ]
        @endcode

        @throw std::invalid_argument parse error.

        @param s The string to parse.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-4.1"
            >4.1. URI Reference</a>
    */
    BOOST_URL_DECL
    url_view(string_view s);

    //--------------------------------------------
    //
    // Observers
    //
    //--------------------------------------------

    /** Return the maximum number of characters allowed in a URL.

        This does not include a null terminator.

        @par Exception Safety
        Throws nothing.
    */
    static
    constexpr
    std::size_t
    max_size() noexcept
    {
        return BOOST_URL_MAX_SIZE;
    }

    /** Return the number of characters in the URL.

        This function returns the number of
        characters in the URL, not including
        any null terminator, if present.

        @par Exception Safety
        Throws nothing.
    */
    std::size_t
    size() const noexcept
    {
        return offset(id_end);
    }

    /** Return true if the URL is empty.

        An empty URL is a <em>relative-ref</em> with
        zero path segments.

        @par Exception Safety
        Throws nothing.
    */
    bool
    empty() const noexcept
    {
        return size() == 0;
    }

    /** Return a pointer to the first character of the view.

        This function returns a pointer to the
        first character of the view, which is
        not guaranteed to be null-terminated.

        @par Exception Safety
        Throws nothing.
    */
    char const*
    data() const noexcept
    {
        return cs_;
    }

    /** Return an iterator to the beginning

        This function returns a constant iterator
        to the first character of the URL, or
        one past the last element if the URL is
        empty.
    */
    char const*
    begin() const noexcept
    {
        return data();
    }

    /** Return an iterator to the end

        This function returns a constant iterator to
        the character following the last character of
        the URL. This character acts as a placeholder,
        attempting to access it results in undefined
        behavior.
    */
    char const*
    end() const noexcept
    {
        return data() + size();
    }

    /** Return the complete encoded URL

        This function returns the URL as a
        percent-encoded string.

        @par Exception Safety
        Throws nothing.
    */
    string_view
    string() const noexcept
    {
        return string_view(
            data(), size());
    }

    /** Return a read-only copy of the URL, with shared lifetime.

        This function makes a copy of the storage
        pointed to by this, and attaches it to a
        new constant @ref url_view returned in a
        shared pointer. The lifetime of the storage
        for the characters will extend for the
        lifetime of the shared object. This allows
        the new view to be copied and passed around
        after the original string buffer is destroyed.

        @par Example
        @code
        std::shared_ptr<url_view const> sp;
        {
            std::string s( "http://example.com" );
            url_view u( s );                        // u references characters in s

            assert( u.data() == s.data() );         // same buffer

            sp = u.collect();

            assert( sp->data() != s.data() );       // different buffer
            assert( sp->string() == s);        // same contents

            // s is destroyed and thus u
            // becomes invalid, but sp remains valid.
        }
        std::cout << *sp; // works
        @endcode
    */
    BOOST_URL_DECL
    std::shared_ptr<
        url_view const>
    collect() const;

    //--------------------------------------------
    //
    // Scheme
    //
    //--------------------------------------------

    /** Return true if this contains a scheme

        This function returns true if this
        contains a scheme. URLs with schemes
        are called absolute URLs.

        @par Example
        @code
        url_view u( "http://www.example.com" );

        assert( u.has_scheme() );
        @endcode

        @par BNF
        @code
        URI             = scheme ":" hier-part [ "?" query ] [ "#" fragment ]

        absolute-URI    = scheme ":" hier-part [ "?" query ]

        scheme          = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.1"
            >3.1. Scheme (rfc3986)</a>

        @see
            @ref scheme,
            @ref scheme_id.
    */
    BOOST_URL_DECL
    bool
    has_scheme() const noexcept;

    /** Return the scheme

        This function returns the scheme if it
        exists, without a trailing colon (':').
        Otherwise it returns an empty string.

        @par Example
        @code
        assert( url_view( "http://www.example.com" ).scheme() == "http" );
        @endcode

        @par BNF
        @code
        scheme          = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )

        URI             = scheme ":" hier-part [ "?" query ] [ "#" fragment ]

        absolute-URI    = scheme ":" hier-part [ "?" query ]
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.1"
            >3.1. Scheme (rfc3986)</a>

        @see
            @ref has_scheme,
            @ref scheme_id.
    */
    BOOST_URL_DECL
    string_view
    scheme() const noexcept;

    /** Return a constant representing the scheme

        This function returns a @ref urls::scheme constant
        to identify the scheme as a well-known scheme.
        If the scheme is not recognized, the value
        @ref urls::scheme::unknown is returned. If
        this does not contain a scheme, then
        @ref urls::scheme::none is returned.

        @par BNF
        @code
        URI             = scheme ":" hier-part [ "?" query ] [ "#" fragment ]

        absolute-URI    = scheme ":" hier-part [ "?" query ]

        scheme          = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.1"
            >3.1. Scheme (rfc3986)</a>

        @see @ref urls::scheme
    */
    BOOST_URL_DECL
    urls::scheme
    scheme_id() const noexcept;

    //--------------------------------------------
    //
    // Authority
    //
    //--------------------------------------------

    /** Return true if an authority is present.

        This function returns `true` if the URL
        contains an authority. The authority is
        always preceded by a double slash ("//").

        @par Example
        @code
        assert( url_view( "http://www.example.com/index.htm" ).has_authority() == true );

        assert( url_view( "//" ).has_authority() == true );

        assert( url_view( "/file.txt" ).has_authority() == false );
        @endcode

        @par BNF
        @code
        authority       = [ userinfo "@" ] host [ ":" port ]

        URI             = scheme ":" hier-part [ "?" query ] [ "#" fragment ]

        absolute-URI    = scheme ":" hier-part [ "?" query ]

        URI-reference   = URI / relative-ref

        relative-ref    = relative-part [ "?" query ] [ "#" fragment ]

        hier-part       = "//" authority path-abempty
                        ; (more...)

        relative-part   = "//" authority path-abempty
                        ; (more...)

        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2"
            >3.2. Authority (rfc3986)</a>

        @see
            @ref encoded_authority.
    */
    bool
    has_authority() const noexcept
    {
        return len(id_user) > 0;
    }

    /** Return the authority.

        This function returns the authority as a
        percent-encoded string.

        @par Example
        @code
        assert( url_view( "http://www.example.com/index.htm" ).encoded_authority() == "www.example.com" );
        @endcode

        @par BNF
        @code
        authority   = [ userinfo "@" ] host [ ":" port ]
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2"
            >3.2. Authority (rfc3986)</a>

        @see
            @ref has_authority.
    */
    BOOST_URL_DECL
    string_view
    encoded_authority() const noexcept;

    /** Return the authority.

        This function returns the authority as a
        an @ref authority_view.

        @par Example
        @code
        assert( url_view( "http://www.example.com/index.htm" ).authority().encoded_host() == "www.example.com" );
        @endcode

        @par BNF
        @code
        authority   = [ userinfo "@" ] host [ ":" port ]
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2"
            >3.2. Authority (rfc3986)</a>

        @see
            @ref has_authority.
    */
    BOOST_URL_DECL
    authority_view
    authority() const noexcept;

    //--------------------------------------------

    /** Return true if a userinfo is present.

        This function returns true if this
        contains a userinfo.

        @par Example
        @code
        url_view u( "http://user@example.com" );

        assert( u.has_userinfo() == true );
        @endcode

        @par BNF
        @code
        userinfo    = user [ ":" [ password ] ]

        authority   = [ userinfo "@" ] host [ ":" port ]
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.1"
            >3.2.1. User Information (rfc3986)</a>

        @see
            @ref encoded_userinfo,
            @ref userinfo.
    */
    BOOST_URL_DECL
    bool
    has_userinfo() const noexcept;

    /** Return the userinfo

        This function returns the userinfo
        as a percent-encoded string.

        @par Example
        @code
        assert( url_view( "http://user:pass@example.com" ).encoded_userinfo() == "user:pass" );
        @endcode

        @par BNF
        @code
        userinfo    = user [ ":" [ password ] ]

        authority   = [ userinfo "@" ] host [ ":" port ]
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.1"
            >3.2.1. User Information (rfc3986)</a>

        @see
            @ref has_userinfo,
            @ref userinfo.
    */
    BOOST_URL_DECL
    string_view
    encoded_userinfo() const noexcept;

    /** Return the userinfo

        This function returns the userinfo as a
        string with percent-decoding applied, using
        the optionally specified allocator.

        @par Example
        @code
        url_view u( "http://user:pass@example.com" );

        assert( u.userinfo() == "user:pass" );
        @endcode

        @par BNF
        @code
        userinfo    = user [ ":" [ password ] ]

        authority   = [ userinfo "@" ] host [ ":" port ]
        @endcode

        @par Exception Safety
        Calls to allocate may throw.

        @param a An optional allocator the returned
        string will use. If this parameter is omitted,
        the default allocator is used.

        @return A @ref const_string using the
        specified allocator.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.1"
            >3.2.1. User Information (rfc3986)</a>

        @see
            @ref has_userinfo,
            @ref encoded_userinfo.
    */
    template<
        class Allocator =
            std::allocator<char>>
    const_string
    userinfo(
        Allocator const& a = {}) const
    {
        pct_decode_opts opt;
        opt.plus_to_space = false;
        return pct_decode_unchecked(
            encoded_userinfo(), opt, a);
    }

    //--------------------------------------------

    /** Return the user

        This function returns the user portion of
        the userinfo as a percent-encoded string.

        @par Example
        @code
        url_view u( "http://user:pass@example.com" );

        assert( u.encoded_user() == "user" );
        @endcode

        @par BNF
        @code
        userinfo    = user [ ":" [ password ] ]

        user        = *( unreserved / pct-encoded / sub-delims )
        password    = *( unreserved / pct-encoded / sub-delims / ":" )
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.1"
            >3.2.1. User Information (rfc3986)</a>

        @see
            @ref encoded_password,
            @ref has_password,
            @ref password,
            @ref user.
    */
    BOOST_URL_DECL
    string_view
    encoded_user() const noexcept;

    /** Return the user

        This function returns the user portion of
        the userinfo as a string with percent-decoding
        applied, using the optionally specified
        allocator.

        @par Example
        @code
        assert( url_view( "http://user:pass@example.com" ).user() == "user" );
        @endcode

        @par BNF
        @code
        userinfo    = user [ ":" [ password ] ]

        user        = *( unreserved / pct-encoded / sub-delims )
        password    = *( unreserved / pct-encoded / sub-delims / ":" )
        @endcode

        @par Exception Safety
        Calls to allocate may throw.

        @param a An optional allocator the returned
        string will use. If this parameter is omitted,
        the default allocator is used.

        @return A @ref const_string using the
        specified allocator.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.1"
            >3.2.1. User Information (rfc3986)</a>

        @see
            @ref encoded_password,
            @ref encoded_user,
            @ref has_password,
            @ref password.
    */
    template<
        class Allocator =
            std::allocator<char>>
    const_string
    user(
        Allocator const& a = {}) const
    {
        pct_decode_opts opt;
        opt.plus_to_space = false;
        return pct_decode_unchecked(
            encoded_user(), opt, a);
    }

    /** Return true if this contains a password

        This function returns true if the userinfo
        contains a password (which may be empty).

        @par Example
        @code
        assert( url_view( "http://user@example.com" ).has_password() == false );

        assert( url_view( "http://user:pass@example.com" ).has_password() == true );

        assert( url_view( "http://:@" ).has_password() == true );
        @endcode

        @par BNF
        @code
        userinfo    = user [ ":" [ password ] ]

        user        = *( unreserved / pct-encoded / sub-delims )
        password    = *( unreserved / pct-encoded / sub-delims / ":" )
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.1"
            >3.2.1. User Information (rfc3986)</a>

        @see
            @ref encoded_password,
            @ref encoded_user,
            @ref password,
            @ref user.
    */
    BOOST_URL_DECL
    bool
    has_password() const noexcept;

    /** Return the password

        This function returns the password portion
        of the userinfo as a percent-encoded string.

        @par Example
        @code
        url_view u( "http://user:pass@example.com" );

        assert( u.encoded_user() == "user" );
        @endcode

        @par BNF
        @code
        userinfo    = user [ ":" [ password ] ]

        user        = *( unreserved / pct-encoded / sub-delims )
        password    = *( unreserved / pct-encoded / sub-delims / ":" )
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.1"
            >3.2.1. User Information (rfc3986)</a>

        @see
            @ref encoded_password,
            @ref has_password,
            @ref password,
            @ref user.
    */
    BOOST_URL_DECL
    string_view
    encoded_password() const noexcept;

    /** Return the password

        This function returns the password from the
        userinfo with percent-decoding applied,
        using the optionally specified allocator.

        @par Exception Safety
        Calls to allocate may throw.

        @param a An allocator to use for the string.
        If this parameter is omitted, the default
        allocator is used, and the return type of
        the function becomes `std::string`.

        @return A @ref const_string using the
        specified allocator.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.1"
            >3.2.1. User Information (rfc3986)</a>

        @see
            @ref encoded_password,
            @ref encoded_user,
            @ref has_password,
            @ref password.
    */
    template<
        class Allocator =
            std::allocator<char>>
    const_string
    password(
        Allocator const& a = {}) const
    {
        pct_decode_opts opt;
        opt.plus_to_space = false;
        return pct_decode_unchecked(
            encoded_password(), opt, a);
    }

    //--------------------------------------------

    /** Return the type of host present, if any

        This function returns a
            @ref urls::host_type
        constant representing the type of
        host this contains, which may be
        @ref urls::host_type::none.

        @par Example
        @code
        assert( url_view( "/favicon.png" ).host_type() == host_type::none );

        assert( url_view( "http://example.com" ).host_type() == host_type::name );

        assert( url_view( "http://192.168.0.1" ).host_type() == host_type::ipv4 );
        @endcode

        @par BNF
        @code
        host        = IP-literal / IPv4address / reg-name

        IP-literal  = "[" ( IPv6address / IPvFuture  ) "]"

        IPvFuture   = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.2"
            >3.2.2. Host (rfc3986)</a>

        @see
            @ref encoded_host,
            @ref has_port,
            @ref host,
            @ref port,
            @ref port_number.
    */
    urls::host_type
    host_type() const noexcept
    {
        return host_type_;
    }

    /** Return the host

        This function returns the host portion of
        the authority as a percent-encoded string,

        @par Example
        @code
        assert( url_view( "/favicon.png" ).encoded_host() == "" );

        assert( url_view( "http://example.com" ).encoded_host() == "example.com" );

        assert( url_view( "http://192.168.0.1" ).encoded_host() == "192.168.0.1" );
        @endcode

        @par BNF
        @code
        host        = IP-literal / IPv4address / reg-name

        IP-literal  = "[" ( IPv6address / IPvFuture  ) "]"

        IPvFuture   = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )

        reg-name    = *( unreserved / pct-encoded / "-" / ".")
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.2"
            >3.2.2. Host (rfc3986)</a>

        @see
            @ref has_port,
            @ref host,
            @ref host_type,
            @ref port,
            @ref port_number.
    */
    BOOST_URL_DECL
    string_view
    encoded_host() const noexcept;

    /** Return the host

        This function returns the host portion of
        the authority as a string with percent-decoding
        applied, using the optionally specified
        allocator.

        @par Example
        @code
        assert( url_view( "/favicon.png" ).host() == "" );

        assert( url_view( "http://example.com" ).host() == "example.com" );

        assert( url_view( "http://192.168.0.1" ).host() == "192.168.0.1" );
        @endcode

        @par BNF
        @code
        host        = IP-literal / IPv4address / reg-name

        IP-literal  = "[" ( IPv6address / IPvFuture  ) "]"

        IPvFuture   = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )

        reg-name    = *( unreserved / pct-encoded / "-" / ".")
        @endcode

        @par Exception Safety
        Calls to allocate may throw.

        @param a An optional allocator the returned
        string will use. If this parameter is omitted,
        the default allocator is used, and the return
        type of the function becomes `std::string`.

        @return A @ref const_string using the
        specified allocator.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.2"
            >3.2.2. Host (rfc3986)</a>

        @see
            @ref encoded_host,
            @ref encoded_host_and_port,
            @ref has_port,
            @ref host_type,
            @ref port,
            @ref port_number.
    */
    template<
        class Allocator =
            std::allocator<char>>
    const_string
    host(
        Allocator const& a = {}) const
    {
        auto const s0 =
            encoded_host();
        if(host_type_ !=
            urls::host_type::name)
        {
            // no decoding
            return const_string(s0, a);
        }
        pct_decode_opts opt;
        opt.plus_to_space = false;
        return pct_decode_unchecked(
            s0, opt, a, decoded_[id_host]);
    }

    /** Return the host as an IPv4 address

        If @ref host_type equals @ref urls::host_type::ipv4,
        this function returns the corresponding
        @ref ipv4_address
        of the host if it exists, otherwise it
        returns the unspecified address which
        is equal to "0.0.0.0".

        @par BNF
        @code
        IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet

        dec-octet   = DIGIT                 ; 0-9
                    / %x31-39 DIGIT         ; 10-99
                    / "1" 2DIGIT            ; 100-199
                    / "2" %x30-34 DIGIT     ; 200-249
                    / "25" %x30-35          ; 250-255
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.2"
            >3.2.2. Host (rfc3986)</a>

        @see
            @ref ipv4_address.
    */
    BOOST_URL_DECL
    urls::ipv4_address
    ipv4_address() const noexcept;

    /** Return the host as an IPv6 address

        If @ref host_type equals
        @ref urls::host_type::ipv6, this
        function returns the corresponding
        @ref ipv6_address of the host if it
        exists, otherwise it returns the
        unspecified address which is equal
        to "0:0:0:0:0:0:0:0".

        @par BNF
        @code
        IPv6address =                            6( h16 ":" ) ls32
                    /                       "::" 5( h16 ":" ) ls32
                    / [               h16 ] "::" 4( h16 ":" ) ls32
                    / [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
                    / [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
                    / [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32
                    / [ *4( h16 ":" ) h16 ] "::"              ls32
                    / [ *5( h16 ":" ) h16 ] "::"              h16
                    / [ *6( h16 ":" ) h16 ] "::"

        ls32        = ( h16 ":" h16 ) / IPv4address
                    ; least-significant 32 bits of address

        h16         = 1*4HEXDIG
                    ; 16 bits of address represented in hexadecimal
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.2"
            >3.2.2. Host (rfc3986)</a>

        @see
            @ref ipv6_address.
    */
    BOOST_URL_DECL
    urls::ipv6_address
    ipv6_address() const noexcept;

    /** Return the host as an IPvFuture string

        If @ref host_type equals
        @ref urls::host_type::ipvfuture, this
        function returns a string representing
        the address. Otherwise it returns the
        empty string.

        @par BNF
        @code
        IPvFuture  = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.2"
            >3.2.2. Host (rfc3986)</a>
    */
    BOOST_URL_DECL
    string_view
    ipv_future() const noexcept;

    /** Return true if the URL contains a port

        This function returns true if the
        authority contains a port.

        @par BNF
        @code
        authority   = [ userinfo "@" ] host [ ":" port ]

        port        = *DIGIT
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.3"
            >3.2.3. Port (rfc3986)</a>

        @see
            @ref encoded_host,
            @ref encoded_host_and_port
            @ref host,
            @ref host_type,
            @ref port,
            @ref port_number.
    */
    BOOST_URL_DECL
    bool
    has_port() const noexcept;

    /** Return the port

        This function returns the port specified
        in the authority, or an empty string if
        there is no port.

        @par BNF
        @code
        port        = *DIGIT
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.3"
            >3.2.3. Port (rfc3986)</a>

        @see
            @ref encoded_host,
            @ref encoded_host_and_port
            @ref has_port,
            @ref host,
            @ref host_type,
            @ref port_number.
    */
    BOOST_URL_DECL
    string_view
    port() const noexcept;

    /** Return the port as an integer.

        This function returns the port as an
        integer if the authority specifies
        a port and the number can be represented.
        Otherwise it returns zero.

        @par BNF
        @code
        port        = *DIGIT
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.3"
            >3.2.3. Port (rfc3986)</a>

        @see
            @ref encoded_host,
            @ref encoded_host_and_port
            @ref has_port,
            @ref host,
            @ref host_type,
            @ref port,
    */
    BOOST_URL_DECL
    std::uint16_t
    port_number() const noexcept;

    /** Return the host and port

        This function returns the host and
        port of the authority as a single
        percent-encoded string.

        @par BNF
        @code
        authority   = [ userinfo "@" ] host [ ":" port ]
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.3"
            >3.2.3. Port (rfc3986)</a>

        @see
            @ref encoded_host,
            @ref encoded_host_and_port
            @ref has_port,
            @ref host,
            @ref host_type,
            @ref port,
    */
    BOOST_URL_DECL
    string_view
    encoded_host_and_port() const noexcept;

    //--------------------------------------------

    /** Return the origin

        This function returns the origin as
        a percent-encoded string. The origin
        consists of the scheme and authority.
        This string will be empty if no
        authority is present.
    */
    BOOST_URL_DECL
    string_view
    encoded_origin() const noexcept;

    //--------------------------------------------
    //
    // Path
    //
    //--------------------------------------------

    /** Return true if the path is absolute.

        This function returns true if the path
        begins with a forward slash ('/').
    */
    bool
    is_path_absolute() const noexcept
    {
        return
            len(id_path) > 0 &&
            cs_[offset(id_path)] == '/';
    }

    /** Return the path.

        This function returns the path as a
        percent-encoded string.

        @par BNF
        @code
        path          = [ "/" ] segment *( "/" segment )
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.3"
            >3.3. Path (rfc3986)</a>
    */
    string_view
    encoded_path() const noexcept
    {
        return get(id_path);
    }

    /** Return the path segments

        This function returns the path segments as
        a read-only bidirectional range.

        @par BNF
        @code
        path          = [ "/" ] segment *( "/" segment )
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.3"
            >3.3. Path (rfc3986)</a>
    */
    segments_encoded_view
    encoded_segments() const noexcept
    {
        return segments_encoded_view(
            encoded_path(), nseg_);
    }

    /** Return the path segments

        This function returns the path segments as
        a read-only bidirectional range.

        @par BNF
        @code
        path          = [ "/" ] segment *( "/" segment )
        @endcode

        @par Exception Safety
        Throws nothing.

        @param alloc An optional allocator the
        container will use when returning
        percent-decoded strings. If omitted,
        the default allocator is used.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.3"
            >3.3. Path (rfc3986)</a>
    */
    template<class Allocator =
        std::allocator<char>>
    segments_view
    segments(Allocator const& alloc = {}) const noexcept
    {
        return segments_view(
            encoded_path(), nseg_, alloc);
    }

    //--------------------------------------------
    //
    // Query
    //
    //--------------------------------------------

    /** Return true if this contains a query

        This function returns true if this
        contains a query.

        @par BNF
        @code
        query           = *( pchar / "/" / "?" )

        query-part      = [ "?" query ]
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.4"
            >3.4. Query (rfc3986)</a>

        @see
            @ref encoded_query,
            @ref query.
    */
    BOOST_URL_DECL
    bool
    has_query() const noexcept;

    /** Return the query

        This function returns the query as
        a percent-encoded string.

        @par BNF
        @code
        query           = *( pchar / "/" / "?" )

        query-part      = [ "?" query ]
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.4"
            >3.4. Query (rfc3986)</a>

        @see
            @ref encoded_query,
            @ref query.
    */
    BOOST_URL_DECL
    string_view
    encoded_query() const noexcept;

    /** Return the query

        This function returns the query as a
        string with percent-decoding applied,
        using the optionally specified allocator.

        @par BNF
        @code
        query           = *( pchar / "/" / "?" )

        query-part      = [ "?" query ]
        @endcode

        @par Exception Safety
        Calls to allocate may throw.

        @param a An optional allocator the returned
        string will use. If this parameter is omitted,
        the default allocator is used

        @return A @ref const_string using the
        specified allocator.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.4"
            >3.4. Query (rfc3986)</a>

        @see
            @ref encoded_query,
            @ref has_query.
    */
    template<
        class Allocator =
            std::allocator<char>>
    const_string
    query(
        Allocator const& a = {}) const
    {
        pct_decode_opts opt;
        opt.plus_to_space = true;
        return pct_decode_unchecked(
            encoded_query(), opt, a);
    }

    /** Return the query parameters

        This function returns the query
        parameters as a non-modifiable
        forward range of key/value pairs.
        Each string returned by the container
        is percent-encoded.

        @par BNF
        @code
        query-params    = [ query-param ] *( "&" [ query-param ] )

        query-param     = key [ "=" value ]

        @endcode
    */
    BOOST_URL_DECL
    params_encoded_view
    encoded_params() const noexcept;

    /** Return the query parameters

        This function returns the query
        parameters as a non-modifiable
        forward range of key/value pairs
        where each returned string has
        percent-decoding applied.

        @par BNF
        @code
        query-params    = [ query-param ] *( "&" [ query-param ] )

        query-param     = key [ "=" value ]
        @endcode

        @param alloc An optional allocator the
        container will use when returning
        percent-decoded strings. If omitted,
        the default allocator is used.
    */
    template<
        class Allocator =
            std::allocator<char>>
    params_view
    params(Allocator const&
        alloc = {}) const noexcept;

    //--------------------------------------------
    //
    // Fragment
    //
    //--------------------------------------------

    /** Return true if a fragment exists.

        This function returns true if this
        contains a fragment.

        @par BNF
        @code
        URI           = scheme ":" hier-part [ "?" query ] [ "#" fragment ]

        relative-ref  = relative-part [ "?" query ] [ "#" fragment ]
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.5"
            >3.5. Fragment (rfc3986)</a>

        @see
            @ref encoded_fragment,
            @ref fragment.
    */
    BOOST_URL_DECL
    bool
    has_fragment() const noexcept;

    /** Return the fragment.

        This function returns the fragment as a
        percent-encoded string.

        @par BNF
        @code
        fragment        = *( pchar / "/" / "?" )

        pchar           = unreserved / pct-encoded / sub-delims / ":" / "@"
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.5"
            >3.5. Fragment (rfc3986)</a>

        @see
            @ref fragment,
            @ref has_fragment,
            @ref pchars.
    */
    BOOST_URL_DECL
    string_view
    encoded_fragment() const noexcept;

    /** Return the fragment.

        This function returns the fragment as a
        string with percent-decoding applied,
        using the optionally specified allocator.

        @par BNF
        @code
        fragment        = *( pchar / "/" / "?" )

        fragment-part   = [ "#" fragment ]
        @endcode

        @par Exception Safety
        Calls to allocate may throw.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.5"
            >3.5. Fragment (rfc3986)</a>

        @param a An optional allocator the returned
        string will use. If this parameter is omitted,
        the default allocator is used.

        @return A @ref const_string using the
        specified allocator.

        @see
            @ref encoded_fragment,
            @ref has_fragment.
    */
    template<
        class Allocator =
            std::allocator<char>>
    const_string
    fragment(
        Allocator const& a = {}) const
    {
        pct_decode_opts opt;
        opt.plus_to_space = false;
        return pct_decode_unchecked(
            encoded_fragment(),
            opt, a, decoded_[id_frag]);
    }

    //--------------------------------------------
    //
    // Comparison
    //
    //--------------------------------------------

    /** Compare two URLs.

        This function compares two URLs
        according to Syntax-Based comparison
        algorithm.

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-6.2.2"
            >6.2.2 Syntax-Based Normalization (rfc3986)</a>
    */
    BOOST_URL_DECL
    int
    compare(const url_view& other) const noexcept;

    /** Return `true` if two url_views are equal.

        Two url_views are equal when their
        normalized representations are the same.

        Two URLs can compare equal even if their
        serialized representations are not
        identical strings.

        @par Example
        @code
        url_view u1( "HtTp://cXaMpLe.oRG/" );
        url_view u2( "http://example.org/" );
        assert( u1.compare(u2) == 0 );
        @endcode

        @par Effects
        `return url( lhs ).normalize() == url( rhs ).normalize();`

        @par Complexity
        Linear in string size.

        @par Exception Safety
        Throws nothing
    */
    friend
    bool
    operator==(
        url_view const& lhs,
        url_view const& rhs) noexcept
    {
        return lhs.compare(rhs) == 0;
    }

    /** Return `true` if two url_views are not equal.

        Two url_views are equal when their
        normalized representations are the same.

        Two URLs can compare not equal even if
        their serialized representations represent
        the same resource.

        @par Effects
        `return ! url( lhs ).normalize() == url( rhs ).normalize();`

        @par Complexity
        Linear in string size.

        @par Exception Safety
        Throws nothing
    */
    friend
    bool
    operator!=(
        url_view const& lhs,
        url_view const& rhs) noexcept
    {
        return ! (lhs == rhs);
    }

    /** Lexicographically compares two url_views

        Each URL component is compared
        alphabetically on a character by character
        basis as if each was normalized first.

        Two URLs can compare equal even if their
        serialized representations are not
        identical strings.

        @par Complexity
        Linear in string sizes.

        @par Exception Safety
        Throws nothing
    */
    friend
    bool
    operator<(
        url_view const& lhs,
        url_view const& rhs) noexcept
    {
        return lhs.compare(rhs) < 0;
    }


    /** Lexicographically compares two url_views

        Each URL component is compared
        alphabetically on a character by character
        basis as if each was normalized first.

        Two URLs can compare equal even if their
        serialized representations are not
        identical strings.

        @par Complexity
        Linear in string sizes.

        @par Exception Safety
        Throws nothing
    */
    friend
    bool
    operator<=(
        url_view const& lhs,
        url_view const& rhs) noexcept
    {
        return lhs.compare(rhs) <= 0;
    }


    /** Lexicographically compares two url_views

        Each URL component is compared
        alphabetically on a character by character
        basis as if each was normalized first.

        Two URLs can compare equal even if their
        serialized representations are not
        identical strings.

        @par Complexity
        Linear in string sizes.

        @par Exception Safety
        Throws nothing
    */
    friend
    bool
    operator>(
        url_view const& lhs,
        url_view const& rhs) noexcept
    {
        return lhs.compare(rhs) > 0;
    }


    /** Lexicographically compares two url_views

        Each URL component is compared
        alphabetically on a character by character
        basis as if each was normalized first.

        Two URLs can compare equal even if their
        serialized representations are not
        identical strings.

        @par Complexity
        Linear in string sizes.

        @par Exception Safety
        Throws nothing
    */
    friend
    bool
    operator>=(
        url_view const& lhs,
        url_view const& rhs) noexcept
    {
        return lhs.compare(rhs) >= 0;
    }

    //--------------------------------------------
    //
    // Parsing
    //
    //--------------------------------------------

    BOOST_URL_DECL friend result<url_view>
        parse_absolute_uri(string_view s);
    BOOST_URL_DECL friend result<url_view>
        parse_relative_ref(string_view s);
    BOOST_URL_DECL friend result<url_view>
        parse_uri(string_view s);
    BOOST_URL_DECL friend result<url_view>
        parse_uri_reference(string_view s);


private:
    void apply(scheme_part_rule const& t) noexcept;
    void apply(host_rule const& h) noexcept;
    void apply(authority_rule const& t) noexcept;
    void apply(parsed_path const& path) noexcept;
    void apply(query_part_rule const& t) noexcept;
    void apply(fragment_part_rule const& t) noexcept;
};

//------------------------------------------------

/** Parse an absolute-URI

    This function parses a string according
    to the absolute-URI grammar below, and
    returns a @ref url_view referencing the string.
    Ownership of the string is not transferred;
    the caller is responsible for ensuring that
    the lifetime of the string extends until the
    view is no longer being accessed.

    @par BNF
    @code
    absolute-URI    = scheme ":" hier-part [ "?" query ]

    hier-part       = "//" authority path-abempty
                    / path-absolute
                    / path-rootless
                    / path-empty
    @endcode

    @throw std::length_error `s.size() > url_view::max_size`

    @return A result containing the view to the URL,
        or an error code if the parsing was unsuccessful.

    @param s The string to parse

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-4.3"
        >4.3. Absolute URI (rfc3986)</a>

    @see
        @ref parse_absolute_uri,
        @ref parse_relative_ref,
        @ref parse_uri,
        @ref parse_uri_reference,
        @ref url_view.
*/
BOOST_URL_DECL
result<url_view>
parse_absolute_uri(
    string_view s);

/** Parse a URI

    This function parses a string according
    to the URI grammar below, and returns a
    @ref url_view referencing the string.
    Ownership of the string is not transferred;
    the caller is responsible for ensuring that
    the lifetime of the string extends until the
    view is no longer being accessed.

    @par BNF
    @code
    URI           = scheme ":" hier-part [ "?" query ] [ "#" fragment ]

    hier-part     = "//" authority path-abempty
                  / path-absolute
                  / path-rootless
                  / path-empty
    @endcode

    @par Exception Safety
    Throws nothing.

    @throw std::length_error `s.size() > url_view::max_size`

    @return A result containing the view to the URL,
        or an error code if the parsing was unsuccessful.

    @param s The string to parse

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3"
        >3. Syntax Components (rfc3986)</a>

    @see
        @ref parse_absolute_uri,
        @ref parse_relative_ref,
        @ref parse_uri,
        @ref parse_uri_reference,
        @ref url_view.
*/
BOOST_URL_DECL
result<url_view>
parse_uri(
    string_view s);

/** Parse a relative-ref

    This function parses a string according
    to the relative-ref grammar below, and
    returns a @ref url_view referencing the string.
    Ownership of the string is not transferred;
    the caller is responsible for ensuring that
    the lifetime of the string extends until the
    view is no longer being accessed.

    @par BNF
    @code
    relative-ref  = relative-part [ "?" query ] [ "#" fragment ]

    relative-part = "//" authority path-abempty
                  / path-absolute
                  / path-noscheme
                  / path-abempty
                  / path-empty
    @endcode

    @throw std::length_error `s.size() > url_view::max_size`

    @return A result containing the view to the URL,
        or an error code if the parsing was unsuccessful.

    @param s The string to parse

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-4.2
        >4.2. Relative Reference (rfc3986)</a>
    @li <a href="https://www.rfc-editor.org/errata/eid5428"
        >Errata ID: 5428 (rfc3986)</a>

    @see
        @ref parse_absolute_uri,
        @ref parse_relative_ref,
        @ref parse_uri,
        @ref parse_uri_reference,
        @ref url_view.
*/
BOOST_URL_DECL
result<url_view>
parse_relative_ref(
    string_view s);

/** Parse a URI-reference

    This function parses a string according
    to the URI-reference grammar below, and
    returns a @ref url_view referencing the string.
    Ownership of the string is not transferred;
    the caller is responsible for ensuring that
    the lifetime of the string extends until the
    view is no longer being accessed.

    @par BNF
    @code
    URI           = scheme ":" hier-part [ "?" query ] [ "#" fragment ]

    hier-part     = "//" authority path-abempty
                  / path-absolute
                  / path-rootless
                  / path-empty

    URI-reference = URI / relative-ref

    relative-ref  = relative-part [ "?" query ] [ "#" fragment ]

    relative-part = "//" authority path-abempty
                  / path-absolute
                  / path-noscheme
                  / path-abempty
                  / path-empty
    @endcode

    @throw std::length_error `s.size() > url_view::max_size`

    @return A result containing the view to the URL,
        or an error code if the parsing was unsuccessful.

    @param s The string to parse.

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-4.1"
        >4.1. URI Reference (rfc3986)</a>
    @li <a href="https://www.rfc-editor.org/errata/eid5428"
        >Errata ID: 5428 (rfc3986)</a>

    @see
        @ref parse_absolute_uri,
        @ref parse_relative_ref,
        @ref parse_uri,
        @ref parse_uri_reference,
        @ref url_view.
*/
BOOST_URL_DECL
result<url_view>
parse_uri_reference(
    string_view s);

//------------------------------------------------

/** Format the encoded URL to the output stream.

    This function serializes the encoded URL
    to the output stream.

    @par Example
    @code
    url_view u( "http://www.example.com/index.htm" );

    std::cout << u << std::endl;
    @endcode

    @return A reference to the output stream, for chaining

    @param os The output stream to write to.

    @param u The URL to write.
*/
BOOST_URL_DECL
std::ostream&
operator<<(
    std::ostream& os,
    url_view const& u);

} // urls
} // boost

// std::hash specialization
#ifndef BOOST_URL_DOCS
namespace std {
template<>
struct hash< ::boost::urls::url_view >
{
    hash() = default;
    hash(hash const&) = default;
    hash& operator=(hash const&) = default;

    explicit
    hash(std::size_t salt) noexcept
        : salt_(salt)
    {
    }

    BOOST_URL_DECL
    std::size_t
    operator()(::boost::urls::url_view const&) const noexcept;

private:
    std::size_t salt_ = 0;
};
} // std
#endif


// These includes are here
// due to circular dependencies
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_PARAMS_VIEW_HPP
#define BOOST_URL_IMPL_PARAMS_VIEW_HPP

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_PARAMS_ITERATOR_IMPL_HPP
#define BOOST_URL_DETAIL_PARAMS_ITERATOR_IMPL_HPP

#include <string>

namespace boost {
namespace urls {
namespace detail {

struct params_iterator_impl
{
    std::size_t i_ = 0;
    char const* begin_ = nullptr;
    char const* pos_ = nullptr;
    std::size_t nk_ = 0;
    std::size_t nv_ = 0;
    char const* end_ = nullptr;
    const_string::factory a_;

    BOOST_URL_DECL
    void scan() noexcept;

    BOOST_URL_DECL
    params_iterator_impl(
        string_view s,
        const_string::factory a) noexcept;

    // end ctor
    BOOST_URL_DECL
    params_iterator_impl(
        string_view s,
        std::size_t nparam,
        int,
        const_string::factory a) noexcept;

    params_iterator_impl() = default;

    params_iterator_impl(
        params_iterator_impl const&) noexcept = default;

    params_iterator_impl& operator=(
        params_iterator_impl const&) noexcept = default;

    string_view
    encoded_key() const noexcept;

    BOOST_URL_DECL
    params_view::reference
    dereference() const noexcept;

    BOOST_URL_DECL
    void
    increment() noexcept;

    bool
    equal(
        params_iterator_impl const& other) const noexcept
    {
        BOOST_ASSERT(begin_ == other.begin_);
        BOOST_ASSERT(end_ == other.end_);
        return
            pos_ == other.pos_ &&
            i_ == other.i_;
    }
};

} // detail
} // urls
} // boost

#endif
#include <boost/assert.hpp>

namespace boost {
namespace urls {

//------------------------------------------------

class params_view::iterator
{
    detail::params_iterator_impl impl_;

    friend class params_view;

    iterator(
        string_view s,
        const_string::factory a) noexcept
        : impl_(s, a)
    {
    }

    // end
    iterator(
        string_view s,
        std::size_t nparam,
        int,
        const_string::factory a) noexcept
        : impl_(s, nparam, 0, a)
    {
    }

    string_view
    encoded_key() const noexcept
    {
        return impl_.encoded_key();
    }

public:
    using value_type = query_param;
    using reference = params_view::value_type;
    using pointer = void const*;
    using difference_type = std::ptrdiff_t;
    using iterator_category =
        std::forward_iterator_tag;

    iterator() = default;

    iterator&
    operator++() noexcept
    {
        impl_.increment();
        return *this;
    }

    iterator
    operator++(int) noexcept
    {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    reference
    operator*() const
    {
        return impl_.dereference();
    }

    friend
    bool
    operator==(
        iterator const& a,
        iterator const& b) noexcept
    {
        return a.impl_.equal(b.impl_);
    }

    friend
    bool
    operator!=(
        iterator const& a,
        iterator const& b) noexcept
    {
        return !a.impl_.equal(b.impl_);
    }
};

//------------------------------------------------
//
// Members
//
//------------------------------------------------

template<class Allocator>
params_view::
params_view(
    string_view s,
    std::size_t n,
    Allocator const& a)
    : s_(s)
    , n_(n)
    , a_(a)
{
}

//------------------------------------------------
//
// Capacity
//
//------------------------------------------------

inline
bool
params_view::
empty() const noexcept
{
    return n_ == 0;
}

inline
std::size_t
params_view::
size() const noexcept
{
    return n_;
}

//------------------------------------------------
//
// Lookup
//
//------------------------------------------------

inline
auto
params_view::
find(string_view key) const noexcept ->
    iterator
{
    return find(begin(), key);
}

inline
bool
params_view::
contains(
    string_view key) const noexcept
{
    return find(key) != end();
}

} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_PARAMS_ENCODED_VIEW_HPP
#define BOOST_URL_IMPL_PARAMS_ENCODED_VIEW_HPP

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_PARAMS_ENCODED_ITERATOR_IMPL_HPP
#define BOOST_URL_DETAIL_PARAMS_ENCODED_ITERATOR_IMPL_HPP

#include <string>

namespace boost {
namespace urls {
namespace detail {

struct params_encoded_iterator_impl
{
    std::size_t i_ = 0;
    char const* begin_ = nullptr;
    char const* pos_ = nullptr;
    std::size_t nk_ = 0;
    std::size_t nv_ = 0;
    char const* end_ = nullptr;

    BOOST_URL_DECL
    void scan() noexcept;

    BOOST_URL_DECL
    params_encoded_iterator_impl(
        string_view s) noexcept;

    // end ctor
    BOOST_URL_DECL
    params_encoded_iterator_impl(
        string_view s,
        std::size_t nparam,
        int) noexcept;

    params_encoded_iterator_impl() = default;

    params_encoded_iterator_impl(
        params_encoded_iterator_impl const&) noexcept = default;

    params_encoded_iterator_impl& operator=(
        params_encoded_iterator_impl const&) noexcept = default;

    BOOST_URL_DECL
    void
    increment() noexcept;

    string_view
    encoded_key() const noexcept;

    BOOST_URL_DECL
    query_param_view
    dereference() const;

    bool
    equal(
        params_encoded_iterator_impl const& other) const noexcept
    {
        BOOST_ASSERT(begin_ == other.begin_);
        BOOST_ASSERT(end_ == other.end_);
        return
            pos_ == other.pos_ &&
            i_ == other.i_;
    }
};

} // detail
} // urls
} // boost

#endif
#include <boost/assert.hpp>

namespace boost {
namespace urls {

//------------------------------------------------

class params_encoded_view::iterator
{
    detail::params_encoded_iterator_impl impl_;

    friend class params_encoded_view;

    iterator(
        string_view s) noexcept
        : impl_(s)
    {
    }

    // end
    iterator(
        string_view s,
        std::size_t nparam,
        int) noexcept
        : impl_(s, nparam, 0)
    {
    }

    string_view
    encoded_key() const noexcept
    {
        return impl_.encoded_key();
    }

public:
    using value_type = query_param;
    using reference = query_param_view;
    using pointer = void const*;
    using difference_type = std::ptrdiff_t;
    using iterator_category =
        std::forward_iterator_tag;

    iterator() = default;

    iterator&
    operator++() noexcept
    {
        impl_.increment();
        return *this;
    }

    iterator
    operator++(int) noexcept
    {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    reference
    operator*() const
    {
        return impl_.dereference();
    }

    friend
    bool
    operator==(
        iterator const& a,
        iterator const& b) noexcept
    {
        return a.impl_.equal(b.impl_);
    }

    friend
    bool
    operator!=(
        iterator const& a,
        iterator const& b) noexcept
    {
        return !a.impl_.equal(b.impl_);
    }
};

//------------------------------------------------
//
// Members
//
//------------------------------------------------

inline
params_encoded_view::
params_encoded_view(
    string_view s,
    std::size_t n) noexcept
    : s_(s)
    , n_(n)
{
}

template<class Allocator>
params_view
params_encoded_view::
decoded(Allocator const& alloc) const
{
    return params_view(s_, n_, alloc);
}

//------------------------------------------------
//
// Capacity
//
//------------------------------------------------

inline
bool
params_encoded_view::
empty() const noexcept
{
    return n_ == 0;
}

inline
std::size_t
params_encoded_view::
size() const noexcept
{
    return n_;
}

//------------------------------------------------
//
// Lookup
//
//------------------------------------------------

inline
auto
params_encoded_view::
find(string_view key) const noexcept ->
    iterator
{
    return find(begin(), key);
}

inline
bool
params_encoded_view::
contains(string_view key) const noexcept
{
    return find(key) != end();
}

} // urls
} // boost

#endif

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_URL_VIEW_HPP
#define BOOST_URL_IMPL_URL_VIEW_HPP

namespace boost {
namespace urls {

inline
url_view&
url_view::
base() noexcept
{
    return *this;
}
    
inline
url_view const&
url_view::
base() const noexcept
{
    return *this;
}

// return size of table in bytes
inline
std::size_t
url_view::
table_bytes() const noexcept
{
    std::size_t n = 0;
    if(nseg_ > 1)
        n += nseg_ - 1;
    if(nparam_ > 1)
        n += nparam_ - 1;
    return n * sizeof(pos_t);
}

// return length of [first, last)
inline
auto
url_view::
len(
    int first,
    int last) const noexcept ->
        pos_t
{
    BOOST_ASSERT(first <= last);
    BOOST_ASSERT(last <= id_end);
    return offset(last) - offset(first);
}

// change id to size n
inline
void
url_view::
set_size(
    int id,
    pos_t n) noexcept
{
    auto d = n - len(id);
    for(auto i = id + 1;
        i <= id_end; ++i)
        offset_[i] += d;
}

// trim id to size n,
// moving excess into id+1
inline
void
url_view::
split(
    int id,
    std::size_t n) noexcept
{
    BOOST_ASSERT(id < id_end - 1);
    //BOOST_ASSERT(n <= len(id));
    offset_[id + 1] = offset(id) + n;
}

// add n to [first, last]
inline
void
url_view::
adjust(
    int first,
    int last,
    std::size_t n) noexcept
{
    for(int i = first;
            i <= last; ++i)
        offset_[i] += n;
}

// set [first, last) offset
inline
void
url_view::
collapse(
    int first,
    int last,
    std::size_t n) noexcept
{
    for(int i = first + 1;
            i < last; ++i)
        offset_[i] = n;
}

//------------------------------------------------
//
// Query
//
//------------------------------------------------

template<class Allocator>
params_view
url_view::
params(
    Allocator const& a) const noexcept
{
    auto s = get(id_query);
    if(s.empty())
        return params_view(
            s, 0, a);
    BOOST_ASSERT(s[0] == '?');
    s.remove_prefix(1);
    return params_view(
        s, nparam_, a);
}

} // urls
} // boost

#endif
#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_ANY_PATH_ITER_HPP
#define BOOST_URL_DETAIL_ANY_PATH_ITER_HPP

#include <cstddef>

namespace boost {
namespace urls {
namespace detail {

struct BOOST_SYMBOL_VISIBLE
    any_path_iter
{
    string_view front;

    BOOST_URL_DECL
    virtual
    ~any_path_iter() noexcept = 0;

    virtual
    bool
    measure(
        std::size_t& n,
        error_code& ec) noexcept = 0;

    virtual
    void
    copy(
        char*& dest,
        char const* end) noexcept = 0;
};

//------------------------------------------------

// iterates segments in an
// encoded path string
class BOOST_SYMBOL_VISIBLE
    enc_path_iter
    : public any_path_iter
{
    std::size_t n_;
    char const* p_;
    char const* end_;

    void
    increment() noexcept;

public:
    explicit
    enc_path_iter(
        string_view s) noexcept;

    bool
    measure(
        std::size_t& n,
        error_code& ec) noexcept override;

    void
    copy(
        char*& dest,
        char const* end) noexcept override;
};

//------------------------------------------------

// iterates segments in an
// plain path string
class BOOST_SYMBOL_VISIBLE
    plain_path_iter :
    public any_path_iter
{
    std::size_t n_;
    char const* p_;
    char const* end_;

    void
    increment() noexcept;

public:
    explicit
    plain_path_iter(
        string_view s) noexcept;

    bool
    measure(
        std::size_t& n,
        error_code& ec) noexcept override;

    void
    copy(
        char*& dest,
        char const* end) noexcept override;
};

//------------------------------------------------

class enc_segs_iter_base
{
protected:
    BOOST_URL_DECL
    static
    bool
    measure_impl(
        string_view s,
        std::size_t& n,
        error_code& ec) noexcept;

    BOOST_URL_DECL
    static
    void
    copy_impl(
        string_view s,
        char*& dest,
        char const* end) noexcept;
};

// iterates segments in an
// encoded segment range
template<class FwdIt>
class enc_segs_iter
    : public any_path_iter
    , public enc_segs_iter_base
{
    FwdIt it_;
    FwdIt end_;

public:
    enc_segs_iter(
        FwdIt first,
        FwdIt last) noexcept
        : it_(first)
        , end_(last)
    {
        front = *first;
    }

    bool
    measure(
        std::size_t& n,
        error_code& ec
            ) noexcept override
    {
        if(it_ == end_)
            return false;
        if(! measure_impl(
                *it_, n, ec))
            return false;
        ++it_;
        return true;
    }

    void
    copy(
        char*& dest,
        char const* end
            ) noexcept override
    {
        copy_impl(*it_,
            dest, end);
        ++it_;
    }
};

//------------------------------------------------

class plain_segs_iter_base
{
protected:
    BOOST_URL_DECL
    static
    void
    measure_impl(
        string_view s,
        std::size_t& n) noexcept;

    BOOST_URL_DECL
    static
    void
    copy_impl(
        string_view s,
        char*& dest,
        char const* end) noexcept;
};

// iterates segments in a
// plain segment range
template<class FwdIt>
class plain_segs_iter
    : public any_path_iter
    , public plain_segs_iter_base
{
    FwdIt it_;
    FwdIt end_;

public:
    plain_segs_iter(
        FwdIt first,
        FwdIt last) noexcept
        : it_(first)
        , end_(last)
    {
        front = *first;
    }

    bool
    measure(
        std::size_t& n,
        error_code&
            ) noexcept override
    {
        if(it_ == end_)
            return false;
        measure_impl(*it_, n);
        ++it_;
        return true;
    }

    void
    copy(
        char*& dest,
        char const* end
            ) noexcept override
    {
        copy_impl(*it_,
            dest, end);
        ++it_;
    }
};

//------------------------------------------------

template<class FwdIt>
enc_segs_iter<FwdIt>
make_enc_segs_iter(
    FwdIt first, FwdIt last)
{
    return enc_segs_iter<FwdIt>(
        first, last);
}

template<class FwdIt>
plain_segs_iter<FwdIt>
make_plain_segs_iter(
    FwdIt first, FwdIt last)
{
    return plain_segs_iter<FwdIt>(
        first, last);
}

} // detail
} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_ANY_QUERY_ITER_HPP
#define BOOST_URL_DETAIL_ANY_QUERY_ITER_HPP

#include <cstddef>

namespace boost {
namespace urls {
namespace detail {

struct BOOST_SYMBOL_VISIBLE
    any_query_iter
{
    BOOST_URL_DECL
    virtual
    ~any_query_iter() noexcept = 0;

    virtual
    bool
    measure(
        std::size_t& n,
        error_code& ec) noexcept = 0;

    virtual
    void
    copy(
        char*& dest,
        char const* end) noexcept = 0;
};

//------------------------------------------------

// iterates params in an
// encoded query string
class enc_query_iter
    : public any_query_iter
{
    char const* p_;
    char const* end_;
    std::size_t n_;

    void
    increment() noexcept;

public:
    explicit
    enc_query_iter(
        string_view s) noexcept;

    bool
    measure(
        std::size_t& n,
        error_code& ec) noexcept override;

    void
    copy(
        char*& dest,
        char const* end) noexcept override;
};

//------------------------------------------------

// iterates params in a
// plain query string
class plain_query_iter
    : public any_query_iter
{
    char const* p_;
    char const* end_;
    std::size_t n_;

    void
    increment() noexcept;

public:
    explicit
    plain_query_iter(
        string_view s) noexcept;

    bool
    measure(
        std::size_t& n,
        error_code& ec) noexcept override;

    void
    copy(
        char*& dest,
        char const* end) noexcept override;
};

//------------------------------------------------

class enc_params_iter_base
{
protected:
    BOOST_URL_DECL
    static
    bool
    measure_impl(
        string_view key,
        string_view const* value,
        std::size_t& n,
        error_code& ec) noexcept;

    BOOST_URL_DECL
    static
    void
    copy_impl(
        string_view key,
        string_view const* value,
        char*& dest,
        char const* end) noexcept;
};

// iterates params in an
// encoded params range
template<class FwdIt>
class enc_params_iter
    : public any_query_iter
    , public enc_params_iter_base
{
    FwdIt it_;
    FwdIt end_;

public:
    enc_params_iter(
        FwdIt first,
        FwdIt last) noexcept
        : it_(first)
        , end_(last)
    {
    }

    bool
    measure(
        std::size_t& n,
        error_code& ec
            ) noexcept override
    {
        if(it_ == end_)
            return false;
        query_param_view v(*it_++);
        if(v.has_value)
            measure_impl(v.key,
                &v.value, n, ec);
        else
            measure_impl(v.key,
                nullptr, n, ec);
        return ! ec.failed();
    }

    void
    copy(
        char*& dest,
        char const* end
            ) noexcept override
    {
        query_param_view v(*it_++);
        if(v.has_value)
            copy_impl(v.key,
                &v.value, dest, end);
        else
            copy_impl(v.key,
                nullptr, dest, end);
    }
};

//------------------------------------------------

class plain_params_iter_base
{
protected:
    BOOST_URL_DECL
    static
    void
    measure_impl(
        string_view key,
        string_view const* value,
        std::size_t& n) noexcept;

    BOOST_URL_DECL
    static
    void
    copy_impl(
        string_view key,
        string_view const* value,
        char*& dest,
        char const* end) noexcept;
};

// iterates params in a
// decoded params range
template<class FwdIt>
class plain_params_iter
    : public any_query_iter
    , public plain_params_iter_base
{
    FwdIt it_;
    FwdIt end_;

public:
    plain_params_iter(
        FwdIt first,
        FwdIt last) noexcept
        : it_(first)
        , end_(last)
    {
    }

    bool
    measure(
        std::size_t& n,
        error_code&
            ) noexcept override;

    void
    copy(
        char*& dest,
        char const* end
            ) noexcept override;
};

class plain_value_iter_base
{
protected:
    BOOST_URL_DECL
    static
    void
    measure_impl(
        string_view key,
        string_view const* value,
        std::size_t& n) noexcept;

    BOOST_URL_DECL
    static
    void
    copy_impl(
        string_view key,
        string_view const* value,
        char*& dest,
        char const* end) noexcept;
};

// iterates params in a
// decoded params range
// decoding values only
template<class FwdIt>
class plain_value_iter
    : public any_query_iter
    , public plain_value_iter_base
{
    FwdIt it_;
    FwdIt end_;

public:
    plain_value_iter(
        FwdIt first,
        FwdIt last) noexcept
        : it_(first)
        , end_(last)
    {
    }

    bool
    measure(
        std::size_t& n,
        error_code&
            ) noexcept override;

    void
    copy(
        char*& dest,
        char const* end
            ) noexcept override;
};

//------------------------------------------------

template<class FwdIt>
enc_params_iter<FwdIt>
make_enc_params_iter(
    FwdIt first, FwdIt last)
{
    return enc_params_iter<FwdIt>(
        first, last);
}

template<class FwdIt>
plain_params_iter<FwdIt>
make_plain_params_iter(
    FwdIt first, FwdIt last)
{
    return plain_params_iter<FwdIt>(
        first, last);
}

template<class FwdIt>
plain_value_iter<FwdIt>
make_plain_value_iter(
    FwdIt first, FwdIt last)
{
    return plain_value_iter<FwdIt>(
        first, last);
}

} // detail
} // urls
} // boost

// VFALCO This include is at the bottom of
// url.hpp because of a circular dependency
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_IMPL_ANY_QUERY_ITER_HPP
#define BOOST_URL_DETAIL_IMPL_ANY_QUERY_ITER_HPP


namespace boost {
namespace urls {
namespace detail {

template<class FwdIt>
bool
plain_params_iter<FwdIt>::
measure(
    std::size_t& n,
    error_code&) noexcept
{
    if(it_ == end_)
        return false;
    query_param_view v(*it_++);
    if(v.has_value)
        measure_impl(
            v.key, &v.value, n);
    else
        measure_impl(
            v.key, nullptr, n);
    return true;
}

template<class FwdIt>
void
plain_params_iter<FwdIt>::
copy(
    char*& dest,
    char const* end) noexcept
{
    query_param_view v(*it_++);
    if(v.has_value)
        copy_impl(
            v.key, &v.value,
                dest, end);
    else
        copy_impl(
            v.key, nullptr,
                dest, end);
}

template<class FwdIt>
bool
plain_value_iter<FwdIt>::
measure(
    std::size_t& n,
    error_code&) noexcept
{
    if(it_ == end_)
        return false;
    query_param_view v(*it_++);
    if(v.has_value)
        measure_impl(
            v.key, &v.value, n);
    else
        measure_impl(
            v.key, nullptr, n);
    return true;
}

template<class FwdIt>
void
plain_value_iter<FwdIt>::
copy(
    char*& dest,
    char const* end) noexcept
{
    query_param_view v(*it_++);
    if(v.has_value)
        copy_impl(
            v.key, &v.value,
                dest, end);
    else
        copy_impl(
            v.key, nullptr,
                dest, end);
}

} // detail
} // urls
} // boost

#endif

#endif
#include <cstdint>
#include <iosfwd>
#include <memory>
#include <string>
#include <utility>

namespace boost {
namespace urls {

#ifndef BOOST_URL_DOCS
class url_view;
namespace detail {
struct any_path_iter;
}
#endif

/** A modifiable container for a URL.

    Objects of this type hold URLs which may be
    inspected and modified. The derived class
    is responsible for providing storage.

    The underlying string stored in the container
    is always null-terminated.

    @par Exception Safety

    @li Functions marked `noexcept` provide the
    no-throw guarantee, otherwise:

    @li Functions which throw offer the strong
    exception safety guarantee.

    @par BNF
    @code
    URI-reference = URI / relative-ref

    URI           = scheme ":" hier-part [ "?" query ] [ "#" fragment ]

    relative-ref  = relative-part [ "?" query ] [ "#" fragment ]

    absolute-URI  = scheme ":" hier-part [ "?" query ]
    @endcode

    @par Specification
    @li <a href="https://tools.ietf.org/html/rfc3986"
        >Uniform Resource Identifier (URI): Generic Syntax (rfc3986)</a>

    @see
        @ref parse_absolute_uri,
        @ref parse_relative_ref,
        @ref parse_uri,
        @ref parse_uri_reference,
        @ref resolve.
*/
class BOOST_SYMBOL_VISIBLE url
    : public url_view
{
    friend class urls::segments;
    friend class urls::params;
    friend class segments_encoded;
    friend class params_encoded;

#ifndef BOOST_URL_DOCS
protected:
#endif
    char* s_ = nullptr;
    std::size_t cap_ = 0;

    BOOST_URL_DECL
    url(char* buf,
        std::size_t cap) noexcept;

    BOOST_URL_DECL
    void
    copy(url_view const& u);

    BOOST_URL_DECL
    virtual
    char*
    allocate(
        std::size_t new_cap);

    BOOST_URL_DECL
    virtual
    void
    deallocate(char* s);

public:
    //--------------------------------------------
    //
    // Special Members
    //
    //--------------------------------------------

    /** Destructor

        Any params, segments, or iterators
        which reference this object are
        invalidated.
    */
    BOOST_URL_DECL
    virtual
    ~url();

    /** Constructor

        Default constructed urls contain
        a zero-length string. This matches
        the grammar for a relative-ref with
        an empty path and no query or
        fragment.

        @par BNF
        @code
        relative-ref  = relative-part [ "?" query ] [ "#" fragment ]
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-4.2"
            >4.2. Relative Reference (rfc3986)</a>
    */
    BOOST_URL_DECL
    url() noexcept;

    /** Constructor

        This function performs a move-construction
        from `u`. After the move. the state of `u`
        will be as-if default constructed.

        @par Exception Safety
        Throws nothing.

        @param u The url to construct from.
    */
    BOOST_URL_DECL
    url(url&& u) noexcept;

    /** Constructor

        This function constructs a copy of `u`.

        @param u The url to construct from.
    */
    BOOST_URL_DECL
    url(url const& u);

    /** Constructor

        This function constructs a copy of `u`.

        @param u The url to construct from.
    */
    BOOST_URL_DECL
    url(url_view const& u);

    /** Assignment

        This function performs a move-assignment
        from `u`. After the move. the state of `u`
        will be as-if default constructed.

        @par Exception Safety
        Throws nothing.

        @param u The url to assign from.
    */
    BOOST_URL_DECL
    url&
    operator=(url&& u) noexcept;

    /** Assignment

        This function assigns a copy of `u`
        to `*this`.

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.

        @param u The url to copy.
    */
    url&
    operator=(url const& u)
    {
        *this = u.base();
        return *this;
    }

    /** Assignment

        This function assigns a copy of `u`
        to `*this`.

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.

        @param u The url to copy.
    */
    BOOST_URL_DECL
    url&
    operator=(url_view const& u);

    /** Construct from a string

        This function constructs a URL from
        the string `s`, which must contain a
        valid URI or <em>relative-ref</em> or
        else an exception is thrown.

        @par BNF
        @code
        URI           = scheme ":" hier-part [ "?" query ] [ "#" fragment ]

        relative-ref  = relative-part [ "?" query ] [ "#" fragment ]
        @endcode

        @throw std::invalid_argument parse error.

        @param s The string to parse.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-4.1"
            >4.1. URI Reference</a>
    */
    BOOST_URL_DECL
    url(string_view s);

    //--------------------------------------------

    /** Return the encoded URL as a null-terminated string
    */
    char const*
    c_str() const noexcept
    {
        return cs_;
    }

    /** Returns the total number of bytes currently available to the container
    */
    std::size_t
    capacity_in_bytes() const noexcept
    {
        return cap_;
    }

    /** Adjust the capacity without changing the size

        This function adjusts the capacity
        of the container in bytes, without
        affecting the current contents.

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.

        @throw bad_alloc Allocation failure

        @par bytes The number of bytes to reserve.
    */
    BOOST_URL_DECL
    void
    reserve_bytes(
        std::size_t bytes);

    /** Clear the contents.
    
        @par Exception Safety
        No-throw guarantee.
    */
    BOOST_URL_DECL
    void
    clear() noexcept;

    //--------------------------------------------
    //
    // Scheme
    //
    //--------------------------------------------

private:
    void set_scheme_impl(string_view s, urls::scheme id);
public:

    /** Remove the scheme

        This function removes the scheme if it
        is present.

        @par BNF
        @code
        URI           = scheme ":" hier-part [ "?" query ] [ "#" fragment ]
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.1">
            3.1. Scheme (rfc3986)</a>

        @see @ref set_scheme.
    */
    BOOST_URL_DECL
    url&
    remove_scheme() noexcept;

    /** Set the scheme

        This function sets the scheme to the specified
        string, which must contain a valid scheme
        without a trailing colon (':'). If the scheme
        is invalid, an exception is thrown.

        @par Example
        @code
        url u = parse_uri( "http://www.example.com" );

        u.set_scheme( "https" );         // u is now "https://www.example.com"

        assert( u.string() == "https://www.example.com" );

        u.set_scheme( "1forall");       // throws, invalid scheme
        @endcode

        @par BNF
        @code
        scheme        = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
        @endcode

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.

        @param s The scheme to set.

        @throw std::invalid_argument invalid scheme.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.1">
            3.1. Scheme (rfc3986)</a>

        @see
            @ref remove_scheme.
    */
    BOOST_URL_DECL
    url&
    set_scheme(string_view s);

    /** Set the scheme

        This function sets the scheme to the specified
        known @ref urls::scheme id, which may not be
        @ref scheme::unknown or else an exception is
        thrown. If the id is @ref scheme::none, this
        function behaves as if @ref remove_scheme
        were called.

        @par Example
        @code
        url u;
        u.set_scheme( scheme::http );           // produces "http:"
        u.set_scheme( scheme::none );           // produces ""
        u.set_scheme( scheme::unknown);         // throws, invalid scheme
        @endcode

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.

        @param id The scheme to set.

        @throw std::invalid_argument invalid scheme.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.1">
            3.1. Scheme (rfc3986)</a>
    */
    BOOST_URL_DECL
    url&
    set_scheme(urls::scheme id);

    //--------------------------------------------
    //
    // Authority
    //
    //--------------------------------------------

private:
    char* set_user_impl(std::size_t n);
public:

    /** Set the user.

        The user is set to the specified string,
        replacing any previous user. If a userinfo
        was not present it is added, even if the
        user string is empty. The resulting URL
        will have an authority if it did not have
        one previously.

        Any special or reserved characters in the
        string are automatically percent-encoded.

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.

        @param s The string to set. This string may
        contain any characters, including nulls.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.1">
            3.2.1. User Information (rfc3986)</a>
    */
    BOOST_URL_DECL
    url&
    set_user(
        string_view s);

    /** Set the user.

        The user is set to the specified string,
        replacing any previous user. If a userinfo
        was not present it is added, even if the
        user string is empty. The resulting URL
        will have an authority if it did not have
        one previously.

        The string must be a valid percent-encoded
        string for the user field, otherwise an
        exception is thrown.

        @par BNF
        @code
        user          = *( unreserved / pct-encoded / sub-delims )
        @endcode

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.

        @param s The string to set.
    */
    BOOST_URL_DECL
    url&
    set_encoded_user(
        string_view s);

private:
    char* set_password_impl(std::size_t n);
public:

    /** Remove the password
    */
    BOOST_URL_DECL
    url&
    remove_password() noexcept;

    /** Set the password.

        This function sets the password to the specified
        string, replacing any previous password:

        @li If the string is empty, the password is
        cleared, and the first occurring colon (':') is
        removed from the userinfo if present, otherwise

        @li If ths string is not empty then the password
        is set to the new string.
        Any special or reserved characters in the
        string are automatically percent-encoded.
        If the URL previously did not have an authority
        (@ref has_authority returns `false`), a double
        slash ("//") is prepended to the userinfo.

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param s The string to set. This string may
        contain any characters, including nulls.
    */
    BOOST_URL_DECL
    url&
    set_password(
        string_view s);

    /** Set the password.

        The password is set to the encoded string `s`,
        replacing any previous password:

        @li If the string is empty, the password is
        cleared, and the first occurring colon (':') is
        removed from the userinfo if present, otherwise

        @li If ths string is not empty then the password
        is set to the new string.
        If the URL previously did not have an authority
        (@ref has_authority returns `false`), a double
        slash ("//") is prepended to the userinfo.
        The string must meet the syntactic requirements
        of <em>password</em> otherwise an exception is
        thrown.

        @par ANBF
        @code
        password      = *( unreserved / pct-encoded / sub-delims / ":" )
        @endcode

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param s The string to set.
    */
    BOOST_URL_DECL
    url&
    set_encoded_password(
        string_view s);

private:
    char* set_userinfo_impl(std::size_t n);
public:

    /** Remove the userinfo
    */
    BOOST_URL_DECL
    url&
    remove_userinfo() noexcept;

    /** Set the userinfo.

        Sets the userinfo of the URL to the given
        encoded string:

        @li If the string is empty, the userinfo is
        cleared, else

        @li If the string is not empty, then the userinfo
        is set to the given string. The user is set to
        the characters up to the first colon if any,
        while the password is set to the remaining
        characters if any.
        If the URL previously did not have an authority
        (@ref has_authority returns `false`), a double
        slash ("//") is prepended to the userinfo.
        The string must meet the syntactic requirements
        of <em>userinfo</em> otherwise an exception is
        thrown.

        @par BNF
        @code
        userinfo      = [ [ user ] [ ':' password ] ]

        user          = *( unreserved / pct-encoded / sub-delims )
        password      = *( unreserved / pct-encoded / sub-delims / ":" )
        @endcode

        @par Exception Safety

        Strong guarantee.
        Calls to allocate may throw.

        @param s The string to set.
    */
    BOOST_URL_DECL
    url&
    set_encoded_userinfo(
        string_view s);

    /** Set the userinfo.

        The userinfo is set to the specified string,
        replacing any previous userinfo. If a userinfo
        was not present it is added, even if the
        userinfo string is empty. The resulting URL
        will have an authority if it did not have
        one previously.

        Any special or reserved characters in the
        string are automatically percent-encoded.

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.

        @param s The string to set. This string may
        contain any characters, including nulls.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.1">
            3.2.1. User Information (rfc3986)</a>
    */
    BOOST_URL_DECL
    url&
    set_userinfo(
        string_view s);

    //--------------------------------------------

private:
    char* set_host_impl(std::size_t n);
public:

    /** Set the host

        The host is set to the specified IPv4,
        address, replacing any previous host. If
        an authority was not present, it is added.

        @par Postconditions
        @code
        this->host_type() == host_type::ipv4 && this->ipv4_address() == addr
        @endcode

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.2">
            3.2.2. Host (rfc3986)</a>

        @param addr The address to set.
    */
#ifdef BOOST_URL_DOCS
    url& set_host( ipv4_address const& addr );
#else
    BOOST_URL_DECL
    url&
    set_host(
        urls::ipv4_address const& addr);
#endif

    /** Set the host

        The host is set to the specified IPv6,
        address, replacing any previous host.
        If an authority did not
        previously exist it is added by prepending
        a double slash ("//") at the beginning of
        the URL or after the scheme if a scheme is
        present.

        @par Postconditions
        @code
        this->host_type() == host_type::ipv6 && this->ipv6_address() == addr
        @endcode

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.2">
            3.2.2. Host (rfc3986)</a>

        @param addr The address to set.
    */
#ifdef BOOST_URL_DOCS
    url& set_host( urls::ipv6_address const& addr );
#else
    BOOST_URL_DECL
    url&
    set_host(
        urls::ipv6_address const& addr);
#endif

    /** Set the host

        The host is set to the specified plain
        string, subject to the following rules:

        @li If the string is a valid IPv4 address,
        the address is parsed and the host is set
        as if an instance of the equivalent
        @ref urls::ipv4_address were passed instead.
        In this case @ref url::host_type will return
        @ref host_type::ipv4. Otherwise,

        @li The plain string is percent-encoded and
        the result is set as the reg-name for the
        host. In this case @ref url::host_type will
        return @ref host_type::name.

        In all cases, if an authority did not
        previously exist it is added by prepending
        a double slash ("//") at the beginning of
        the URL or after the scheme if a scheme is
        present.

        @par Postconditions
        @code
        this->encoded_host() == s
        @endcode

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.2">
            3.2.2. Host (rfc3986)</a>

        @param s The string to set.
    */
    BOOST_URL_DECL
    url&
    set_host(
        string_view s);

    /** Set the host

        The host is set to the specified percent-
        encoded string, subject to the following
        rules:

        @li If the string is a valid IPv4 address,
        the address is parsed and the host is set
        as if an instance of the equivalent
        @ref urls::ipv4_address were passed instead. In
        this case @ref url::host_type will return
        @ref host_type::ipv4. Or,

        @li If the string is a valid IPv6 address
        enclosed in square brackets ('[' and ']'),
        the address is parsed and the host is set
        as if an instance of the equivalent
        @ref urls::ipv6_address were passed instead. In
        this case @ref url::host_type will return
        @ref host_type::ipv4. Or,

        @li If the string is a valid IPvFuture address
        enclosed in square brackets ('[' and ']'),
        the address is parsed and the host is set
        to the specified string. In this case
        @ref url::host_type will return
        @ref host_type::ipvfuture. Or,

        @li If the string is a valid percent-encoded
        string with no characters from the reserved
        character set, then it is set as the encoded
        host name. In this case @ref url::host_type
        will return @ref host_type::name. Otherwise,

        @li If the string does not contain a valid
        percent-encoding for the host field, an
        exception is thrown.

        In all cases, if an authority did not
        previously exist it is added by prepending
        a double slash ("//") at the beginning of
        the URL or after the scheme if a scheme is
        present.
        
        @par Postconditions
        @code
        this->encoded_host() == s
        @endcode

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.2">
            3.2.2. Host (rfc3986)</a>

        @param s The string to set.

        @throw std::invalid_argument the percent-encoding is invalid
    */
    BOOST_URL_DECL
    url&
    set_encoded_host(string_view s);

private:
    char* set_port_impl(std::size_t n);
public:

    /** Remove the port

        If a port is present, it is removed.
        The remainder of the authority component
        is left unchanged including the leading
        double slash ("//").

        @par Postconditions
        @code
        this->has_port() == false && this->port_number() == 0 && this->port() == ""
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.3">
            3.2.3. Port (rfc3986)</a>
    */
    BOOST_URL_DECL
    url&
    remove_port() noexcept;

    /** Set the port

        The port of the URL is set to the specified
        integer, replacing any previous port.
        If an authority did not
        previously exist it is added by prepending
        a double slash ("//") at the beginning of
        the URL or after the scheme if a scheme is
        present.

        @par Postconditions
        @code
        this->has_port() == true && this->port_number() == n && this->port() == std::to_string(n)
        @endcode

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.3">
            3.2.3. Port (rfc3986)</a>

        @param n The port number to set.
    */
    BOOST_URL_DECL
    url&
    set_port(std::uint16_t n);

    /** Set the port

        The port of the URL is set to the specified
        string, replacing any previous port. The string
        must meet the syntactic requirements for PORT,
        which consists only of digits. The string
        may be empty. In this case the port is still
        defined, however it is the empty string. To
        remove the port call @ref remove_port.
        If an authority did not
        previously exist it is added by prepending
        a double slash ("//") at the beginning of
        the URL or after the scheme if a scheme is
        present.

        @par Postconditions
        @code
        this->has_port() == true && this->port_number() == n && this->port() == std::to_string(n)
        @endcode

        @par BNF
        @code
        port          = *DIGIT
        @endcode

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.3">
            3.2.3. Port (rfc3986)</a>

        @param s The port string to set.

        @throw std::invalid_argument `s` is not a valid port string.
    */
    BOOST_URL_DECL
    url&
    set_port(string_view s);

    //--------------------------------------------

    /** Remove the authority

        The full authority component is removed
        if present, which includes the leading
        double slashes ("//"), the userinfo,
        the host, and the port.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2">
            3.2. Authority (rfc3986)</a>
    */
    BOOST_URL_DECL
    url&
    remove_authority() noexcept;

    /** Set the authority

        This function sets the authority component
        to the specified encoded string. If a
        component was present it is replaced.
        Otherwise, the authority is added
        including leading double slashes ("//").

        The encoded string must be a valid
        authority or else an exception is thrown.

        @par BNF
        @code
        authority     = [ userinfo "@" ] host [ ":" port ]

        userinfo      = *( unreserved / pct-encoded / sub-delims / ":" )
        host          = IP-literal / IPv4address / reg-name
        port          = *DIGIT
        @endcode

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2">
            3.2. Authority (rfc3986)</a>

        @param s The authority string to set.

        @throw std::invalid_argument `s` is not a valid authority.
    */
    BOOST_URL_DECL
    url&
    set_encoded_authority(
        string_view s);

    //--------------------------------------------

    /** Remove the origin component

        The origin consists of the everything from the
        beginning of the URL up to but not including
        the path.

        @par Exception Safety
        Throws nothing.
    */
    BOOST_URL_DECL
    url&
    remove_origin() noexcept;

    //--------------------------------------------
    //
    // Path
    //
    //--------------------------------------------

private:
    pos_t
    segment(
        std::size_t i) const noexcept;

    char*
    edit_segments(
        std::size_t i0,
        std::size_t i1,
        std::size_t n,
        std::size_t nseg);

    BOOST_URL_DECL
    void
    edit_segments(
        std::size_t i0,
        std::size_t i1,
        detail::any_path_iter&& it0,
        detail::any_path_iter&& it1,
        int abs_hint = -1);
public:

    /** Set whether the path is absolute.

        This modifies the path as needed to
        make it absolute or relative.

        @return true on success.
    */
    BOOST_URL_DECL
    bool
    set_path_absolute(bool absolute);

    /** Set the path.

        This function validates the given percent-encoded
        path according to the allowed grammar based
        on the existing contents of the URL. If the
        path is valid, the old path is replaced with
        the new path. Otherwise, an exception is
        thrown. The requirements for `s` are thus:

        @li If `s` matches <em>path-empty</em>, that is
        if `s.empty() == true`, the path is valid. Else,

        @li If an authority is present (@ref has_authority
        returns `true`), the path syntax must match
        <em>path-abempty</em>. Else, if there is no
        authority then:

        @li If the new path starts with a forward
        slash ('/'), the path syntax must match
        <em>path-absolute</em>. Else, if the
        path is rootless (does not start with '/'),
        then:

        @li If a scheme is present, the path syntax
        must match <em>path-rootless</em>, otherwise

        @li The path syntax must match <em>path-noscheme</em>.

        @par BNF
        @code
        path-abempty  = *( "/" segment )
        path-absolute = "/" [ segment-nz *( "/" segment ) ]
        path-noscheme = segment-nz-nc *( "/" segment )
        path-rootless = segment-nz *( "/" segment )
        path-empty    = 0<pchar>
        @endcode

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.

        @param s The string to set.

        @throws std::invalid_argument invalid path.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.3"
            >3.3. Path (rfc3986)</a>
    */
    BOOST_URL_DECL
    url&
    set_encoded_path(
        string_view s);

    /** Set the path.

        Sets the path of the URL to the specified
        plain string. Any reserved characters in the
        string will be automatically percent-encoded.

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.

        @param s The string to set. This string may
        contain any characters, including nulls.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.3"
            >3.3. Path (rfc3986)</a>

        @see
            @ref encoded_path,
            @ref encoded_segments,
            @ref is_path_absolute,
            @ref normalize_path,
            @ref set_encoded_path,
            @ref set_path_absolute.
    */
    BOOST_URL_DECL
    url&
    set_path(
        string_view s);

    /** Return the path segments

        This function returns the path segments as
        a bidirectional range.

        @par BNF
        @code
        path          = [ "/" ] segment *( "/" segment )
        @endcode

        @par Exception Safety
        Throws nothing.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.3"
            >3.3. Path (rfc3986)</a>
    */
    BOOST_URL_DECL
    segments_encoded
    encoded_segments() noexcept;

    /** Return the path segments

        This function returns the path segments as
        a bidirectional range.

        @par BNF
        @code
        path          = [ "/" ] segment *( "/" segment )
        @endcode

        @par Exception Safety
        Throws nothing.

        @param alloc An optional allocator the
        container will use when returning
        percent-decoded strings. If omitted,
        the default allocator is used.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.3"
            >3.3. Path (rfc3986)</a>
    */
    template<class Allocator =
        std::allocator<char>>
    urls::segments
    segments(Allocator const& alloc = {}) noexcept
    {
        return urls::segments(*this, alloc);
    }

    //--------------------------------------------
    //
    // Query
    //
    //--------------------------------------------

private:
    struct raw_param
    {
        std::size_t pos;
        std::size_t nk;
        std::size_t nv;
    };

    raw_param
    param(
        std::size_t i) const noexcept;

    char*
    edit_params(
        std::size_t i0,
        std::size_t i1,
        std::size_t n,
        std::size_t nparam);

    BOOST_URL_DECL
    void
    edit_params(
        std::size_t i0,
        std::size_t i1,
        detail::any_query_iter&& it0,
        detail::any_query_iter&& it1,
        bool set_hint = false);
public:

    /** Remove the query.

        If a query is present (@ref has_query
        returns `true`), then the query is
        removed including the leading `?`.

        @par Exception Safety
        Throws nothing.

        @see
            @ref has_query,
            @ref set_encoded_query,
            @ref set_query.
    */
    BOOST_URL_DECL
    url&
    remove_query() noexcept;

    /** Set the query.

        Sets the query of the URL to the specified
        encoded string. The string must contain a
        valid percent-encoding or else an exception
        is thrown. When this function returns,
        @ref has_query will return `true`.

        @par BNF
        @code
        query           = *( pchar / "/" / "?" )
        @endcode

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.

        @param s The string to set.

        @throws std::invalid_argument bad encoding.

        @see
            @ref has_query,
            @ref remove_query,
            @ref set_query.
    */
    BOOST_URL_DECL
    url&
    set_encoded_query(
        string_view s);

    /** Set the query.

        Sets the query of the URL to the specified
        plain string. Any reserved characters in the
        string will be automatically percent-encoded.
        When this function returns, @ref has_query
        will return `true`.

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.

        @param s The string to set. This string may
        contain any characters, including nulls.

        @see
            @ref has_query,
            @ref remove_query,
            @ref set_encoded_query.
    */
    BOOST_URL_DECL
    url&
    set_query(
        string_view s);

    /** Return the query parameters

        This function returns the query
        parameters as a forward range of
        key/value pairs.

        Each string returned by the container
        is percent-encoded.

        @par BNF
        @code
        query-params    = [ query-param ] *( "&" [ query-param ] )

        query-param     = key [ "=" value ]

        @endcode
    */
    urls::params_encoded
    encoded_params() noexcept
    {
        return urls::params_encoded(*this);
    }

    /** Return the query parameters

        This function returns the query
        parameters as a forward range of
        key/value pairs where each returned
        string has percent-decoding applied.

        @par BNF
        @code
        query-params    = [ query-param ] *( "&" [ query-param ] )

        query-param     = key [ "=" value ]
        @endcode

        @param alloc An optional allocator the
        container will use when returning
        percent-decoded strings. If omitted,
        the default allocator is used.
    */
    template<class Allocator =
        std::allocator<char>>
    urls::params
    params(Allocator const& alloc = {})
    {
        return urls::params(*this, alloc);
    }

    //--------------------------------------------
    //
    // Fragment
    //
    //--------------------------------------------

private:
    char* set_fragment_impl(std::size_t n);
public:

    /** Remove the fragment.

        If a fragment is present (@ref has_fragment
        returns `true`), then the fragment is
        removed including the leading `#`.

        @par Exception Safety
        Throws nothing.

        @see
            @ref has_fragment,
            @ref set_encoded_fragment,
            @ref set_fragment.
    */
    BOOST_URL_DECL
    url&
    remove_fragment() noexcept;

    /** Set the fragment.

        Sets the fragment of the URL to the specified
        encoded string. The string must contain a
        valid percent-encoding or else an exception
        is thrown. When this function returns,
        @ref has_fragment will return `true`.

        @par BNF
        @code
        fragment      = *( pchar / "/" / "?" )
        @endcode

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.

        @param s The string to set.

        @throws std::invalid_argument bad encoding.

        @see
            @ref has_fragment,
            @ref remove_fragment,
            @ref set_fragment.
    */
    BOOST_URL_DECL
    url&
    set_encoded_fragment(
        string_view s);

    /** Set the fragment.

        Sets the fragment of the URL to the specified
        plain string. Any reserved characters in the
        string will be automatically percent-encoded.
        When this function returns, @ref has_fragment
        will return `true`.

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.

        @param s The string to set. This string may
        contain any characters, including nulls.

        @see
            @ref has_fragment,
            @ref remove_fragment,
            @ref set_encoded_fragment.
    */
    BOOST_URL_DECL
    url&
    set_fragment(
        string_view s);

    //--------------------------------------------
    //
    // Normalization
    //
    //--------------------------------------------
private:
    void
    normalize_octets_impl(
        int id,
        grammar::lut_chars const& cs) noexcept;

    void
    decoded_to_lower_impl(int id) noexcept;

    void
    to_lower_impl(int id) noexcept;
public:

    /** Normalize the URL components

        Applies Syntax-based normalization to
        all components of the URL.

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-6.2.2"
            >6.2.2 Syntax-Based Normalization (rfc3986)</a>

    */
    BOOST_URL_DECL
    url&
    normalize();

    /** Normalize the URL scheme

        Applies Syntax-based normalization to the
        URL scheme.

        The scheme is normalized to lowercase.

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-6.2.2"
            >6.2.2 Syntax-Based Normalization (rfc3986)</a>

    */
    BOOST_URL_DECL
    url&
    normalize_scheme();

    /** Normalize the URL authority

        Applies Syntax-based normalization to the
        URL authority.

        Percent-encoding triplets are normalized
        to uppercase letters. Percent-encoded
        octets that correspond to unreserved
        characters are decoded.

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-6.2.2"
            >6.2.2 Syntax-Based Normalization (rfc3986)</a>

    */
    BOOST_URL_DECL
    url&
    normalize_authority();

    /** Normalize the URL path

        Applies Syntax-based normalization to the
        URL path.

        Percent-encoding triplets are normalized
        to uppercase letters. Percent-encoded
        octets that correspond to unreserved
        characters are decoded. Redundant
        path-segments are removed.

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-6.2.2"
            >6.2.2 Syntax-Based Normalization (rfc3986)</a>

    */
    BOOST_URL_DECL
    url&
    normalize_path();

    /** Normalize the URL query

        Applies Syntax-based normalization to the
        URL query.

        Percent-encoding triplets are normalized
        to uppercase letters. Percent-encoded
        octets that correspond to unreserved
        characters are decoded.

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-6.2.2"
            >6.2.2 Syntax-Based Normalization (rfc3986)</a>

    */
    BOOST_URL_DECL
    url&
    normalize_query();

    /** Normalize the URL fragment

        Applies Syntax-based normalization to the
        URL fragment.

        Percent-encoding triplets are normalized
        to uppercase letters. Percent-encoded
        octets that correspond to unreserved
        characters are decoded.

        @par Exception Safety
        Strong guarantee.
        Calls to allocate may throw.

        @par Specification
        @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-6.2.2"
            >6.2.2 Syntax-Based Normalization (rfc3986)</a>

    */
    BOOST_URL_DECL
    url&
    normalize_fragment();

    //--------------------------------------------
    //
    // Resolution
    //
    //--------------------------------------------

    friend
    void
    resolve(
        url_view const& base,
        url_view const& ref,
        url& dest,
        error_code& ec);

private:
    //--------------------------------------------
    //
    // implementation
    //
    //--------------------------------------------

    friend class static_url_base;

    void check_invariants() const noexcept;
    void build_tab() noexcept;

    void
    ensure_space(
        std::size_t nchar,
        std::size_t nseg,
        std::size_t nparam);

    char*
    resize_impl(
        int id,
        std::size_t new_size);

    char*
    resize_impl(
        int first,
        int last,
        std::size_t new_size);

    char*
    shrink_impl(
        int id,
        std::size_t new_size);

    char*
    shrink_impl(
        int first,
        int last,
        std::size_t new_size);

    BOOST_URL_DECL
    bool
    resolve(
        url_view const& base,
        url_view const& ref,
        error_code& ec);
};

//----------------------------------------------------------

/** Resolve a URL reference against a base URL

    This function attempts to resolve a URL
    reference `ref` against the base URL `base`
    which must satisfy the <em>absolute-URI</em>
    grammar.
    This process is defined in detail in
    rfc3986 (see below).
    The result of the resolution is placed
    into `dest`.
    If an error occurs, the contents of
    `dest` is unspecified.

    @par BNF
    @code
    absolute-URI  = scheme ":" hier-part [ "?" query ]
    @endcode

    @par Exception Safety
    Basic guarantee.
    Calls to allocate may throw.

    @param base The base URL to resolve against.

    @param ref The URL reference to resolve.

    @param dest The container where the result
    is written, upon success.

    @param ec Set to the error if any occurred.

    @par Specification
    <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-5"
        >5. Reference Resolution (rfc3986)</a>

    @see
        @ref url,
        @ref url_view.
*/
inline
void
resolve(
    url_view const& base,
    url_view const& ref,
    url& dest,
    error_code& ec)
{
    BOOST_ASSERT(&dest != &base);
    BOOST_ASSERT(&dest != &ref);
    dest.resolve(base, ref, ec);
}

/** Format the encoded url to the output stream
*/
BOOST_URL_DECL
std::ostream&
operator<<(std::ostream& os, url const& u);

} // urls
} // boost

// std::hash specialization
#ifndef BOOST_URL_DOCS
namespace std {
template<>
struct hash< ::boost::urls::url >
{
    hash() = default;
    hash(hash const&) = default;
    hash& operator=(hash const&) = default;

    explicit
    hash(std::size_t salt) noexcept
        : salt_(salt)
    {
    }

    std::size_t
    operator()(::boost::urls::url const& u) const noexcept
    {
        std::hash<::boost::urls::url_view> h(salt_);
        return h(static_cast<::boost::urls::url_view>(u));
    }

private:
    std::size_t salt_ = 0;
};
} // std
#endif



//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_PARAMS_HPP
#define BOOST_URL_IMPL_PARAMS_HPP

#include <boost/assert.hpp>

namespace boost {
namespace urls {

//------------------------------------------------

class params::iterator
{
    detail::params_iterator_impl impl_;

    friend class params;

    iterator(
        string_view s,
        const_string::factory a) noexcept
        : impl_(s, a)
    {
    }

    // end
    iterator(
        string_view s,
        std::size_t nparam,
        int,
        const_string::factory a) noexcept
        : impl_(s, nparam, 0, a)
    {
    }

    string_view
    encoded_key() const noexcept
    {
        return impl_.encoded_key();
    }

public:
    using value_type = query_param;
    using reference = params::reference;
    using pointer = void const*;
    using difference_type = std::ptrdiff_t;
    using iterator_category =
        std::forward_iterator_tag;

    iterator() = default;

    iterator&
    operator++() noexcept
    {
        impl_.increment();
        return *this;
    }

    iterator
    operator++(int) noexcept
    {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    reference
    operator*() const
    {
        params_view::reference r0 =
            impl_.dereference();
        return {
            std::move(r0.key),
            std::move(r0.value),
            r0.has_value};
    }

    friend
    bool
    operator==(
        iterator const& a,
        iterator const& b) noexcept
    {
        return a.impl_.equal(b.impl_);
    }

    friend
    bool
    operator!=(
        iterator const& a,
        iterator const& b) noexcept
    {
        return !a.impl_.equal(b.impl_);
    }
};

//------------------------------------------------
//
// Members
//
//------------------------------------------------

template<class Allocator>
params::
params(
    url& u,
    Allocator const& a)
    : u_(&u)
    , a_(a)
{
}

inline
params&
params::
operator=(std::initializer_list<
    query_param_view> init)
{
    assign(init);
    return *this;
}

inline
void
params::
assign(std::initializer_list<
    query_param_view> init)
{
    assign(init.begin(), init.end());
}

template<class FwdIt>
void
params::
assign(FwdIt first, FwdIt last)
{
    assign(first, last,
        typename std::iterator_traits<
            FwdIt>::iterator_category{});
}

template<class FwdIt>
void
params::
assign(FwdIt first, FwdIt last,
    std::forward_iterator_tag)
{
    using detail::
        make_plain_params_iter;
    u_->edit_params(
        0,
        size(),
        make_plain_params_iter(
            first, last),
        make_plain_params_iter(
            first, last));
}

//--------------------------------------------
//
// Iterators
//
//--------------------------------------------

inline
auto
params::
begin() const noexcept ->
    iterator
{
    if (u_->nparam_ > 0)
        return { u_->encoded_query(), a_ };
    return end();
}

inline
auto
params::
end() const noexcept ->
    iterator
{
    return {
        u_->encoded_query(),
        u_->nparam_,
        0, a_ };
}

//------------------------------------------------
//
// Capacity
//
//------------------------------------------------

inline
std::size_t
params::
size() const noexcept
{
    return u_->nparam_;
}

inline
bool
params::
empty() const noexcept
{
    return size() == 0;
}

//------------------------------------------------
//
// Modifiers
//
//------------------------------------------------

inline
void
params::
clear() noexcept
{
    erase(begin(), end());
}

//------------------------------------------------

inline
auto
params::
insert(
    iterator before,
    query_param_view const& v) ->
        iterator
{
    return insert(
        before, &v, &v + 1);
}

inline
auto
params::
insert(
    iterator before,
    std::initializer_list<
        query_param_view> init) ->
            iterator
{
    return insert(before,
        init.begin(), init.end());
}

template<class FwdIt>
auto
params::
insert(
    iterator before,
    FwdIt first,
    FwdIt last) ->
        iterator
{
    return insert(before, first, last,
        typename std::iterator_traits<
            FwdIt>::iterator_category{});
}

template<class FwdIt>
auto
params::
insert(
    iterator before,
    FwdIt first,
    FwdIt last,
    std::forward_iterator_tag) ->
        iterator
{
    using detail::
        make_plain_params_iter;
    BOOST_ASSERT(before.impl_.begin_ ==
        u_->encoded_query().data());
    BOOST_ASSERT(before.impl_.end_ ==
        u_->encoded_query().data() +
        u_->encoded_query().size());
    u_->edit_params(
        before.impl_.i_,
        before.impl_.i_,
        make_plain_params_iter(
            first, last),
        make_plain_params_iter(
            first, last));
    return std::next(begin(), before.impl_.i_);
}

//------------------------------------------------

inline
auto
params::
replace(
    iterator pos,
    query_param_view const& value) ->
        iterator
{
    return replace(
        pos,
        std::next(pos),
        &value,
        &value + 1);
}

template<class FwdIt>
auto
params::
replace(
    iterator from,
    iterator to,
    FwdIt first,
    FwdIt last) ->
        iterator
{
    using detail::
        make_plain_params_iter;
    BOOST_ASSERT(from.impl_.begin_ ==
        u_->encoded_query().data());
    BOOST_ASSERT(from.impl_.end_ ==
        u_->encoded_query().data() +
        u_->encoded_query().size());
    BOOST_ASSERT(to.impl_.begin_ ==
        u_->encoded_query().data());
    BOOST_ASSERT(to.impl_.end_ ==
        u_->encoded_query().data() +
        u_->encoded_query().size());
    u_->edit_params(
        from.impl_.i_,
        to.impl_.i_,
        make_plain_params_iter(
            first, last),
        make_plain_params_iter(
            first, last));
    return std::next(begin(), from.impl_.i_);
}

inline
auto
params::
replace(
    iterator from,
    iterator to,
    std::initializer_list<
        query_param_view> init) ->
    iterator
{
    return replace(
        from,
        to,
        init.begin(),
        init.end());
}

//------------------------------------------------

inline
auto
params::
emplace_at(
    iterator pos,
    string_view key,
    string_view value) ->
        iterator
{
    using detail::
        make_plain_params_iter;
    query_param_view v{
        key, value, true };
    BOOST_ASSERT(pos.impl_.begin_ ==
        u_->encoded_query().data());
    BOOST_ASSERT(pos.impl_.end_ ==
        u_->encoded_query().data() +
        u_->encoded_query().size());
    u_->edit_params(
        pos.impl_.i_,
        pos.impl_.i_ + 1,
        make_plain_params_iter(
            &v, &v + 1),
        make_plain_params_iter(
            &v, &v + 1));
    return std::next(begin(), pos.impl_.i_);
}

inline
auto
params::
emplace_at(
    iterator pos,
    string_view key) ->
        iterator
{
    BOOST_ASSERT(pos.impl_.begin_ ==
        u_->encoded_query().data());
    BOOST_ASSERT(pos.impl_.end_ ==
        u_->encoded_query().data() +
        u_->encoded_query().size());
    query_param_view v{key, {}, false};
    u_->edit_params(
        pos.impl_.i_,
        pos.impl_.i_ + 1,
        detail::make_enc_params_iter(
            &v, &v + 1),
        detail::make_enc_params_iter(
            &v, &v + 1));
    return std::next(begin(), pos.impl_.i_);
}

inline
auto
params::
emplace_before(
    iterator before,
    string_view key,
    string_view value) ->
        iterator
{
    return insert(
        before,
        query_param_view{
            key, value, true });
}

inline
auto
params::
emplace_before(
    iterator before,
    string_view key) ->
        iterator
{
    return insert(
        before,
        query_param_view{key, {}, false});
}

//------------------------------------------------

inline
auto
params::
erase(iterator pos) ->
    iterator
{
    return erase(pos, std::next(pos));
}

//------------------------------------------------

inline
auto
params::
emplace_back(
    string_view key) ->
        iterator
{
    return insert(
        end(), query_param_view{
            key, {}, false});
}

inline
auto
params::
emplace_back(
    string_view key,
    string_view value) ->
        iterator
{
    return insert(
        end(), query_param_view{
            key, value, true});
}

inline
void
params::
push_back(
    query_param_view const& v)
{
    insert(end(), v);
}

//------------------------------------------------
//
// Lookup
//
//------------------------------------------------

inline
auto
params::
find(string_view key) const noexcept ->
    iterator
{
    return find(begin(), key);
}

inline
bool
params::
contains(string_view key) const noexcept
{
    return find(key) != end();
}

} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_PARAMS_ENCODED_HPP
#define BOOST_URL_IMPL_PARAMS_ENCODED_HPP

#include <boost/assert.hpp>

namespace boost {
namespace urls {

//------------------------------------------------

class params_encoded::iterator
{

    detail::params_encoded_iterator_impl impl_;

    friend class params_encoded;

    iterator(
        string_view s) noexcept
        : impl_(s)
    {
    }

    // end
    iterator(
        string_view s,
        std::size_t nparam,
        int) noexcept
        : impl_(s, nparam, 0)
    {
    }

    string_view
    encoded_key() const noexcept
    {
        return impl_.encoded_key();
    }

public:
    using value_type = query_param;
    using reference = query_param_view;
    using pointer = void const*;
    using difference_type = std::ptrdiff_t;
    using iterator_category =
        std::forward_iterator_tag;

    iterator() = default;

    iterator&
    operator++() noexcept
    {
        impl_.increment();
        return *this;
    }

    iterator
    operator++(int) noexcept
    {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    reference
    operator*() const
    {
        return impl_.dereference();
    }

    friend
    bool
    operator==(
        iterator const& a,
        iterator const& b) noexcept
    {
        return a.impl_.equal(b.impl_);
    }

    friend
    bool
    operator!=(
        iterator const& a,
        iterator const& b) noexcept
    {
        return !a.impl_.equal(b.impl_);
    }
};

//------------------------------------------------
//
// Members
//
//------------------------------------------------

inline
params_encoded::
params_encoded(
    url& u) noexcept
    : u_(&u)
{
}

inline
params_encoded&
params_encoded::
operator=(std::initializer_list<
    reference> init)
{
    assign(init);
    return *this;
}

inline
void
params_encoded::
assign(std::initializer_list<
    reference> init)
{
    assign(init.begin(), init.end());
}

template<class FwdIt>
void
params_encoded::
assign(FwdIt first, FwdIt last)
{
    assign(first, last,
        typename std::iterator_traits<
            FwdIt>::iterator_category{});
}

template<class FwdIt>
void
params_encoded::
assign(FwdIt first, FwdIt last,
    std::forward_iterator_tag)
{
    using detail::
        make_enc_params_iter;
    u_->edit_params(
        0,
        size(),
        make_enc_params_iter(
            first, last),
        make_enc_params_iter(
            first, last));
}

template<class Allocator>
params
params_encoded::
decoded(Allocator const& alloc) const
{
    return params(*u_, alloc);
}

//------------------------------------------------
//
// Element Access
//
//------------------------------------------------

inline
auto
params_encoded::
at(std::size_t pos) const ->
    reference
{
    if(pos >= size())
        detail::throw_out_of_range(
            BOOST_CURRENT_LOCATION);
    return (*this)[pos];
}

//--------------------------------------------
//
// Iterators
//
//--------------------------------------------

inline
auto
params_encoded::
begin() const noexcept ->
    iterator
{
    if(u_->nparam_ > 0)
        return { u_->encoded_query() };
    return end();
}

inline
auto
params_encoded::
end() const noexcept ->
    iterator
{
    return { u_->encoded_query(), u_->nparam_, 0 };
}

//------------------------------------------------
//
// Capacity
//
//------------------------------------------------

inline
bool
params_encoded::
empty() const noexcept
{
    return size() == 0;
}

inline
std::size_t
params_encoded::
size() const noexcept
{
    return u_->nparam_;
}

//------------------------------------------------
//
// Modifiers
//
//------------------------------------------------

inline
void
params_encoded::
clear() noexcept
{
    erase(begin(), end());
}

//------------------------------------------------

inline
auto
params_encoded::
insert(
    iterator before,
    query_param_view const& v) ->
        iterator
{
    return insert(
        before, &v, &v + 1);
}

inline
auto
params_encoded::
insert(
    iterator before,
    std::initializer_list<
        reference> init) ->
            iterator
{
    return insert(before,
        init.begin(), init.end());
}

template<class FwdIt>
auto
params_encoded::
insert(
    iterator before,
    FwdIt first,
    FwdIt last) ->
        iterator
{
    return insert(before, first, last,
        typename std::iterator_traits<
            FwdIt>::iterator_category{});
}

template<class FwdIt>
auto
params_encoded::
insert(
    iterator before,
    FwdIt first,
    FwdIt last,
    std::forward_iterator_tag) ->
        iterator
{
    using detail::
        make_enc_params_iter;
    BOOST_ASSERT(before.impl_.begin_ ==
        u_->encoded_query().data());
    BOOST_ASSERT(before.impl_.end_ ==
        u_->encoded_query().data() +
        u_->encoded_query().size());
    u_->edit_params(
        before.impl_.i_,
        before.impl_.i_,
        make_enc_params_iter(
            first, last),
        make_enc_params_iter(
            first, last));
    return std::next(begin(), before.impl_.i_);
}

//------------------------------------------------

inline
auto
params_encoded::
replace(
    iterator pos,
    reference const& value) ->
        iterator
{
    return replace(
        pos,
        std::next(pos),
        &value,
        &value + 1);
}

template<class FwdIt>
auto
params_encoded::
replace(
    iterator from,
    iterator to,
    FwdIt first,
    FwdIt last) ->
        iterator
{
    using detail::
        make_enc_params_iter;
    BOOST_ASSERT(from.impl_.begin_ ==
        u_->encoded_query().data());
    BOOST_ASSERT(from.impl_.end_ ==
        u_->encoded_query().data() +
        u_->encoded_query().size());
    BOOST_ASSERT(to.impl_.begin_ ==
        u_->encoded_query().data());
    BOOST_ASSERT(to.impl_.end_ ==
        u_->encoded_query().data() +
        u_->encoded_query().size());
    u_->edit_params(
        from.impl_.i_,
        to.impl_.i_,
        make_enc_params_iter(
            first, last),
        make_enc_params_iter(
            first, last));
    return std::next(begin(), from.impl_.i_);
}

inline
auto
params_encoded::
replace(
    iterator from,
    iterator to,
    std::initializer_list<
        reference> init) ->
    iterator
{
    return replace(
        from,
        to,
        init.begin(),
        init.end());
}

//------------------------------------------------

inline
auto
params_encoded::
emplace_at(
    iterator pos,
    string_view key,
    string_view value) ->
        iterator
{
    using detail::
        make_enc_params_iter;
    reference v{
        key, value, true };
    BOOST_ASSERT(pos.impl_.begin_ ==
        u_->encoded_query().data());
    BOOST_ASSERT(pos.impl_.end_ ==
        u_->encoded_query().data() +
        u_->encoded_query().size());
    u_->edit_params(
        pos.impl_.i_,
        pos.impl_.i_ + 1,
        make_enc_params_iter(
            &v, &v + 1),
        make_enc_params_iter(
            &v, &v + 1));
    return std::next(begin(), pos.impl_.i_);
}

inline
auto
params_encoded::
emplace_back(
    string_view key) ->
        iterator
{
    return insert(
        end(), query_param_view{
            key, {}, false});
}

inline
auto
params_encoded::
emplace_back(
    string_view key,
    string_view value) ->
        iterator
{
    return insert(
        end(), query_param_view{
            key, value, true});
}

inline
auto
params_encoded::
emplace_at(
    iterator pos,
    string_view key) ->
        iterator
{
    BOOST_ASSERT(pos.impl_.begin_ ==
        u_->encoded_query().data());
    BOOST_ASSERT(pos.impl_.end_ ==
        u_->encoded_query().data() +
        u_->encoded_query().size());
    using detail::
        make_enc_params_iter;
    query_param_view v{key,
        string_view{}, false};
    u_->edit_params(
        pos.impl_.i_,
        pos.impl_.i_ + 1,
        make_enc_params_iter(
            &v, &v + 1),
        make_enc_params_iter(
            &v, &v + 1));
    return std::next(begin(), pos.impl_.i_);
}

inline
auto
params_encoded::
emplace_before(
    iterator before,
    string_view key,
    string_view value) ->
        iterator
{
    return insert(
        before,
        query_param_view{
            key,
            value,
            true});
}

inline
auto
params_encoded::
emplace_before(
    iterator before,
    string_view key) ->
        iterator
{
    return insert(
        before,
        query_param_view{
            key,
            string_view{},
            false});
}

//------------------------------------------------

inline
auto
params_encoded::
erase(iterator pos) ->
    iterator
{
    return erase(pos, std::next(pos));
}

//------------------------------------------------

inline
void
params_encoded::
push_back(
    reference const& v)
{
    insert(end(), v);
}

inline
void
params_encoded::
pop_back() noexcept
{
    erase(std::next(begin(), u_->nparam_ - 1));
}

//------------------------------------------------
//
// Lookup
//
//------------------------------------------------

inline
auto
params_encoded::
find(string_view key) const noexcept ->
    iterator
{
    return find(begin(), key);
}

inline
bool
params_encoded::
contains(string_view key) const noexcept
{
    return find(key) != end();
}

} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_SEGMENTS_HPP
#define BOOST_URL_IMPL_SEGMENTS_HPP

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/vinniefalco/http_proto
//

#ifndef BOOST_URL_DETAIL_COPIED_STRINGS_HPP
#define BOOST_URL_DETAIL_COPIED_STRINGS_HPP


namespace boost {
namespace urls {
namespace detail {

// Makes copies of string_view parameters as
// needed when the storage for the parameters
// overlap the container being modified.
class basic_copied_strings
{
    struct dynamic_buf
    {
        dynamic_buf* next;
    };

    string_view s_;
    char* local_buf_;
    std::size_t local_remain_;
    dynamic_buf* dynamic_list_ = nullptr;

    bool
    is_overlapping(
        string_view s) const noexcept;

public:
    BOOST_URL_DECL
    ~basic_copied_strings();

    BOOST_URL_DECL
    basic_copied_strings(
        string_view s,
        char* local_buf,
        std::size_t local_size) noexcept;

    BOOST_URL_DECL
    string_view
    maybe_copy(
        string_view s);
};

class copied_strings
    : public basic_copied_strings
{
    char buf_[4096];

public:
    copied_strings(
        string_view s)
        : basic_copied_strings(
            s, buf_, sizeof(buf_))
    {
    }
};

} // detail
} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_OPTIONAL_ALLOCATOR_HPP
#define BOOST_URL_DETAIL_OPTIONAL_ALLOCATOR_HPP

#include <new>

namespace boost {
namespace urls {
namespace detail {

// VFALCO This is so we can make
// iterators default-constructible
template<class Allocator>
class optional_allocator
{
    char buf_[sizeof(Allocator)];
    bool has_value_ = false;

public:
    ~optional_allocator()
    {
        if(has_value_)
            (*(*this)).~Allocator();
    }

    optional_allocator() = default;

    explicit
    optional_allocator(
        Allocator const& a) noexcept
        : has_value_(true)
    {
        ::new(buf_) Allocator(a);
    }

    optional_allocator(
        optional_allocator const& other) noexcept
        : has_value_(other.has_value_)
    {
        if(has_value_)
            ::new(buf_) Allocator(*other);
    }

    optional_allocator&
    operator=(optional_allocator const& other
        ) noexcept
    {
        if(has_value_)
            (*(*this)).~Allocator();
        has_value_ = other.has_value_;
        if(has_value_)
            ::new(buf_) Allocator(*other);
        return *this;
    }

    Allocator const&
    operator*() const noexcept
    {
        return *reinterpret_cast<
            Allocator const*>(buf_);
    }
};

} // detail
} // urls
} // boost

#endif
#include <boost/assert.hpp>
#include <iterator>
#include <new>

namespace boost {
namespace urls {

//------------------------------------------------

class segments::iterator
{
    detail::segments_iterator_impl impl_;

    friend class segments;

    iterator(
        string_view s,
        std::size_t nseg,
        const_string::factory const& a) noexcept
        : impl_(s, nseg, a)
    {
    }

    // end ctor
    iterator(
        string_view s,
        std::size_t nseg,
        const_string::factory const& a,
        int) noexcept
        : impl_(s, nseg, a, 0)
    {
    }

public:
    using value_type = const_string;
    using reference = const_string;
    using pointer = void const*;
    using difference_type = std::ptrdiff_t;
    using iterator_category =
        std::bidirectional_iterator_tag;

    iterator() noexcept = default;

    iterator(iterator const&) noexcept = default;

    iterator&
    operator=(iterator const&) noexcept = default;

    reference
    operator*() const noexcept
    {
        return impl_.dereference();
    }

    iterator&
    operator++() noexcept
    {
        impl_.increment();
        return *this;
    }

    iterator&
    operator--() noexcept
    {
        impl_.decrement();
        return *this;
    }

    iterator
    operator++(int) noexcept
    {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    iterator
    operator--(int) noexcept
    {
        auto tmp = *this;
        --*this;
        return tmp;
    }

    bool
    operator==(
        iterator const& other) const noexcept
    {
        return impl_.equal(other.impl_);
    }

    bool
    operator!=(
        iterator const& other) const noexcept
    {
        return !impl_.equal(other.impl_);
    }
};

//------------------------------------------------
//
// Members
//
//------------------------------------------------

template<class Allocator>
segments::
segments(
    url& u,
    Allocator const& a)
    : u_(&u)
    , a_(a)
{
}

inline
bool
segments::
is_absolute() const noexcept
{
    return
        u_->len(id_path) != 0 &&
        u_->s_[u_->offset(id_path)] == '/';
}

inline
segments&
segments::
operator=(std::initializer_list<
    string_view> init)
{
    assign(init.begin(), init.end());
    return *this;
}

template<class FwdIt>
auto
segments::
assign(FwdIt first, FwdIt last) ->
    typename std::enable_if<
        std::is_convertible<typename
            std::iterator_traits<
                FwdIt>::value_type,
            string_view>::value>::type
{
    u_->edit_segments(
        0,
        size(),
        detail::make_plain_segs_iter(
            first, last),
        detail::make_plain_segs_iter(
            first, last));
}

//------------------------------------------------
//
// Element Access
//
//------------------------------------------------

inline
auto
segments::
front() const ->
    const_string
{
    BOOST_ASSERT(! empty());
    return *begin();
}

inline
auto
segments::
back() const ->
    const_string
{
    BOOST_ASSERT(! empty());
    return *std::prev(end());
}

//------------------------------------------------
//
// Iterators
//
//------------------------------------------------

inline
auto
segments::
begin() const noexcept ->
    iterator
{
    return iterator(
        u_->encoded_path(), u_->nseg_, a_);
}

inline
auto
segments::
end() const noexcept ->
    iterator
{
    return iterator(
        u_->encoded_path(), u_->nseg_, a_, 0);
}

//------------------------------------------------
//
// Capacity
//
//------------------------------------------------

inline
bool
segments::
empty() const noexcept
{
    return size() == 0;
}

inline
std::size_t
segments::
size() const noexcept
{
    return u_->nseg_;
}

//------------------------------------------------
//
// Modifiers
//
//------------------------------------------------

inline
void
segments::
clear() noexcept
{
    erase(begin(), end());
}

//------------------------------------------------

inline
auto
segments::
insert(
    iterator before,
    std::initializer_list<
            string_view> init) ->
        iterator
{
    return insert(
        before,
        init.begin(),
        init.end());
}

template<class FwdIt>
auto
segments::
insert(
    iterator before,
    FwdIt first,
    FwdIt last) ->
        typename std::enable_if<
            std::is_convertible<typename
                std::iterator_traits<
                    FwdIt>::value_type,
                string_view>::value,
            iterator>::type
{
    return insert(before, first, last,
        typename std::iterator_traits<
            FwdIt>::iterator_category{});
}

template<class FwdIt>
auto
segments::
insert(
    iterator before,
    FwdIt first,
    FwdIt last,
    std::forward_iterator_tag) ->
        iterator
{
    u_->edit_segments(
        before.impl_.i_,
        before.impl_.i_,
        detail::make_plain_segs_iter(
            first, last),
        detail::make_plain_segs_iter(
            first, last));
    return std::next(begin(), before.impl_.i_);
}

//------------------------------------------------

inline
auto
segments::
replace(
    iterator pos,
    string_view s) ->
        iterator
{
    return replace(
        pos, std::next(pos),
            &s, &s + 1);
}

inline
auto
segments::
replace(
    iterator from,
    iterator to,
    std::initializer_list<
        string_view> init) ->
    iterator
{
    return replace(
        from,
        to,
        init.begin(),
        init.end());
}

template<class FwdIt>
auto
segments::
replace(
    iterator from,
    iterator to,
    FwdIt first,
    FwdIt last) ->
        typename std::enable_if<
            std::is_convertible<typename
                std::iterator_traits<
                    FwdIt>::value_type,
                string_view>::value,
            iterator>::type
{
    BOOST_ASSERT(from.impl_.begin_ >= u_->string().data());
    BOOST_ASSERT(from.impl_.end_ <= u_->string().data() +
        u_->string().size());
    BOOST_ASSERT(to.impl_.begin_ >= u_->string().data());
    BOOST_ASSERT(to.impl_.end_ >= u_->string().data() +
        u_->string().size());
    u_->edit_segments(
        from.impl_.i_,
        to.impl_.i_,
        detail::make_plain_segs_iter(
            first, last),
        detail::make_plain_segs_iter(
            first, last));
    return std::next(begin(), from.impl_.i_);
}

//------------------------------------------------

inline
auto
segments::
erase(
    iterator pos) noexcept ->
        iterator
{
    return erase(pos, std::next(pos));
}

//------------------------------------------------

inline
void
segments::
push_back(
    string_view s)
{
    insert(end(), s);
}

inline
void
segments::
pop_back() noexcept
{
    erase(std::prev(end()));
}

} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_SEGMENTS_ENCODED_HPP
#define BOOST_URL_IMPL_SEGMENTS_ENCODED_HPP

#include <iterator>

namespace boost {
namespace urls {

class segments_encoded::iterator
{
    friend class segments_encoded;

    detail::segments_encoded_iterator_impl impl_;

    iterator(
        string_view s,
        std::size_t nseg) noexcept
        : impl_(s, nseg)
    {
    }

    // end ctor
    iterator(
        string_view s,
        std::size_t nseg,
        int) noexcept
        : impl_(s, nseg, 0)
    {
    }

public:
    using value_type = const_string;
    using reference = string_view;
    using pointer = void const*;
    using difference_type = std::ptrdiff_t;
    using iterator_category =
        std::bidirectional_iterator_tag;

    iterator() = default;

    iterator(iterator const&) noexcept = default;

    iterator& operator=(iterator const&) noexcept = default;

    reference
    operator*() const noexcept
    {
        return impl_.s_;
    }

    iterator&
    operator++() noexcept
    {
        impl_.increment();
        return *this;
    }

    iterator&
    operator--() noexcept
    {
        impl_.decrement();
        return *this;
    }

    iterator
    operator++(int) noexcept
    {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    iterator
    operator--(int) noexcept
    {
        auto tmp = *this;
        --*this;
        return tmp;
    }

    bool
    operator==(
        iterator const& other) const noexcept
    {
        return impl_.equal(other.impl_);
    }

    bool
    operator!=(
        iterator const& other) const noexcept
    {
        return !impl_.equal(other.impl_);
    }
};

//------------------------------------------------
//
// Members
//
//------------------------------------------------

inline
segments_encoded::
segments_encoded(
    url& u) noexcept
    : u_(&u)
{
}

inline
bool
segments_encoded::
is_absolute() const noexcept
{
    return
        u_->len(id_path) != 0 &&
        u_->s_[u_->offset(id_path)] == '/';
}

template<class Allocator>
segments
segments_encoded::
decoded(Allocator const& alloc) const
{
    return segments(*u_, alloc);
}

inline
segments_encoded&
segments_encoded::
operator=(std::initializer_list<string_view> init)
{
    assign( init.begin(), init.end() );
    return *this;
}

template<class FwdIt>
auto
segments_encoded::
assign(
    FwdIt first, FwdIt last) ->
    typename std::enable_if<
        std::is_convertible<typename
            std::iterator_traits<
                FwdIt>::value_type,
            string_view>::value>::type
{
    u_->edit_segments(
        0,
        size(),
        detail::make_enc_segs_iter(first, last),
        detail::make_enc_segs_iter(first, last));
}

//------------------------------------------------
//
// Element Access
//
//------------------------------------------------

inline
string_view
segments_encoded::
front() const noexcept
{
    BOOST_ASSERT(!empty());
    return *begin();
}

inline
string_view
segments_encoded::
back() const noexcept
{
    BOOST_ASSERT(! empty());
    return *std::prev(end());
}

//------------------------------------------------
//
// Capacity
//
//------------------------------------------------

inline
bool
segments_encoded::
empty() const noexcept
{
    return size() == 0;
}

inline
std::size_t
segments_encoded::
size() const noexcept
{
    return u_->nseg_;
}

//------------------------------------------------
//
// Modifiers
//
//------------------------------------------------

inline
void
segments_encoded::
clear() noexcept
{
    erase(begin(), end());
}

//------------------------------------------------

inline
auto
segments_encoded::
insert(
    iterator before,
    std::initializer_list<
            string_view> init) ->
        iterator
{
    return insert(
        before,
        init.begin(),
        init.end());
}

template<class FwdIt>
auto
segments_encoded::
insert(
    iterator before,
    FwdIt first,
    FwdIt last) ->
        typename std::enable_if<
            std::is_convertible<typename
                std::iterator_traits<
                    FwdIt>::value_type,
                string_view>::value,
            iterator>::type
{
    return insert(before, first, last,
        typename std::iterator_traits<
            FwdIt>::iterator_category{});
}

template<class FwdIt>
auto
segments_encoded::
insert(
    iterator before,
    FwdIt first,
    FwdIt last,
    std::forward_iterator_tag) ->
        iterator
{
    u_->edit_segments(
        before.impl_.i_,
        before.impl_.i_,
        detail::make_enc_segs_iter(
            first, last),
        detail::make_enc_segs_iter(
            first, last));
    return std::next(begin(), before.impl_.i_);
}

//------------------------------------------------

inline
auto
segments_encoded::
replace(
    iterator pos,
    string_view s) ->
        iterator
{
    return replace(
        pos, std::next(pos),
            &s, &s + 1);
}

inline
auto
segments_encoded::
replace(
    iterator from,
    iterator to,
    std::initializer_list<
        string_view> init) ->
    iterator
{
    return replace(
        from,
        to,
        init.begin(),
        init.end());
}

template<class FwdIt>
auto
segments_encoded::
replace(
    iterator from,
    iterator to,
    FwdIt first,
    FwdIt last) ->
        typename std::enable_if<
            std::is_convertible<typename
                std::iterator_traits<
                    FwdIt>::value_type,
                string_view>::value,
            iterator>::type
{
    BOOST_ASSERT(from.impl_.begin_ >= u_->string().data());
    BOOST_ASSERT(from.impl_.end_ <= u_->string().data() +
        u_->string().size());
    BOOST_ASSERT(to.impl_.begin_ >= u_->string().data());
    BOOST_ASSERT(to.impl_.end_ >= u_->string().data() +
        u_->string().size());
    u_->edit_segments(
        from.impl_.i_,
        to.impl_.i_,
        detail::make_enc_segs_iter(first, last),
        detail::make_enc_segs_iter(first, last));
    return std::next(begin(), from.impl_.i_);
}

//------------------------------------------------

inline
auto
segments_encoded::
erase(
    iterator pos) noexcept ->
        iterator
{
    return erase(pos, std::next(pos));
}

//------------------------------------------------

inline
void
segments_encoded::
push_back(
    string_view s)
{
    insert(end(), s);
}

inline
void
segments_encoded::
pop_back() noexcept
{
    erase(std::prev(end()));
}

} // urls
} // boost

#endif

#endif
#include <boost/align/align_up.hpp>
#include <boost/static_assert.hpp>
#include <cstddef>

namespace boost {
namespace urls {

// VFALCO This is here to reduce the
// number of template instantiations,
// and keep definitions in the library
#ifndef BOOST_URL_DOCS
class BOOST_SYMBOL_VISIBLE
    static_url_base : public url
{
protected:
    BOOST_URL_DECL
    virtual
    ~static_url_base();

    BOOST_URL_DECL static_url_base(
        char* buf, std::size_t cap) noexcept;
    BOOST_URL_DECL void construct(string_view s);
    BOOST_URL_DECL void copy(url const& u);
    BOOST_URL_DECL void copy(url_view const& u);
    BOOST_URL_DECL url_view convert() const noexcept;
    BOOST_URL_DECL char* allocate(
        std::size_t n) override;
    BOOST_URL_DECL void deallocate(char*) override;
};
#endif

//------------------------------------------------

/** A URL with fixed-capacity storage

    This container acts like @ref url,
    except that dynamic allocations are
    never performed. Instead, the capacity
    for the data comes from inline storage.

    @tparam Capacity The maximum capacity
    in bytes. A URL requires bytes equal to
    at least the number of characters plus
    one for the terminating NULL, plus an
    additional number of bytes proportional
    to the larger of the count of path segments
    and the count of query params. Due to
    alignment requirements, the usable
    capacity may be slightly less than
    this number.

    @see
        @ref url,
        @ref url_view.
*/
template<std::size_t Capacity>
class static_url
#ifdef BOOST_URL_DOCS
    : public url
#else
    : public static_url_base
#endif
{
    BOOST_STATIC_ASSERT(
        Capacity < (std::size_t(-1) &
            ~(alignof(pos_t)-1)));
    char buf_[alignment::align_up(
        Capacity, alignof(pos_t))];

public:
    /** Destructor
    */
    ~static_url()
    {
    }

    /** Constructor
    */
    static_url() noexcept
        : static_url_base(
            buf_, sizeof(buf_))
    {
    }

    /** Constructor

        This constructs a copy of `u`.

        @par Exception Safety
        Strong guarantee.

        @throw std::bad_alloc insufficient space
    */
    static_url(url const& u)
        : static_url()
    {
        copy(u);
    }

    /** Constructor

        This constructs a copy of `u`.

        @par Exception Safety
        Strong guarantee.

        @throw std::bad_alloc insufficient space
    */
    static_url(url_view const& u)
        : static_url()
    {
        copy(u);
    }

    /** Constructor

        This constructs a copy of `u`.

        @par Exception Safety
        Strong guarantee.

        @throw std::bad_alloc insufficient space
    */
    static_url(static_url const& u)
        : static_url()
    {
        copy(u);
    }

    /** Assignment

        @par Exception Safety
        Strong guarantee.

        @throw std::bad_alloc insufficient space
    */
    static_url&
    operator=(url const& u)
    {
        copy(u);
        return *this;
    }

    /** Assignment

        @par Exception Safety
        Strong guarantee.

        @throw std::bad_alloc insufficient space
    */
    static_url&
    operator=(url_view const& u)
    {
        copy(u);
        return *this;
    }

    /** Assignment

        @par Exception Safety
        Strong guarantee.

        @throw std::bad_alloc insufficient space
    */
    static_url&
    operator=(static_url const& u)
    {
        copy(u);
        return *this;
    }

    /** Construct from a string
    */
    static_url(string_view s)
        : static_url()
    {
        construct(s);
    }
};

} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_URLS_HPP
#define BOOST_URL_URLS_HPP


namespace boost {

using urls::url;
using urls::url_view;

} // boost

#endif

//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_ABSOLUTE_URI_RULE_HPP
#define BOOST_URL_RFC_ABSOLUTE_URI_RULE_HPP

//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_HIER_PART_HPP
#define BOOST_URL_RFC_HIER_PART_HPP

//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_AUTHORITY_RULE_HPP
#define BOOST_URL_RFC_AUTHORITY_RULE_HPP

//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_HOST_RULE_HPP
#define BOOST_URL_RFC_HOST_RULE_HPP


namespace boost {
namespace urls {

/** Rule for host

    @par BNF
    @code
    host          = IP-literal / IPv4address / reg-name
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.2"
        >3.2.2. Host (rfc3986)</a>

    @see
        @ref host_type,
        @ref ipv4_address,
        @ref ipv6_address.
*/
struct host_rule
{
    urls::host_type host_type =
        urls::host_type::none;
    pct_encoded_str name;
    ipv4_address ipv4;
    ipv6_address ipv6;
    string_view ipvfuture;
    string_view host_part;

    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* const end,
        error_code& ec,
        host_rule& t) noexcept
    {
        return parse(it, end, ec, t);
    }

private:
    BOOST_URL_DECL
    static
    void
    parse(
        char const*& it,
        char const* const end,
        error_code& ec,
        host_rule& t) noexcept;
};

} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_PORT_RULE_HPP
#define BOOST_URL_RFC_PORT_RULE_HPP

#include <cstdint>

namespace boost {
namespace urls {

/** Rule for port

    @par BNF
    @code
    port          = *DIGIT
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.2"
        >3.2.2. Host (rfc3986)</a>

    @see
        @ref port_part_rule.
*/
struct port_rule
{
    using number_type =
        std::uint16_t;

    string_view str;
    std::uint16_t number;
    bool has_number = false;

    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* const end,
        error_code& ec,
        port_rule& t) noexcept
    {
        parse(it, end, ec, t);
    }

private:
    BOOST_URL_DECL
    static
    void
    parse(
        char const*& it,
        char const* const end,
        error_code& ec,
        port_rule& t) noexcept;

};

/** Rule for port-part

    @par BNF
    @code
    port-part       = [ ":" port ]

    port            = *DIGIT
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.2"
        >3.2.2. Host (rfc3986)</a>

    @see
        @ref port_rule.
*/
struct port_part_rule
{
    using number_type =
        std::uint16_t;

    bool has_port = false;
    string_view port;
    bool has_number = false;
    std::uint16_t port_number = 0;

    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* const end,
        error_code& ec,
        port_part_rule& t)
    {
        parse(it, end, ec, t);
    }

private:
    BOOST_URL_DECL
    static
    void
    parse(
        char const*& it,
        char const* const end,
        error_code& ec,
        port_part_rule& t) noexcept;
};

} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_USERINFO_RULE_HPP
#define BOOST_URL_RFC_USERINFO_RULE_HPP


namespace boost {
namespace urls {

/** Rule for userinfo

    @par BNF
    @code
    userinfo    = user [ ":" [ password ] ]

    user        = *( unreserved / pct-encoded / sub-delims )
    password    = *( unreserved / pct-encoded / sub-delims / ":" )
    @endcode

    @par Specification
    <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.1"
        >3.2.1. User Information (3986)</a>
*/
struct userinfo_rule
{
    pct_encoded_str user;
    bool            has_password = false;
    pct_encoded_str password;

    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* const end,
        error_code& ec,
        userinfo_rule& t) noexcept
    {
        return parse(it, end, ec, t);
    }

private:
    BOOST_URL_DECL
    static
    void
    parse(
        char const*& it,
        char const* const end,
        error_code& ec,
        userinfo_rule& t) noexcept;
};

} // urls
} // boost

#endif

namespace boost {
namespace urls {

/** Rule for authority

    @par BNF
    @code
    authority   = [ userinfo "@" ] host [ ":" port ]
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2"
        >3.2. Authority (rfc3986)</a>

    @see
        @ref host_rule,
        @ref port_part_rule,
        @ref userinfo_rule.
*/
struct authority_rule
{
    // userinfo
    bool has_userinfo = false;
    userinfo_rule userinfo;

    // host
    host_rule host;

    // port
    port_part_rule port;

    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* const end,
        error_code& ec,
        authority_rule& t) noexcept
    {
        parse(it, end, ec, t);
    }

private:
    BOOST_URL_DECL
    static
    void
    parse(
        char const*& it,
        char const* const end,
        error_code& ec,
        authority_rule& t) noexcept;
};

} // urls
} // boost

#endif

namespace boost {
namespace urls {

/** Rule for hier-part

    @par BNF
    @code
    hier-part     = "//" authority path-abempty
                  / path-absolute
                  / path-rootless
                  / path-empty
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3"
        >3. Syntax Components (rfc3986)</a>
*/
struct hier_part_rule
{
    bool has_authority = false;
    authority_rule authority;
    parsed_path path;

    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* const end,
        error_code& ec,
        hier_part_rule& t) noexcept
    {
        return parse(it, end, ec, t);
    }

private:
    BOOST_URL_DECL
    static
    void
    parse(
        char const*& it,
        char const* const end,
        error_code& ec,
        hier_part_rule& t) noexcept;
};

} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_QUERY_RULE_HPP
#define BOOST_URL_RFC_QUERY_RULE_HPP

#include <cstddef>

namespace boost {
namespace urls {

/** Rule for query

    @par BNF
    @code
    query           = *( pchar / "/" / "?" )

    query-params    = query-param *( "&" query-param )
    query-param     = key [ "=" value ]
    key             = *qpchar
    value           = *( qpchar / "=" )
    qpchar          = unreserved
                    / pct-encoded
                    / "!" / "$" / "'" / "(" / ")"
                    / "*" / "+" / "," / ";"
                    / ":" / "@" / "/" / "?"
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.4"
        >3.4. Query (rfc3986)</a>

    @see
        @ref query_part_rule.
*/
struct query_rule
{
    using value_type = query_param;
    using reference = query_param_view;

    grammar::range<query_rule> v;

    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* const end,
        error_code& ec,
        query_rule& t) noexcept
    {
        grammar::parse(
            it, end, ec, t.v);
    }

    BOOST_URL_DECL
    static
    bool
    begin(
        char const*& it,
        char const* const end,
        error_code& ec,
        reference& t) noexcept;

    BOOST_URL_DECL
    static
    bool
    increment(
        char const*& it,
        char const* const end,
        error_code& ec,
        reference& t) noexcept;

private:
    struct key_chars;
    struct value_chars;
};

//------------------------------------------------

/** Rule for query-part

    @par BNF
    @code
    query-part      = [ "?" query ]

    query           = *( pchar / "/" / "?" )
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.4"
        >3.4. Query (rfc3986)</a>

    @see
        @ref query_rule.
*/
struct query_part_rule
{
    bool has_query = false;
    query_rule query;
    string_view query_part;

    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* const end,
        error_code& ec,
        query_part_rule& t) noexcept
    {
        parse(it, end, ec, t);
    }

private:
    BOOST_URL_DECL
    static
    void
    parse(
        char const*& it,
        char const* const end,
        error_code& ec,
        query_part_rule& t) noexcept;

};

} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_SCHEME_RULE_HPP
#define BOOST_URL_RFC_SCHEME_RULE_HPP


namespace boost {
namespace urls {

/** Rule for scheme

    @par BNF
    @code
    scheme      = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.1"
        >3.1. Scheme (rfc3986)</a>

    @see
        @ref scheme.
*/
struct scheme_rule
{
    string_view scheme;
    urls::scheme scheme_id{urls::scheme::unknown};

    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* const end,
        error_code& ec,
        scheme_rule& t) noexcept
    {
        return parse(it, end, ec, t);
    }

private:
    BOOST_URL_DECL
    static
    void
    parse(
        char const*& it,
        char const* const end,
        error_code& ec,
        scheme_rule& t) noexcept;
};

/** Rule for scheme-part

    @par BNF
    @code
    scheme-part     = scheme ":"

    scheme          = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.1"
        >3.1. Scheme (rfc3986)</a>

    @see
        @ref scheme.
*/
struct scheme_part_rule
{
    string_view scheme;
    urls::scheme scheme_id =
        urls::scheme::none;
    string_view scheme_part;

    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* const end,
        error_code& ec,
        scheme_part_rule& t) noexcept
    {
        parse(it, end, ec, t);
    }

private:
    BOOST_URL_DECL
    static
    void
    parse(
        char const*& it,
        char const* const end,
        error_code& ec,
        scheme_part_rule& t) noexcept;

};

} // urls
} // boost

#endif

namespace boost {
namespace urls {

/** Rule for absolute-URI

    @par BNF
    @code
    absolute-URI    = scheme ":" hier-part [ "?" query ]

    hier-part       = "//" authority path-abempty
                    / path-absolute
                    / path-rootless
                    / path-empty
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-4.3"
        >4.3. Absolute URI (rfc3986)</a>

    @see
        @ref hier_part_rule,
        @ref query_part_rule,
        @ref scheme_part_rule.
*/
struct absolute_uri_rule
{
    scheme_part_rule scheme_part;
    hier_part_rule   hier_part;
    query_part_rule  query_part;

    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* const end,
        error_code& ec,
        absolute_uri_rule& t) noexcept
    {
        parse(it, end, ec, t);
    }

private:
    BOOST_URL_DECL
    static
    void
    parse(
        char const*& it,
        char const* const end,
        error_code& ec,
        absolute_uri_rule& t) noexcept;
};

} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_FRAGMENT_RULE_HPP
#define BOOST_URL_RFC_FRAGMENT_RULE_HPP


namespace boost {
namespace urls {

/** Rule for fragment

    @par BNF
    @code
    fragment        = *( pchar / "/" / "?" )
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.5"
        >3.5. Fragment (rfc3986)</a>

    @see
        @ref fragment_part_rule.
*/
struct fragment_rule
{
    pct_encoded_str s;

    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* const end,
        error_code& ec,
        fragment_rule& t) noexcept
    {
        parse(it, end, ec, t);
    }

private:
    BOOST_URL_DECL
    static
    void
    parse(
        char const*& it,
        char const* const end,
        error_code& ec,
        fragment_rule& t) noexcept;
};

/** Rule for fragment-part

    @par BNF
    @code
    fragment-part   = [ "#" fragment ]

    fragment        = *( pchar / "/" / "?" )
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.5"
        >3.5. Fragment (rfc3986)</a>

    @see
        @ref fragment_rule.
*/
struct fragment_part_rule
{
    bool has_fragment = false;
    pct_encoded_str fragment;

    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* const end,
        error_code& ec,
        fragment_part_rule& t) noexcept
    {
        parse(it, end, ec, t);
    }

private:
    BOOST_URL_DECL
    static
    void
    parse(
        char const*& it,
        char const* const end,
        error_code& ec,
        fragment_part_rule& t) noexcept;
};

} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_IP_LITERAL_RULE_HPP
#define BOOST_URL_RFC_IP_LITERAL_RULE_HPP


namespace boost {
namespace urls {

/** Rule for IP-literal

    @par BNF
    @code
    IP-literal = "[" ( IPv6address / IPvFuture  ) "]"
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.2"
        >3.2.2. Host (rfc3986)</a>

    @see
        @ref ipv6_address.
*/
struct ip_literal_rule
{
    bool is_ipv6 = false;
    ipv6_address ipv6;
    string_view ipvfuture;

    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* const end,
        error_code& ec,
        ip_literal_rule& t) noexcept
    {
        parse(it, end, ec, t);
    }

private:
    BOOST_URL_DECL
    static
    void
    parse(
        char const*& it,
        char const* const end,
        error_code& ec,
        ip_literal_rule& t) noexcept;
};

} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_IPV_FUTURE_RULE_HPP
#define BOOST_URL_RFC_IPV_FUTURE_RULE_HPP


namespace boost {
namespace urls {

/** Rule for IPvFuture

    @par BNF
    @code
    IPvFuture     = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.2"
        >3.2.2. Host (rfc3986)</a>
*/
struct ipv_future_rule
{
    string_view str;
    string_view major;
    string_view minor;

    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* const end,
        error_code& ec,
        ipv_future_rule& t) noexcept
    {
        parse(it, end, ec, t);
    }

private:
    BOOST_URL_DECL
    static
    void
    parse(
        char const*& it,
        char const* const end,
        error_code& ec,
        ipv_future_rule& t) noexcept;
};

} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_PCT_ENCODED_RULE_HPP
#define BOOST_URL_RFC_PCT_ENCODED_RULE_HPP

#include <boost/static_assert.hpp>


namespace boost {
namespace urls {

/** Rule for a string of percent-encoded characters from a character set

    @tparam CharSet The character set indicating
    which characters are unreserved. Any character
    not in this set will be considered reserved.

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-2.1">
        2.1. Percent-Encoding (rfc3986)</a>
*/
template<class CharSet>
struct pct_encoded_rule
{
    pct_encoded_str s;

    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* end,
        error_code& ec,
        pct_encoded_rule& t) noexcept
    {
        parse(it, end, ec, t);
    }

private:
    static
    void
    parse(
        char const*& it,
        char const* end,
        error_code& ec,
        pct_encoded_rule& t) noexcept;
};

} // urls
} // boost

//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_PCT_ENCODED_RULE_HPP
#define BOOST_URL_IMPL_PCT_ENCODED_RULE_HPP


namespace boost {
namespace urls {

template<class CharSet>
void
pct_encoded_rule<CharSet>::
parse(
    char const*& it,
    char const* end,
    error_code& ec,
    pct_encoded_rule& t) noexcept
{
    auto const start = it;
    static constexpr CharSet cs{};
    // VFALCO TODO
    // opt.plus_to_space?
    std::size_t n = 0;
    char const* it0;
skip:
    it0 = it;
    it = grammar::find_if_not(
        it0, end, cs);
    n += it - it0;
    if(it == end)
        goto finish;
    if(*it != '%')
        goto finish;
    for(;;)
    {
        ++it;
        if(it == end)
        {
            // missing HEXDIG
            ec = grammar::error::syntax;
            return;
        }
        char r;
        if(!grammar::hexdig_value(*it, r))
        {
            // expected HEXDIG
            ec = grammar::error::syntax;
            return;
        }
        ++it;
        if(it == end)
        {
            // missing HEXDIG
            ec = grammar::error::syntax;
            return;
        }
        if(!grammar::hexdig_value(*it, r))
        {
            // expected HEXDIG
            ec = grammar::error::syntax;
            return;
        }
        ++n;
        ++it;
        if(it == end)
            break;
        if(*it != '%')
            goto skip;
    }
finish:
    t.s.str = string_view(
        start, it - start);
    t.s.decoded_size = n;
}

} // urls
} // boost

#endif

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_REG_NAME_RULE_HPP
#define BOOST_URL_RFC_REG_NAME_RULE_HPP


namespace boost {
namespace urls {

/** Rule for reg-name

    @par BNF
    @code
    reg-name    = *( unreserved / pct-encoded / "-" / ".")
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.2.2"
        >3.2.2. Host (rfc3986)</a>
    @li <a href="https://www.rfc-editor.org/errata/eid4942"
        >Errata ID: 4942</a>
*/
struct reg_name_rule
{
    pct_encoded_str v;

    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* const end,
        error_code& ec,
        reg_name_rule& t) noexcept
    {
        parse(it, end, ec, t);
    }

private:
    BOOST_URL_DECL
    static
    void
    parse(
        char const*& it,
        char const* const end,
        error_code& ec,
        reg_name_rule& t) noexcept;
};

} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_RELATIVE_PART_RULE_HPP
#define BOOST_URL_RFC_RELATIVE_PART_RULE_HPP


namespace boost {
namespace urls {

/** Rule for relative-part

    @par BNF
    @code
    relative-part = "//" authority path-abempty
                  / path-absolute
                  / path-noscheme
                  / path-abempty
                  / path-empty
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-4.2
        >4.2. Relative Reference (rfc3986)</a>
    @li <a href="https://www.rfc-editor.org/errata/eid5428"
        >Errata ID: 5428 (rfc3986)</a>

    @see
        @ref authority_rule.
*/
struct relative_part_rule
{
    bool has_authority = false;
    authority_rule authority;
    parsed_path path;

    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* const end,
        error_code& ec,
        relative_part_rule& t) noexcept
    {
        return parse(it, end, ec, t);
    }

private:
    BOOST_URL_DECL
    static
    void
    parse(
        char const*& it,
        char const* const end,
        error_code& ec,
        relative_part_rule& t) noexcept;
};

} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_RELATIVE_REF_RULE_HPP
#define BOOST_URL_RFC_RELATIVE_REF_RULE_HPP


namespace boost {
namespace urls {

/** Rule for relative-ref

    @par BNF
    @code
    relative-ref  = relative-part [ "?" query ] [ "#" fragment ]
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-4.2
        >4.2. Relative Reference (rfc3986)</a>

    @see
        @ref fragment_part_rule,
        @ref query_part_rule,
        @ref relative_part_rule.
*/
struct relative_ref_rule
{
    relative_part_rule   relative_part;
    query_part_rule      query_part;
    fragment_part_rule   fragment_part;

    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* const end,
        error_code& ec,
        relative_ref_rule& t) noexcept
    {
        return parse(it, end, ec, t);
    }

private:
    BOOST_URL_DECL
    static
    void
    parse(
        char const*& it,
        char const* const end,
        error_code& ec,
        relative_ref_rule& t) noexcept;
};

} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_URI_RULE_HPP
#define BOOST_URL_RFC_URI_RULE_HPP


namespace boost {
namespace urls {

/** Rule for URI

    @par BNF
    @code
    URI           = scheme ":" hier-part [ "?" query ] [ "#" fragment ]
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3"
        >3. Syntax Components (rfc3986)</a>

    @see
        @ref fragment_part_rule,
        @ref hier_part_rule,
        @ref query_part_rule,
        @ref scheme_part_rule.
*/
struct uri_rule
{
    scheme_part_rule     scheme_part;
    hier_part_rule       hier_part;
    query_part_rule      query_part;
    fragment_part_rule   fragment_part;

    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* const end,
        error_code& ec,
        uri_rule& t) noexcept
    {
        return parse(it, end, ec, t);
    }

private:
    BOOST_URL_DECL
    static
    void
    parse(
        char const*& it,
        char const* const end,
        error_code& ec,
        uri_rule& t) noexcept;
};

} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_URI_REFERENCE_RULE_HPP
#define BOOST_URL_RFC_URI_REFERENCE_RULE_HPP


namespace boost {
namespace urls {

/** Rule for URI-reference

    @par BNF
    @code
    URI-reference = URI / relative-ref

    URI           = scheme ":" hier-part [ "?" query ] [ "#" fragment ]
    relative-ref  = relative-part [ "?" query ] [ "#" fragment ]
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3"
        >3. Syntax Components (rfc3986)</a>


    @see
        @ref authority_rule,
        @ref fragment_part_rule,
        @ref query_part_rule,
        @ref scheme_part_rule.
*/
struct uri_reference_rule
{
    scheme_part_rule     scheme_part;
    bool                has_authority = false;
    authority_rule       authority;
    parsed_path         path;
    query_part_rule      query_part;
    fragment_part_rule   fragment_part;

    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* const end,
        error_code& ec,
        uri_reference_rule& t) noexcept
    {
        return parse(it, end, ec, t);
    }

private:
    BOOST_URL_DECL
    static
    void
    parse(
        char const*& it,
        char const* const end,
        error_code& ec,
        uri_reference_rule& t) noexcept;
};

} // urls
} // boost

#endif

#endif

 //
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_IMPL_ANY_PATH_ITER_IPP
#define BOOST_URL_DETAIL_IMPL_ANY_PATH_ITER_IPP


namespace boost {
namespace urls {
namespace detail {

any_path_iter::
~any_path_iter() noexcept = default;

//------------------------------------------------

void
enc_path_iter::
increment() noexcept
{
    p_ += n_;
    if(p_ == end_)
    {
        p_ = nullptr;
        return;
    }
    ++p_;
    string_view s(p_, end_ - p_);
    auto pos = s.find_first_of('/');
    if(pos != string_view::npos)
        n_ = pos;
    else
        n_ = s.size();
}

enc_path_iter::
enc_path_iter(
    string_view s) noexcept
    : end_(s.data() + s.size())
{
    if(s.empty())
    {
        n_ = 0;
        p_ = nullptr;
        return;
    }
    std::size_t pos;
    if(s.starts_with('/'))
        s.remove_prefix(1);
    pos = s.find_first_of('/');
    p_ = s.data();
    if(pos != string_view::npos)
        n_ = pos;
    else
        n_ = s.size();
    front = { p_, n_ };
}

bool
enc_path_iter::
measure(
    std::size_t& n,
    error_code& ec) noexcept
{
    if(! p_)
        return false;
    string_view s(p_, n_);
    urls::validate_pct_encoding(
        s, ec, {}, pchars);
    if(ec.failed())
        return false;
    n += s.size();
    increment();
    return true;
}

void
enc_path_iter::
copy(
    char*& dest,
    char const* end) noexcept
{
    (void)end;
    BOOST_ASSERT(static_cast<
        std::size_t>(
            end - dest) >= n_);
    BOOST_ASSERT(p_ != nullptr);
    if(n_ > 0)
    {
        std::memcpy(
            dest, p_, n_);
        dest += n_;
    }
    increment();
}

//------------------------------------------------

void
plain_path_iter::
increment() noexcept
{
    p_ += n_;
    if(p_ == end_)
    {
        p_ = nullptr;
        return;
    }
    ++p_;
    string_view s(p_, end_ - p_);
    auto pos = s.find_first_of('/');
    if(pos != string_view::npos)
        n_ = pos;
    else
        n_ = s.size();
}

plain_path_iter::
plain_path_iter(
    string_view s) noexcept
    : end_(s.data() + s.size())
{
    if(s.empty())
    {
        n_ = 0;
        p_ = nullptr;
        return;
    }
    std::size_t pos;
    if(s.starts_with('/'))
        s.remove_prefix(1);
    pos = s.find_first_of('/');
    p_ = s.data();
    if(pos != string_view::npos)
        n_ = pos;
    else
        n_ = s.size();
    front = { p_, n_ };
}

bool
plain_path_iter::
measure(
    std::size_t& n,
    error_code&) noexcept
{
    if(! p_)
        return false;
    string_view s(p_, n_);
    n += urls::pct_encode_bytes(
        s, {}, pchars);
    increment();
    return true;
}

void
plain_path_iter::
copy(
    char*& dest,
    char const* end) noexcept
{
    BOOST_ASSERT(p_ != nullptr);
    dest += pct_encode(
        dest, end,
        string_view(p_, n_),
        {},
        pchars);
    increment();
}

//------------------------------------------------

bool
enc_segs_iter_base::
measure_impl(
    string_view s,
    std::size_t& n,
    error_code& ec) noexcept
{
    urls::validate_pct_encoding(
        s, ec, {}, pchars);
    if(ec.failed())
        return false;
    n += s.size();
    return true;
}

void
enc_segs_iter_base::
copy_impl(
    string_view s,
    char*& dest,
    char const* end) noexcept
{
    (void)end;
    BOOST_ASSERT(static_cast<
        std::size_t>(end - dest) >=
            s.size());
    if(! s.empty())
    {
        std::memcpy(dest,
            s.data(), s.size());
        dest += s.size();
    }
}

//------------------------------------------------

void
plain_segs_iter_base::
measure_impl(
    string_view s,
    std::size_t& n) noexcept
{
    n += pct_encode_bytes(
        s, {}, pchars);
}

void
plain_segs_iter_base::
copy_impl(
    string_view s,
    char*& dest,
    char const* end) noexcept
{
    dest += pct_encode(
        dest, end, s, {}, pchars);
}

} // detail
} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_IMPL_ANY_QUERY_ITER_IPP
#define BOOST_URL_DETAIL_IMPL_ANY_QUERY_ITER_IPP

//
// Copyright (c) 2022 alandefreitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_DETAIL_CHARSETS_HPP
#define BOOST_URL_RFC_DETAIL_CHARSETS_HPP


namespace boost {
namespace urls {
namespace detail {

struct user_chars_t : grammar::lut_chars
{
    constexpr
    user_chars_t() noexcept
        : grammar::lut_chars(
            unreserved_chars +
            subdelim_chars)
    {
    }
};

constexpr user_chars_t user_chars{};

struct password_chars_t : grammar::lut_chars
{
    constexpr
    password_chars_t() noexcept
        : grammar::lut_chars(
            unreserved_chars +
            subdelim_chars + ':')
    {
    }
};

constexpr password_chars_t password_chars{};

struct userinfo_chars_t : grammar::lut_chars
{
    constexpr
        userinfo_chars_t() noexcept
        : grammar::lut_chars(
            unreserved_chars +
            subdelim_chars)
    {
    }
};

constexpr userinfo_chars_t userinfo_chars{};

struct host_chars_t : grammar::lut_chars
{
    constexpr
        host_chars_t() noexcept
        : grammar::lut_chars(
            unreserved_chars +
            subdelim_chars)
    {
    }
};

constexpr host_chars_t host_chars{};

struct reg_name_chars_t : grammar::lut_chars
{
    constexpr
        reg_name_chars_t() noexcept
        : grammar::lut_chars(
            unreserved_chars
            + '-' + '.')
    {
    }
};

constexpr reg_name_chars_t reg_name_chars{};

struct segment_chars_t : grammar::lut_chars
{
    constexpr
        segment_chars_t() noexcept
        : grammar::lut_chars(
            pchars)
    {
    }
};

constexpr segment_chars_t segment_chars{};

struct path_chars_t : grammar::lut_chars
{
    constexpr
        path_chars_t() noexcept
        : grammar::lut_chars(
            segment_chars + '/')
    {
    }
};

constexpr path_chars_t path_chars{};

} // detail
} // urls
} // boost

#endif

namespace boost {
namespace urls {
namespace detail {

any_query_iter::
~any_query_iter() noexcept = default;

//------------------------------------------------

void
enc_query_iter::
increment() noexcept
{
    p_ += n_;
    if(p_ == end_)
    {
        p_ = nullptr;
        return;
    }
    ++p_;
    string_view s(p_, end_ - p_);
    auto pos = s.find_first_of('&');
    if(pos != string_view::npos)
        n_ = pos;
    else
        n_ = s.size();
}

enc_query_iter::
enc_query_iter(
    string_view s) noexcept
    : end_(s.data() + s.size())
{
    if(s.empty())
    {
        n_ = 0;
        p_ = nullptr;
        return;
    }
    std::size_t pos;
    pos = s.find_first_of('&');
    p_ = s.data();
    if(pos != string_view::npos)
        n_ = pos;
    else
        n_ = s.size();
}

bool
enc_query_iter::
measure(
    std::size_t& n,
    error_code& ec) noexcept
{
    if(! p_)
        return false;
    string_view s(p_, n_);
    urls::validate_pct_encoding(
        s, ec, {}, query_chars);
    if(ec.failed())
        return false;
    n += s.size();
    increment();
    return true;
}

void
enc_query_iter::
copy(
    char*& dest,
    char const* end) noexcept
{
    (void)end;
    BOOST_ASSERT(static_cast<
        std::size_t>(
            end - dest) >= n_);
    BOOST_ASSERT(p_ != nullptr);
    if(n_ > 0)
    {
        std::memcpy(
            dest, p_, n_);
        dest += n_;
    }
    increment();
}

//------------------------------------------------

void
plain_query_iter::
increment() noexcept
{
    p_ += n_;
    if(p_ == end_)
    {
        p_ = nullptr;
        return;
    }
    ++p_;
    string_view s(p_, end_ - p_);
    auto pos = s.find_first_of('&');
    if(pos != string_view::npos)
        n_ = pos;
    else
        n_ = s.size();
}

plain_query_iter::
plain_query_iter(
    string_view s) noexcept
    : end_(s.data() + s.size())
{
    if(s.empty())
    {
        n_ = 0;
        p_ = nullptr;
        return;
    }
    std::size_t pos;
    pos = s.find_first_of('&');
    p_ = s.data();
    if(pos != string_view::npos)
        n_ = pos;
    else
        n_ = s.size();
}

bool
plain_query_iter::
measure(
    std::size_t& n,
    error_code&) noexcept
{
    if(! p_)
        return false;
    string_view s(p_, n_);
    n += urls::pct_encode_bytes(
        s, {}, query_chars);
    increment();
    return true;
}

void
plain_query_iter::
copy(
    char*& dest,
    char const* end) noexcept
{
    BOOST_ASSERT(p_ != nullptr);
    dest += pct_encode(
        dest, end,
        string_view(p_, n_),
        {},
        query_chars);
    increment();
}

//------------------------------------------------

bool
enc_params_iter_base::
measure_impl(
    string_view key,
    string_view const* value,
    std::size_t& n,
    error_code& ec) noexcept
{
    pct_decode_opts opt;
    opt.plus_to_space = true;
    validate_pct_encoding(
        key, ec, opt, query_chars);
    if(ec.failed())
        return false;
    n += key.size();
    if(value)
    {
        validate_pct_encoding(
            *value, ec, opt, query_chars);
        if(ec.failed())
            return false;
        n += 1 + value->size();
    }
    return ! ec.failed();
}

void
enc_params_iter_base::
copy_impl(
    string_view key,
    string_view const* value,
    char*& dest,
    char const* end) noexcept
{
    (void)end;
    // avoid self-copy
    std::size_t kn = key.size();
    BOOST_ASSERT(end - kn >= dest);
    if( key.data() != dest &&
        key.data() != nullptr)
    {
        std::memcpy(dest,
            key.data(), kn);
    }
    dest += kn;
    if(value)
    {
        BOOST_ASSERT(
            end - 1 >= dest);
        *dest++ = '=';
        std::size_t vn =
            value->size();
        BOOST_ASSERT(
            end - vn >= dest);
        if(vn > 0)
        {
            std::memcpy(dest,
                value->data(), vn);
            dest += vn;
        }
    }
}

//------------------------------------------------

void
plain_params_iter_base::
measure_impl(
    string_view key,
    string_view const* value,
    std::size_t& n) noexcept
{
    n += pct_encode_bytes(
        key, {}, query_chars);
    if(value)
    {
        ++n; // '='
        n += pct_encode_bytes(
            *value, {}, query_chars);
    }
}

void
plain_params_iter_base::
copy_impl(
    string_view key,
    string_view const* value,
    char*& dest,
    char const* end) noexcept
{
    dest += pct_encode(
        dest, end, key, {}, query_chars);
    if(value)
    {
        *dest++ = '=';
        dest += pct_encode(
            dest, end, *value, {}, query_chars);
    }
}

//------------------------------------------------

void
plain_value_iter_base::
measure_impl(
    string_view key,
    string_view const* value,
    std::size_t& n) noexcept
{
    n += key.size();
    if(value)
    {
        ++n; // '='
        n += pct_encode_bytes(
            *value, {}, query_chars);
    }
}

void
plain_value_iter_base::
copy_impl(
    string_view key,
    string_view const* value,
    char*& dest,
    char const* end) noexcept
{
    // avoid self-copy
    std::size_t n = key.size();
    BOOST_ASSERT(end - n >= dest);
    // iterator for value only
    BOOST_ASSERT(key.data() == dest);
    dest += n;
    if(value)
    {
        *dest++ = '=';
        dest += pct_encode(
            dest, end, *value, {}, query_chars);
    }
}

} // detail
} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/vinniefalco/http_proto
//

#ifndef BOOST_URL_DETAIL_COPIED_STRINGS_IPP
#define BOOST_URL_DETAIL_COPIED_STRINGS_IPP

#include <functional>

namespace boost {
namespace urls {
namespace detail {

bool
basic_copied_strings::
is_overlapping(
    string_view s) const noexcept
{
    auto const b1 = s_.data();
    auto const e1 = b1 + s_.size();
    auto const b2 = s.data();
    auto const e2 = b2 + s.size();
    auto const less_equal =
        std::less_equal<char const*>();
    if(less_equal(e1, b2))
        return false;
    if(less_equal(e2, b1))
        return false;
    return true;
}

basic_copied_strings::
~basic_copied_strings()
{
    while(dynamic_list_)
    {
        auto p = dynamic_list_;
        dynamic_list_ =
            dynamic_list_->next;
        delete[] p;
    }
}

basic_copied_strings::
basic_copied_strings(
    string_view s,
    char* local_buf,
    std::size_t local_size) noexcept
    : s_(s)
    , local_buf_(local_buf)
    , local_remain_(local_size)
{
}

string_view
basic_copied_strings::
maybe_copy(
    string_view s)
{
    if(! is_overlapping(s))
        return s;
    if(local_remain_ >= s.size())
    {
        std::memcpy(local_buf_,
            s.data(), s.size());
        s = string_view(
            local_buf_, s.size());
        local_buf_ += s.size();
        local_remain_ -= s.size();
        return s;
    }
    auto const n =
        sizeof(dynamic_buf);
    auto p = new dynamic_buf[1 +
        sizeof(n) * ((s.size() +
            sizeof(n) - 1) /
                sizeof(n))];
    std::memcpy(p + 1,
        s.data(), s.size());
    s = string_view(reinterpret_cast<
        char const*>(p + 1), s.size());
    p->next = dynamic_list_;
    dynamic_list_ = p;
    return s;
}

} // detail
} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_IMPL_EXCEPT_IPP
#define BOOST_URL_DETAIL_IMPL_EXCEPT_IPP

#include <boost/throw_exception.hpp>
#include <new>
#include <stdexcept>

namespace boost {
namespace urls {
namespace detail {

void
throw_bad_alloc(
    source_location const& loc)
{
    throw_exception(
        std::bad_alloc(), loc);
}

void
throw_length_error(
    char const* what,
    source_location const& loc)
{
    throw_exception(
        std::length_error(what), loc);
}

void
throw_invalid_argument(
    source_location const& loc)
{
    throw_exception(
        std::invalid_argument(""), loc);
}

void
throw_invalid_argument(
    char const* what,
    source_location const& loc)
{
    throw_exception(
        std::invalid_argument(what), loc);
}

void
throw_out_of_range(
    source_location const& loc)
{
    throw_exception(
        std::out_of_range(
            "out of range"), loc);
}

void
throw_system_error(
    error_code const& ec,
    source_location const& loc)
{
    throw_exception(
        system_error(ec), loc);
}

} // detail
} // url
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
// Copyright (c) 2022 Alan Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_IMPL_NORMALIZE_IPP
#define BOOST_URL_DETAIL_IMPL_NORMALIZE_IPP

//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
// Copyright (c) 2022 Alan Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_NORMALIZED_HPP
#define BOOST_URL_DETAIL_NORMALIZED_HPP


namespace boost {
namespace urls {
namespace detail {

class fnv_1a
{
public:
    using digest_type = std::size_t;

#if BOOST_URL_ARCH == 64
    static constexpr std::size_t const prime =
        static_cast<std::size_t>(0x100000001B3ULL);
    static constexpr std::size_t init_hash  =
        static_cast<std::size_t>(0xcbf29ce484222325ULL);
#else
    static constexpr std::size_t const prime =
        static_cast<std::size_t>(0x01000193UL);
    static constexpr std::size_t init_hash  =
        static_cast<std::size_t>(0x811C9DC5UL);
#endif

    explicit
    fnv_1a(std::size_t salt) noexcept
    : h_(init_hash + salt)
    {
    }

    void
    put(char c) noexcept
    {
        h_ ^= c;
        h_ *= prime;
    }

    void
    put(string_view s) noexcept
    {
        for (char c: s)
        {
            put(c);
        }
    }

    digest_type
    digest() const noexcept
    {
        return h_;
    }

private:
    std::size_t h_;
};

void
pop_encoded_front(
    string_view& s,
    char& c,
    std::size_t& n) noexcept;

// compare two string_views as if they are both
// percent-decoded
int
compare_encoded(
    string_view lhs,
    string_view rhs) noexcept;

// digest a string_view as if it were
// percent-decoded
void
digest_encoded(
    string_view s,
    fnv_1a& hasher) noexcept;

void
digest(
    string_view s,
    fnv_1a& hasher) noexcept;

// check if string_view lhs starts with string_view
// rhs as if they are both percent-decoded. If
// lhs starts with rhs, return number of chars
// matched in the encoded string_view
std::size_t
path_starts_with(
    string_view lhs,
    string_view rhs) noexcept;

// check if string_view lhs ends with string_view
// rhs as if they are both percent-decoded. If
// lhs ends with rhs, return number of chars
// matched in the encoded string_view
std::size_t
path_ends_with(
    string_view lhs,
    string_view rhs) noexcept;

// compare two string_views as if they are both
// percent-decoded and lowercase
int
ci_compare_encoded(
    string_view lhs,
    string_view rhs) noexcept;

// digest a string_view as if it were decoded
// and lowercase
void
ci_digest_encoded(
    string_view s,
    fnv_1a& hasher) noexcept;

// compare two ascii string_views
int
compare(
    string_view lhs,
    string_view rhs) noexcept;

// compare two string_views as if they are both
// lowercase
int
ci_compare(
    string_view lhs,
    string_view rhs) noexcept;

// digest a string_view as if it were lowercase
void
ci_digest(
    string_view s,
    fnv_1a& hasher) noexcept;

} // detail
} // urls
} // boost

#endif
#include <boost/assert.hpp>
#include <cstring>

namespace boost {
namespace urls {
namespace detail {

void
pop_encoded_front(
    string_view& s,
    char& c,
    std::size_t& n) noexcept
{
    if(s.front() != '%')
    {
        c = s.front();
        s.remove_prefix(1);
    }
    else
    {
        pct_decode_unchecked(
            &c,
            &c + 1,
            s.substr(0, 3));
        s.remove_prefix(3);
    }
    ++n;
}

int
compare_encoded(
    string_view lhs,
    string_view rhs) noexcept
{
    std::size_t n0 = 0;
    std::size_t n1 = 0;
    char c0 = 0;
    char c1 = 0;
    while(
        !lhs.empty() &&
        !rhs.empty())
    {
        pop_encoded_front(lhs, c0, n0);
        pop_encoded_front(rhs, c1, n1);
        if (c0 < c1)
            return -1;
        if (c1 < c0)
            return 1;
    }
    n0 += pct_decode_bytes_unchecked(lhs);
    n1 += pct_decode_bytes_unchecked(rhs);
    if (n0 == n1)
        return 0;
    if (n0 < n1)
        return -1;
    return 1;
}

void
digest_encoded(
    string_view s,
    fnv_1a& hasher) noexcept
{
    char c = 0;
    std::size_t n = 0;
    while(!s.empty())
    {
        pop_encoded_front(s, c, n);
        hasher.put(c);
    }
}

int
ci_compare_encoded(
    string_view lhs,
    string_view rhs) noexcept
{
    std::size_t n0 = 0;
    std::size_t n1 = 0;
    char c0 = 0;
    char c1 = 0;
    while (
        !lhs.empty() &&
        !rhs.empty())
    {
        pop_encoded_front(lhs, c0, n0);
        pop_encoded_front(rhs, c1, n1);
        c0 = grammar::ascii_tolower(c0);
        c1 = grammar::ascii_tolower(c1);
        if (c0 < c1)
            return -1;
        if (c1 < c0)
            return 1;
    }
    n0 += pct_decode_bytes_unchecked(lhs);
    n1 += pct_decode_bytes_unchecked(rhs);
    if (n0 == n1)
        return 0;
    if (n0 < n1)
        return -1;
    return 1;
}

void
ci_digest_encoded(
    string_view s,
    fnv_1a& hasher) noexcept
{
    char c = 0;
    std::size_t n = 0;
    while(!s.empty())
    {
        pop_encoded_front(s, c, n);
        c = grammar::ascii_tolower(c);
        hasher.put(c);
    }
}

int
compare(
    string_view lhs,
    string_view rhs) noexcept
{
    auto rlen = (std::min)(lhs.size(), rhs.size());
    for (std::size_t i = 0; i < rlen; ++i)
    {
        char c0 = lhs[i];
        char c1 = rhs[i];
        if (c0 < c1)
            return -1;
        if (c1 < c0)
            return 1;
    }
    if ( lhs.size() == rhs.size() )
        return 0;
    if ( lhs.size() < rhs.size() )
        return -1;
    return 1;
}

int
ci_compare(
    string_view lhs,
    string_view rhs) noexcept
{
    auto rlen = (std::min)(lhs.size(), rhs.size());
    for (std::size_t i = 0; i < rlen; ++i)
    {
        char c0 = grammar::ascii_tolower(lhs[i]);
        char c1 = grammar::ascii_tolower(rhs[i]);
        if (c0 < c1)
            return -1;
        if (c1 < c0)
            return 1;
    }
    if ( lhs.size() == rhs.size() )
        return 0;
    if ( lhs.size() < rhs.size() )
        return -1;
    return 1;
}

void
ci_digest(
    string_view s,
    fnv_1a& hasher) noexcept
{
    for (char c: s)
    {
        c = grammar::ascii_tolower(c);
        hasher.put(c);
    }
}

std::size_t
path_starts_with(
    string_view lhs,
    string_view rhs) noexcept
{
    auto consume_one = [](
        string_view::iterator& it,
        char &c)
    {
        if(*it != '%')
        {
            c = *it;
            ++it;
            return;
        }
        pct_decode_unchecked(
            &c,
            &c + 1,
            string_view(it, 3));
        if (c != '/')
        {
            it += 3;
            return;
        }
        c = *it;
        ++it;
    };

    auto it0 = lhs.begin();
    auto it1 = rhs.begin();
    auto end0 = lhs.end();
    auto end1 = rhs.end();
    char c0 = 0;
    char c1 = 0;
    while (
        it0 < end0 &&
        it1 < end1)
    {
        consume_one(it0, c0);
        consume_one(it1, c1);
        if (c0 != c1)
            return 0;
    }
    if (it1 == end1)
        return it0 - lhs.begin();
    return 0;
}

std::size_t
path_ends_with(
    string_view lhs,
    string_view rhs) noexcept
{
    auto consume_last = [](
        string_view::iterator& it,
        string_view::iterator& end,
        char& c)
    {
        if ((end - it) < 3 ||
            *(std::prev(end, 3)) != '%')
        {
            c = *--end;
            return;
        }
        pct_decode_unchecked(
            &c,
            &c + 1,
            string_view(std::prev(
                end, 3), 3));
        if (c != '/')
        {
            end -= 3;
            return;
        }
        c = *--end;
    };

    auto it0 = lhs.begin();
    auto it1 = rhs.begin();
    auto end0 = lhs.end();
    auto end1 = rhs.end();
    char c0 = 0;
    char c1 = 0;
    while(
        it0 < end0 &&
        it1 < end1)
    {
        consume_last(it0, end0, c0);
        consume_last(it1, end1, c1);
        if (c0 != c1)
            return 0;
    }
    if (it1 == end1)
        return lhs.end() - end0;
    return 0;
}

} // detail
} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_IMPL_PATH_IPP
#define BOOST_URL_DETAIL_IMPL_PATH_IPP

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_PATH_HPP
#define BOOST_URL_DETAIL_PATH_HPP


namespace boost {
namespace urls {
namespace detail {

// Return the number of characters at
// the front of the path that are reserved
inline
std::size_t
path_prefix(
    string_view s) noexcept
{
    switch(s.size())
    {
    case 0:
        return 0;

    case 1:
        if(s[0] == '/')
            return 1;
        return 0;

    case 2:
        if(s[0] == '/')
            return 1;
        if( s[0] == '.' &&
            s[1] == '/')
            return 2;
        return 0;

    default:
        if(s[0] == '/')
        {
            if( s[1] == '.' &&
                s[2] == '/')
                return 3;
            return 1;
        }
        if( s[0] == '.' &&
            s[1] == '/')
            return 2;
        break;
    }
    return 0;
}

// returns the number of adjusted
// segments based on the malleable prefix.
inline
std::size_t
path_segments(
    string_view s,
    std::size_t nseg) noexcept
{
    switch(s.size())
    {
    case 0:
        BOOST_ASSERT(nseg == 0);
        return 0;

    case 1:
        BOOST_ASSERT(nseg == 1);
        if(s[0] == '/')
            return 0;
        return 1;

    case 2:
        if(s[0] == '/')
            return nseg;
        if( s[0] == '.' &&
            s[1] == '/')
        {
            BOOST_ASSERT(nseg > 1);
            return nseg - 1;
        }
        return nseg;

    default:
        if(s[0] == '/')
        {
            if( s[1] == '.' &&
                s[2] == '/')
            {
                BOOST_ASSERT(nseg > 1);
                return nseg - 1;
            }
            return nseg;
        }
        if( s[0] == '.' &&
            s[1] == '/')
        {
            BOOST_ASSERT(nseg > 1);
            return nseg - 1;
        }
        break;
    }
    return nseg;
}

// Trim reserved characters from
// the front of the path.
inline
string_view
clean_path(
    string_view s) noexcept
{
    s.remove_prefix(
        path_prefix(s));
    return s;
}

} // detail
} // urls
} // boost

#endif
namespace boost {
namespace urls {
namespace detail {

} // detail
} // url
} // boost

#endif//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
// Copyright (c) 2022 Alan Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_IMPL_PCT_ENCODING_IPP
#define BOOST_URL_DETAIL_IMPL_PCT_ENCODING_IPP

#include <boost/assert.hpp>
#include <cstring>

namespace boost {
namespace urls {
namespace detail {

// returns true if plain_key == decode(encoded)
bool
key_equal_encoded(
    string_view plain_key,
    pct_encoded_str encoded) noexcept
{
    if(plain_key.size() !=
        encoded.decoded_size)
        return false; // trivial reject
    if(encoded.str.size() ==
        encoded.decoded_size)
    {
        // plain comparison
        return std::memcmp(
            plain_key.data(),
            encoded.str.data(),
            plain_key.size()) == 0;
    }
    return key_equal_encoded(
        plain_key, encoded.str);
}

// returns true if plain_key == decode(encoded)
bool
key_equal_encoded(
    string_view plain_key,
    string_view encoded) noexcept
{
    auto it0 = plain_key.data();
    auto it1 = encoded.data();
    auto const end0 = it0 + plain_key.size();
    auto const end1 = it1 + encoded.size();
    for(;;)
    {
        if(it1 == end1)
            return it0 == end0;
        if(it0 == end0)
            return false;
        if(*it1 != '%')
        {
            if(*it1++ != *it0++)
                return false;
            continue;
        }
        // VFALCO Was
        // BOOST_ASSERT(end1 - it1 >= 3);
        if(end1 - it1 < 3)
            return false;
        char h1;
        grammar::hexdig_value(it1[1], h1);
        char h2;
        grammar::hexdig_value(it1[2], h2);
        auto const ch = static_cast<char>(
            (static_cast<unsigned char>(
                h1) << 4) +
            static_cast<unsigned char>(
                h2));
        if(ch != *it0++)
            return false;
        it1 += 3;
    }
}

} // detail
} // urls
} // boost

#endif
//
// Copyright (c) 2022 alandefreitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_IMPL_REMOVE_DOT_SEGMENTS_IPP
#define BOOST_URL_DETAIL_IMPL_REMOVE_DOT_SEGMENTS_IPP

//
// Copyright (c) 2022 alandefreitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_REMOVE_DOT_SEGMENTS_HPP
#define BOOST_URL_DETAIL_REMOVE_DOT_SEGMENTS_HPP

#include <cstdint>

namespace boost {
namespace urls {
namespace detail {

std::size_t
remove_dot_segments(
    char* dest,
    char const* end,
    string_view s,
    bool remove_unmatched) noexcept;

void
pop_last_segment(
    string_view& s,
    string_view& c,
    std::size_t& level,
    bool r) noexcept;

char
path_pop_back( string_view& s );

int
normalized_path_compare(
    string_view lhs,
    string_view rhs,
    bool remove_unmatched_lhs,
    bool remove_unmatched_rhs) noexcept;

void
normalized_path_digest(
    string_view s,
    bool remove_unmatched,
    fnv_1a& hasher) noexcept;

} // detail
} // urls
} // boost

#endif
#include <boost/assert.hpp>
#include <cstring>

namespace boost {
namespace urls {
namespace detail {

std::size_t
remove_dot_segments(
    char* dest0,
    char const* end,
    string_view s,
    bool remove_unmatched) noexcept
{
    // 1. The input buffer is initialized with
    // the now-appended path components and the
    // output buffer is initialized to the empty
    // string.
    char* dest = dest0;
    auto append =
        [&dest, &end]
        (string_view in)
    {
        BOOST_ASSERT(in.size() <= std::size_t(end - dest));
        std::memcpy(dest, in.data(), in.size());
        dest += in.size();
        (void)end;
    };

    auto find_last_slash =
        [&dest0, &dest]() -> std::size_t
    {
        char* const first = dest0;
        char* last = dest;
        while (last != first)
        {
            --last;
            if (*last == '/')
                return last - first;
        }
        return string_view::npos;
    };

    // Step 2 is a loop through 5 production rules
    // There are no transitions between all rules,
    // which enables some optimizations.
    // A.  If the input buffer begins with a
    // prefix of "../" or "./", then remove
    // that prefix from the input buffer;
    // otherwise,
    // Rule A can only happen at the beginning:
    // - B and C write "/" to the output
    // - D can only happen at the end
    // - E leaves "/" or happens at the end
    while (!s.empty())
    {
        if (s.starts_with("../"))
        {
            if (!remove_unmatched)
                append(s.substr(0, 3));
            s.remove_prefix(3);
            continue;
        }
        if (!s.starts_with("./"))
            break;
        s.remove_prefix(2);
    }

    // D.  if the input buffer consists only
    // of "." or "..", then remove that from
    // the input buffer; otherwise,
    // Rule D can only happen after A is consumed:
    // - B and C write "/" to the output
    // - D can only happen at the end
    // - E leaves "/" or happens at the end
    if( s == "." ||
        s == "..")
    {
        if( ! remove_unmatched &&
                s == "..")
            append(s);
        s = {};
    }

    // 2. While the input buffer is not empty,
    // loop as follows:
    while (!s.empty())
    {
        // B.  if the input buffer begins with a
        // prefix of "/./" or "/.", where "." is
        // a complete path segment, then replace
        // that prefix with "/" in the input
        // buffer; otherwise,
        if (s.starts_with("/./"))
        {
            s.remove_prefix(2);
            continue;
        }
        if (s == "/.")
        {
            // equivalent to replacing s with '/'
            // and executing the next iteration
            append(s.substr(0, 1));
            s.remove_prefix(2);
            continue;
        }

        // C. if the input buffer begins with a
        // prefix of "/../" or "/..", where ".."
        // is a complete path segment, then
        // replace that prefix with "/" in the
        // input buffer and remove the last
        // segment and its preceding "/"
        // (if any) from the output buffer;
        // otherwise,
        if (s.starts_with("/../"))
        {
            std::size_t p = find_last_slash();
            if (p != string_view::npos)
                // "erase" [p, end]
                dest = dest0 + p;
            else if (!remove_unmatched)
                append(s.substr(0, 3));
            s.remove_prefix(3);
            continue;
        }
        if (s == "/..")
        {
            std::size_t p = find_last_slash();
            if (p != string_view::npos)
            {
                // erase [p, end]
                dest = dest0 + p;
                // equivalent to replacing s with '/'
                // and executing the next iteration.
                // this is the only point that would
                // require input memory allocations
                // in remove_dot_segments
                append(s.substr(0, 1));
            }
            else if (remove_unmatched)
                append(s.substr(0, 1));
            else
                append(s.substr(0, 3));
            s.remove_prefix(3);
            continue;
        }

        // E.  move the first path segment in the
        // input buffer to the end of the output
        // buffer, including the initial "/"
        // character (if any) and any subsequent
        // characters up to, but not including,
        // the next "/" character or the end of
        // the input buffer.
        std::size_t p = s.find_first_of('/', 1);
        if (p != string_view::npos)
        {
            append(s.substr(0, p));
            s.remove_prefix(p);
        }
        else
        {
            append(s);
            s = {};
        }
    }

    // 3. Finally, the output buffer is returned
    // as the result of remove_dot_segments.
    return dest - dest0;
}

char
path_pop_back( string_view& s )
{
    if (s.size() < 3 ||
        *std::prev(s.end(), 3) != '%')
    {
        char c = s.back();
        s.remove_suffix(1);
        return c;
    }
    char c = 0;
    pct_decode_unchecked(
        &c, &c + 1, s.substr(s.size() - 3));
    if (c != '/')
    {
        s.remove_suffix(3);
        return c;
    }
    c = s.back();
    s.remove_suffix(1);
    return c;
};

void
pop_last_segment(
    string_view& s,
    string_view& c,
    std::size_t& level,
    bool r) noexcept
{
    c = {};
    std::size_t n = 0;
    while (!s.empty())
    {
        // B.  if the input buffer begins with a
        // prefix of "/./" or "/.", where "." is
        // a complete path segment, then replace
        // that prefix with "/" in the input
        // buffer; otherwise,
        n = detail::path_ends_with(s, "/./");
        if (n)
        {
            c = s.substr(s.size() - n);
            s.remove_suffix(n);
            continue;
        }
        n = detail::path_ends_with(s, "/.");
        if (n)
        {
            c = s.substr(s.size() - n, 1);
            s.remove_suffix(n);
            continue;
        }

        // C. if the input buffer begins with a
        // prefix of "/../" or "/..", where ".."
        // is a complete path segment, then
        // replace that prefix with "/" in the
        // input buffer and remove the last
        // segment and its preceding "/"
        // (if any) from the output buffer
        // otherwise,
        n = detail::path_ends_with(s, "/../");
        if (n)
        {
            c = s.substr(s.size() - n);
            s.remove_suffix(n);
            ++level;
            continue;
        }
        n = detail::path_ends_with(s, "/..");
        if (n)
        {
            c = s.substr(s.size() - n);
            s.remove_suffix(n);
            ++level;
            continue;
        }

        // E.  move the first path segment in the
        // input buffer to the end of the output
        // buffer, including the initial "/"
        // character (if any) and any subsequent
        // characters up to, but not including,
        // the next "/" character or the end of
        // the input buffer.
        std::size_t p = s.size() > 1
            ? s.find_last_of('/', s.size() - 2)
            : string_view::npos;
        if (p != string_view::npos)
        {
            c = s.substr(p + 1);
            s.remove_suffix(c.size());
        }
        else
        {
            c = s;
            s = {};
        }

        if (level == 0)
            return;
        if (!s.empty())
            --level;
    }
    // we still need to skip n_skip + 1
    // but the string is empty
    if (r && level)
    {
        c = "/";
        level = 0;
        return;
    }
    else if (level)
    {
        if (c.empty())
            c = "/..";
        else
            c = "/../";
        --level;
        return;
    }
    c = {};
}


int
normalized_path_compare(
    string_view s0_init,
    string_view s1_init,
    bool r0,
    bool r1) noexcept
{
    // Pseudocode:
    // Execute remove_dot_segments iterations in reverse:
    // - keep track of number of elements
    // - keep track of normalized size
    // Iterate the both path segments in reverse again:
    // - use normalized size to identify the
    //   positions we are comparing

    // 1. The input buffer is initialized with
    // the now-appended path components and the
    // output buffer is initialized to the empty
    // string.
    // - These memory allocations are logically removed.

    // Step 2 is a loop through 5 production rules
    // There are no transitions between all rules,
    // which enables some optimizations, such as
    // ignoring the prefix rules before applying
    // other rules.
    auto remove_prefix =
        []( string_view& s )
    {
        // A.  If the input buffer begins with a
        // prefix of "../" or "./", then remove
        // that prefix from the input buffer,
        // otherwise,
        // Rule A can only happen at the beginning:
        // - B and C write "/" to the output
        // - D can only happen at the end
        // - E leaves "/" or happens at the end
        std::size_t out = 0;
        std::size_t n = 0;
        while (!s.empty())
        {
            n = detail::path_starts_with(s, "../");
            if (n)
            {
                out += 3;
                s.remove_prefix(n);
                continue;
            }
            n = detail::path_starts_with(s, "./");
            if (n)
            {
                s.remove_prefix(n);
                continue;
            }
            break;
        }

        // D.  if the input buffer consists only
        // of "." or "..", then remove that from
        // the input buffer; otherwise, ...
        // Rule D can only happen after A is consumed:
        // - B and C write "/" to the output
        // - D can only happen at the end
        // - E leaves "/" or happens at the end
        if (detail::compare_encoded(s, ".") == 0)
            s = {};
        else if (detail::compare_encoded(s, "..") == 0)
        {
            out += 2;
            s = {};
        }
        return out;
    };
    string_view s0 = s0_init;
    string_view s1 = s1_init;
    std::size_t s0_prefix_n = remove_prefix(s0);
    std::size_t s1_prefix_n = remove_prefix(s1);

    // number of decoded bytes in a path segment
    auto path_decoded_bytes =
        []( string_view s )
    {
        auto it = s.data();
        auto const end =
            it + s.size();
        std::size_t n = 0;
        while(it < end)
        {
            if(*it != '%')
            {
                // unescaped
                ++it;
                ++n;
                continue;
            }
            if(end - it < 3)
                return n;
            char c = 0;
            pct_decode_unchecked(
                &c,
                &c + 1,
                string_view(it, 3));
            if (c != '/')
                it += 3;
            else
                ++it;
            ++n;
        }
        return n;
    };

    // Calculate the normalized size
    auto norm_bytes =
        [&path_decoded_bytes]
        ( string_view p,
          bool r)
    {
        string_view c;
        std::size_t s{0};
        std::size_t n{0};
        do
        {
            pop_last_segment(p, c, s, r);
            n += path_decoded_bytes(c);
        }
        while (!c.empty());
        return n;
    };
    std::size_t s0n = norm_bytes(
        s0,
        r0);
    if (!r0)
        s0n += s0_prefix_n;

    std::size_t s1n = norm_bytes(
        s1,
        r1);
    if (!r1)
        s1n += s1_prefix_n;

    // Remove child segments until last intersection
    s0 = s0_init;
    s1 = s1_init;
    string_view s0c;
    string_view s1c;
    std::size_t s0l = 0;
    std::size_t s1l = 0;
    std::size_t s0i = s0n;
    std::size_t s1i = s1n;
    pop_last_segment(
        s0, s0c, s0l,
        r0);
    pop_last_segment(
        s1, s1c, s1l,
        r1);

    // Consume incomparable segments
    auto pop_decoded_back =
        []( string_view& s )
    {
        if (s.size() < 3 ||
            *std::prev(s.end(), 3) != '%')
        {
            char c = s.back();
            s.remove_suffix(1);
            return c;
        }
        char c = 0;
        pct_decode_unchecked(
            &c, &c + 1, s.substr(s.size() - 3));
        if (c != '/')
        {
            s.remove_suffix(3);
            return c;
        }
        c = s.back();
        s.remove_suffix(1);
        return c;
    };

    while (s0i != s1i)
    {
        // Consume more child segments
        if (s0c.empty())
            pop_last_segment(
                s0, s0c, s0l, r0);
        if (s1c.empty())
            pop_last_segment(
                s1, s1c, s1l, r1);

        // Remove incomparable suffix
        while (
            !s0c.empty() &&
            !s1c.empty())
        {
            if (s1i > s0i)
            {
                pop_decoded_back(s1c);
                --s1i;
                continue;
            }
            else if (s0i > s1i)
            {
                pop_decoded_back(s0c);
                --s0i;
                continue;
            }
            break;
        }
    }

    int cmp = 0;
    BOOST_ASSERT(s0i == s1i);
    while (s0i > 0)
    {
        // Consume more child segments
        if (s0c.empty())
            pop_last_segment(
                s0, s0c, s0l, r0);
        if (s1c.empty())
            pop_last_segment(
                s1, s1c, s1l, r1);

        // Compare intersection
        while (
            !s0c.empty() &&
            !s1c.empty())
        {
            BOOST_ASSERT(s0i == s1i);
            char c0 = pop_decoded_back(s0c);
            char c1 = pop_decoded_back(s1c);
            if (c0 < c1)
                cmp = -1;
            else if (c1 < c0)
                cmp = 1;
            --s0i;
            --s1i;
        }
    }

    if (cmp != 0)
        return cmp;
    if (s0n == s1n )
        return 0;
    if (s0n < s1n )
        return -1;
    return 1;
}

void
normalized_path_digest(
    string_view s,
    bool remove_unmatched,
    fnv_1a& hasher) noexcept
{
    string_view child;
    std::size_t level = 0;
    do
    {
        pop_last_segment(
            s, child, level, remove_unmatched);
        while (!child.empty())
        {
            char c = path_pop_back(child);
            hasher.put(c);
        }
    }
    while (!s.empty());
}

} // detail
} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_IMPL_PARAMS_ENCODED_ITERATOR_IMPL_IPP
#define BOOST_URL_DETAIL_IMPL_PARAMS_ENCODED_ITERATOR_IMPL_IPP

#include <boost/assert.hpp>

namespace boost {
namespace urls {
namespace detail {

void
params_encoded_iterator_impl::
scan() noexcept
{
    string_view s(pos_, end_ - pos_);
    std::size_t i;
    if(pos_ != begin_ || i_ != 0)
    {
        BOOST_ASSERT(
            s.starts_with('&'));
        i = s.find_first_of('&', 1);
    }
    else
    {
        i = s.find_first_of('&');
    }
    if( i == string_view::npos)
        i = s.size();
    nk_ = string_view(
        pos_, i).find_first_of('=');
    if(nk_ != string_view::npos)
    {
        nv_ = i - nk_;
    }
    else
    {
        // has_value==false
        nk_ = i;
        nv_ = 0;
    }
}

params_encoded_iterator_impl::
params_encoded_iterator_impl(
    string_view s) noexcept
    : begin_(s.data())
    , pos_(s.data())
    , end_(s.data() + s.size())
{
    scan();
}

params_encoded_iterator_impl::
params_encoded_iterator_impl(
    string_view s,
    std::size_t nparam,
    int) noexcept
    : i_(nparam)
    , begin_(s.data())
    , pos_(s.data() + s.size())
    , end_(s.data() + s.size())
{
}

void
params_encoded_iterator_impl::
increment() noexcept
{
    BOOST_ASSERT(begin_ != nullptr);
    BOOST_ASSERT(end_ != nullptr);
    BOOST_ASSERT(pos_ != nullptr);
    BOOST_ASSERT(pos_ != end_);

    ++i_;
    pos_ += nk_ + nv_;
    if(pos_ == end_)
    {
        nk_ = 0;
        nv_ = 0;
        return;
    }
    scan();
}

string_view
params_encoded_iterator_impl::
encoded_key() const noexcept
{
    BOOST_ASSERT(begin_ != nullptr);
    BOOST_ASSERT(end_ != nullptr);
    BOOST_ASSERT(pos_ != end_);
    BOOST_ASSERT(pos_ != nullptr);

    if(pos_ != begin_)
        return string_view(
            pos_ + 1, nk_ - 1);
    return string_view{ pos_, nk_ };
}

auto
params_encoded_iterator_impl::
dereference() const ->
    query_param_view
{
    BOOST_ASSERT(begin_ != nullptr);
    BOOST_ASSERT(end_ != nullptr);

    BOOST_ASSERT(pos_ != end_);

    BOOST_ASSERT(pos_ != nullptr);

    using reference = query_param_view;
    int prefix = pos_ != begin_ || i_ != 0;
    if(nv_ > 0)
        return reference{
            string_view(
                pos_ + prefix, nk_ - prefix),
            string_view(
                pos_ + nk_ + 1, nv_ - 1),
            true};
    return reference{
        string_view(
            pos_ + prefix, nk_ - prefix),
        string_view{},
        false};
}

} // detail
} // url
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_IMPL_PARAMS_ITERATOR_IMPL_IPP
#define BOOST_URL_DETAIL_IMPL_PARAMS_ITERATOR_IMPL_IPP

#include <boost/assert.hpp>

namespace boost {
namespace urls {
namespace detail {

void
params_iterator_impl::
scan() noexcept
{
    string_view s(pos_, end_ - pos_);
    std::size_t i;
    if(pos_ != begin_ || i_ != 0)
    {
        BOOST_ASSERT(
            s.starts_with('&'));
        i = s.find_first_of('&', 1);
    }
    else
    {
        i = s.find_first_of('&');
    }
    if( i == string_view::npos)
        i = s.size();
    nk_ = string_view(
        pos_, i).find_first_of('=');
    if(nk_ != string_view::npos)
    {
        nv_ = i - nk_;
    }
    else
    {
        // has_value==false
        nk_ = i;
        nv_ = 0;
    }
}

params_iterator_impl::
params_iterator_impl(
    string_view s,
    const_string::factory a) noexcept
    : begin_(s.data())
    , pos_(s.data())
    , end_(s.data() + s.size())
    , a_(a)
{
    scan();
}

params_iterator_impl::
params_iterator_impl(
    string_view s,
    std::size_t nparam,
    int,
    const_string::factory a) noexcept
    : i_(nparam)
    , begin_(s.data())
    , pos_(s.data() + s.size())
    , end_(s.data() + s.size())
    , a_(a)
{
}



params_view::reference
params_iterator_impl::
dereference() const noexcept
{
    BOOST_ASSERT(begin_ != nullptr);
    BOOST_ASSERT(end_ != nullptr);
    BOOST_ASSERT(pos_ != end_);
    BOOST_ASSERT(pos_ != nullptr);

    int prefix = pos_ != begin_ || i_ != 0;
    if(nv_ > 0)
        return params_view::reference{
            pos_ + prefix,
            nk_ - prefix,
            nv_,
            a_};
    return params_view::reference{
        pos_ + prefix,
        nk_ - prefix,
        nv_,
        a_};
}


void
params_iterator_impl::
increment() noexcept
{
    BOOST_ASSERT(begin_ != nullptr);
    BOOST_ASSERT(end_ != nullptr);
    BOOST_ASSERT(pos_ != nullptr);
    BOOST_ASSERT(pos_ != end_);

    ++i_;
    pos_ += nk_ + nv_;
    if(pos_ == end_)
    {
        nk_ = 0;
        nv_ = 0;
        return;
    }
    scan();
}

string_view
params_iterator_impl::
encoded_key() const noexcept
{
    BOOST_ASSERT(begin_ != nullptr);
    BOOST_ASSERT(end_ != nullptr);
    BOOST_ASSERT(pos_ != end_);
    BOOST_ASSERT(pos_ != nullptr);

    if(pos_ != begin_)
        return string_view(
            pos_ + 1, nk_ - 1);
    return string_view{ pos_, nk_ };
}


} // detail
} // url
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_IMPL_SEGMENTS_ENCODED_ITERATOR_IMPL_IPP
#define BOOST_URL_DETAIL_IMPL_SEGMENTS_ENCODED_ITERATOR_IMPL_IPP

#include <boost/assert.hpp>

namespace boost {
namespace urls {
namespace detail {

segments_encoded_iterator_impl::
segments_encoded_iterator_impl(
    string_view s,
    std::size_t nseg) noexcept
    : begin_(s.data())
    , pos_(s.data())
    , next_(s.data())
    , end_(s.data() + s.size())
{
    if(nseg == 0)
    {
        next_ = nullptr;
        return;
    }
    auto const n = path_prefix(s);
    begin_ += n;
    next_ += n;
    pos_ += n;
    auto const i = string_view(
        begin_, s.size() - n
            ).find_first_of('/');
    if(i != string_view::npos)
        next_ += i;
    else
        next_ = end_;
    s_ = string_view(
        pos_, next_ - pos_);
}

segments_encoded_iterator_impl::
segments_encoded_iterator_impl(
    string_view s,
    std::size_t nseg,
    int) noexcept
    : i_(nseg)
    , begin_(s.data())
    , pos_(s.data() + s.size())
    , end_(s.data() + s.size())
{
    auto const n = path_prefix(s);
    begin_ += n;
}

void
segments_encoded_iterator_impl::
increment() noexcept
{
    using bnf_t =
        path_rootless_rule;
    BOOST_ASSERT(next_ != nullptr);
    ++i_;
    pos_ = next_;
    error_code ec;
    // "/" segment
    pct_encoded_str t;
    bnf_t::increment(
        next_, end_, ec, t);
    if(ec == grammar::error::end)
    {
        next_ = nullptr;
        return;
    }
    BOOST_ASSERT(! ec);
    s_ = t.str;
}

void
segments_encoded_iterator_impl::
decrement() noexcept
{
    using bnf_t =
        path_rootless_rule;
    BOOST_ASSERT(i_ != 0);
    --i_;
    if(i_ == 0)
    {
        next_ = pos_;
        pos_ = begin_;
        s_ = string_view(
            pos_, next_ - pos_);
        return;
    }
    error_code ec;
    while(--pos_ != begin_)
    {
        if(*pos_ != '/')
            continue;
        // "/" segment
        next_ = pos_;
        pct_encoded_str t;
        bnf_t::increment(next_,
            end_, ec, t);
        BOOST_ASSERT(! ec);
        s_ = t.str;
        return;
    }
    next_ = pos_;
    if(*next_ == '/')
    {
        // "/" segment
        pct_encoded_str t;
        bnf_t::increment(next_,
            end_, ec, t);
        BOOST_ASSERT(! ec);
        s_ = t.str;
    }
    else
    {
        // segment-nz
        pct_encoded_str t;
        bnf_t::begin(next_,
            end_, ec, t);
        BOOST_ASSERT(! ec);
        s_ = t.str;
    }
}


} // detail
} // url
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_IMPL_SEGMENTS_ITERATOR_IMPL_IPP
#define BOOST_URL_DETAIL_IMPL_SEGMENTS_ITERATOR_IMPL_IPP

#include <boost/assert.hpp>

namespace boost {
namespace urls {
namespace detail {

segments_iterator_impl::
segments_iterator_impl(
    string_view s,
    std::size_t nseg,
    const_string::factory const& a) noexcept
    : begin_(s.data())
    , pos_(s.data())
    , next_(s.data())
    , end_(s.data() + s.size())
    , a_(a)
{
    if(nseg == 0)
    {
        next_ = nullptr;
        return;
    }
    auto const n = path_prefix(s);
    begin_ += n;
    next_ += n;
    pos_ += n;
    error_code ec;
    grammar::parse(
        next_, end_, ec,
        segment_rule{t_});
    BOOST_ASSERT(! ec);
}

segments_iterator_impl::
segments_iterator_impl(
    string_view s,
    std::size_t nseg,
    const_string::factory const& a,
    int) noexcept
    : i_(nseg)
    , begin_(s.data())
    , pos_(s.data() + s.size())
    , end_(s.data() + s.size())
    , a_(a)
{
    auto const n = path_prefix(s);
    begin_ += n;
}

const_string
segments_iterator_impl::
dereference() const noexcept
{
    return a_(t_.decoded_size,
        [this](
            std::size_t, char* dest)
        {
            pct_decode_opts opt;
            opt.plus_to_space = false;
            pct_decode_unchecked(
                dest, dest + t_.decoded_size,
                    t_.str, opt);
        });
}


void
segments_iterator_impl::
increment() noexcept
{
    using bnf_t = path_rootless_rule;
    BOOST_ASSERT(next_ != nullptr);
    ++i_;
    pos_ = next_;
    error_code ec;
    // "/" segment
    bnf_t::increment(
        next_, end_, ec, t_);
    if(ec == grammar::error::end)
    {
        next_ = nullptr;
        return;
    }
    BOOST_ASSERT(! ec);
}

void
segments_iterator_impl::
decrement() noexcept
{
    using bnf_t = path_rootless_rule;
    BOOST_ASSERT(i_ != 0);
    --i_;
    error_code ec;
    if(i_ == 0)
    {
        next_ = begin_;
        pos_ = begin_;
        grammar::parse(
            next_, end_, ec,
            segment_rule{t_});
        BOOST_ASSERT(! ec.failed());
        return;
    }
    while(--pos_ != begin_)
    {
        if(*pos_ != '/')
            continue;
        // "/" segment
        next_ = pos_;
        bnf_t::increment(next_,
                         end_, ec, t_);
        BOOST_ASSERT(! ec);
        return;
    }
    next_ = pos_;
    if(*next_ == '/')
    {
        // "/" segment
        bnf_t::increment(next_,
                         end_, ec, t_);
        BOOST_ASSERT(! ec);
    }
    else
    {
        // segment-nz
        bnf_t::begin(next_,
                     end_, ec, t_);
        BOOST_ASSERT(! ec);
    }
}


} // detail
} // url
} // boost

#endif

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_AUTHORITY_VIEW_IPP
#define BOOST_URL_IMPL_AUTHORITY_VIEW_IPP

#include <array>
#include <ostream>

namespace boost {
namespace urls {

struct authority_view::shared_impl :
    authority_view
{
    virtual
    ~shared_impl()
    {
    }

    shared_impl(
        authority_view const& u) noexcept
        : authority_view(u, reinterpret_cast<
            char const*>(this + 1))
    {
    }
};

authority_view::
authority_view(
    char const* cs) noexcept
    : cs_(cs)
{
}

authority_view::
authority_view(
    authority_view const& u,
    char const* cs) noexcept
    : authority_view(u)
{
    cs_ = cs;
}

//------------------------------------------------

authority_view::
~authority_view()
{
}

authority_view::
authority_view() noexcept = default;

authority_view::
authority_view(
    authority_view const&) noexcept = default;

authority_view&
authority_view::
operator=(
    authority_view const&) noexcept = default;

//------------------------------------------------
//
// Observers
//
//------------------------------------------------

std::shared_ptr<
    authority_view const>
authority_view::
collect() const
{
    using T = shared_impl;
    using Alloc = std::allocator<char>;
    Alloc a;
    auto p = std::allocate_shared<T>(
        detail::over_allocator<T, Alloc>(
            size(), a), *this);
    std::memcpy(
        reinterpret_cast<char*>(
            p.get() + 1), data(), size());
    return p;
}

//----------------------------------------------------------
//
// Authority
//
//----------------------------------------------------------

// userinfo

bool
authority_view::
has_userinfo() const noexcept
{
    auto n = len(id_pass);
    if(n == 0)
        return false;
    BOOST_ASSERT(get(
        id_pass).ends_with('@'));
    return true;
}

string_view
authority_view::
encoded_userinfo() const noexcept
{
    auto s = get(
        id_user, id_host);
    if(s.empty())
        return s;
    if(s.empty())
        return s;
    BOOST_ASSERT(
        s.ends_with('@'));
    s.remove_suffix(1);
    return s;
}

bool
authority_view::
has_password() const noexcept
{
    auto const n = len(id_pass);
    if(n > 1)
    {
        BOOST_ASSERT(get(id_pass
            ).starts_with(':'));
        BOOST_ASSERT(get(id_pass
            ).ends_with('@'));
        return true;
    }
    BOOST_ASSERT(n == 0 || get(
        id_pass).ends_with('@'));
    return false;
}

string_view
authority_view::
encoded_password() const noexcept
{
    auto s = get(id_pass);
    switch(s.size())
    {
    case 1:
        BOOST_ASSERT(
            s.starts_with('@'));
        BOOST_FALLTHROUGH;
    case 0:
        return s.substr(0, 0);
    default:
        break;
    }
    BOOST_ASSERT(
        s.ends_with('@'));
    BOOST_ASSERT(
        s.starts_with(':'));
    return s.substr(1,
        s.size() - 2);
}

// host

string_view
authority_view::
encoded_host() const noexcept
{
    return get(id_host);
}

urls::ipv4_address
authority_view::
ipv4_address() const noexcept
{
    if(host_type_ !=
        urls::host_type::ipv4)
        return {};
    std::array<
        unsigned char, 4> bytes;
    std::memcpy(
        &bytes[0],
        &ip_addr_[0], 4);
    return urls::ipv4_address(
        bytes);
}

urls::ipv6_address
authority_view::
ipv6_address() const noexcept
{
    if(host_type_ ==
        urls::host_type::ipv6)
    {
        std::array<
            unsigned char, 16> bytes;
        std::memcpy(
            &bytes[0],
            &ip_addr_[0], 16);
        return urls::ipv6_address(
            bytes);
    }
    return urls::ipv6_address();
}

string_view
authority_view::
ipv_future() const noexcept
{
    if(host_type_ ==
        urls::host_type::ipvfuture)
        return get(id_host);
    return {};
}

// port

bool
authority_view::
has_port() const noexcept
{
    auto const n = len(id_port);
    if(n == 0)
        return false;
    BOOST_ASSERT(
        get(id_port).starts_with(':'));
    return true;
}

string_view
authority_view::
port() const noexcept
{
    auto s = get(id_port);
    if(s.empty())
        return s;
    BOOST_ASSERT(has_port());
    return s.substr(1);
}

std::uint16_t
authority_view::
port_number() const noexcept
{
    BOOST_ASSERT(
        has_port() ||
        port_number_ == 0);
    return port_number_;
}

string_view
authority_view::
encoded_host_and_port() const noexcept
{
    return get(id_host, id_end);
}

//------------------------------------------------
//
// Parsing
//
//------------------------------------------------

void
authority_view::
apply(
    host_rule const& t) noexcept
{
    host_type_ = t.host_type;
    switch(t.host_type)
    {
    default:
    case urls::host_type::none:
    {
        break;
    }
    case urls::host_type::name:
    {
        decoded_[id_host] =
            t.name.decoded_size;
        break;
    }
    case urls::host_type::ipv4:
    {
        auto const bytes =
            t.ipv4.to_bytes();
        std::memcpy(
            &ip_addr_[0],
            bytes.data(), 4);
        break;
    }
    case urls::host_type::ipv6:
    {
        auto const bytes =
            t.ipv6.to_bytes();
        std::memcpy(
            &ip_addr_[0],
            bytes.data(), 16);
        break;
    }
    case urls::host_type::ipvfuture:
    {
        break;
    }
    }

    if(t.host_type !=
        urls::host_type::none)
    {
        set_size(
            id_host,
            t.host_part.size());
    }
}

void
authority_view::
apply(
    authority_rule const& t) noexcept
{
    if(t.has_userinfo)
    {
        auto const& u = t.userinfo;

        set_size(
            id_user,
            u.user.str.size());
        decoded_[id_user] = u.user.decoded_size;

        if(u.has_password)
        {
            // leading ':' for password,
            // trailing '@' for userinfo
            set_size(
                id_pass,
                u.password.str.size() + 2);
            decoded_[id_pass] =
                u.password.decoded_size;
        }
        else
        {
            // trailing '@' for userinfo
            set_size(id_pass, 1);
            decoded_[id_pass] = 0;
        }
    }
    else
    {
        set_size(id_user, 0);
        decoded_[id_user] = 0;
    }

    // host
    apply(t.host);

    // port
    if(t.port.has_port)
    {
        set_size(
            id_port,
            t.port.port.size() + 1);

        if(t.port.has_number)
            port_number_ =
                t.port.port_number;
    }
}

//------------------------------------------------
//
// Parsing
//
//------------------------------------------------

result<authority_view>
parse_authority(
    string_view s) noexcept
{
    if(s.size() > authority_view::max_size())
        detail::throw_length_error(
            "authority_view::max_size exceeded",
            BOOST_CURRENT_LOCATION);

    error_code ec;
    authority_rule t;
    if(! grammar::parse_string(s, ec, t))
        return ec;

    authority_view a(s.data());

    // authority
    a.apply(t);

    return a;
}

//------------------------------------------------

std::ostream&
operator<<(
    std::ostream& os,
    authority_view const& a)
{
    os << a.encoded_authority();
    return os;
}

} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_CONST_STRING_IPP
#define BOOST_URL_IMPL_CONST_STRING_IPP

#include <memory>
#include <new>

namespace boost {
namespace urls {

const_string::
factory::
~factory()
{
    p_->release();
}

const_string::
factory::
factory(
    factory const& other) noexcept
    : p_(other.p_)
{
    ++p_->refs;
}

auto
const_string::
factory::
operator=(
    factory const& other) noexcept ->
        factory&
{
    ++other.p_->refs;
    p_->release();
    p_ = other.p_;
    return *this;
}

const_string::
factory::
factory() noexcept
    : p_([]
    {
        using A = std::allocator<char>;

        struct default_impl final
            : public factory::base
        {
            void
            destroy() noexcept override
            {
            }

            result
            construct(std::size_t size) const override
            {
                struct string final
                    : const_string::base
                {
                    void
                    destroy(std::size_t size) noexcept override
                    {
                        detail::over_allocator<
                            string, A> a(size, A{});
                        this->~string();
                        a.deallocate(this, 1);
                    }
                };
                detail::over_allocator<
                    string, A> a(size, A{});
                auto p = ::new(a.allocate(1)) string;
                return result{ p, reinterpret_cast<
                    char*>(p + 1), size };
            }
        };

        struct deleter
        {
            default_impl* p;

            ~deleter()
            {
                p->release();
            }
        };

        static default_impl d{};
        static deleter dp{&d};

        ++d.refs;

        return &d;
    }())
{
}

const_string
const_string::
factory::
operator()(string_view s) const
{
    return (*this)(s.size(),
        [&s](
            std::size_t,
            char* dest)
        {
            if (!s.empty())
                std::memcpy(
                    dest,
                    s.data(),
                    s.size());
        });
}

//------------------------------------------------

const_string::
const_string(
    result const& r) noexcept
    : string_view(r.data, r.size)
    , data_(r.p)
{
}

const_string::
~const_string()
{
    if (!is_small(size()))
        data_.p_->release(size());
}

const_string::
const_string() noexcept
    : string_view()
{
}

const_string::
const_string(
    const_string const& other) noexcept
    : string_view()
{
    if (is_small(other.size()))
    {
        // other is small,
        // nothing to release
        std::memcpy( data_.buf_,
            other.data_.buf_, other.size());
        static_cast<string_view&>(*this) =
            string_view(data_.buf_, other.size());
        return;
    }
    data_.p_ = other.data_.p_;
    ++data_.p_->refs;
    static_cast<string_view&>(
        *this) = string_view(other);
}

const_string&
const_string::
operator=(
    const_string const& other) noexcept
{
    if (is_small(other.size()))
    {
        if (!is_small(size()))
            data_.p_->release(size());
        else if(this == &other)
            return *this;
        std::memcpy(data_.buf_,
            other.data_.buf_, other.size());
        static_cast<string_view&>(*this) =
            string_view(data_.buf_, other.size());
        return *this;
    }
    ++other.data_.p_->refs;
    if (!is_small(size()))
        data_.p_->release(size());
    data_.p_ = other.data_.p_;
    static_cast<string_view&>(
        *this) = string_view(other);
    return *this;
}

} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_ERROR_IPP
#define BOOST_URL_IMPL_ERROR_IPP


namespace boost {
namespace urls {

error_code
make_error_code(error e)
{
    struct codes : error_category
    {
        const char*
        name() const noexcept override
        {
            return "boost.url";
        }

        std::string
        message(int ev) const override
        {
            switch(static_cast<error>(ev))
            {
            default:
case error::bad_alpha: return "bad alpha";
case error::bad_digit: return "bad digit";
case error::bad_empty_element: return "bad empty element";
case error::bad_hexdig: return "bad hexdig";
case error::bad_ipv6: return "bad ipv6";
case error::bad_leading_zero: return "bad leading zero";
case error::bad_octet: return "bad octet";
case error::bad_schemeless_path_segment: return "bad segment-nz-nc";
case error::empty: return "incomplete";
case error::empty_path_segment: return "empty path segment";
case error::illegal_null: return "illegal null";
case error::illegal_reserved_char: return "illegal reserved char";
case error::invalid: return "invalid";
case error::missing_char_literal: return "missing char literal";
case error::missing_path_segment: return "missing path segment";
case error::missing_path_separator: return "missing path sep";
case error::missing_words: return "missing words";
case error::non_canonical: return "non canonical";
case error::wrong_char_literal: return "wrong char literal";

case error::bad_pct_hexdig: return "bad hexdig in pct-encoding";
case error::incomplete_pct_encoding: return "incomplete pct-encoding";
case error::missing_pct_hexdig: return "missing hexdig in pct-encoding";
case error::no_space: return "no space";
case error::not_a_base: return "not a base";
            }
        }

        error_condition
        default_error_condition(
            int ev) const noexcept override
        {
            switch(static_cast<error>(ev))
            {
            default:
                return {ev, *this};

case error::bad_pct_hexdig:
case error::incomplete_pct_encoding:
case error::missing_pct_hexdig:
    return grammar::condition::fatal;
            }
        }
    };

    static codes const cat{};
    return error_code{static_cast<
        std::underlying_type<error>::type>(e), cat};
}

} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_IPV4_ADDRESS_IPP
#define BOOST_URL_IMPL_IPV4_ADDRESS_IPP

//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_DETAIL_DEC_OCTET_HPP
#define BOOST_URL_RFC_DETAIL_DEC_OCTET_HPP


namespace boost {
namespace urls {
namespace detail {

struct dec_octet
{
    unsigned char& v;

    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* const end,
        error_code& ec,
        dec_octet const& t) noexcept
    {
        if(it == end)
        {
            // expected DIGIT
            ec = grammar::error::incomplete;
            return;
        }
        if(! grammar::digit_chars(*it))
        {
            // not a digit
            ec = error::bad_digit;
            return;
        }
        unsigned v = *it - '0';
        ++it;
        if(it == end)
        {
            t.v = static_cast<
                std::uint8_t>(v);
            return;
        }
        if(! grammar::digit_chars(*it))
        {
            t.v = static_cast<
                std::uint8_t>(v);
            return;
        }
        if(v == 0)
        {
            // bad leading '0'
            ec = error::bad_leading_zero;
            return;
        }
        v = (10 * v) + *it - '0';
        ++it;
        if(it == end)
        {
            t.v = static_cast<
                std::uint8_t>(v);
            return;
        }
        if(! grammar::digit_chars(*it))
        {
            t.v = static_cast<
                std::uint8_t>(v);
            return;
        }
        if(v > 25)
        {
            // out of range
            ec = error::bad_octet;
            return;
        }
        v = (10 * v) + *it - '0';
        if(v > 255)
        {
            // out of range
            ec = error::bad_octet;
            return;
        }
        ++it;
        t.v = static_cast<
            std::uint8_t>(v);
    }
};

} // detail
} // urls
} // boost

#endif
#include <cstring>

namespace boost {
namespace urls {

ipv4_address::
ipv4_address(
    uint_type addr) noexcept
    : addr_(addr)
{
}

ipv4_address::
ipv4_address(
    bytes_type const& bytes) noexcept
{
    addr_ =
(static_cast<unsigned long>(bytes[0]) << 24) |
(static_cast<unsigned long>(bytes[1]) << 16) |
(static_cast<unsigned long>(bytes[2]) <<  8) |
(static_cast<unsigned long>(bytes[3]));
}

ipv4_address::
ipv4_address(
    string_view s)
{
    auto r = parse_ipv4_address(s);
    if(r.has_error())
        detail::throw_invalid_argument(
            BOOST_CURRENT_LOCATION);
    *this = r.value();
}

auto
ipv4_address::
to_bytes() const noexcept ->
    bytes_type
{
    bytes_type bytes;
    bytes[0] = (addr_ >> 24) & 0xff;
    bytes[1] = (addr_ >> 16) & 0xff;
    bytes[2] = (addr_ >>  8) & 0xff;
    bytes[3] =  addr_        & 0xff;
    return bytes;
}

auto
ipv4_address::
to_uint() const noexcept ->
    uint_type
{
    return addr_;
}

string_view
ipv4_address::
to_buffer(
    char* dest,
    std::size_t dest_size) const
{
    if(dest_size < max_str_len)
        detail::throw_length_error(
            "ipv4_address::to_buffer",
            BOOST_CURRENT_LOCATION);
    auto n = print_impl(dest);
    return string_view(dest, n);
}

bool
ipv4_address::
is_loopback() const noexcept
{
    return (to_uint() & 0xFF000000) ==
        0x7F000000;
}

bool
ipv4_address::
is_unspecified() const noexcept
{
    return to_uint() == 0;
}

bool
ipv4_address::
is_multicast() const noexcept
{
    return (to_uint() & 0xF0000000) ==
        0xE0000000;
}

void
ipv4_address::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    ipv4_address& t) noexcept
{
    std::array<unsigned char, 4> v;
    if(! grammar::parse(
        it, end, ec,
        detail::dec_octet{v[0]}, '.',
        detail::dec_octet{v[1]}, '.',
        detail::dec_octet{v[2]}, '.',
        detail::dec_octet{v[3]}))
        return;
    t = ipv4_address(v);
}

std::size_t
ipv4_address::
print_impl(
    char* dest) const noexcept
{
    auto const start = dest;
    auto const write =
        []( char*& dest,
            unsigned char v)
        {
            if(v >= 100)
            {
                *dest++ = '0' +
                    v / 100;
                v %= 100;
            }
            if(v >= 10)
            {
                *dest++ = '0' +
                    v / 10;
                v %= 10;
            }
            *dest++ = '0' + v;
        };
    auto const v = to_uint();
    write(dest, (v >> 24) & 0xff);
    *dest++ = '.';
    write(dest, (v >> 16) & 0xff);
    *dest++ = '.';
    write(dest, (v >>  8) & 0xff);
    *dest++ = '.';
    write(dest, (v      ) & 0xff);
    return dest - start;
}

std::ostream&
operator<<(
    std::ostream& os,
    ipv4_address const& addr)
{
    char buf[ipv4_address::max_str_len];
    os << addr.to_buffer(buf, sizeof(buf));
    return os;
}

result<ipv4_address>
parse_ipv4_address(
    string_view s) noexcept
{
    error_code ec;
    ipv4_address addr;
    if(! grammar::parse_string(
            s, ec, addr))
        return ec;
    return addr;
}

} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_IPV6_ADDRESS_IPP
#define BOOST_URL_IMPL_IPV6_ADDRESS_IPP

#include <cstring>

namespace boost {
namespace urls {

namespace detail {

struct h16
{
    unsigned char* p;

    // return `true` if the hex
    // word could be 0..255 if
    // interpreted as decimal
    static
    bool
    is_octet(unsigned char const* p) noexcept
    {
        unsigned short word =
            static_cast<unsigned short>(
                p[0]) * 256 +
            static_cast<unsigned short>(
                p[1]);
        if(word > 0x255)
            return false;
        if(((word >>  4) & 0xf) > 9)
            return false;
        if((word & 0xf) > 9)
            return false;
        return true;
    }

    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* const end,
        error_code& ec,
        h16 const& t) noexcept
    {
        BOOST_ASSERT(it != end);
        std::uint16_t v;
        for(;;)
        {
            char d;
            if(!grammar::hexdig_value(*it, d))
            {
                // not a HEXDIG
                ec = error::bad_hexdig;
                return;
            }
            v = d;
            ++it;
            if(it == end)
                break;
            if(!grammar::hexdig_value(*it, d))
                break;
            v = (16 * v) + d;
            ++it;
            if(it == end)
                break;
            if(!grammar::hexdig_value(*it, d))
                break;
            v = (16 * v) + d;
            ++it;
            if(it == end)
                break;
            if(!grammar::hexdig_value(*it, d))
                break;
            v = (16 * v) + d;
            ++it;
            break;
        }
        ec = {};
        t.p[0] = static_cast<
            unsigned char>(
                v / 256);
        t.p[1] = static_cast<
            unsigned char>(
                v % 256);
    }
};

} // detail

ipv6_address::
ipv6_address(
    bytes_type const& bytes) noexcept
{
    std::memcpy(&addr_,
        bytes.data(), 16);
}

ipv6_address::
ipv6_address(
    ipv4_address const& addr) noexcept
{
    auto const v = addr.to_bytes();
    ipv6_address::bytes_type bytes = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0xff, 0xff, v[0], v[1], v[2], v[3] } };
    std::memcpy(&addr_, bytes.data(), 16);
}

ipv6_address::
ipv6_address(
    string_view s)
{
    auto r = parse_ipv6_address(s);
    if(r.has_error())
        detail::throw_invalid_argument(
            BOOST_CURRENT_LOCATION);
    *this = r.value();
}

string_view
ipv6_address::
to_buffer(
    char* dest,
    std::size_t dest_size) const
{
    if(dest_size < max_str_len)
        detail::throw_length_error(
            "ipv6_address::to_buffer",
            BOOST_CURRENT_LOCATION);
    auto n = print_impl(dest);
    return string_view(dest, n);
}

bool
ipv6_address::
is_loopback() const noexcept
{
    return *this == loopback();
}

bool
ipv6_address::
is_unspecified() const noexcept
{
    return *this == ipv6_address();
}

bool
ipv6_address::
is_v4_mapped() const noexcept
{
    return
        addr_[ 0] == 0 && addr_[ 1] == 0 &&
        addr_[ 2] == 0 && addr_[ 3] == 0 &&
        addr_[ 4] == 0 && addr_[ 5] == 0 &&
        addr_[ 6] == 0 && addr_[ 7] == 0 &&
        addr_[ 8] == 0 && addr_[ 9] == 0 &&
        addr_[10] == 0xff &&
        addr_[11] == 0xff;
}

bool
operator==(
    ipv6_address const& a1,
    ipv6_address const& a2) noexcept
{
    return a1.addr_ == a2.addr_;
}

ipv6_address
ipv6_address::
loopback() noexcept
{
    ipv6_address a;
    a.addr_[15] = 1;
    return a;
}

std::size_t
ipv6_address::
print_impl(
    char* dest) const noexcept
{
    auto const count_zeroes =
    []( unsigned char const* first,
        unsigned char const* const last)
    {
        std::size_t n = 0;
        while(first != last)
        {
            if( first[0] != 0 ||
                first[1] != 0)
                break;
            n += 2;
            first += 2;
        }
        return n;
    };
    auto const print_hex =
    []( char* dest,
        unsigned short v)
    {
        char const* const dig =
            "0123456789abcdef";
        if(v >= 0x1000)
        {
            *dest++ = dig[v>>12];
            v &= 0x0fff;
            *dest++ = dig[v>>8];
            v &= 0x0ff;
            *dest++ = dig[v>>4];
            v &= 0x0f;
            *dest++ = dig[v];
        }
        else if(v >= 0x100)
        {
            *dest++ = dig[v>>8];
            v &= 0x0ff;
            *dest++ = dig[v>>4];
            v &= 0x0f;
            *dest++ = dig[v];
        }
        else if(v >= 0x10)
        {
            *dest++ = dig[v>>4];
            v &= 0x0f;
            *dest++ = dig[v];
        }
        else
        {
            *dest++ = dig[v];
        }
        return dest;
    };
    auto const dest0 = dest;
    // find longest run of zeroes
    std::size_t best_len = 0;
    int best_pos = -1;
    auto it = addr_.data();
    auto const v4 =
        is_v4_mapped();
    auto const end = v4 ?
        (it + addr_.size() - 4)
        : it + addr_.size();
    while(it != end)
    {
        auto n = count_zeroes(
            it, end);
        if(n == 0)
        {
            it += 2;
            continue;
        }
        if(n > best_len)
        {
            best_pos = static_cast<
                int>(it - addr_.data());
            best_len = n;
        }
        it += n;
    }
    it = addr_.data();
    if(best_pos != 0)
    {
        unsigned short v =
            (it[0] * 256U) + it[1];
        dest = print_hex(dest, v);
        it += 2;
    }
    else
    {
        *dest++ = ':';
        it += best_len;
        if(it == end)
            *dest++ = ':';
    }
    while(it != end)
    {
        *dest++ = ':';
        if(it - addr_.data() ==
            best_pos)
        {
            it += best_len;
            if(it == end)
                *dest++ = ':';
            continue;
        }
        unsigned short v =
            (it[0] * 256U) + it[1];
        dest = print_hex(dest, v);
        it += 2;
    }
    if(v4)
    {
        ipv4_address::bytes_type bytes;
        bytes[0] = it[0];
        bytes[1] = it[1];
        bytes[2] = it[2];
        bytes[3] = it[3];
        ipv4_address a(bytes);
        *dest++ = ':';
        dest += a.print_impl(dest);
    }
    return dest - dest0;
}

void
ipv6_address::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    ipv6_address& t) noexcept
{
    int n = 8;      // words needed
    int b = -1;     // value of n
                    // when '::' seen
    bool c = false; // need colon
    auto prev = it;
    ipv6_address::bytes_type bytes;
    for(;;)
    {
        if(it == end)
        {
            if(b != -1)
            {
                // end in "::"
                break;
            }
            BOOST_ASSERT(n > 0);
            // not enough words
            ec = error::missing_words;
            return;
        }
        if(*it == ':')
        {
            ++it;
            if(it == end)
            {
                // missing ':'
                ec = error::missing_char_literal;
                return;
            }
            if(*it == ':')
            {
                if(b == -1)
                {
                    // first "::"
                    ++it;
                    --n;
                    b = n;
                    if(n == 0)
                        break;
                    c = false;
                    continue;
                }
                // two "::"
                ec = error::bad_ipv6;
                return;
            }
            if(c)
            {
                prev = it;
                if(! grammar::parse(
                    it, end, ec, 
                    detail::h16{
                        &bytes[2*(8-n)]}))
                    return;
                --n;
                if(n == 0)
                    break;
                continue;
            }
            // expected h16
            ec = error::missing_words;
            return;
        }
        if(*it == '.')
        {
            if(b == -1 && n > 1)
            {
                // not enough h16
                ec = error::bad_ipv6;
                return;
            }
            if(! detail::h16::is_octet(
                &bytes[2*(7-n)]))
            {
                // invalid octet
                ec = error::bad_octet;
                return;
            }
            // rewind the h16 and
            // parse it as ipv4
            ipv4_address v4;
            it = prev;
            if(! grammar::parse(
                    it, end, ec, v4))
                return;
            auto const b4 =
                v4.to_bytes();
            bytes[2*(7-n)+0] = b4[0];
            bytes[2*(7-n)+1] = b4[1];
            bytes[2*(7-n)+2] = b4[2];
            bytes[2*(7-n)+3] = b4[3];
            --n;
            break;
        }
        char d;
        if( b != -1 &&
            !grammar::hexdig_value(*it, d))
        {
            // ends in "::"
            break;
        }
        if(! c)
        {
            prev = it;
            if(! grammar::parse(
                it, end, ec,
                detail::h16{
                    &bytes[2*(8-n)]}))
                return;
            --n;
            if(n == 0)
                break;
            c = true;
            continue;
        }
        // ':' divides a word
        ec = error::bad_ipv6;
        return;
    }
    ec = {};
    if(b == -1)
    {
        t = bytes;
        return;
    }
    if(b == n)
    {
        // "::" last
        auto const i =
            2 * (7 - n);
        std::memset(
            &bytes[i],
            0, 16 - i);
    }
    else if(b == 7)
    {
        // "::" first
        auto const i =
            2 * (b - n);
        std::memmove(
            &bytes[16 - i],
            &bytes[2],
            i);
        std::memset(
            &bytes[0],
            0, 16 - i);
    }
    else
    {
        // "::" in middle
        auto const i0 =
            2 * (7 - b);
        auto const i1 =
            2 * (b - n);
        std::memmove(
            &bytes[16 - i1],
            &bytes[i0 + 2],
            i1);
        std::memset(
            &bytes[i0],
            0, 16 - (i0 + i1));
    }
    t = bytes;
}

std::ostream&
operator<<(
    std::ostream& os,
    ipv6_address const& addr)
{
    char buf[ipv6_address::max_str_len];
    auto const s = addr.to_buffer(
        buf, sizeof(buf));
    os << s;
    return os;
}

result<ipv6_address>
parse_ipv6_address(
    string_view s) noexcept
{
    error_code ec;
    ipv6_address addr;
    if(! grammar::parse_string(
            s, ec, addr))
        return ec;
    return addr;
}

} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_PARAMS_IPP
#define BOOST_URL_IMPL_PARAMS_IPP

#include <boost/assert.hpp>

namespace boost {
namespace urls {

//------------------------------------------------
//
// Modifiers
//
//------------------------------------------------

auto
params::
remove_value(
    iterator pos) ->
        iterator
{
    BOOST_ASSERT(pos.impl_.begin_ ==
        u_->encoded_query().data());
    BOOST_ASSERT(pos.impl_.end_ ==
        u_->encoded_query().data() +
        u_->encoded_query().size());
    auto r = u_->param(pos.impl_.i_);
    query_param_view v{
        string_view{
            u_->s_ + r.pos + 1,
            r.nk - 1},
        string_view{},
        false};
    u_->edit_params(
        pos.impl_.i_,
        pos.impl_.i_ + 1,
        detail::make_enc_params_iter(
            &v, &v + 1),
        detail::make_enc_params_iter(
            &v, &v + 1));
    return std::next(begin(), pos.impl_.i_);
}

auto
params::
replace_value(
    iterator pos,
    string_view value) ->
        iterator
{
    auto const r =
        u_->param(pos.impl_.i_);
    string_view key{
        u_->s_ + r.pos + 1,
        r.nk - 1};
    query_param_view v{
        key, value, true };
    BOOST_ASSERT(pos.impl_.begin_ ==
        u_->encoded_query().data());
    BOOST_ASSERT(pos.impl_.end_ ==
        u_->encoded_query().data() +
        u_->encoded_query().size());
    using detail::
        make_plain_value_iter;
    u_->edit_params(
        pos.impl_.i_,
        pos.impl_.i_ + 1,
        make_plain_value_iter(
            &v, &v + 1),
        make_plain_value_iter(
            &v, &v + 1));
    return std::next(begin(), pos.impl_.i_);
}

auto
params::
erase(
    iterator first,
    iterator last) ->
        iterator
{
    BOOST_ASSERT(first.impl_.begin_ ==
        u_->encoded_query().data());
    BOOST_ASSERT(first.impl_.end_ ==
        u_->encoded_query().data() +
        u_->encoded_query().size());
    BOOST_ASSERT(last.impl_.begin_ ==
        u_->encoded_query().data());
    BOOST_ASSERT(last.impl_.end_ ==
        u_->encoded_query().data() +
        u_->encoded_query().size());
    string_view s;
    u_->edit_params(
        first.impl_.i_,
        last.impl_.i_,
        detail::enc_query_iter(s),
        detail::enc_query_iter(s));
    return std::next(begin(), first.impl_.i_);
}

std::size_t
params::
erase(string_view key) noexcept
{
    std::size_t n = 0;
    auto it = find(key);
    while(it != end())
    {
        ++n;
        it = erase(it);
        it = find(it, key);
    }
    return n;
}

//------------------------------------------------
//
// Lookup
//
//------------------------------------------------

std::size_t
params::
count(string_view key) const noexcept
{
    std::size_t n = 0;
    auto it = find(key);
    auto const end_ = end();
    while(it != end_)
    {
        ++n;
        ++it;
        it = find(it, key);
    }
    return n;
}

auto
params::
find(
    iterator from,
    string_view key) const noexcept ->
        iterator
{
    BOOST_ASSERT(from.impl_.begin_ ==
        u_->encoded_query().data());
    BOOST_ASSERT(from.impl_.end_ ==
        u_->encoded_query().data() +
        u_->encoded_query().size());
    auto const end_ = end();
    while(from != end_)
    {
        auto r = u_->param(
            from.impl_.i_);
        if( detail::key_equal_encoded(
            key, string_view(u_->s_ +
            r.pos + 1, r.nk - 1)))
            break;
        ++from;
    }
    return from;
}

} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_PARAMS_ENCODED_IPP
#define BOOST_URL_IMPL_PARAMS_ENCODED_IPP

#include <boost/assert.hpp>

namespace boost {
namespace urls {

auto
params_encoded::
operator[](
    std::size_t pos) const ->
        reference
{
    BOOST_ASSERT(pos < u_->nparam_);
    auto const r = u_->param(pos);
    if(r.nv > 0)
        return {
            string_view(
                u_->s_ + r.pos + 1,
                r.nk - 1),
            string_view(
                u_->s_ + r.pos + r.nk + 1,
                r.nv - 1),
            true};
    return {
        string_view(
            u_->s_ + r.pos + 1,
            r.nk - 1),
        string_view{},
        false};
}

//------------------------------------------------
//
// Element Access
//
//------------------------------------------------

auto
params_encoded::
at(string_view key) const ->
    string_view
{
    url::raw_param r;
    auto it = find(key);
    for(;;)
    {
        if(it == end())
            detail::throw_out_of_range(
                BOOST_CURRENT_LOCATION);
        r = u_->param(it.impl_.i_);
        if(r.nv != 0)
            break;
        ++it;
        it = find(it, key);
    }
    return {
        u_->s_ + r.pos +
            r.nk + 1,
        r.nv - 1 };
}

//------------------------------------------------
//
// Modifiers
//
//------------------------------------------------

auto
params_encoded::
remove_value(
    iterator pos) ->
        iterator
{
    BOOST_ASSERT(pos.impl_.begin_ ==
        u_->encoded_query().data());
    BOOST_ASSERT(pos.impl_.end_ ==
        u_->encoded_query().data() +
        u_->encoded_query().size());
    using detail::
        make_enc_params_iter;
    auto r = u_->param(pos.impl_.i_);
    reference v{
        string_view{
            u_->s_ + r.pos + 1,
            r.nk - 1},
        string_view{},
        false};
    u_->edit_params(
        pos.impl_.i_,
        pos.impl_.i_ + 1,
        make_enc_params_iter(
            &v, &v + 1),
        make_enc_params_iter(
            &v, &v + 1));
    return std::next(begin(), pos.impl_.i_);
}

auto
params_encoded::
replace_value(
    iterator pos,
    string_view value) ->
        iterator
{
    return emplace_at(
        pos,
        (*pos).key,
        value);
}

auto
params_encoded::
erase(
    iterator first,
    iterator last) ->
        iterator
{
    BOOST_ASSERT(first.impl_.begin_ ==
        u_->encoded_query().data());
    BOOST_ASSERT(first.impl_.end_ ==
        u_->encoded_query().data() +
        u_->encoded_query().size());
    BOOST_ASSERT(last.impl_.begin_ ==
        u_->encoded_query().data());
    BOOST_ASSERT(last.impl_.end_ ==
        u_->encoded_query().data() +
        u_->encoded_query().size());
    string_view s;
    u_->edit_params(
        first.impl_.i_,
        last.impl_.i_,
        detail::enc_query_iter(s),
        detail::enc_query_iter(s));
    return std::next(begin(), first.impl_.i_);
}

std::size_t
params_encoded::
erase(string_view key) noexcept
{
    std::size_t n = 0;
    auto it = find(key);
    while(it != end())
    {
        ++n;
        it = erase(it);
        it = find(it, key);
    }
    return n;
}

//------------------------------------------------
//
// Lookup
//
//------------------------------------------------

std::size_t
params_encoded::
count(string_view key) const noexcept
{
    std::size_t n = 0;
    auto it = find(key);
    auto const end_ = end();
    while(it != end_)
    {
        ++n;
        ++it;
        it = find(it, key);
    }
    return n;
}

auto
params_encoded::
find(
    iterator from,
    string_view key) const noexcept ->
        iterator
{
    BOOST_ASSERT(from.impl_.begin_ ==
        u_->encoded_query().data());
    BOOST_ASSERT(from.impl_.end_ ==
        u_->encoded_query().data() +
        u_->encoded_query().size());

    auto const end_ = end();
    while(from != end_)
    {
        auto r = u_->param(
            from.impl_.i_);
        if( detail::key_equal_encoded(
            key, string_view(u_->s_ +
            r.pos + 1, r.nk - 1)))
            break;
        ++from;
    }
    return std::next(begin(), from.impl_.i_);
}

//------------------------------------------------

} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_PARAMS_ENCODED_VIEW_IPP
#define BOOST_URL_IMPL_PARAMS_ENCODED_VIEW_IPP

#include <boost/assert.hpp>

namespace boost {
namespace urls {

//------------------------------------------------
//
// Element Access
//
//------------------------------------------------

auto
params_encoded_view::
at(string_view key) const ->
    string_view
{
    auto it = find(key);
    for(;;)
    {
        if(it == end())
            detail::throw_out_of_range(
                BOOST_CURRENT_LOCATION);
        if(it.impl_.nv_ != 0)
            break;
        ++it;
        it = find(it, key);
    }
    return {
        it.impl_.pos_ + it.impl_.nk_ + 1,
        it.impl_.nv_ - 1 };
}

//--------------------------------------------
//
// Iterators
//
//--------------------------------------------

auto
params_encoded_view::
begin() const noexcept ->
    iterator
{
    if(n_ > 0)
        return { s_ };
    return end();
}

auto
params_encoded_view::
end() const noexcept ->
    iterator
{
    return { s_, n_, 0 };
}

//------------------------------------------------
//
// Lookup
//
//------------------------------------------------

std::size_t
params_encoded_view::
count(string_view key) const noexcept
{
    std::size_t n = 0;
    auto it = find(key);
    auto const end_ = end();
    while(it != end_)
    {
        ++n;
        ++it;
        it = find(it, key);
    }
    return n;
}

auto
params_encoded_view::
find(
    iterator from,
    string_view key) const noexcept ->
        iterator
{
    BOOST_ASSERT(from.impl_.end_ ==
        s_.data() + s_.size());

    auto const end_ = end();
    while(from != end_)
    {
        if( detail::compare_encoded(
                key, from.encoded_key()) == 0)
            break;
        ++from;
    }
    return from;
}

//------------------------------------------------
//
// Parsing
//
//------------------------------------------------

result<params_encoded_view>
parse_query_params(
    string_view s) noexcept
{
    error_code ec;
    query_rule t;
    if(! grammar::parse_string(s, ec, t))
        return ec;
    return params_encoded_view(
        t.v.str(), t.v.size());
}

} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_PARAMS_VIEW_IPP
#define BOOST_URL_IMPL_PARAMS_VIEW_IPP

#include <boost/assert.hpp>

namespace boost {
namespace urls {

//------------------------------------------------
//
// Element Access
//
//------------------------------------------------

auto
params_view::
at(string_view key) const ->
    const_string
{
    auto it = find(key);
    for(;;)
    {
        if(it == end())
            detail::throw_out_of_range(
                BOOST_CURRENT_LOCATION);
        if(it.impl_.nv_ != 0)
            break;
        ++it;
        it = find(it, key);
    }
    string_view ev{
        it.impl_.pos_ + it.impl_.nk_ + 1,
        it.impl_.nv_ - 1 };
    auto n =
        pct_decode_bytes_unchecked(ev);
    return a_(n, [ev]
        (std::size_t n, char* dest)
        {
            pct_decode_unchecked(
                dest, dest + n, ev);
        });
}

//------------------------------------------------
//
// Iterators
//
//------------------------------------------------

auto
params_view::
begin() const noexcept ->
    iterator
{
    if(n_ > 0)
        return { s_, a_ };
    return end();
}

auto
params_view::
end() const noexcept ->
    iterator
{
    return { s_, n_, 0, a_ };
}

//------------------------------------------------
//
// Lookup
//
//------------------------------------------------

std::size_t
params_view::
count(string_view key) const noexcept
{
    std::size_t n = 0;
    auto it = find(key);
    auto const end_ = end();
    while(it != end_)
    {
        ++n;
        ++it;
        it = find(it, key);
    }
    return n;
}

auto
params_view::
find(
    iterator from,
    string_view key) const noexcept ->
        iterator
{
    BOOST_ASSERT(from.impl_.end_ ==
        s_.data() + s_.size());

    auto const end_ = end();
    while(from != end_)
    {
        if( detail::key_equal_encoded(
            key,
            from.encoded_key()))
            break;
        ++from;
    }
    return from;
}

} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_PCT_ENCODING_IPP
#define BOOST_URL_IMPL_PCT_ENCODING_IPP

#include <memory>

namespace boost {
namespace urls {

std::size_t
pct_decode_bytes_unchecked(
    string_view s) noexcept
{
    auto it = s.data();
    auto const end =
        it + s.size();
    std::size_t n = 0;
    while(it < end)
    {
        if(*it != '%')
        {
            // unescaped
            ++it;
            ++n;
            continue;
        }
        if(end - it < 3)
            return n;
        it += 3;
        ++n;
    }
    return n;
}

std::size_t
pct_decode_unchecked(
    char* const dest0,
    char const* end,
    string_view s,
    pct_decode_opts const& opt) noexcept
{
    auto const decode_hex = [](
        char const* it)
    {
        char d0;
        grammar::hexdig_value(it[0], d0);
        char d1;
        grammar::hexdig_value(it[1], d1);
        return static_cast<char>(
            ((static_cast<
                unsigned char>(d0) << 4) +
            (static_cast<
                unsigned char>(d1))));
    };
    auto it = s.data();
    auto const last = it + s.size();
    auto dest = dest0;

    if(opt.plus_to_space)
    {
        while(it != last)
        {
            if(dest == end)
            {
                // dest too small
                return dest - dest0;
            }
            if(*it == '+')
            {
                // plus to space
                *dest++ = ' ';
                ++it;
                continue;
            }
            if(*it == '%')
            {
                // escaped
                ++it;
                if(last - it < 2)
                {
                    // missing input,
                    // initialize output
                    std::memset(dest,
                        0, end - dest);
                    return dest - dest0;
                }
                *dest++ = decode_hex(it);
                it += 2;
                continue;
            }
            // unescaped
            *dest++ = *it++;
        }
        return dest - dest0;
    }

    while(it != last)
    {
        if(dest == end)
        {
            // dest too small
            return dest - dest0;
        }
        if(*it == '%')
        {
            // escaped
            ++it;
            if(last - it < 2)
            {
                // missing input,
                // initialize output
                std::memset(dest,
                    0, end - dest);
                return dest - dest0;
            }
            *dest++ = decode_hex(it);
            it += 2;
            continue;
        }
        // unescaped
        *dest++ = *it++;
    }
    return dest - dest0;
}

} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_QUERY_PARAM_IPP
#define BOOST_URL_IMPL_QUERY_PARAM_IPP


namespace boost {
namespace urls {

query_param::
query_param(
    char const* s,
    std::size_t nk,
    std::size_t const nv,
    const_string::factory const& a)
{
    if(nk + nv == 0)
    {
        has_value = false;
        return;
    }
    // key
    string_view ek{s, nk};
    auto n =
        pct_decode_bytes_unchecked(ek);
    key = a(n, [nk, ek]
        (std::size_t, char* dest)
        {
            pct_decode_unchecked(
                dest, dest + nk, ek);
        });
    if(nv > 0)
    {
        // value
        BOOST_ASSERT(s[nk] == '=');
        has_value = true;
        string_view ev{
            s + nk + 1, nv - 1 };
        n = pct_decode_bytes_unchecked(ev);
        value = a(n, [ev]
            (std::size_t n, char* dest)
            {
                pct_decode_unchecked(
                    dest, dest + n, ev);
            });
    }
    else
    {
        has_value = false;
    }
}

} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_SCHEME_IPP
#define BOOST_URL_IMPL_SCHEME_IPP


namespace boost {
namespace urls {

scheme
string_to_scheme(
    string_view s) noexcept
{
    using grammar::ascii_tolower;
    switch(s.size())
    {
    case 0: // none
        return scheme::none;

    case 2: // ws
        if( ascii_tolower(s[0]) == 'w' &&
            ascii_tolower(s[1]) == 's')
            return scheme::ws;
        break;

    case 3:
        switch(tolower(s[0]))
        {
        case 'w': // wss
            if( ascii_tolower(s[1]) == 's' &&
                ascii_tolower(s[2]) == 's')
                return scheme::wss;
            break;

        case 'f': // ftp
            if( ascii_tolower(s[1]) == 't' &&
                ascii_tolower(s[2]) == 'p')
                return scheme::ftp;
            break;

        default:
            break;
        }
        break;

    case 4:
        switch(tolower(s[0]))
        {
        case 'f': // file
            if( ascii_tolower(s[1]) == 'i' &&
                ascii_tolower(s[2]) == 'l' &&
                ascii_tolower(s[3]) == 'e')
                return scheme::file;
            break;

        case 'h': // http
            if( ascii_tolower(s[1]) == 't' &&
                ascii_tolower(s[2]) == 't' &&
                ascii_tolower(s[3]) == 'p')
                return scheme::http;
            break;

        default:
            break;
        }
        break;

    case 5: // https
        if( ascii_tolower(s[0]) == 'h' &&
            ascii_tolower(s[1]) == 't' &&
            ascii_tolower(s[2]) == 't' &&
            ascii_tolower(s[3]) == 'p' &&
            ascii_tolower(s[4]) == 's')
            return scheme::https;
        break;

    default:
        break;
    }
    return scheme::unknown;
}

string_view
to_string(scheme s) noexcept
{
    switch(s)
    {
    case scheme::ftp:   return "ftp";
    case scheme::file:  return "file";
    case scheme::http:  return "http";
    case scheme::https: return "https";
    case scheme::ws:    return "ws";
    case scheme::wss:   return "wss";
    case scheme::none:  return {};
    default:
        break;
    }
    return "<unknown>";
}

} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_SEGMENTS_IPP
#define BOOST_URL_IMPL_SEGMENTS_IPP

#include <boost/assert.hpp>

namespace boost {
namespace urls {

//------------------------------------------------
//
// Modifiers
//
//------------------------------------------------

auto
segments::
insert(
    iterator before,
    string_view s) ->
        iterator
{
    BOOST_ASSERT(before.impl_.pos_ >= u_->string().data());
    BOOST_ASSERT(before.impl_.pos_ <= u_->string().data() +
        u_->string().size());
    detail::copied_strings cs(
        u_->string());
    s = cs.maybe_copy(s);
    u_->edit_segments(
        before.impl_.i_,
        before.impl_.i_,
        detail::make_plain_segs_iter(
            &s, &s + 1),
        detail::make_plain_segs_iter(
            &s, &s + 1));
    return std::next(begin(), before.impl_.i_);
}

auto
segments::
segments::
erase(
    iterator first,
    iterator last) noexcept ->
        iterator
{
    BOOST_ASSERT(first.impl_.pos_ >= u_->string().data());
    BOOST_ASSERT(last.impl_.pos_ >= u_->string().data());
    BOOST_ASSERT(first.impl_.pos_ <= u_->string().data() +
        u_->string().size());
    BOOST_ASSERT(last.impl_.pos_ <= u_->string().data() +
        u_->string().size());
    string_view s;
    u_->edit_segments(
        first.impl_.i_, last.impl_.i_,
        detail::make_enc_segs_iter(&s, &s),
        detail::make_enc_segs_iter(&s, &s));
    return std::next(begin(), first.impl_.i_);
}

} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_SEGMENTS_ENCODED_IPP
#define BOOST_URL_IMPL_SEGMENTS_ENCODED_IPP


namespace boost {
namespace urls {

//------------------------------------------------
//
// Iterators
//
//------------------------------------------------

auto
segments_encoded::
begin() const noexcept ->
    iterator
{
    return {u_->encoded_path(), u_->nseg_};
}

auto
segments_encoded::
end() const noexcept ->
    iterator
{
    return {u_->encoded_path(), u_->nseg_, 0};
}

//------------------------------------------------
//
// Modifiers
//
//------------------------------------------------

auto
segments_encoded::
insert(
    iterator before,
    string_view s0) ->
        iterator
{
    BOOST_ASSERT(before.impl_.pos_ >= u_->string().data());
    BOOST_ASSERT(before.impl_.pos_ <= u_->string().data() +
        u_->string().size());
    detail::copied_strings cs(
        u_->string());
    auto s = cs.maybe_copy(s0);
    u_->edit_segments(
        before.impl_.i_,
        before.impl_.i_,
        detail::make_enc_segs_iter(
            &s, &s + 1),
        detail::make_enc_segs_iter(
            &s, &s + 1));
    return std::next(begin(), before.impl_.i_);
}

auto
segments_encoded::
erase(
    iterator first,
    iterator last) noexcept ->
        iterator
{
    BOOST_ASSERT(first.impl_.pos_ >= u_->string().data());
    BOOST_ASSERT(last.impl_.pos_ >= u_->string().data());
    BOOST_ASSERT(first.impl_.pos_ <= u_->string().data() +
        u_->string().size());
    BOOST_ASSERT(last.impl_.pos_ <= u_->string().data() +
        u_->string().size());
    string_view s;
    u_->edit_segments(
        first.impl_.i_, last.impl_.i_,
        detail::make_enc_segs_iter(&s, &s),
        detail::make_enc_segs_iter(&s, &s));
    return std::next(begin(), first.impl_.i_);
}

} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_SEGMENTS_ENCODED_VIEW_IPP
#define BOOST_URL_IMPL_SEGMENTS_ENCODED_VIEW_IPP

#include <ostream>

namespace boost {
namespace urls {

//------------------------------------------------

segments_encoded_view::
segments_encoded_view(
    string_view s,
    std::size_t nseg) noexcept
    : s_(s)
    , n_(nseg)
{
}

//------------------------------------------------
//
// Iterators
//
//------------------------------------------------

auto
segments_encoded_view::
begin() const noexcept ->
    iterator
{
    return iterator(s_, n_);
}

auto
segments_encoded_view::
end() const noexcept ->
    iterator
{
    return iterator(s_, n_, 0);
}

//------------------------------------------------
//
// Friends
//
//------------------------------------------------

std::ostream&
operator<<(
    std::ostream& os,
    segments_encoded_view const& pv)
{
    os << pv.s_;
    return os;
}

//------------------------------------------------

result<segments_encoded_view>
parse_path(string_view s) noexcept
{
    if(s.empty())
        return segments_encoded_view();
    if(s[0] == '/')
        return parse_path_abempty(s);
    return parse_path_rootless(s);
}

result<segments_encoded_view>
parse_path_abempty(
    string_view s) noexcept
{
    error_code ec;
    path_abempty_rule t;
    if(! grammar::parse_string(s, ec, t))
        return ec;
    return segments_encoded_view(
        t.str, detail::path_segments(
            t.str, t.count));
}

result<segments_encoded_view>
parse_path_absolute(
    string_view s) noexcept
{
    error_code ec;
    path_absolute_rule t;
    if(! grammar::parse_string(s, ec, t))
        return ec;
    return segments_encoded_view(
        t.str, detail::path_segments(
            t.str, t.count));
}

result<segments_encoded_view>
parse_path_noscheme(
    string_view s) noexcept
{
    error_code ec;
    path_noscheme_rule t;
    if(! grammar::parse_string(s, ec, t))
        return ec;
    return segments_encoded_view(
        t.str, detail::path_segments(
            t.str, t.count));
}

result<segments_encoded_view>
parse_path_rootless(
    string_view s) noexcept
{
    error_code ec;
    path_rootless_rule t;
    if(! grammar::parse_string(s, ec, t))
        return ec;
    return segments_encoded_view(
        t.str, detail::path_segments(
            t.str, t.count));
}

} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_SEGMENTS_VIEW_IPP
#define BOOST_URL_IMPL_SEGMENTS_VIEW_IPP

#include <boost/assert.hpp>
#include <ostream>

namespace boost {
namespace urls {

//------------------------------------------------
//
// Iterators
//
//------------------------------------------------

auto
segments_view::
begin() const noexcept ->
    iterator
{
    return iterator(s_, n_, a_);
}

auto
segments_view::
end() const noexcept ->
    iterator
{
    return iterator(s_, n_, a_, 0);
}

//------------------------------------------------
//
// Friends
//
//------------------------------------------------

std::ostream&
operator<<(
    std::ostream& os,
    segments_view const& vw)
{
    auto it = vw.begin();
    auto const end = vw.end();
    if( it != end )
    {
        if( vw.is_absolute() )
            os << "/";
        os << *it;
        while( ++it != end )
            os << '/' << *it;
    }
    return os;
}

} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_STATIC_POOL_IPP
#define BOOST_URL_IMPL_STATIC_POOL_IPP


namespace boost {
namespace urls {

void**
basic_static_pool::
table() noexcept
{
    return reinterpret_cast<
        void**>(begin_);
}

void**
basic_static_pool::
find(void* p) noexcept
{
    auto t = table();
    auto const end =
        t + n_;
    (void)end;
    BOOST_ASSERT(t != end);
    while(*t != p)
    {
        ++t;
        BOOST_ASSERT(t != end);
    }
    return t;
}

basic_static_pool::
~basic_static_pool()
{
    BOOST_ASSERT(n_ == 0);
    BOOST_ASSERT(top_ == end_);
}

void*
basic_static_pool::
allocate(
    std::size_t bytes,
    std::size_t align)
{
    if( align < 2)
        align = 2;
    static constexpr auto S =
        sizeof(void*);
    auto p = reinterpret_cast<char*>(
        reinterpret_cast<std::uintptr_t>(
            top_ - bytes) & ~(align - 1));
    auto low = begin_ + S * (n_ + 1);
    if(p < low)
        detail::throw_bad_alloc(
            BOOST_CURRENT_LOCATION);
    top_ = p;
    table()[n_] = p;
    ++n_;
    return p;
}

void
basic_static_pool::
deallocate(
    void* p,
    std::size_t,
    std::size_t) noexcept
{
    BOOST_ASSERT(n_ > 0);
    if(p != top_)
    {
        auto t = find(p);
        *t = reinterpret_cast<void*>(
            reinterpret_cast<
                std::uintptr_t>(*t) | 1);
        return;
    }
    --n_;
    while(n_ > 0)
    {
        auto i = reinterpret_cast<
            std::uintptr_t>(table()[n_ - 1]);
        if((i & 1) == 0)
        {
            // not free
            top_ = reinterpret_cast<
                char*>(i);
            return;
        }
        --n_;
    }
    top_ = end_;
}

} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_STATIC_URL_IPP
#define BOOST_URL_IMPL_STATIC_URL_IPP

#include <boost/assert.hpp>

namespace boost {
namespace urls {

static_url_base::
~static_url_base()
{
    // prevent url from
    // deallocating memory
    s_ = nullptr;
}

static_url_base::
static_url_base(
    char* buf,
    std::size_t cap) noexcept
    : url(buf, cap)
{
}

void
static_url_base::
copy(url const& u)
{
    this->url::copy(u);
}

void
static_url_base::
construct(string_view s)
{
    auto r = parse_uri_reference(s);
    if(r.has_error())
        detail::throw_invalid_argument(
            BOOST_CURRENT_LOCATION);
    this->url::operator=(r.value());
}

void
static_url_base::
copy(url_view const& u)
{
    this->url::copy(u);
}

url_view
static_url_base::
convert() const noexcept
{
    return url_view(*this);
}

char*
static_url_base::
allocate(std::size_t n)
{
    (void)n;
    // should never get here
    BOOST_ASSERT(
        n > capacity_in_bytes());
    detail::throw_invalid_argument(
        "alloc_impl",
        BOOST_CURRENT_LOCATION);
}

void
static_url_base::
deallocate(char*)
{
    // should never get here
    detail::throw_invalid_argument(
        "free_impl",
        BOOST_CURRENT_LOCATION);
}

} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_URL_IPP
#define BOOST_URL_IMPL_URL_IPP

 //
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_PRINT_HPP
#define BOOST_URL_DETAIL_PRINT_HPP

#ifndef BOOST_URL_SOURCE
#error
#endif

#include <cstdint>
#include <type_traits>

namespace boost {
namespace urls {
namespace detail {

// std::uint64_t
// 18446744073709551615
//          1          2
template<class T>
struct printed
    : std::false_type
{
};

// 16-bit unsigned
template<>
class printed<std::uint16_t>
    : std::false_type
{
    char n_;
    char buf_[5];

public:
    printed(std::uint16_t n)
    {
        char* it =
            buf_ + sizeof(buf_);
        if(n == 0)
        {
            *--it = '0';
            n_ = 1;
        }
        else
        {
            while(n > 0)
            {
                *--it = '0' + (n % 10);
                n /= 10;
            }
            n_ = static_cast<char>(
                sizeof(buf_) - (
                    it - buf_));
        }
    }

    string_view
    string() const noexcept
    {
        return string_view(buf_ +
            sizeof(buf_) - n_, n_);
    }
};

template<class T>
printed<T>
make_printed(T t)
{
    return printed<T>(t);
}

} // detail
} // urls
} // boost

#endif
#include <boost/align/align_up.hpp>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace boost {
namespace urls {

// construct from static storage
url::
url(char* buf,
    std::size_t cap) noexcept
    : s_(buf)
    , cap_(cap)
{
    using alignment::align_up;
    BOOST_ASSERT(cap > 0);
    BOOST_ASSERT(align_up(cap_,
        alignof(pos_t)) == cap_);
    s_[0] = '\0';
    cs_ = s_;
}

// make a copy of u
void
url::
copy(url_view const& u)
{
    if(u.size() == 0)
    {
        clear();
        return;
    }
    ensure_space(
        u.size(),
        u.nseg_,
        u.nparam_);
    base() = u;
    cs_ = s_;
    std::memcpy(s_,
        u.data(), u.size());
    s_[size()] = '\0';
}

// allocate n aligned up
char*
url::
allocate(std::size_t n)
{
    using alignment::align_up;
    n = align_up(
        n, alignof(pos_t));
    auto s = new char[n];
    cap_ = n;
    return s;
}

void
url::
deallocate(char* s)
{
    delete[] s;
}

//------------------------------------------------

url::
~url()
{
    if(s_)
    {
        BOOST_ASSERT(
            cap_ != 0);
        deallocate(s_);
    }
}

url::
url() noexcept = default;

url::
url(url&& u) noexcept
{
    base() = u;
    cap_ = u.cap_;
    s_ = u.s_;
    u.s_ = nullptr;
    u.cap_ = 0;
    u.base() = url_view();
}

url::
url(url const& u)
    : url(u.base())
{
}

url::
url(url_view const& u)
{
    copy(u);
}

url&
url::
operator=(url&& u) noexcept
{
    if(s_)
        deallocate(s_);
    base() = u;
    cap_ = u.cap_;
    s_ = u.s_;
    u.s_ = nullptr;
    u.cap_ = 0;
    u.base() = url_view();
    return *this;
}

url&
url::
operator=(url_view const& u)
{
    copy(u);
    return *this;
}

url::
url(string_view s)
{
    auto r = parse_uri_reference(s);
    if(r.has_error())
        detail::throw_invalid_argument(
            BOOST_CURRENT_LOCATION);
    *this = r.value();
}

//------------------------------------------------

void
url::
reserve_bytes(
    std::size_t bytes)
{
    if(bytes <= capacity_in_bytes())
        return;
    std::size_t n = 0;
    if(nseg_ > 1)
        n += sizeof(pos_t) *
            (nseg_ - 1);
    if(nparam_ > 0)
        n += 2 * sizeof(pos_t) *
            nparam_;
    BOOST_ASSERT(n <= bytes);
    bytes -= n;
    if(bytes > 0)
        ensure_space(bytes,
            nseg_, nparam_);
}

void
url::
clear() noexcept
{
    if(s_)
    {
        base() = {};
        s_[0] = '\0';
        cs_ = s_;
    }
    else
    {
        BOOST_ASSERT(
            cs_ == empty_);
    }
}

//------------------------------------------------
//
// Scheme
//
//------------------------------------------------

void
url::
set_scheme_impl(
    string_view s,
    urls::scheme id)
{
    check_invariants();
    scheme_rule b;
    error_code ec;
    grammar::parse_string(s, ec, b);
    if(ec.failed())
        detail::throw_invalid_argument(
            BOOST_CURRENT_LOCATION);

    auto const n = s.size();
    auto const p = offset(id_path);

    // check for "./" prefix
    bool const has_dot =
        [this, p]
    {
        if(nseg_ == 0)
            return false;
        if(segment(1) <
            offset(id_path) + 2)
            return false;
        auto const src = s_ + p;
        if(src[0] != '.')
            return false;
        if(src[1] != '/')
            return false;
        return true;
    }();

    // Remove "./"
    if(has_dot)
    {
        // do this first, for
        // strong exception safety
        ensure_space(
            size() + n + 1 - 2,
            nseg_,
            nparam_);
        std::memmove(
            s_ + p,
            s_ + p + 2,
            size() + 1 -
                (p + 2));
        set_size(
            id_path,
            len(id_path) - 2);
        s_[size()] = '\0';
    }

    auto dest = resize_impl(
        id_scheme, n + 1);
    s.copy(dest, n);
    dest[n] = ':';
    scheme_ = id;
    check_invariants();
}

url&
url::
remove_scheme() noexcept
{
    check_invariants();
    auto const n = len(id_scheme);
    if(n == 0)
        return *this;

    auto const p = offset(id_path);

    // Check if we are changing
    // path-rootless to path-noscheme
    bool const need_dot =
        [this, p]
    {
        if(has_authority())
            return false;
        if(nseg_ == 0)
            return false;
        BOOST_ASSERT(len(id_path) > 0);
        if(s_[p] == '/')
            return false;
        string_view const s(
            s_ + p, segment(1) - p);
        if(s.find_first_of(':') ==
            string_view::npos)
            return false;
        return true;
    }();

    if(! need_dot)
    {
        // just remove the scheme
        resize_impl(id_scheme, 0);
        scheme_ = urls::scheme::none;
        check_invariants();
        return *this;
    }

    // remove the scheme but add "./"
    // to the beginning of the path
    BOOST_ASSERT(n >= 2);
    // move [id_scheme, id_path) left
    std::memmove(
        s_,
        s_ + n,
        p - n);
    // move [id_path, id_end) left
    std::memmove(
        s_ + p - (n - 2),
        s_ + p,
        offset(id_end) - p);
    // VFALCO adjust table
    // adjust part offsets.
    // (p is invalidated)
    adjust(id_user, id_path, 0-n);
    adjust(id_query, id_end, 0-(n - 2));
    auto dest = s_ + offset(id_path);
    dest[0] = '.';
    dest[1] = '/';
    s_[size()] = '\0';
    scheme_ = urls::scheme::none;
    check_invariants();
    return *this;
}

url&
url::
set_scheme(string_view s)
{
    detail::copied_strings cs(
        this->string());
    s = cs.maybe_copy(s);
    set_scheme_impl(
        s, string_to_scheme(s));
    return *this;
}

url&
url::
set_scheme(urls::scheme id)
{
    if(id == urls::scheme::unknown)
        detail::throw_invalid_argument(
            BOOST_CURRENT_LOCATION);
    if(id == urls::scheme::none)
        return remove_scheme();
    set_scheme_impl(
        to_string(id), id);
    return *this;
}

//------------------------------------------------
//
// authority
//
//------------------------------------------------

char*
url::
set_user_impl(std::size_t n)
{
    check_invariants();
    if(len(id_pass) != 0)
    {
        // keep "//"
        auto dest = resize_impl(
            id_user, 2 + n);
        check_invariants();
        return dest + 2;
    }
    // add authority
    auto dest = resize_impl(
        id_user, 2 + n + 1);
    split(id_user, 2 + n);
    dest[0] = '/';
    dest[1] = '/';
    dest[2 + n] = '@';
    check_invariants();
    return dest + 2;
}

url&
url::
set_user(string_view s)
{
    detail::copied_strings buf(
        this->string());
    s = buf.maybe_copy(s);
    check_invariants();
    auto const n = pct_encode_bytes(
        s, {}, detail::user_chars);
    auto dest = set_user_impl(n);
    pct_encode(dest, get(id_pass).data(),
        s, {}, detail::user_chars);
    decoded_[id_user] = s.size();
    check_invariants();
    return *this;
}

url&
url::
set_encoded_user(
    string_view s)
{
    detail::copied_strings buf(
        this->string());
    s = buf.maybe_copy(s);
    check_invariants();
    error_code ec;
    auto const n =
        validate_pct_encoding(s, ec, {}, detail::user_chars);
    if(ec.failed())
        detail::throw_invalid_argument(
            BOOST_CURRENT_LOCATION);
    auto dest = set_user_impl(s.size());
    decoded_[id_user] = n;
    if(! s.empty())
    {
        BOOST_ASSERT(dest != nullptr);
        std::memcpy(dest,
            s.data(), s.size());
    }
    check_invariants();
    return *this;
}

//------------------------------------------------

char*
url::
set_password_impl(
    std::size_t n)
{
    check_invariants();
    if(len(id_user) != 0)
    {
        // already have authority
        auto const dest = resize_impl(
            id_pass, 1 + n + 1);
        dest[0] = ':';
        dest[n + 1] = '@';
        check_invariants();
        return dest + 1;
    }
    // add authority
    auto const dest =
        resize_impl(
        id_user, id_host,
        2 + 1 + n + 1);
    split(id_user, 2);
    dest[0] = '/';
    dest[1] = '/';
    dest[2] = ':';
    dest[2 + n + 1] = '@';
    check_invariants();
    return dest + 3;
}

url&
url::
remove_password() noexcept
{
    check_invariants();
    auto const n = len(id_pass);
    if(n < 2)
        return *this;
    // clear password, retain '@'
    auto dest =
        resize_impl(id_pass, 1);
    dest[0] = '@';
    decoded_[id_pass] = 0;
    check_invariants();
    return *this;
}

url&
url::
set_password(string_view s)
{
    detail::copied_strings buf(
        this->string());
    s = buf.maybe_copy(s);
    check_invariants();
    auto const n = pct_encode_bytes(
        s, {}, detail::password_chars);
    auto dest = set_password_impl(n);
    pct_encode(
        dest,
        get(id_host).data() - 1,
        s,
        {},
        detail::password_chars);
    decoded_[id_pass] = s.size();
    check_invariants();
    return *this;
}

url&
url::
set_encoded_password(
    string_view s)
{
    detail::copied_strings buf(
        this->string());
    s = buf.maybe_copy(s);
    check_invariants();
    error_code ec;
    auto const n =
        validate_pct_encoding(s, ec, {}, detail::password_chars);
    if(ec.failed())
        detail::throw_invalid_argument(
            BOOST_CURRENT_LOCATION);
    auto dest =
        set_password_impl(s.size());
    decoded_[id_pass] = n;
    if(! s.empty())
    {
        BOOST_ASSERT(dest != nullptr);
        std::memcpy(dest,
            s.data(), s.size());
    }
    check_invariants();
    return *this;
}

//------------------------------------------------

char*
url::
set_userinfo_impl(
    std::size_t n)
{
    // "//" {dest} "@"
    check_invariants();
    auto dest = resize_impl(
        id_user, id_host, n + 3);
    split(id_user, n + 2);
    dest[0] = '/';
    dest[1] = '/';
    dest[n + 2] = '@';
    check_invariants();
    return dest + 2;
}

url&
url::
remove_userinfo() noexcept
{
    check_invariants();
    if(len(id_pass) == 0)
    {
        // no userinfo
        return *this;
    }
    // keep authority '//'
    resize_impl(
        id_user, id_host, 2);
    decoded_[id_user] = 0;
    decoded_[id_pass] = 0;
    check_invariants();
    return *this;
}

url&
url::
set_userinfo(
    string_view s)
{
    detail::copied_strings buf(
        this->string());
    s = buf.maybe_copy(s);
    check_invariants();
    auto const n = pct_encode_bytes(
        s, {}, detail::userinfo_chars);
    auto dest = set_userinfo_impl(n);
    pct_encode(
        dest,
        get(id_host).data() - 1,
        s,
        {},
        detail::userinfo_chars);
    decoded_[id_user] = s.size();
    check_invariants();
    return *this;
}

url&
url::
set_encoded_userinfo(
    string_view s)
{
    detail::copied_strings buf(
        this->string());
    s = buf.maybe_copy(s);
    check_invariants();
    error_code ec;
    userinfo_rule t;
    if(! grammar::parse_string(s, ec, t))
        detail::throw_invalid_argument(
            BOOST_CURRENT_LOCATION);
    auto dest = set_userinfo_impl(s.size());
    split(id_user, 2 + t.user.str.size());
    if(! s.empty())
        std::memcpy(dest, s.data(), s.size());
    decoded_[id_user] =
        t.user.decoded_size;
    if(t.has_password)
        decoded_[id_pass] =
            t.password.decoded_size;
    else
        decoded_[id_pass] = 0;
    check_invariants();
    return *this;
}

//------------------------------------------------

char*
url::
set_host_impl(std::size_t n)
{
    check_invariants();
    if(len(id_user) == 0)
    {
        // add authority
        auto dest = resize_impl(
            id_user, n + 2);
        split(id_user, 2);
        split(id_pass, 0);
        dest[0] = '/';
        dest[1] = '/';
        check_invariants();
        return dest + 2;
    }
    // already have authority
    auto const dest =
        resize_impl(id_host, n);
    check_invariants();
    return dest;
}

url&
url::
set_host(
    urls::ipv4_address const& addr)
{
    check_invariants();
    char buf[urls::
        ipv4_address::max_str_len];
    auto s = addr.to_buffer(
        buf, sizeof(buf));
    auto dest =
        set_host_impl(s.size());
    std::memcpy(
        dest, s.data(), s.size());
    decoded_[id_host] = len(id_host);
    host_type_ =
        urls::host_type::ipv4;
    auto bytes = addr.to_bytes();
    std::memcpy(ip_addr_,
        bytes.data(), bytes.size());
    check_invariants();
    return *this;
}

url&
url::
set_host(
    urls::ipv6_address const& addr)
{
    check_invariants();
    char buf[2 + urls::
        ipv6_address::max_str_len];
    auto s = addr.to_buffer(
        buf + 1, sizeof(buf) - 2);
    buf[0] = '[';
    buf[s.size() + 1] = ']';
    auto dest =
        set_host_impl(s.size() + 2);
    std::memcpy(
        dest, buf, s.size() + 2);
    decoded_[id_host] = len(id_host);
    host_type_ =
        urls::host_type::ipv6;
    auto bytes = addr.to_bytes();
    std::memcpy(ip_addr_,
        bytes.data(), bytes.size());
    check_invariants();
    return *this;
}

url&
url::
set_host(
    string_view s)
{
    detail::copied_strings buf(
        this->string());
    s = buf.maybe_copy(s);
    // try ipv4
    {
        auto r = parse_ipv4_address(s);
        if(! r.has_error())
            return set_host(r.value());
    }
    check_invariants();
    auto const n = pct_encode_bytes(
        s, {}, detail::host_chars);
    auto dest = set_host_impl(n);
    pct_encode(
        dest,
        get(id_path).data(),
        s,
        {},
        detail::host_chars);
    decoded_[id_host] = s.size();
    host_type_ =
        urls::host_type::name;
    check_invariants();
    return *this;
}

url&
url::
set_encoded_host(string_view s)
{
    detail::copied_strings buf(
        this->string());
    s = buf.maybe_copy(s);
    // first try parsing it
    host_rule t;
    error_code ec;
    if(! grammar::parse_string(s, ec, t))
        detail::throw_invalid_argument(
            BOOST_CURRENT_LOCATION);
    BOOST_ASSERT(t.host_type !=
        urls::host_type::none);
    check_invariants();
    switch(t.host_type)
    {
    case urls::host_type::ipv4:
        return set_host(t.ipv4);

    case urls::host_type::ipv6:
        return set_host(t.ipv6);

    case urls::host_type::none:
    case urls::host_type::name:
    {
        auto dest =
            set_host_impl(s.size());
        std::memcpy(
            dest, s.data(), s.size());
        decoded_[id_host] =
            t.name.decoded_size;
        break;
    }
        BOOST_FALLTHROUGH;

    case urls::host_type::ipvfuture:
    {
        auto dest =
            set_host_impl(s.size());
        std::memcpy(
            dest, s.data(), s.size());
        decoded_[id_host] = s.size();
        break;
    }
    }
    host_type_ = t.host_type;
    check_invariants();
    return *this;
}

//------------------------------------------------

char*
url::
set_port_impl(std::size_t n)
{
    check_invariants();
    if(len(id_user) != 0)
    {
        // authority exists
        auto dest = resize_impl(
            id_port, n + 1);
        dest[0] = ':';
        check_invariants();
        return dest + 1;
    }
    auto dest = resize_impl(
        id_user, 3 + n);
    split(id_user, 2);
    split(id_pass, 0);
    split(id_host, 0);
    dest[0] = '/';
    dest[1] = '/';
    dest[2] = ':';
    check_invariants();
    return dest + 3;
}

url&
url::
remove_port() noexcept
{
    check_invariants();
    resize_impl(id_port, 0);
    port_number_ = 0;
    check_invariants();
    return *this;
}

url&
url::
set_port(std::uint16_t n)
{
    check_invariants();
    auto s =
        detail::make_printed(n);
    auto dest = set_port_impl(
        s.string().size());
    std::memcpy(
        dest, s.string().data(),
            s.string().size());
    port_number_ = n;
    check_invariants();
    return *this;
}

url&
url::
set_port(string_view s)
{
    detail::copied_strings buf(
        this->string());
    s = buf.maybe_copy(s);
    check_invariants();
    port_rule t;
    error_code ec;
    if(! grammar::parse_string(
            s, ec, t))
        detail::throw_invalid_argument(
            BOOST_CURRENT_LOCATION);
    auto dest =
        set_port_impl(t.str.size());
    std::memcpy(dest,
        t.str.data(), t.str.size());
    if(t.has_number)
        port_number_ = t.number;
    else
        port_number_ = 0;
    check_invariants();
    return *this;
}

//------------------------------------------------

url&
url::
remove_authority() noexcept
{
    check_invariants();
    if(len(id_user) == 0)
    {
        // no authority
        return *this;
    }
    if(get(id_path
        ).starts_with("//"))
    {
        // prepend "/."
        auto p = resize_impl(
            id_user, id_path, 2);
        p[0] = '/';
        p[1] = '.';
        split(id_user, 0);
        split(id_pass, 0);
        split(id_host, 0);
        split(id_port, 0);
    }
    else
    {
        resize_impl(
            id_user, id_path, 0);
    }
    host_type_ =
        urls::host_type::none;
    check_invariants();
    return *this;
}

url&
url::
set_encoded_authority(string_view s)
{
    detail::copied_strings buf(
        this->string());
    s = buf.maybe_copy(s);
    error_code ec;
    authority_rule t;
    if(! grammar::parse_string(s, ec, t))
        detail::throw_invalid_argument(
            BOOST_CURRENT_LOCATION);
    auto n = s.size() + 2;
    auto const need_slash =
        ! is_path_absolute() &&
        len(id_path) > 0;
    if(need_slash)
        ++n;
    auto dest = resize_impl(
        id_user, id_path, n);
    dest[0] = '/';
    dest[1] = '/';
    std::memcpy(dest + 2,
        s.data(), s.size());
    if(need_slash)
    {
        dest[n - 1] = '/';
        // VFALCO adjust table
    }
    if(t.has_userinfo)
    {
        auto const& t0 = t.userinfo;
        split(id_user,
            2 + t0.user.str.size());
        n -= 2 + t0.user.str.size();
        decoded_[id_user] =
            t0.user.decoded_size;
        if(t0.has_password)
        {
            split(id_pass, 2 +
                t0.password.str.size());
            decoded_[id_pass] =
                t0.password.decoded_size;
            n -= 2 + t0.password.str.size();
        }
        else
        {
            split(id_pass, 1);
            decoded_[id_pass] = 0;
            n -= 1;
        }
    }
    else
    {
        n -= 2;
        split(id_user, 2);
        split(id_pass, 0);
    }
    split(id_host,
        t.host.host_part.size());
    n -= t.host.host_part.size();
    host_type_ = t.host.host_type;
    if(host_type_ ==
        urls::host_type::ipv4)
    {
        auto const bytes =
            t.host.ipv4.to_bytes();
        std::memcpy(ip_addr_,
            bytes.data(), bytes.size());
        decoded_[id_host] =
            len(id_host);
    }
    else if(host_type_ ==
        urls::host_type::ipv6)
    {
        auto const bytes =
            t.host.ipv6.to_bytes();
        std::memcpy(ip_addr_,
            bytes.data(), bytes.size());
        decoded_[id_host] =
            len(id_host);
    }
    else if(host_type_ ==
        urls::host_type::ipvfuture)
    {
        decoded_[id_host] =
            len(id_host);
    }
    else
    {
        decoded_[id_host] =
            t.host.name.decoded_size;
    }
    if(need_slash)
        split(id_port, n - 1);
    else
        BOOST_ASSERT(len(id_port) == n);
    if(t.port.has_port)
    {
        if(t.port.has_number)
            port_number_ =
                t.port.port_number;
        else
            port_number_ = 0;
    }
    else
    {
        port_number_ = 0;
    }
    check_invariants();
    return *this;
}

//------------------------------------------------

url&
url::
remove_origin() noexcept
{
    check_invariants();
    if(len(id_scheme,
        id_path) == 0)
    {
        // no origin
        return *this;
    }

    decoded_[id_user] = 0;
    decoded_[id_pass] = 0;
    decoded_[id_host] = 0;
    host_type_ =
        urls::host_type::none;
    port_number_ = 0;

    // Check if we will be left with
    // "//" or a rootless segment
    // with a colon
    auto s = get(id_path);
    if(s.starts_with("//"))
    {
        // need "."
        auto dest = resize_impl(
            id_scheme, id_path, 1);
        dest[0] = '.';
        split(id_scheme, 0);
        split(id_user, 0);
        split(id_pass, 0);
        split(id_host, 0);
        split(id_port, 0);
        return *this;
    }
    if( s.empty() ||
        s.starts_with('/'))
    {
        // path-empty,
        // path-absolute
        resize_impl(
            id_scheme, id_path, 0);
        check_invariants();
        return *this;
    }
    auto const p =
        url_view::encoded_segments();
    BOOST_ASSERT(! p.empty());
    auto it = p.begin();
    if((*it).find_first_of(':') ==
        string_view::npos)
    {
        // path-noscheme
        resize_impl(
            id_scheme, id_path, 0);
        check_invariants();
        return *this;
    }

    // need "./"
    auto dest = resize_impl(
        id_scheme, id_path, 2);
    dest[0] = '.';
    dest[1] = '/';
    split(id_scheme, 0);
    split(id_user, 0);
    split(id_pass, 0);
    split(id_host, 0);
    split(id_port, 0);
    return *this;
}

//------------------------------------------------
//
// Path
//
//------------------------------------------------

/*  Return offset of i-th segment
*/
pos_t
url::
segment(
    std::size_t i) const noexcept
{
    if(i == 0)
        return offset(id_path);
    if(i == nseg_)
        return offset(id_query);
    BOOST_ASSERT(i < nseg_);
    auto it = s_ + offset(id_path) +
        detail::path_prefix(
            get(id_path));
    BOOST_ASSERT(it < s_ +
        offset(id_query));
    for(;;)
    {
        while(*it != '/')
            ++it;
        BOOST_ASSERT(it < s_ +
            offset(id_query));
        --i;
        if(i == 0)
            break;
        ++it;
    }
    return it - s_;
}

/*  Remove segments [first, last) and make
    room for nseg new segments inserted
    before first, with space for n chars
    including prefix and/or separators.

    Segments look like this, where ## is the
    malleable prefix and '/' is a literal slash:

    ##_0_ /_1_ /_2_ /_3_
*/
char*
url::
edit_segments(
    std::size_t i0,
    std::size_t i1,
    std::size_t n,
    std::size_t nseg)
{
    BOOST_ASSERT(i1 >= i0);
    BOOST_ASSERT(i1 - i0 <= nseg_);

    // new number of segments
    std::size_t const nseg1 =
        nseg_ + nseg - (i1 - i0);

    // [p0, p1) range to replace
    auto p0 = segment(i0);
    auto p1 = segment(i1);
    if(i1 == 0)
    {
        p1 += detail::path_prefix(
            get(id_path));
    }
    else if(
        i0 == 0 &&
        nseg == 0 &&
        i1 < nseg_)
    {
        // Remove the slash from segment i1
        // if it is becoming the new first
        // segment.
        BOOST_ASSERT(s_[p1] == '/');
        ++p1;
    }

    // old size of [p0, p1)
    auto const n0 = p1 - p0;

    // adjust capacity
    ensure_space(
        size() + n - n0,
        nseg1,
        nparam_);

    // start of output
    auto dest = s_ + p0;

    // move and size
    std::memmove(
        dest + n,
        s_ + p1,
        size() - p1);
    set_size(
        id_path,
        len(id_path) -
            (n0 - n));
    nseg_ = nseg1;
    s_[size()] = '\0';
    return dest;
}

// insert or replace [i0, i1)
// with [it0, it1)
void
url::
edit_segments(
    std::size_t i0,
    std::size_t i1,
    detail::any_path_iter&& it0,
    detail::any_path_iter&& it1,
    int abs_hint)
{
    bool abs;
    if( has_authority() ||
        abs_hint == -1)
        abs = is_path_absolute();
    else if(abs_hint == 1)
        abs = true;
    else
        abs = false;

/*
    Measure the number of characters and
    the number of segments we are inserting.
    This does not include leading or trailing
    separators.
*/
    error_code ec;
    std::size_t n = 0;
    std::size_t nseg = 0;
    bool more = it0.measure(n, ec);
    if(ec.failed())
        detail::throw_invalid_argument(
            BOOST_CURRENT_LOCATION);
    if(more)
    {
        for(;;)
        {
            ++nseg;
            more = it0.measure(n, ec);
            if(ec.failed())
                detail::throw_invalid_argument(
                    BOOST_CURRENT_LOCATION);
            if(! more)
                break;
            ++n;
        }
    }

/*  Calculate prefix size for new segment range:
        0 = ""
        1 = "/"
        2 = "./"
        3 = "/./"

    This is malleable prefix that might need to
    change according the URL scheme and authority.

*/
    int prefix;
    if(i0 > 0)
    {
        if(nseg > 0)
            prefix = 1;
        else
            prefix = 0;
    }
    else if(
        it0.front == "." &&
        nseg > 1)
    {
        if( abs ||
            has_authority())
            prefix = 3;
        else
            prefix = 2;
    }
    else if(has_authority())
    {
        if(nseg == 0)
            prefix = abs ? 1 : 0;
        else if(! it0.front.empty())
            prefix = 1;
        else
            prefix = 3;
    }
    else if(
        nseg > 1 &&
        it0.front.empty())
    {
        prefix = 3;
    }
    else if(
        ! abs &&
        ! has_scheme() &&
        (
            it0.front.find_first_of(
                ':') != string_view::npos ||
            it0.front.empty()))
    {
        BOOST_ASSERT(nseg > 0);
        prefix = 2;
    }
    else if(
        abs &&
        nseg > 0 &&
        it0.front.empty())
    {
        BOOST_ASSERT(
            ! has_authority());
        prefix = 3;
    }
    else
    {
        if(abs)
            prefix = 1;
        else
            prefix = 0;
    }

/*  Calculate suffix size for the new segments:
        0 = ""
        1 = "/"

    This extra suffix should cover the case where
    insertion at the first indexes leaves a
    missing slash in a relative path:

    "file.txt"
    -> insert "etc" as first segment
    -> becomes "etc" "/" "file.txt"

    "file.txt"
    -> insert "path/to" as first segments
    -> becomes "path/to" "/" "file.txt"

    "the/file.txt"
    -> insert "path/to" as first segments
    -> becomes "path/to" "/" "the/file.txt"

    The extra slash is not necessary when
    insertion is not at the first position:

    "path/file.txt"
    -> insert "to/the" as second segment
    -> becomes "path" "/to/the" "/file.txt"

    The extra slash is not necessary when
    the following position already has a slash
    (i.e. other existing valid segments):

    "/path/to/the/file.txt"
    -> replace "etc" as first segment
    -> becomes "/etc" "/to/the/file.txt"

*/
    int suffix;
    // inserting non-empty segments at the
    // beginning of non-empty segments
    if( nseg > 0 &&
        i0 == 0 &&
        i1 == 0 &&
        nseg_ != 0)
    {
        suffix = 1;
    }
    else
    {
        suffix = 0;
    }

    // copy
    n += prefix + suffix;
    auto dest = edit_segments(
        i0, i1, n, nseg);
    auto const last = dest + n;

/*  Write all characters in the destination:

    The output proceeds as:

        prefix [ segment [ '/' segment ] ] suffix
*/
    switch(prefix)
    {
    case 3:
        *dest++ = '/';
        *dest++ = '.';
        *dest++ = '/';
        break;
    case 2:
        *dest++ = '.';
        BOOST_FALLTHROUGH;
    case 1:
        *dest++ = '/';
        break;
    default:
        break;
    }
/*
    Output each segment, placing a slash
    only in between new segments. Leading
    or trailing separators are handled
    outside the loop.
*/
    if(nseg > 0)
    {
        for(;;)
        {
            it1.copy(dest, last);
            if(--nseg == 0)
                break;
            *dest++ = '/';
        }
    }
    if(suffix == 1)
        *dest++ = '/';
}

//------------------------------------------------

bool
url::
set_path_absolute(bool absolute)
{
    if(len(id_path) == 0)
    {
        if(! absolute)
            return true;
        auto dest = resize_impl(
            id_path, 1);
        *dest = '/';
        // VFALCO Update table
        return true;
    }

    if(s_[offset(id_path)] == '/')
    {
        if(absolute)
            return true;
        if( has_authority() &&
            len(id_path) > 1)
            return false;
        auto n = len(id_port);
        split(id_port, n + 1);
        resize_impl(id_port, n);
        // VFALCO Update table
        return true;
    }

    if(! absolute)
        return true;
    auto n = len(id_port);
    auto dest = resize_impl(
        id_port, n + 1) + n;
    split(id_port, n);
    *dest = '/';
    // VFALCO Update table
    return true;
}

url&
url::
set_encoded_path(
    string_view s)
{
    detail::copied_strings buf(
        this->string());
    s = buf.maybe_copy(s);
    int abs_hint;
    if(s.starts_with('/'))
        abs_hint = 1;
    else
        abs_hint = 0;
    edit_segments(
        0,
        nseg_,
        detail::enc_path_iter(s),
        detail::enc_path_iter(s),
        abs_hint);
    return *this;
}

url&
url::
set_path(
    string_view s)
{
    detail::copied_strings buf(
        this->string());
    s = buf.maybe_copy(s);
    int abs_hint;
    if(s.starts_with('/'))
        abs_hint = 1;
    else
        abs_hint = 0;
    edit_segments(
        0, nseg_,
        detail::plain_path_iter(s),
        detail::plain_path_iter(s),
        abs_hint);
    return *this;
}

segments_encoded
url::
encoded_segments() noexcept
{
    return segments_encoded(*this);
}

//------------------------------------------------
//
// Query
//
//------------------------------------------------

auto
url::
param(
    std::size_t i) const noexcept ->
        raw_param
{
    auto const make_param =
    [this](
        std::size_t pos,
        std::size_t n)
    {
        string_view s(s_ + pos, n);
        auto i = s.find_first_of('=');
        if(i == string_view::npos)
            return raw_param{ pos, n, 0 };
        return raw_param{ pos, i, n - i };
    };

    if(nparam_ == 0)
        return { offset(id_query), 0, 0 };
    if(i == nparam_)
        return { offset(id_frag), 0, 0 };
    BOOST_ASSERT(i <= nparam_);
    auto n = len(id_query);
    if(nparam_ < 2)
        return make_param(
            offset(id_query), n);
    auto it = s_ + offset(id_query);
    auto start = it;
    auto const last =
        s_ + offset(id_frag);
    BOOST_ASSERT(n > 0);
    for(;;)
    {
        for(;;)
        {
            ++it;
            if(it == last)
                break;
            if(*it == '&')
                break;
        }
        if(i == 0)
            break;
        start = it;
        --i;
    }
    return make_param(
        start - s_, it - start);
}

char*
url::
edit_params(
    std::size_t i0,
    std::size_t i1,
    std::size_t n,
    std::size_t nparam)
{
    BOOST_ASSERT(i1 >= i0);
    BOOST_ASSERT(i1 - i0 <= nparam_);

    // new number of params
    auto const nparam1 =
        nparam_+ nparam - (i1 - i0);

    // [r0, r1) range to replace
    auto const r0 = param(i0);
    auto const r1 = param(i1);

    // old size of [r0, r1)
    auto const n0 = r1.pos - r0.pos;

    // adjust capacity
    ensure_space(
        size() + n - n0,
        nseg_,
        nparam1);

    // start of output
    auto dest = s_ + r0.pos;

    // move and size
    std::memmove(
        dest + n,
        s_ + r1.pos,
        size() - r1.pos);
    set_size(
        id_query,
        len(id_query) + (
            n - n0));
    nparam_ = nparam1;
    s_[size()] = '\0';
    return dest;
}

void
url::
edit_params(
    std::size_t i0,
    std::size_t i1,
    detail::any_query_iter&& it0,
    detail::any_query_iter&& it1,
    bool set_hint)
{
    check_invariants();
    if(! set_hint)
        set_hint = has_query();

    // measure
    error_code ec;
    std::size_t n = 0;
    std::size_t nparam = 0;
    bool more = it0.measure(n, ec);
    if(ec.failed())
        detail::throw_invalid_argument(
            BOOST_CURRENT_LOCATION);
    bool prefix;
    if(more)
    {
        ++n;
        prefix = i0 == 0;
        for(;;)
        {
            ++nparam;
            more = it0.measure(n, ec);
            if(ec.failed())
                detail::throw_invalid_argument(
                    BOOST_CURRENT_LOCATION);
            if(! more)
                break;
            ++n;
        }
    }
    else if(
        i0 == 0 &&
        set_hint)
    {
        prefix = true;
        ++n;
    }
    else
    {
        prefix = false;
    }

    // copy
    auto dest = edit_params(
        i0, i1, n, nparam);
    if(prefix)
        *dest++ = '?';
    if(nparam > 0)
    {
        auto const last = dest + n;
        if(i0 != 0)
            *dest++ = '&';
        for(;;)
        {
            it1.copy(dest, last);
            if(--nparam == 0)
                break;
            *dest++ = '&';
        }
    }

    check_invariants();
}

//------------------------------------------------
//
// Query
//
//------------------------------------------------

url&
url::
remove_query() noexcept
{
    resize_impl(id_query, 0);
    nparam_ = 0;
    return *this;
}

url&
url::
set_encoded_query(
    string_view s)
{
    detail::copied_strings buf(
        this->string());
    s = buf.maybe_copy(s);
    check_invariants();
    edit_params(
        0,
        nparam_,
        detail::enc_query_iter(s),
        detail::enc_query_iter(s),
        true);
    check_invariants();
    return *this;
}

url&
url::
set_query(
    string_view s)
{
    detail::copied_strings buf(
        this->string());
    s = buf.maybe_copy(s);
    edit_params(
        0,
        nparam_,
        detail::plain_query_iter(s),
        detail::plain_query_iter(s),
        true);
    return *this;
}

//------------------------------------------------
//
// Fragment
//
//------------------------------------------------

char*
url::
set_fragment_impl(
    std::size_t n)
{
    auto dest = resize_impl(
        id_frag, n + 1);
    dest[0] = '#';
    return dest + 1;
}

url&
url::
remove_fragment() noexcept
{
    resize_impl(id_frag, 0);
    decoded_[id_frag] = 0;
    return *this;
}

url&
url::
set_encoded_fragment(
    string_view s)
{
    detail::copied_strings buf(
        this->string());
    s = buf.maybe_copy(s);
    check_invariants();
    error_code ec;
    fragment_rule t;
    if(! grammar::parse_string(s, ec, t))
        detail::throw_invalid_argument(
            BOOST_CURRENT_LOCATION);
    auto dest = set_fragment_impl(s.size());
    decoded_[id_frag] = t.s.decoded_size;
    if(! s.empty())
        std::memcpy(
            dest, s.data(), s.size());
    check_invariants();
    return *this;
}

url&
url::
set_fragment(
    string_view s)
{
    detail::copied_strings buf(
        this->string());
    s = buf.maybe_copy(s);
    check_invariants();
    auto const n = pct_encode_bytes(
        s, {}, fragment_chars);
    auto dest = set_fragment_impl(n);
    pct_encode(
        dest,
        get(id_end).data(),
        s,
        {},
        fragment_chars);
    decoded_[id_frag] = s.size();
    check_invariants();
    return *this;
}

//------------------------------------------------
//
// Resolution
//
//------------------------------------------------

bool
url::
resolve(
    url_view const& base,
    url_view const& ref,
    error_code& ec)
{
    auto const remove_dot_segments =
        [this]
    {
        auto segs = encoded_segments();
        if(segs.empty())
            return;
        auto it = segs.begin();
        int n = 0;
        for(;;)
        {
            if(*it == ".")
            {
                if(it != std::prev(segs.end()))
                    it = segs.erase(it);
                else
                    it = segs.replace(it, "");
            }
            else if(it == segs.begin())
            {
            #if 0
                if(*it == "..")
                    it = segs.erase(it);
                else
                    ++it;
            #else
            /*  Errata 4547
                https://www.rfc-editor.org/errata/eid4547
            */
                if(*it != "..")
                    ++n;
                ++it;
            #endif
            }
            else if(*it == "..")
            {
                if(n > 0)
                {
                    it = segs.erase(std::prev(it));
                    if( it == segs.begin() ||
                        it != std::prev(segs.end()))
                        it = segs.erase(it);
                    else
                        it = segs.replace(it, "");
                    --n;
                }
                else
                {
                    ++it;
                }
            }
            else
            {
                ++n;
                ++it;
            }
            if(it == segs.end())
                break;
        }
    };

    if(! base.has_scheme())
    {
        ec = error::not_a_base;
        return false;
    }

    ec = {};

    //
    // 5.2.2. Transform References
    // https://datatracker.ietf.org/doc/html/rfc3986#section-5.2.2
    //

    if(ref.has_scheme())
    {
        ensure_space(
            ref.size(),
            ref.nseg_,
            ref.nparam_);
        copy(ref);
        remove_dot_segments();
        return true;
    }
    if(ref.has_authority())
    {
        ensure_space(
            base.offset(id_user) +
                ref.size(),
            ref.nseg_,
            ref.nparam_);
        clear();
        set_scheme(base.scheme());
        set_encoded_authority(
            ref.encoded_authority());
        set_encoded_path(
            ref.encoded_path());
        remove_dot_segments();
        if(ref.has_query())
            set_encoded_query(
                ref.encoded_query());
        if(ref.has_fragment())
            set_encoded_fragment(
                ref.encoded_fragment());
        return true;
    }
    if(ref.encoded_path().empty())
    {
        if(ref.has_query())
        {
            ensure_space(
                base.offset(id_query) +
                    ref.size(),
                base.nseg_,
                ref.nparam_);
            clear();
            set_scheme(base.scheme());
            if(base.has_authority())
                set_encoded_authority(
                    base.encoded_authority());
            set_encoded_path(
                base.encoded_path());
            remove_dot_segments();
            set_encoded_query(
                ref.encoded_query());
        }
        else
        {
            ensure_space(
                base.offset(id_query) +
                    ref.size(),
                base.nseg_,
                base.nparam_);
            clear();
            set_scheme(base.scheme());
            if(base.has_authority())
                set_encoded_authority(
                    base.encoded_authority());
            set_encoded_path(
                base.encoded_path());
            remove_dot_segments();
            if(base.has_query())
                set_encoded_query(
                    base.encoded_query());
        }
        if(ref.has_fragment())
            set_encoded_fragment(
                ref.encoded_fragment());
        return true;
    }
    if(ref.encoded_path().starts_with('/'))
    {
        ensure_space(
            base.offset(id_path) +
                ref.size(),
            ref.nseg_,
            ref.nparam_);
        clear();
        set_scheme(base.scheme());
        if(base.has_authority())
            set_encoded_authority(
                base.encoded_authority());
        set_encoded_path(
            ref.encoded_path());
        remove_dot_segments();
        if(ref.has_query())
            set_encoded_query(
                ref.encoded_query());
        if(ref.has_fragment())
            set_encoded_fragment(
                ref.encoded_fragment());
        return true;
    }
    ensure_space(
        base.offset(id_query) +
            ref.size(),
        base.nseg_ + ref.nseg_,
        ref.nparam_);
    clear();
    set_scheme(base.scheme());
    if(base.has_authority())
        set_encoded_authority(
            base.encoded_authority());
    {
        // 5.2.3. Merge Paths
        auto es = encoded_segments();
        if(base.nseg_ > 0)
        {
            set_encoded_path(
                base.encoded_path());
            if(nseg_ > 0)
                es.pop_back();
        }
        es.insert(es.end(),
            ref.encoded_segments().begin(),
            ref.encoded_segments().end());
    }
    remove_dot_segments();
    if(ref.has_query())
        set_encoded_query(
            ref.encoded_query());
    if(ref.has_fragment())
        set_encoded_fragment(
            ref.encoded_fragment());
    return true;
}

//------------------------------------------------
//
// Normalization
//
//------------------------------------------------

void
url::
normalize_octets_impl(
    int id,
    grammar::lut_chars const& cs) noexcept
{
    char* it = s_ + offset(id);
    char* end = s_ + offset(id + 1);
    char buf = 0;
    char* dest = it;
    while (it < end)
    {
        if (*it != '%')
        {
            *dest = *it;
            ++it;
            ++dest;
            continue;
        }
        if (end - it < 3)
            break;

        // decode unreserved octets
        pct_decode_unchecked(
            &buf,
            &buf + 1,
            string_view(it, 3));
        if (cs(buf))
        {
            *dest = buf;
            it += 3;
            ++dest;
            continue;
        }

        // uppercase percent-encoding triplets
        ++it;
        *it = grammar::ascii_toupper(*it);
        ++it;
        *it = grammar::ascii_toupper(*it);
        ++it;
        dest += 3;
    }
    if (it != dest)
    {
        std::size_t diff = it - dest;
        std::size_t n = len(id) - diff;
        shrink_impl(id, n);
        s_[size()] = '\0';
    }
}

void
url::
decoded_to_lower_impl(int id) noexcept
{
    char* it = s_ + offset(id);
    char const* const end = s_ + offset(id + 1);
    while(it < end)
    {
        if (*it != '%')
        {
            *it = grammar::ascii_tolower(
                *it);
            ++it;
            continue;
        }
        it += 3;
    }
}

void
url::
to_lower_impl(int id) noexcept
{
    char* it = s_ + offset(id);
    char const* const end = s_ + offset(id + 1);
    while(it < end)
    {
        *it = grammar::ascii_tolower(
            *it);
        ++it;
    }
}

url&
url::
normalize_scheme()
{
    to_lower_impl(id_scheme);
    return *this;
}

url&
url::
normalize_authority()
{
    // normalize host
    if (host_type() == urls::host_type::name)
    {
        normalize_octets_impl(
            id_host,
            detail::reg_name_chars);
    }
    decoded_to_lower_impl(id_host);

    // normalize password
    normalize_octets_impl(id_pass, detail::password_chars);

    // normalize user
    normalize_octets_impl(id_user, detail::user_chars);
    return *this;
}

url&
url::
normalize_path()
{
    normalize_octets_impl(id_path, detail::path_chars);
    string_view p = encoded_path();
    char* p_dest = s_ + offset(id_path);
    char* p_end = s_ + offset(id_path + 1);
    std::size_t pn = p.size();
    bool abs = is_path_absolute();
    std::size_t n = detail::remove_dot_segments(
        p_dest, p_end, p, abs);
    if (n != pn)
    {
        BOOST_ASSERT(n < pn);
        shrink_impl(id_path, n);
        p = encoded_path();
        if (!p.empty())
            nseg_ = std::count(
                p.begin() + 1, p.end(), '/') + 1;
        else
            nseg_ = 0;
    }
    return *this;
}

url&
url::
normalize_query()
{
    normalize_octets_impl(id_query, query_chars);
    return *this;
}

url&
url::
normalize_fragment()
{
    normalize_octets_impl(id_frag, fragment_chars);
    return *this;
}

url&
url::
normalize()
{
    normalize_fragment();
    normalize_query();
    normalize_path();
    normalize_authority();
    normalize_scheme();
    return *this;
}

//------------------------------------------------
//
// Implementation
//
//------------------------------------------------

void
url::
check_invariants() const noexcept
{
    using alignment::align_up;
    BOOST_ASSERT(align_up(cap_,
        alignof(pos_t)) == cap_);
    BOOST_ASSERT(
        len(id_scheme) == 0 ||
        get(id_scheme).ends_with(':'));
    BOOST_ASSERT(
        len(id_user) == 0 ||
        get(id_user).starts_with("//"));
    BOOST_ASSERT(
        len(id_pass) == 0 ||
        get(id_user).starts_with("//"));
    BOOST_ASSERT(
        len(id_pass) == 0 ||
        (len(id_pass) == 1 &&
            get(id_pass) == "@") ||
        (len(id_pass) > 1 &&
            get(id_pass).starts_with(':') &&
            get(id_pass).ends_with('@')));
    BOOST_ASSERT(
        len(id_user, id_path) == 0 ||
        get(id_user).starts_with("//"));
    BOOST_ASSERT(
        len(id_port) == 0 ||
        get(id_port).starts_with(':'));
    BOOST_ASSERT(
        len(id_query) == 0 ||
        get(id_query).starts_with('?'));
    BOOST_ASSERT(
        len(id_frag) == 0 ||
        get(id_frag).starts_with('#'));
    BOOST_ASSERT(c_str()[size()] == '\0');
    // validate segments
#if 0
    if(nseg > 0)
    {
        auto it = cs_ +
            offset(id_path);
        auto const end = s_ +
            offset(id_query);
        error_code ec;
        pct_encoded_str t;
        auto start = it;
        if(get(id_path).starts_with('/'))
            path_abempty_rule::begin(
                it, end, ec, t);
        else
            path_rootless_rule::begin(
                it, end, ec, t);
        for(std::size_t i = 0;;++i)
        {
            if(ec == grammar::error::end)
                break;
            BOOST_ASSERT(! ec.failed());
            if(ec.failed())
                break;
            BOOST_ASSERT(
                cs_ + segment_pos(i) == start);
            BOOST_ASSERT(
                start + segment_len(i) == it);
            start = it;
            path_abempty_rule::increment(
                it, end, ec, t);
        }
    }
#endif
}

void
url::
build_tab() noexcept
{
#if 0
    // path
    if(nseg_ > 1)
    {
        error_code ec;
        // path table
        pos_t* tab = tab_end() - 1;
        auto s = get(id_path);
        auto it = s.data();
        auto const end = it + s.size();
        pct_encoded_str t;
        if( s.starts_with('/') ||
            s.empty())
            path_abempty_rule::begin(
                it, end, ec, t);
        else
            path_rootless_rule::begin(
                it, end, ec, t);
        for(;;)
        {
            if(ec == grammar::error::end)
                break;
            if(ec)
                detail::throw_system_error(ec,
                    BOOST_CURRENT_LOCATION);
            *tab = it - s_;
            tab -= 2;
            path_abempty_rule::increment(
                it, end, ec, t);
        }
    }
    // query
    if(nparam_ > 1)
    {
        error_code ec;
        // query table
        pos_t* tab = tab_end() - 2;
        auto s = get(id_query);
        auto it = s.data();
        auto const end = it + s.size();
        query_param_view t;
        query_rule::begin(
            it, end, ec, t);
        for(;;)
        {
            if(ec == grammar::error::end)
                break;
            if(ec)
                detail::throw_system_error(ec,
                    BOOST_CURRENT_LOCATION);
            *tab = it - s_;
            tab -= 2;
            query_rule::increment(
                it, end, ec, t);
        }
    }
#endif
}

void
url::
ensure_space(
    std::size_t nchar, // excluding null
    std::size_t nseg,
    std::size_t nparam)
{
    // minimum size
    if( nchar < 15)
        nchar = 15;
    if(nchar > max_size() - 1)
        detail::throw_length_error(
            "nchar > max_size",
            BOOST_CURRENT_LOCATION);
    std::size_t new_cap = nchar + 1;
    if(nseg_ > 1)
        new_cap += sizeof(pos_t) *
            (nseg - 1);
    if(nparam > 0)
        new_cap += 2 * sizeof(pos_t) *
            nparam;
    if(new_cap <= cap_)
        return;
    char* s;
    if(s_ != nullptr)
    {
        // 50% growth policy
        auto n = cap_ + (cap_ / 2);
        if(n < cap_)
        {
            // overflow
            n = std::size_t(-1) &
                ~(alignof(pos_t)-1);
        }
        if( new_cap < n)
            new_cap = n;
        s = allocate(new_cap);
        std::memcpy(s, s_, size());
        deallocate(s_);
    }
    else
    {
        s = allocate(new_cap);
    }
    s_ = s;
    cs_ = s;
}

char*
url::
resize_impl(
    int id,
    std::size_t new_size)
{
    return resize_impl(
        id, id + 1, new_size);
}

char*
url::
resize_impl(
    int first,
    int last,
    std::size_t new_len)
{
    auto const n0 = len(first, last);
    if(new_len == 0 && n0 == 0)
        return s_ + offset(first);
    if(new_len <= n0)
    {
        return shrink_impl(first, last, new_len);
    }

    // growing
    std::size_t n = new_len - n0;
    ensure_space(size() + n,
        nseg_, nparam_);
    auto const pos =
        offset(last);
    // adjust chars
    std::memmove(
        s_ + pos + n,
        s_ + pos,
        offset(id_end) -
            pos + 1);
    // collapse (first, last)
    collapse(first, last,
        offset(last) + n);
    // shift (last, end) right
    adjust(last, id_end, n);
#if 0
    // update table
    if( nseg > 1 &&
        last > id_path &&
        first < id_path)
    {
        // adjust segments
        auto const tab =
            tab_end() - 1;
        for(std::size_t i = 0;
            i < nseg - 1; ++i)
            tab[0-2*i] += n;
    }
    if( nparam > 1 &&
        last > id_query &&
        first < id_query)
    {
        // adjust params
        auto const tab =
            tab_end() - 2;
        for(std::size_t i = 0;
            i < nparam - 1; ++i)
            tab[0-2*i] += n;
    }
#endif
    s_[size()] = '\0';
    return s_ + offset(first);
}

char*
url::
shrink_impl(
    int id,
    std::size_t new_size)
{
    return shrink_impl(
        id, id + 1, new_size);
}

char*
url::
shrink_impl(
    int first,
    int last,
    std::size_t new_len)
{
    // shrinking
    auto const n0 = len(first, last);
    BOOST_ASSERT(new_len <= n0);
    std::size_t n = n0 - new_len;
    auto const pos =
        offset(last);
    // adjust chars
    std::memmove(
        s_ + pos - n,
        s_ + pos,
        offset(
            id_end) - pos + 1);
    // collapse (first, last)
    collapse(first,  last,
        offset(last) - n);
    // shift (last, end) left
    adjust(
        last, id_end, 0 - n);
#if 0
    // update table
    if( nseg > 1 &&
        first <= id_path)
    {
        // adjust segments
        auto const tab =
            tab_end() - 1;
        for(std::size_t i = 0;
            i < nseg - 1; ++i)
            tab[0-2*i] += 0 - n;
    }
    if( nparam > 1 &&
        first <= id_query)
    {
        // adjust params
        auto const tab =
            tab_end() - 2;
        for(std::size_t i = 0;
            i < nparam - 1; ++i)
            tab[0-2*i] += 0 - n;
    }
#endif
    s_[size()] = '\0';
    return s_ + offset(first);
}

//------------------------------------------------

std::ostream&
operator<<(
    std::ostream& os,
    url const& u)
{
    os << u.string();
    return os;
}

} // urls
} // boost

#endif
//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_URL_VIEW_IPP
#define BOOST_URL_IMPL_URL_VIEW_IPP

#include <array>
#include <ostream>

namespace boost {
namespace urls {

struct url_view::shared_impl :
    url_view
{
    virtual
    ~shared_impl()
    {
    }

    shared_impl(
        url_view const& u) noexcept
        : url_view(u, reinterpret_cast<
            char const*>(this + 1))
    {
    }
};

url_view::
url_view(url_view const&) noexcept = default;

url_view::
url_view(
    int,
    char const* cs) noexcept
    : cs_(cs)
{
}

url_view::
url_view(
    url_view const& u,
    char const* cs) noexcept
    : url_view(u)
{
    cs_ = cs;
}

//------------------------------------------------

url_view::
~url_view()
{
}

url_view::
url_view() noexcept = default;

url_view&
url_view::
operator=(url_view const&) noexcept = default;

url_view::
url_view(string_view s)
{
    auto r = parse_uri_reference(s);
    if(r.has_error())
        detail::throw_invalid_argument(
            BOOST_CURRENT_LOCATION);
    *this = r.value();
}

//------------------------------------------------
//
// Observers
//
//------------------------------------------------

std::shared_ptr<
    url_view const>
url_view::
collect() const
{
    using T = shared_impl;
    using Alloc = std::allocator<char>;
    Alloc a;
    auto p = std::allocate_shared<T>(
        detail::over_allocator<T, Alloc>(
            size(), a), *this);
    std::memcpy(
        reinterpret_cast<char*>(
            p.get() + 1), data(), size());
    return p;
}

//------------------------------------------------
//
// Scheme
//
//------------------------------------------------

bool
url_view::
has_scheme() const noexcept
{
    auto const n = len(
        id_scheme);
    if(n == 0)
        return false;
    BOOST_ASSERT(n > 1);
    BOOST_ASSERT(
        get(id_scheme
            ).ends_with(':'));
    return true;
}

string_view
url_view::
scheme() const noexcept
{
    auto s = get(id_scheme);
    if(! s.empty())
    {
        BOOST_ASSERT(s.size() > 1);
        BOOST_ASSERT(s.ends_with(':'));
        s.remove_suffix(1);
    }
    return s;
}

urls::scheme
url_view::
scheme_id() const noexcept
{
    return scheme_;
}

//----------------------------------------------------------
//
// Authority
//
//----------------------------------------------------------

string_view
url_view::
encoded_authority() const noexcept
{
    auto s = get(id_user, id_path);
    if(! s.empty())
    {
        BOOST_ASSERT(has_authority());
        s.remove_prefix(2);
    }
    return s;
}

authority_view
url_view::
authority() const noexcept
{
    string_view s = encoded_authority();
    authority_view a;
    a.cs_ = s.data();
    pos_t off_user = s.data() - data();
    a.offset_[0] = offset(id_pass) - off_user;
    a.offset_[1] = offset(id_host) - off_user;
    a.offset_[2] = offset(id_port) - off_user;
    a.offset_[3] = offset(id_path) - off_user;
    a.decoded_[0] = decoded_[id_user];
    a.decoded_[1] = decoded_[id_pass];
    a.decoded_[2] = decoded_[id_host];
    a.decoded_[3] = decoded_[id_port];
    for (int i = 0; i < 16; ++i)
        a.ip_addr_[i] = ip_addr_[i];
    a.port_number_ = port_number_;
    a.host_type_ = host_type_;
    return a;
}

// userinfo

bool
url_view::
has_userinfo() const noexcept
{
    auto n = len(id_pass);
    if(n == 0)
        return false;
    BOOST_ASSERT(has_authority());
    BOOST_ASSERT(get(
        id_pass).ends_with('@'));
    return true;
}

string_view
url_view::
encoded_userinfo() const noexcept
{
    auto s = get(
        id_user, id_host);
    if(s.empty())
        return s;
    BOOST_ASSERT(
        has_authority());
    s.remove_prefix(2);
    if(s.empty())
        return s;
    BOOST_ASSERT(
        s.ends_with('@'));
    s.remove_suffix(1);
    return s;
}

string_view
url_view::
encoded_user() const noexcept
{
    auto s = get(id_user);
    if(! s.empty())
    {
        BOOST_ASSERT(
            has_authority());
        s.remove_prefix(2);
    }
    return s;
}

bool
url_view::
has_password() const noexcept
{
    auto const n = len(id_pass);
    if(n > 1)
    {
        BOOST_ASSERT(get(id_pass
            ).starts_with(':'));
        BOOST_ASSERT(get(id_pass
            ).ends_with('@'));
        return true;
    }
    BOOST_ASSERT(n == 0 || get(
        id_pass).ends_with('@'));
    return false;
}

string_view
url_view::
encoded_password() const noexcept
{
    auto s = get(id_pass);
    switch(s.size())
    {
    case 1:
        BOOST_ASSERT(
            s.starts_with('@'));
        BOOST_FALLTHROUGH;
    case 0:
        return s.substr(0, 0);
    default:
        break;
    }
    BOOST_ASSERT(
        s.ends_with('@'));
    BOOST_ASSERT(
        s.starts_with(':'));
    return s.substr(1,
        s.size() - 2);
}

// host

string_view
url_view::
encoded_host() const noexcept
{
    return get(id_host);
}

urls::ipv4_address
url_view::
ipv4_address() const noexcept
{
    if(host_type_ !=
        urls::host_type::ipv4)
        return {};
    std::array<
        unsigned char, 4> bytes;
    std::memcpy(
        &bytes[0],
        &ip_addr_[0], 4);
    return urls::ipv4_address(
        bytes);
}

urls::ipv6_address
url_view::
ipv6_address() const noexcept
{
    if(host_type_ ==
        urls::host_type::ipv6)
    {
        std::array<
            unsigned char, 16> bytes;
        std::memcpy(
            &bytes[0],
            &ip_addr_[0], 16);
        return urls::ipv6_address(
            bytes);
    }
    return urls::ipv6_address();
}

string_view
url_view::
ipv_future() const noexcept
{
    if(host_type_ ==
        urls::host_type::ipvfuture)
        return get(id_host);
    return {};
}

// port

bool
url_view::
has_port() const noexcept
{
    auto const n = len(id_port);
    if(n == 0)
        return false;
    BOOST_ASSERT(
        get(id_port).starts_with(':'));
    return true;
}

string_view
url_view::
port() const noexcept
{
    auto s = get(id_port);
    if(s.empty())
        return s;
    BOOST_ASSERT(has_port());
    return s.substr(1);
}

std::uint16_t
url_view::
port_number() const noexcept
{
    BOOST_ASSERT(
        has_port() ||
        port_number_ == 0);
    return port_number_;
}

string_view
url_view::
encoded_host_and_port() const noexcept
{
    return get(id_host, id_path);
}

string_view
url_view::
encoded_origin() const noexcept
{
    if(len(id_user) < 2)
        return {};
    return get(id_scheme, id_path);
}

//----------------------------------------------------------
//
// Query
//
//----------------------------------------------------------

bool
url_view::
has_query() const noexcept
{
    auto const n = len(
        id_query);
    if(n == 0)
        return false;
    BOOST_ASSERT(
        get(id_query).
            starts_with('?'));
    return true;
}

string_view
url_view::
encoded_query() const noexcept
{
    auto s = get(id_query);
    if(s.empty())
        return s;
    BOOST_ASSERT(
        s.starts_with('?'));
    return s.substr(1);
}

params_encoded_view
url_view::
encoded_params() const noexcept
{
    auto s = get(id_query);
    if(s.empty())
        return params_encoded_view(s, 0);
    BOOST_ASSERT(s[0] == '?');
    s.remove_prefix(1);
    return params_encoded_view(s, nparam_);
}

//----------------------------------------------------------
//
// Fragment
//
//----------------------------------------------------------

bool
url_view::
has_fragment() const noexcept
{
    auto const n = len(id_frag);
    if(n == 0)
        return false;
    BOOST_ASSERT(
        get(id_frag).
            starts_with('#'));
    return true;
}

string_view
url_view::
encoded_fragment() const noexcept
{
    auto s = get(id_frag);
    if(s.empty())
        return s;
    BOOST_ASSERT(
        s.starts_with('#'));
    return s.substr(1);
}

//------------------------------------------------
//
// Comparisons
//
//------------------------------------------------

int
url_view::
compare(const url_view& other) const noexcept
{
    int comp = detail::ci_compare(
        scheme(),
        other.scheme());
    if ( comp != 0 )
        return comp;

    comp = detail::compare_encoded(
        encoded_user(),
        other.encoded_user());
    if ( comp != 0 )
        return comp;

    comp = detail::compare_encoded(
        encoded_password(),
        other.encoded_password());
    if ( comp != 0 )
        return comp;

    comp = detail::ci_compare_encoded(
        encoded_host(),
        other.encoded_host());
    if ( comp != 0 )
        return comp;

    comp = detail::compare(
        port(),
        other.port());
    if ( comp != 0 )
        return comp;

    comp = detail::normalized_path_compare(
        encoded_path(),
        other.encoded_path(),
        is_path_absolute(),
        other.is_path_absolute());
    if ( comp != 0 )
        return comp;

    comp = detail::compare_encoded(
        encoded_query(),
        other.encoded_query());
    if ( comp != 0 )
        return comp;

    comp = detail::compare_encoded(
        encoded_fragment(),
        other.encoded_fragment());
    if ( comp != 0 )
        return comp;

    return 0;
}


//------------------------------------------------
//
// Parsing
//
//------------------------------------------------

void
url_view::
apply(
    scheme_part_rule const& t) noexcept
{
    scheme_ = t.scheme_id;
    if(t.scheme_id !=
        urls::scheme::none)
    set_size(
        id_scheme,
        t.scheme_part.size());
}

void
url_view::
apply(
    host_rule const& t) noexcept
{
    host_type_ = t.host_type;
    if(t.host_type ==
        urls::host_type::name)
    {
        decoded_[id_host] =
            t.name.decoded_size;
    }
    else if(t.host_type ==
        urls::host_type::ipv4)
    {
        auto const bytes =
            t.ipv4.to_bytes();
        std::memcpy(
            &ip_addr_[0],
            bytes.data(), 4);
    }
    else if(t.host_type ==
        urls::host_type::ipv6)
    {
        auto const bytes =
            t.ipv6.to_bytes();
        std::memcpy(
            &ip_addr_[0],
            bytes.data(), 16);
    }

    if(t.host_type !=
        urls::host_type::none)
        set_size(
            id_host,
            t.host_part.size());
}

void
url_view::
apply(
    authority_rule const& t) noexcept
{
    if(t.has_userinfo)
    {
        auto const& u = t.userinfo;

        // leading "//" for authority
        set_size(
            id_user,
            u.user.str.size() + 2);
        decoded_[id_user] = u.user.decoded_size;

        if(u.has_password)
        {
            // leading ':' for password,
            // trailing '@' for userinfo
            set_size(
                id_pass,
                u.password.str.size() + 2);
            decoded_[id_pass] =
                u.password.decoded_size;
        }
        else
        {
            // trailing '@' for userinfo
            set_size(id_pass, 1);
            decoded_[id_pass] = 0;
        }
    }
    else
    {
        // leading "//" for authority
        set_size(id_user, 2);
        decoded_[id_user] = 0;
    }

    // host
    apply(t.host);

    // port
    if(t.port.has_port)
    {
        set_size(
            id_port,
            t.port.port.size() + 1);

        if(t.port.has_number)
            port_number_ =
                t.port.port_number;
    }
}

void
url_view::
apply(
    parsed_path const& t) noexcept
{
    auto s = t.path;
    set_size(id_path, s.size());
    nseg_ = detail::path_segments(
        t.path, t.count);
}

void
url_view::
apply(
    query_part_rule const& t) noexcept
{
    if(t.has_query)
    {
        set_size(
            id_query,
            t.query_part.size());
        nparam_ = t.query.v.size();
    }
    else
    {
        nparam_ = 0;
    }
}

void
url_view::
apply(
    fragment_part_rule const& t) noexcept
{
    if(t.has_fragment)
    {
        set_size(
            id_frag,
            t.fragment.str.size() + 1);
        decoded_[id_frag] =
            t.fragment.decoded_size;
    }
    else
    {
        decoded_[id_frag] = 0;
    }
}

//------------------------------------------------
//
// Parsing
//
//------------------------------------------------

result<url_view>
parse_absolute_uri(
    string_view s)
{
    if(s.size() > url_view::max_size())
        detail::throw_length_error(
            "url_view::max_size exceeded",
            BOOST_CURRENT_LOCATION);

    error_code ec;
    absolute_uri_rule t;
    if(! grammar::parse_string(s, ec, t))
        return ec;

    url_view u(0, s.data());

    // scheme
    u.apply(t.scheme_part);

    // authority
    if(t.hier_part.has_authority)
        u.apply(t.hier_part.authority);

    // path
    u.apply(t.hier_part.path);

    // query
    u.apply(t.query_part);

    return u;
}

result<url_view>
parse_uri(
    string_view s)
{
    if(s.size() > url_view::max_size())
        detail::throw_length_error(
            "url_view::max_size exceeded",
            BOOST_CURRENT_LOCATION);

    error_code ec;
    uri_rule t;
    if(! grammar::parse_string(s, ec, t))
        return ec;

    url_view u(0, s.data());

    // scheme
    u.apply(t.scheme_part);

    // authority
    if(t.hier_part.has_authority)
        u.apply(t.hier_part.authority);

    // path
    u.apply(t.hier_part.path);

    // query
    u.apply(t.query_part);

    // fragment
    u.apply(t.fragment_part);

    return u;
}

result<url_view>
parse_relative_ref(
    string_view s)
{
    if(s.size() > url_view::max_size())
        detail::throw_length_error(
            "url_view::max_size exceeded",
            BOOST_CURRENT_LOCATION);

    error_code ec;
    relative_ref_rule t;
    if(! grammar::parse_string(
            s, ec, t))
        return ec;

    url_view u(0, s.data());

    // authority
    if(t.relative_part.has_authority)
        u.apply(t.relative_part.authority);

    // path
    u.apply(t.relative_part.path);

    // query
    u.apply(t.query_part);

    // fragment
    u.apply(t.fragment_part);

    return u;
}

result<url_view>
parse_uri_reference(
    string_view s)
{
    if(s.size() > url_view::max_size())
        detail::throw_length_error(
            "url_view::max_size exceeded",
            BOOST_CURRENT_LOCATION);

    error_code ec;
    uri_reference_rule t;
    if(! grammar::parse_string(s, ec, t))
        return ec;

    url_view u(0, s.data());

    // scheme
    u.apply(t.scheme_part);

    // authority
    if(t.has_authority)
        u.apply(t.authority);

    // path
    u.apply(t.path);

    // query
    u.apply(t.query_part);

    // fragment
    u.apply(t.fragment_part);

    return u;
}

//------------------------------------------------

std::ostream&
operator<<(
    std::ostream& os,
    url_view const& u)
{
    os << u.string();
    return os;
}

} // urls
} // boost

//----------------------------------------------------------
//
// std::hash specialization
//
//----------------------------------------------------------

std::size_t
std::hash<::boost::urls::url_view>::operator()(
    ::boost::urls::url_view const& u) const noexcept
{
    ::boost::urls::detail::fnv_1a hasher(salt_);
    using parts = ::boost::urls::detail::parts_base;
    ::boost::urls::detail::ci_digest(
        u.get(parts::id_scheme), hasher);
    ::boost::urls::detail::digest_encoded(
        u.get(parts::id_user), hasher);
    ::boost::urls::detail::digest_encoded(
        u.get(parts::id_pass), hasher);
    ::boost::urls::detail::ci_digest_encoded(
        u.get(parts::id_host), hasher);
    hasher.put(u.get(parts::id_port));
    ::boost::urls::detail::normalized_path_digest(
        u.get(parts::id_path),
        u.is_path_absolute(),
        hasher);
    ::boost::urls::detail::digest_encoded(
        u.get(parts::id_query), hasher);
    ::boost::urls::detail::digest_encoded(
        u.get(parts::id_frag), hasher);
    return hasher.digest();
}


#endif

//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_GRAMMAR_IMPL_ERROR_IPP
#define BOOST_URL_GRAMMAR_IMPL_ERROR_IPP


namespace boost {
namespace urls {
namespace grammar {

error_code
make_error_code(
    error e) noexcept
{
    struct codes : error_category
    {
        const char*
        name() const noexcept override
        {
            return "boost.url.bnf";
        }

        std::string
        message(int ev) const override
        {
            switch(static_cast<error>(ev))
            {
            default:
case error::success: return "success";
case error::end: return "end";
case error::incomplete: return "incomplete";
case error::leftover: return "leftover";
case error::overflow: return "overflow";
case error::syntax: return "syntax";
            }
        }

        error_condition
        default_error_condition(
            int ev) const noexcept override
        {
            switch(static_cast<error>(ev))
            {
case error::overflow:
case error::syntax:
                return condition::fatal;
            default:
                return {ev, *this};
            }
        }
    };

    static codes const cat{};
    return error_code{static_cast<
        std::underlying_type<error>::type>(e), cat};
}

//------------------------------------------------

error_condition
make_error_condition(
    condition c) noexcept
{
    struct codes : error_category
    {
        const char*
        name() const noexcept override
        {
            return "boost.url.grammar";
        }

        std::string
        message(int cv) const override
        {
            switch(static_cast<condition>(cv))
            {
            default:
            case condition::fatal:
                return "fatal condition";
            }
        }
    };
    static codes const cat{};
    return error_condition{static_cast<
        std::underlying_type<condition>::type>(c), cat};
}

} // grammar
} // urls
} // boost

#endif

//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_ABSOLUTE_URI_RULE_IPP
#define BOOST_URL_IMPL_ABSOLUTE_URI_RULE_IPP


namespace boost {
namespace urls {

void
absolute_uri_rule::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    absolute_uri_rule& t) noexcept
{
    grammar::parse(
        it, end, ec,
        t.scheme_part,
        t.hier_part,
        t.query_part);
}

} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_AUTHORITY_RULE_IPP
#define BOOST_URL_IMPL_AUTHORITY_RULE_IPP


namespace boost {
namespace urls {

void
authority_rule::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    authority_rule& t) noexcept
{
    auto start = it;

    // [ userinfo "@" ]
    if(grammar::parse(it, end, ec,
        t.userinfo, '@'))
    {
        t.has_userinfo = true;
        start = it;
    }
    else
    {
        // rewind
        t.has_userinfo = false;
        it = start;
    }

    // host
    if(! grammar::parse(
            it, end, ec, t.host))
        return;

    // [ ":" port ]
    if(! grammar::parse(it, end, ec,
            t.port))
        return;
}

} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_FRAGMENT_RULE_IPP
#define BOOST_URL_IMPL_FRAGMENT_RULE_IPP


namespace boost {
namespace urls {

void
fragment_rule::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    fragment_rule& t) noexcept
{
    pct_encoded_rule<
        fragment_chars_t> t0;
    grammar::parse(it, end, ec, t0);
    t.s = t0.s;
}

void
fragment_part_rule::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    fragment_part_rule& t) noexcept
{
    if(it == end)
    {
        t.has_fragment = false;
        return;
    }
    if(*it != '#')
    {
        t.has_fragment = false;
        return;
    }
    ++it;
    fragment_rule t0;
    if(! grammar::parse(it, end, ec, t0))
        return;
    t.has_fragment = true;
    t.fragment = t0.s;
}

} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_HIER_PART_RULE_IPP
#define BOOST_URL_IMPL_HIER_PART_RULE_IPP


namespace boost {
namespace urls {

void
hier_part_rule::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    hier_part_rule& t) noexcept
{
    if(it == end)
    {
        // path-empty
        t.path = {};
        t.has_authority = false;
        ec = {};
        return;
    }
    if(it[0] != '/')
    {
        // path-rootless
        path_rootless_rule t0;
        if(! grammar::parse(
                it, end, ec, t0))
            return;
        t.path.path = t0.str;
        t.path.count = t0.count;
        t.has_authority = false;
        return;
    }
    if( end - it == 1 ||
        it[1] != '/')
    {
        // path-absolute
        path_absolute_rule t0;
        if(! grammar::parse(
                it, end, ec, t0))
            return;
        t.path.path = t0.str;
        t.path.count = t0.count;
        t.has_authority = false;
        return;
    }
    // "//" authority path-abempty
    it += 2;
    // authority
    if(! grammar::parse(
        it, end, ec,
            t.authority))
        return;
    // path-abempty
    path_abempty_rule t0;
    if(! grammar::parse(
            it, end, ec, t0))
        return;

    t.path.path = t0.str;
    t.path.count = t0.count;
    t.has_authority = true;
}

} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_HOST_RULE_IPP
#define BOOST_URL_IMPL_HOST_RULE_IPP


namespace boost {
namespace urls {

void
host_rule::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    host_rule& t) noexcept
{
    if(it == end)
    {
        t.host_type =
            urls::host_type::name;
        t.name = {};
        t.host_part = {};
        return;
    }
    auto const start = it;
    if(*it == '[')
    {
        // IP-literal
        ip_literal_rule v;
        if(! grammar::parse(
                it, end, ec, v))
            return;
        if(v.is_ipv6)
        {
            // IPv6address
            t.ipv6 = v.ipv6;
            t.host_type =
                urls::host_type::ipv6;
            t.host_part = string_view(
                start, it - start);
            return;
        }

        // IPvFuture
        t.ipvfuture = v.ipvfuture;
        t.host_type =
            urls::host_type::ipvfuture;
        t.host_part = string_view(
            start, it - start);
        return;
    }
    // IPv4address
    {
        if(grammar::parse(
            it, end, ec, t.ipv4))
        {
            t.host_type =
                urls::host_type::ipv4;
            t.host_part = string_view(
                start, it - start);
            return;
        }
        // rewind
        it = start;
    }
    // reg-name
    reg_name_rule t0;
    if(! grammar::parse(
        it, end, ec, t0))
    {
        // bad reg-name
        return;
    }
    t.name = t0.v;

    t.host_type =
        urls::host_type::name;
    t.host_part = string_view(
        start, it - start);
}

} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_IP_LITERAL_RULE_IPP
#define BOOST_URL_IMPL_IP_LITERAL_RULE_IPP


namespace boost {
namespace urls {

void
ip_literal_rule::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    ip_literal_rule& t) noexcept
{
    // '['
    if(! grammar::parse(
            it, end, ec, '['))
        return;
    if(it == end)
    {
        // expected address
        ec = grammar::error::incomplete;
        return;
    }
    if(*it != 'v')
    {
        // IPv6address
        if(! grammar::parse(
                it, end, ec,
                t.ipv6, ']'))
            return;
        t.is_ipv6 = true;
    }
    else
    {
        // IPvFuture
        ipv_future_rule p;
        if(! grammar::parse(
                it, end, ec,
                p, ']'))
            return;
        t.is_ipv6 = false;
        t.ipvfuture = p.str;
    }
}

} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_IPV_FUTURE_RULE_IPP
#define BOOST_URL_IMPL_IPV_FUTURE_RULE_IPP


namespace boost {
namespace urls {

void
ipv_future_rule::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    ipv_future_rule& t) noexcept
{
    auto const start = it;
    struct minor_chars_t
        : grammar::lut_chars
    {
        constexpr
        minor_chars_t() noexcept
            : grammar::lut_chars(
                unreserved_chars +
                subdelim_chars + ':')
        {
        }
    };
    grammar::token<
        grammar::hexdig_chars_t> major;
    grammar::token<
        minor_chars_t> minor;
    if(! grammar::parse(
        it, end, ec,
            'v',
            major,
            '.',
            minor))
        return;
    t.major = *major;
    t.minor = *minor;
    if(t.major.empty())
    {
        // can't be empty
        ec = error::bad_empty_element;
        return;
    }
    if(t.minor.empty())
    {
        // can't be empty
        ec = error::bad_empty_element;
        return;
    }
    t.str = string_view(
        start, it - start);
}

} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_IMPL_PATHS_RULE_IPP
#define BOOST_URL_RFC_IMPL_PATHS_RULE_IPP


namespace boost {
namespace urls {

void
segment_rule::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    segment_rule const& t) noexcept
{
    pct_encoded_rule<pchars_t> t0;
    if(! grammar::parse(
        it, end, ec, t0))
        return;
    t.v = t0.s;
}

//------------------------------------------------

void
segment_nz_rule::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    segment_nz_rule const& t) noexcept
{
    pct_encoded_rule<pchars_t> t0;
    if(! grammar::parse(
        it, end, ec, t0))
        return;
    t.v = t0.s;
    if(t.v.str.empty())
    {
        // can't be empty
        ec = grammar::error::syntax;
        return;
    }
}

//------------------------------------------------

void
segment_nz_nc_rule::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    segment_nz_nc_rule const& t) noexcept
{
    struct seg_chars
        : grammar::lut_chars
    {
        constexpr
        seg_chars() noexcept
            : lut_chars(
                pchars - ':')
        {
        }
    };
    pct_encoded_rule<seg_chars> t0;
    if(! grammar::parse(
        it, end, ec, t0))
        return;
    if(t0.s.str.empty())
    {
        // can't be empty
        ec = error::empty_path_segment;
        return;
    }
    t.v = t0.s;
}

//------------------------------------------------

bool
path_abempty_rule::
begin(
    char const*& it,
    char const* const end,
    error_code& ec,
    pct_encoded_str& t) noexcept
{
    return increment(
        it, end, ec, t);
}

bool
path_abempty_rule::
increment(
    char const*& it,
    char const* const end,
    error_code& ec,
    pct_encoded_str& t) noexcept
{
    auto const start = it;
    if(grammar::parse(
        it, end, ec,
        '/', segment_rule{t}))
        return true;
    ec = BOOST_URL_ERR(
        grammar::error::end);
    it = start;
    return false;
}

//------------------------------------------------

bool
path_absolute_rule::
begin(
    char const*& it,
    char const* const end,
    error_code& ec,
    pct_encoded_str& t) noexcept
{
    if(it == end)
    {
        // expected '/'
        ec = BOOST_URL_ERR(
            error::missing_path_segment);
        return false;
    }
    if(*it != '/')
    {
        // expected '/'
        ec = BOOST_URL_ERR(
            error::missing_path_separator);
        return false;
    }
    ++it;
    if(it == end)
        return true;
    if(*it == '/')
    {
        // can't begin with "//"
        ec = BOOST_URL_ERR(
            error::empty_path_segment);
        return false;
    }
    return grammar::parse(
        it, end, ec,
        segment_rule{t});
}

bool
path_absolute_rule::
increment(
    char const*& it,
    char const* const end,
    error_code& ec,
    pct_encoded_str& t) noexcept
{
    auto const start = it;
    if(grammar::parse(
        it, end, ec,
        '/', segment_rule{t}))
        return true;
    ec = BOOST_URL_ERR(
        grammar::error::end);
    it = start;
    return false;
}

//------------------------------------------------

bool
path_noscheme_rule::
begin(
    char const*& it,
    char const* const end,
    error_code& ec,
    pct_encoded_str& t) noexcept
{
    if(grammar::parse(
        it, end, ec,
        segment_nz_nc_rule{t}))
        return true;
    // bad segment-nz-nc
    ec = error::bad_schemeless_path_segment;
    return false;
}

bool
path_noscheme_rule::
increment(
    char const*& it,
    char const* const end,
    error_code& ec,
    pct_encoded_str& t) noexcept
{
    auto const start = it;
    if(grammar::parse(
        it, end, ec,
        '/', segment_rule{t}))
        return true;
    ec = grammar::error::end;
    it = start;
    return false;
}


//------------------------------------------------

bool
path_rootless_rule::
begin(
    char const*& it,
    char const* const end,
    error_code& ec,
    pct_encoded_str& t) noexcept
{
    return grammar::parse(
        it, end, ec,
        segment_nz_rule{t});
}

bool
path_rootless_rule::
increment(
    char const*& it,
    char const* const end,
    error_code& ec,
    pct_encoded_str& t) noexcept
{
    auto const start = it;
    if(grammar::parse(
        it, end, ec,
        '/', segment_rule{t}))
        return true;
    ec = grammar::error::end;
    it = start;
    return false;
}

} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_PORT_RULE_IPP
#define BOOST_URL_IMPL_PORT_RULE_IPP

#include <boost/static_assert.hpp>
#include <type_traits>

namespace boost {
namespace urls {

void
port_rule::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    port_rule& t) noexcept
{
    port_rule::number_type u = 0;
    auto const start = it;
    while(it != end)
    {
        if(! grammar::digit_chars(*it))
            break;
        auto u0 = u;
        u = 10 * u + *it - '0';
        if(u < u0)
        {
            // overflow
            it = grammar::find_if_not(
                it, end, grammar::digit_chars);
            t.str = string_view(
                start, it - start);
            t.has_number = false;
            ec = {};
            return;
        }
        ++it;
    }
    t.str = string_view(
        start, it - start);
    if(! t.str.empty())
    {
        t.has_number = true;
        t.number = u;
    }
    else
    {
        t.has_number = false;
    }
}

void
port_part_rule::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    port_part_rule& t) noexcept
{
    if( it == end ||
        *it != ':')
    {
        ec = {};
        t.has_port = false;
        return;
    }
    ++it;
    port_rule t0;
    if(! grammar::parse(
            it, end, ec, t0))
        return;
    t.has_port = true;
    t.port = t0.str;
    t.has_number = t0.has_number;
    t.port_number = t0.number;
}

} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_QUERY_RULE_IPP
#define BOOST_URL_IMPL_QUERY_RULE_IPP


namespace boost {
namespace urls {

struct query_rule::key_chars
    : grammar::lut_chars
{
    constexpr
    key_chars() noexcept
        : grammar::lut_chars(
            pchars
            + '/' + '?' + '[' + ']'
            - '&' - '=')
    {
    }
};

struct query_rule::value_chars
    : grammar::lut_chars
{
    constexpr
    value_chars() noexcept
        : grammar::lut_chars(
            pchars
            + '/' + '?'
            - '&')
    {
    }
};

//------------------------------------------------

bool
query_rule::
begin(
    char const*& it,
    char const* const end,
    error_code& ec,
    query_param_view& t) noexcept
{
    pct_encoded_rule<
        query_rule::key_chars> t0;
    pct_encoded_rule<
        query_rule::value_chars> t1;

    // key
    if(! grammar::parse(it, end, ec, t0))
        return false;
    t.key = t0.s.str;

    // "="
    t.has_value = grammar::parse(
        it, end, ec, '=');
    if(t.has_value)
    {
        // value
        if(! grammar::parse(
            it, end, ec, t1))
            return false;
        t.value = t1.s.str;
    }
    else
    {
        // key with no value
        ec = {};
    }
    return true;
}

bool
query_rule::
increment(
    char const*& it,
    char const* const end,
    error_code& ec,
    query_param_view& t) noexcept
{
    pct_encoded_rule<
        query_rule::key_chars> t0;
    pct_encoded_rule<
        query_rule::value_chars> t1;

    if(! grammar::parse(
        it, end, ec, '&'))
    {
        // end of list
        ec = grammar::error::end;
        return false;
    }

    // key
    if(! grammar::parse(
        it, end, ec, t0))
        return false;
    t.key = t0.s.str;

    // "="
    t.has_value = grammar::parse(
        it, end, ec, '=');
    if(t.has_value)
    {
        // value
        if(! grammar::parse(
                it, end, ec, t1))
            return false;
        t.value = t1.s.str;
    }
    else
    {
        // key with no value
        ec = {};
    }

    return true;
}

//------------------------------------------------

void
query_part_rule::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    query_part_rule& t) noexcept
{
    auto start = it;
    if( it == end ||
        *it != '?')
    {
        ec = {};
        t.has_query = false;
        return;
    }
    ++it;
    if(! grammar::parse(
        it, end, ec, t.query))
        return;
    t.has_query = true;
    t.query_part = string_view(
        start, it - start);
}

} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_REG_NAME_RULE_IPP
#define BOOST_URL_IMPL_REG_NAME_RULE_IPP


namespace boost {
namespace urls {

/*  VFALCO In theory we should enforce these
    additional requirements from errata 4942:

    Such a name consists of a sequence of domain
    labels separated by ".", each domain label
    starting and ending with an alphanumeric character
    and possibly also containing "-" characters.  The
    rightmost domain label of a fully qualified domain
    name in DNS may be followed by a single "." and
    should be if it is necessary to distinguish between
    the complete domain name and some local domain.
*/
void
reg_name_rule::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    reg_name_rule& t) noexcept
{
    struct reg_name_chars
        : grammar::lut_chars
    {
        constexpr
        reg_name_chars() noexcept
            : lut_chars(
                unreserved_chars
                + '-' + '.')
        {
        }
    };

    pct_encoded_rule<
        reg_name_chars> t0;
    grammar::parse(
        it, end, ec, t0);
    t.v = t0.s;
}

} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_RELATIVE_PART_RULE_IPP
#define BOOST_URL_IMPL_RELATIVE_PART_RULE_IPP


namespace boost {
namespace urls {

void
relative_part_rule::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    relative_part_rule& t) noexcept
{
    if(it == end)
    {
        // path-empty
        ec = {};
        t.path = {};
        t.has_authority = false;
        return;
    }
    if(it[0] != '/')
    {
        path_noscheme_rule t0;
        if(grammar::parse(
            it, end, ec, t0))
        {
            // path-noscheme
            t.path.path = t0.str;
            t.path.count = t0.count;
            t.has_authority = false;
            return;
        }
        // path-empty
        ec = {};
        t.path = {};
        t.has_authority = false;
        return;
    }
    if( end - it == 1 ||
        it[1] != '/')
    {
        // path-absolute
        path_absolute_rule t0;
        if(! grammar::parse(
                it, end, ec, t0))
            return;
        t.path.path = t0.str;
        t.path.count = t0.count;
        t.has_authority = false;
        return;
    }
    // "//" authority path-abempty
    it += 2;
    if(! grammar::parse(
        it, end, ec, t.authority))
        return;
    path_abempty_rule t0;
    if(! grammar::parse(
            it, end, ec, t0))
        return;
    t.path.path = t0.str;
    t.path.count = t0.count;
    t.has_authority = true;
}

} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_RELATIVE_REF_RULE_IPP
#define BOOST_URL_IMPL_RELATIVE_REF_RULE_IPP


namespace boost {
namespace urls {

void
relative_ref_rule::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    relative_ref_rule& t) noexcept
{
    // relative-part
    if(! grammar::parse(
        it, end, ec,
            t.relative_part))
        return;

    // [ "?" query ]
    if(! grammar::parse(
        it, end, ec,
            t.query_part))
        return;

    // [ "#" fragment ]
    if(! grammar::parse(
        it, end, ec,
            t.fragment_part))
        return;
}

} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_SCHEME_RULE_IPP
#define BOOST_URL_IMPL_SCHEME_RULE_IPP


namespace boost {
namespace urls {

void
scheme_rule::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    scheme_rule& t) noexcept
{
    auto const start = it;
    if(it == end)
    {
        // expected alpha
        ec = grammar::error::incomplete;
        return;
    }
    if(! grammar::alpha_chars(*it))
    {
        // expected alpha
        ec = error::bad_alpha;
        return;
    }
    static
    constexpr
    grammar::lut_chars scheme_chars(
        "0123456789" "+-."
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz");
    it = grammar::find_if_not(
        it + 1, end, scheme_chars);
    t.scheme = string_view(
        start, it - start);
    t.scheme_id = string_to_scheme(
        t.scheme);
}

void
scheme_part_rule::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    scheme_part_rule& t) noexcept
{
    auto const start = it;
    scheme_rule t0;
    if(! grammar::parse(
        it, end, ec, t0, ':'))
        return;
    t.scheme = t0.scheme;
    t.scheme_id = t0.scheme_id;
    t.scheme_part = string_view(
        start, it - start);
}

} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_URI_RULE_IPP
#define BOOST_URL_IMPL_URI_RULE_IPP


namespace boost {
namespace urls {

void
uri_rule::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    uri_rule& t) noexcept
{
    // scheme ":"
    if(! grammar::parse(
        it, end, ec,
            t.scheme_part))
        return;

    // hier-part
    if(! grammar::parse(
        it, end, ec,
            t.hier_part))
        return;

    // [ "?" query ]
    if(! grammar::parse(
        it, end, ec,
            t.query_part))
        return;

    // [ "#" fragment ]
    if(! grammar::parse(
        it, end, ec,
            t.fragment_part))
        return;
}

} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_URI_REFERENCE_RULE_IPP
#define BOOST_URL_IMPL_URI_REFERENCE_RULE_IPP


namespace boost {
namespace urls {

void
uri_reference_rule::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    uri_reference_rule& t) noexcept
{
    auto const start = it;

    // scheme ":"
    if(! grammar::parse(
        it, end, ec,
            t.scheme_part))
    {
        // rewind
        it = start;

        // relative-ref
        relative_part_rule t0;
        if(! grammar::parse(
            it, end, ec,t0))
            return;

        t.has_authority =
            t0.has_authority;
        t.authority = t0.authority;
        t.path = t0.path;
    }
    else
    {
        // hier-part
        hier_part_rule t0;
        if(! grammar::parse(
            it, end, ec, t0))
            return;

        t.has_authority =
            t0.has_authority;
        t.authority = t0.authority;
        t.path = t0.path;
    }

    // [ "?" query ]
    if(! grammar::parse(
        it, end, ec,
            t.query_part))
        return;

    // [ "#" fragment ]
    if(! grammar::parse(
        it, end, ec,
            t.fragment_part))
        return;
}

} // urls
} // boost

#endif
//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_USERINFO_RULE_IPP
#define BOOST_URL_IMPL_USERINFO_RULE_IPP


namespace boost {
namespace urls {

void
userinfo_rule::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    userinfo_rule& t) noexcept
{
    struct uchars
        : grammar::lut_chars
    {
        constexpr
        uchars() noexcept
            : lut_chars(
                unreserved_chars +
                subdelim_chars)
        {
        }
    };

    struct pwchars
        : grammar::lut_chars
    {
        constexpr
        pwchars() noexcept
            : lut_chars(
                unreserved_chars +
                subdelim_chars + ':')
        {
        }
    };

    pct_encoded_rule<uchars> t0;
    pct_encoded_rule<pwchars> t1;

    if(! grammar::parse(
        it, end, ec, t0))
        return;
    t.user = t0.s;
    if( it == end ||
        *it != ':')
    {
        t.has_password = false;
        t.password = {};
        return;
    }
    ++it;
    if(! grammar::parse(
        it, end, ec, t1))
        return;
    t.has_password = true;
    t.password = t1.s;
}

} // urls
} // boost

#endif

#endif
