#pragma once 

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <math.h>


/// Class Utils
/**
 * This class includes some generic function that are used in various places 
 * inside the project.
*/
class Utils
{
public:
    /// Constructor
    Utils();

    /// Range stucture (templated)
    template <typename T>
    struct Range
    {
        T min, max;
    };

    /// Convert degrees to rad
    inline static double deg2rad(double deg) { return (deg * (M_PI / 180.0)); }
    
    /// Convert comma-delimited string to int vector of analog values
    static std::vector<int> analog_str_buf_to_int_vec(const std::string& str);

    /// Convert comma-delimited string to double vector of analog values
    static std::vector<double> analog_str_buf_to_double_vec(const std::string& str);
};