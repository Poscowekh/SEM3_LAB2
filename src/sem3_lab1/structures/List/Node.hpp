#ifndef NODE_HPP
#define NODE_HPP

template<typename T>
struct Node{
    T data = T();
    Node* prev = 0;
    Node* next = 0;

    Node() : data(T()), prev(0), next(0) {};
    Node(const T* _data, Node* _prev, Node* _next) : data(T(*_data)), prev(_prev), next(_next) {};
    Node(const T& _data, Node* _prev, Node* _next) : Node(&_data, _prev, _next) {};
    Node(const Node* other) : data(T(*other->data)), prev(0), next(0) {};
    Node(const Node& other) : Node(&other) {};

    Node& operator=(const Node& other){
        if(this != &other)
            *this = Node(&other);
        return *this;
    };

    ~Node() {
        prev = next = 0;
    };
};

#endif // NODE_HPP
