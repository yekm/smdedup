#ifndef KEYCRC_HPP
#define KEYCRC_HPP

#include "keybase.hpp"

namespace keys
{

class KeyCRC : public KeyBase<uint32_t>
{
public:
    KeyCRC();
private:
    virtual key_type process_chunk(reader_type::buffer_type & chunk, ssize_t len) override;
};

} // ns keys
#endif // KEYCRC_HPP
