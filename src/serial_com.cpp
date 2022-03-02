#include "../include/serial_com.h"

/**
 * Write a string to the serial device.
    * \param s string to write.
    * \throws boost::system::system_error on failure.
    */
void SerialCOM::writeString(std::string s)
{
    boost::asio::write(serial,boost::asio::buffer(s.c_str(),s.size()));
}

/**
 * Blocks until a line is received from the serial device.
    * Eventual '\n' or '\r\n' characters at the end of the string are removed.
    * \return a string containing the received line.
    * \throws boost::system::system_error on failure.
    */
std::string SerialCOM::readLine()
{
    //Reading data char by char, code is optimized for simplicity, not speed
    using namespace boost;
    char c;
    std::string result;
    for(;;)
    {
        asio::read(serial,asio::buffer(&c,1));
        switch(c)
        {
            case '\r':
                break;
            case '\n':
                return result;
            default:
                result+=c;
        }
    }
}

/**
 * @brief Setup up stream by reading the values a couple times first.
 * 
 * @param iter Number of times to read for warming up.
 */
void SerialCOM::initialize_stream(int iter)
{
    // Read first (iter) lines to start
    for (size_t i = 0; i < iter; i++)
    {
      std::string incoming_str = this->readLine();
    }
}