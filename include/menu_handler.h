#pragma once

#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <Python.h>
#include <stdio.h>

#include <igl/opengl/glfw/imgui/ImGuiPlugin.h>
#include <igl/opengl/glfw/imgui/ImGuiMenu.h>
#include <igl/opengl/glfw/imgui/ImGuiHelpers.h>

/// Class MenuHandler
/**
 * This class handles the animation menu.
*/

class MenuHandler
{
public:
    /// Constructor
    MenuHandler(igl::opengl::glfw::imgui::ImGuiMenu* menu);

    /// Menu callback function.
    void callback(void);

    /// Check whether the USB ports have been sent by the user (Is OK pressed?).
    bool are_ports_set(void) { return m_ports_set; }

    /// Get the USB port for the left exoskeleton.
    std::string get_left_exoskeleton_port(void) { return m_left_exoskeleton_port; }

    /// Get the USB port for the right exoskeleton.
    std::string get_right_exoskeleton_port(void) { return m_right_exoskeleton_port; }

private:
    /// ImGui menu handle pointer.
    igl::opengl::glfw::imgui::ImGuiMenu *m_menu;

    /// Get available USB ports.
    std::vector<std::string> get_available_usb_ports(void);

    /// Generate absolute file name.
    std::string generate_absolute_filename(const std::filesystem::path&
        absolute_path, const std::filesystem::path& folder,
        const std::string& relative_filename);

private:
    /// Name of the python script that reads the available USB ports.
    std::string m_script_name_rel = "ports_read.py";
    
    /// Name of the file in which the available USB ports are stored.
    std::string m_ports_file_rel = "available_ports.txt";

    /// Name of the left exoskeleton USB port.
    std::string m_left_exoskeleton_port;

    /// Name of the right exoskeleton USB port.
    std::string m_right_exoskeleton_port;

    /// Flag that stores the state of the USB port (whether are set or not).
    bool m_ports_set = 0;
};