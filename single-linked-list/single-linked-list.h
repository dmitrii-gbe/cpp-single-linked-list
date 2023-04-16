#pragma once
#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>
#include <iostream>

template <typename Type>
class SingleLinkedList {
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };
    
    template <typename ValueType>
    class BasicIterator {
        friend class SingleLinkedList;

        explicit BasicIterator(Node* node) {
            node_ = node;
        }
        
     public:

        using iterator_category = std::forward_iterator_tag;

        using value_type = Type;

        using difference_type = std::ptrdiff_t;

        using pointer = ValueType*;

        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept : node_(other.node_) {
        }

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
           if (this->node_ == rhs.node_){
               return true;
           }
           else {
               return false;
           }
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
           if (this->node_ != rhs.node_){
               return true;
           }
           else {
               return false;
           }
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
        if (this->node_ == rhs.node_){
               return true;
           }
        else {
               return false;
           }
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
           if (this->node_ != rhs.node_){
               return true;
           }
           else {
               return false;
           }
        }

        BasicIterator& operator++() noexcept {
            this->node_ = node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            auto old_value(*this);
            ++(*this);
            return old_value;
        }

        [[nodiscard]] reference operator*() const noexcept {
            return node_->value;   
        }

        [[nodiscard]] pointer operator->() const noexcept {
            return &(node_->value);
        }

    private:
        Node* node_ = nullptr;
        
    };

public:

    SingleLinkedList() = default;
    
     SingleLinkedList(std::initializer_list<Type> values) {
        for (auto it = values.end() - 1; it + 1 != values.begin(); --it){
            PushFront(*it);
        }
     }

    SingleLinkedList(const SingleLinkedList& other) {
        SingleLinkedList<Type> reversed_buffer;
        for (const auto& item : other){
            reversed_buffer.PushFront(item);
        }
        SingleLinkedList<Type> direct_buffer;
        for (const auto& item : reversed_buffer){
            direct_buffer.PushFront(item);
        }
        swap(direct_buffer);
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (this != &rhs){
            auto buffer(rhs);
            swap(buffer);
        }
        return *this;
    }

 void swap(SingleLinkedList& other) noexcept {
        std::swap(this->head_.next_node, other.head_.next_node);
        std::swap(this->size_, other.size_);
    }
    
    ~SingleLinkedList() {
        Clear();
    }
    
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator begin() noexcept {
        Iterator it(head_.next_node);
        return it;
    }

    [[nodiscard]] Iterator end() noexcept {
        Iterator it(nullptr);
        return it;
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        ConstIterator it(head_.next_node);
        return it;
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        ConstIterator it(nullptr);
        return it;
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        ConstIterator it(head_.next_node);
        return it;
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        ConstIterator it(nullptr);
        return it;
    }
    
     [[nodiscard]] Iterator before_begin() noexcept {
        Iterator it(&head_);
        return it;
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        ConstIterator it(const_cast<Node*>(&head_));
        return it;
    }
    
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        ConstIterator it(const_cast<Node*>(&head_));
        return it;
    }

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0;
    }
    
    void PushFront(const Type& value){
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }
    
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        auto ptr = new Node(value, pos.node_->next_node);
        pos.node_->next_node = ptr;
        ++size_;
        return Iterator{ptr};
    }

    void PopFront() noexcept {
        auto element_to_delete_ptr = head_.next_node;
        head_.next_node = (*head_.next_node).next_node;
        delete element_to_delete_ptr;
        --size_;
    }


    Iterator EraseAfter(ConstIterator pos) noexcept {
        auto next = pos.node_->next_node;
        pos.node_->next_node = next->next_node;
        delete next;
        --size_;
        return Iterator{pos.node_->next_node};
    }
    
    void Clear() noexcept {
        while (head_.next_node != nullptr){
            Node* first = head_.next_node;
            head_.next_node = first->next_node;
            delete first;
            --size_;
        }
    }

private:

    Node head_ = {};
    size_t size_ = 0;
    };

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if (std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end())){
        return true;
    }
    else {
        return false;
    }
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if (!std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end())){
        return true;
    }
    else {
        return false;
    }
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if (std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end())){
        return true;
    }
    else{
        return false;
    }
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if (lhs < rhs || lhs == rhs){
        return true;
    }
    else {
        return false;
    }
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if (rhs < lhs){
        return true;
    }
    else {
        return false;
    }
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if (rhs < lhs || rhs == lhs){
        return true;
    }
    else {
        return false;
    }
}

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

