#ifndef AVLT_ITERATOR_HPP
#define AVLT_ITERATOR_HPP
#include "Node2.hpp"

template<typename Key, typename Type>
class AVLT_Iterator {
public:
    using value_type = Type;
    using reference = Type&;
    using const_reference = const Type&;
    using pointer = Node<Key, Type>*;
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = int;

private:
    using Node = Node<Key, Type>;

    pointer data_ = 0;

public:
    AVLT_Iterator() : data_(0) {};
    AVLT_Iterator(const AVLT_Iterator& other) : data_(other.data_) {};
    AVLT_Iterator(AVLT_Iterator&& other) {
        std::swap(data_, other.data_);
    };
    AVLT_Iterator(pointer data) : data_(data) {};



    const AVLT_Iterator& operator=(const AVLT_Iterator& other) {
        data_ = other.data_;
        return *this;
    };
    const AVLT_Iterator& operator=(AVLT_Iterator&& other) {
        std::swap(data_, other.data_);
        return *this;
    };

    friend std::ostream& operator<<(std::ostream& out, const AVLT_Iterator&);

    ~AVLT_Iterator() {
        data_ = 0;
        delete data_;
    };
};

#endif // AVLT_ITERATOR_HPP
