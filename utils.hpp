#ifndef UTILS_HPP
#define UTILS_HPP

#include <map>
#include <fstream>
#include <streambuf>

#include <iostream>
#include <chrono>
#include <thread>

namespace utils {

//template <typename K, typename V>
template <typename T>
void pmap(const T & m)
{
    for (auto && [k, v] : m)
    {
        std::cout << k << ":" << v << " ";
    }
    std::cout << std::endl;
}

void print_stat()
{
    std::ifstream s("/proc/self/status");
    std::string str((std::istreambuf_iterator<char>(s)),
                    std::istreambuf_iterator<char>());

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(20000s);
}

} // ns utils

#endif // UTILS_HPP