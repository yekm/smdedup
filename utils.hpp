#ifndef UTILS_HPP
#define UTILS_HPP

#include <map>
#include <fstream>
#include <streambuf>

#include <iostream>
#include <chrono>
#include <thread>

template <typename T, size_t N>
std::ostream& operator<<(std::ostream& stream, const std::array<T, N>& v)
{
    for (const auto & x : v)
        stream << std::hex << (unsigned) x;
    return stream;
}

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

    std::cout << str << std::endl;
    //using namespace std::chrono_literals;
    //std::this_thread::sleep_for(20000s);
}

} // ns utils

#endif // UTILS_HPP
