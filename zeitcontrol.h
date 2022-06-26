#pragma once

#include <functional>
#include <memory>
#include <thread>

#include "serialport.h"
#include "jthread.h"

class zeitcontrol
{
public:
    zeitcontrol();
    ~zeitcontrol();

    std::string read();

    using callback = std::function<void(std::string_view)>;

    // callback is called when a tag is detected and read.
    void subscribe_on_tag_read(callback function);

private:
    void poll();
    std::unique_ptr<serialport> m_serialport;
    callback m_callback;
    std::unique_ptr<zeit::jthread> m_background_thread;
    bool m_stopped = false;
};