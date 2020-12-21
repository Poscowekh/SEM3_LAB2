#ifndef FUNCTIONARRAY_HPP
#define FUNCTIONARRAY_HPP
//#include <stdlib.h>

using namespace std;        //MAJOR REWORK NEEDED

template<typename ReturnType, typename ...Args>
struct FunctionArray
{
    const int size = 10;
    ReturnType(*funcs[10])(Args...);
};

#endif // FUNCTIONARRAY_HPP
