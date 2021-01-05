//
// Created by 李源 on 2020-05-25.
//

#ifndef LEARNOPENGL_SHADER_H
#define LEARNOPENGL_SHADER_H


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string.h>
#include <sstream>
#include <iostream>
#include <fstream>




class Shader {
public :
    unsigned int ID ;
    Shader(){} ;
    Shader(const char* vertexShaderFile,const char * fragmentShaderFile) ;
    Shader(const char* vertexShaderFile,const char* fragmentShaderFile,const char* geometryPath) ;
    void use() ;
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name,glm::mat4 value ) const ;
    void setVec3Array(const std::string &name,glm::vec3* vecs,int size) const ;
    void setVec3(const std::string &name,glm::vec3 value ) const ;
    void setVec3(const std::string &name,float a,float b,float c) const ;
    void deleteShader();

};


#endif //LEARNOPENGL_SHADER_H
