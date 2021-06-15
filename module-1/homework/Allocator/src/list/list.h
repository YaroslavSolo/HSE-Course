#pragma once

#include <list>
#include <memory>
#include <type_traits>

namespace task {

template <typename T, typename Allocator = std::allocator<T>>
class List {
private:
    class Node;

public:
    class Iterator;

    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using allocator_type = Allocator;
    using difference_type = ptrdiff_t;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    using iterator = Iterator;
    using const_iterator = const Iterator;

    using node_allocator = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
    using node_allocator_traits = std::allocator_traits<node_allocator>;

    // Special member functions
    List();

    List(const List& other)
        : allocator_(node_allocator_traits::select_on_container_copy_construction(other.allocator_)),
          root_(allocator_.allocate(1)) {
        allocator_.construct(root_);
        Node* cur = other.root_->next;

        while (cur != other.root_) {
            PushBack(cur->value);
            cur = cur->next;
        }
    }

    List(const List& other, const Allocator& alloc);

    List(List&& other);
    List(List&& other, const Allocator& alloc);

    ~List();

    List& operator=(const List& other);

    List& operator=(List&& other) noexcept;

    // Element access
    reference Front();
    const_reference Front() const;
    reference Back();
    const_reference Back() const;

    // Iterators
    iterator Begin() noexcept;
    const_iterator Begin() const noexcept;

    iterator End() noexcept;
    const_iterator End() const noexcept;

    // Capacity
    bool Empty() const noexcept;

    size_type Size() const noexcept;
    size_type MaxSize() const noexcept;

    // Modifiers
    void Clear();
    void Swap(List& other) noexcept;

    void PushBack(const T& value);
    void PushBack(T&& value);

    template <typename... Args>
    void EmplaceBack(Args&&... args);
    void PopBack();
    void PushFront(const T& value);
    void PushFront(T&& value);
    template <typename... Args>
    void EmplaceFront(Args&&... args);
    void PopFront();

    void Resize(size_type count);

    // Operations
    void Remove(const T& value);
    void Unique();
    void Sort();

    allocator_type GetAllocator() const noexcept;

    class Iterator {
    public:
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;
        using pointer = T*;
        using difference_type = ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;

        explicit Iterator(Node* node) : node_(node) {
        }

        Iterator& operator++() {
            node_ = node_->next;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp(*this);
            ++(*this);
            return temp;
        }

        Iterator& operator--() {
            node_ = node_->prev;
            return *this;
        }

        Iterator operator--(int) {
            iterator temp(*this);
            --(*this);
            return temp;
        }

        bool operator==(const Iterator& rhs) {
            return node_ == rhs.node_;
        }

        bool operator!=(const Iterator& rhs) {
            return node_ != rhs.node_;
        }

        reference operator*() {
            return node_->value;
        }

        pointer operator->() {
            return &(node_->value);
        }

    private:
        Node* node_;
    };

private:
    class Node {
    public:
        Node() = default;

        explicit Node(value_type value, Node* prev = nullptr, Node* next = nullptr) noexcept {
            this->value = value;
            this->prev = prev;
            this->next = next;
        }

        template <typename... Args>
        Node(Args&&... args, Node* prev, Node* next) {
            this->value = value_type(std::forward<Args>(args)...);
            this->prev = prev;
            this->next = next;
        }

        value_type value;
        Node* prev = nullptr;
        Node* next = nullptr;
    };

    static std::pair<List<T, Allocator>*, List<T, Allocator>*> Split(const List<T, Allocator>& l);

    static List<T, Allocator> Merge(const List<T, Allocator>& left,
                                    const List<T, Allocator>& right);

    static List<T, Allocator> MergeSort(const List<T, Allocator>& l);

    Node* root_ = nullptr;

    size_type size_ = 0;

