#include "../include/lib/skeletal_animation.h"

/**
 * @brief Initializes skeletal animation by copying the input arguments
 * to the member variables.
 * @param viewer Pointer to the igl Viewer object.
 * @param left_exo Pointer to the left exoskeleton object.
 * @param anim_hand Pointer to the animated hand object.
 * @param menu_handler Pointer to menu handler object.
 */
void SkeletalAnimation::initialize(igl::opengl::glfw::Viewer* viewer, 
    Exoskeleton* left_exo, AnimatedHand* anim_hand, MenuHandler* menu_handler)
{
    // Get exoskeleton handler pointer
    m_left_exo = left_exo;

    // Get animation hand pointer
    m_anim_hand = anim_hand;

    // Get menu handler pointer
    m_menu_handler = menu_handler;
}

/**
 * @brief 
 *  This is the main animation callback function. This is where the rendering is 
 * happening. The function is passed as a lambda function to
 * the Viewer handler (see main.cpp).
 * @param viewer Reference to the viewer handle.
 * @return true Animation should stop.
 * @return false Animations keeps playing.
 */
bool SkeletalAnimation::animation_loop(igl::opengl::glfw::Viewer& viewer)
{
    if (viewer.core().is_animating)
    {
        // Initialize if ports are set
        if (m_menu_handler->are_ports_set() && m_initialize_animation)
        {
            // Setup exoskeleton
            setup_exoskeletons(viewer);

            // Dont initialize animations again
            m_initialize_animation = 0;
        }

        if(m_menu_handler->are_ports_set())
        {
            // Get euler angles
            auto euler_id =
                m_anim_hand->get_hand_angles(m_left_exo->get_joint_angles());
        
            // Update left hand
            m_left_hand->update(euler_id);
            viewer.data_list.at(0).set_vertices(m_left_hand->get_vertices());

            // Update right hand
            m_right_hand->update(euler_id);
            viewer.data_list.at(1).set_vertices(m_right_hand->get_vertices());
        }
    } 
    
    return false;
}

/**
 * @brief This function setups the exoskeletons. First it initializes the 
 * serial communications and defines the hand shared pointers. It also generates 
 * the hand meshes and passes them to the viewer mesh list (or data_list). See 
 * how libigl handles multiple meshes at
 * https://github.com/libigl/libigl/blob/main/tutorial/107_MultipleMeshes/main.cpp.
 * @param viewer A reference to the viewer handle.
 */

void SkeletalAnimation::setup_exoskeletons(igl::opengl::glfw::Viewer& viewer)
{
    // Resize viewer data list
    viewer.data_list.resize(2);

   // Define serial COM for left exoskeleton
    std::string serial_com_left = m_menu_handler->get_left_exoskeleton_port();

   // Define serial COM for right exoskeleton
    std::string serial_com_right = m_menu_handler->get_right_exoskeleton_port();

    // Define baudrate
    unsigned int baud_rate = 115200;
    
    // Initialize left exoskeleton
    m_left_exo->initialize(serial_com_left, baud_rate);

    // Initialize right exoskeleton (to be done)

    // Initialize left hand
    m_left_hand = std::make_shared<Hand>(m_left_exo, m_anim_hand,
        m_hand_mesh_name, m_hand_graph_name, m_hand_texture_name, 0, 
        m_left_origin);
    
    // Set mesh for left hand
    viewer.data_list.at(0).set_mesh(m_left_hand->get_vertices(), 
        m_left_hand->get_surface_indices());

    // Initialze right hand
    m_right_hand = std::make_shared<Hand>(m_left_exo, m_anim_hand,
        m_hand_mesh_name, m_hand_graph_name, m_hand_texture_name, 1,
        m_right_origin);

    // Set mesh for right hand
    viewer.data_list.at(1).set_mesh(m_right_hand->get_vertices(), 
        m_right_hand->get_surface_indices());
}