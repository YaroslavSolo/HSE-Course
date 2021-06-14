#pragma once

#include <atomic>

class SharedCount {
public:
    SharedCount() = default;

    explicit SharedCount(size_t count) noexcept : sharedCount(count) {
    }

    virtual ~SharedCount() = default;

    void AddShared() noexcept {
        ++sharedCount;
    }

    bool ReleaseShared() noexcept {
        if (--sharedCount == 0) {
            OnZeroShared();
            return false;
        }

        return true;
    }

    size_t GetShared() {
        return sharedCount;
    }

    virtual void OnZeroShared() noexcept = 0;

protected:
    std::atomic<size_t> sharedCount{0};
};

class SharedWeakCount : public SharedCount {
public:
    SharedWeakCount() = default;

    virtual ~SharedWeakCount() = default;

    void AddWeak() noexcept {
        ++weakCount;
    }

    void ReleaseWeak() noexcept {
        --weakCount;
    }

    size_t GetWeak() {
        return weakCount;
    }

protected:
    std::atomic<size_t> weakCount{0};
};

template <typename T, typename Deleter = std::default_delete<std::remove_pointer_t<T>>>
class ControlBlock : public SharedWeakCount {
public:
    ControlBlock() = default;

    explicit ControlBlock(T& object)
        : object_(object), deleter_(std::default_delete<std::remove_pointer_t<T>>()){};

    ControlBlock(T& object, Deleter& deleter) : object_(object), deleter_(deleter){};

    ControlBlock(ControlBlock&) = delete;

    void operator=(ControlBlock&) = delete;

    void OnZeroShared() noexcept override {
        deleter_(object_);
    }

private:
    T object_;
    Deleter deleter_;
};