#ifndef INCLUDE_DISPLAY_H
#define INCLUDE_DISPLAY_H

#ifdef _WIN32
    #define APIENTRY __stdcall
#endif
#include "glad/glad.h"

#include <GLFW/glfw3.h>
#include <thread>
#include <string>

class Display{
public:
    static Display* getInstance();
    void draw();
    void pressed(float x, float y);
    const int width, height;
private:
    static Display* instance;
    Display(int width, int height);
    GLFWwindow* window;
    float startTime;

    std::string vertexShaderText;
    std::string fragmentShaderText;

    Node *actualNode;
};

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

#endif //INCLUDE_DISPLAY_H