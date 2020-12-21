#ifndef BSTREE_HPP
#define BSTREE_HPP
#include "Node2.hpp"

template<typename Key, typename Type>
class AVL_Tree {
private:
    using Node = TreeNode<Key, Type>;
    using _Pair = std::pair<Key, Type>;

public:
    using Traversal = void (&)(Node*& a, Node*& b, Node*& c);
    static void LeftRightRoot(Node*& a, Node*& b, Node*& c) { std::swap(b, c); }
    static void RightLeftRoot(Node*& a, Node*& b, Node*& c) { std::swap(a, c); std::swap(b, c); }
    static void LeftRootRight(Node*& a, Node*& b, Node*& c) {};
    static void RightRootLeft(Node*& a, Node*& b, Node*& c) { std::swap(a, c); }
    static void RootLeftRight(Node*& a, Node*& b, Node*& c) { std::swap(a, b); }
    static void RootRightLeft(Node*& a, Node*& b, Node*& c) { std::swap(b, c); std::swap(a, c); }

    //using iterator = ;
    //using const_iterator = ;

protected:
    Node* root = 0;
    int size_ = 0;

    inline void _check_except_null_ptr(Node* node) const {
        if(node == 0)
            throw node_pointer_null;
    };
    inline void _check_except_empty() const {
        if(root == 0)
            throw tree_empty;
    };

    Node* find_node(Node* parent, const Key& key) const {
        if(parent == 0)
            return 0;

        if(key == parent->get_key())
            return parent;
        if(key < parent->get_key())
            return find_node(parent->get_left(), key);
        if(key > parent->get_key())
            return find_node(parent->get_right(), key);

        return 0;
    };
    Node* find_min_subnode(Node* parent) const {
        _check_except_null_ptr(parent);
        return parent->find_min_subnode();
    };
    Node* find_max_subnode(Node *parent) const {
        _check_except_null_ptr(parent);
        return parent->find_max_subnode();
    };

    Node* find_node_value(const Type& value) const { return find_node_value(root, value); }
    Node* find_node_value(Node* parent, const Type& value) const {
        if(parent == 0)
            return 0;

        if(value == parent->get_data())
            return parent;

        Node* node = find_node_value(parent->get_left(), value);
        return (node == 0 ? find_node_value(parent->get_right(), value) : 0);
    };

    Node* get_node(Node* parent, const Key& key) const {
        _check_except_null_ptr(parent);
        return parent->find_node(key);
    };

    Node* insert_node(Node* parent, Node&& node) {
        using std::forward;

        if(parent == 0)
            return new Node(forward<Node&&>(node));
        if(parent->get_key() == node.get_key())
            throw node_key_exists;
        if(node.get_key() < parent->get_key())
            parent->get_left() = insert_node(parent->get_left(), forward<Node&&>(node));
        else
            parent->get_right() = insert_node(parent->get_right(), forward<Node&&>(node));

        return balance_node(parent);
    };

    Node* remove_node(Node* parent, const Key& key) {
        _check_except_null_ptr(parent);

        if(key < parent->get_key())
            parent->get_left() = remove_node(parent->get_left(), key);
        else if(key > parent->get_key())
            parent->get_right() = remove_node(parent->get_right(), key);
        else {
            Node* left = parent->get_left();
            Node* right = parent->get_right();

            parent->get_left() = parent->get_right() = 0;
            delete parent;

            if(right == 0)
                return left;

            Node* min = find_min_subnode(right);
            min->get_right() = remove_min_node(right);
            min->get_left() = left;

            return balance_node(min);
        }

        return balance_node(parent);
    };
    Node* remove_min_node(Node* parent) {
        if(parent->get_left() == 0)
            return parent->get_right();

        parent->get_left() = remove_min_node(parent->get_left());
        return balance_node(parent);
    };

    int node_height_diff(Node* parent) const {
        int left = 0, right = 0;
        if(parent->get_left() != 0)
            left = parent->get_left()->max_height();
        if(parent->get_right() != 0)
            right = parent->get_right()->max_height();
        return right - left;
    };

