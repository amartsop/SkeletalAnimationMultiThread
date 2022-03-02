#pragma once 

#include <boost/asio.hpp>

/// Class SerialCOM
/**
 * This class handles all the serial communication between the PC and 
 * the exoskeleton board. It's based on boost::asio. See
 * https://www.boost.org/doc/libs/1_75_0/doc/html/boost_asio.html.
*/

class SerialCOM
{
public:
    /**
     * Constructor.
     * \param port device name, example "/dev/ttyUSB0" or "COM4".
     * \param baud_rate communication speed, example 9600 or 115200.
     * \throws boost::system::system_error if cannot open the
     * serial device.
     */
    /// Constructor
    SerialCOM(std::string port, unsigned int baud_rate)
    : io(), serial(io,port)
    {
        serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
    }

    /// Write a string to the serial device.
    void writeString(std::string s);

    /// Blocks until a line is received from the serial device.
    std::string readLine(void);

    // Initialize stream.
    void initialize_stream(int iter=3);

private:
    /// Boost io service.
    boost::asio::io_service io;
    
    /// Boost serial port handle. 
    boost::asio::serial_port serial;
};