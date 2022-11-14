#pragma once

#include <vector>
#include <memory>

template<typename T>
struct DataIsland
{
    DataIsland()
    {

    }

    std::vector<std::shared_ptr<T>> Data;
};

template<typename T>
inline DataIsland<T>& GDataIsland()
{
    static DataIsland<T> instance;
    return instance;
}

