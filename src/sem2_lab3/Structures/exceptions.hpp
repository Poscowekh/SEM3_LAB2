#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP
#include <stdexcept>
#include <iostream>

using namespace std;

/// is thrown during search if key belongs to the node from which we start searching parent
runtime_error node_parent_unreachable = runtime_error("Node exception: parent of node is out of reach");

/// is thrown if key was not inserted into the tree
runtime_error node_key_invalid = runtime_error("Node exception: key is invalid");

/// is thrown when operating with empty tree
logic_error tree_empty = logic_error("Binary tree exception: tree is empty");

/// is thrown when operating with empty tree
out_of_range tree_out_of_range = out_of_range("Binary tree exception: tree is out of range");

/// is thrown when inserting an existing key
runtime_error node_key_exists = runtime_error("Node exception: key already exists in the tree");

/// is thrown when operation with null pointer to node
runtime_error node_pointer_null = runtime_error("Node exception: operating with null pointer");

template<typename T, typename TT = T>
std::ostream& operator<<(std::ostream& out, const std::pair<T, TT>& pair) {
    out << '(' << pair.first << ", " << pair.second << ')';
    return out;
};

#endif // EXCEPTIONS_HPP
