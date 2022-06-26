#pragma once

#include <thread>

namespace zeit
{

class jthread : public std::thread
{
public:
    using std::thread::thread;

    ~jthread()
    {
        if (joinable())
        {
            join();
        }
    }
};

}