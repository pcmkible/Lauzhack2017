#ifndef INCLUDE_NODE_H
#define INCLUDE_NODE_H

#include <vector>
#include "brate/Element.h"

class Node: public Element{
public:
    Node(GLuint *pProgram);
    void draw();
private:
    GLuint vertex_buffer, tex, texSampler;
    bool firstDrawn;
    float startTime;

    float radius = 0.5f;
    std::vector<float> color;
    static const int nSides = 50;
};

#endif //INCLUDE_NODE_H