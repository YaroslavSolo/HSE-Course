#include <cstdlib>
#include <utility>

#pragma once

namespace task {

template <typename... Types>
class Variant;

// -------Union-------
template <size_t Idx, typename... Tail>
union StoreUnion;

template <size_t Idx>
union StoreUnion<Idx> {};

template <size_t Idx, typename Head, typename... Tail>
union StoreUnion<Idx, Head, Tail...> {
public:
    Head head;

    StoreUnion<Idx + 1, Tail...> tail;

    StoreUnion() : tail() {
        if (!std::is_trivially_constructible_v<Head>) {
            new (&head) Head();
        }
    }

    ~StoreUnion() {
        if (!std::is_trivially_destructible_v<Head>) {
            head.~Head();
        }
    }
};
// -------Union-------

struct UnionHelper {
    template <typename U>
    static constexpr auto&& Get(U&& v, std::in_place_index_t<0>) {
        return std::forward<U>(v).head;
    }

    template <typename U, size_t Idx>
    static constexpr auto&& Get(U&& v, std::in_place_index_t<Idx>) {
        return Get(std::forward<U>(v).tail, std::in_place_index<Idx - 1>);
    }

    template <std::size_t Idx, typename U, std::size_t UnIdx, typename Head, typename... Tail>
    static void Set(U&& value, std::in_place_index_t<0>, StoreUnion<UnIdx, Head, Tail...>& u) {
        u.head = value;
    }

    template <std::size_t Idx, typename U, std::size_t UnIdx, typename Head, typename... Tail>
    static void Set(U&& value, std::in_place_index_t<Idx>, StoreUnion<UnIdx, Head, Tail...>& u) {
        UnionHelper::Set<Idx - 1>(std::forward<U>(value), std::in_place_index<Idx - 1>, u.tail);
    }
};

// -------TypeList-------
template <typename...>
struct TypeList {};

template <size_t Idx, typename TList>
struct TypeAt {};

template <typename Head, typename... Tail>
struct TypeAt<0, TypeList<Head, Tail...>> {
    typedef Head target_type;
};

template <size_t Idx, typename Head, typename... Tail>
struct TypeAt<Idx, TypeList<Head, Tail...>> {
    typedef typename TypeAt<Idx - 1, TypeList<Tail...>>::target_type target_type;
};

template <size_t Idx, typename T>
struct VariantAlternative;

template <size_t Idx, typename T>
using variant_alternative_t = typename VariantAlternative<Idx, T>::type;

template <size_t Idx, typename... Types>
struct VariantAlternative<Idx, Variant<Types...>> {
    using type = typename TypeAt<Idx, TypeList<Types...>>::target_type;
};
// -------TypeList-------

template <typename... Types>
class Variant {
public:
    // Special member functions
    constexpr Variant() noexcept {};

    template <typename T>
    Variant& operator=(T&& t) noexcept;

    template <typename T, typename... OtherTypes>
    friend auto&& GenericGet(Variant<OtherTypes...>& v);

private:
    StoreUnion<0, Types...> data_;
};

// -------FindExactlyOneT-------
const static size_t kNotFound = -1;
const static size_t kAmbiguity = kNotFound - 1;

template <size_t SizeofFound>
constexpr size_t CheckDuplicatesHelper(size_t cur_pos, size_t res, const bool (&exact)[SizeofFound],
                                       const bool (&convertible)[SizeofFound]) {
    if (res == kAmbiguity) {
        return kAmbiguity;
    }

    if (exact[cur_pos]) {
        if (res == kNotFound || !exact[res]) {
            return cur_pos;
        }

        return kAmbiguity;
    }

    if (convertible[cur_pos] && res == kNotFound) {
        return cur_pos;
    }

    return res;
}

template <size_t SizeofFound>
constexpr size_t CheckDuplicates(size_t cur_pos, const bool (&exact)[SizeofFound],
                                 const bool (&convertible)[SizeofFound]) {
    if (cur_pos == SizeofFound) {
        return kNotFound;
    }

    return CheckDuplicatesHelper(cur_pos, CheckDuplicates(cur_pos + 1, exact, convertible),
            exact, convertible);
}

template <typename TargetType, typename... Types>
struct FindExactlyOneChecked {
    constexpr static bool kExact[sizeof...(Types)] = {std::is_same<TargetType, Types>::value...};
    constexpr static bool kConvertible[sizeof...(Types)] =
            {std::is_convertible<TargetType, Types>::value...};
    constexpr static size_t kValue = CheckDuplicates(0, kExact, kConvertible);

    static_assert(kValue != kNotFound, "no such type in parameter list");
    static_assert(kValue != kAmbiguity, "several same types encountered");
};

template <typename T>
struct FindExactlyOneChecked<T> {
    static_assert(!std::is_same<T, T>::value, "type not in empty type list");
};

template <typename TargetType, typename... Types>
struct FindExactlyOneT : public FindExactlyOneChecked<TargetType, Types...> {};
// -------FindExactlyOneT-------

template <typename... Types>
template <typename T>
Variant<Types...>& Variant<Types...>::operator=(T&& t) noexcept {
    constexpr size_t kIdx = FindExactlyOneT<T, Types...>::kValue;
    UnionHelper::Set<kIdx>(std::forward<T>(t), std::in_place_index<kIdx>, data_);

    return *this;
}

// Non-member functions
template <typename T, typename... Types>
auto&& GenericGet(Variant<Types...>& v) {
    return UnionHelper::Get(std::forward<Variant<Types...>>(v).data_,
                            std::in_place_index<FindExactlyOneT<T, Types...>::kValue>);
}

template <size_t I, typename... Types>
constexpr const variant_alternative_t<I, Variant<Types...>>& Get(Variant<Types...>& v) {
    return GenericGet<typename TypeAt<I, TypeList<Types...>>::target_type>(v);
}

template <size_t I, typename... Types>
constexpr variant_alternative_t<I, Variant<Types...>>&& Get(Variant<Types...>&& v) {
    return GenericGet<typename TypeAt<I, TypeList<Types...>>::target_type>(std::move(v));
}

template <typename T, typename... Types>
constexpr const T& Get(Variant<Types...>& v) {
    return GenericGet<T>(v);
}

template <typename T, typename... Types>
constexpr T&& Get(Variant<Types...>&& v) {
    return GenericGet<T>(std::move(v));
}

}  // namespace task