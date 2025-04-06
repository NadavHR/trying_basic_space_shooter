#include "../headers/shader.hpp"

unsigned int Shader::currentlyUsedProgramID = 0;

char * openRawSource(std::string fileName) {
    std::string outStr;
    std::ifstream read;
    read.open(fileName);
    std::string line;
    while (getline(read, line)){
        outStr += line + '\n';
    }
    read.close();

    char * out = (char *)(malloc(outStr.length() + 1));
    
    strcpy(out, outStr.c_str());

    return out;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath) 
{

    const char* vShaderCode = openRawSource(vertexPath);
    const char* fShaderCode = openRawSource(fragmentPath);

    // 2. compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];
    
    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };
    
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };
    
    // shader Program
    mProgramID = glCreateProgram();
    glAttachShader(mProgramID, vertex);
    glAttachShader(mProgramID, fragment);
    glLinkProgram(mProgramID);
    // print linking errors if any
    glGetProgramiv(mProgramID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(mProgramID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

void Shader::use() 
{ 
    glUseProgram(mProgramID);
    currentlyUsedProgramID = mProgramID;
} 

bool Shader::isCurrentlyUsed()
{

    return (currentlyUsedProgramID == mProgramID);
} 

void Shader::setBool(const std::string &name, bool value) const
{         
    glUniform1i(glGetUniformLocation(mProgramID, name.c_str()), (int)value); 
}

void Shader::setInt(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(mProgramID, name.c_str()), value); 
}

void Shader::setFloat(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(mProgramID, name.c_str()), value); 
} 

void Shader::setTransform(const std::string &name, glm::mat4 value) const {
    glUniformMatrix4fv(
         glGetUniformLocation(mProgramID, name.c_str()),
         1, 
         GL_FALSE,
         glm::value_ptr(value));
}

void Shader::setVec3(const std::string &name, glm::vec3 value) const {
    glUniform3fv(glGetUniformLocation(mProgramID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec2(const std::string &name, glm::vec2 value) const {
    glUniform2fv(glGetUniformLocation(mProgramID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setProjection(glm::mat4 value) const
{
    Shader::setTransform(UNIFORM_PROJECTION_FIELD, value);
}

unsigned int Shader::getProgramID() const{ 
    return mProgramID;
}


 

