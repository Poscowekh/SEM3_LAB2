#ifndef TreeNode_HPP
#define TreeNode_HPP
#include "exceptions.hpp"

/// Binary Search Tree TreeNode
template<typename Key, typename Type>
class TreeNode {
public:
    using _Pair = std::pair<Key, Type>;

private:
    void _insert_left(TreeNode&& node) {
        if(left == 0)
            left = new TreeNode(std::forward(node));
        else
            left->insert(std::forward(node));
    };
    void _insert_right(TreeNode* node) {
        if(right == 0)
            right = new TreeNode(std::forward(node));
        else
            right->insert(std::forward(node));
    };

    Type data = Type();
    Key key = Key();
    TreeNode* left = 0;
    TreeNode* right = 0;

    void _init_children(TreeNode* left_, TreeNode* right_) {
        left = left_ == 0 ? 0 : new TreeNode(left_);
        right = right_ == 0 ? 0 : new TreeNode(right_);
    };

public:
    TreeNode() : data(Type()), key(Key()), left(0), right(0) {};
    TreeNode(const TreeNode* other) : data(other->data), key(other->key) {
        _init_children(other->left, other->right);
    };
    TreeNode(const TreeNode& other) : TreeNode(&other) {};
    TreeNode(TreeNode&& other)
        : data(forward<Type&&>(other.data)),
          key(forward<Key&&>(other.key)),
          left(0), right(0)
    {
        std::swap(left, other.left);
        std::swap(right, other.right);
    };

    TreeNode(const Key& key_, const Type& data_, TreeNode* left_ = 0, TreeNode* right_ = 0)
        : data(data_), key(key_)
    {
        _init_children(left_, right_);
    };
    TreeNode(Key&& key_, Type&& data_, TreeNode* left_ = 0, TreeNode* right_ = 0)
        : data(std::forward<Type&&>(data_)), key(std::forward<Key&&>(key_))
    {
        _init_children(left_, right_);
    };

    TreeNode(const _Pair& pair, TreeNode* left_ = 0, TreeNode* right_ = 0)
        : TreeNode(std::forward(pair.first),
                   std::forward(pair.second),
                   left_, right_)
    {};
    TreeNode(_Pair&& pair, TreeNode* left_ = 0, TreeNode* right_ = 0)
        : TreeNode(std::forward(pair.first),
                   std::forward(pair.second),
                   left_, right_)
    {};

    int size() const {
        int result = 1;

        if(left != 0)
            result += left->Size();
        if(right != 0)
            result += right->Size();

        return result;
    };
    int max_height() const {
        int result = 1;

        result += std::max(left == 0 ? 0 : left->max_height(),
                           right == 0 ? 0 : right->max_height());

        return result;
    };

    Key& get_key() { return key; };
    const Key& get_key() const { return key; };

    Type& get_data() { return data; };
    const Type& get_data() const { return data; };

    TreeNode*& get_left() { return left; };
    const TreeNode*& get_left() const { return left; };

    TreeNode*& get_right() { return right; };
    const TreeNode*& get_right() const { return right; };

    const Type& get(const Key& key_) const {
        return find_node(key_)->get_data();
    };
    Type& get(const Key& key_) {
        return find_node(key_)->get_data();
    };

    TreeNode* find_node(const Key& key_) {
        if(key_ == key)
            return this;

        if(key_ < key)
            return left == 0 ? 0 : left->find_node(key_);
        else
            return right == 0 ? 0 : right->find_node(key_);
    };
    TreeNode* find_parent(const Key& key_) {
        if(key_ == key)
            throw node_parent_unreachable;

        if(key_ < key) {
            if(left == 0)
                throw node_key_invalid;

            if(key_ == left->get_key())
                return this;
            return left->find_parent(key_);
        } else {
            if(right == 0)
                throw node_key_invalid;

            if(key_ == right->get_key())
                return this;
            return right->find_parent(key_);
        };
    };
    TreeNode* find_node_data(const Type& data_) {
        if(data == data_)
            return this;

        TreeNode* result = 0;
        if(left != 0)
            result = left->find_node_data(data_);
        if(result == 0 && right != 0)
            result = right->find_node_data(data_);

        return result;
    };

