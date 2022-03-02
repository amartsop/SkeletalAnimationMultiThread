#pragma once

#include <iostream>
#include <vector>
#include <eigen3/Eigen/Dense>

/// Class EulerRotations
/**
 * This class contains definitions of rotation matrices and useful functionality 
 * for rotation releated operations.
 */
class EulerRotations
{
public:
    // -------------------- Methods -------------------- //

    /// Custom quaternions struct: \f$w + x \vec{i} + y \vec{j} + z \vec{k}\f$.
    struct Quaternions { double w, x, y, z; };

    /// Custom Euler angles struct
    struct Euler{ double phi, theta, psi; };

    /// Convert Euler angles to quaternions
    static Quaternions euler_to_quaternions(double phi, double
        theta, double psi);

    /// Convert quaternions to Euler angles
    static Euler quaternions_to_euler(double w, double x, double y, 
        double z);

public:
    /// Basic rotation matrix wrt x axis.
    static Eigen::Matrix3d basic_rotation_x(double x);

    /// Basic rotation matrix wrt y axis.
    static Eigen::Matrix3d basic_rotation_y(double x);

    /// Basic rotation matrix wrt z axis.
    static Eigen::Matrix3d basic_rotation_z(double x);

    /// Euler rotation matrix z-y'-x''.
    static Eigen::Matrix3d rotation(double phi, double theta, double psi);
    static Eigen::Matrix3d rotation(Eigen::Vector3d euler_angles);
    static Eigen::Matrix3d rotation(std::vector<double> euler_angles);
    static Eigen::Matrix3d rotation(Euler euler_angles);
};