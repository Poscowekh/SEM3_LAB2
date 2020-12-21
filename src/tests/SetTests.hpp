#ifndef SETTESTS_HPP
#define SETTESTS_HPP
#include "../sem3_lab1/testing/tester/_assert_functions.hpp"
#include "../structures/Set.hpp"

namespace Testing {
    std::string test_set_create(){
        std::string result = std::string();
        using Set = Set<int>;
        int size = rand() % 600;
        _sample test_data = sorted_sample(size);

        Set test1 = Set();
        if(!test1.empty())
            _add_str(result, std::string("empty construction"));

        test1 = Set(test_data.data, test_data.size);
        Set test2 = Set(test1);
        Set test3 = Set(test1.begin(), test1.end());
        if(test2.empty() || test3.empty() || !_assert_equal(test2.size(), test3.size()) || !_assert_equal(test1.size(), test2.size()))
            _add_str(result, std::string("copy construction"));

        if(result.empty())
            return std::string("OK");
        return result;
    };

    std::string test_set_add_remove(){
        std::string result = std::string();
        using Set = Set<int>;
        int size = rand() % 600 + 10;
        _sample test_data = sorted_sample(size);
        Set test = Set(test_data.data, test_data.size);

        int r = test[test.size() / 2];
        bool flag = true;
        try {
            test.add(r);
        } catch (...) {
            flag = false;
        }
        if(!flag)
             _add_str(result, std::string("adding a duplicate to set"));

        if(result.empty())
            return std::string("OK");
        return result;
    };

    std::string test_set_iter(){
        std::string result = std::string();
        using Set = Set<int>;
        int size = rand() % 400;
        _sample test_data = sorted_sample(size);
        Set test = Set(test_data.data, test_data.size);

        auto iter = test.begin();
        for(int i = 0; i < test.size(); i++, iter++)
            if(*iter != test[i]) {
                _add_str(result, std::string("using set iterator"));
                break;
            };

        if(result.empty())
            return std::string("OK");
        return result;
    }

    std::string test_set_subset(){
        std::string result = std::string();
        using Set = Set<int>;
        int size = rand() % 600 + 10;
        _sample data = sorted_sample(size);
        Set test = Set(data.data, data.size);

        int index1 = rand() % (size / 2);
        int index2 = rand() % (size / 2) + size / 2;

        Set test2 = test.subset(index1, index2);
        if(test2.size() >= index1)
            _add_str(result, std::string("creating subset (size check)"));

        for(int i = 0; i < test2.size(); i++)
            if(test[i + index1] != test2[i]) {
                _add_str(result, std::string("creating subset"));
                break;
            };

        if(result.empty())
            return std::string("OK");
        return result;
    };

    std::string test_set_operations(){
        std::string result = std::string();
        using Set = Set<int>;
        int size1 = rand() % 400;
        int size2 = rand() % 400;
        _sample data1 = sorted_sample(size1);
        _sample data2 = sorted_sample(size2);
        Set test1 = Set(data1.data, data1.size);
        Set test2 = Set(data2.data, data2.size);

        Set test3 = test1 + test2;
        Set test4 = test1 - test2 + test1 * test2 + test2 - test1;
        if(test3.size() != test3.size())
            _add_str(result, std::string("operating with sets"));

        if(result.empty())
            return std::string("OK");
        return result;
    };
};

#endif // SETTESTS_HPP
