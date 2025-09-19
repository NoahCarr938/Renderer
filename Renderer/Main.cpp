#include "Context.h"
#include "Renderer.h"

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

    const char * basicVert =
        "#version 430 core\n"
        "layout (location = 0) in vec4 position;\n"
        "layout (location = 1) in vec4 colors;\n"
        "out vec4 outcolors;\n"
        "void main() { outcolors = colors; gl_Position = position; }";

    const char * basicFrag =
        "#version 430 core\n"
        "out vec4 vertColor;\n"
        "in vec4 outcolors;\n"
        "void main() { vertColor = outcolors; }";

    Shader basicShad = MakeShader(basicVert, basicFrag);


    while (!Window.ShouldClose())
    {
        // update logic here
        Window.Tick();

        // draw logic goes here
        Window.Clear();

        // draw the red triangle
        Draw(basicShad, basicTriangleGeo);
    }


    Window.Term();

    return 0;
}