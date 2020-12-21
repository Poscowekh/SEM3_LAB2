#ifndef SPARSEMATRIX_HPP
#define SPARSEMATRIX_HPP
#include "../sem2_lab3/Structures/Tree.hpp"
#include "../sem3_lab1/Structures.hpp"
#include <tuple>

template<typename value>
class SparseMatrix : private AVL_Tree<std::pair<long long, long long>, value>
{
private:
    using Node = TreeNode<std::pair<long long, long long>, value>;
    using Tree = AVL_Tree<std::pair<long long, long long>, value>;
    using Matrix = SparseMatrix;

    long long rows_;
    long long cols_;

    void _unsafe_set(const pair<long long, long long>& indexes, const value& val) {
        this->insert(indexes, val);
    }


public:
    SparseMatrix()
        : Tree(), rows_(0), cols_(0) {};
    SparseMatrix(const Matrix& other)
        : Tree((const Matrix&)other),
          rows_(other.rows_),
          cols_(other.cols_)
    {};
    SparseMatrix(Matrix&& other)
        : Tree((Matrix&&)other),
          rows_(other.rows_),
          cols_(other.cols_)
    {};

    /// all elements wiil be empty
    SparseMatrix(const long long& rows, const long long& cols)
        : Tree(), rows_(rows), cols_(cols) {};

    SparseMatrix(const std::initializer_list<std::initializer_list<value> >& list)
        : SparseMatrix(list, bool())
    {};
    SparseMatrix(value** matrix, const long long& rows, const long long& cols)
        : SparseMatrix(rows, cols)
    {
        auto row_iter = matrix;

        for(auto i = 0; i < rows_; ++i, ++row_iter) {
            auto col_iter = *row_iter;

            for(auto j = 0; j < cols_; ++j, ++col_iter)
                if(*col_iter != value())
                    Tree::insert(make_pair(i,j), *col_iter);
        };
    };
    /// requires rai iterators in class and it's value-class
    template<template<typename Container2> typename Container1,
             typename Container2 = Container1<value> >
    SparseMatrix(const Container1<Container2>& matrix_like_cont, bool)
        : SparseMatrix(matrix_like_cont.size(), 0)
    {
        auto row_iter = matrix_like_cont.begin();

        for(auto i = 0; i < rows_; ++i, ++row_iter) {
            auto col_iter = (*row_iter).begin();
            auto count = (*row_iter).size();

            for(auto j = 0; j < count; ++j, ++col_iter)
                if(*col_iter != value())
                    Tree::insert(make_pair(i,j), *col_iter);

            if(cols_ < count)
                cols_ = count;
        };
    };


    SparseMatrix(const Sequence<std::tuple<long long, long long, value> >* sequence)
        : SparseMatrix(*sequence, int()) {}
    SparseMatrix(const std::initializer_list<std::tuple<long long, long long, value> >& list)
        : SparseMatrix(list, int()) {}
    /// requires rai iterators in container, tuple_type = row index, col index, value
    template<template<typename tuple_type = std::tuple<long long, long long, value> > typename Container,
             typename tuple_type = std::tuple<long long, long long, value> >
    SparseMatrix(const Container<tuple_type>& vector_like_container, int)
        : SparseMatrix(vector_like_container.size(), 0)
    {
        auto iter = vector_like_container.begin();
        auto last = vector_like_container.end();

        for(; iter != last; ++iter) {
            long long row = std::get(0, *iter);
            long long col = std::get(1, *iter);
            value val = std::get(2, *iter);

            if(val != value())
                this->insert(std::make_pair(row, col), val);

            if(rows_ < row)
                rows_ = row;
            if(cols_ < col)
                cols_ = col;
        };
    };



    template<typename Function, typename ...Arguments>
    void traverse(Function func, Arguments... args) const {
        Tree::traverse(Tree::LeftRootRight, func, args...);
    };


    value operator()(const long long& row, const long long& col) const {
        return operator()(make_pair(row, col));
    };
    value& operator()(const long long& row, const long long& col) {
        return operator()(make_pair(row, col));
    };

    value& operator()(const pair<long long, long long>& pair_) {
        if(pair_.first >= rows_ || pair_.second >= cols_)
            throw tree_out_of_range;

        if(!Tree::find_key(pair_))
            Tree::insert(pair_, value());

        return Tree::operator[](pair_);
    };
    value operator()(const pair<long long, long long>& pair_) const {
        if(pair_.first >= rows_ || pair_.second >= cols_)
            throw tree_out_of_range;

        if(!Tree::find_key(pair_))
            return value();

        return Tree::operator[](pair_);
    };

    value operator[](const pair<long long, long long>& pair_) const { return operator()(pair_); };
    value& operator[](const pair<long long, long long>& pair_) { return operator()(pair_); };


    bool is_not_empty(const pair<long long, long long>& pair_) const {
        return Tree::find_key(pair_);
    };
    bool is_not_empty(const long long& row, const long long& col) const {
        return is_not_empty(make_pair(row, col));
    };
    bool empty() const { return Tree::empty(); };