    node_allocator allocator_;
};

template <typename T, typename Allocator>
List<T, Allocator>::~List() {
    Clear();
    allocator_.destroy(root_);
    allocator_.deallocate(root_, 1);
}

template <typename T, typename Allocator>
List<T, Allocator>::List() {
    root_ = allocator_.allocate(1);
    allocator_.construct(root_);
    root_->prev = root_;
    root_->next = root_;
}

template <typename T, typename Allocator>
List<T, Allocator>& List<T, Allocator>::operator=(const List& other) {
    if (this == &other) {
        return *this;
    }

    if (node_allocator_traits::propagate_on_container_copy_assignment::value) {
        if (allocator_ != other.allocator_) {
            Clear();
        }
        allocator_ = other.allocator_;
    }

    Clear();
    Node* cur = other.root_->next;
    while (cur != other.root_) {
        PushBack(cur->value);
        cur = cur->next;
    }

    return *this;
}

template <typename T, typename Allocator>
typename List<T, Allocator>::reference List<T, Allocator>::Front() {
    if (Empty()) {
        throw std::runtime_error("Accessing element in an empty list");
    }
    return root_->next->value;
}

template <typename T, typename Allocator>
typename List<T, Allocator>::const_reference List<T, Allocator>::Front() const {
    if (Empty()) {
        throw std::runtime_error("Accessing element in an empty list");
    }
    return root_->next->value;
}

template <typename T, typename Allocator>
typename List<T, Allocator>::reference List<T, Allocator>::Back() {
    if (Empty()) {
        throw std::runtime_error("Accessing element in an empty list");
    }
    return root_->prev->value;
}

template <typename T, typename Allocator>
typename List<T, Allocator>::const_reference List<T, Allocator>::Back() const {
    if (Empty()) {
        throw std::runtime_error("Accessing element in an empty list");
    }
    return root_->prev->value;
}

template <typename T, typename Allocator>
bool List<T, Allocator>::Empty() const noexcept {
    return size_ == 0;
}

template <typename T, typename Allocator>
size_t List<T, Allocator>::Size() const noexcept {
    return size_;
}

template <typename T, typename Allocator>
void List<T, Allocator>::Clear() {
    if (Empty()) {
        return;
    }

    size_ = 0;
    Node* cur = root_->next;
    while (cur != root_) {
        cur = cur->next;
        allocator_.destroy(cur->prev);
        allocator_.deallocate(cur->prev, 1);
    }

    root_->next = root_;
    root_->prev = root_;
}

template <typename T, typename Allocator>
void List<T, Allocator>::PushBack(const T& value) {
    root_->prev->next = allocator_.allocate(1);
    allocator_.construct(root_->prev->next, value, root_->prev, root_);
    root_->prev = root_->prev->next;
    ++size_;
}

template <typename T, typename Allocator>
void List<T, Allocator>::PopBack() {
    if (Empty()) {
        return;
    }

    Node* target = root_->prev;
    target->prev->next = root_;
    root_->prev = target->prev;
    allocator_.destroy(target);
    allocator_.deallocate(target, 1);
    --size_;
}

template <typename T, typename Allocator>
void List<T, Allocator>::PushFront(const T& value) {
    root_->next->prev = allocator_.allocate(1);
    allocator_.construct(root_->next->prev, value, root_, root_->next);
    root_->next = root_->next->prev;
    ++size_;
}

template <typename T, typename Allocator>
void List<T, Allocator>::PopFront() {
    if (Empty()) {
        return;
    }

    Node* target = root_->next;
    target->next->prev = root_;
    root_->next = target->next;
    allocator_.destroy(target);
    allocator_.deallocate(target, 1);
    --size_;
}

template <typename T, typename Allocator>
void List<T, Allocator>::Resize(size_t count) {
    while (size_ > count) {
        PopBack();
    }

    while (size_ < count) {
        PushBack({});
    }
}

template <typename T, typename Allocator>
void List<T, Allocator>::Swap(List<T, Allocator>& other) noexcept {
    std::swap(root_, other.root_);
    std::swap(size_, other.size_);
    bool swap_alloc = node_allocator_traits::propagate_on_container_swap::value;

    if (!swap_alloc && allocator_ != other.allocator_) {
        throw std::runtime_error("Swapping lists with different allocators without propagation");
    }

    if (swap_alloc) {
        std::swap(allocator_, other.allocator_);
    }
}

template <typename T, typename Allocator>
void List<T, Allocator>::Remove(const T& value) {
    if (Empty()) {
        return;
    }

    Node* cur = root_->next;
    while (cur != root_) {
        cur = cur->next;
        if (cur->prev->value == value) {
            Node* target = cur->prev;
            target->prev->next = cur;
            target->next->prev = target->prev;
            allocator_.destroy(target);
            allocator_.deallocate(target, 1);
            --size_;
        }
    }
}

template <typename T, typename Allocator>
void List<T, Allocator>::Unique() {
    if (size_ < 2) {
        return;
    }

    Node* cur = root_->next->next;
    value_type cur_value = root_->next->value;

    while (cur != root_) {
        if (cur->value == cur_value) {
            Node* target = cur;
            target->prev->next = cur->next;
            target->next->prev = target->prev;
            allocator_.destroy(target);
            allocator_.deallocate(target, 1);
            --size_;
        } else {
            cur_value = cur->value;
        }
        cur = cur->next;
    }
}

template <typename T, typename Allocator>
void List<T, Allocator>::Sort() {
    *this = MergeSort(*this);
}

template <typename T, typename Allocator>
std::pair<List<T, Allocator>*, List<T, Allocator>*> List<T, Allocator>::Split(const List& l) {
    List<T, Allocator>* left = new List<T, Allocator>();
    List<T, Allocator>* right = new List<T, Allocator>();

    Node* cur = l.root_->next;
    for (size_t i = 0; i < l.Size() / 2; ++i, cur = cur->next) {
        left->PushBack(std::move(cur->value));
    }

    for (size_t i = l.Size() / 2; i < l.Size(); ++i, cur = cur->next) {
        right->PushBack(std::move(cur->value));
    }

    return {left, right};
}

template <typename T, typename Allocator>
List<T, Allocator> List<T, Allocator>::Merge(const List<T, Allocator>& left,
                                             const List<T, Allocator>& right) {
    List<T, Allocator> res;
    Node* cur_left = left.root_->next;
    Node* cur_right = right.root_->next;

    while (cur_left != left.root_ && cur_right != right.root_) {
        if (cur_right->value < cur_left->value) {
            res.PushBack(std::move(cur_right->value));
            cur_right = cur_right->next;
        } else {
            res.PushBack(std::move(cur_left->value));
            cur_left = cur_left->next;
        }
    }

    while (cur_left != left.root_) {
        res.PushBack(std::move(cur_left->value));
        cur_left = cur_left->next;
    }

    while (cur_right != right.root_) {
        res.PushBack(std::move(cur_right->value));
        cur_right = cur_right->next;
    }

    return res;
}

template <typename T, typename Allocator>
List<T, Allocator> List<T, Allocator>::MergeSort(const List<T, Allocator>& l) {
    if (l.Size() > 1) {
        auto halves = Split(l);
        return Merge(MergeSort(*halves.first), MergeSort(*halves.second));
    }

    return l;
}

template <typename T, typename Allocator>
List<T, Allocator>::List(const List& other, const Allocator& alloc)
    : allocator_(alloc), root_(allocator_.allocate(1)) {
    allocator_.construct(root_);
    Node* cur = other.root_->next;

    while (cur != other.root_) {
        PushBack(cur->value);
        cur = cur->next;
    }
}

template <typename T, typename Allocator>
List<T, Allocator>::List(List<T, Allocator>&& other)
        : allocator_(std::move(other.allocator_)), root_(other.root_), size_(other.size_) {
    other.root_ = nullptr;
}

template <typename T, typename Allocator>
List<T, Allocator>::List(List<T, Allocator>&& other, const Allocator& alloc)
        : allocator_(alloc), root_(other.root_), size_(other.size_) {
    other.root_ = nullptr;
}

template <typename T, typename Allocator>
List<T, Allocator>& List<T, Allocator>::operator=(List<T, Allocator>&& other) noexcept {
    Clear();
    if (node_allocator_traits::is_always_equal::value || allocator_ == other.allocator_) {
        root_ = other.root_;
        size_ = other.size_;
    } else if (node_allocator_traits::propagate_on_container_move_assignment::value) {
        allocator_ = std::move(other.allocator_);
        root_ = other.root_;
        size_ = other.size_;
    } else {
        for (auto iter = other.Begin(); iter != other.End(); ++iter) {
            PushBack(std::move(*iter));
        }
    }

    other.root_ = nullptr;

    return *this;
}

template <typename T, typename Allocator>
typename List<T, Allocator>::size_type List<T, Allocator>::MaxSize() const noexcept {
    return std::numeric_limits<difference_type>::max();
}

template <typename T, typename Allocator>
typename List<T, Allocator>::iterator List<T, Allocator>::Begin() noexcept {
    return task::List<T, Allocator>::iterator(root_->next);
}

template <typename T, typename Allocator>
typename List<T, Allocator>::const_iterator List<T, Allocator>::Begin() const noexcept {
    return task::List<T, Allocator>::const_iterator(root_->next);
}

template <typename T, typename Allocator>
typename List<T, Allocator>::iterator List<T, Allocator>::End() noexcept {
    return task::List<T, Allocator>::iterator(root_);
}

template <typename T, typename Allocator>
typename List<T, Allocator>::const_iterator List<T, Allocator>::End() const noexcept {
    return task::List<T, Allocator>::const_iterator(root_);
}

template <typename T, typename Allocator>
void List<T, Allocator>::PushBack(T&& value) {
    root_->prev->next = allocator_.allocate(1);
    allocator_.construct(root_->prev->next, std::forward<T>(value), root_->prev, root_);
    root_->prev = root_->prev->next;
    ++size_;
}

template <typename T, typename Allocator>
template <typename... Args>
void List<T, Allocator>::EmplaceBack(Args&&... args) {
    root_->prev->next = allocator_.allocate(1);
    allocator_.construct(root_->prev->next, std::forward<Args>(args)..., root_->prev, root_);
    root_->prev = root_->prev->next;
    ++size_;
}

template <typename T, typename Allocator>
void List<T, Allocator>::PushFront(T&& value) {
    root_->next->prev = allocator_.allocate(1);
    allocator_.construct(root_->next->prev, std::forward<T>(value), root_, root_->next);
    root_->next = root_->next->prev;
    ++size_;
}

template <typename T, typename Allocator>
template <typename... Args>
void List<T, Allocator>::EmplaceFront(Args&&... args) {
    root_->next->prev = allocator_.allocate(1);
    allocator_.construct(root_->next->prev, std::forward<Args>(args)..., root_, root_->next);
    root_->next = root_->next->prev;
    ++size_;
}

template <typename T, typename Allocator>
typename List<T, Allocator>::allocator_type List<T, Allocator>::GetAllocator() const noexcept {
    return allocator_type(allocator_);
}

}  // namespace task