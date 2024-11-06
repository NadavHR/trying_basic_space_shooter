#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h> 
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


const std::string UNIFORM_PROJECTION_FIELD = "projection";

class Shader
{
public:
    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    // use/activate the shader
    void use();
    bool isCurrentlyUsed();
    // utility uniform functions
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
    void setTransform(const std::string &name, glm::mat4 value) const;
    void setVec3(const std::string &name, glm::vec3 value) const;
    void setProjection(glm::mat4 value) const;
    unsigned int getProgramID() const;

private:
    unsigned int mProgramID;
    static unsigned int currentlyUsedProgramID;
};

#endif