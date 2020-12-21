#ifndef SPARSEMATRIXTEST_HPP
#define SPARSEMATRIXTEST_HPP
#include "../sem3_lab1/testing/tester/_assert_functions.hpp"
#include "../structures/SparseMatrix.hpp"

namespace Testing {
    using _sample_indxs = __sample<std::pair<long long, long long> >;
    using _tuple_sequence = Sequence<std::tuple<long long, long long, int> >;
    _sample_indxs rows_and_cols(const int& amount) {
        _sample_indxs result = _sample_indxs();
        result.data = (std::pair<long long, long long>*)malloc(sizeof(std::pair<long long, long long>) * amount);
        result.size = amount;

        long long r1 = 0;
        long long r2 = 0;
        for(int i = 0; i < amount; ++i) {
            r1 += rand() % 4 + 1;
            r2 += rand() % 4 + 1;
            result[i] = std::make_pair(r1, r2);
        }

        return result;
    }
    _tuple_sequence* vector_like_sample(const int& amount) {
        _tuple_sequence* result = new ArraySequence<std::tuple<long long, long long, int> >(amount);
        _sample_indxs indexes = rows_and_cols(amount);
        _sample values = _sample(amount);

        for(int i = 0; i < amount; ++i)
            result->operator[](i) = std::make_tuple(indexes[i].first,
                                                    indexes[i].second,
                                                    values[i]);

        return result;
    }

    std::string test_sparse_matrix_create(){
        std::string result = std::string();
        using Matrix = SparseMatrix<int>;
        const int filled_size = rand() % 100 + 50;
        _tuple_sequence* test_data = vector_like_sample(filled_size);

        Matrix test1 = Matrix();
        if(!test1.empty())
            _add_str(result, std::string("empty construction"));

        test1 = Matrix(*test_data, int());
        Matrix test2 = Matrix(test1);
        if(!_assert_equal(test1.size(), test2.size()))
            _add_str(result, std::string("copy construction"));

        if(result.empty())
            return std::string("OK");
        return result;
    };

    std::string test_sparse_matrix_add_remove(){
        std::string result = std::string();
        using Matrix = SparseMatrix<int>;
        const int filled_size = rand() % 100 + 50;
        _tuple_sequence* test_data = vector_like_sample(filled_size);
        Matrix test = Matrix(*test_data, int());

        std::pair<long long, long long> indxs = std::make_pair(0, rand() % test.cols());
        test[indxs] = indxs.first;
        if(!_assert_equal(filled_size + 1, test.filled_size()) || !_assert_equal(indxs, test[indxs]))
            _add_str(result, std::string("adding to sparse matrix"));

        test.remove_index(indxs.first, indxs.second);
        if(!_assert_equal(filled_size, test.filled_size()))
            _add_str(result, std::string("removing from sparse matrix"));

        if(result.empty())
            return std::string("OK");
        return result;
    };

    std::string test_sparse_matrix_traverse(){
        std::string result = std::string();
        using Matrix = SparseMatrix<int>;
        const int filled_size = rand() % 100 + 50;
        _tuple_sequence* test_data = vector_like_sample(filled_size);
        const Matrix test = Matrix(*test_data, int());

        test.traverse([](const std::pair<long long, long long> indxs, int& val){
            val = indxs.first;
        });

        for(long long row = 0; row < test.rows(); ++row)
            for(long long col = 0; col < test.cols(); ++col)
                if(test(row, col) != int())
                    if(!_assert_equal(row, test(row, col)))
                        _add_str(result, std::string("traversing sparse matrix"));

        if(result.empty())
            return std::string("OK");
        return result;
    };

    /*std::string test_sparse_matrix_concate(){
        std::string result = std::string();
        using Matrix = SparseMatrix<int>;
        const int size1 = rand() % 100 + 50;
        _tuple_sequence* data1 = vector_like_sample(size1);
        const int size2 = rand() % 100 + 50;
        _tuple_sequence* data2 = vector_like_sample(size2);
        const Matrix test1 = Matrix(data1);
        const Matrix test2 = Matrix(data2);

        Matrix test = test1.get_concated(test2);
        if(!_assert_equal(test.size(), size1 + size2))
            _add_str(result, std::string("concating sparse matrixs (size check)"));

        for(int i = 0; i < size1; i++)
            if(test[i] != test1[i]) {
                _add_str(result, std::string("concating sparse matrixs (member check 1)"));
                break;
            };

        for(int i = 0; i < size2; i++)
            if(test[i + size1] != test2[i]) {
                _add_str(result, std::string("concating sparse matrixs (member check 1)"));
                break;
            };

        if(result.empty())
            return std::string("OK");
        return result;
    };

    std::string test_sparse_matrix_submatrix(){
        std::string result = std::string();
        using Matrix = SparseMatrix<int>;
        int size = rand() + 10;
        _sample data = _sample(size);
        const Matrix test = Matrix(data.data, data.size);

        int index1 = rand() % (size / 2);
        int index2 = rand() % (size / 2) + size / 2;

        Matrix test2 = test.submatrix(index1, index2);
        if(!_assert_equal(test2.size(), index2 - index1 + 1))
            _add_str(result,std::string("creating subsparse matrix (size check)"));

        for(int i = 0; i < test2.size(); i++)
            if(test[i + index1] != test2[i]) {
                _add_str(result,std::string("creating subsparse matrix"));
                break;
            };

        if(result.empty())
            return std::string("OK");
        return result;
    };*/
};

#endif // SPARSEMATRIXTEST_HPP
