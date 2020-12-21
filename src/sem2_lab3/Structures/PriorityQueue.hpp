#ifndef PRIORITYQUEUE_HPP
#define PRIORITYQUEUE_HPP
#include "Tree.hpp"
#include "../../sem3_lab1/structures/List/_list.hpp"

using namespace std;

template<typename Key, typename Type>
class PriorityQueue {
private: /*
    class Priority {
    private:
        Key key_;
        Type* data_;

    public:
        Priority() : key_(), data_(new Type()) {}
        Priority(const Priority& other)
            : key_(other.key_),
              data_(new Type(*other.data_))
        {}
        Priority(Priority&& other) : Priority()
        {
            swap(key_, other.key_);
            swap(data_, other.data_);
        }

        Priority(const Key& key, Type*&& data)
            : key_(key), data_(data) {}


        Key& key() { return key_; }
        const Key& key() const { return key_; }

        Type& data() { return *data_; }
        const Type& data() const { return *data_; }


        const Priority& operator=(const Priority& other) {
            key_ = other.key_;
            data_ = new Type(*other.data_);
            return *this;
        }
        const Priority& operator=(Priority&& other) {
            swap(key_, other.key_);
            swap(data_, other.data_);
            return *this;
        }


        bool operator==(const Priority& other) const {
            if(key_ != other.key_)
                return false;
            return *data_ == *other.data_;
        }
        bool operator!=(const Priority& other) const { return !operator==(other); }

        bool operator<(const Priority& other) const {
            if(key_ >= other.key_)
                return false;
            return *data_ < *other.data_;
        }
        bool operator<=(const Priority& other) const {
            if(key_ > other.key_)
                return false;
            return *data_ <= *other.data_;
        }

        bool operator>(const Priority& other) const {
            if(key_ <= other.key_)
                return false;
            return *data_ > *other.data_;
        }
        bool operator>=(const Priority& other) const {
            if(key_ < other.key_)
                return false;
            return *data_ >= *other.data_;
        }


        ~Priority() { data_ = 0; };
    };
*/
    using Queue = PriorityQueue<Key, Type>;
    using List = List<Type>;

    struct Tree : public AVL_Tree<Key, List> {
        using _Tree = AVL_Tree<Key, List>;

        friend class PriorityQueue;

        Tree() : _Tree() {}
        Tree(const Tree& other) : _Tree((const _Tree&)other) {}
        Tree(Tree&& other) : _Tree((_Tree&&)other) {}


        const Tree& operator=(const Tree& other) {
            _Tree::operator=((const _Tree&) other);
            return *this;
        }
        const Tree& operator=(Tree&& other) {
            _Tree::operator=((_Tree&&)other);
            return *this;
        }
    };


public:
    enum PriorityType {
        Max,
        Min
    };


private:
    Tree tree;
    PriorityType type;
    long long elem_count;


public:
    PriorityQueue()
        : tree(Tree()),
          type(PriorityType::Max),
          elem_count(0)
    {};
    PriorityQueue(const Queue& other)
        : tree(other.tree),
          type(other.type),
          elem_count(other.elem_count)
    {};
    PriorityQueue(Queue&& other)
        : tree(forward<Tree&&>(other.tree)),
          type(other.type),
          elem_count(other.elem_count)
    {};

    PriorityQueue(const initializer_list<pair<Key, Type> >& list)
        : PriorityQueue()
    {
        auto iter = list.begin();
        auto last = list.end();

        for(; iter != last; ++iter)
            push((*iter).first, (*iter).second);
    }


    const Queue& operator=(const Queue& other) {
        type = other.type;
        tree = other.tree;
        elem_count = other.elem_count;
        return *this;
    };
    const Queue& operator=(Queue&& other) {
        type = other.type;
        tree = Tree(forward<Tree&&>(other.tree));
        elem_count = other.elem_count;
        return *this;
    };
    const Queue& operator=(const initializer_list<pair<Key, Type> >& list) {
        return *this = Queue(list);
    }


    const PriorityType& priority_type() const { return type; }
    void set_priority_type(const PriorityType& type_) { type = type_; }


    void push(const Key& priority, const Type& data) {
        push(Key(priority), Type(data));
    }
    void push(Key&& priority, Type&& data) {
        auto node = tree.find_node(tree.root, priority);

        if(node == 0) {
            List list = { data };
            tree.insert(forward<Key&&>(priority), list);
        }
        else {
            List& list = tree[priority];
            list.push_back(data);
        }

        ++elem_count;
    }


    const List& top() const {
        if(type == PriorityType::Max)
            return tree.max();
        return tree.min();
    };

