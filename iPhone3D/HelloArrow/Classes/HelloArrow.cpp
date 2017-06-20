#include <cmath>
#include "HelloArrow.h"

#define STRINGIFY(A)  #A
#include "../Shaders/Simple.vert"
#include "../Shaders/Simple.frag"
#include "GLShader.h"
#include "GLProgram.h"

struct Vertex {
    float Position[2];
    float Color[4];
};

// Define the positions and colors of two triangles.
const Vertex Vertices[] = {
    {{-0.5, -0.866}, {1, 1, 0.5f, 1}},
    {{0.5, -0.866},  {1, 1, 0.5f, 1}},
    {{0, 1},         {1, 1, 0.5f, 1}},
    {{-0.5, -0.866}, {0.5f, 0.5f, 0.5f}},
    {{0.5, -0.866},  {0.5f, 0.5f, 0.5f}},
    {{0, -0.4f},     {0.5f, 0.5f, 0.5f}},
};

HelloArrow::HelloArrow(){
    m_desiredAngle = 0.0f;
    m_currentAngle = 0.0f;
}

void HelloArrow::init(){
    GLShader vertexShader;
    vertexShader.createVertex();
    vertexShader.compile(SimpleVertexShader);
    
    GLShader fragmentShader;
    fragmentShader.createFragment();
    fragmentShader.compile(SimpleFragmentShader);
    
    m_glProgram.create();
    m_glProgram.attachShader(vertexShader);
    m_glProgram.attachShader(fragmentShader);
    m_glProgram.link();
    m_glProgram.use();
    // Initialize the projection matrix.
    this->applyOrtho(2, 3);
    
    m_currentAngle = m_desiredAngle;
}

void HelloArrow::applyOrtho(float maxX, float maxY){
    float a = 1.0f / maxX;
    float b = 1.0f / maxY;
    float ortho[16] = {
        a, 0,  0, 0,
        0, b,  0, 0,
        0, 0, -1, 0,
        0, 0,  0, 1
    };
    
    GLUniform* projectionUniform = m_glProgram.getUniform("Projection");
    projectionUniform->matrix4fv( 1, 0, &ortho[0]);
}

void HelloArrow::applyRotation(float degrees){
    float radians = degrees * 3.14159f / 180.0f;
    float s = std::sin(radians);
    float c = std::cos(radians);
    float zRotation[16] = {
        c, s, 0, 0,
       -s, c, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    
    GLUniform* modelviewUniform = m_glProgram.getUniform("Modelview");
    modelviewUniform->matrix4fv(1, 0, &zRotation[0]);
}

void HelloArrow::render(){
    glClearColor(0.5f, 0.5f, 0.5f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    this->applyRotation(m_currentAngle);
    
    GLAttribute* positionSlot = m_glProgram.getAttribute("Position");
    GLAttribute* colorSlot = m_glProgram.getAttribute("SourceColor");
    
    positionSlot->enableVertexAttribArray();
    colorSlot->enableVertexAttribArray();
    
    GLsizei stride = sizeof(Vertex);
    const GLvoid* pCoords = &Vertices[0].Position[0];
    const GLvoid* pColors = &Vertices[0].Color[0];
    
    positionSlot->vertexAttribPointer(2, GL_FLOAT, GL_FALSE, stride, pCoords);
    colorSlot->vertexAttribPointer(4, GL_FLOAT, GL_FALSE, stride, pColors);
    
    GLsizei vertexCount = sizeof(Vertices) / sizeof(Vertex);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    
    positionSlot->disableVertexAttribArray();
    colorSlot->disableVertexAttribArray();
}

