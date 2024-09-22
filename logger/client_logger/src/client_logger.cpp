#include <fstream>

#include <not_implemented.h>
#include <file_cannot_be_opened.h>

#include "../include/client_logger.h"

std::unordered_map<std::string, std::pair<std::ostream *, size_t>> client_logger::_all_streams;

client_logger::client_logger(
    std::string const &format_string,
    std::map<std::string, std::set<logger::severity>> const &configuration):
    _format_string(format_string)
{
    for (auto iter = configuration.begin(); iter != configuration.end(); ++iter)
    {
        const std::string &file_path = iter->first;
        const std::set<logger::severity> &severities = iter->second;
        
        if (file_path.size() == 0)
        {
            _streams[""] = std::make_pair(&std::cout, severities);
            continue;
        }
        
        if (!_all_streams.count(file_path))
        {
            std::ofstream *stream = nullptr;
            
            try
            {
                stream = new std::ofstream(file_path);
                
                if (!stream->is_open())
                {
                    throw file_cannot_be_opened(file_path);
                }
            }
            catch (const std::exception&)
            {
                delete stream;
                
                for (auto del_iter = configuration.begin(); del_iter != iter; ++del_iter)
                {
                    decrement_stream_users(del_iter->first);
                }
                
                throw;
            }
            
            _all_streams[file_path] = std::make_pair(stream, size_t(0));
        }
        
        _streams[file_path] = std::make_pair(_all_streams[file_path].first, severities);
        _all_streams[file_path].second++;
    }
}

client_logger::client_logger(
    client_logger const &other):
    _format_string(other._format_string),
    _streams(other._streams)
{
    for (auto record : _streams)
    {
        _all_streams[record.first].second++;
    }
}

client_logger &client_logger::operator=(
    client_logger const &other)
{
    if (this == &other)
    {
        return *this;
    }
    
    for (auto record : _streams)
    {
        decrement_stream_users(record.first);
    }
    
    _format_string = other._format_string;
    _streams = other._streams;
    
    for (auto record : _streams)
    {
        _all_streams[record.first].second++;
    }
    
    return *this;
}

client_logger::client_logger(
    client_logger &&other) noexcept:
    _format_string(std::move(other._format_string)),
    _streams(std::move(other._streams))
{ }

client_logger &client_logger::operator=(
    client_logger &&other) noexcept
{
    if (this == &other)
    {
        return *this;
    }
    
    for (auto record : _streams)
    {
        decrement_stream_users(record.first);
    }
    
    _format_string = std::move(other._format_string);
    _streams = std::move(other._streams);
    
    return *this;
}


client_logger::~client_logger() noexcept
{
    for (auto record : _streams)
    {
        decrement_stream_users(record.first);
    }
}

logger const *client_logger::log(
    std::string const &text,
    logger::severity severity) const noexcept
{
    std::string datetime = logger::global_datetime_to_string();
    
    for (auto record : _streams)
    {
        std::ostream &stream = *(record.second.first);
        auto &severities = record.second.second;
        
        if (severities.count(severity))
        {
            for (size_t i = 0; i < _format_string.size(); ++i)
            {
                if (_format_string[i] == '%')
                {
                    if (_format_string[i+1] == 'd')
                    {
                        stream << datetime.substr(0, 10);
                    }
                    else if (_format_string[i+1] == 't')
                    {
                        stream << datetime.substr(11, 8);
                    }
                    else if (_format_string[i+1] == 's')
                    {
                        stream << logger::severity_to_string(severity);
                    }
                    else if (_format_string[i+1] == 'm')
                    {
                        stream << text;
                    }
                    else
                    {
                        stream << _format_string[i] << _format_string[i+1];
                    }
                    
                    ++i;
                }
                else
                {
                    stream << _format_string[i];
                }
            }
            
            stream << std::endl;
        }
    }
    
    return this;
}

void client_logger::decrement_stream_users(std::string const &file_path) const noexcept
{
    if (file_path.size() == 0)
    {
        return;
    }
    
    auto iter = _all_streams.find(file_path);
    
    auto &stream = iter->second.first;
    auto &counter = iter->second.second;
    
    counter--;
    
    if (counter == 0)
    {
        stream->flush();
        delete stream;
        _all_streams.erase(iter);
    }
}
