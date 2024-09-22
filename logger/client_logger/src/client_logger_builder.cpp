#include <vector>
#include <fstream>
#include <filesystem>

#include <not_implemented.h>
#include <file_cannot_be_opened.h>
#include <client_logger.h>

#include <nlohmann/json.hpp>

#include "../include/client_logger_builder.h"

client_logger_builder::client_logger_builder():
    _format_string("[%s]: %m")
{ } 

client_logger_builder::client_logger_builder(std::string const &format_string):
    _format_string(format_string)
{ }

logger_builder *client_logger_builder::set_format_string(
        std::string const& format_string)
{
    _format_string = format_string;
    
    return this;
}

logger_builder *client_logger_builder::add_file_stream(
    std::string const &stream_file_path,
    logger::severity severity)
{
    std::string abs_path = std::filesystem::weakly_canonical(stream_file_path).string();
    _configuration[abs_path].insert(severity);
    
    return this;
}

logger_builder *client_logger_builder::add_console_stream(
    logger::severity severity)
{
    _configuration[""].insert(severity);
    
    return this;
}

logger_builder* client_logger_builder::transform_with_configuration(
    std::string const &configuration_file_path,
    std::string const &configuration_path)
{
    std::ifstream stream(configuration_file_path);
    
    if (!stream.is_open())
    {
        throw file_cannot_be_opened(configuration_file_path);
    }
    
    std::vector<std::string> data_path_components;
    
    for (size_t ind = 0; ind < configuration_path.size(); )
    {
        size_t tmp_ind = std::min(configuration_path.find(':', ind), configuration_path.size());
        
        std::string component = configuration_path.substr(ind, tmp_ind - ind);
        if (component.size() == 0)
        {
            throw std::runtime_error("Invalid JSON data path");
        }
        
        data_path_components.push_back(std::move(component));
        ind = tmp_ind + 1;
    }
    
    nlohmann::json json_obj = nlohmann::json::parse(stream);
    
    for (auto path_elem : data_path_components)
    {
        json_obj = json_obj[path_elem];
    }
    
    clear();
    
    _format_string = json_obj["format_string"];
    json_obj = json_obj["logger_files"];
    
    for (auto &[file_path, severities] : json_obj.items())
    {
        for (std::string severity_str : severities)
        {
            logger::severity severity = client_logger::string_to_severity(severity_str);
            
            if (file_path == "console")
            {
                add_console_stream(severity);
            }
            else
            {
                add_file_stream(file_path, severity);
            }
        }
    }
    
    return this;
}

logger_builder *client_logger_builder::clear()
{
    _format_string = "[%s]: %m";
    _configuration.clear();
    
    return this;
}

logger *client_logger_builder::build() const
{
    return new client_logger(_format_string, _configuration);
}
