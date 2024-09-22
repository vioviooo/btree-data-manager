#include <gtest/gtest.h>

#include <client_logger.h>
#include <client_logger_builder.h>
#include <extra_utility.h>

#include <fstream>
#include <set>
#include <filesystem>

int main(
    int argc,
    char *argv[])
{
    
    std::string config_path = "/home/remi/Code/VSCode/os_cw/logger/client_logger/tests/config_example.json";
    
    logger_builder *builder = new client_logger_builder("%d %t - [%s] %m");
    
    /*
    logger *lg = builder->clear()
        ->add_console_stream(logger::severity::trace)
        ->add_file_stream("./aboba", logger::severity::trace)
        ->add_file_stream("../tests/aboba", logger::severity::debug)
        ->build();
    */
    
    logger *lg = builder
        ->transform_with_configuration(config_path, "wrapper:client_logger_builder.config")
        ->build();
    
    //logger *lg2 = new client_logger(*lg);
    
    lg->trace("TRACE!!!");
    lg->debug("DEBUG!!!");
    lg->information("INFORMATION!!!");
    
    delete builder;
    delete lg;
    //delete lg2;
}
