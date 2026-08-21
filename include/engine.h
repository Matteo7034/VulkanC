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

extern const bool enableValidationLayers;

extern const char* validationLayers[];
extern const uint32_t validationLayerCount;


typedef struct App{
    VkInstance instance;
    GLFWwindow* window;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice physicalDevice;
    VkDevice device; 
    VkPhysicalDeviceFeatures deviceFeatures;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkSurfaceKHR surface;
}App;


bool app_run(App *app);
void app_cleanup(App *app);
void init_window(App* app);
bool init_Vulkan(App* app);
bool checkValidationLayerSupport();
VkSurfaceKHR createSurface(App* app);

#endif
