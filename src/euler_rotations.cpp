#include "../include/euler_rotations.h"

/**
 * @brief 
 * Return the basic rotation matrix around x axis by a given angle x.
 * @param x The rotation angle.
 * @return Eigen::Matrix3d The basic rotation matrix around x axis.
 */
Eigen::Matrix3d EulerRotations::basic_rotation_x(double x)
{
    // Matrix initialization
    Eigen::Matrix3d m;

    m << 1.0f, 0.0f, 0.0f, 
        0.0f, cos(x), -sin(x), 
        0.0f, sin(x), cos(x);
    return m;
}

/**
 * @brief 
 * Return the basic rotation matrix around y axis by a given angle x.
 * @param x The rotation angle.
 * @return Eigen::Matrix3d The basic rotation matrix around y axis.
 */
Eigen::Matrix3d EulerRotations::basic_rotation_y(double x)
{
    // Matrix initialization
    Eigen::Matrix3d m;

    m << cos(x), 0.0f, sin(x), 
        0.0f, 1.0f, 0.0f, 
        -sin(x), 0.0f, cos(x);
    return m;
}

/**
 * @brief 
 * Return the basic rotation matrix around z axis by a given angle x.
 * @param x The rotation angle.
 * @return Eigen::Matrix3d The basic rotation matrix around z axis.
 */
Eigen::Matrix3d EulerRotations::basic_rotation_z(double x)
{
    // Matrix initialization
    Eigen::Matrix3d m;

    m << cos(x), -sin(x), 0.0f, 
        sin(x), cos(x), 0.0f, 
        0.0f, 0.0f, 1.0f;
    return m;
}


/**
 * @brief Compound rotation matrix given three Euler angles
 * The Euler angles follow the post multiply * sequence zyx. Rotate
 * "psi" around Z (yaw), "theta" around y (pitch)
 * and "phi" around x (roll).
 * Return the basic rotation matrix around z axis by a given angle x.
 * @param phi Roll angle around x axis. (rad)
 * @param theta Pitch angle around y axis. (rad)
 * @param psi  Yaw angle around z axis. (rad)
 * @return Eigen::Matrix3d The compound rotation matrix.
 */
Eigen::Matrix3d EulerRotations::rotation(double phi, double theta, double psi)
{
    // Matrix initialization
    Eigen::Matrix3d m;

    // Basic rotations
    Eigen::Matrix3d rotx = basic_rotation_x(phi);
    Eigen::Matrix3d roty = basic_rotation_y(theta);
    Eigen::Matrix3d rotz = basic_rotation_z(psi);

    // Total rotation matrix
    m = rotz * roty * rotx;

    return m;
}

/**
 * \overload Eigen::Matrix3d EulerRotations::rotation(Eigen::Vector3d euler_angles)
 */
Eigen::Matrix3d EulerRotations::rotation(Eigen::Vector3d euler_angles)
{
    return rotation(euler_angles(0), euler_angles(1), euler_angles(2));
}

/**
 * \overload Eigen::Matrix3d EulerRotations::rotation(std::vector<double> euler_angles)
 */
Eigen::Matrix3d EulerRotations::rotation(std::vector<double> euler_angles)
{
    return rotation(euler_angles[0], euler_angles[1], euler_angles[2]);
}

/**
 * \overload Eigen::Matrix3d EulerRotations::rotation(Euler euler_angles)
 */
Eigen::Matrix3d EulerRotations::rotation(Euler euler_angles)
{
    return rotation(euler_angles.phi, euler_angles.theta, euler_angles.psi);
}

/**
 * @brief 
 * This method will return a custom Quaternions struct from a
 * set of Euler angles. Euler angles follow the post multiply 
 * sequence zyx. Rotate "psi" around Z (yaw), "theta" around y (pitch)
 * and "phi" around x (roll).
 * @param phi Roll angle around x axis. (rad)
 * @param theta Pitch angle around y axis. (rad)
 * @param psi  Yaw angle around z axis. (rad)
 * @return EulerRotations::Quaternions Quaterinos struct.
 */
EulerRotations::Quaternions EulerRotations::euler_to_quaternions(double phi,
    double theta, double psi)
{
    EulerRotations::Quaternions quatern;

    double cy = cos(psi * 0.5); double sy = sin(psi * 0.5);
    double cp = cos(theta * 0.5); double sp = sin(theta * 0.5);
    double cr = cos(phi * 0.5); double sr = sin(phi * 0.5);

    quatern.w = cr * cp * cy + sr * sp * sy;
    quatern.x = sr * cp * cy - cr * sp * sy;
    quatern.y = cr * sp * cy + sr * cp * sy;
    quatern.z = cr * cp * sy - sr * sp * cy;

    // Return Quaternions
    return quatern;
}


/**
 * @brief 
 * This method will return a custom Euler struct from a
 * set of Quaternions. Euler angles follow the post multiply
 * sequence zyx. Rotate "psi" around Z (yaw), "theta" around y (pitch)
 * and "phi" around x (roll).
 * @param w Quaternion parameter w.
 * @param x Quaternion parameter x.
 * @param y Quaternion parameter y.
 * @param z Quaternion parameter z.
 * @return EulerRotations::Euler Euler angles given in radians.
 */
EulerRotations::Euler EulerRotations::quaternions_to_euler(double w, double x,
    double y, double z)
{
    EulerRotations::Euler euler_angles;

    // Roll (x-axis rotation)
    double sinr_cosp = 2.0 * (w * x + y * z);
    double cosr_cosp = 1.0 - 2.0 * (x * x + y * y);
    euler_angles.phi = std::atan2(sinr_cosp, cosr_cosp);

    // Pitch (y-axis rotation)
    double sinp = 2.0 * (w * y - z * x);
    if (std::abs(sinp) >= 1.0)
    {
        euler_angles.theta = std::copysign(M_PI / 2.0, sinp); // use 90 degrees if out of range
    }
    else
    {
        euler_angles.theta = std::asin(sinp);
    }

    // Yaw (z-axis rotation)
    double siny_cosp = 2.0 * (w * z + x * y);
    double cosy_cosp = 1.0 - 2.0 * (y * y + z * z);
    euler_angles.psi = std::atan2(siny_cosp, cosy_cosp);

    // Return Euler angles
    return euler_angles;
}