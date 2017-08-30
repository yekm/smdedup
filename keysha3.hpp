#ifndef KEYSHA3_HPP
#define KEYSHA3_HPP

#include "keybase.hpp"

#include "array_print.hpp"

#include <cryptopp/sha3.h>

namespace keys {

template <int N>
class KeySHA3 : public KeyBase<std::array<unsigned char, N>>
{
public:
    typedef KeyBase<std::array<unsigned char, N>> base_type;

    KeySHA3() {}

    virtual void process_chunk(
            typename base_type::reader_type::buffer_type chunk,
            ssize_t len) override
    {
        m_sha.Update(chunk, len);
    }

    virtual void final()
    {
        m_sha.Final(base_type::get_key().data());
    }

private:
    CryptoPP::SHA3_Final<N> m_sha;
};

} // ns keys

#endif // KEYSHA3_HPP
