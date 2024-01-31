#ifndef __SCC_STDCPP_UTILITY_HPP__
#define __SCC_STDCPP_UTILITY_HPP__

#include <type_traits>
#include <utility>
#include <iterator>

namespace stdcpp
{
    namespace v1
    {
        template <class Enum>
        constexpr std::underlying_type_t<Enum> to_underlying(Enum e) noexcept
        {
            return static_cast<std::underlying_type_t<Enum>>(e);
        }

        template <class T>
        constexpr auto ssize(const T &t) noexcept(noexcept(static_cast<std::make_signed_t<decltype(t.size())>>(t.size())))
        {
            return static_cast<std::make_signed_t<decltype(t.size())>>(t.size());
        }

        template <class T>
        constexpr auto ssize(T &&t)
        {
            return static_cast<std::make_signed_t<decltype(std::forward<T>(t).size())>>(std::forward<T>(t).size());
        }

        template <class T>
        constexpr auto cdata(const T &t) noexcept(noexcept(t.data()))
        {
            return t.data();
        }

        template <class T, std::size_t N>
        constexpr auto cdata(const T (&t)[N]) noexcept
        {
            return t;
        }

        template <class T>
        constexpr std::add_const_t<T> &as_const(T &t) noexcept
        {
            return t;
        }

        namespace common_reference_detail
        {
            // Fallback for the case when common_reference is not defined
            struct no_common_reference
            {
            };

            // Alias template for adding cv and reference qualifiers
            template <typename T, typename U>
            using with_qualifiers = typename std::conditional<
                std::is_lvalue_reference<T>::value,
                typename std::add_lvalue_reference<
                    typename std::conditional<std::is_const<T>::value, const U, U>::type>::type,
                typename std::conditional<
                    std::is_rvalue_reference<T>::value,
                    typename std::add_rvalue_reference<U>::type,
                    U>::type>::type;

            // Helper to get common_reference for two types
            template <typename T1, typename T2, typename = void>
            struct binary_common_reference
            {
                using type = no_common_reference;
            };

            template <typename T1, typename T2>
            struct binary_common_reference<T1, T2, std::void_t<decltype(false ? std::declval<T1>() : std::declval<T2>()), decltype(std::declval<with_qualifiers<T1, T2>>()), decltype(std::declval<with_qualifiers<T2, T1>>())>>
            {
                using type = decltype(false ? std::declval<T1>() : std::declval<T2>());
            };

            // Helper for folding over common_reference
            template <typename...>
            struct multi_common_reference
            {
            };

            template <typename T1>
            struct multi_common_reference<T1>
            {
                using type = T1;
            };

            template <typename T1, typename T2>
            struct multi_common_reference<T1, T2>
            {
                using type = typename binary_common_reference<T1, T2>::type;
            };

            template <typename T1, typename T2, typename... R>
            struct multi_common_reference<T1, T2, R...>
            {
                using type = typename multi_common_reference<
                    typename binary_common_reference<T1, T2>::type,
                    R...>::type;
            };
        } // namespace common_reference_detail

        // is same_v
        template <class T, class U>
        constexpr bool is_same_v = std::is_same<T, U>::value;

        template <typename... Ts>
        struct common_reference
        {
        };

        template <>
        struct common_reference<>
        {
        };

        template <typename... Ts>
        using common_reference_t = typename common_reference<Ts...>::type;

        template <typename T0>
        struct common_reference<T0>
        {
            using type = T0;
        };

        template <typename T1, typename T2>
        struct common_reference<T1, T2>
        {
            using type = typename common_reference_detail::binary_common_reference<T1, T2>::type;
        };

        template <typename T1, typename T2, typename... R>
        struct common_reference<T1, T2, R...>
        {
            using type = typename common_reference_detail::multi_common_reference<T1, T2, R...>::type;
        };

        template <class T, class U, template <class> class TQual, template <class> class UQual>
        struct basic_common_reference
        {
        };

        template <class T>
        constexpr auto iter_move(T &&t) noexcept(noexcept(static_cast<decltype(std::move(*std::declval<T>())) &&>(*std::declval<T>())))
            -> decltype(std::move(*std::declval<T>()))
        {
            return std::move(*std::forward<T>(t));
        }

        template <class T>
        using iter_value_t = typename std::iterator_traits<T>::value_type;

        template <class T>
        using iter_reference_t = decltype(*std::declval<T &>());

        template <class T>
        using iter_rvalue_reference_t = decltype(iter_move(std::declval<T &>()));

        template <class T>
        using iter_difference_t = typename std::iterator_traits<T>::difference_type;

        template <class T>
        using iter_const_value_t = typename std::iterator_traits<T>::value_type;

        template <typename T>
        using iter_const_reference_t = common_reference_t<iter_reference_t<T> const &, iter_value_t<T> const &>;

        template <class T>
        using iter_common_reference_t = typename common_reference<iter_reference_t<T>, iter_value_t<T> &>::type;
    }

    template <class Enum>
    constexpr auto to_underlying(Enum e) noexcept
    {
        return v1::to_underlying(e);
    }

    template <class T>
    constexpr auto ssize(const T &t) noexcept(noexcept(static_cast<std::make_signed_t<decltype(t.size())>>(t.size())))
    {
        return v1::ssize(t);
    }

    template <class T>
    constexpr auto ssize(T &&t)
    {
        return v1::ssize(std::forward<T>(t));
    }

    template <class T>
    constexpr auto cdata(const T &t) noexcept(noexcept(t.data()))
    {
        return v1::cdata(t);
    }

    template <class T, std::size_t N>
    constexpr auto cdata(const T (&t)[N]) noexcept
    {
        return v1::cdata(t);
    }

    template <class T>
    using common_reference_t = typename v1::common_reference<T>::type;

    template <class T>
    using iter_value_t = typename v1::iter_value_t<T>;

    template <class T>
    using iter_reference_t = typename v1::iter_reference_t<T>;

    template <class T>
    using iter_rvalue_reference_t = typename v1::iter_rvalue_reference_t<T>;

    template <class T>
    using iter_difference_t = typename v1::iter_difference_t<T>;

    template <class T>
    using iter_const_value_t = typename v1::iter_const_value_t<T>;

    template <class T>
    using iter_const_reference_t = typename v1::iter_const_reference_t<T>;

    template <class T>
    using iter_common_reference_t = typename v1::iter_common_reference_t<T>;

    template <class T, class U>
    constexpr auto &is_same_v = v1::is_same_v<T, U>;

    template <class T>
    constexpr auto iter_move(T &&t) noexcept(noexcept(static_cast<decltype(std::move(*std::declval<T>())) &&>(*std::declval<T>())))
        -> decltype(std::move(*std::declval<T>()))
    {
        return v1::iter_move(std::forward<T>(t));
    }

    template <class T>
    constexpr std::add_const_t<T> &as_const(T &t) noexcept
    {
        return v1::as_const(t);
    }
}

#endif // __SCC_STDCPP_UTILITY_HPP__