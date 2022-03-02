#pragma once

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <memory>
#include <boost/filesystem.hpp>

#include "exoskeleton.h"
#include "animated_hand.h"
#include "hand.h"
#include "menu_handler.h"


#include <Eigen/Geometry>
#include <Eigen/StdVector>
#include "igl/opengl/glfw/Viewer.h"
#include "igl/readMESH.h"
#include "igl/readTGF.h"
#include "igl/png/readPNG.h"
#include "igl/directed_edge_parents.h"
#include "igl/boundary_conditions.h"
#include "igl/bbw.h"
#include "igl/lbs_matrix.h"
#include "igl/normalize_row_sums.h"
#include "igl/forward_kinematics.h"
#include "igl/jet.h"
#include "igl/deform_skeleton.h"

typedef std::vector<Eigen::Quaterniond, 
    Eigen::aligned_allocator<Eigen::Quaterniond>> RotationList;


/// Class SkeletalAnimation
/**
 * This class implements the skeletal animation for the hand.
*/
class SkeletalAnimation
{

public:
    /// Constructor.
    SkeletalAnimation() {};

    /// Initialize animation.
    void initialize(igl::opengl::glfw::Viewer* viewer, 
        Exoskeleton* left_exo, AnimatedHand* anim_hand, 
        MenuHandler* menu_handler);

    /// Animation loop callback.
    bool animation_loop(igl::opengl::glfw::Viewer & viewer);

private:
    /// Mesh filename.
    std::string m_hand_mesh_name =
        boost::filesystem::system_complete("share/hand.mesh").string();
    
    /// Skeleton graph filename.
    std::string m_hand_graph_name = 
        boost::filesystem::system_complete("share/hand.tgf").string();

    /// Texture name.
    std::string m_hand_texture_name = 
        boost::filesystem::system_complete("share/texture.png").string();

private: 
    /// Exoskeleton handler pointer.
    Exoskeleton* m_left_exo;

    /// Animated hand pointer.
    AnimatedHand* m_anim_hand;

    /// Menu handler pointer.
    MenuHandler* m_menu_handler;

    /// Left and right hand.
    std::shared_ptr<Hand> m_left_hand, m_right_hand;

    /// Left hand origin.
    Eigen::Vector3d m_left_origin = Eigen::Vector3d(1.0, 0.0, 0.0);

    /// Right hand origin.
    Eigen::Vector3d m_right_origin = Eigen::Vector3d(-1.0, 0.0, 0.0);

    /// Start animation flag.
    bool m_initialize_animation = 1;

    /// Setup meshes.
    void setup_exoskeletons(igl::opengl::glfw::Viewer& viewer);
};