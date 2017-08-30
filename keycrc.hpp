#ifndef KEYCRC_HPP
#define KEYCRC_HPP

#include "keybase.hpp"

namespace keys
{

class KeyCRC : public KeyBase<uint32_t>
{
public:
    typedef KeyBase<uint32_t> base_type;
    KeyCRC();
private:
    virtual void process_chunk(reader_type::buffer_type chunk, ssize_t len) override;
};

} // ns keys
#endif // KEYCRC_HPP
