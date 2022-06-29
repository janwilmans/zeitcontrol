#include "zeitcontrol/serialport.h"
#include "zeitcontrol/string_format.h"

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

serialport::serialport(std::string_view port) :
    m_port(port)
{
    m_fd = open(m_port.data(), O_RDWR);
    if (m_fd < 0)
    {
        throw std::runtime_error(fmt::format("cannot open '{}'\n", m_port));
    }
}

serialport::~serialport()
{
    if (m_fd > 0)
    {
        ::close(m_fd);
    }
}

int serialport::handle() const
{
    return m_fd;
}

std::string serialport::read()
{
    constexpr size_t buffer_size = 512;
    m_buffer.resize(buffer_size);
    auto result = ::read(m_fd, &m_buffer[0], buffer_size);
    if (result > 0)
    {
        m_buffer.resize(result);
        return m_buffer;
    }

    auto err = errno;
    if (err == EAGAIN) // this is normal for non-blocking reads, just retry later
    {
        return "";
    }
    print_errno("read");
    return "";
}

void serialport::write(std::string_view data)
{
    ::write(m_fd, &data[0], data.size());
}
