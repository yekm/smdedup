#ifndef MMAPREADER_HPP
#define MMAPREADER_HPP

#include "filereader.hpp"

#include "raii_file.hpp"
#include "errno.hpp"

#include <sys/mman.h> // mmap
#include <string.h> // strerror
#include <experimental/filesystem>

#include <sys/stat.h>

namespace fs = std::experimental::filesystem;

namespace utils
{

class MmapReader : public FileReader
{
public:
    MmapReader(const MmapReader &) = delete;
    MmapReader & operator =(const MmapReader &) = delete;
    MmapReader(MmapReader &&) = delete;

    MmapReader() = default;

    MmapReader(std::string filename)
    {
        m_file = utils::file_default(filename);
        m_len = fs::file_size(filename);

        if (m_len == 0)
            return;

        m_addr = ::mmap(nullptr, m_len, PROT_READ, MAP_POPULATE|MAP_PRIVATE, m_file.get(), 0u);

        if (m_addr == MAP_FAILED)
            throw utils::Errno("mmap failed for", filename);
    }

    MmapReader & operator = (MmapReader && other)
    {
        unmap();
        m_file = std::move(other.m_file);
        m_all_read = other.m_all_read;
        m_len = other.m_len;
        m_addr = other.m_addr;
        other.m_addr = MAP_FAILED;
        other.m_len = 0;
        other.m_all_read = true;
        return *this;
    }

    virtual read_buffer_type read() override
    {
        if (m_all_read)
            return {0, 0};
        else
        {
            m_all_read = true;
            return {static_cast<buffer_type>(m_addr), m_len};
        }
    }

    virtual ~MmapReader() override
    {
        unmap();
    }

private:
    void unmap()
    {
        if (m_addr != MAP_FAILED)
        {
            if (munmap(m_addr, m_len) == -1)
                std::cerr << "munmap failed" << std::endl;
            m_addr = MAP_FAILED;
            m_len = 0;
        }
    }

    utils::file_default m_file;
    void * m_addr = MAP_FAILED;
    size_t m_len = 0;
    bool m_all_read = false;
    read_buffer_type m_map = {nullptr, 0};
};

} // ns utils

#endif // MMAPREADER_HPP
