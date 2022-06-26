#pragma once

#include <string_view>
#include <string>
#include <array>

class serialport
{
public:
    serialport(std::string_view port);
    ~serialport();
    std::string read();
    void write(std::string_view);

private:
    std::string m_port;
    int m_fd = -1;
    std::string m_buffer;
};