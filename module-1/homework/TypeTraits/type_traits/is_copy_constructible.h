#pragma once

#include <type_traits>
#include <utility>

#include "utility.h"

template <typename T, typename... Args>
struct LibCppIsConstructible;

template <typename Derived, typename Base>
struct IsInvalidBaseToDerivedCast {
    using raw_base = Uncvref<Base>;
    using raw_derived = Uncvref<Derived>;
    static constexpr bool kValue = std::conjunction_v<
        std::negation<std::is_same<raw_base, raw_derived>>, std::is_base_of<raw_base, raw_derived>,
        std::negation<typename LibCppIsConstructible<raw_derived, raw_base>::value>>;
};

template <typename To, typename From>
struct IsInvalidLvalueToRvalueCast : std::false_type {
    static constexpr bool kValue = false;
};

template <typename RefTo, typename RefFrom>
struct IsInvalidLvalueToRvalueCast<RefTo&&, RefFrom&> {
    using raw_ref_to = Uncvref<RefTo>;
    using raw_ref_from = Uncvref<RefFrom>;
    static constexpr bool kValue = std::disjunction_v<std::is_same<raw_ref_to, raw_ref_from>,
                                   std::is_base_of<raw_ref_to, raw_ref_from>>;
};

template <typename T>
add_rvalue_reference_t<T> Declval() noexcept;

struct IsConstructibleHelper {
    template <typename T, typename... Args, typename = decltype(::new T(Declval<Args>()...))>
    static std::true_type Nary(int);

    template <typename, typename>
    static std::false_type Nary(...);

    template <typename T, typename Arg, typename = decltype(::new T(Declval<Arg>()))>
    static std::is_destructible<T> Unary(int);

    template <typename, typename>
    static std::false_type Unary(...);

    template <typename To>
    static void ImplicitCast(To);

    template <typename To, typename From, typename = decltype(ImplicitCast<To>(Declval<From>()))>
    static std::true_type TypeCast(int);

    template <typename To, typename From, typename = decltype(static_cast<To>(Declval<From>()))>
    static std::integral_constant<bool, !IsInvalidBaseToDerivedCast<To, From>::kValue &&
                                            !IsInvalidLvalueToRvalueCast<To, From>::kValue>
        TypeCast(int64_t);

    template <typename, typename>
    static std::false_type TypeCast(...);
};

// LibCppIsConstructible - partial specializations
template <typename T, typename... Args>
struct LibCppIsConstructible {
    using value = decltype(IsConstructibleHelper::Nary<T, Args...>(0));
};

template <typename T, typename Arg>
struct LibCppIsConstructible<T, Arg> {
    using value = decltype(IsConstructibleHelper::Unary<T, Arg>(0));
};

template <typename T, typename Arg>
struct LibCppIsConstructible<T&, Arg> {
    using value = decltype(IsConstructibleHelper::TypeCast<T&, Arg>(0));
};

template <typename T, typename Arg>
struct LibCppIsConstructible<T&&, Arg> {
    using value = decltype(IsConstructibleHelper::TypeCast<T&&, Arg>(0));
};
// LibCppIsConstructible - partial specializations

template <typename T, typename... Args>
struct IsConstructible : LibCppIsConstructible<T, Args...>::value {};

template <typename T>
struct IsCopyConstructible : LibCppIsConstructible<T, const T&>::value {};