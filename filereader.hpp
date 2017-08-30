#ifndef FILEREADER_HPP
#define FILEREADER_HPP

#include <string>
#include <tuple>

namespace utils
{

class FileReader
{
public:
    typedef unsigned char elenemt_type;
    typedef elenemt_type * buffer_type;
    typedef std::tuple<buffer_type, ssize_t> read_buffer_type;

    virtual read_buffer_type read() = 0;
    virtual ~FileReader() {}
};

} // ns utils

#endif // FILEREADER_HPP
