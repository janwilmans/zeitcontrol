#include "zeitcontrol/zeitcontrol.h"
#include "zeitcontrol/jthread.h"
#include "zeitcontrol/string_format.h"

#include <chrono>
#include <functional>
#include <memory>

#include <termios.h> // terminal control definitions

using namespace std::literals;

constexpr std::string_view zeitcontrol_port = "/dev/ttyACM0";
using tty_t = struct termios;

// $ sudo adduser $USER dialout
void set_zeit_defaults(int fd)
{
    // Create new termios struct, we call it 'tty' for convention
    tty_t tty{};

    // the struct passed to tcsetattr() must have been initialized with
    // a call to tcgetattr() overwise behaviour is undefined
    if (tcgetattr(fd, &tty) != 0)
    {
        print_errno("tcgetattr");
    }

    // set 9600/8/N/1
    tty.c_cflag &= ~PARENB; // No parity
    tty.c_cflag |= CS8; // 8 bit
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control
    tty.c_cflag |= CREAD | CLOCAL;
    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable any special handling of received bytes
    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    tty.c_cc[VTIME] = 2; // Wait for up to 0.5s (5 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 12;

    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    // Save tty settings, also checking for error
    if (tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        print_errno("tcsetattr");
    }
}

zeitcontrol::zeitcontrol()
{
    m_serialport = std::make_unique<serialport>(zeitcontrol_port);
    set_zeit_defaults(m_serialport->handle());
    m_serialport->write("x");
}

zeitcontrol::~zeitcontrol()
{
    m_stopped = true;
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
        auto id = strip(m_serialport->read());
        if (!id.empty())
        {
            m_callback(id);
        }

        // the zeitcontrol RFID tag will send a continuous stream of data
        // this is will make sure we send a callback only once per second
        // and the flush well make sure, we're not going to read old data
        std::this_thread::sleep_for(1s);
        tcflush(m_serialport->handle(), TCIFLUSH);
    }
}
