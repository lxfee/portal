#pragma once
#include "Global.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
    // pragram ID
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    Shader(const Shader& shader);
    void use();

    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, glm::mat4) const;
    void setVec3(const std::string&name, glm::vec3) const;
};

typedef shared_ptr<Shader> ShaderPtr;
