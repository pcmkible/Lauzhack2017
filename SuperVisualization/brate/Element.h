#ifndef INCLUDE_ELEMENT_H
#define INCLUDE_ELEMENT_H

#ifdef _WIN32
    #define APIENTRY __stdcall
#endif
#include "glad/glad.h"

#include <GLFW/glfw3.h>
#include "brate/lodepng.h"
#include <string>
#include <iostream>

class Element{
public:
    Element(GLuint *pProgram){
        program = pProgram;

        vpos_location = glGetAttribLocation(*program, "vPos");
        vcol_location = glGetAttribLocation(*program, "vCol");
        uv_location = glGetAttribLocation(*program, "vertexUV");
        m_location = glGetUniformLocation(*program, "M");
    }
    struct Vertex{
        float x,y;
        float u,v;
    };
    virtual void draw() = 0;

    static GLuint LoadPNGImage(std::string fileName){
        std::vector<unsigned char> image; //the raw pixels
        unsigned width, height;

        //decode
        unsigned error = lodepng::decode(image, width, height, fileName);
        
        // Create one OpenGL texture
        GLuint textureID;
        glGenTextures(1, &textureID);

        // "Bind" the newly created texture : all future texture functions will modify this texture
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Give the image to OpenGL
        glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        return textureID;
    }
protected:
    GLuint *program;
    GLint vpos_location, vcol_location, m_location, uv_location;
};

#endif //INCLUDE_ELEMENT_H