    const List& bottom() const {
        if(type == PriorityType::Min)
            return tree.max();
        return tree.min();
    };

    const List& at(const Key& key) const { return tree[key]; };

    const Key& top_priority() const {
        if(type == PriorityType::Max)
            return tree.max_pair().first.key();
        return tree.min_pair().first.key();
    };
    const Key& bottom_priority() const {
        if(type == PriorityType::Min)
            return tree.max_pair().first.key();
        return tree.min_pair().first.key();
    };


    Type pop() {
        pair<Key, List> pair_ = tree.max_pair();
        List& list = tree.max();

        if(type == PriorityType::Min) {
            pair_ = tree.min_pair();
            list = tree.min();
        }

        Type result;

        if(type == PriorityType::Max)
            result = list.pop_back();
        else
            result = list.pop_front();

        if(list.empty())
            tree.remove(pair_.first);

        --elem_count;

        return result;
    };


    bool find_key(const Key& key) const { return tree.find_key(key); }


    void clear() { tree.clear(); elem_count = 0; };
    const int& key_count() const { return tree.size(); };
    const long long& element_count() const { return elem_count; };
    bool empty() const { return tree.empty(); };


    template<typename Function = void (*)(Type&), typename ...Args>
    void traverse(Function func, Args... args) {
        typename Tree::Traversal trav_t = type == PriorityType::Max ? Tree::RightRootLeft : Tree::LeftRootRight;

        tree.traverse(trav_t, [&func, &args...](const Key&, List& list) {
            auto iter = list.begin();
            auto last = list.end();

            for(; iter != last; ++iter)
                func(*iter, args...);
        });
    }
    template<typename Function = void (*)(const Type&), typename ...Args>
    void traverse(Function func, Args... args) const {
        typename Tree::Traversal trav_t = type == PriorityType::Max ? Tree::RightRootLeft : Tree::LeftRootRight;

        tree.traverse(trav_t, [&func, &args...](const Key&, const List& list) {
            auto iter = list.begin();
            auto last = list.end();

            for(; iter != last; ++iter)
                func(*iter, args...);
        });
    }

    template<typename Function = void (*)(const Key&, Type&), typename ...Args>
    void traverse_with_keys(Function func, Args... args) {
        typename Tree::Traversal trav_t = type == PriorityType::Max ? Tree::RightRootLeft : Tree::LeftRootRight;

        tree.traverse(trav_t, [&func, &args...](const Key& key, List& list) {
            auto iter = list.begin();
            auto last = list.end();

            for(; iter != last; ++iter)
                func(key, *iter, args...);
        });
    }
    template<typename Function = void (*)(const Key&, const Type&), typename ...Args>
    void traverse_with_keys(Function func, Args... args) const {
        typename Tree::Traversal trav_t = type == PriorityType::Max ? Tree::RightRootLeft : Tree::LeftRootRight;

        tree.traverse(trav_t, [&func, &args...](const Key& key, const List& list) {
            auto iter = list.begin();
            auto last = list.end();

            for(; iter != last; ++iter)
                func(key, *iter, args...);
        });
    }


    template<typename MapFunction = Type (*)(const Type&), typename ...Args>
    Queue map(MapFunction func, Args... args) const {
        Queue result = Queue(*this);

        result.traverse([&func, &args...](const Key&, Type& value) {
            value = func(value, args...);
        });

        return result;
    };

    template<typename WhereFunction = bool (*)(const Type&), typename ...Args>
    Queue where(WhereFunction func, Args... args) const {
        Queue result = Queue();

        traverse([&result, &func, &args...](const Key& priority, const Type& value) {
            if(func(value, args...))
                result.push(priority, value);
        });

        return result;
    };

    template<typename ReduceFunction = Type (*)(const Type&, const Type&), typename ...Args>
    Type Reduce(ReduceFunction func, const Type& init_value = Type(), Args... args) {
        Type result = init_value;

        traverse([&func, &result, &args...](const Key&, const Type& value) {
            result = func(result, value, args...);
        });

        return result;
    };


    friend std::ostream& operator<<(std::ostream& out, const Queue& queue) {
        if(queue.empty())
            out << "{  }";
        else {
            out << "{";
            Key last_key = Key();

            queue.traverse_with_keys([&out, &last_key](const Key& key, const Type& value){
                if(key != last_key) {
                    out << '\n' << "  " << key << ":  ";
                    last_key = key;
                };
                out << value << ' ';
            });

            out << "\n}";
        }

        return out;
    }
};

#endif // PRIORITYQUEUE_HPP
