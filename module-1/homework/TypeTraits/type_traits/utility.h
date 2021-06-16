#pragma once

#include <type_traits>
#include <utility>

template <typename T>
struct RemoveRef {
    using type = T;
};

template <typename T>
struct RemoveRef<T&> {
    using type = T;
};

template <typename T>
struct RemoveRef<T&&> {
    using type = T;
};

template <typename T>
struct RemoveConstVol {
    using type = T;
};

template <typename T>
struct RemoveConstVol<const T> {
    using type = T;
};

template <typename T>
struct RemoveConstVol<volatile T> {
    using type = T;
};

template <typename T>
struct RemoveConstVol<const volatile T> {
    using type = T;
};

template <typename T>
struct Uncvref {
    using type = typename RemoveConstVol<typename RemoveRef<T>::type>::type;
};

template <typename T>
using uncvref_t = typename Uncvref<T>::type;

template <typename T>
struct AddConst {
    using type = const T;
};

template <typename T>
using add_const_t = typename AddConst<T>::type;

template <typename T>
struct AddLvalueReference {
    using type = T&;
};

template <typename T>
struct AddRvalueReference {
    using type = T&&;
};

template <typename T>
using add_lvalue_reference_t = typename AddLvalueReference<T>::type;

template <typename T>
using add_rvalue_reference_t = typename AddRvalueReference<T>::type;