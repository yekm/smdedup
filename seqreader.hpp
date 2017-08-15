#pragma once

#include <string>
#include <array>
#include <tuple>
#include <exception>
#include <fcntl.h>
#include <unistd.h>

namespace utils
{

template <int N = 4096*32>
class SeqReader
{
public:
    typedef std::array<char, N> buffer_type;
    SeqReader()
    {
    }

    std::tuple<buffer_type &, ssize_t> read()
    {
        ssize_t bytes_read = ::read(m_fd, m_array.data(), N);
        return {m_array, bytes_read};
    }

    void reset(const std::string & filename)
    {
        if (m_fd != 0)
            ::close(m_fd);

        m_fd = ::open(filename.c_str(), O_RDONLY | O_NOFOLLOW);
        if (m_fd == -1)
            throw std::runtime_error("open failed for " + filename); // TODO: meaningful exceptions

        posix_fadvise(m_fd, 0, 0, POSIX_FADV_SEQUENTIAL);
    }

    ~SeqReader()
    {
        ::close(m_fd);
    }

private:
    int m_fd;
    buffer_type m_array;
};

} // ns utils
