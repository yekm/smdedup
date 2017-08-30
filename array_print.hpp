#ifndef ARRAY_PRINT_HPP
#define ARRAY_PRINT_HPP

#include <ostream>
#include <array>

template <typename T, size_t N>
std::ostream& operator<<(std::ostream& stream, const std::array<T, N>& v)
{
    for (const auto & x : v)
        stream << std::hex << (unsigned) x;
    return stream;
}

#endif // ARRAY_PRINT_HPP
