#include "engine.h"
#include "init.h"
#include "device.h"
#include <stdio.h>
#include <string.h>
#include "../include/debug.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

const char* validationLayers[]={
    "VK_LAYER_KHRONOS_validation"
};
const uint32_t validationLayerCount = sizeof(validationLayers) / sizeof(validationLayers[0]);
void init_window(App *app){
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    app->window = glfwCreateWindow(WIDTH,HEIGHT,"Vulkan",NULL,NULL);
}

bool init_Vulkan(App* app){
    if(!create_Instance(app)) return false;
    if(!setupDebugMessenger(app)) return false;
    app->physicalDevice = pickPhysicalDevice(app);
    if(app->physicalDevice == VK_NULL_HANDLE) return false;
    app->device = createLogicalDevice(
            app->physicalDevice,
            app->deviceFeatures,
            &app->graphicsQueue);
    if(app->device == VK_NULL_HANDLE) return false;
    return true;
}


static void main_loop(App *app){
    while(!glfwWindowShouldClose(app->window)){
        glfwPollEvents();
    }
}

bool app_run(App*app){
    init_window(app);
    if(!init_Vulkan(app)){
        return false;
    }
    
    main_loop(app);
    return true;
}

void app_cleanup(App *app){
    printf("[INFO] Cleaning up...\n");
    if(app->device != VK_NULL_HANDLE){
        vkDestroyDevice(app->device,NULL);
        app->device = VK_NULL_HANDLE;
    }
    if (enableValidationLayers && app->debugMessenger != VK_NULL_HANDLE) {
        destroyDebugUtilsMessengerEXT(app->instance, app->debugMessenger, NULL);
        app->debugMessenger = VK_NULL_HANDLE;
    }
    if (app->instance != VK_NULL_HANDLE) {
        vkDestroyInstance(app->instance, NULL);
        app->instance = VK_NULL_HANDLE;
    }

    if(app->window != NULL){
        printf("[INFO] Cleaning GLFW...\n");
        glfwDestroyWindow(app->window);
        app->window = NULL;
        glfwTerminate();
    }
}

bool checkValidationLayerSupport(){
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount,NULL);
    
    VkLayerProperties* availableLayers = malloc(layerCount*sizeof(VkLayerProperties));
    if(availableLayers == NULL){
        fprintf(stderr,"[ERROR] unable to allocate memory to avaiableLayers...\n");
        return false;
    }
    vkEnumerateInstanceLayerProperties(&layerCount,availableLayers);

    for(uint32_t i=0;i<validationLayerCount;i++){
        bool layerFound =false;
        for(uint32_t j=0;j<layerCount;j++){
            if(strcmp(validationLayers[i],availableLayers[j].layerName)==0){
                layerFound=true;
                break;
            }
        }
        
        if(!layerFound){
            free(availableLayers);
            return false;
        }
    }
    free(availableLayers);

    return true;
}

