#include "Renderer.h"
#include "glm/gtc/type_ptr.hpp"
#include "Utils.h"
#define STB_IMAGE_IMPLEMENTATION 1
#include <stb_image.h>

namespace aie
{

    Geometry MakeGeometry(const Vertex* const Verts, GLsizei VertCount, const GLuint* const Indicies, GLsizei IndexCount)
    {

        Geometry NewGeo = {};
        NewGeo.Size = IndexCount;

        glGenVertexArrays(1, &NewGeo.Vao);
        glGenBuffers(1, &NewGeo.Vbo);
        glGenBuffers(1, &NewGeo.Ibo);

        glBindVertexArray(NewGeo.Vao);
        glBindBuffer(GL_ARRAY_BUFFER, NewGeo.Vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NewGeo.Ibo);

        // buffer vertex data
        glBufferData(GL_ARRAY_BUFFER, VertCount * sizeof(Vertex), Verts, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexCount * sizeof(GLsizei), Indicies, GL_STATIC_DRAW);

        // describe vertex data
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UVs));
        // or 1, 4

        //unbinding buffers to prevent overwriting the information accidentally
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        return NewGeo;
    }

    void FreeGeometry(Geometry& Geo)
    {
        // telling opengl to delete the buffers we have created and zero out the geomertry object to prevent referring to the de allocated handles.
        // This will free up VRAM on the GPU
        glDeleteBuffers(1, &Geo.Vbo);
        glDeleteBuffers(1, &Geo.Ibo);
        glDeleteVertexArrays(1, &Geo.Vao);

        Geo = {};
    }

    Shader LoadShader(const char* VertPath, const char* FragPath)
    {
        std::string VertSource = DumpToString(VertPath);
        std::string FragSource = DumpToString(FragPath);
        return MakeShader(VertSource.c_str(), FragSource.c_str());
    }

    Shader MakeShader(const char* vertSource, const char* fragSource)
    {
        // make the shader object
        Shader newShad = {};
        newShad.Program = glCreateProgram();

        // create shaders
        GLuint vert = glCreateShader(GL_VERTEX_SHADER);
        GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);

        // We have a shader program and two shaders but they are not attached to each other or programmed yet
        // Before we attach the shaders to the program, let's load the source code for our vertex and fragment shader and compile them.

        // compile shaders
        glShaderSource(vert, 1, &vertSource, 0);
        glShaderSource(frag, 1, &fragSource, 0);
        glCompileShader(vert);
        glCompileShader(frag);

        // Now the shader should be able to compile, now attach it to the shader program and link it.

        // attach shaders
        glAttachShader(newShad.Program, vert);
        glAttachShader(newShad.Program, frag);

        // Link shaders
        glLinkProgram(newShad.Program);

        // It is now ready to use, we no longer need out og vert and frag shaders (they are compiled and packaged into the shader program) so we can delete them

        // delete shaders
        glDeleteShader(vert);
        glDeleteShader(frag);

        // return the shader object
        return newShad;
    }

    // Zeroing out the shader object to prevent us from referring to the shader program after it's been freed.
    void FreeShader(Shader& shad)
    {
        glDeleteProgram(shad.Program);
        shad = {};
    }

    Texture MakeTexture(unsigned Width, unsigned Height, unsigned Channels, const unsigned char* Pixels)
    {
        // create our return object
        Texture RetVal = { 0, Width, Height, Channels };
        // setup the encoding in a way that open gl understands
        GLenum OGLFormat = GL_RED;
        switch (Channels)
        {
        case 1:
            OGLFormat = GL_RED;     // monochrome
            break;
        case 2:
            OGLFormat = GL_RG;      // Not many uses for a 2 channel solution
            break;
        case 3:
            OGLFormat = GL_RGB;     // color or data (specificallt directions or offsets)
            break;
        case 4:
            OGLFormat = GL_RGBA;    // color or data (that needs a fourth channel)
            break;
        default:
            assert(false && "Unknown channel configuration!");
        }
        // create a texture object on open gl and bind it
        glGenTextures(1, &RetVal.Handle);
        glBindTexture(GL_TEXTURE_2D, RetVal.Handle);
        // buffer in the texture data
        glTexImage2D(GL_TEXTURE_2D,
            0,
            OGLFormat,
            Width,
            Height,
            0,
            OGLFormat,
            GL_UNSIGNED_BYTE,
            Pixels);
        // describe the data
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);

        // return
        return RetVal;
    }

    Texture LoadTexture(const char* ImagePath)
    {
        // create some variables to be filled in by stb
        int ImageWidth = 0;
        int ImageHeight = 0;
        int ImageFormat = 0;

        unsigned char* RawPixelData = nullptr;

        // create our return value
        Texture RetVal = {};
        // load the texture
        // aling with bottom left origin
        stbi_set_flip_vertically_on_load(true);

        RawPixelData = stbi_load(ImagePath,
            &ImageWidth,
            &ImageHeight,
            &ImageFormat,
            STBI_default);

        assert(RawPixelData != nullptr && "Failed to load image!");

        RetVal = MakeTexture(ImageWidth, ImageHeight, ImageFormat, RawPixelData);

        stbi_image_free(RawPixelData);

        // return the texture
        return RetVal;
    }

    void FreeTexture(Texture& tex)
    {
        glDeleteTextures(1, &tex.Handle);
        // zero out to prevent accidental reuse
        tex = {};
    }

    void Draw(const Shader& shad, const Geometry& geo)
    {
        // bind the shader program
        glUseProgram(shad.Program);
        // bind the VAO
        glBindVertexArray(geo.Vao);

        // draw the object
        glDrawElements(GL_TRIANGLES, geo.Size, GL_UNSIGNED_INT, 0);
    }

    void SetUniform(const Shader& shad, GLuint location, const glm::mat4& value)
    {
        glProgramUniformMatrix4fv(shad.Program, location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void SetUniform(const Shader& shad, GLuint location, float value)
    {
        glProgramUniform1fv(shad.Program, location, 1, &value);
    }

    void SetUniform(const Shader& shad, GLuint location, const Texture& tex, int TextureSlot)
    {
        // Specify the texture slot we want to activate
        glActiveTexture(GL_TEXTURE0 + TextureSlot);
        // bind the texture to that slot
        glBindTexture(GL_TEXTURE_2D, tex.Handle);
        // specify that texture slot as the value for that uniform
        glProgramUniform1i(shad.Program, location, TextureSlot);
    }

}