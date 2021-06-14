#pragma once

#include "../control/control.h"

// SharedPtr
template <typename T>
class WeakPtr;

template <typename T>
class SharedPtr {
public:
    using element_type = T;

    constexpr SharedPtr() noexcept = default;
    ~SharedPtr();

    template <typename Y>
    explicit SharedPtr(Y* p);

    template <typename Y, typename Deleter>
    SharedPtr(Y* p, Deleter deleter) noexcept;

    SharedPtr(const SharedPtr& other) noexcept;
    SharedPtr(SharedPtr&& other) noexcept;

    SharedPtr& operator=(const SharedPtr& r) noexcept;

    template <typename Y>
    SharedPtr& operator=(const SharedPtr<Y>& r) noexcept;

    SharedPtr& operator=(SharedPtr&& r) noexcept;

    template <typename Y>
    SharedPtr& operator=(SharedPtr<Y>&& r) noexcept;

    // Modifiers
    void Reset() noexcept;

    template <typename Y>
    void Reset(Y* p) noexcept;

    template <typename Y, typename Deleter>
    void Reset(Y* p, Deleter deleter) noexcept;

    void Swap(SharedPtr& other) noexcept;

    // Observers
    T* Get() const noexcept;
    int64_t UseCount() const noexcept;
    T& operator*() const noexcept;
    T* operator->() const noexcept;
    element_type& operator[](std::ptrdiff_t idx) const;
    explicit operator bool() const noexcept;

    template <typename U>
    friend class WeakPtr;

private:
    element_type* ptr_ = nullptr;
    SharedWeakCount* control_block_ = nullptr;
};

// MakeShared
template <typename T, typename... Args>
SharedPtr<T> MakeShared(Args&&... args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}
// MakeShared

// SharedPtr
template <typename T>
SharedPtr<T>::~SharedPtr() {
    if (control_block_ != nullptr) {
        control_block_->ReleaseShared();
    }
    ptr_ = nullptr;
    control_block_ = nullptr;
}

template <typename T>
template <typename Y>
SharedPtr<T>::SharedPtr(Y* p) : ptr_(p), control_block_(new ControlBlock<T*>(p)) {
    control_block_->AddShared();
}

template <typename T>
template <typename Y, typename Deleter>
SharedPtr<T>::SharedPtr(Y* p, Deleter deleter) noexcept
    : ptr_(p), control_block_(new ControlBlock<T*, Deleter>(p, deleter)) {
    control_block_->AddShared();
}

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& other) noexcept
    : ptr_(other.ptr_), control_block_(other.control_block_) {
    if (control_block_ != nullptr) {
        control_block_->AddShared();
    }
}

template <typename T>
SharedPtr<T>::SharedPtr(SharedPtr&& other) noexcept
    : ptr_(other.ptr_), control_block_(other.control_block_) {
    other.ptr_ = nullptr;
    other.control_block_ = nullptr;
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& r) noexcept {
    SharedPtr<T>(r).Swap(*this);
    return *this;
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<T>&& r) noexcept {
    SharedPtr<T>(std::move(r)).Swap(*this);
    return *this;
}

template <typename T>
void SharedPtr<T>::Reset() noexcept {
    SharedPtr<T>().Swap(*this);
}

template <typename T>
template <typename Y>
void SharedPtr<T>::Reset(Y* p) noexcept {
    SharedPtr<T>(p).Swap(*this);
}

template <typename T>
template <typename Y, typename Deleter>
void SharedPtr<T>::Reset(Y* p, Deleter deleter) noexcept {
    SharedPtr<T>(p, deleter).Swap(*this);
}

template <typename T>
void SharedPtr<T>::Swap(SharedPtr& other) noexcept {
    std::swap(ptr_, other.ptr_);
    std::swap(control_block_, other.control_block_);
}

template <typename T>
T* SharedPtr<T>::Get() const noexcept {
    return ptr_;
}

