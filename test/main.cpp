#include "zeitcontrol/string_format.h"
#include "zeitcontrol/zeitcontrol.h"

#include <cstddef>
#include <cstdio>
#include <string>

using namespace std::literals;

// EM4102 is the 'blue drop' type of RF tag from EM MICROELECTRONIC
// EM4102 (read only)
// EM4150, HITAG-1, HITAG-2 (read/write)

// stty -F /dev/ttyACM0 raw
// picocom -b 9600 /dev/ttyACM0
// exit picocom with CTRL-A followed by CTRL-X

void tag_reader_main()
{
    zeitcontrol tagreader;
    tagreader.subscribe_on_tag_read([](std::string_view id) { fmt::print(stderr, "read: '{}'\n", id); });
    while (1)
    {
        std::this_thread::sleep_for(200ms);
    }
}

int main()
{
    try
    {
        tag_reader_main();
    }
    catch (const std::exception & e)
    {
        fmt::print("Exception: {}\n", e.what());
    }
}
