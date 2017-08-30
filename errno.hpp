#ifndef ERRNO_HPP
#define ERRNO_HPP

#include <sstream>
#include <exception>
#include <string.h> // strerror

namespace utils
{

namespace lol
{

template <typename T>
std::ostream & operator ^ (std::ostream & ss, const T & v)
{
    return ss << v << " ";
}

} // ns lol

template<typename ...Args>
std::string all_to_string(Args&&... args) {
    using lol::operator ^;

    std::stringstream ss;
    (ss ^ ... ^ args);
    return ss.str();
}

template<typename... Args>
class Errno : public std::runtime_error
{
public:
    Errno(Args... a)
        : std::runtime_error(all_to_string(a...) + strerror(errno))
    {}
};

} // ns utils

#endif // ERRNO_HPP
