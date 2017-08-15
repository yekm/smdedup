#ifndef KEYBASE_HPP
#define KEYBASE_HPP

#include <string>

#include "seqreader.hpp"

namespace keys
{

template<typename T>
class KeyBase
{
public:
    typedef T key_type;
    typedef utils::SeqReader<> reader_type;
    KeyBase() {}
    key_type compute(std::string filename)
    {
        m_key = key_type{};
        m_reader.reset(filename);
        do {
            auto && [chunk, len] = m_reader.read();
            if (len == 0)
                break;
            process_chunk(chunk, len);
        } while(true);
        return m_key;
    }

protected:
    key_type m_key; // protected.. uh..

private:
    virtual key_type process_chunk(reader_type::buffer_type & chunk, ssize_t len) = 0;
    reader_type m_reader;
};

} // ns keys
#endif // KEYBASE_HPP
