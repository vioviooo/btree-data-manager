#include <sstream>
#include <iomanip>

#include <../include/allocator.h>

std::string allocator::get_block_dump(
    block_pointer_t block,
    block_size_t size)
{
    std::ostringstream str_stream(size > 0 ? " with data:" : "", std::ios::ate);
    str_stream << std::hex << std::setfill('0');
    
    for (size_t i = 0; i < size; ++i)
    {
        str_stream << " 0x" << std::setw(2) << static_cast<unsigned int>(
                *reinterpret_cast<unsigned char*>(block) + i);
    }
    
    return str_stream.str();
}