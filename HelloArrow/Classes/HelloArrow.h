#ifndef HELLO_ARROW_H
#define HELLO_ARROW_H
#include "GLProgram.h"

class HelloArrow {
    
public:
    
    HelloArrow();
    
    void init();
    
    void render();

private:
    
    void applyOrtho(float maxX, float maxY);
    
    void applyRotation(float degrees);

private:
    
    float m_desiredAngle;
    float m_currentAngle;    
    GLProgram m_glProgram;
};
#endif
