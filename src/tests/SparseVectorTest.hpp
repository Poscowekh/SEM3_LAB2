#ifndef SPARSEVECTORTEST_HPP
#define SPARSEVECTORTEST_HPP
#include "../sem3_lab1/testing/tester/_assert_functions.hpp"
#include "../structures/SparseVector.hpp"

namespace Testing {
    std::string test_sparse_vector_create(){
        std::string result = std::string();
        using Vector = SparseVector<int>;
        int size = rand() % 1000;
        _sample test_data = _sample(size);

        Vector test1 = Vector();
        if(!test1.empty())
            _add_str(result, std::string("empty construction"));

        test1 = Vector(test_data.data, test_data.size);
        Vector test2 = Vector(test1);
        if(test1.size() != test2.size())
            _add_str(result, std::string("copy construction"));

        if(result.empty())
            return std::string("OK");
        return result;
    };

    std::string test_sparse_vector_operations(){
        std::string result = std::string();
        using Vector = SparseVector<int>;
        int size = rand() % 1000 + 10;
        _sample test_data = _sample(size);
        Vector test = Vector(test_data.data, test_data.size);

        test.increase_size(1);
        int index = test.size() - 1;
        test[index] = index;
        if(size + 1 != test.size() || index != test[index])
            _add_str(result, std::string("changing value in sparse vector"));

        test.remove_index(index);
        if(size != test.size())
            _add_str(result, std::string("removing from sparse vector"));

        if(result.empty())
            return std::string("OK");
        return result;
    };

    std::string test_sparse_vector_traverse(){
        std::string result = std::string();
        using Vector = SparseVector<int>;
        int size = rand() % 1000;
        _sample test_data = _sample(size);
        const Vector test = Vector(test_data.data, test_data.size);

        test.traverse([](const long long& index, int& val){
            val = index;
        });

        for(long long index = 0; index < test.size(); ++index)
            if(test[index] != 0)
                if(index != test[index])
                    _add_str(result, std::string("traversing sparse vector"));

        if(result.empty())
            return std::string("OK");
        return result;
    };

    std::string test_sparse_vector_concate(){
        std::string result = std::string();
        using Vector = SparseVector<int>;
        int size1 = rand() % 500, size2 = rand() % 500;
        _sample data1 = _sample(size1);
        _sample data2 = _sample(size2);
        const Vector test1 = Vector(data1.data, data1.size);
        const Vector test2 = Vector(data2.data, data2.size);

        const Vector test = test1.get_concated(test2);
        if(test.size() != test1.size() + test2.size())
            _add_str(result, std::string("concating sparse vectors (size check)"));

        /*for(int i = 0; i < size1; i++)
            if(test[i] != test1[i]) {
                _add_str(result, std::string("concating sparse vectors (member check 1)"));
                break;
            };

        for(int i = 0; i < size2; i++)
            if(test[i + size1] != test2[i]) {
                _add_str(result, std::string("concating sparse vectors (member check 2)"));
                break;
            };*/

        if(result.empty())
            return std::string("OK");
        return result;
    };

    std::string test_sparse_vector_subvector(){
        std::string result = std::string();
        using Vector = SparseVector<int>;
        int size = rand() % 1000 + 10;
        _sample data = _sample(size);
        const Vector test = Vector(data.data, data.size);

        int index1 = rand() % (size / 2);
        int index2 = rand() % (size / 2) + size / 2;

        Vector test2 = test.subvector(index1, index2);
        if(test2.size() != index2 - index1 + 1)
            _add_str(result,std::string("creating subsparse vector (size check)"));

        for(int i = index2; i < index2 - index1 + 1; i++)
            if(test[i] != test2[i]) {
                _add_str(result,std::string("creating sparse subvector"));
                break;
            };

        if(result.empty())
            return std::string("OK");
        return result;
    };
};

#endif // SPARSEVECTORTEST_HPP
