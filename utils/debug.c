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
