#ifndef EXAMPLE_HPP
#define EXAMPLE_HPP
#include "../histogram.hpp"
#include "../AlphabeticPointer.hpp"
#include "../structures/SparseMatrix.hpp"
#include "../structures/SparseVector.hpp"

void sparse_vector_example() {
    using SV = SparseVector<int>;

    const auto list = {0,0,0,1,5,4,3,1,2,0,0,0,0,4,7,6,5,7,4,3,0,0,0,0,5,0};
    cout << "Initializer list:" << endl << "    ";
    for(const auto& val : list)
        cout << val << ' ';

    cout << endl << "SparseVector from list (only stored index & values):" << endl << "    ";
    const SV vector = list;

    vector.traverse([](const long long& key, const int& data) {
        cout << key << ':' << data << "  ";
    });
    cout << endl << "Vector's fill percent: " << vector.fill_percent() << "\%" << endl;

    const SV subvector = vector.subvector(4,13);
    cout << "Subvector 3-13 (only stored index & values):" << endl << "    ";
    subvector.traverse([](const long long& key, const int& data) {
        cout << key << ':' << data << "  ";
    });
    cout << endl << "Subector's fill percent: " << subvector.fill_percent() << "\%" << endl << endl;
}

void sparse_matrix_example() {
    using LoL = std::initializer_list<std::initializer_list<int>>;
    using SM = SparseMatrix<int>;

    const LoL list_of_lists = {{0,3,2,1,5,7},
                               {0,2,4,0,0,0,7},
                               {1,3,7,0},
                               {0,0,0,5}};

    cout << endl << "List of lists of values: " << endl;
    for(const auto& list : list_of_lists) {
        cout << "    ";
        for(const auto& val : list)
            cout << val << ' ';
        cout << endl;
    }

    cout << "SparseMatrix from list of lists of values:" << endl;
    const SM matrix = list_of_lists;
    for(int row = 0; row < matrix.rows(); ++row) {
        cout << "    ";
        for(int col = 0; col < matrix.cols(); ++col)
            cout << matrix(row, col) << ' ';
        cout << endl;
    }
    cout << "Matrix's fill percent: " << matrix.fill_percent() << '\%' << endl;

    const SM submatrix = matrix.submatrix(1,1,3,5);
    cout << "Submatrix (1,1)-(3,5):" << endl;
    for(int row = 0; row < submatrix.rows(); ++row) {
        cout << "    ";
        for(int col = 0; col < submatrix.cols(); ++col)
            cout << submatrix(row, col) << ' ';
        cout << endl;
    }
    cout << "Submatrix fill percent: " << submatrix.fill_percent() << '\%' << endl << endl;
}

void histogram_example() {
    using Set = Set<int>;
    using LS = ListSequence<int>;
    using GS = ListSequence<Set>;
    const LS seq = {1,2,4,5,6,7,8,8,9,9,1,2,3,5,1,5,2,4,4,6,4,5};
    cout << endl << "Sequence of values:" << endl << "   ";
    cout << seq << endl;

    const GS group_seq = {{1,2,4},
                          {4,5},
                          {6},
                          {3,6,7,8,9},
                          {8,9}};
    cout << "Groups of elements to build histogram from:" << endl;
    auto iter = group_seq.begin();
    auto last = group_seq.end();
    for(; iter != last; ++iter) {
        cout << "   (";
        auto set_iter = (*iter).begin();
        auto set_last = (*iter).end();
        auto last_val = (*iter).back();
        for(; set_iter != set_last; ++set_iter) {
            cout << *set_iter;
            if(*set_iter != last_val)
                cout << ", ";
        }
        cout << ')' << endl;
    }

    auto el_hist = histogram_elementary<int>(&seq);
    cout << "Elementary histogram:" << endl << "   " << el_hist << endl;

    auto hist = histogram_by_groups<int>(&seq, (const Sequence<Set>*)&group_seq);
    cout << "Histogram by groups:" << endl << "   " << hist << endl << endl;
}

void alphabetic_pointer_example() {
    using string = std::string;
    const string words = "1 2 3 4 first word second third other 6 5 8 9 gnome flower sequence array list tree";
    cout << endl << "List of words:" << endl << words << endl;
    auto tree = alphabetic_pointer_creator.pointer_by_word_count(&words, 4);
    cout << "Created alphabetic pointer with max of 4 words on page:" << endl << tree << endl << endl;
}

void example() {
    sparse_vector_example();
    sparse_matrix_example();
    histogram_example();
    alphabetic_pointer_example();
};

#endif // EXAMPLE_HPP
