#include "keycrc.hpp"

extern "C" {
uint32_t crc32c(uint32_t crc, const void *buf, size_t len);
}

namespace keys
{

KeyCRC::KeyCRC()
{

}

KeyCRC::key_type KeyCRC::process_chunk(reader_type::buffer_type &chunk, ssize_t len)
{
    m_key = crc32c(m_key, chunk.data(), len);
}

} // ns keys