    Node* rotate_right(Node* parent) {
        Node* tmp = parent->get_left();

        parent->get_left() = tmp->get_right();
        tmp->get_right() = parent;

        return tmp;
    };
    Node* rotate_left(Node* parent) {
        Node* tmp = parent->get_right();

        parent->get_right() = tmp->get_left();
        tmp->get_left() = parent;

        return tmp;
    };

    Node* balance_node(Node* parent) {
        if(node_height_diff(parent) == 2) {
            if(node_height_diff(parent->get_right()) < 0)
                parent->get_right() = rotate_right(parent->get_right());
            parent = rotate_left(parent);
        }
        else if(node_height_diff(parent) == -2) {
            if(node_height_diff(parent->get_left()) > 0)
                parent->get_left() = rotate_left(parent->get_left());
            parent = rotate_right(parent);
        };

        return parent;
    };

    template<typename Func, typename ...Args>
    void _traverse(Node* node, Node* parent, Traversal type, Func func, Args... args) {
        if(node == 0)
            return;

        if(node == parent) {
            func(node->get_key(), node->get_data(), args...);
        }
        else {
            Node
                    *left = node->get_left(),
                    *centre = node,
                    *right = node->get_right();

            type(left, centre, right);

            _traverse(left, node, type, func, args...);
            _traverse(centre, node, type, func, args...);
            _traverse(right, node, type, func, args...);
        };
    };
    template<typename Func, typename ...Args>
    void _traverse(Node* node, Node* parent, Traversal type, Func func, Args... args) const {
        if(node == 0)
            return;

        if(node == parent) {
            func(node->get_key(), node->get_data(), args...);
        }
        else {
            Node
                    *left = node->get_left(),
                    *centre = node,
                    *right = node->get_right();

            type(left, centre, right);

            _traverse(left, node, type, func, args...);
            _traverse(centre, node, type, func, args...);
            _traverse(right, node, type, func, args...);
        };
    };

public:
    AVL_Tree() : root(0), size_(0) {};
    AVL_Tree(const AVL_Tree& other) : size_(0) {
        root = other.root == 0 ? 0 : new Node(other.root);
    };
    AVL_Tree(const AVL_Tree* other) : AVL_Tree(*other) {};
    AVL_Tree(AVL_Tree&& other) : AVL_Tree() {
        std::swap(root, other.root);
        std::swap(size_, other.size_);
    };

    AVL_Tree(const std::initializer_list<_Pair>& list)
        : AVL_Tree(list.begin(), list.end()) {};
    template<typename ForwardIterator,
             typename std::enable_if<std::is_convertible<
                            typename ForwardIterator::value_type, _Pair>::value > >
    AVL_Tree(const ForwardIterator& from, const ForwardIterator& to) : AVL_Tree() {
        insert(from, to);
    };

    const AVL_Tree& operator=(const AVL_Tree& other) {
        delete root;
        root = new Node(other.root);
        size_ = other.size_;

        return *this;
    };
    const AVL_Tree& operator=(AVL_Tree&& other) {
        std::swap(root, other.root);
        std::swap(size_, other.size_);

        return *this;
    };
    const AVL_Tree& operator=(const std::initializer_list<_Pair>& list) {
        delete root;

        insert(list.begin(), list.end());

        return *this;
    };

    ~AVL_Tree() {
        delete root;
    };

    const int& size() const { return size_; };
    int height() const { return root ? root->max_height() : 0; };

    void clear() {
        delete root;
        size_ = 0;
    };

    bool empty() const { return size_ == 0; };
    bool find_key(const Key& key) const {
        return root ? root->find(key) : false;
    };
    bool find_value(const Type& data) const {
        return root->find_data(data);
    };

    void insert(Node&& node) {
        root = insert_node(root, std::forward<Node&&>(node));
        ++size_;
    };
    void insert(const Key& key, const Type& data) {
      insert(std::forward<Node&&>(Node(key, data)));
    };
    void insert(Key&& key, Type&& data) {
      insert(std::forward<Node&&>(Node(std::forward<Key&&>(key), std::forward<Type&&>(data))));
    };
    void insert(const _Pair& pair) {
        insert(std::forward<Node&&>(Node(pair)));
    };
    void insert(_Pair&& pair) {
        insert(std::forward<Node&&>(Node(std::forward<_Pair&&>(pair))));
    };
    template<typename ForwardIterator,
             typename std::enable_if<std::is_convertible<
                            typename ForwardIterator::value_type, _Pair>::value > >
    void insert(const ForwardIterator& from, const ForwardIterator& to) {
        auto iter = from,
             last = to;

        while(iter != last) {
            insert(*iter);
            ++iter;
        };
    };

