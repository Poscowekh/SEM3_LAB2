#ifndef INTERFACE_HPP
#define INTERFACE_HPP
#include "../tests/TestMain.hpp"
#include "example.hpp"

class Interface {
private:
    int sequence_switch = 0;
    ArraySequence<int> sequence;
    ArraySequence<Set<int>> group_sequence;
    ArraySequence<ArraySequence<int>> matrix;


    int get_seed() {
        using std::cout;
        using std::cin;
        using std::endl;

        cout << "Choose a seed: ";
        int seed = -1;
        cin >> seed;

        if(seed < 0){
            cout << "Wrong input, try again...\n";
            return get_seed();
        };

        return seed;
    };
    void manual_gen() {
        using std::cout;
        using std::cin;
        using std::endl;

        cout << "Start entering " << sequence.size() << " values:\n";
        auto iter = sequence.begin();
        auto last = sequence.end();

        for(int i = 0; iter != last; ++i, ++iter){
            cout << "   " << i << ": ";
            int value = 0;
            cin >> value;
            *iter = value;
        };

        call_funcs(choose_action());
    };
    void random_seed_gen() {
        using std::cout;
        using std::cin;
        using std::endl;

        int choise = -1;

        cout << "Choose action:\n";
        cout << "   0: Gp back to main choices\n";
        cout << "   1: Go back to choosing a way to generate a sequence\n";
        cout << "   2: Seed from current system time\n";
        cout << "   3: Manual seed input\n";
        cout << "Chosen: ";

        cin >> choise;

        if(choise < 0 || choise > 3){
            cout << "Wrong input, try again...\n";
            random_seed_gen();
        };

        int seed = 0;
        switch(choise) {
        case 0:
            call_funcs(choose_action());
            return;
        case 1:
            choose_how_to_create();
            return;
            break;
        case 2:
            seed = time(0);
            break;
        case 3:
            seed = get_seed();
            break;
        };

        Testing::_sample data = Testing::_sample(sequence.size(), seed);

        sequence = ArraySequence<int>(data.data, data.size);

        call_funcs(choose_action());
    };
    void choose_size() {
        using std::cout;
        using std::cin;
        using std::endl;

        int choise = -1;

        cout << "Enter valid size or a negative number to go back:\nSize = ";
        cin >> choise;

        if(choise == 0){
            cout << "Wrong input, try again...\n";
            return choose_size();
        };

        if(choise < 0) {
            call_funcs(choose_action());
            return;
        };

        sequence = ArraySequence<int>(choise);
    };
    void choose_how_to_create() {
        using std::cout;
        using std::cin;
        using std::endl;

        int choise = -1;

        cout << "Choose how to create your sequence:\n";
        cout << "   0: Go back main choices\n";
        cout << "   1: Random seed generation\n";
        cout << "   2: Manual input\n";
        cout << "Chosen: ";

        cin >> choise;

        if(choise < 0 || choise > 2){
            cout << "Wrong input, try again...\n";
            choose_how_to_create();
        };

        choose_size();

        switch(choise) {
        case 0:
            call_funcs(choose_action());
            return;
            break;
        case 1:
            random_seed_gen();
            break;
        case 2:
            manual_gen();
            break;
        };
    };

    Set<int> create_group() {
        using std::cin;
        using std::cout;
        using std::endl;

        cout << "Enter group size: ";
        int size = 1;
        cin >> size;

        Set<int> result = Set<int>();

        cout << "Enter values in group:\n";
        for(int i = 0; i < size; ++i) {
            int value = 0;
            cin >> value;
            result.add(value);
        }

        return result;
    }
    void create_groups() {
        using std::cin;
        using std::cout;
        using std::endl;

        cout << "Enter group count: ";
        int group_count = 1;
        cin >> group_count;

        group_sequence = ArraySequence<Set<int> >();

        for(int i = 0; i < group_count; ++i)
            group_sequence.append(create_group());

        call_funcs(choose_action());
    }
    void create_histogram() {
        cout << histogram_by_groups<int>((const Sequence<int>*)&sequence, (const Sequence<Set<int>>*)&group_sequence) << endl;
    }

    void create_matrix() {
        using std::cout;
        using std::cin;

        cout << "Enter rows: ";
        int rows = 1, cols = 1;
        cin >> rows;
        cout << "Enter cols: ";
        cin >> cols;
        matrix = ArraySequence<ArraySequence<int>>(rows);
        for(int i = 0; i < rows; ++i)
            matrix[i] = ArraySequence<int>(cols);

        cout << "Enter row index, col index, value one by one:\n"
             << "To stop enter a negative row index";
        while (true) {
            long long row = 0, col = 0;
            int val = 0;
            cin >> row;
            if(row < 0)
                break;
            cin >> col >> val;
            matrix[row][col] = val;
        }
    }
    void create_sparse_matrix() {
        cout << "Non-zero members of sparse matrix:\n";
        const SparseMatrix<int> mtrx = SparseMatrix<int>(matrix, true);
        for(long long i = 0; i < mtrx.rows(); ++i) {
            for(long long j = 0; j < mtrx.cols(); ++j)
                cout << mtrx(i,j) << ' ';
            cout << endl;
        }
    }

    int choose_action() const{
        using std::cout;
        using std::cin;

        int choise = -1;

        cout << "Choose action:\n";
        cout << "   0: exit\n";
        cout << "   1: run examples\n";
        cout << "   2: run tests\n";
        cout << "   3: create sequence\n";
        cout << "   4: create groups\n";
        //cout << "   5: create histogram using my sequence and groups\n";
        cout << "   5: create matrix\n";
        cout << "   6: crate sparse matrix from my matrix\n";
        //cout << "   7: create sequence of words";
        //cout << "   8: create alphabetic pointer from my words";
        cout << "Chosen: ";

        cin >> choise;

        if(choise < 0 || choise > 8) {
            cout << "Wrong input, try again...\n";
            return choose_action();
        };

        return choise;
    };
    void call_funcs(const int choise){
        using std::cout;
        using std::endl;

        switch(choise){
        case 0:
            cout << "Goodbye!\n";
            exit(0);
        case 1:
            example();
            break;
        case 2:
            Testing::tester();
            cout << endl << endl;
            break;
        case 3:
            choose_how_to_create();
            break;
        case 4:
            create_groups();
            break;
        case 5:
            create_histogram();
            break;
        case 6:
            create_matrix();
            break;
        case 7:
            create_sparse_matrix();
            break;

        };

        call_funcs(choose_action());
    };

public:
    Interface() = default;
    Interface(Interface&&) = default;

    void operator()(){
        call_funcs(choose_action());
    };

    void run() { this->operator()(); }
};

#endif // INTERFACE_HPP
