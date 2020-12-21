#ifndef HISTOGRAM_HPP
#define HISTOGRAM_HPP
#include "structures/Set.hpp"
#include "sem3_lab1/Structures.hpp"
#include "sem2_lab3/Structures/Tree.hpp"

template<typename value, typename parameter>
using ParamFunc = parameter (*)(const value&);
template<typename value, typename parameter = value>
parameter DefParamFunc(const value& val) { return val; }


/// helper class
template<typename value>
class Histogram : private AVL_Tree<value, std::size_t> {
private:
    using Node = TreeNode<value, std::size_t>;

    template<typename Iter>
    std::size_t count(const Iter& from, const Iter& to) const {
        auto iter = from;
        std::size_t result = 0;

        for(; iter != to; ++iter)
            result += operator[](*iter);

        return result;
    }

    size_t all_elems;

public:
    Histogram() = default;
    Histogram(const Histogram&) = default;
    Histogram(Histogram&&) = default;


    Histogram& operator=(const Histogram&) = default;
    Histogram& operator=(Histogram&&) = default;


    void increase_group(const value& val) {
        increase_group(val, 1);
    }
    void increase_group(const value& val, const std::size_t& amount) {
        /*Node* node = this->find_node(this->root, val);

        if(node == 0)
            this->insert(val, amount);
        else
            node->get_data() += amount;*/

        if(this->find_key(val))
            this->get(val) += amount;
        else
            this->insert(val, amount);

        all_elems += amount;
    }


    const int& group_count() const { return this->size(); }
    const size_t& all_elements() const { return all_elems; }


    void clear() { this->clear(); }
    bool empty() const { return this->empty(); }
    bool find(const value& val) const { return this->find_key(val); }


    std::size_t operator[](const value& val) const {
        Node* node = this->find_node(this->root, val);

        if(node == 0)
            return 0;
        else
            return node->get_data();
    }
    std::size_t operator[](const Set<value>& values) const {
        return count(values.begin(), values.end());
    }
    std::size_t operator[](const Sequence<value>* values) const {
        return count(values->begin(), values->end());
    }
    std::size_t operator[](const std::initializer_list<value>& values) const {
        return count(values.begin(), values.end());
    }


    friend std::ostream& operator<<(std::ostream& out, const Histogram& hist) {
        return out << (const AVL_Tree<value, std::size_t>&)hist;
    }


    template<typename Function = void (*)(const value&, const std::size_t&, ...), typename ...Args>
    void traverse(Function func, Args... args) const {
        AVL_Tree<value, std::size_t>::template traverse<Function, Args...>(this->LeftRootRight, func, args...);
    };

    using ReduceFunction = value (*)(const value& init_val, const value& val, const std::size_t& count, ...);
    template<typename ...Args>
    value reduce(const value& init_val, ReduceFunction func, Args... args) const {
        value result = init_val;

        traverse([&result, &func, &args...](const value& val, const std::size_t& count) {
            result = func(result, val, count, args...);
        });

        return result;
    }

    using WhereFunction = bool (*)(const value&, const std::size_t&, ...);
    template<typename ...Args>
    Histogram<value> where(WhereFunction func, Args... args) const {
        Histogram<value> result = Histogram<value>();

        traverse([&result, &func, &args...](const value& val, const std::size_t& count) {
            if(func(val, count, args...))
                result.increase_group(val, count);
        });

        return result;
    }

    using MapFunction = value (*)(const value&, const std::size_t&, ...);
    template<typename ...Args>
    Histogram<value> map(MapFunction func, Args... args) const {
        Histogram<value> result = Histogram<value>(*this);

        result.traverse([&func, &args...](value& val, const std::size_t& count) {
            val = func(val, count, args...);
        });

        return result;
    }
};


template<typename value, typename Iter, typename Param = value>
Histogram<value> histogram_elementary(const Iter& from, const Iter& to,
                                      ParamFunc<value, Param> param_func = DefParamFunc<value>)
{
    Histogram<value> result = Histogram<value>();
    auto iter = from;

    for(; iter != to; ++iter)
        result.increase_group(*iter);

    return result;
}
template<typename value, typename Param = value>
Histogram<value> histogram_elementary(const Sequence<value>* sequence, ParamFunc<value, Param> param_func = DefParamFunc<value>)
{
    return histogram_elementary<value>(sequence->begin(), sequence->end(), param_func);
}
template<typename value, typename Param = value>
Histogram<value> histogram_elementary(const SortedSequence<value>& sequence, ParamFunc<value, Param> param_func = DefParamFunc<value>)
{
    return histogram_elementary<value>(sequence.begin(), sequence.end(), param_func);
}
template<typename value, typename Param = value>
Histogram<value> histogram_elementary(const std::initializer_list<value>& list, ParamFunc<value, Param> param_func = DefParamFunc<value>)
{
    return histogram_elementary<value>(list.begin(), list.end(), param_func);
}


