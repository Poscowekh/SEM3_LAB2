#ifndef BINARYTREE_HPP
#define BINARYTREE_HPP
#include <utility>

template<typename value>
class BinaryTree {
protected:
    struct Node {
        value data;

        Node* right;
        Node* left;
        Node* parent;


        Node()
            : data(value()),
              right(0),
              left(0),
              parent(0)
        {}
        Node(const Node& other)
            : data(other.data),
              right(new Node(*other.right)),
              left(new Node(*other.left)),
              parent(new Node(*other.parent))
        {}
        Node(Node&& other)
            : data(other.data),
              right(other.right),
              left(other.left),
              parent(other.parent)
        {}
        Node(const value& data_, Node* right_ = 0, Node* left_ = 0, Node* parent_ = 0)
            : data(data_),
              right(right_),
              left(left_),
              parent(parent_)
        {}


        const Node& operator=(const Node& other) {
            data = other.data;
            right = other.right;
            left = other.left;
            parent = other.parent;
            return *this;
        }
        const Node& operator=(Node&& other) {
            data = other.data;
            right = other.right;
            left = other.left;
            parent = other.parent;
            return *this;
        }


        ~Node() {
            delete right;
            delete left;
            parent = 0;
        }
    };

    long long size_;
    Node* root;

    using Tree = BinaryTree;

    Node* search(value&& data) const {

    }

public:
    BinaryTree() : size_(0), root(new Node()) {}
    BinaryTree(const Tree& other)
        : size_(other.size_),
          root(new Node(*other.root))
    {}
    BinaryTree(Tree&& other)
        : BinaryTree()
    {
        swap(root, other.root);
        swap(size_, other.size_);
    }


    BinaryTree(const std::initializer_list<value>& list)
        : BinaryTree(list.begin(), list.end())
    {}

    template<typename ForwardIter>
    BinaryTree(const ForwardIter& from, const ForwardIter& to)
        : BinaryTree()
    {
        auto iter = from;

        while (iter != to) {

            ++iter;
        }
    }


    const Tree& operator=(const Tree& other) {
        size_ = other.size_;
        root = new Node(*other.root);
        return *this;
    }
    const Tree& operator=(Tree&& other) {
        swap(root, other.root);
        swap(size_, other.size_);
        return *this;
    }
    const Tree& operator=(const std::initializer_list<value>& list) {
        return *this = Tree(list);
    }


    void clear() {
        size_ = 0;
        delete root;
        root = new Node();
    }
    const long long& size() const { return size_; }
    bool empty() const { return size_ == 0; }


    void insert(const value& data) { insert(value(data)); }
    void insert(value&& data) {

    }


    ~BinaryTree() { clear(); }
};

#endif // BINARYTREE_HPP
