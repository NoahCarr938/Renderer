#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
namespace aie
{
    struct Vertex
    {
        glm::vec4 Pos;
        glm::vec4 Color;
    };

    struct Geometry
    {
        GLuint Vao = 0, Vbo = 0, Ibo = 0;
        GLuint Size = 0;
    };

    struct Shader
    {
        GLuint Program;
    };

    Geometry MakeGeometry(const Vertex* const Verts, GLsizei VertCount, const GLuint* const Indicies, GLsizei IndexCount);
    void FreeGeometry(Geometry& Geo);
    Shader LoadShader(const char* VertPath, const char* FragPath);
    // Need both a shad and geometry to draw something so make a func for this.
    Shader MakeShader(const char* vertSource, const char* fragSource);
    void FreeShader(Shader& Shad);
    void Draw(const Shader& shad, const Geometry& geo);
    // Sets a uniform (like a global variable for the shader
    // @param Shad The Shader.
    // @param Location The Location (should match what's given
    
    void SetUniform(const Shader& shad, GLuint location, const glm::mat4& value);
    void SetUniform(const Shader& shad, GLuint location, float value);
}

