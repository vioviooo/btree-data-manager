#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_COMMON_FILE_CANNOT_BE_OPENED_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_COMMON_FILE_CANNOT_BE_OPENED_H

#include <stdexcept>

class file_cannot_be_opened final:
    public std::runtime_error
{

public:

    explicit file_cannot_be_opened(
        std::string const &file_path);

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_COMMON_FILE_CANNOT_BE_OPENED_H