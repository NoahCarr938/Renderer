#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
namespace aie
{
    struct Vertex
    {
        glm::vec4 Pos;
        glm::vec4 Normals;
        //glm::vec4 Color;
        glm::vec2 UVs;
    };

    struct Geometry
    {
        GLuint Vao = 0, Vbo = 0, Ibo = 0;
        GLuint Size = 0;
    };

    struct Shader
    {
        GLuint Program;    // ogl program names
    };

    struct Texture
    {
        GLuint Handle;  // ogl texture name
        unsigned Width, Height, Channels;
    };

    Geometry MakeGeometry(const Vertex* const Verts, GLsizei VertCount, const GLuint* const Indicies, GLsizei IndexCount);
    void FreeGeometry(Geometry& Geo);
    Geometry LoadGeometry(const char* filePath);

    Shader LoadShader(const char* VertPath, const char* FragPath);
    // Need both a shad and geometry to draw something so make a func for this.
    Shader MakeShader(const char* vertSource, const char* fragSource);
    void FreeShader(Shader& Shad);
    // creates a texture in open gl, width and height in pixels, channel is number of channels ex 1-r 2-rg 3-rgb 4-rgba, pixels pointer to pixel buffer
    // return the texture
    
    Texture MakeTexture(unsigned Width, unsigned Height, unsigned Channels, const unsigned char* Pixels);
    Texture LoadTexture(const char* ImagePath);
    // free a texture in open gl
    void FreeTexture(Texture& Tex);

    void Draw(const Shader& shad, const Geometry& geo);
    // Sets a uniform (like a global variable for the shader
    // @param Shad The Shader.
    // @param Location The Location (should match what's given
    
    void SetUniform(const Shader& shad, GLuint location, const glm::mat4& value);
    void SetUniform(const Shader& shad, GLuint location, float value);
    // assign a texture to a texture slot and then assign that slot number as the uniform value.
    // the shader, the location (should match what's given in the Shader source code
    // the actual texture object (containing the OGL name)
    // the slot that will be actually assigned to the shader
    void SetUniform(const Shader& shad, GLuint location, const Texture& tex, int TextureSlot);
    void SetUniform(const Shader& shad, GLuint location, const glm::vec3& value);
}

