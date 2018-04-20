#pragma once

#include <typeinfo>

namespace stdex
{
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    class type_name
    {
    public:
        static const char * value;
    };
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    const char * type_name<T>::value = typeid(T).name();
}