#include "../include/hand.h"

/**
 * @brief Construct a new Hand:: Hand object.
 * 
 * @param exo_handler Pointer to the Exoskeleton object.
 * @param anim_hand Pointer to the AnimatedHand object.
 * @param mesh_name The name of the mesh file.
 * @param graph_name The name of the graph file (contains the conections 
 * between the bones aka the kinematic model).
 * @param texture_name The name of the texture file.
 * @param type Bool the define whether the hand is left(0) or right(1).
 * @param origin The origin of the hand.
 */
Hand::Hand(Exoskeleton* exo_handler, AnimatedHand* anim_hand,
    const std::string& mesh_name, const std::string& graph_name,
    const std::string& texture_name, bool type, const Eigen::Vector3d& origin)
{
    // Read volume mesh
    igl::readMESH(mesh_name, m_vol_mesh_vert, m_tetr_indices, m_surf_indices);

    // Set initial mesh vertices
    m_vol_mesh_vert_init = m_vol_mesh_vert;
    
    // Read graph file
    igl::readTGF(graph_name, m_vertex_pos, m_bone_edges_indices);

    // Retrieve parents for forward kinematics
    igl::directed_edge_parents(m_bone_edges_indices, m_point_handles);

    // Set boundary conditions
    igl::boundary_conditions(m_vol_mesh_vert, m_tetr_indices, m_vertex_pos, 
        Eigen::VectorXi(), m_bone_edges_indices , Eigen::MatrixXi(),
        m_boundary_indices, m_boundary_conditions);

    // Only a few iterations
    m_bbw_data.active_set_params.max_iter = 8;
    m_bbw_data.verbosity = 2;

    if(!igl::bbw(m_vol_mesh_vert, m_tetr_indices, m_boundary_indices, 
        m_boundary_conditions, m_bbw_data, m_weights))
    {
        std::cout << "Error: BBW Error" << std::endl;
    }

    // Normalize weights to sum to one
    igl::normalize_row_sums(m_weights, m_weights);

    // Precompute linear blend skinning matrix
    igl::lbs_matrix(m_vol_mesh_vert, m_weights, m_g_weights);

    // Get exoskeleton handler pointer
    m_exo_handler = exo_handler;

    // Get animation hand pointer
    m_anim_hand = anim_hand;

    // Get hand index iterator
    m_hand_idx_iter = m_anim_hand->get_hand_idx_iterator();

    // Initialize animation pose
    m_anim_pose.resize(m_pose_size);
    
    for (size_t i = 0; i < m_pose_size; i++)
    {
        m_anim_pose.at(i) = Eigen::Quaterniond::Identity();
    }

    // Read texture
    igl::png::readPNG(texture_name, m_texture.red, m_texture.green,
        m_texture.blue, m_texture.a);

    // Get hand type
    m_hand_type = type;

    // Get hand origin
    m_origin = origin;
}
    
/**
 * @brief It updates the hand vertices based on the euler angles for its 
 * skeleton joints. These are fed throught the AnimatedHand::EulerID 
 * struct. Based on the defined mapping it performs the forward kinematics 
 * and calcualtes all the hand vertices.
 * @param euler_id The custom EulerID structure as described in AnimatedHand::EulerID.
 */
void Hand::update(const std::vector<AnimatedHand::EulerID>& euler_id)
{
    for (size_t i = 0; i < euler_id.size(); i++)
    {
        // Get euler id i
        std::vector<double> euler_i = euler_id.at(i).euler_angles;

        // // Get frame id i
        int frame_id_i = euler_id.at(i).frame_id;

        // Transform angles to quaternions
        EulerRotations::Quaternions quatern = 
            EulerRotations::euler_to_quaternions(euler_i.at(0),
            euler_i.at(1), euler_i.at(2));

        // Set animation pose
        m_anim_pose.at(frame_id_i) = Eigen::Quaterniond(quatern.w, quatern.x, 
            quatern.y, quatern.z);
    }
        
    // Propagate relative rotations via FK to retrieve absolute transformations
    RotationList v_rot;
    std::vector<Eigen::Vector3d> v_tran;

    // Forward kinematics 
    igl::forward_kinematics(m_vertex_pos, m_bone_edges_indices,
        m_point_handles, m_anim_pose, v_rot, v_tran);

    // Get verted dimension 
    int vert_dim = m_vertex_pos.cols();

    // Total transformation matrix
    Eigen::MatrixXd t_mat(m_bone_edges_indices.rows()*(vert_dim+1), vert_dim);

    // Perform trasnformation
    for (int i = 0 ; i < m_bone_edges_indices.rows(); i++)
    {
        Eigen::Affine3d a = Eigen::Affine3d::Identity();

        // Translate (has a trick to account for mirroring in case of right hand)
        a.translate(v_tran.at(i) + (-2.0 * (double)m_hand_type + 1) *  m_origin);

        // Rotate
        a.rotate(v_rot.at(i));

        t_mat.block(i*(vert_dim+1), 0, vert_dim+1, vert_dim) =
            a.matrix().transpose().block(0, 0, vert_dim+1, vert_dim);
    }
    
    // Compute deformation via LBS as matrix multiplication
    m_vol_mesh_vert = m_g_weights * t_mat;
        
    if (m_hand_type) // check if it's the right hand and mirror it
    {
        Eigen::Matrix3d mirror = Eigen::Matrix3d::Identity();
        mirror(0, 0) = -1.0;
        m_vol_mesh_vert = (mirror * m_vol_mesh_vert.transpose()).transpose();
    }
}