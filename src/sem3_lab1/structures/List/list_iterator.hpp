#ifndef LIST_ITERATOR_HPP
#define LIST_ITERATOR_HPP
#include "../_libs.hpp"
#include "Node.hpp"

template<typename value>
class ListIterator : public BaseIterator<value>{
private:
    //friend class List;
    using base_iterator = BaseIterator<value>*;
    using reference = value&;
    using const_reference = const value&;
    using pointer = value*;
    using const_pointer = const value*;
    using node = Node<value>*;
    using const_node = const Node<value>*;

    node _node;

    const_pointer cptr() const{
        if(_is_end())
            throw std::runtime_error("\nList iterator exception: dereferencing end iterator");
        return &_node->data;
    };

    reference ref(){
        if(_is_end())
            throw std::runtime_error("\nList iterator exception: dereferencing end iterator");
        return _node->data;
    };
    const_reference cref() const{
        if(_is_end())
            throw std::runtime_error("\nList iterator exception: dereferencing end iterator");
        return _node->data;
    };

    value val() const{
        if(_is_end())
            throw std::runtime_error("\nList iterator exception: dereferencing end iterator");
        return _node->data;
    };

    bool _is_end() const{
        if(_node == 0)
            return true;
        return false;
    };

    ListIterator* _copy() const{
        return new ListIterator(this);
    };

public:
    ListIterator() = delete;
    ListIterator(node other_node) : _node(other_node) {};
    ListIterator(const_node other_node) : _node(const_cast<node>(other_node)) {};
    ListIterator(const ListIterator* other) : _node(other->_node) {};
    ListIterator(const ListIterator& other) : _node(other._node) {};
    ListIterator(ListIterator&& other) : _node(other._node) {
        other._node = 0;
    };

    const ListIterator& operator=(const ListIterator& other) {
        _node = other._node;
        return *this;
    };
    const ListIterator& operator=(ListIterator&& other) {
        _node = other._node;
        other._node = 0;
        return *this;
    };
    base_iterator operator=(base_iterator other) {
        dynamic_cast<BaseIterator<value>&>(*this) = other;
        _node = other->cptr();
        return dynamic_cast<base_iterator>(this);
    };

    base_iterator operator++(){
        if(!_is_end())
            _node = _node->next;

        return dynamic_cast<base_iterator>(this);
    };/*
    base_iterator operator++(int){
        if(_is_end())
            throw std::runtime_error("\nList iterator exception: incrementing end iterator");
        base_iterator copy = dynamic_cast<base_iterator>(new ListIterator(this));
        _node = _node->next;
        return copy;
    };*/

    base_iterator operator--(){
        if(!_is_end())
            _node = _node->prev;

        return dynamic_cast<base_iterator>(this);
    };/*
    base_iterator operator--(int){
        if(_is_end())
            throw std::runtime_error("\nList iterator exception: decrementing to null-pointing iterator");
        base_iterator copy = dynamic_cast<base_iterator>(new ListIterator(this));
        _node = _node->prev;
        return copy;
    };*/

    base_iterator copy() const{
        return dynamic_cast<base_iterator>(new ListIterator(this));
    };

    reference operator*(){
        return ref();
    };
    const_reference operator*() const{
        return cref();
    };

    const_pointer operator->() const{
        return cptr();
    };

    base_iterator operator+(const int val) const{
        ListIterator* result = _copy();
        (*result) += val;
        return dynamic_cast<base_iterator>(result);
    };
    base_iterator operator-(const int val) const{
         ListIterator* result = _copy();
         (*result) -= val;
         return dynamic_cast<base_iterator>(result);
    };

    void operator+=(const int val){
        for(int i = 0; i < val; ++i, ++(*this));
    };
    void operator-=(const int val){
        for(int i = 0; i < val; ++i, --(*this));
    };

    int operator-(const ListIterator& other) const{
        if(_is_end() && other._is_end())
            return 0;
        if(other._is_end())
            return other - *this;

        int distance = 0;
        ListIterator copy = other;
        while(!copy._is_end() || copy != *this){
            ++copy;
            distance++;
        };
        if(copy == *this)
            return distance;
        else {
            distance = 0;
            copy = other;
            while(!copy._is_end() || copy != *this){
                --copy;
                distance++;
            };
        };
        if(copy == *this)
            return distance;
        else
            throw std::runtime_error("\nIterator exception: evaluating distance between list iterators unsuccessful");
    };

    int operator-(const BaseIterator<value>& other) const override{
        if(typeid(*this) != typeid(other))
            throw std::runtime_error("\nIterator exception: evaluating distance between iterators of different types");
        return *this - dynamic_cast<const ListIterator&>(other);
    };
};

#endif // LIST_ITERATOR_HPP
