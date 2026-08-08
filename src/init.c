#include "init.h"
#include "app.h"
#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan.h>
bool create_Instance(App *app){
    if(enableValidationLayers && !checkValidationLayerSupport()){
        fprintf(stderr,"[ERROR] validation layers requested, but not available!\n");
        return false;
    }
    VkApplicationInfo appInfo = {0};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle C";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;



    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    
    if (glfwExtensions == NULL) {
        fprintf(stderr, "[ERROR] impossible to get GLFW extensions!\n");
        return false;
    }
    uint32_t extensionCount = glfwExtensionCount + 1;
    const char** requiredExtensions = malloc(extensionCount * sizeof(const char*));
    if (requiredExtensions == NULL) {
        fprintf(stderr, "[ERROR] not enough to allocate for extensions!\n");
        return false;
    }
    for (uint32_t i =0;i<glfwExtensionCount;i++){
        requiredExtensions[i] = glfwExtensions[i];
    }
    
    requiredExtensions[glfwExtensionCount] = VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME;
    
    VkInstanceCreateInfo createInfo = {0};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = extensionCount;
    createInfo.enabledExtensionCount = extensionCount;
    createInfo.ppEnabledExtensionNames = requiredExtensions;
    if(enableValidationLayers){
        createInfo.enabledLayerCount = validationLayerCount;
        createInfo.ppEnabledLayerNames = validationLayers;
    }
    else{
        createInfo.enabledLayerCount = 0;
        createInfo.ppEnabledLayerNames = NULL;
    }
    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
   
    check_available_extensions();

    VkResult result = vkCreateInstance(&createInfo, NULL, &app->instance);
    free(requiredExtensions);
    if (result != VK_SUCCESS) {
        fprintf(stderr, "Error: Failed to create Vulkan instance! (codice %d)\n", result);
        return false;
    }
    

    return true;
}

void check_available_extensions(void) {

    printf("[INFO] Available extensions:\n");
    
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);
    VkExtensionProperties* extensions = malloc(extensionCount * sizeof(VkExtensionProperties));
    if(extensions == NULL){
        fprintf(stderr,"[ERROR] not enough memory for aviabile extension...\n");
        return;
    }
    vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extensions);
    printf("[INFO] Aviabile Extensions: (%u):\n",extensionCount);
    for(uint32_t i=0;i<extensionCount;i++){
        printf("\t%s (v%u)\n",extensions[i].extensionName,extensions[i].specVersion);
    }
    free(extensions);
}
