#include "zeitcontrol.h"
#include "jthread.h"

#include <chrono>
#include <functional>
#include <memory>

#define FMT_HEADER_ONLY
#include <fmt/format.h>

using namespace std::literals;

constexpr std::string_view zeitcontrol_port = "/dev/ttyACM0";

zeitcontrol::zeitcontrol()
{
    fmt::print(stderr, "zeitcontrol()\n");
    m_serialport = std::make_unique<serialport>(zeitcontrol_port);
    m_serialport->write("x");
}

zeitcontrol::~zeitcontrol()
{
    fmt::print(stderr, "~zeitcontrol()\n");
    m_stopped = true;
}

std::string zeitcontrol::read()
{
    return m_serialport->read();
}

void zeitcontrol::subscribe_on_tag_read(zeitcontrol::callback function)
{
    m_callback = function;
    m_background_thread = std::make_unique<zeit::jthread>(&zeitcontrol::poll, this);
}

void zeitcontrol::poll()
{
    while (!m_stopped)
    {
        std::this_thread::sleep_for(2000ms);
        auto id = read();
        if (!id.empty())
        {
            m_callback(id);   
        }
    }
}