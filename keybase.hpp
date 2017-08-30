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
            process_chunk(chunk, len);
        } while(true);
        final();
        return m_key;
    }

protected:
    key_type & get_key()
    {
        return m_key;
    }

private:
    virtual void process_chunk(reader_type::buffer_type chunk, /* read_buffer_type ? */
                                   ssize_t len) = 0;
    virtual void final() {}
    reader_type m_reader;
    key_type m_key;
};

} // ns keys
#endif // KEYBASE_HPP