    void increase_rows(const long long& amount) {
        if(amount < 0)
            std::logic_error("sparse matrix exception: increasing rows by negative amount");
        rows_ += amount;
    };
    void increase_cols(const long long& amount) {
        if(amount < 0)
            std::logic_error("sparse matrix exception: increasing cols by negative amount");
        cols_ += amount;
    };
    void clear() const { Tree::clear(); };
    void remove_index(const long long& row, const long long& col) {
        this->remove(std::make_pair(row, col));
    }
    void remove_value(const value& val) {
        Node* node = this->remove_data(val);
        if(node != 0)
            this->remove(node->get_key());
    }


    const long long& rows() const { return rows_; };
    const long long& cols() const { return cols_; };
    const int& filled_size() const { return Tree::size(); };
    long long max_capacity() const { return rows_ * cols_; }
    long long free_capacity() const { return max_capacity() - filled_size(); }
    double fill_percent() const { return static_cast<double>(filled_size()) / max_capacity(); }


    bool operator==(const Matrix& other) const {
        if(rows_ != other.rows_)
            return false;

        if(cols_ != other.cols_)
            return false;

        return Tree::operator==((const Tree&)other);
    };
    bool operator!=(const Matrix& other) const { return !operator==(other); };


    const Matrix& operator=(const Matrix& other) {
        clear();
        Tree::operator=((const Matrix&)other);
        rows_ = other.rows_;
        cols_ = other.cols_;
        return *this;
    };
    const Matrix& operator=(Matrix&& other) {
        clear();
        Tree::operator=((Matrix&&)other);
        rows_ = other.rows_;
        cols_ = other.cols_;
        return *this;
    };
    const Matrix& operator=(const Sequence<std::tuple<long long, long long, value> >* sequence) {
        clear();
        return *this = Matrix(sequence);
    };
    const Matrix& operator=(const std::initializer_list<std::initializer_list<value> >& list) {
        clear();
        return *this = Matrix(list);
    };
    const Matrix& operator=(const std::initializer_list<std::tuple<long long, long long, value> >& list) {
        clear();
        return *this = Matrix(list);
    };


    Matrix submatrix(const long long& top_left_row, const long long& top_left_col,
                     const long long& bottom_right_row, const long long& bottom_right_col) const
    {
        return submatrix(make_pair(top_left_row, top_left_col), make_pair(bottom_right_row, bottom_right_col));
    }
    Matrix submatrix(const std::pair<long long, long long>& top_left,
                     const std::pair<long long, long long>& bottom_right) const
    {
        Matrix result = this->where([&top_left, &bottom_right](const auto& pair, const value&){
            if(pair.first >= top_left.first && pair.first <= bottom_right.first &&
               pair.second >= top_left.second && pair.second <= bottom_right.second)
                return true;
            return false;
        });
        result.rows_ = bottom_right.first - top_left.first + 1;
        result.cols_ = bottom_right.second - top_left.second + 1;
        return result;
    }


    friend std::ostream& operator<<(std::ostream& out, const Matrix& mtrx) {
        long long prev_index = -1;
        mtrx.traverse([&out, &prev_index](const std::pair<long long, long long>& indexes, const value& data) {
            if(prev_index == -1)
                prev_index = indexes.first;

            if(indexes.first > prev_index) {
                out << endl;
                prev_index = indexes.first;
            };

            out << data << ' ';
        });
        return out;
    };


    template<typename ReduceFunc = value (*)(const value&, const value&), typename ...Args>
    value reduce(ReduceFunc func, const value& init_value = value(), Args... args) const {
        value result = init_value;

        traverse([&result, &func, &args...](const pair<long long, long long>&, const value& val) {
            result = func(result, val, args...);
        });

        return result;
    };

    template<typename WhereFunc = bool (*)(const pair<long long, long long>&, const value&, ...), typename ...Args>
    Matrix where(WhereFunc func, Args... args) const {
        Matrix result = Matrix();

        long long min_row = LONG_LONG_MAX, min_col = LONG_LONG_MAX;
        long long max_row = LONG_LONG_MIN, max_col = LONG_LONG_MIN;

        traverse([&result, &func, &min_row, &min_col, &max_row, &max_col, &args...]
                 (const pair<long long, long long>& indexes, const value& val)
        {
            if(func(indexes, val, args...)) {
                result._unsafe_set(indexes, val);

                if(indexes.first > max_row)
                    max_row = indexes.first;
                if(indexes.first < min_row)
                    min_row = indexes.first;

                if(indexes.second > max_col)
                    max_col = indexes.second;
                if(indexes.second < min_col)
                    min_col = indexes.second;
            }
        });

        result.traverse([&min_row, &min_col](pair<long long, long long>& indexes, const value&){
            indexes.first -= min_row;
            indexes.second -= min_col;
        });

        result.rows_ = max_row - min_row + 1;
        result.cols_ = max_col - min_col + 1;

        return result;
    };

    template<typename MapFunc = value (*)(const pair<long long, long long>&, const value&, ...), typename ...Args>
    Matrix map(MapFunc func, Args... args) const {
        Matrix result = Matrix(*this);

        result.traverse([&func, &args...](const pair<long long, long long>& indexes, const value& val) {
            val = func(indexes, val, args...);
        });

        return result;
    };
};

#endif // SPARSEMATRIX_HPP
