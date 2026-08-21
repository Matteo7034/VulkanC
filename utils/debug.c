#include "../include/debug.h"
#include "../include/engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

const char** getRequiredExtensions(uint32_t *outCount){
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    if(glfwExtensions == NULL ) {
        fprintf(stderr,"[ERROR] impossible to recover GLFW extensions\n");
        *outCount = 0;
        return NULL;
    }
    uint32_t count = glfwExtensionCount + 1;
    if(enableValidationLayers)
        count++;
    const char** extensions = malloc(count * sizeof(const char*));
    if(extensions == NULL){
        fprintf(stderr,"[ERROR] not enough memory to extensions!\n");
        *outCount = 0;
        return NULL;
    }
    for(uint32_t i = 0; i<glfwExtensionCount;i++){
        extensions[i] = glfwExtensions[i];
    }
    extensions[glfwExtensionCount] = VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME;
    if(enableValidationLayers){
        extensions[count -1] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
    }
    *outCount = count;

    return extensions;
}


 VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {
    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
    // Message is important enough to show
    }
    (void)pUserData;
    //(VkDebugUtilsMessageTypeFlagsEXT)messageType;
    (void)messageType;

    fprintf(stderr,"[VALIDATION LAYER] %s\n",pCallbackData->pMessage);
    return VK_FALSE;
}


// Funzione helper per caricare ed eseguire vkCreateDebugUtilsMessengerEXT
static VkResult createDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger) {

    PFN_vkCreateDebugUtilsMessengerEXT func =
        (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    if (func != NULL) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

// Funzione helper per la distruzione del Messenger
void destroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks* pAllocator) {

    PFN_vkDestroyDebugUtilsMessengerEXT func =
        (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

    if (func != NULL) {
        func(instance, debugMessenger, pAllocator);
    }
}

void populateDebugMessengerCreateInfo(
        VkDebugUtilsMessengerCreateInfoEXT* createInfo){
    *createInfo = (VkDebugUtilsMessengerCreateInfoEXT){0};
    createInfo->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo->pfnUserCallback = debugCallback;
}
bool setupDebugMessenger(App* app) {
    if (!enableValidationLayers) return true;

    VkDebugUtilsMessengerCreateInfoEXT createInfo = {0};
    populateDebugMessengerCreateInfo(&createInfo);

    VkResult result = createDebugUtilsMessengerEXT(app->instance, &createInfo, NULL, &app->debugMessenger);
    if (result != VK_SUCCESS) {
        fprintf(stderr, "[ERROR] Impossibile configurare il Debug Messenger! (codice %d)\n", result);
        return false;
    }

    return true;
}


