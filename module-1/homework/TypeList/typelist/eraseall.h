#pragma once

#include "typelist.h"

template<typename TList, typename TargetType>
struct EraseAll;

template<typename Tail, typename TargetType>
struct EraseAll<TypeList<TargetType, Tail>, TargetType> {
    typedef typename EraseAll<Tail, TargetType>::NewTypeList NewTypeList;
};

template<typename TargetType>
struct EraseAll<NullType, TargetType> {
    typedef NullType NewTypeList;
};

template<typename TList>
struct EraseAll<TList, NullType> {
    typedef TList NewTypeList;
};

template<typename Head, typename Tail, typename TargetType>
struct EraseAll<TypeList<Head, Tail>, TargetType> {
    typedef TypeList<Head, typename EraseAll<Tail, TargetType>::NewTypeList> NewTypeList;
};
