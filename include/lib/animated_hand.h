#pragma once 

#include <iostream>
#include <vector>
#include <map>
#include "euler_rotations.h"


class AnimatedHand
{

public:
    AnimatedHand() {};

    struct EulerID
    {
        // Euler angles
        std::vector<double> euler_angles{0.0, 0.0, 0.0};

        // Frame ID
        int frame_id;
    };

    struct HandMap
    {
        // Frame's unique index
        int frame_idx;

        // Frames name/id
        int frame_id;

        //Rotation type sequence of rotations (roll=0, pitch=1, yaw=2)
        int rot_type;

        // Rotation direction (1: positive, -1:negative)
        double rot_dir;
    };

public:
    
    // Generate hand angles
    std::vector<EulerID>
        get_hand_angles(const std::vector<double>& joint_angles);

    // Get hand index iterator
    std::vector<int> get_hand_idx_iterator(void) { return m_hand_idx_iter; }

private:

    std::vector<HandMap> m_hand_map{
        HandMap{0, 0, 2, -1}, 
        HandMap{0, 0, 0, 1},
        HandMap{1, 19, 0, 1},  
        HandMap{2, 11, 0, 1},  
        HandMap{3, 7, 2, -1},  
        HandMap{3, 7, 0, 1},   
        HandMap{4, 16, 0, 1},  
        HandMap{5, 18, 0, 1},  
        HandMap{6, 5, 1, 1},   
        HandMap{6, 5, 0, 1},   
        HandMap{6, 5, 2, -1},  
        HandMap{7, 17, 2, -1}, 
        HandMap{8, 8, 2, -1}};

    // Number of hand frames
    int m_hand_frames_num = 9;

    // Hand index iterator
    std::vector<int> m_hand_idx_iter = {0, 19, 11, 7, 16, 18, 5, 17, 8};

};
