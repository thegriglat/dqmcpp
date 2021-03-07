#ifndef COMMON_HH

#include <vector>
#include <string>
#include <algorithm>
#include <assert.h>
#include <bits/stl_function.h>

/**
 * @brief Splits std::string by delimiter
 *
 * @param str
 * @param delimiter
 * @return std::vector<std::string>
 */
std::vector<std::string> split(const std::string str, const std::string delimiter = " ");
/**
 * @brief Joins vector of strings with delimiter
 *
 * @param list
 * @param delimiter
 * @return std::string
 */
std::string join(const std::vector<std::string> list, const std::string delimiter = ",");

/**
 * @brief Returns T* of maximum value from std::vector<T>
 *
 * @tparam T
 * @param list
 * @return T*
 */
template<class T>
T* maximum(std::vector<T> &list)
{
    // TODO: do we need const here?
    assert(list.size() != 0);
    T* maxel = &(list.at(0));
    auto len = list.size();
    for (int i = 1; i < len; ++i) {
        T* e = &(list.at(i));
        if(*e > *maxel) {
            maxel = e;
        }
    }
    return maxel;
}

template <class T>
bool has(const std::vector<T> &data, const T &elem)
{
    return std::find(data.begin(), data.end(), elem) != data.end();
}

#define COMMON_HH
#endif