#ifndef INCLUDE_NODE_H
#define INCLUDE_NODE_H

#include <vector>
#include "brate/Element.h"
#include "brate/txtresources.h"

class Node: public Element{
public:
    Node(GLuint *pProgram, std::vector<float> pPosition, bool isActual, float amount);
    void draw();
    bool gotClicked(float x, float y);
    int checkChildrenClicked(float x, float y);
    void toggle(void);
    bool showingKids;
private:
    GLuint vertex_buffer, tex, texSampler;
    bool firstDrawn;
    float startTime;

    std::vector<Node *> children;

    std::vector<float> position;

    StringWithLimits *title;

    float scale;
    std::vector<float> color;
    float money;
    bool triggeredChange = false;
    bool isInTransition = false;
    float transitionTime;
    float threshold = 1.0f;
};

#endif //INCLUDE_NODE_H