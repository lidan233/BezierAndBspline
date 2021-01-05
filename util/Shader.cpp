//
// Created by 李源 on 2020-05-25.
//

#include "Shader.h"

inline void checkCompileErrors(GLuint shader,std::string type)
{
    GLint success ;
    GLchar infoLog[1024] ;
    if(type!="PROGRAM")
    {
        glGetShaderiv(shader,GL_COMPILE_STATUS,&success) ;
        if(!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }else{
        glGetShaderiv(shader,GL_LINK_STATUS,&success) ;
        if(!success)
        {
            glGetShaderInfoLog(shader,1024,NULL,infoLog) ;
            std::cout<< "ERROR::PROGRAM_LINKING_ERROR of type: "<<type <<" \n"<<infoLog <<"\n -- ---------------------------------- --"<<std::endl ;

        }
    }
}
Shader::Shader(const char* vertexShaderFile,const char* fragmentShaderFile,const char* geometryPath )
{
    std::string vertexCode ;
    std::string fragmentCode ;
    std::string geometryCode ;

    std::ifstream vShaderFile ;
    std::ifstream fShaderFile ;
    std::ifstream gShaderFile ;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit) ;
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit) ;
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit) ;

    try{
        vShaderFile.open(vertexShaderFile) ;
        fShaderFile.open(fragmentShaderFile) ;
        std::stringstream vShaderStream,fShaderStream ;

        vShaderStream <<vShaderFile.rdbuf() ;
        fShaderStream <<fShaderFile.rdbuf() ;

        vShaderFile.close() ;
        fShaderFile.close() ;

        vertexCode = vShaderStream.str() ;
        fragmentCode = fShaderStream.str() ;

        if(geometryPath != nullptr)
        {
            gShaderFile.open(geometryPath) ;
            std::stringstream gShaderstream ;
            gShaderstream<<gShaderFile.rdbuf() ;
            gShaderFile.close() ;
            geometryCode = gShaderstream.str() ;

        }

    }catch(std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" <<std::endl ;
    }

    const char* vShaderCode = vertexCode.c_str() ;
    const char* fShaderCode = fragmentCode.c_str() ;

    unsigned int vertex,fragment ;
    vertex = glCreateShader(GL_VERTEX_SHADER) ;
    glShaderSource(vertex,1,&vShaderCode,NULL) ;
    glCompileShader(vertex) ;
    checkCompileErrors(vertex,"VERTEX") ;

    fragment = glCreateShader(GL_FRAGMENT_SHADER) ;
    glShaderSource(fragment,1,&fShaderCode,NULL) ;
    glCompileShader(fragment) ;
    checkCompileErrors(fragment,"FRAGMENT") ;

    unsigned int geometry ;
    if(geometryPath != nullptr)
    {
        const char* gShaderCode = geometryCode.c_str() ;
        geometry = glCreateShader(GL_GEOMETRY_SHADER) ;
        glShaderSource(geometry,1,&gShaderCode,NULL) ;
        glCompileShader(geometry) ;
        checkCompileErrors(geometry,"GEOMETRY") ;

    }

    ID = glCreateProgram() ;
    glAttachShader(ID,vertex) ;
    glAttachShader(ID,fragment) ;
    if(geometryPath!= nullptr)
    {
        glAttachShader(ID,geometry) ;
    }
    glLinkProgram(ID) ;
    checkCompileErrors(ID,"PROGRAM") ;
    glDeleteShader(vertex) ;
    glDeleteShader(fragment) ;
    if(geometryPath!= nullptr)
    {
        glDeleteShader(geometry) ;
    }



}

Shader::Shader(const char* vertexShaderFile,const char * fragmentShaderFile)
{
    std::string vertexCode;
    std::string fragCode ;
    std::ifstream vertexFile ;
    std::ifstream fragFile ;

//    vertexFile.exceptions(std::ifstream::badbit||std::ifstream::failbit) ;
//    fragFile.exceptions(std::ifstream::badbit||std::ifstream::failbit) ;

    try{
        vertexFile.open(vertexShaderFile) ;
        fragFile.open(fragmentShaderFile) ;
        std::stringstream ssvert,ssfrag ;
        ssvert << vertexFile.rdbuf() ;
        ssfrag << fragFile.rdbuf() ;

        vertexFile.close() ;
        fragFile.close() ;

        vertexCode = ssvert.str() ;
        fragCode = ssfrag.str() ;


    } catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragCode.c_str();

    unsigned int vertex,fragment ;
    int success ;
    char loginfo[512] ;

    vertex = glCreateShader(GL_VERTEX_SHADER) ;
    glShaderSource(vertex,1,&vShaderCode,NULL) ;
    glCompileShader(vertex) ;
    glGetShaderiv(vertex,GL_COMPILE_STATUS,&success) ;

    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, loginfo);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << loginfo << std::endl;
    }



    fragment = glCreateShader(GL_FRAGMENT_SHADER) ;
    glShaderSource(fragment,1,&fShaderCode,NULL ) ;
    glCompileShader(fragment) ;
    glGetShaderiv(fragment,GL_COMPILE_STATUS,&success) ;

    if(!success)
    {
        glGetShaderInfoLog(fragment,512,NULL,loginfo) ;
        std::cout << "ERROR::SHADER::Fragment::COMPILATION_FAILED\n" << loginfo << std::endl;
    }


    ID = glCreateProgram();
    glAttachShader(ID,vertex) ;
    glAttachShader(ID,fragment) ;
    glLinkProgram(ID) ;

// 打印连接错误（如果有的话）
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, loginfo);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << loginfo << std::endl;
    }

    glDeleteShader(vertex) ;
    glDeleteShader(fragment) ;



}
void Shader:: use()
{
    glUseProgram(ID) ;
}
void Shader:: setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID,name.c_str()),(int)value) ;

}
void Shader::setInt(const std::string &name, int value) const{
    glUniform1i(glGetUniformLocation(ID,name.c_str()),(int)value) ;
}
void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3Array(const std::string &name, glm::vec3 *vecs,int size) const {
    glUniform3fv(glGetUniformLocation(ID,name.c_str()),size,&vecs[0][0]) ;
}

void Shader::setMat4(const std::string &name,glm::mat4 value ) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID,name.c_str()),1, GL_FALSE,glm::value_ptr(value)) ;
}

void Shader::setVec3(const std::string &name,glm::vec3 value ) const
{
    glUniform3f(glGetUniformLocation(ID,name.c_str()),value[0],value[1],value[2]);
}
void Shader::setVec3(const std::string &name,float a ,float b,float c ) const
{
    glUniform3f(glGetUniformLocation(ID,name.c_str()),a,b,c);
}
void Shader::deleteShader()
{
    glDeleteProgram(ID) ;
}