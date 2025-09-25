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
        {{ -.5f, -.5f, 0, 1 }, {1, 0, 1, 1} },
        {{ .5f, -.5f, 0, 1 }, { 1, 0, 0, 1 } },
        {{ 0, .5f, 0, 1 }, { 1, 0, 0, 1} }
    };
    unsigned int triIndices[] = { 0, 1, 2 };

    Geometry basicTriangleGeo = MakeGeometry(triVerts, 3, triIndices, 3);

    Shader basicShaderFromFile = LoadShader("Res/Shaders/Basic.vert", "Res/Shaders/Basic.frag");
    Shader cameraShaderFromFile = LoadShader("Res/Shaders/BasicCamera.vert", "Res/Shaders/Basic.frag");
    

    //const char * basicVert =
    //    "#version 430 core\n"
    //    "layout (location = 0) in vec4 position;\n"
    //    "layout (location = 1) in vec4 colors;\n"
    //    "out vec4 outcolors;\n"
    //    "void main() { outcolors = colors; gl_Position = position; }";

    //const char * basicFrag =
    //    "#version 430 core\n"
    //    "out vec4 vertColor;\n"
    //    "in vec4 outcolors;\n"
    //    "void main() { vertColor = outcolors; }";

    //Shader basicShad = MakeShader(basicVert, basicFrag);

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
        SetUniform(cameraShaderFromFile, 0, Camera_Proj);
        SetUniform(cameraShaderFromFile, 1, Camera_View);
        SetUniform(cameraShaderFromFile, 2, Triangle_Model);

        SetUniform(cameraShaderFromFile, 3, (float)glfwGetTime());
        SetUniform(cameraShaderFromFile, 4, 3.0f);

        // draw the red triangle
        //Draw(basicShad, basicTriangleGeo);
        Draw(cameraShaderFromFile, basicTriangleGeo);
    }

    FreeGeometry(basicTriangleGeo);

    FreeShader(cameraShaderFromFile);

    Window.Term();

    return 0;
}