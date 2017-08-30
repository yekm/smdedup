#pragma once

#include <array>
#include <exception>
#include <fcntl.h>
#include <unistd.h>

#include "filereader.hpp"
#include "errno.hpp"

namespace utils
{

template <int N = 4096*32>
class SeqReader : public FileReader
{
public:
    SeqReader(const SeqReader &) = delete;
    SeqReader & operator= (const SeqReader &) = delete;
    SeqReader(SeqReader &&) = delete; // or *this = std::move(that)?

    SeqReader() = default;

    SeqReader(const std::string & filename)
    {
        m_fd = ::open(filename.c_str(), O_RDONLY | O_NOFOLLOW);
        if (m_fd == -1)
            throw utils::Errno("open failed for", filename);

        posix_fadvise(m_fd, 0, 0, POSIX_FADV_SEQUENTIAL);
    }

    SeqReader & operator = (SeqReader && other)
    {
        close();
        m_fd = other.m_fd;
        m_array = std::move(other.m_array);
        other.m_fd = -1;
        return *this;
    }


    virtual FileReader::read_buffer_type read() override
    {
        ssize_t bytes_read = ::read(m_fd, m_array.data(), N);
        return {m_array.data(), bytes_read};
    }

    ~SeqReader()
    {
        close();
    }

private:
    void close()
    {
        if (m_fd != -1)
            ::close(m_fd);
    }

    int m_fd = -1;
    std::array<FileReader::elenemt_type, N> m_array;
};

} // ns utils
