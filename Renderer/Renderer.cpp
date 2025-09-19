#include "Renderer.h"
#include "glm/gtc/type_ptr.hpp"

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
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)16);

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

}