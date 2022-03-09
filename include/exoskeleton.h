#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <thread>

#include "utils.h"
#include "serial_com.h"

/// Class Exoskeleton
/**
 * This class is the interface between the hardware and the software. 
 * Its goal is to provide a callback function that reads asychronously 
 * the serial data from the SerialCOM:: class and translates it to 
 * joint angle values. It then sends the data to the animation loop and to the 
 * rendering engine
*/
class Exoskeleton
{

public:

    /// Empty constructor.
    Exoskeleton() {};

    /// Initialize the exoskeleton.
    void initialize(const std::string& serial_com, unsigned int serial_baudrate);

    /// Read incoming data.
    std::vector<double> incoming_data_callback(void);

    /// Get joint angles.
    std::vector<double> get_joint_angles(void);

private:

    /// Measurements num.
    const int m_meas_num = 13;

    /// Sensors data vector (raw).
    std::vector<double> m_raw_sensor_data;

private:

    /// Serial communication handler.
    std::shared_ptr<SerialCOM> m_serial;

    /// Future function handle.
    std::future<std::vector<double>> m_future_fun;

    /// Termination flag for callback function.
    bool m_return_value = 0;
};