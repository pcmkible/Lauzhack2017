#include "brate/Node.h"
#include "brate/linmath.h"
#include <cstdlib>

Node::Node(GLuint *pProgram, std::vector<float> pPosition, bool isActual, float amount): Element(pProgram){
    color = std::vector<float>{0.2f, 0.6f, 0.1f};
    
    money = amount;
    if(isActual){
        scale = 0.7f;
    } else {
        scale = 0.5f;
    }
    showingKids = false; //Always

    Vertex vertices[5] = {
        { -0.5f, -0.5f, 0.0f, 1.0f},
        { 0.5f, -0.5f, 1.0f, 1.0f},
        { 0.5f,  0.5f, 1.0f, 0.0f},
        { -0.5f, 0.5, 0.0f, 0.0f},
        { -0.5f, -0.5f, 0.0f, 0.0f}
    };
    
    position = pPosition;

    title = new StringWithLimits("Something", new float[2]{0,0}, scale, 2, 2, 0, program);

    /*
    Vertex vertices[nSides + 2];
    vertices[0] = {0.0f, 0.0f, 0.5f, 0.5f};

    for(int i = 0; i <= nSides; i++){
        float angle = ( (float) i ) / nSides * 2 * 3.141592;
        vertices[i+1] = {(float) cos(angle) * radius, (float) sin(angle) * radius, (float) cos(angle)*0.5f + 0.5f, (float) sin(angle) * 0.5f + 0.5f};
    }
    */

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 4, (void*) 0);

    tex = LoadPNGImage("resources/sw.png");

    glEnableVertexAttribArray(uv_location);
    glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 4, (void*) (sizeof(float) * 2) );

    texSampler = glGetUniformLocation(*program, "myTextureSampler");
                          /*
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 5, (void*) (sizeof(float) * 2));
                          */

    if(isActual){
        int nChildren = rand()%6+5;
        int maxMoney = 0;
        for(int i = 0; i < nChildren; i++){
            float amount = 1024+rand()%128;
            children.push_back(new Node(program, std::vector<float>{0,0}, false, amount));
            if(amount > maxMoney){
                maxMoney = amount;
            }
        }
        for(int i = 0; i < children.size(); i++){
            children[i]->scale = children[i]->money / maxMoney * 0.4f;
            std::cout << children[i]->scale << std::endl;
        }
    }

    firstDrawn = true;
}

bool Node::gotClicked(float x, float y){
    if(isInTransition){
        return false;
    }
    if(x < position[0] + scale * 0.5f && x > position[0] - scale * 0.5f){
        if(y < position[1] + scale * 0.5f && y > position[1] - scale * 0.5f){
            return true;
        }
    }
    return false;
}

void Node::toggle(void){
    if(!isInTransition){
        triggeredChange = true;
        showingKids = !showingKids;
    }
}

int Node::checkChildrenClicked(float x, float y){
    if(!isInTransition){
        return -1;
    }
    for(int i = 0; i < children.size(); i++){
        if(children[i]->gotClicked(x, y)){
            return i;
        }
    }
    return -1;
}

void Node::draw(){
    if(firstDrawn){
        startTime = (float) glfwGetTime();
        firstDrawn = false;
    }

    if(triggeredChange){
        transitionTime = (float) glfwGetTime();
        isInTransition = true;
        triggeredChange = false;
    }

    if(isInTransition){
        float  progress = (float) glfwGetTime() - transitionTime;
        if(progress > threshold){
            isInTransition = false;
        } else {
            float radius = showingKids ? 0.8f * sin(progress / threshold * 3.141692 / 2) : 0.8 * sin((threshold - progress) / threshold * 3.141692 / 2) ;
            for(int i = 0; i < children.size(); i++){
                float angle = ((float) i ) / children.size() * 2 * 3.141592;
                children[i]->position[0] = radius * cos(angle);
                children[i]->position[1] = radius * sin(angle);
            }
        }
    }

    if(isInTransition || showingKids){
        for(int i = 0; i < children.size(); i++){
            children[i]->draw();
        }
    }

    // Tell the texture uniform sampler to use this texture in the shader by binding to texture unit 0.
    glBindTexture(GL_TEXTURE_2D, tex);
    glUniform1i(texSampler, 0);

    mat4x4 m;
    mat4x4_identity(m);
    mat4x4_translate_in_place(m, position[0], position[1], 0.0f);
    mat4x4_rotate_Z(m, m, (float) sin( (float) glfwGetTime() - startTime ) * 0.2f );
    mat4x4_scale_aniso(m, m , scale, scale, 1);
    glUniformMatrix4fv(m_location, 1, GL_FALSE, (const GLfloat*) m);
    glUniform3fv(vcol_location, 3, (const GLfloat*) &color[0]);
    //glDrawArrays(GL_TRIANGLE_FAN, 0, nSides+2);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 5);
}