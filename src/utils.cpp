#include "../include/utils.h"

/**
 * @brief Return a vector of integers from a comma-delimited string.
 *  (could have been templated if you can template stoi and stod).
 * @param str The comma-delimited string.
 * @return std::vector<int> The output vector of integers.
 */
std::vector<int> Utils::analog_str_buf_to_int_vec(const std::string& str)
{
    // Initialize data vector
    std::vector<int> data_vec;

    // Initialize string stream
    std::stringstream ss(str);

    // Get data and convert to analog
    while (ss.good())
    {
        std::string substr;
        std::getline(ss, substr, ',');
        data_vec.push_back(std::stoi(substr));
    }

    return data_vec;
}

/**
 * @brief Return a vector of doubles from a comma-delimited string.
 * 
 * @param str The comma-delimited string.
 * @return std::vector<int> The output vector of doubles.
 */
std::vector<double> Utils::analog_str_buf_to_double_vec(const std::string& str)
{
    // Initialize data vector
    std::vector<double> data_vec;

    // Initialize string stream
    std::stringstream ss(str);

    // Get data and convert to analog
    while (ss.good())
    {
        std::string substr;
        std::getline(ss, substr, ',');
        data_vec.push_back(std::stod(substr));
    }

    return data_vec;
}