#pragma once

#include <array>
#include <string>
#include <string_view>

class serialport
{
public:
    serialport(std::string_view port);
    ~serialport();
    std::string read();
    void write(std::string_view);
    int handle() const;

private:
    std::string m_port;
    int m_fd = -1;
    std::string m_buffer;
};
