#pragma once

#include <type_traits>

#include "is_copy_constructible.h"
#include "utility.h"

//<IsConstructible, is_reference, T, Args...>
template <bool, bool, typename T, typename... Args>
struct LibCppIsNoThrowConstructible;

template <typename To>
static void ImplicitCast(To);

// LibCppIsNoThrowConstructible - partial specializations
template <typename T, typename... Args>
struct LibCppIsNoThrowConstructible<true, false, T, Args...> {
    using value = std::integral_constant<bool, noexcept(T(Declval<Args>()...))>;
};

template <typename To, typename From>
struct LibCppIsNoThrowConstructible<true, true, To, From> {
    using value = std::integral_constant<bool, noexcept(ImplicitCast<To>(Declval<From>()))>;
};

template <bool IsReference, typename T, typename... Args>
struct LibCppIsNoThrowConstructible<false, IsReference, T, Args...> {
    using value = std::false_type;
};
// LibCppIsNoThrowConstructible - partial specializations

template <typename T, typename... Args>
struct IsNoThrowConstructible
    : LibCppIsNoThrowConstructible<IsConstructible<T, Args...>::value, std::is_reference_v<T>, T,
                                   Args...>::value {};

template <typename T, std::size_t N>
struct IsNoThrowConstructible<T[N]>
    : LibCppIsNoThrowConstructible<IsConstructible<T>::value, std::is_reference_v<T>, T>::value {};

template <typename T>
struct IsNoThrowMoveConstructible : IsNoThrowConstructible<T, T&&> {};