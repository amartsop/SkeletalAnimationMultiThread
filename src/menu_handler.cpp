#include "../include/menu_handler.h"

/**
 * @brief Construct a new Menu Handler:: Menu Handler object
 * 
 * @param menu The menu pointer provided by ImGui.
 */
MenuHandler::MenuHandler(igl::opengl::glfw::imgui::ImGuiMenu* menu)
{
    // Copy menu pointer
    m_menu = menu;
}

/**
 * @brief 
 *  The callback function responds to the actions of the ImGui menu. This 
 * function is passed as a lambda function to the menu handler (see main.cpp).
 */
void MenuHandler::callback(void)
{
    // Draw viewer menu
    m_menu->draw_viewer_menu();

    // Add new group
    if (ImGui::CollapsingHeader("Set USB Ports", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (!m_ports_set)
        {
            // Get available usb poirts
            std::vector<std::string> available_ports = get_available_usb_ports();

            static std::vector<std::string> choices;

            static int left_exo_idx_choise = 0;
            static int right_exo_idx_choise = 0;
        
            // Get left exoskeleton port
            ImGui::Combo("Left exoskeleton", &left_exo_idx_choise,
                available_ports);
        
            // Get right exoskeleton port
            ImGui::Combo("Right exoskeleton", &right_exo_idx_choise,
                available_ports);
        
            if (ImGui::Button("OK"))
            {
                m_left_exoskeleton_port = available_ports.at(left_exo_idx_choise);
                m_right_exoskeleton_port = available_ports.at(right_exo_idx_choise);
                m_ports_set = 1;
            }
        }
    }
}

/**
 * @brief This function simply generates the available USB ports. 
 * @return std::vector<std::string> The available USB ports
 */
std::vector<std::string> MenuHandler::get_available_usb_ports(void)
{
    // Initialize all ports
    std::vector<std::string> ports;

    // Get the listed usb ports
    std::string usb_path("/dev/");
    std::string key("tty");
    for (const auto & entry : std::filesystem::directory_iterator(usb_path))
    {
        //Get path string
        std::string path_string = entry.path().string();
    
        // Check if key is in path string
        if (path_string.find(key) != std::string::npos)
        {
            ports.push_back(path_string);
        }
    }

    // Initialize availale ports
    std::vector<std::string> available_ports;

    // Check for all ports if they are available
    for (size_t i = 0; i < ports.size(); i++)
    {
        int serial_port = open(ports.at(i).c_str(), O_RDWR);

        struct termios tty;
        if(tcgetattr(serial_port, &tty) == 0) {
            available_ports.push_back(ports.at(i));
        }

        close(serial_port);
    }

    return available_ports;
}
