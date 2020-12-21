#ifndef SORTEDSEQUENCE_HPP
#define SORTEDSEQUENCE_HPP
#include "../sem3_lab1/Structures.hpp"
#include "../sem3_lab1/Sorts.hpp"
#include "exception_list.hpp"

/// A sequence_ of sorted values class
/// Unless specifically defined SequenceType will be ListSequence
template<typename value>
class SortedSequence {
public:
    using reference = value&;
    using const_reference = const value&;
    using pointer = value*;
    using const_pointer = const value*;
    using List = List<value>;
    using Array = Array<value>;
    using Sequence = Sequence<value>;
    using initializer_list = std::initializer_list<value>;
    using iterator = typename Sequence::iterator;
    using const_iterator = typename Sequence::const_iterator;

private:
    Sequence* sequence_ = 0;

    void _sort() {
        /// seems to be the fastest
        _merge_sort(sequence_);
    };

    /// checks whether sequence was initialized and creates a new list sequence to hold data if not
    void _check_init_seq() {
        if(sequence_ == 0)
            sequence_ = new ListSequence<value>();
        else
            sequence_->clear();
    };

protected:
    // searches data and returns its position or -1
    int _bin_find(const_reference data, iterator& iter,
                    const int& left, const int& right) const
    {
        if(right >= left) {
            int middle = left + (right - left) / 2;
            for(int i = 0; i < middle - left; ++i, ++iter);

            if(*iter == data)
                return middle;
            else if(*iter < data)
                return _bin_find(data, ++iter, middle + 1, right);
            else {
                for(int i = 0; i < middle - left; ++i, --iter);
                // iter is back at the left element
                return _bin_find(data, iter, left, middle - 1);
            };
        };

        return -1;
    };

    /// searches place for data
    /// right iter is valid last iter (not end)
    int _bin_search(const_reference data, iterator& iter,
                    const int& left, const int& right) const
    {
        if(right <= left)
            return (data >= *iter) ? left : left;

        int middle = (left + right) / 2;
        //auto middle_iter = left_iter; //sequence_->at(left + middle);
        for(int i = 0; i < middle - left; ++i, ++iter);

        if(data == *iter)
            return middle;
        else if(data > *iter)
            return _bin_search(data, ++iter, middle + 1, right);
        else {
            for(int i = 0; i < middle - left; ++i, --iter);
            return _bin_search(data, iter, left, middle);
        };
    };

    /// inserts into sequence keeping order
    void _insert(const_reference data) {
        if(sequence_->size() == 0 || sequence_->front() >= data)
            sequence_->prepend(data);
        else if(sequence_->back() <= data)
            sequence_->append(data);
        else {
            iterator iter = ++sequence_->begin();
            sequence_->insert_at(data, _bin_search(data, iter, 1, size() - 2));
        };
    };
    /// inserts into sequence withous order checks
    void _unsafe_insert(const int& at, const_reference data) {
        sequence_->insert_at(data, at);
    };

    iterator _begin() const {
        return sequence_->begin();
    };
    iterator _at(const int& index) const {
        return sequence_->at(index);
    };
    iterator _end() const {
        return sequence_->end();
    };

public:
    SortedSequence() : sequence_(new ListSequence<value>()) {};
    SortedSequence(const SortedSequence& other)
        : sequence_(other.sequence_->get_copy()) {};
    SortedSequence(const SortedSequence* other)
        : SortedSequence(*other) {};
    SortedSequence(SortedSequence&& other) {
        std::swap(sequence_, other.sequence_);
    };

    /*SortedSequence(const int& size_, const_reference default_member = value())
        : sequence_(new ListSequence<value>(size_, default_member)) {
        _sort();
    };
    SortedSequence(const int& size_, const_pointer default_member)
        : SortedSequence(size_, *default_member) {};*/
    SortedSequence(const_pointer data, const int& count)
        : SortedSequence() {
        const_pointer data_ = data;

        for(int i = 0; i < count; ++i, data_ += 1)
            _insert(*(data + i));
    };

    SortedSequence(const Sequence& sequence)
        : SortedSequence(sequence.begin(), sequence.end()) {};
    SortedSequence(const Sequence* sequence)
        : SortedSequence(sequence->begin(), sequence->end()) {};

    SortedSequence(initializer_list list)
        : SortedSequence(list.begin(), list.end()) {};

