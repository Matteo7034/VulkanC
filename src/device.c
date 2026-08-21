#include "device.h"
#include <stdio.h>
#include <vulkan/vulkan.h>
#include <stdlib.h>
VkPhysicalDevice pickPhysicalDevice(App* app){
    uint32_t deviceCount = 0;

    vkEnumeratePhysicalDevices(app->instance,&deviceCount,NULL);
    if(deviceCount == 0){
        fprintf(stderr,"[ERROR] failed to find GPUs with Vulkan support!\n");
    }
    VkPhysicalDevice* devices = malloc(deviceCount * sizeof(VkPhysicalDevice));

    if(devices == NULL){
        fprintf(stderr,"[ERROR] Failed to allocate memory for Vulkan devices!\n");
        return VK_NULL_HANDLE;
    }
    vkEnumeratePhysicalDevices(app->instance,&deviceCount,devices);
    
    VkPhysicalDevice bestDevice = VK_NULL_HANDLE;
    int highestScore = 0;

    for (uint32_t i = 0; i < deviceCount; i++) {
        int score = rateDeviceSuitability(devices[i],app->surface);

        if (score > highestScore) {
            bestDevice = devices[i];
            highestScore = score;
        }
    }

    free(devices);
    
    if(bestDevice == VK_NULL_HANDLE){
        fprintf(stderr,"[ERROR] Failed to find a suitable GPU!\n");
    }

    return bestDevice;
}

bool isDeviceSuitable(VkPhysicalDevice device){
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    bool isGpuTypeOk = (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) ||
                       (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU);

    return isGpuTypeOk && deviceFeatures.geometryShader;
}

int rateDeviceSuitability(VkPhysicalDevice device,VkSurfaceKHR surface){
    
    QueueFamilyIndices indices = findQueueFamilies(device,surface);
    if (!isQueueFamilyIndicesComplete(indices)) return 0;
    
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    
    int score =0;
    if(!deviceFeatures.geometryShader) return 0;
    
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
        score +=1000;
    }
    
    score += deviceProperties.limits.maxImageDimension2D;

    return score;
}

bool isQueueFamilyIndicesComplete(QueueFamilyIndices indices) {
    return indices.graphicsFamily.has_value && indices.presentFamily.has_value;
}



QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device,VkSurfaceKHR surface){
    QueueFamilyIndices indices = {0};

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device,&queueFamilyCount,NULL);

    if(queueFamilyCount == 0) return indices;

    VkQueueFamilyProperties* queueFamilies = malloc(queueFamilyCount* sizeof(VkQueueFamilyProperties));
    if(queueFamilies == NULL){
        fprintf(stderr,"[ERROR] not enough memory to allocate QueueFamilies!\n");
        return indices;
    }
    vkGetPhysicalDeviceQueueFamilyProperties(device,&queueFamilyCount,queueFamilies);

    for(uint32_t i = 0; i<queueFamilyCount;i++){
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
    
        if(queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){
            indices.graphicsFamily.value = i;
            indices.graphicsFamily.has_value = true;
        }
        if (presentSupport) {
            indices.presentFamily.value = i;
            indices.presentFamily.has_value = true;
        }
        if(isQueueFamilyIndicesComplete(indices)) break;
    }
    free(queueFamilies);
    return indices;
}

VkDevice createLogicalDevice(
        VkPhysicalDevice physicalDevice,
        VkPhysicalDeviceFeatures deviceFeatures,
        VkQueue* outGraphicsQueue,
        VkQueue* outPresentQueue,
        VkSurfaceKHR surface){
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice,surface);

    uint32_t uniqueQueueFamilies[2];
    uint32_t uniqueQueueCount = 0;

    uniqueQueueFamilies[uniqueQueueCount++] = indices.graphicsFamily.value;
    if(indices.graphicsFamily.value != indices.presentFamily.value) {
        uniqueQueueFamilies[uniqueQueueCount++] = indices.presentFamily.value;
    }
    VkDeviceQueueCreateInfo queueCreateInfos[2] = {0};
    float queuePriority = 1.0f;
    for(uint32_t i =0; i<uniqueQueueCount;i++){
        queueCreateInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfos[i].queueFamilyIndex = uniqueQueueFamilies[i];
        queueCreateInfos[i].queueCount = 1;
        queueCreateInfos[i].pQueuePriorities = &queuePriority;
    }

    VkDeviceCreateInfo createInfo={0};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos;
    createInfo.queueCreateInfoCount = uniqueQueueCount;
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 0;

    if(enableValidationLayers){
        createInfo.enabledLayerCount =validationLayerCount;
        createInfo.ppEnabledLayerNames = validationLayers;
    }else{
        createInfo.enabledLayerCount = 0;
    }

    VkDevice device = VK_NULL_HANDLE;
    if(vkCreateDevice(physicalDevice,&createInfo,NULL,&device)!=VK_SUCCESS){
        fprintf(stderr,"[ERROR] failed to create logical device!\n");
    }
    vkGetDeviceQueue(device,indices.graphicsFamily.value,0,outGraphicsQueue);
    vkGetDeviceQueue(device,indices.presentFamily.value,0,outPresentQueue);
    return device;
}
