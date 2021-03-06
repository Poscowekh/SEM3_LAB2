#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "list_iterator.hpp"
#include "../InterfaceIterator.hpp"

template<typename T> class List{
private:
    using InitializerFunction = T (*)(const int index);
    //using InitializerLambda = [](const int index) -> T;
    using list_iter = ListIterator<T>;
    using c_list_iter = const ListIterator<T>;
    using iterator = Iterator<T>;
    using base_iter = BaseIterator<T>*;
    using const_iterator = const Iterator<T>;
    using cbase_iter = const BaseIterator<T>*;

    using Node = Node<T>;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using value = T;

    Node* _head;
    Node* _tail;
    int _size;

    /*Node* _end_iterator_dummy() const{
        return new Node(0, _tail, 0);
    };*/

    Node* _get_node(const int index) const{
        if(index > _size - 1 || index < 0)
            throw std::runtime_error("\nList exception: index out of range");
        if(index > _size / 2)
            return _from__tail(index);
        return _from__head(index);
    };
    Node* _from__head(const int index) const{
        Node* tmp = _head;
        for(int i = 0; i < index; ++i)
            tmp = tmp->next;
        return tmp;
    };
    Node* _from__tail(int index) const{
        Node* tmp = _tail;
        for(int i = _size; i > index + 1; --i)
            tmp = tmp->prev;
        return tmp;
    };

    reference _get_data(const int index){
        return _get_node(index)->data;
    };
    const_reference _get_data(const int index) const{
        return _get_node(index)->data;
    };

public:
    List() : _size(0), _head(0), _tail(0) {};
    List(const List* other) : List() {
        Node* tmp = other->_head;
        while(tmp != 0){
            push_back(tmp->data);
            tmp = tmp->next;
        };
    };
    List(const List& other) : List(&other) {};
    List(List&& other) : _head(other._head), _tail(other._tail), _size(other._size) {
        other._head = other._tail = 0;
    };

    List(const_pointer data, const int count, const int from = 0) : List() {
        if(data != 0)
            for(int i = from; i < count + from; ++i)
                push_back(data + i);
    };
    List(const int size, const_reference default_member = T()) : List() {
        for(int i = 0; i < size; ++i)
            push_back(&default_member);
    };
    List(const int size, const_pointer default_member) : List(size, *default_member) {};
    List(const int size, InitializerFunction func) : List() {
        for(int i = 0; i < size; ++i)
            push_back(func(i));
    };
    List(const std::initializer_list<T>& list) : List() {
        auto iter = list.begin();
        auto last = list.end();
        for(; iter != last; ++iter)
            push_back(*iter);
    };
    List(iterator from, iterator to) : List() {
        auto iter = from;
        while(iter != to){
            push_back(*iter);
            ++iter;
        };
    };

    void push_front(const_pointer data, const int count = 1){
        for(int i = 0; i < count; ++i){
            Node* tmp = new Node(data + i, 0, _head);
            if(_size == 0)
                _tail = tmp;
            else
                _head->prev = tmp;
            _head = tmp;
        };
        _size += count;
    };
    void push_front(const_reference data){
        push_front(&data);
    };
    void push_back(const_pointer data, const int count = 1){
        for(int i = 0; i < count; ++i){
            Node* tmp = new Node(data + i, _tail, 0);
            if(_size == 0)
                _head = tmp;
            else
                _tail->next = tmp;
            _tail = tmp;
        };
        _size += count;
    };
    void push_back(const_reference data){
        push_back(&data);
    };

    reference front(){
        return _get_data(0);
    };
    value front() const{
        return _get_data(0);
    };
    reference get(const int index){
        return _get_data(index);
    };
    value get(const int index) const{
        return _get_data(index);
    };
    reference back(){
        return _get_data(_size - 1);
    };
    value back() const{
        return _get_data(_size - 1);
    };

    void set(const int index, const_pointer data){
        _get_node(index)->data = *data;
    };
    void set(const int index, const_reference data){
        set(index, &data);
    };

    void expand(const int count){
        for(int i = 0; i < count; ++i)
            push_back(T());
    };

    int size() const{
        return _size;
    };
    bool empty() const{
        if(_size > 0)
            return false;
        return true;
    };
    void clear(){
        pop_back(_size);
        _head = _tail = 0;
        _size = 0;
    };
    /*void print(const int members_in_line = -1, bool show_indexes = false, bool add_ws = true) const
    //prints the list int []-brackets, if show_indexes=true then prints evvery member as index:member
    {
        std::cout << to_string(members_in_line, show_indexes, add_ws);
    };*/

    value pop_back(const int count = 1){
        value result;
        for(int i = 0; i < count; ++i){
            if(i == count - 1)
                result = T(back());
            if(_size > 1){
                _tail->prev->next = 0;
                _tail = _tail->prev;
            } else {
                free(_tail);
                _head = _tail = 0;
            };
            --_size;
        };
        return result;
    };
    value pop_front(const int count = 1){
        value result;
        for(int i = 0; i < count; ++i){
            if(i == count - 1)
                result = T(back());
            if(_size > 1){
                _head->next->prev = 0;
                _head = _head->next;
            } else {
                free(_head);
                _head = _tail = 0;
            };
            --_size;
        };
        return result;
    };

    void insert(const_pointer data, const int index){
        if(index == 0)
            push_front(data);
        else if(index == _size)
            push_back(data);
        else if(index < _size){
            Node* new_node = new Node(data, 0, _get_node(index));
            new_node->prev = new_node->next->prev;
            new_node->next->prev->next = new_node;
            new_node->next->prev = new_node;
            ++_size;
        };
    };
    void insert(const_reference data, const int index){
        insert(&data, index);
    };
    void remove(const int index){
        if(index == 0)
            pop_front();
        else if(index == _size - 1)
            pop_back();
        else{
            Node* tmp = _get_node(index);
            tmp->next->prev = tmp->prev;
            tmp->prev->next = tmp->next;
            delete tmp;
            --_size;
        };
    };

    List sublist(const int from, const int to) const{
        List result = List();
        auto iter = at(from),
             last = at(to);
        ++last;
        while(iter != last){
            result.push_back(*iter);
            ++iter;
        };
        return result;
    };

    void concate(const List* other){
        auto iter = other->begin(),
             last = other->end();
        while(iter != last) {
            push_back(*iter);
            ++iter;
        };
    };
    void concate(const List& other){
        concate(&other);
    };
    List get_concated(const List* other){
        List result = List(*this);
        result.concate(other);
        return result;
    };
    List get_concated(const List& other){
        return get_concated(&other);
    };

    List get_copy() const{
        return List(this);
    };

    reference operator[](const int index){
        //allows push_back!
        /*if(index == _size)
            push_back(T());*/
        return get(index);
    };
    value operator[](const int index) const{
        return get(index);
    };

    List operator+(const List& other) const{
        List result = List(this);
        if(this == &other){
            List copy = List(this);
            result.concate(&copy);
        } else
            result.concate(&other);
        return result;
    };
    List& operator+=(const List& other){
        if(this != &other)
            concate(&other);
        else{
            List copy = List(this);
            concate(&copy);
        };
        return *this;
    };
    List operator+(const_reference data) const{
        List result = List(*this);
        result.push_back(&data);
        return result;
    };
    List& operator+=(const_reference data){
        push_back(&data);
        return *this;
    };

    List operator++(){
        List result = List(this);
        push_back(T());
        return result;
    };
    List operator++(const int count){
        List result = List(this);
        for(int i = 0; i < count; ++i)
            push_back(T());
        return result;
    };

    bool operator==(const List& list) const{
        if(_size == 0 && list._size == 0)
            return true;
        if(_size != list._size)
            return false;
        Node* tmp1 = _head;
        Node* tmp2 = list._head;
        for(int i = 0; i < _size; ++i){
            if(*(tmp1->data) != *(tmp2->data))
                return false;
            tmp1 = tmp1->next;
            tmp2 = tmp2->next;
        };
        return true;
    };
    bool operator!=(const List& list) const{
        return !(*this == list);
    };

    List& operator=(const List& other){
        if(this != &other){
            if(&other == 0)
                *this = List();
            else{
                if(this == 0){
                    _size = 0;
                    _head = _tail = 0;
                } else
                    clear();
                Node* tmp = other._head;
                while(tmp != 0){
                    push_back(tmp->data);
                    tmp = tmp->next;
                };
            };
        };
        return *this;
    };/*
    List& operator=(List&& other){
        if(&other != 0) {
            *_head = *other._head;
            *_tail = *other._tail;
            _size = other._size;
        };
        other._head = other._tail = 0;
        return *this;
    };*/
    List& operator=(const std::initializer_list<T>& list){
        return *this = List(list);
    };

    /*std::string to_string(const int members_in_line = -1, bool show_indexes = false, bool add_ws = false) const{
        if(members_in_line == -1)
            return to_string(_size, show_indexes, add_ws);
        std::string result = " [ ";
        if(_size == 0)
            return result + ']';
        else {
            for(int i = 0; i < _size - 1; ++i){
                std::string tmp;
                if(show_indexes)
                    tmp += i + ':';
                tmp += get(i);
                tmp += ", ";
                result += tmp;
                if(members_in_line != 1){
                    if(i != 0 && i % members_in_line == members_in_line - 1)
                        result += "\n   ";
                } else
                    result += "\n   ";
            };
            if(show_indexes)
                result += std::to_string(_size - 1) + ':';
            result += back();
            result += " ]";
            if(add_ws)
                result += '\n';
        };
        return result;
    };
    explicit operator std::string() const{
        return to_string(1, true);
    };*/
    friend std::ostream& operator<<(std::ostream& out, const List& list){
        if(list.empty())
            out << "[  ]";
        else {
            out << "[ ";

            auto iter = list.begin();
            value last_val = list.back();

            while (*iter != last_val) {
                out << *iter << ", ";

                ++iter;
            }

            out << last_val << " ]";
        };

        return out;
    };

    friend List operator+(const_reference value, const List& list){
        List result = List(list);
        result.push_front(value);
        return result;
    };

    /*friend std::istream& operator>>(std::istream& in, List& list){
        list = List();
        int size = 0;
        in >> size;
        for(int i = 0; i < size; i++){
            T tmp = T();
            in >> tmp;
            list.push_back(tmp);
        };
        return in;
    };*/

    int find_index(const_reference data){
        auto tmp = _head;
        int index = 0;
        while(tmp != 0){
            ++index;
            if(tmp->_data == data)
                return index;
        };
        return -1;
    };
    bool find(const_reference data){
        if(find_index(data) >= 0)
            return true;
        return false;
    };

    iterator begin(){
        return iterator(dynamic_cast<base_iter>(new list_iter(_head)));
    };
    const_iterator begin() const{
        return cbegin();
    };
    const_iterator cbegin() const{
        return const_iterator(dynamic_cast<cbase_iter>(new c_list_iter(_head)));
    };

    iterator at(const int index){
        if(index >= _size)
            return end();
        return iterator(dynamic_cast<base_iter>(new list_iter(_get_node(index))));
    };
    const_iterator at(const int index) const{
        return cat(index);
    };
    const_iterator cat(const int index) const{
        if(index >= _size)
            return cend();
        return const_iterator(dynamic_cast<cbase_iter>(new c_list_iter(_get_node(index))));
    };

    iterator end(){
        return iterator(dynamic_cast<base_iter>(new list_iter(_tail->next)));
    };
    const_iterator end() const{
        return cend();
    };
    const_iterator cend() const{
        return const_iterator(dynamic_cast<cbase_iter>(new c_list_iter(_tail->next)));
    };

    ~List(){
        clear();
    };
};

#endif // LINKEDLIST_H
