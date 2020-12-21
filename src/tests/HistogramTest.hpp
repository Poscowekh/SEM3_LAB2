#ifndef HISTOGRAMTEST_HPP
#define HISTOGRAMTEST_HPP
#include "../sem3_lab1/testing/tester/_assert_functions.hpp"
#include "../histogram.hpp"

namespace Testing {
    std::string test_histogram_elementary(){
        std::string result = std::string();
        using Histogram = Histogram<int>;
        int size = rand() % 200 + 10;
        _sample test_data = _sample(size);
        Histogram test = histogram_elementary<int>(test_data.data, test_data.data + size);

        if(test.empty())
            _add_str(result, std::string("creating elementary histogram"));

        for(int i = 0; i < test_data.size; ++i)
            if(!test.find(test_data[i]))
                _add_str(result, std::string("creating elementary histogram"));

        if(result.empty())
            return std::string("OK");
        return result;
    };

    std::string test_histogram_by_group_count(){
        std::string result = std::string();
        using Histogram = Histogram< Set<int> >;
        using Set = Set<int>;

        int size = rand() % 300 + 10;
        _sample test_data = sorted_sample(size);
        int group_count = rand() % test_data.size / 5 + 1;
        Histogram test = histogram_by_count(SortedSequence(test_data.data, test_data.size), group_count);

        if(test.empty() || group_count != test.group_count() || test.all_elements() != size_t(test_data.size))
            _add_str(result, std::string("creating histogram by group count"));

        size_t elem_count = 0;
        test.traverse([&test_data, &elem_count](const Set& set, const size_t&){
            auto iter = set.begin();
            auto last = set.end();

            for(; iter != last; ++iter)
                for(int i = 0; i < test_data.size; ++i)
                    if(*iter == test_data[i])
                        ++elem_count;
        });
        if(elem_count != test_data.size)
            _add_str(result, std::string("creating histogram by group count"));

        if(result.empty())
            return std::string("OK");
        return result;
    };

    std::string test_histogram_by_groups(){
        std::string result = std::string();
        using Set = Set<int>;
        using Histogram = Histogram<Set>;
        using Seq = Sequence<Set>;

        int size = rand() % 400 + 10;
        _sample test_data = sorted_sample(size);
        Seq* groups = new ListSequence<Set>();
        int group_count = rand() % test_data.size / 5 + 1;
        for(int i = 0; i < group_count; ++i) {
            int r1 = rand() % (group_count / 2);
            int r2 = rand() % (group_count / 2) + r1;
            _sample group = _sample(r2 - r1);
            memcpy(group.data, test_data.data, sizeof(int) * group.size);
            groups->append(Set(group.data, group.size));
        }

        Histogram test = histogram_by_groups(SortedSequence(test_data.data, test_data.size), groups);

        if(test.empty() || group_count != test.group_count())
            _add_str(result, std::string("creating histogram by group count"));

        if(result.empty())
            return std::string("OK");
        return result;
    };
};

#endif // HISTOGRAMTEST_HPP
