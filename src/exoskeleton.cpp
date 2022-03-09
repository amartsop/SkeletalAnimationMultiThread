#include "../include/exoskeleton.h"

/**
 * @brief It initialiazes the serial communication and sets the asychronous 
 * callback function. See https://en.cppreference.com/w/cpp/thread/async.
 * 
 * @param serial_com The serial communication port.
 * @param serial_baudrate The serial communication baudrate.
 */
void Exoskeleton::initialize(const std::string& serial_com,
    unsigned int serial_baudrate)
{
    // Generate serial communication channel
    m_serial = std::make_shared<SerialCOM>(serial_com, serial_baudrate);

    // Initialize stream
    m_serial->initialize_stream();

    // Initialize asychronous function
    m_future_fun = std::async(&Exoskeleton::incoming_data_callback, this);
}

/**
 * @brief This is the callback function for reading asynchronously the incoming 
 * data from the serial port. The data come in the format:
 *\f[
 *    data =\left[{\theta}_{i_1}, {\theta}_{i_2}, {\theta}_{i_3}, {\theta}_{i_4},
 *    {\theta}_{m_1}, {\theta}_{m_2}, {\theta}_{m_3}, {\theta}_{m_4},
 *    {\theta}_{t_1}, {\theta}_{t_2}, {\theta}_{t_3}, {\theta}_{t_4},
 *    {\theta}_{t_5}
 *          \right]^{T}
 * \f]
 *   \image html hand_kinematics.png width=600px
 * @return std::vector<double> The raw angle data coming from the exoskeleton board.
 * 
 */
std::vector<double> Exoskeleton::incoming_data_callback(void)
{
    while(!m_return_value)
    {
        m_raw_sensor_data =
            Utils::analog_str_buf_to_double_vec(m_serial->readLine());
    }

    return m_raw_sensor_data;
}

/**
 * @brief It is the point of entry that feeds the animation
 * loop with the exoskeleton data. It returns a vector
 * of the raw jont angle data converted to radians. It also manages the 
 * asynchronous threading.
 * @return std::vector<double> The joint angles.
 */
std::vector<double> Exoskeleton::get_joint_angles(void)
{
    // Set termination flag for asychronous function
    m_return_value = 1;

    // Get joint angles from future function
    std::vector<double> joint_angles = m_future_fun.get();

    // Convert to rad
    for (size_t i = 0; i < joint_angles.size(); i++)
    {
        joint_angles.at(i) = Utils::deg2rad(joint_angles.at(i));
    }

    // Reinitialize async function
    m_return_value = 0;
    m_future_fun = std::async(&Exoskeleton::incoming_data_callback, this);

    return joint_angles;
}