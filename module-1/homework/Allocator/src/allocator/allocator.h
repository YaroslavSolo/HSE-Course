#include <memory>
#include <type_traits>

template <typename T>
class CustomAllocator {
public:
    template <typename U>
    struct rebind {  // NOLINT
        using other = CustomAllocator<U>;
    };

    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using propagate_on_container_copy_assignment = std::false_type;
    using propagate_on_container_move_assignment = std::false_type;
    using propagate_on_container_swap = std::true_type;
    using is_always_equal = std::false_type;

    CustomAllocator();
    CustomAllocator(const CustomAllocator& other) noexcept;
    ~CustomAllocator();

    template <typename U>
    explicit CustomAllocator(const CustomAllocator<U>& other) noexcept;

    T* allocate(size_t n) {  // NOLINT
        if (*offset_ + n > kArenaSize) {
            throw std::runtime_error("Allocator arena memory limit exceeded");
        }

        *offset_ += n;

        return static_cast<pointer>(arena_) + (*offset_ - n);
    }

    void deallocate(T* p, size_t n) {  // NOLINT
    };

    template <typename... Args>
    void construct(pointer p, Args&&... args) {  // NOLINT
        ::new (p) value_type(std::forward<Args>(args)...);
    };

    void destroy(pointer p) {  // NOLINT
        p->~T();
    };

    void* GetArena() const {
        return arena_;
    }

    size_type* GetOffset() const {
        return offset_;
    }

    size_type* GetNumAllocators() const {
        return num_allocators_;
    }

    template <typename K, typename U>
    friend bool operator==(const CustomAllocator<K>& lhs, const CustomAllocator<U>& rhs) noexcept;

    template <typename K, typename U>
    friend bool operator!=(const CustomAllocator<K>& lhs, const CustomAllocator<U>& rhs) noexcept;

private:
    static const size_t kArenaSize = 100000;
    void* arena_ = nullptr;
    size_type* offset_ = nullptr;
    size_type* num_allocators_ = nullptr;
};

template <typename T, typename U>
bool operator==(const CustomAllocator<T>& lhs, const CustomAllocator<U>& rhs) noexcept {
    return lhs.arena_ == rhs.arena_;
}

template <typename T, typename U>
bool operator!=(const CustomAllocator<T>& lhs, const CustomAllocator<U>& rhs) noexcept {
    return !(lhs == rhs);
}

template <typename T>
CustomAllocator<T>::CustomAllocator() {
    arena_ = ::operator new(kArenaSize * sizeof(value_type));
    offset_ = new size_type(0);
    num_allocators_ = new size_type(1);
}

template <typename T>
CustomAllocator<T>::CustomAllocator(const CustomAllocator& other) noexcept
    : arena_(other.arena_), offset_(other.offset_), num_allocators_(other.num_allocators_) {
    ++(*num_allocators_);
}

template <typename T>
CustomAllocator<T>::~CustomAllocator() {
    if (--(*num_allocators_) == 0) {
        ::operator delete(arena_);
        delete offset_;
        delete num_allocators_;
    }
}

template <typename T>
template <typename U>
CustomAllocator<T>::CustomAllocator(const CustomAllocator<U>& other) noexcept
    : arena_(other.GetArena()),
    offset_(other.GetOffset()),
    num_allocators_(other.GetNumAllocators()) {
    ++(*num_allocators_);
}
