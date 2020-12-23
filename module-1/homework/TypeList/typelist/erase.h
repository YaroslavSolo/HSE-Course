#pragma once

#include "typelist.h"

template<typename TList, typename TargetType>
struct Erase;

template<typename Tail, typename TargetType>
struct Erase<TypeList<TargetType, Tail>, TargetType> {
    typedef Tail NewTypeList;
};

template<typename TargetType>
struct Erase<NullType, TargetType> {
    typedef NullType NewTypeList;
};

template<typename TList>
struct Erase<TList, NullType> {
    typedef TList NewTypeList;
};

template<typename Head, typename Tail, typename TargetType>
struct Erase<TypeList<Head, Tail>, TargetType> {
    typedef TypeList<Head, typename Erase<Tail, TargetType>::NewTypeList> NewTypeList;
};
