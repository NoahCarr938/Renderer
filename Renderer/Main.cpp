#include "Context.h"
#include "Renderer.h"
#include "Camera.h"
#include "Object.h"
#include "Transform.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <GLFW/glfw3.h>

using namespace aie;

int main()
{
    Context Window;
    Window.Init(640, 480, "Hello Window");

    //Geometry LoadSpear = LoadGeometry("Res/obj/soulspear.obj");
    Geometry LoadSpear = LoadGeometry("Res/obj/cube.obj");
    //Texture SpearTex = LoadTexture("Res/img/soulspear_diffuse.tga");
    Texture SpearTex = LoadTexture("Res/img/terry.png");
    Shader newShad = LoadShader("Res/Shaders/BasicCamera.vert", "Res/Shaders/Tex.frag");

    // Model matrix - this will transform the object into world space
    glm::mat4 Spear_Model = glm::identity<glm::mat4>();

    // define ambient color
    glm::vec3 ambient(2.0f, 2.0f, 2.0f);

    // NEW: define sun direction
    glm::vec3 sunDirection(1, 0, 1);
    glm::vec3 sunDirection2(2, 0, 2);

    glm::vec3 lightColor(255, 0, 0);
    glm::vec3 lightColor2(0, 255, 0);

    // View Matrix - this makes things relative to the camera (i.e, puts the camera at the center of the world)
    glm::mat4 Camera_View = glm::lookAt(
        glm::vec3(0, 5, 20), // eye
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
        SetUniform(newShad, 0, Camera_Proj);  // projection mat
        SetUniform(newShad, 1, Camera_View);  // view mat
        SetUniform(newShad, 2, Spear_Model);  // model mat

        SetUniform(newShad, 3, SpearTex, 0);   // texture
        SetUniform(newShad, 4, ambient);   // ambient light
        SetUniform(newShad, 5, sunDirection); // NEW: directional light
        SetUniform(newShad, 6, lightColor); // albedo (main color)
        SetUniform(newShad, 7, sunDirection2);
        SetUniform(newShad, 8, lightColor2);

        Draw(newShad, LoadSpear);
    }

    FreeGeometry(LoadSpear);
    FreeShader(newShad);

    Window.Term();

    return 0;
}