template<typename value, typename Iter, typename Param = value>
Histogram<Set<value> > histogram_by_count(const Iter& from, const Iter& to, const std::size_t& count,
                                          ParamFunc<value, Param> param_func = DefParamFunc<value>) {
    Histogram<Set<value> > result = Histogram<Set<value> >();
    Histogram<value> elementary = histogram_elementary<value>(from, to, param_func);

    const std::size_t group_size = elementary.group_count() / count == 0 ? 1 : elementary.group_count() / count;
    //const value& last_elem = elementary.top();
    std::size_t elem_count = 0,
                group_count = 0;
    Set<value> group = Set<value>();

    elementary.traverse([&result, &group_size, &elem_count, &group, &param_func, &count, &group_count]
                        (const value& val, const std::size_t& size)
    {
        if(group.size() == group_size && group_count != count - 1) {
            result.increase_group(group, elem_count);
            elem_count = 0;
            group = Set<value>();
            ++group_count;
        }

        group.add(val);
        elem_count += size;
    });

    result.increase_group(group, elem_count);

    return result;
}
template<typename value, typename Param = value>
Histogram<Set<value> > histogram_by_count(const Sequence<value>* sequence, const std::size_t& group_count,
                                          ParamFunc<value, Param> param_func = DefParamFunc<value>)
{
    return histogram_by_count<value>(sequence->begin(), sequence->end(), group_count, param_func);
}
template<typename value, typename Param = value>
Histogram<Set<value> > histogram_by_count(const SortedSequence<value>& sequence, const std::size_t& group_count,
                                          ParamFunc<value, Param> param_func = DefParamFunc<value>)
{
    return histogram_by_count<value>(sequence.begin(), sequence.end(), group_count, param_func);
}
template<typename value, typename Param = value>
Histogram<Set<value> > histogram_by_count(const std::initializer_list<value>& list, const std::size_t& group_count,
                                          ParamFunc<value, Param> param_func = DefParamFunc<value>)
{
    return histogram_by_count<value>(list.begin(), list.end(), group_count, param_func);
}


template<typename value, typename Iter, typename Param = value>
Histogram<Set<value> > histogram_by_groups(const Iter& from, const Iter& to, const Sequence< Set<Param> >* groups,
                                           ParamFunc<value, Param> param_func = DefParamFunc<value>) {
    Histogram<Set<value> > result = Histogram<Set<value> >();
    Histogram<value> elementary = histogram_elementary<value>(from, to);

    elementary.traverse([&result, &groups, &param_func](const value& val, const std::size_t& count) {
        auto group_iter = groups->begin();
        auto group_last = groups->end();

        /// through sequence of groups
        while (group_iter != group_last) {
            auto iter = (*group_iter).begin();
            auto last = (*group_iter).end();

            // through each group
            while (iter != last) {
                if(param_func(val) == param_func(*iter)) {
                    result.increase_group(*group_iter, count);
                    break;
                }

                ++iter;
            }

            ++group_iter;
        }
    });

    return result;
}
template<typename value, typename Param = value>
Histogram<Set<value> > histogram_by_groups(const Sequence<value>* sequence, const Sequence< Set<Param> >* groups,
                                           ParamFunc<value, Param> param_func = DefParamFunc<value>)
{
    return histogram_by_groups<value>(sequence->begin(), sequence->end(), groups, param_func);
}
template<typename value, typename Param = value>
Histogram<Set<value> > histogram_by_groups(const SortedSequence<value>& sequence, const Sequence< Set<Param> >* groups,
                                           ParamFunc<value, Param> param_func = DefParamFunc<value>)
{
    return histogram_by_groups<value>(sequence.begin(), sequence.end(), groups, param_func);
}
template<typename value, typename Param = value>
Histogram<Set<value> > histogram_by_groups(const std::initializer_list<value>& list, const Sequence< Set<Param> >* groups,
                                           ParamFunc<value, Param> param_func = DefParamFunc<value>)
{
    return histogram_by_groups<value>(list.begin(), list.end(), groups, param_func);
}

#endif // HISTOGRAM_HPP
