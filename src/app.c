#include "app.h"
#include "init.h"
#include <stdio.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

void init_window(App *app){
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    app->window = glfwCreateWindow(WIDTH,HEIGHT,"Vulkan",NULL,NULL);
}

bool init_Vulkan(App* app){
    return create_Instance(app);
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


