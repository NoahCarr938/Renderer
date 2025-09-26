#include "Context.h"
#include "Renderer.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <GLFW/glfw3.h>

using namespace aie;

int main()
{
    Context Window;
    Window.Init(640, 480, "Hello Window");

    Vertex triVerts[] =
    {
        {{ -.5f, -.5f, 0, 1 }, {0.0f, 0.0f } }, // bottom left
        {{ .5f, -.5f, 0, 1 }, { 1.0f, 0.0f } }, // bottom right
        {{ 0, .5f, 0, 1 }, { .5f, 1.0f} }  // top
    };
    unsigned int triIndices[] = { 0, 1, 2 };

    Geometry basicTriangleGeo = MakeGeometry(triVerts, 3, triIndices, 3);

    Shader basicShaderFromFile = LoadShader("Res/Shaders/Basic.vert", "Res/Shaders/Basic.frag");
    Shader cameraShaderFromFile = LoadShader("Res/Shaders/BasicCamera.vert", "Res/Shaders/Basic.frag");
    Shader TexShadFromFile = LoadShader("Res/Shaders/BasicCamera.vert", "Res/Shaders/Tex.frag");
    

    // Model matrix - this will transform the object into world space
    glm::mat4 Triangle_Model = glm::identity<glm::mat4>();

    // View Matrix - this makes things relative to the camera (i.e, puts the camera at the center of the world)
    glm::mat4 Camera_View = glm::lookAt(
        glm::vec3(0, 1, 20), // eye
        glm::vec3(0, 5, 0), // look at position
        glm::vec3(0, 1, 0) // up direction
    ); 

    // Projection Matrix - transforms view space into clip space (-1 to +1 zone)
    glm::mat4 Camera_Proj = glm::perspective(
        glm::radians(45.f), // FOV
        640.0f / 480.0f, // Aspect Ratio
        0.1f, // Near-Plane
        1000.0f // Far-Plane
    );



    while (!Window.ShouldClose())
    {
        // update logic here
        Window.Tick();

        // draw logic goes here
        Window.Clear();

        // Setup my uniforms
        SetUniform(TexShadFromFile, 0, Camera_Proj);
        SetUniform(TexShadFromFile, 1, Camera_View);
        SetUniform(TexShadFromFile, 2, Triangle_Model);

        //SetUniform(TexShaderFromFile, 3, (float)glfwGetTime());
        //SetUniform(TexShaderFromFile, 4, 3.0f);

        // draw the red triangle
        //Draw(basicShad, basicTriangleGeo);
        Draw(TexShadFromFile, basicTriangleGeo);
    }

    FreeGeometry(basicTriangleGeo);

    FreeShader(basicShaderFromFile);

    Window.Term();

    return 0;
}