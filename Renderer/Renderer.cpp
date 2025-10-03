#include "Renderer.h"
#include "glm/gtc/type_ptr.hpp"
#include "Utils.h"
#include <vector>
#define STB_IMAGE_IMPLEMENTATION 1
#include <stb_image.h>
#define TINYOBJLOADER_IMPLEMENTATION 1
#include "tiny_obj_loader.h"
#include <iostream>




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

        // position location 0
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        //normals location 1
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normals));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UVs));
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

    
    Geometry LoadGeometry(const char* filePath)
    {
        using namespace tinyobj;

        // contains all data for all vertex attributes loaded from the file
        attrib_t vertexAttributes;

        // enumeration of all shapes in obj file
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string error;
        std::string warn;

        bool success = LoadObj(&vertexAttributes, &shapes, &materials, &warn, &error, filePath);

        if (!error.empty())
        {
            fprintf(stderr, "%s", error.c_str());
        }
        if (!success || shapes.size() < 1)
        {
            return {};
        }

        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        // Get mesh data
        size_t offSet = 0;
        for (GLsizei i = 0; i < shapes[0].mesh.num_face_vertices.size(); ++i)
        {
            // number of vertices for current face
            GLsizei faceVertices = shapes[0].mesh.num_face_vertices[i];

            assert(faceVertices == 3 && "Faces must be provided in triangles");

            // iterate over vertices used to form current face
            for (unsigned char j = 0; j < faceVertices; ++j)
            {
                tinyobj::index_t idx = shapes[0].mesh.indices[offSet + j];

                tinyobj::real_t vx = vertexAttributes.vertices[3 * idx.vertex_index + 0];
                tinyobj::real_t vy = vertexAttributes.vertices[3 * idx.vertex_index + 1];
                tinyobj::real_t vz = vertexAttributes.vertices[3 * idx.vertex_index + 2];

                tinyobj::real_t nx = vertexAttributes.normals[3 * idx.normal_index + 0];
                tinyobj::real_t ny = vertexAttributes.normals[3 * idx.normal_index + 1];
                tinyobj::real_t nz = vertexAttributes.normals[3 * idx.normal_index + 2];

                tinyobj::real_t tx = vertexAttributes.texcoords[2 * idx.texcoord_index + 0];
                tinyobj::real_t ty = vertexAttributes.texcoords[2 * idx.texcoord_index + 1];

                vertices.push_back(Vertex { {vx, vy, vz, 1}, { nx, ny, nz, 1}, {tx, ty} });
                indices.push_back(faceVertices * i + j);
            }
            offSet += faceVertices;
        }

        assert(vertices.size() <= std::numeric_limits<GLsizei>::max());
        assert(indices.size() <= std::numeric_limits<GLsizei>::max());

        return MakeGeometry(&vertices[0], (GLsizei)vertices.size(), &indices[0], (GLsizei)shapes[0].mesh.indices.size());
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
        assert(CheckSubShader(vert));
        assert(CheckSubShader(frag));

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

    Texture LoadTexture(const char* imagePath)
    {
        int imageWidth, imageHeight, imageFormat;
        unsigned char* rawPixelData = nullptr;

        Texture newTexture = {};

        stbi_set_flip_vertically_on_load(true);

        rawPixelData = stbi_load(imagePath, &imageWidth, &imageHeight, &imageFormat, STBI_default);

        assert(rawPixelData != nullptr && "Image failed to load.");
        newTexture = MakeTexture(imageWidth, imageHeight, imageFormat, rawPixelData);
        stbi_image_free(rawPixelData);

        return newTexture;
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

    void SetUniform(const Shader& shad, GLuint location, const glm::vec3& value)
    {
        glProgramUniform3fv(shad.Program, location, 1, glm::value_ptr(value));
    }

    /**
     * Checks if shader program is linking correctly.
     *
     * Linker errors can occur if a later shader is expecting an input
     * that an earlier shader did not output.
     *
     * @return True if good, otherwise false
     */
    bool CheckShader(const Shader& Shad)
    {
        GLint status = GL_FALSE;
        glGetProgramiv(Shad.Program, GL_LINK_STATUS, &status);

        if (status != GL_TRUE)
        {
            GLint logLength = 0;
            glGetProgramiv(Shad.Program, GL_INFO_LOG_LENGTH, &logLength);
            GLchar* log = new GLchar[logLength];
            glGetProgramInfoLog(Shad.Program, logLength, 0, log);

            std::cerr << log << std::endl;

            delete[] log;
        }

        return status == GL_TRUE;
    }

    /**
       * Checks if shader (not the shader PROGRAM), such as Vertex or Fragment
       * shader is compiled correctly
       *
       * @return True if good, otherwise false
       */
    bool CheckSubShader(GLuint subshader)
    {
        GLint status = GL_FALSE;
        glGetShaderiv(subshader, GL_COMPILE_STATUS, &status);

        if (status != GL_TRUE)
        {
            GLint logLength = 0;
            glGetShaderiv(subshader, GL_INFO_LOG_LENGTH, &logLength);
            GLchar* log = new GLchar[logLength];
            glGetShaderInfoLog(subshader, logLength, 0, log);

            std::cerr << log << std::endl;

            delete[] log;
        }

        return status == GL_TRUE;
    }

}