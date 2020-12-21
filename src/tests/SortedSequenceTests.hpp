#ifndef SORTEDSEQUENCETESTS_HPP
#define SORTEDSEQUENCETESTS_HPP
#include "../sem3_lab1/testing/tester/_assert_functions.hpp"
#include "../structures/SortedSequence.hpp"

namespace Testing {
    Sequence<int>* sequence_sample(const int& size) {
        _sample tmp = sorted_sample(size);
        return new ArraySequence(tmp.data, tmp.size);
    }

    std::string test_sorted_sequence_create(){
        std::string result = std::string();
        using SS = SortedSequence<int>;
        int size = rand();
        Sequence<int>* test_data = sequence_sample(size);

        SS test1 = SS();
        if(!test1.empty())
            _add_str(result, std::string("empty construction"));

        test1 = SS(test_data);
        SS test2 = SS(test1);
        SS test3 = SS(&test1);
        SS test4 = SS(test1.begin(), test1.end());
        if(!_assert_equal(test1, test2) || !_assert_equal(test1, test3) ||
                !_assert_equal(test1, test4))
            _add_str(result, std::string("copy construction"));

        delete test_data;

        if(result.empty())
            return std::string("OK");
        return result;
    };

    std::string test_sorted_sequence_add_remove(){
        std::string result = std::string();
        using SS = SortedSequence<int>;
        int size = rand() + 10;
        Sequence<int>* test_data = sequence_sample(size);
        SS test = SS(test_data);

        int r = INT_MAX;
        test.push(r);
        if(!_assert_equal(size + 1, test.size()) || test.back() != r)
            _add_str(result, std::string("adding to sorted sequence"));

        test.remove(r);
        if(!_assert_equal(size, test.size()) || test.find(r))
            _add_str(result, std::string("removing from sorted sequence"));

        delete test_data;

        if(result.empty())
            return std::string("OK");
        return result;
    };

    std::string test_sorted_sequence_iter(){
        std::string result = std::string();
        using SS = SortedSequence<int>;
        int size = rand();
        Sequence<int>* test_data = sequence_sample(size);
        SS test = SS(test_data);

        auto iter = test.begin();
        for(int i = 0; i < test.size(); i++, iter++)
            if(*iter != test[i]) {
                _add_str(result, std::string("using sorted sequence iterator"));
                break;
            };

        delete test_data;

        if(result.empty())
            return std::string("OK");
        return result;
    };

    std::string test_sorted_sequence_concate(){
        std::string result = std::string();
        using SS = SortedSequence<int>;
        int size1 = rand() % 200, size2 = rand() % 200;
        Sequence<int>* data1 = sequence_sample(size1);
        Sequence<int>* data2 = sequence_sample(size2);
        SS test1 = SS(data1);
        SS test2 = SS(data2);

        SS test = test1.get_concated(test2);
        if(test.size() != size1 + size2)
            _add_str(result, std::string("concating sequences (size check)"));

        for(int i = 0; i < size1; i++)
            if(test[i] != test1[i]) {
                _add_str(result, std::string("concating sequences (member check 1)"));
                break;
            };

        for(int i = 0; i < size2; i++)
            if(test[i + size1] != test2[i]) {
                _add_str(result, std::string("concating sequences (member check 1)"));
                break;
            };

        if(result.empty())
            return std::string("OK");
        return result;
    };

    std::string test_sorted_sequence_subsequence(){
        std::string result = std::string();
        using SS = SortedSequence<int>;
        int size = rand() + 10;
        Sequence<int>* data = sequence_sample(size);
        SS test = SS(data);

        int index1 = rand() % (size / 2);
        int index2 = rand() % (size / 2) + size / 2;

        SS test2 = test.subsequence(index1, index2);
        if(!_assert_equal(test2.size(), index2 - index1 + 1))
            _add_str(result, std::string("creating subsequence (size check)"));

        for(int i = 0; i < test2.size(); i++)
            if(test[i + index1] != test2[i]) {
                _add_str(result, std::string("creating subsequence"));
                break;
            };

        if(result.empty())
            return std::string("OK");
        return result;
    };
};

#endif // SORTEDSEQUENCETESTS_HPP
