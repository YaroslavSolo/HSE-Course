#include <stdexcept>
#include "list.h"

task::list::list() {
    root_->prev = root_;
    root_->next = root_;
}

task::list::list(const list& other) : list() {
    Node* cur = other.root_->next;
    while (cur != other.root_) {
        push_back(cur->value);
        cur = cur->next;
    }
}

task::list::list(size_t count, const int& value) : list() {
    for (int i = 0; i < count; ++i)
        push_back(value);
}

task::list::~list() {
    clear();
    delete root_;
}

task::list& task::list::operator=(const list& other) {
    if (this == &other)
        return *this;

    clear();
    Node* cur = other.root_->next;
    while (cur != other.root_) {
        push_back(cur->value);
        cur = cur->next;
    }

    return *this;
}

int& task::list::front() {
    if (empty())
        throw std::runtime_error("Accessing element in an empty list");
    return root_->next->value;
}

const int& task::list::front() const {
    if (empty())
        throw std::runtime_error("Accessing element in an empty list");
    return root_->next->value;
}

int& task::list::back() {
    if (empty())
        throw std::runtime_error("Accessing element in an empty list");
    return root_->prev->value;
}

const int& task::list::back() const {
    if (empty())
        throw std::runtime_error("Accessing element in an empty list");
    return root_->prev->value;
}

bool task::list::empty() const {
    return size_ == 0;
}

size_t task::list::size() const {
    return size_;
}

void task::list::clear() {
    if (empty())
        return;

    size_ = 0;
    Node* cur = root_->next;
    while (cur != root_) {
        cur = cur->next;
        delete cur->prev;
    }

    root_->next = root_;
    root_->prev = root_;
}

void task::list::push_back(const int& value) {
    root_->prev->next = new Node(value, root_->prev, root_);
    root_->prev = root_->prev->next;
    ++size_;
}

void task::list::pop_back() {
    if (empty())
        return;

    Node* target = root_->prev;
    target->prev->next = root_;
    root_->prev = target->prev;
    delete target;
    --size_;
}

void task::list::push_front(const int& value) {
    root_->next->prev = new Node(value, root_, root_->next);
    root_->next = root_->next->prev;
    ++size_;
}

void task::list::pop_front() {
    if (empty())
        return;

    Node* target = root_->next;
    target->next->prev = root_;
    root_->next = target->next;
    delete target;
    --size_;
}

void task::list::resize(size_t count) {
    while (size_ > count)
        pop_back();

    while (size_ < count)
        push_back(int());
}

void task::list::swap(list& other) {
    std::swap(root_, other.root_);
    std::swap(size_, other.size_);
}

void task::list::remove(const int& value) {
    if (empty())
        return;

    Node* cur = root_->next;
    while (cur != root_) {
        cur = cur->next;
        if (cur->prev->value == value) {
            Node* target = cur->prev;
            target->prev->next = cur;
            target->next->prev = target->prev;
            delete target;
            --size_;
        }
    }
}

void task::list::unique() {
    if (size_ < 2)
        return;

    Node* cur = root_->next->next;
    int cur_value = root_->next->value;

    while (cur != root_) {
        if (cur->value == cur_value) {
            Node* target = cur;
            target->prev->next = cur->next;
            target->next->prev = target->prev;
            delete target;
            --size_;
        } else {
            cur_value = cur->value;
        }
        cur = cur->next;
    }
}

void task::list::sort() {
    *this = mergeSort(*this);
}

std::pair<task::list*, task::list*> task::list::split(const list& l) {
    list* left = new list();
    list* right = new list();

    Node* cur = l.root_->next;
    for (int i = 0; i < l.size() / 2; ++i, cur = cur->next)
        left->push_back(cur->value);

    for (int i = l.size() / 2; i < l.size(); ++i, cur = cur->next)
        right->push_back(cur->value);

    return {left, right};
}

task::list task::list::merge(const list& left, const list& right) {
    list res;
    Node* cur_left = left.root_->next;
    Node* cur_right = right.root_->next;

    while (cur_left != left.root_ && cur_right != right.root_) {
        if (cur_right->value < cur_left->value) {
            res.push_back(cur_right->value);
            cur_right = cur_right->next;
        } else {
            res.push_back(cur_left->value);
            cur_left = cur_left->next;
        }
    }

    while (cur_left != left.root_) {
        res.push_back(cur_left->value);
        cur_left = cur_left->next;
    }

    while (cur_right != right.root_) {
        res.push_back(cur_right->value);
        cur_right = cur_right->next;
    }

    return res;
}

task::list task::list::mergeSort(const list& l) {
    if (l.size() > 1) {
        auto halves = split(l);
        return merge(mergeSort(*halves.first), mergeSort(*halves.second));
    }

    return l;
}
