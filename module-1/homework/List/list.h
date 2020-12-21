#pragma once
#include <cstddef>


namespace task {


class list {

public:

    list();
    list(const list& other);
    list(size_t count, const int& value = int());

    ~list();
    list& operator=(const list& other);


    int& front();
    const int& front() const;

    int& back();
    const int& back() const;


    bool empty() const;
    size_t size() const;
    void clear();


    void push_back(const int& value);
    void pop_back();
    void push_front(const int& value);
    void pop_front();
    void resize(size_t count);
    void swap(list& other);


    void remove(const int& value);
    void unique();
    void sort();

    // Your code goes here?..

private:

    class Node {
    public:
        explicit Node(int value, Node* prev = nullptr, Node* next = nullptr) {
            this->value = value;
            this->prev = prev;
            this->next = next;
        }

        int value;
        Node* prev;
        Node* next;
    };

    static std::pair<list*, list*> split(const list& l);

    static list merge(const list& left, const list& right);

    static list mergeSort(const list& l);

    Node* root_ = new Node(0);

    size_t size_ = 0;
};

}  // namespace task