    bool find(const Key& key_) {
        return find_node(key_) == 0 ? false : true;
    };
    bool find_data(const Type& data_) {
        if(find_node_data(data_) == 0)
            return false;
        return true;
    };

    void remove_node(TreeNode* parent, Key key_) {
        if(parent == 0)
            throw tree_empty;

        if(this == parent) {
            if(left != 0)
                left->remove_node(this, key_);
            if(right != 0)
                right->remove_node(this, key_);
            return;
        };

        if(key > key_)
            left->remove_node(this, key_);
        else if(key < key_)
            right->remove_node(this, key_);
        else {
            if(left == 0 && right == 0) {
                delete this;
                return;
            } else if(left == 0 && right != 0) {
                    if(parent->left == this)
                        parent->left = right;
                    else
                        parent->right = right;

                    delete this;
                    return;
            } else if(right == 0) {
                    if(parent->left == this)
                        parent->left = left;
                    else
                        parent->right = left;

                    delete this;
                    return;
            } else {
                if(right->left == 0) {
                    TreeNode* delete_ = right;

                    data = right->get_data();
                    key = right->get_key();
                    right = right->get_right();

                    delete_->get_right() == 0;
                    delete delete_;
                } else {
                    TreeNode* tmp = right->find_min_subnode();

                    data = tmp->data;
                    key = tmp->key;

                    right->remove_node(right, key);
                };
            };
        };
    };

    void insert(TreeNode&& node) {
        if(node.get_key() == key)
            throw node_key_exists;

        if(node.get_key() < key)
            _insert_left(std::forward(node));
        else
            _insert_right(std::forward(node));
    };
    void insert(const TreeNode& node) {
        insert(std::forward(node));
    };
    void insert(TreeNode* node) {
        insert(*node);
    };
    void insert(const Key& key_, const Type& data_) {
        insert(std::move(TreeNode(key_, data_)));
    };
    void insert(Key&& key_, Type&& data_) {
        insert(std::move(TreeNode(std::forward(key_), std::forward(data_))));
    };
    void insert(const _Pair& pair) {
        insert(std::move(TreeNode(pair)));
    };
    void insert(_Pair&& pair) {
        insert(std::move(TreeNode(std::forward(pair))));
    };

    const TreeNode& operator=(const TreeNode& other) {
        if(this != & other) {
            data = other.get_data();
            key = other.get_key();

            delete left;
            left = new TreeNode(other.get_left());
            delete left;
            right = new TreeNode(other.get_right());
        };

        return *this;
    };
    const TreeNode& operator=(TreeNode* other) {
        return *this = *other;
    };
    const TreeNode& operator=(TreeNode&& other) {
        using std::swap;

        data = std::forward<Type&&>(other.data);
        key = std::forward<Key&&>(other.key);
        swap(left, other.left);
        swap(right, other.right);

        return *this;
    };

    const TreeNode& operator=(const _Pair& pair) {
        key = pair.first;
        data = pair.second;

        return *this;
    };
    const TreeNode& operator=(_Pair&& pair) {
        key = std::forward(pair.first);
        data = std::forward(pair.second);

        return *this;
    };

    bool operator==(const TreeNode& other) const {
        if(data != other.get_data())
            return false;
        if(key != other.get_key())
            return false;

        if((left == 0 && other.get_left() != 0) || (left != 0 && other.get_left() == 0))
            return false;
        else if(left != 0 && other.get_left() != 0)
            if(*left != *other.get_left())
                return false;

        if((right == 0 && other.get_right() != 0) || (right != 0 && other.get_right() == 0))
            return false;
        else if(right != 0 && other.get_right() != 0)
            if(*right != *other.get_right())
                return false;

        return true;
    };
    bool operator!=(const TreeNode& other) const {
        return !(*this == other);
    };

    bool operator==(const _Pair& pair) const {
        if(key != pair.first)
            return false;
        if(data != pair.second)
            return false;

        return true;
    };
    bool operator!=(const _Pair& pair) const {
        return !(*this == pair);
    };

    TreeNode* find_min_subnode() {
        return left ? left->find_min_subnode() : this;
    };
    TreeNode* find_max_subnode() {
        return right ? right->find_max_subnode() : this;
    };

    operator _Pair() const {
        return std::make_pair(key, data);
    };

    ~TreeNode() {
        delete left;
        delete right;
    };
};

#endif // TreeNode_HPP
