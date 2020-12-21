#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
//#include <string>

using namespace std;            //Template Functions For Containers

template<typename Container, typename Function>
Container* Map(Container* container, Function function)
{
    Container* result = new Container;
    for(int i = 0; i < container->Size(); ++i)
        result->PushBack(function(container->Get(i)));
    return result;
};

template<typename Container, typename Function>
Container* Where(Container* container, Function function)
{
    Container* result = new Container;
    for(int i = 0; i < container->Size(); ++i)
        if(function(container->Get(i)))
            result->PushBack(container->Get(i));
    return result;
};

template<typename DataType, typename Container, typename Function>
DataType Reduce(Container* container, Function function, DataType const_value)
{
    DataType result = function(container->Get(0), const_value);
    for(int i = 1; i < container->Size(); ++i)
        result = function(container->Get(i), result);
    return result;
};

template<typename Container>
Container* Sort(Container* container)
{
    Container* result = new Container(*container);
    for(int i = 0; i < result->Size() - 1; i++)
        for(int j = i; j < result->Size(); j++)
            if(result->Get(i) > result->Get(j))
            {
                auto tmp = result->Get(i);
                result->Set(i, result->Get(j));
                result->Set(j, tmp);
            };
    return result;
};

#endif // FUNCTIONS_HPP
