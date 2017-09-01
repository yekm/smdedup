#ifndef KEYBASE_HPP
#define KEYBASE_HPP

#include <string>

#include "seqreader.hpp"
#include "mmapreader.hpp"

namespace keys
{

template<typename T>
class KeyBase
{
public:
    typedef T key_type;
    //typedef utils::SeqReader<> reader_type;
    typedef utils::MmapReader reader_type;
    KeyBase() {}
    const key_type & compute(std::string filename)
    {
        m_key = key_type{};
        m_reader = reader_type(filename);
        do {
            auto && [chunk, len] = m_reader.read();
            if (len == 0)
                break;
            base_process_chunk(chunk, len);
        } while(true);
        final();
        return m_key;
    }

    ssize_t get_total_bytes_processed()
    {
        return m_total_len;
    }

protected:
    key_type & get_key()
    {
        return m_key;
    }

private:
    void base_process_chunk(reader_type::buffer_type chunk, /* read_buffer_type ? */
                               ssize_t len)
    {
        m_total_len += len;
        process_chunk(chunk, len);
    }

    virtual void process_chunk(reader_type::buffer_type chunk, /* read_buffer_type ? */
                               ssize_t len) = 0;
    virtual void final() {}
    reader_type m_reader;
    key_type m_key;
    ssize_t m_total_len = 0;
};

} // ns keys
#endif // KEYBASE_HPP
