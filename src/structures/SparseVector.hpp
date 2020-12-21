#ifndef SPARSEVECTOR_HPP
#define SPARSEVECTOR_HPP
#include "../sem2_lab3/Structures/Tree.hpp"
#include "../sem3_lab1/structures/Sequences/_sequence.hpp"

template<typename value>
class SparseVector : private AVL_Tree<long long, value> {
private:
    using Tree = AVL_Tree<long long, value>;
    using Node = TreeNode<long long, value>;
    using Vector = SparseVector<value>;
    using Sequence = Sequence<value>;

    long long _size = 0;


public:
    using Pair = pair<long long, value>;

    SparseVector()
        : Tree(), _size(0) {};
    SparseVector(const Vector& other)
        : Tree((const Tree&)other), _size(other._size) {};
    SparseVector(Vector&& other)
        : Tree((Tree&&)other), _size(other._size) {};

    /// all elements will be initialized empty
    SparseVector(const long long& size)
        : Tree(), _size(size) {};

    SparseVector(const Sequence* sequence, const value& val_to_ignore = value())
        : SparseVector(sequence->begin(), sequence->end(), sequence->size(),val_to_ignore) {}
    SparseVector(const Sequence& sequence, const value& val_to_ignore = value())
        : SparseVector(sequence.begin(), sequence.end(), sequence.size(), val_to_ignore) {};

    SparseVector(const value* data, const long long& size, const value& val_to_ignore = value())
        : SparseVector(data, data + size, size, val_to_ignore) {};
    SparseVector(const std::initializer_list<value>& list, const long long& size = -1,
                 const value& val_to_ignore = value())
        : SparseVector(list.begin(), list.end(), size, val_to_ignore) {};

    template<typename ForwardIterator>
    SparseVector(const ForwardIterator& from, const ForwardIterator& to,
                 const long long& size = -1, const value& val_to_ignore = value())
        : SparseVector(size)
    {
        auto iter = from;
        long long index = 0;

        for(; iter != to; ++iter, ++index) {
            if(*iter != val_to_ignore)
                Tree::insert(index, *iter);
        }

        if(index > _size)
            _size = index;
    }


    const long long& size() const { return _size; };
    const int& filled_size() const { return Tree::size(); };
    double fill_percent() const { return double(Tree::size()) / _size; };


    bool empty() const { return Tree::empty(); };
    bool find_index(const long long& index) const { return Tree::find_key(index); };
    bool find_value(const value& val) const { return Tree::find_value(val); };


    long long find(const value& val) const {
        Node* node = this->find_node_value(val);

        return node == 0 ? -1 : node->get_key();
    }


    void increase_size(const long long& amount) {
        if(amount < 0)
            throw logic_error("sparse vector exception: increasing size by negative amount");
        _size += amount;
    };
    void clear() { Tree::clear(); };


    void remove_index(const long long& index) {
        if(index >= size())
            throw tree_out_of_range;
        if(this->find_key(index))
            this->remove(index);
        if(index == size() - 1)
            --_size;
    }
    void remove_value(const value& val) {
        Node* node = this->find_node_value(val);
        if(node != 0)
            this->remove_index(node->get_key());
    }


    /// is only applied to non-zero elements
    template<typename Function = void (*)(const long long&, const value&), typename ...Args>
    void traverse(Function func, Args... args) const {
        Tree::traverse(Tree::LeftRootRight, func, args...);
    };
    template<typename Function = void (*)(const long long&, value&), typename ...Args>
    void traverse(Function func, Args... args) {
        Tree::traverse(Tree::LeftRootRight, func, args...);
    };


    value operator[](const long long& index) const {
        if(index >= _size)
            throw tree_out_of_range;

        if(!Tree::find_key(index))
            return value();

        return Tree::operator[](index);
    };
    value& operator[](const long long& index) {
        if(index >= _size)
            throw tree_out_of_range;

        if(!Tree::find_key(index))
            Tree::insert(index, value());

        return Tree::operator[](index);
    };


    value operator()(const long long& index) const { return operator[](index); };
    value& operator()(const long long& index) { return operator[](index); };


    const SparseVector& operator()(const Sequence* sequence, const value& ignore_val = value()) {
        return *this = SparseVector(sequence, ignore_val);
    };
    const SparseVector& operator()(const Sequence& sequence, const value& ignore_val = value()) {
        return *this = SparseVector(sequence, ignore_val);
    };


    const SparseVector& operator=(const Vector& other) {
        clear();
        Tree::operator=((const Tree&)other);
        _size = other._size;
        return *this;
    };
    const SparseVector& operator=(Vector&& other) {
        clear();
        Tree::operator=((Tree&&)other);
        _size = other._size;
        return *this;
    };
    const SparseVector& operator=(const std::initializer_list<value>& list) {
        clear();
        return *this = SparseVector(list);
    };
    const SparseVector& operator=(const Sequence* sequence) {
        clear();
        return *this = SparseVector(sequence);
    };
    const SparseVector& operator=(const Sequence& sequence) {
        clear();
        return *this = SparseVector(sequence);
    };


    friend std::ostream& operator<<(std::ostream& out, const Vector& vector) {
        vector.traverse([&out](const long long&, const int& data) {
            out << data << ' ';
        });
        return out;
    };


    Vector subvector(const long long& from, const long long& to) const {
        Vector result = where([&from, &to](const long long& index, const value&){
            if(index >= from && index <= to)
                return true;
            return false;
        });
        result._size = to - from + 1;
        return result;
    }

    void concate(const Vector& other) {
        *this = std::move(get_concated(other));
    }
    Vector get_concated(const Vector& other) const {
        Vector result = Vector(*this);
        const long long old_size = this->size();
        other.traverse([&result, &old_size](const long long& index, const value& val){
            result.insert(index + old_size, val);
        });
        result._size = old_size + other.size();

        return result;
    }


    template<typename ReduceFunc = value (*)(const value&, const value&), typename ...Args>
    value reduce(ReduceFunc func, const value& init_value = value(), Args... args) const {
        value result = init_value;

        traverse([&result, &func, &args...](const long long&, const value& val) {
            result = func(result, val, args...);
        });

        return result;
    };

    template<typename WhereFunc = bool (*)(const long long&, const value&, ...), typename ...Args>
    Vector where(WhereFunc func, Args... args) const {
        Vector result = Vector();
        long long first_index = -1;
        long long last_index = -1;

        traverse([&first_index, &last_index, &result, &func, &args...](const long long& index, const value& val) {
            if(func(index, val, args...)) {
                result.insert(index, val);

                if(first_index == -1)
                    first_index = index;
                if(last_index < index)
                    last_index = index;
            }
        });
        result._size = 0;
        if(first_index != -1)
            result._size = last_index - first_index + 1;

        return result;
    };

    template<typename MapFunc = value (*)(const value&), typename ...Args>
    Vector map(MapFunc func, Args... args) const {
        Vector result = *this;

        result.traverse([&func, &args...](const long long&, const value& val) {
            val = func(val, args...);
        });

        return result;
    };
};

#endif // SPARSEVECTOR_HPP
