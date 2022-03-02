#include <iostream>
#include <future>

#include "./include/exoskeleton.h"
#include "./include/skeletal_animation.h"
#include "./include/animated_hand.h"
#include "./include/menu_handler.h"

#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/imgui/ImGuiPlugin.h>
#include <igl/opengl/glfw/imgui/ImGuiMenu.h>
#include <igl/opengl/glfw/imgui/ImGuiHelpers.h>
#include <igl/slim.h>
#include <igl/Camera.h>

/**
 * @brief This is the main execution function. It first initiates the libigl 
 * viewer and then calls the menu handler from ImGui. It then defines the 
 * exoskeleton handlers and defines the callback function for the rendering loop.
 * For a better understanding of the dependencies see the provided call graph.
 */

int main(void)
{
  // Initialize viewer
  igl::opengl::glfw::Viewer viewer;
  
  // Attach a menu plugin
  igl::opengl::glfw::imgui::ImGuiPlugin plugin;
  viewer.plugins.push_back(&plugin);
  igl::opengl::glfw::imgui::ImGuiMenu menu;
  plugin.widgets.push_back(&menu);

  // Menu handler
  MenuHandler menu_handler(&menu);

  // Generate lamda function pointing to the menu callback
  auto lamda_menu_fun = [&menu_handler]() { return menu_handler.callback(); };
  menu.callback_draw_viewer_menu = lamda_menu_fun;

  // Initialize left exoskeleton handle
  Exoskeleton left_exoskeleton;
  
  // Initialize animated hand handler
  AnimatedHand anim_hand; 

  // Initialize skeletal animation 
  SkeletalAnimation sa;
  sa.initialize(&viewer, &left_exoskeleton, &anim_hand, &menu_handler);

  // Generate lamda function pointing to the animation loop member function
  auto lamda_anim_fun =
      [&sa](igl::opengl::glfw::Viewer& viewer) { return sa.animation_loop(viewer); };
  
  // Set animation
  viewer.data().show_overlay_depth = false;
  viewer.data().line_width = 1;
  viewer.data().show_lines = false;
  viewer.callback_pre_draw = lamda_anim_fun;
  viewer.core().is_animating = true;
  viewer.core().animation_max_fps = 30;
  viewer.launch();
}
