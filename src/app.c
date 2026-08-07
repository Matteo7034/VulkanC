#include "app.h"
#include "init.h"
#include <stdio.h>

static void main_loop(App *app){
    (void)app;
}

bool app_run(App*app){
    if(!init_vulkan(app)){
        return false;
    }
    main_loop(app);
    return true;
}

void app_cleanup(App *app){
    if (app->instance != VK_NULL_HANDLE) {
        vkDestroyInstance(app->instance, NULL);
        app->instance = VK_NULL_HANDLE;
    }
}


