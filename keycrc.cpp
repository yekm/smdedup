#include "keycrc.hpp"

extern "C" {
uint32_t crc32c(uint32_t crc, const void *buf, size_t len);
}

namespace keys
{

KeyCRC::KeyCRC()
{

}

void KeyCRC::process_chunk(reader_type::buffer_type chunk, ssize_t len)
{
    base_type::get_key() = crc32c(base_type::get_key(), chunk, len);
}

} // ns keys
