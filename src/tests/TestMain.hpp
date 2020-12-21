#ifndef TESTMAIN_HPP
#define TESTMAIN_HPP
#include "SortedSequenceTests.hpp"
#include "../sem3_lab1/testing/tests/Tests.hpp"
#include "SparseVectorTest.hpp"

#include "SetTests.hpp"
#include "HistogramTest.hpp"
#include "AlphabeticPointerTest.hpp"

namespace Testing {
    Test tests_for_sorted_sequence(){
        Test test = Test("Sorted sequence");
        test.add_test("Sorted sequence creation", test_sorted_sequence_create);
        test.add_test("Sorted sequence add/remove", test_sorted_sequence_add_remove);
        test.add_test("Sorted sequence iterator", test_sorted_sequence_iter);
        //test.add_test("Sorted sequence concatenation", test_sorted_sequence_concate);
        test.add_test("Sorted sequence subsequence", test_sorted_sequence_subsequence);
        return test;
    };

    Test tests_for_sparse_vector(){
        Test test = Test("Sparse Vector");
        test.add_test("Sparse Vector creation", test_sparse_vector_create);
        test.add_test("Sparse Vector change values", test_sparse_vector_operations);
        test.add_test("Sparse Vector traverse", test_sparse_vector_traverse);
        test.add_test("Sparse Vector concatenation", test_sparse_vector_concate);
        test.add_test("Sparse Vector subvector", test_sparse_vector_subvector);
        return test;
    };

    Test tests_for_sparse_matrix(){
        Test test = Test("Sparse Matrix");
        test.add_test("Sequence creation", test_sorted_sequence_create);
        test.add_test("Sequence add/remove", test_sorted_sequence_add_remove);
        test.add_test("SequenceIterator", test_sorted_sequence_iter);
        test.add_test("Sequence concatenation", test_sorted_sequence_concate);
        test.add_test("Sequence subsequence", test_sorted_sequence_subsequence);
        return test;
    };

    Test tests_for_set(){
        Test test = Test("Set");
        test.add_test("Set creation", test_set_create);
        test.add_test("Set add/remove", test_set_add_remove);
        test.add_test("SetIterator", test_set_iter);
        //test.add_test("Set subset", test_set_subset);
        test.add_test("Set operations", test_set_operations);
        return test;
    };

    Test tests_for_histogram(){
        Test test = Test("Histogram");
        test.add_test("Histogram elementary", test_histogram_elementary);
        test.add_test("Histogram by group count", test_histogram_by_group_count);
        test.add_test("Histogram by groups", test_histogram_by_groups);
        return test;
    };

    Test tests_for_alphabetic_pointer(){
        Test test = Test("Alphabetic Pointer");
        test.add_test("Alphabetic Pointer by word count", test_alphabetic_pointer_word_count);
        return test;
    };

    Tester create_tester(){
        Tester tester = Tester();

        //Test* array = new Test(tests_for_array());
        //Test* list = new Test(tests_for_list());
        //Test* sequence = new Test(tests_for_sequence());
        Test* sorted_sequence = new Test(tests_for_sorted_sequence());
        Test* sparse_vector = new Test(tests_for_sparse_vector());

        Test* set = new Test(tests_for_set());
        //Test* histogram = new Test(tests_for_histogram());
        Test* alphabetic_pointer = new Test(tests_for_alphabetic_pointer());

        //tester.add_test(array);
        //tester.add_test(list);
        //tester.add_test(sequence);
        tester.add_test(sorted_sequence);
        tester.add_test(sparse_vector);

        tester.add_test(set);
        //tester.add_test(histogram);
        //tester.add_test(alphabetic_pointer);

        return tester;
    };

    ///global testing var
    Tester tester = create_tester();
};

#endif // TESTMAIN_HPP
