#ifndef APP_H
#define APP_H

#include <vulkan/vulkan.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


#define WIDTH 800
#define HEIGHT 600

typedef struct App{
    VkInstance instance;
    GLFWwindow* window;
}App;


bool app_run(App *app);
void app_cleanup(App *app);
void init_window(App* app);
bool init_Vulkan(App* app);

#endif