template <typename T>
int64_t SharedPtr<T>::UseCount() const noexcept {
    return control_block_ == nullptr ? 0 : control_block_->GetShared();
}

template <typename T>
T& SharedPtr<T>::operator*() const noexcept {
    return *ptr_;
}

template <typename T>
T* SharedPtr<T>::operator->() const noexcept {
    return ptr_;
}

template <typename T>
typename SharedPtr<T>::element_type& SharedPtr<T>::operator[](std::ptrdiff_t idx) const {
    return ptr_ == nullptr ? nullptr : *(ptr_ + idx);
}

template <typename T>
SharedPtr<T>::operator bool() const noexcept {
    return ptr_ != nullptr;
}
// SharedPtr

// WeakPtr
template <typename T>
class WeakPtr {
public:
    using element_type = T;

    // Special-member functions
    constexpr WeakPtr() noexcept = default;
    template <typename Y>
    explicit WeakPtr(const SharedPtr<Y>& other);
    WeakPtr(const WeakPtr& other) noexcept;
    WeakPtr(WeakPtr&& other) noexcept;
    template <typename Y>
    WeakPtr& operator=(const SharedPtr<Y>& other);
    WeakPtr& operator=(const WeakPtr& other) noexcept;
    WeakPtr& operator=(WeakPtr&& other) noexcept;

    ~WeakPtr();

    // Modifiers
    void Reset() noexcept;
    void Swap(WeakPtr<T>& other) noexcept;

    // Observers
    bool Expired() const noexcept;
    SharedPtr<T> Lock() const noexcept;

    template <typename U>
    friend class SharedPtr;

public:
    element_type* ptr_ = nullptr;
    SharedWeakCount* control_block_ = nullptr;
};

// WeakPtr
template <typename T>
template <typename Y>
WeakPtr<T>::WeakPtr(const SharedPtr<Y>& other)
    : ptr_(other.ptr_), control_block_(other.control_block_) {
    if (control_block_ != nullptr) {
        control_block_->AddWeak();
    }
}

template <typename T>
WeakPtr<T>::WeakPtr(const WeakPtr& other) noexcept
    : ptr_(other.ptr_), control_block_(other.control_block_) {
    if (control_block_ != nullptr) {
        control_block_->AddWeak();
    }
}

template <typename T>
WeakPtr<T>::WeakPtr(WeakPtr&& other) noexcept
    : ptr_(other.ptr_), control_block_(std::move(other.control_block_)) {
    other.ptr_ = nullptr;
    other.control_block_ = nullptr;
}

template <typename T>
template <typename Y>
WeakPtr<T>& WeakPtr<T>::operator=(const SharedPtr<Y>& other) {
    WeakPtr<T>(other).Swap(*this);
    return *this;
}

template <typename T>
WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr& other) noexcept {
    WeakPtr<T>(other).Swap(*this);
    return *this;
}

template <typename T>
WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr&& other) noexcept {
    WeakPtr<T>(std::move(other)).Swap(*this);
    return *this;
}

template <typename T>
WeakPtr<T>::~WeakPtr() {
    if (control_block_ != nullptr) {
        control_block_->ReleaseWeak();
    }
}

template <typename T>
void WeakPtr<T>::Reset() noexcept {
    if (control_block_ != nullptr) {
        control_block_->ReleaseWeak();
    }
    ptr_ = nullptr;
    control_block_ = nullptr;
}

template <typename T>
void WeakPtr<T>::Swap(WeakPtr<T>& other) noexcept {
    std::swap(ptr_, other.ptr_);
    std::swap(control_block_, other.control_block_);
}

template <typename T>
bool WeakPtr<T>::Expired() const noexcept {
    return control_block_ == nullptr || control_block_->GetShared() == 0;
}

template <typename T>
SharedPtr<T> WeakPtr<T>::Lock() const noexcept {
    return Expired() ? SharedPtr<T>() : SharedPtr<T>(ptr_);
}
// WeakPtr