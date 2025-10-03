#include "Context.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
bool aie::Context::Init(int Width, int Height, const char* Title)
{
    glfwInit();
    Window = glfwCreateWindow(640, 480, "Hello Window", nullptr, nullptr);
    glfwMakeContextCurrent(Window);

    glewInit();

    std::cout << "OpenGL Version: " << (const char*)glGetString(GL_VERSION) << std::endl;
    std::cout << "Renderer: " << (const char*)glGetString(GL_RENDERER) << std::endl;
    std::cout << "Vendor: " << (const char*)glGetString(GL_VENDOR) << std::endl;
    std::cout << "GLSL: " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LEQUAL);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    //glClearColor(0.0f, 0.75f, 0.75f, 1);



    return false;
}

void aie::Context::Tick()
{
    glfwPollEvents();

    glfwSwapBuffers(Window);
}

void aie::Context::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void aie::Context::Term()
{
    glfwDestroyWindow(Window);
    glfwTerminate();
}

bool aie::Context::ShouldClose()
{
    return glfwWindowShouldClose(Window);
}