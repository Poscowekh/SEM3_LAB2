#ifndef SET_HPP
#define SET_HPP
#include "SortedSequence.hpp"

template<typename value>
class Set : public SortedSequence<value> {
protected:
    using _Sequence = SortedSequence<value>;
    using _Sequence::push;
    using _Sequence::pop_back;
    using _Sequence::pop_front;
    using _Sequence::subsequence;

    template<typename _Iterator>
    void _push_range(const _Iterator& from, const _Iterator& to) {
        auto iter = from;

        while(iter != to) {
            if(!this->find(*iter))
                this->push(*iter);
            ++iter;
        };
    };

public:
    using typename _Sequence::iterator;
    using typename _Sequence::const_iterator;
    using typename _Sequence::const_reference;
    using typename _Sequence::const_pointer;
    using typename _Sequence::pointer;

    Set() : _Sequence() {};
    Set(const Set& other) : _Sequence(other) {};
    Set(Set&& other) : _Sequence(other) {};

    Set(const std::initializer_list<value>& list)
        : Set(list.begin(), list.end()) {};
    Set(const value* data, const int& count)
        : Set(data, data + count) {};

    /*template<typename ForwardIterator = const_pointer,
             typename = typename std::enable_if_t<!std::is_class_v<ForwardIterator> &&
                                                  (std::is_same_v<ForwardIterator, const_pointer> ||
                                                  std::is_same_v<ForwardIterator, pointer>) > >
    /// push a range initialized by two pointers
    /// removing duplicates
    Set(const ForwardIterator& from, const ForwardIterator& to)
        : Set() { _push_range(from, to); };

    template<class ForwardIterator = const_iterator,
             typename = typename std::enable_if_t<std::is_class_v<ForwardIterator> &&
                                                  (std::is_convertible_v<typename ForwardIterator::value_type, value > ||
                                                  std::is_constructible_v<value, typename ForwardIterator::value_type>) > >
    /// push a range initialized by two iterators
    /// removing duplicates
    Set(const ForwardIterator& from, const ForwardIterator& to)
        : Set() { _push_range(from, to); };*/

    template<typename ForwardIterator = const_iterator>
    Set(const ForwardIterator& from, const ForwardIterator& to)
        : Set() { _push_range(from, to); };

    /// ignores duplicates
    void add(const_reference data) {
        if(!this->find(data))
            push(data);
    };/*
    template<typename ForwardIterator = const_pointer,
             typename = typename std::enable_if_t<!std::is_class_v<ForwardIterator> >,
             typename = typename std::enable_if_t<std::is_same_v<ForwardIterator, const_pointer> ||
                                                  std::is_same_v<ForwardIterator, pointer> > >
    /// ignores duplicates
    void add(const ForwardIterator& from, const ForwardIterator& to) { push(from, to); };
    template<typename ForwardIterator = const_iterator,
             typename = typename std::enable_if_t<std::is_class_v<ForwardIterator> >,
             typename = typename std::enable_if_t<std::is_convertible_v<typename ForwardIterator::value_type, value> > >
    /// ignores duplicates
    void add(const ForwardIterator& from, const ForwardIterator& to) { push(from, to); }; */
    template<typename ForwardIterator = const_iterator>
    void add(const ForwardIterator& from, const ForwardIterator& to) { _push_range(from, to); };


    const Set& operator=(const Set& other) {
        this->clear();
        ((_Sequence*)this)->operator=((const _Sequence&)other);
        return *this;
    };
    const Set& operator=(Set&& other) {
        ((_Sequence*)this)->operator=(std::forward<_Sequence>(other));
        return *this;
    };
    const Set& operator=(const std::initializer_list<value>& list) {
        _push_range(list.begin(), list.end());
        return *this;
    };


    void operator+=(const_reference data) {
        add(data);
    };
    void operator+=(const Set& other) {
        add(other.begin(), other.end());
    };

    void operator-=(const_reference data) {
        if(find(data))
            remove(data);
    };
    void operator-=(const Set& other) {
        auto iter = other.begin(),
             last = other.end();

        while (iter != last) {
            if(this->find(*iter))
                this->remove(*iter);
            ++iter;
        };
    };

    Set operator+(const_reference data) const {
        Set result = Set(*this);
        result += data;
        return result;
    };
    Set operator+(const Set& other) const {
        Set result = Set(*this);
        result += other;
        return result;
    };

    Set operator-(const_reference data) const {
        Set result = Set(*this);
        result -= data;
        return result;
    };
    Set operator-(const Set& other) const {
        Set result = Set(*this);
        result -= other;
        return result;
    };

    void operator*=(const Set& other) {
        *this = std::move(*this * other);
    };
    void operator*=(const_reference data) {
        *this = std::move(*this * data);
    };

    Set operator*(const Set& other) const {
        Set result = Set();

        if(this->size() >= other.size()) {
            auto iter = other.begin(),
                 last = other.end();

            while(iter != last) {
                if(this->find(*iter))
                    result.add(*iter);
                ++iter;
            };
        } else {
            auto iter = this->begin(),
                 last = this->end();

            while(iter != last) {
                if(other.find(*iter))
                    result.add(*iter);
                ++iter;
            };
        };

        return result;
    };
    Set operator*(const_reference data) const {
        Set result = Set();

        for(auto element : *this)
            if(element == data) {
                result.add(data);
                break;
            };

        return result;
    };

    Set subset(const const_iterator& from, const const_iterator& to) const {
        return Set(from, to);
    };
    Set subset(const int& from, const int& to) const {
        return subset(this->at(from), this->at(to + 1));
    };


    bool operator==(const Set& other) const {
        if(this->size() != other.size())
            return false;

        auto iter1 = this->begin();
        auto iter2 = other.begin();
        auto last = this->end();

        for(; iter1 != last; ++iter1, ++iter2)
            if(*iter1 != *iter2)
                return false;

        return true;
    }
    bool operator!=(const Set& other) const { return !operator==(other); }

    bool operator<(const Set& other) const {
        auto iter1 = this->begin();
        auto iter2 = other.begin();
        auto last = (this->size() > other.size() ? this->end() : other.end());

        for(; iter1 != last && iter2 != last; ++iter1, ++iter2)
            if(*iter1 > *iter2)
                return false;

        return this->size() < other.size();
    }
    bool operator<=(const Set& other) const {
        auto iter1 = this->begin();
        auto iter2 = other.begin();
        auto last = (this->size() > other.size() ? this->end() : other.end());

        for(; iter1 != last && iter2 != last; ++iter1, ++iter2)
            if(*iter1 > *iter2)
                return false;

        return this->size() <= other.size();
    }

    bool operator>(const Set& other) const {
        auto iter1 = this->begin();
        auto iter2 = other.begin();
        auto last = (this->size() > other.size() ? this->end() : other.end());

        for(; iter1 != last && iter2 != last; ++iter1, ++iter2)
            if(*iter1 < *iter2)
                return false;

        return this->size() > other.size();
    }
    bool operator>=(const Set& other) const {
        auto iter1 = this->begin();
        auto iter2 = other.begin();
        auto last = (this->size() > other.size() ? this->end() : other.end());

        for(; iter1 != last && iter2 != last; ++iter1, ++iter2)
            if(*iter1 < *iter2)
                return false;

        return this->size() >= other.size();
    }


    friend std::ostream& operator<<(std::ostream& out, const Set<value>& set) {
        return out << (const SortedSequence<value>&)set;
    }
};

#endif // SET_HPP
