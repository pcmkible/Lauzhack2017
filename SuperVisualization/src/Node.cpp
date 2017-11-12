#include "brate/Node.h"
#include "brate/linmath.h"

Node::Node(GLuint *pProgram): Element(pProgram){
    color = std::vector<float>{0.2f, 0.6f, 0.1f};
    /*
    Vertex vertices[3] = {
        { -0.6f, -0.4f, 1.f, 0.f, 0.f },
        {  0.6f, -0.4f, 0.f, 1.f, 0.f },
        {   0.f,  0.6f, 0.f, 0.f, 1.f }
    };
    */
    Vertex vertices[nSides + 2];
    vertices[0] = {0.0f, 0.0f, 0.5f, 0.5f};

    for(int i = 0; i <= nSides; i++){
        float angle = ( (float) i ) / nSides * 2 * 3.141592;
        vertices[i+1] = {(float) cos(angle) * radius, (float) sin(angle) * radius, (float) cos(angle)*0.5f + 0.5f, (float) sin(angle) * 0.5f + 0.5f};
    }

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

    firstDrawn = true;
}

void Node::draw(){
    if(firstDrawn){
        startTime = (float) glfwGetTime();
        firstDrawn = false;
    }

    // Tell the texture uniform sampler to use this texture in the shader by binding to texture unit 0.
    glBindTexture(GL_TEXTURE_2D, tex);
    glUniform1i(texSampler, 0);

    mat4x4 m;
    mat4x4_identity(m);
    mat4x4_rotate_Z(m, m, (float) glfwGetTime() - startTime);
    glUniformMatrix4fv(m_location, 1, GL_FALSE, (const GLfloat*) m);
    glUniform3fv(vcol_location, 3, (const GLfloat*) &color[0]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, nSides+2);
}