    Type& get(const Key& key) {
        _check_except_empty();
        return root->get(key);
    };
    const Type& get(const Key& key) const {
        _check_except_empty();
        return root->get(key);
    };

    Type& operator[](const Key& key) {
        Node* node = find_node(root, key);
        if(node == 0)
            insert(key, Type());

        return get(key);
    };
    const Type& operator[](const Key& key) const {
        return get(key);
    };

    void remove(const Key& key) {
        _check_except_empty();
        root = remove_node(root, key);
        --size_;
    };
    void remove_data(const Type& data) {
        _check_except_empty();
        remove(root->find_node_data(data)->get_key());
    };

    AVL_Tree subtree(const Key& key) const {
        Node* parent = find_node(root, key);
        AVL_Tree result = AVL_Tree();
        result.root = new Node(parent);
        result.size_ = result.root->size();

        return result;
    };

    template<typename Function, typename ...Arguments>
    void traverse(Traversal traversal, Function function, Arguments... args) {
        _traverse(root, 0, traversal, function, args...);
    };
    template<typename Function, typename ...Arguments>
    void traverse(Traversal traversal, Function function, Arguments... args) const {
        _traverse(root, 0, traversal, function, args...);
    };

    std::string to_string(Traversal traversal) const {
        std::string result = std::string();
        Key last_key = Key();

        if(traversal == LeftRightRoot || traversal == RightLeftRoot)
            last_key = root->get_key();
        else if(traversal == LeftRootRight || traversal == RootLeftRight)
            last_key = root->find_max_subnode()->get_key();
        else
            last_key = root->find_min_subnode()->get_key();

        traverse(traversal, [&result, &last_key](const Key& key, const Type& data){
            result += "(" + key + ": " + data + ")";

            if(key != last_key)
                result += ",";
            result += " ";
        });

        return result;
    };
    operator std::string() const {
        return to_string(LeftRootRight);
    };

    //iterator begin(Traversal traversal = LeftRootRight) {};
    //iterator begin(Traversal traversal = LeftRootRight) const {
    //    return cbegin(traversal);
    //};
    //const_iterator cbegin(Traversal traversa = LeftRootRightl) const {};

    //iterator at(const Key& key, Traversal traversa = LeftRootRightl) {};
    //iterator at(const Key& key, Traversal traversal = LeftRootRight) const {
    //    return cat(key, traversal);
    //};
    //const_iterator cbegin(const Key& key, Traversal traversal = LeftRootRight) const {};

    //iterator end(Traversal traversal = LeftRootRight) {};
    //iterator end(Traversal traversal = LeftRootRight) const {
    //    return cend(traversal);
    //};
    //const_iterator cend(Traversal traversal = LeftRootRight) const {};

    const Type& top() const {
        _check_except_empty();
        return root->get_data();
    };
    Type& top() {
        _check_except_empty();
        return root->get_data();
    };

    const Type& min() const {
        _check_except_empty();
        return find_min_subnode(root)->get_data();
    };
    Type& min() {
        _check_except_empty();
        return find_min_subnode(root)->get_data();
    };

    const Type& max() const {
        _check_except_empty();
        return find_max_subnode(root)->get_data();
    };
    Type& max() {
        _check_except_empty();
        return find_max_subnode(root)->get_data();
    };

    _Pair min_pair() const {
        return (_Pair)(*find_min_subnode(root));
    };
    _Pair max_pair() const {
        return (_Pair)(*find_max_subnode(root));
    };

    friend std::ostream& operator<<(std::ostream& out, const AVL_Tree& tree) {
        out << "{ ";

        if(!tree.empty()) {
            auto last_key = tree.find_max_subnode(tree.root)->get_key();

            tree.traverse(LeftRootRight, [&out, &last_key](const Key& key, const Type& data){
                out << '(' << key << ": " << data << ')';
                if(key != last_key)
                    out << ", ";
            });
        };

        out << " }";

        return out;
    };
};

#endif // BSTREE_HPP
