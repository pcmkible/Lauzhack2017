#include "brate/Display.h"
#include<GLFW/glfw3.h>
#include<stdexcept>

#include "brate/linmath.h"

#include "brate/Node.h"

Display* Display::instance = nullptr;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        Display* mDisplay = Display::getInstance();
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        mDisplay->pressed(((float) (2*xpos-mDisplay->width))/mDisplay->width, ((float) (2*ypos-mDisplay->height))/mDisplay->height);
    }
}

Display* Display::getInstance(){
    if(instance == nullptr){
        instance = new Display(1920, 1080);
    }
    return instance;
}

Display::Display(int width, int height): width(width), height(height){
    vertexShaderText = "uniform mat4 VP;\n"
        "uniform mat4 M;\n"
        "uniform vec3 vCol;\n"
        "attribute vec2 vPos;\n"
        "attribute vec2 vertexUV;\n"
        "varying vec3 color;\n"
        "varying vec2 UV;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = VP * M * vec4(vPos, 0.0, 1.0);\n"
        "    color = vCol;\n"
        "    UV = vertexUV;\n"
        "}\n";

    fragmentShaderText = "varying vec3 color;\n"
        "uniform sampler2D myTextureSampler;"
        "varying vec2 UV;"
        "void main()\n"
        "{\n"
        //"    gl_FragColor = vec4(color, 1.0);\n"
        "   gl_FragColor = texture( myTextureSampler, UV );\n"
        "}\n";
}

void Display::pressed(float x, float y){
    
}

void Display::draw(){
    GLuint vertex_shader, fragment_shader, program;
    GLint mvp_location;

    if (!glfwInit())
        throw std::invalid_argument("Something went wrong!");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(width, height, "Credit Swiss is the best", NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw std::invalid_argument("Nooooo!");
    }
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);
    // NOTE: OpenGL error checks have been omitted for brevity
    
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    const char *vst = vertexShaderText.c_str();
    glShaderSource(vertex_shader, 1, &vst, NULL);
    glCompileShader(vertex_shader);
    
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fst = fragmentShaderText.c_str();
    glShaderSource(fragment_shader, 1, &fst, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    mvp_location = glGetUniformLocation(program, "VP");

    float ratio;
    ratio = width / (float) height;
    glViewport(0, 0, width, height);
    mat4x4 m, p, mvp;
    mat4x4_identity(m);
    mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    mat4x4_mul(mvp, p, m);

    startTime = (float) glfwGetTime();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    Node *mNode = new Node(&program);

    while (!glfwWindowShouldClose(window)) {
        
        glClear(GL_COLOR_BUFFER_BIT);
    
        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
        
        mNode->draw();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}