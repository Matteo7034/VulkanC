#ifndef DEVICE_H
#define DEVICE_H
#include<vulkan/vulkan.h>
#include "engine.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct OptionalUint32 {
    uint32_t value;
    bool has_value;
} OptionalUint32;

typedef struct QueueFamilyIndices {
    OptionalUint32 graphicsFamily;
} QueueFamilyIndices;

bool isQueueFamilyIndicesComplete(QueueFamilyIndices indices);

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

VkPhysicalDevice pickPhysicalDevice(App* app);
bool isDeviceSuitable(VkPhysicalDevice device);
int rateDeviceSuitability(VkPhysicalDevice device);

#endif
