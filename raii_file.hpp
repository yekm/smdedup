#ifndef RAII_FILE_HPP
#define RAII_FILE_HPP

#include "errno.hpp"

#include <iostream>
#include <assert.h>

#include <fcntl.h> // open
#include <unistd.h> // close

namespace utils
{

template <int policy> // no use, actually
class raii_file
{
    raii_file (const raii_file &) = delete;
    raii_file & operator= (const raii_file &) = delete;

public:
    raii_file() = default;
    raii_file(raii_file &&) = default;

    raii_file & operator= (raii_file && other) noexcept
    {
        this->~raii_file();
        m_fd = other.m_fd;
        other.m_fd = -1;
    }

    raii_file(std::string filename)
    {
        m_fd = ::open(filename.c_str(), policy);
        if (m_fd == -1)
            throw utils::Errno("open failed for", filename);
    }
/*
    operator const int() const
    {
        return get();
    }
*/
    int get() noexcept
    {
        assert(m_fd != -1);
        return m_fd;
    }

    ~raii_file() noexcept
    {
        if (m_fd != -1)
            assert(::close(m_fd) != -1);
    }

private:
    int m_fd = -1;
};

typedef raii_file<O_RDONLY | O_NOFOLLOW> file_default;

} // ns utils

#endif // RAII_FILE_HPP
