#ifndef COMMON_HH

#include <vector>
#include <string>

std::vector<std::string> split(const std::string str, const std::string delimiter = " ");
std::string join(const std::vector<std::string> list, const std::string delimiter = ",");

#define COMMON_HH
#endif