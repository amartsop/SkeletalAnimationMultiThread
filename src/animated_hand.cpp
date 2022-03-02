#include "../include/animated_hand.h"

// Generate hand angles 
std::vector<AnimatedHand::EulerID> AnimatedHand::get_hand_angles(const
    std::vector<double>& joint_angles)
{
    // Initialize eulerid
    std::vector<AnimatedHand::EulerID> eul_id(m_hand_frames_num);

    for (size_t i = 0; i < joint_angles.size(); i++)
    {
        // Get config i
        auto config_i = m_hand_map.at(i);
        
        // Set frame id
        eul_id.at(config_i.frame_idx).frame_id = config_i.frame_id;
        
        // Set euler angles
        eul_id.at(config_i.frame_idx).euler_angles.at(config_i.rot_type) = 
            config_i.rot_dir * joint_angles.at(i);
    }

    return eul_id;
}