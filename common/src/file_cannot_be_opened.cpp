#include "../include/file_cannot_be_opened.h"
#include <string>

file_cannot_be_opened::file_cannot_be_opened(
    std::string const &file_path):
    runtime_error("File `" + file_path + "` cannot be opened")
{ }