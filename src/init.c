#include "init.h"
#include "debug.h"
#include "engine.h"
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

    check_available_extensions();
    
    //get required Extensions
    uint32_t extensionCount = 0;
    const char** requiredExtensions = getRequiredExtensions(&extensionCount);
    if(requiredExtensions == NULL) return false;

    VkInstanceCreateInfo createInfo = {0};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = extensionCount;
    createInfo.ppEnabledExtensionNames = requiredExtensions;
    //Debug
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {0};
    if(enableValidationLayers){
        createInfo.enabledLayerCount = validationLayerCount;
        createInfo.ppEnabledLayerNames = validationLayers;
        populateDebugMessengerCreateInfo(&debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    }
    else{
        createInfo.enabledLayerCount = 0;
        createInfo.ppEnabledLayerNames = NULL;
    }
    
    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
   
    VkResult result = vkCreateInstance(&createInfo, NULL, &app->instance);
    free(requiredExtensions);
    if (result != VK_SUCCESS) {
        fprintf(stderr, 
                "Error: Failed to create Vulkan instance! (codice %d)\n", result);
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
