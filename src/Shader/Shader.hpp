#pragma once

#include "../Lib.hpp"

#include <iostream>

using namespace std;

/*
 * This class creates a shader by reading the vertex and fragment shader files given.
 */
class Shader {
private:
    // Shader id
    GLuint id;

    // This method check the vertex and fragment shader file seeking for compilation or linking errors.
    void checkCompileErrors(GLuint shader, string type) {
        GLint success;
        GLchar infoLog[1024];

        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << endl
                    << infoLog << endl
                    << "-- -------------------------------------------------- --" << endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << endl
                    << infoLog << endl
                    << "-- --------------------------------------------------- --" << endl;
            }
        }
    }

public:
    // This constructor creates a shader with the vertex and fragment shader file path.
    Shader(const char *vertexPath, const char *fragmentPath);

    Shader() {}

    // Retrieves the shader id.
    GLuint getId() { return this->id; }

    // Set the shader id.
    void setId(GLuint val) { this->id = val; }

    // This method use the shader.
    void use();

    void clearShader();

    //---List of method for uniform values---

    void setBool(const string &name, bool value);
    void setInt(const string &name, int value);
    void setFloat(const string &name, float value);
    void setVec2(const string &name, const vec2 &value);
    void setVec2(const string &name, float x, float y);
    void setVec3(const string &name, const vec3 &value);
    void setVec3(const string &name, float x, float y, float z);
    void setVec4(const string &name, const vec4 &value);
    void setVec4(const string &name, float x, float y, float z, float w);
    void setMat2(const string &name, const mat2 &mat);
    void setMat3(const string &name, const mat3 &mat);
    void setMat4(const string &name, const mat4 &mat);

    inline void clear() {
        glDeleteShader(this->id);
    }

    ~Shader() = default;
};
