
#include "serialport.h"
#include "string_format.h"

#include <unistd.h>
#include <fcntl.h>

void set_serial_defaults(int)
{
    // set 9600/8/N/1
}

serialport::serialport(std::string_view port) : 
    m_port(port)
{
    m_fd = open(m_port.data(), O_RDWR | O_NONBLOCK);
    if (m_fd < 0)
    {
        throw std::runtime_error(fmt::format("cannot open '{}'\n", m_port));
    }
    //set_serial_defaults(m_fd);
}

serialport::~serialport()
{
    fmt::print(stderr, "~serialport\n");

    if (m_fd > 0)
    {
        ::close(m_fd);
    }
}

void print_errno(int err)
{
    fmt::print(stderr, "errno: {} - {}\n", err, strerror(err));
}

std::string serialport::read()
{
    constexpr size_t buffer_size = 512;
    m_buffer.resize(buffer_size);
    auto result = ::read(m_fd, &m_buffer[0], buffer_size);
    if (result > 0)
    {
        m_buffer.resize(result);
        fmt::print(stderr, "read result [{}]: '{}'\n", result, m_buffer);
        return m_buffer;
    }
    
    auto err = errno;
    if (err == EAGAIN)
    {
        fmt::print(stderr, "read EAGAIN\n");
        return "";
    }
    fmt::print(stderr, "read error [{}]\n", result);
    print_errno(err);
    return "";
}

void serialport::write(std::string_view data)
{
    fmt::print(stderr, "write: '{}'\n", data);
    ::write(m_fd, &data[0], data.size());
}