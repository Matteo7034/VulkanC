#ifndef APP_H
#define APP_H

#include <vulkan/vulkan.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct App{
    VkInstance instance;
}App;


bool app_run(App *app);
void app_cleanup(App *app);
#endif
