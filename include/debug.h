

#ifndef DEBUG_H
#define DEBUG_H
#include <vulkan/vulkan.h>
#include <stdint.h>
#include <stdbool.h>
#include "engine.h"
const char** getRequiredExtensions(uint32_t* outCount);
VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData);
bool setupDebugMessenger(App* app);
void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
#endif
