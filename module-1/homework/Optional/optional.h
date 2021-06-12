#include <cstdlib>
#include <type_traits>

#pragma once

namespace task {

struct NullOpt {};

constexpr NullOpt kNullOpt{};

struct InPlace {
    InPlace() noexcept = default;

    ~InPlace() noexcept = default;
};

constexpr InPlace kInPlace{};

template <typename T, bool>
class OptionalDestructBase {
public:
    constexpr OptionalDestructBase() : engaged_(false) {
    }

    constexpr explicit OptionalDestructBase(NullOpt) : engaged_(false) {
    }

    template <typename U = T>
    constexpr explicit OptionalDestructBase(U&& val)
            : value_(std::forward<U>(val)), engaged_(true) {
    }

    template <typename... Args>
    constexpr explicit OptionalDestructBase(InPlace, Args&&... args)
            : value_(std::forward<Args>(args)...), engaged_(true) {
    }

protected:
    template <typename U = T>
    void Set(U&& value) {
        value_ = std::forward<U>(value);
        engaged_ = true;
    }

    void ResetHelper() {
        engaged_ = false;
    }

    T value_;

    bool engaged_;
};

template <typename T>
class OptionalDestructBase<T, false> {
public:
    constexpr OptionalDestructBase() : engaged_(false) {
    }

    constexpr explicit OptionalDestructBase(NullOpt) : engaged_(false) {
    }

    template <typename U = T>
    constexpr explicit OptionalDestructBase(U&& val)
            : value_(std::forward<U>(val)), engaged_(true) {
    }

    template <typename... Args>
    constexpr explicit OptionalDestructBase(InPlace, Args&&... args)
            : value_(std::forward<Args>(args)...), engaged_(true) {
    }

    ~OptionalDestructBase() {
        if (engaged_) {
            value_.~T();
        }
    }

protected:
    template <typename U = T>
    void Set(U&& value) {
        if (engaged_) {
            value_.~T();
        }
        value_ = std::forward<U>(value);
        engaged_ = true;
    }

    void ResetHelper() {
        if (engaged_) {
            value_.~T();
        }
        engaged_ = false;
    }

    T value_;

    bool engaged_;
};

template <typename T>
class Optional : public OptionalDestructBase<T, std::is_trivially_destructible_v<T>> {
private:
    using base = OptionalDestructBase<T, std::is_trivially_destructible_v<T>>;

public:
    using value_type = T;

    constexpr Optional() noexcept : base(){};

    template <typename U = value_type>
    constexpr explicit Optional(U&& value);

    constexpr explicit Optional(NullOpt) noexcept;

    template <typename... Args>
    constexpr explicit Optional(InPlace, Args&&... args);

    Optional& operator=(NullOpt) noexcept;

    template <typename U = T>
    Optional& operator=(U&& value);

    void Reset() noexcept;

    template <typename U>
    constexpr T ValueOr(U&& default_value) const&;

    template <typename U>
    constexpr T ValueOr(U&& default_value) &&;

    constexpr bool HasValue() const noexcept;

    constexpr explicit operator bool() const noexcept;

    constexpr std::add_pointer_t<const value_type> operator->() const;

    constexpr std::add_pointer_t<value_type> operator->();

    constexpr const value_type& operator*() const&;

    constexpr value_type& operator*() &;

    constexpr const value_type&& operator*() const&&;

    constexpr value_type&& operator*() &&;
};

template <typename T>
template <typename U>
constexpr Optional<T>::Optional(U&& value) : base(std::forward<U>(value)) {
}

template <typename T>
constexpr Optional<T>::Optional(NullOpt) noexcept : base() {
}

template <typename T>
template <typename... Args>
constexpr Optional<T>::Optional(InPlace, Args&&... args) : base(kInPlace, args...) {
}

template <typename T>
Optional<T>& Optional<T>::operator=(NullOpt) noexcept {
    this->ResetHelper();
    return *this;
}

template <typename T>
template <typename U>
Optional<T>& Optional<T>::operator=(U&& value) {
    Set(std::forward<U>(value));
    return *this;
}

template <typename T>
void Optional<T>::Reset() noexcept {
    this->ResetHelper();
}

template <typename T>
template <typename U>
constexpr T Optional<T>::ValueOr(U&& default_value) const& {
    return this->engaged_ ? this->value_ : default_value;
}

template <typename T>
template <typename U>
constexpr T Optional<T>::ValueOr(U&& default_value) && {
    return this->engaged_ ? this->value_ : default_value;
}

template <typename T>
constexpr bool Optional<T>::HasValue() const noexcept {
    return this->engaged_;
}

template <typename T>
constexpr Optional<T>::operator bool() const noexcept {
    return this->engaged_;
}

template <typename T>
constexpr std::add_pointer_t<const typename Optional<T>::value_type> Optional<T>::operator->()
const {
    return &(this->value_);
}

template <typename T>
constexpr std::add_pointer_t<typename Optional<T>::value_type> Optional<T>::operator->() {
    return &(this->value_);
}

template <typename T>
constexpr const typename Optional<T>::value_type& Optional<T>::operator*() const& {
    return this->value_;
}

template <typename T>
constexpr typename Optional<T>::value_type& Optional<T>::operator*() & {
    return this->value_;
}

template <typename T>
constexpr const typename Optional<T>::value_type&& Optional<T>::operator*() const&& {
    return this->value_;
}

template <typename T>
constexpr typename Optional<T>::value_type&& Optional<T>::operator*() && {
    return this->value_;
}

}  // namespace task