    /*
    /// ForwardIterator must support prefix increment and dereferencing!
    template<typename ForwardIterator = const_pointer,
             typename = typename std::enable_if_t<!std::is_class_v<ForwardIterator> >,
             typename = typename std::enable_if_t<std::is_same_v<ForwardIterator, const_pointer> ||
                                                  std::is_same_v<ForwardIterator, pointer> > >
    SortedSequence(const ForwardIterator& from, const ForwardIterator& to)
        : SortedSequence() {
        auto iter = from,
             last = to;

        while (iter != last) {
            _insert(*iter);
            ++iter;
        };
    };
    template<typename ForwardIterator = const_iterator,
             typename = typename std::enable_if_t<std::is_class_v<ForwardIterator> ||
                                                  std::is_convertible_v<typename ForwardIterator::value_type, value> > >
    SortedSequence(const ForwardIterator& from, const ForwardIterator& to)
        : SortedSequence() {
        auto iter = from,
             last = to;

        while (iter != last) {
            _insert(*iter);
            ++iter;
        };
    };*/
    template<typename ForwardIterator = const_iterator>
    SortedSequence(const ForwardIterator& from, const ForwardIterator& to) : SortedSequence() {
        auto iter = from,
             last = to;

        while (iter != last) {
            _insert(*iter);
            ++iter;
        };
    };

    void clear() {
        sequence_->clear();
    };
    bool empty() const {
        return sequence_->empty();
    };
    int size() const {
        return sequence_->size();
    };

    /// inserts data keeping sequence sorted
    void push(const_reference data) {
        _insert(data);
    };
    template<typename ForwardIterator = const_iterator/*,
             typename = typename std::enable_if<std::is_convertible<typename ForwardIterator::value_type, value>::value> */>
    void push(const ForwardIterator& from, const ForwardIterator& to) {
        auto iter = from,
             last = to;

        while(iter != last) {
            push(*iter);
        };
    };

    /// removes last element and returns its copy
    value pop_back() {
        value result = back();
        sequence_->remove_at(size() - 1);

        return result;
    };

    /// removes first element and returns its copy
    value pop_front() {
        value result = front();
        sequence_->remove_at(0);

        return result;
    };

    const_reference front() const {
        return sequence_->front();
    };
    const_reference get(const int& index) const {
        return sequence_->get(index);
    };
    const_reference back() const {
        return sequence_->back();
    };

    const_iterator begin() const {
        return cbegin();
    };
    const_iterator cbegin() const {
        return sequence_->cbegin();
    };

    const_iterator at(const int& index) const {
        return cat(index);
    };
    const_iterator cat(const int& index) const {
        return sequence_->cat(index);
    };

    const_iterator end() const {
        return cend();
    };
    const_iterator cend() const {
        return sequence_->cend();
    };

    const_reference operator[](const int& index) const {
        return get(index);
    };

    const SortedSequence& operator=(const Sequence& sequence) {
        _check_init_seq();
        clear();

        auto iter = sequence.begin(),
             last = sequence.end();

        while (iter != last) {
            _insert(*iter);
            ++iter;
        };

        return *this;
    };
    const SortedSequence& operator=(const Sequence* sequence) {
        return *this = *sequence;
    };
    const SortedSequence& operator=(const SortedSequence& other) {
        return *this = other.sequence_;
    };
    const SortedSequence& operator=(SortedSequence&& other) {
        _check_init_seq();
        std::swap(sequence_, other.sequence_);
        return *this;
    };
    /// works faster with presorted data
    const SortedSequence& operator=(initializer_list list) {
        return *this = SortedSequence(list);
    };

    friend std::ostream& operator<<(std::ostream& out, const SortedSequence& sequence) {
        return out << sequence.sequence_;
    };

    void remove_at(const int& index) {
        sequence_->remove_at(index);
    };
    void remove(const_reference data) {
        int index = find_index(data);
        if(index != -1)
            sequence_->remove_at(index);
    };

    bool find(const_reference data) const {
        return find_index(data) != -1;
    };
    int find_index(const_reference data) const {
        if(size() == 0)
            return -1;

        auto _iter = sequence_->begin();
        return _bin_find(data, _iter, 0, size() - 1);
    };


    SortedSequence subsequence(const const_iterator& from, const const_iterator& to) const {
        return SortedSequence(from, to);
    };
    SortedSequence subsequence(const int& from, const int& to) const {
        return SortedSequence(at(from), at(to + 1));
    };


    void concate(const SortedSequence& other) {
        push(other.begin(), other.end());
    }
    SortedSequence get_concated(const SortedSequence& other) const {
        if(other.size() > size())
            return other.get_concated(*this);

        SortedSequence result = SortedSequence(*this);
        result.push(other.begin(), other.end());

        return result;
    }


    SortedSequence operator+(const SortedSequence& other) const {
        return get_concated(other);
    }
    void operator+=(const SortedSequence& other) {
        concate(other);
    }


    bool operator==(const SortedSequence& other) const {
        return *sequence_ == *other.sequence_;
    };
    bool operator!=(const SortedSequence& other) const {
        return !(*this == other);
    };


    ~SortedSequence() {
        delete sequence_;
    };
};

#endif // SORTEDSEQUENCE_HPP
