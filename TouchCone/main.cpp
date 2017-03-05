//
//  main.cpp
//  HelloArrow
//
//  Created by zhuang yusong on 2017/3/4.
//  Copyright © 2017年 zhuang yusong. All rights reserved.
//

#include <glfw3.h>
#include <iostream>
#include "TouchCone.h"
#include "Path.h"

GLApp* g_app;
bool g_captured= false;

void  GLAppMouseMoveCallBack(GLFWwindow*, double x,double y){
    if(g_captured)
        g_app->onTouchMove(x, y);
}

static void GLAppMouseCallBack(GLFWwindow* window, int button, int action, int modify)
{
    double mouseX;
    double mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    if(GLFW_MOUSE_BUTTON_LEFT == button){
        if(GLFW_PRESS == action){
            g_captured = true;
            g_app->onTouchBegan(mouseX, mouseY);

        }
        else if(GLFW_RELEASE == action){
            if (g_captured){
                g_captured = false;
                g_app->onTouchEnd(mouseX, mouseY);
            }
        }
    }
}

int main(void){
    GLFWwindow* window;
    
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(320, 480, "Touch Cone", NULL, NULL);
    
    if (!window){
        glfwTerminate();
        return -1;
    }
    //8, 8, 8, 8, 24, 8
    glfwWindowHint(GLFW_RED_BITS,8);
    glfwWindowHint(GLFW_GREEN_BITS,8);
    glfwWindowHint(GLFW_BLUE_BITS,8);
    glfwWindowHint(GLFW_ALPHA_BITS,8);
    glfwWindowHint(GLFW_DEPTH_BITS,24);
    glfwWindowHint(GLFW_STENCIL_BITS,8);
    
    glfwSetCursorPosCallback(window, GLAppMouseMoveCallBack);
    glfwSetMouseButtonCallback(window, GLAppMouseCallBack);
    
    glfwMakeContextCurrent(window);
    
    TouchCone app;
    g_app = &app;
    app.initProgram();
    app.init();
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)){
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        app.render();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}
