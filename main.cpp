#include <cstdio>
#include <array>
#include <cstddef>
#include <string>

#define FMT_HEADER_ONLY
#include <fmt/format.h>

#include "zeitcontrol.h"

using namespace std::literals;

// EM4102 is the 'blue drop' type of RF tag from EM MICROELECTRONIC
// EM4102 (read only)
// EM4150, HITAG-1, HITAG-2 (read/write)

// stty -F /dev/ttyACM0 -a
// picocom -b 9600 /dev/ttyACM0
// sudo socat - /dev/ttyACM0,raw,echo=0,sane

// ASCII protocol:
// send 'r' 
// response: 'N' if no data is read
// response: 'U3400B55DDC' example tag content

// send 'x'
// response: 'MULTITAG 0.12b'

// send 'w'
// response: 'N'

// send 'y'
// response: '>?'



// check  setvbuff
// fread using 

void tag_reader_main()
{
    zeitcontrol tagreader;
    tagreader.subscribe_on_tag_read([](std::string_view id)
    {
        fmt::print(stderr, "read: '{}'\n", id);
    });

    while(1) std::this_thread::sleep_for(200ms);
}

int main()
{
    try
    {
        tag_reader_main();
    }
    catch(const std::exception& e)
    {
        fmt::print("Exception: {}\n", e.what());
    }

    // std::string port = "/dev/ttyACM0";
    // auto fd = fopen(port.data() ,"rb");
    // if (fd == nullptr)
    // {
    //     fmt::print("could not open: '{}'\n", port);
    //     return -1;
    // }
    
    // // initialize
    // std::string start_token = "x";
    // std::fwrite(start_token.data(), 1, start_token.size(), fd);

    // std::array<std::byte, 32> buffer;
    // auto bytesread = fread(buffer.data(), 1, buffer.size(), fd);
    // if (bytesread != buffer.size())
    // {
    //     // eof ?
    // }

    // fmt::print("read: {}\n", bytesread);
    // fclose (fd);

    // return 0;
}