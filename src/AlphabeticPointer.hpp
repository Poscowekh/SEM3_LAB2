#ifndef ALPHABETICPOINTER_HPP
#define ALPHABETICPOINTER_HPP
#include "sem2_lab3/Structures/Tree.hpp"

class AlphabeticPointerCreator {
private:
    using string = std::string;
    using Tree = AVL_Tree<string, size_t>;
    using Pointer = AlphabeticPointerCreator;

    const string* words;
    Tree tree;
    size_t count;
    typename string::const_iterator word_iter;


    string next_word() {
        string result = "";

        while (word_iter != words->end() && *word_iter != ' ') {
            result += *word_iter;
            ++word_iter;
        }

        if(word_iter != words->end())
            ++word_iter;

        return result;
    }
    string next_word_const() const {
        string result = "";

        auto iter = word_iter;

        while (iter != words->end() && *iter != ' ') {
            result += *iter;
            ++iter;
        }

        return result;
    }


    void revert_iter() {
        if(!tree.empty())
            while(*word_iter != ' ')
                --word_iter;
        else
            while(word_iter != words->end())
                --word_iter;
    }


    void add_page_words(const size_t& page_number) {
        if(page_number == 1)
            add_page_words_(page_number, count / 2 == 0 ? 1 : count / 2);
        else if(page_number % 10 == 0)
            add_page_words_(page_number, count / 4 * 3 == 0 ? 1 : count / 4 * 3);
        else
            add_page_words_(page_number, count);
    }
    void add_page_words_(const size_t& page_number, const size_t& count_) {
        string word = "";
        size_t cnt = 0;

        while (!next_word_const().empty() && cnt < count_) {
            tree.insert(next_word(), page_number);
            ++cnt;
        }
    }

    void add_page_symbols(const size_t& page_number) {
        if(page_number == 1)
            add_page_symbols_(count / 2 == 0 ? 1 : count / 2, page_number);
        else if(page_number % 10 == 0)
            add_page_symbols_(count * 3 / 4, page_number);
        else
            add_page_symbols_(count, page_number);
    }
    void add_page_symbols_(const size_t& count_, const size_t& page_number) {
        string word = next_word();
        size_t symbols = 0;

        while (!word.empty() && symbols < count_) {
            if(word.size() + symbols > count_) {
                if(word.size() > count)
                    throw runtime_error("Alphabetic Pointer Creator exception: a word in string has bigger length than max possible length");
                else
                    break;
            }

            tree.insert(word, page_number);
            symbols += symbols == 0 ? word.size() : (word.size() + 1);
            word = next_word();
        }

        if(!word.empty())
            revert_iter();
    }


public:
    AlphabeticPointerCreator() = default;
    AlphabeticPointerCreator(const Pointer&) = default;
    AlphabeticPointerCreator(Pointer&&) = default;


    Pointer& operator=(const Pointer&) = default;
    Pointer& operator=(Pointer&&) = default;


    Tree&& pointer_by_word_count(const string* words_, const size_t& count_) {
        count = count_;
        words = words_;
        word_iter = words->begin();
        tree = Tree();

        for(size_t i = 1; !next_word_const().empty(); ++i)
            add_page_words(i);

        return move(tree);
    }


    Tree&& pointer_by_symbol_count(const string* words_, const size_t& count_) {
        count = count_;
        words = words_;
        word_iter = words->begin();
        tree = Tree();

        for(size_t i = 1; !next_word_const().empty(); ++i) {
            add_page_symbols(i);
            cout << next_word_const() << ' ' << i << endl;
        }

        return move(tree);
    }


    ~AlphabeticPointerCreator() { words = 0; }
};

AlphabeticPointerCreator alphabetic_pointer_creator = AlphabeticPointerCreator();

#endif // ALPHABETICPOINTER_HPP
