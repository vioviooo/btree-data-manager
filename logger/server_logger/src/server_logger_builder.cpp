#include <filesystem>
#include <fstream>

#include <not_implemented.h>
#include <file_cannot_be_opened.h>
#include <server_logger.h>

#include <nlohmann/json.hpp>

#include "../include/server_logger_builder.h"

logger_builder *server_logger_builder::add_file_stream(
    std::string const &stream_file_path,
    logger::severity severity)
{
    std::string abs_path = std::filesystem::weakly_canonical(stream_file_path).string();
    _configuration[abs_path].insert(severity);
    
    return this;
}

logger_builder *server_logger_builder::add_console_stream(
    logger::severity severity)
{
    _configuration[""].insert(severity);
    
    return this;
}

logger_builder* server_logger_builder::transform_with_configuration(
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
    
    json_obj = json_obj["logger_files"];
    
    for (auto &[file_path, severities] : json_obj.items())
    {
        for (std::string severity_str : severities)
        {
            logger::severity severity = server_logger::string_to_severity(severity_str);
            
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

logger_builder *server_logger_builder::clear()
{
    _configuration.clear();
    
    return this;
}

logger *server_logger_builder::build() const
{
    return new server_logger(_configuration);
}