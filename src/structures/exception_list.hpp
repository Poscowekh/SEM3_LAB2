#ifndef EXCEPTION_LIST_HPP
#define EXCEPTION_LIST_HPP
#include <stdexcept>
#include <iostream>

/// is thrown when class is initialized with a null pointer to an array of elements;
std::runtime_error null_pointer = std::runtime_error("exception: null pointer");

/// is thrown when operating with empty container
std::logic_error container_empty = std::logic_error("exception: container is empty");

/// is thrown when removing non existent member
std::out_of_range sparse_vector_out_of_range = std::out_of_range("Sparse vector exception: element out of range");



#endif // EXCEPTION_LIST_HPP
