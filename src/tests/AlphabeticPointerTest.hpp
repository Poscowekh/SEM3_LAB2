#ifndef ALPHABETICPOINTERTEST_HPP
#define ALPHABETICPOINTERTEST_HPP
#include "../sem3_lab1/testing/tester/_assert_functions.hpp"
#include "../AlphabeticPointer.hpp"
#include "../sem3_lab1/structures/Array/_array.hpp"

namespace Testing {
    std::string test_alphabetic_pointer_word_count(){
        using APC = AlphabeticPointerCreator;
        using Tree = AVL_Tree<std::string, std::size_t>;
        APC apc = APC();
        std::string result = "";

        int word_count = rand() % 300 + 10;
        std::string words = std::string();
        Array<std::string> word_array = Array<std::string>();
        for(auto i = 1; i <= word_count; ++i){
            std::string word = std::string();

            auto max = i > 20 ? 20 : i;
            for(auto j = 0; j < max; ++j)
                word += char(rand() % 100 + 50);
            word_array.push_back(word);

            words += word + " ";
            }


        int page_word_count = rand() % (word_count / 5);
        Tree test = apc.pointer_by_word_count(&words, page_word_count);

        auto iter = word_array.begin();
        auto last = word_array.end();

        for(; iter != last; ++iter)
            if(!test.find_key(*iter)) {
                _add_str(result, "creation alphabetic pointer by max words on page");
                break;
            }


        if(result.empty())
            return std::string("OK");
        return result;
    };
};
#endif // ALPHABETICPOINTERTEST_HPP
