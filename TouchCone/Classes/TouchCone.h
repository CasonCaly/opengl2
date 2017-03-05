#ifndef TOUCH_CONE_H
#define TOUCH_CONE_H
#include "GLApp.h"

struct Vertex {
    vec3 Position;
    vec4 Color;
};

struct Animation {
    Quaternion Start;
    Quaternion End;
    Quaternion Current;
    float Elapsed;
    float Duration;
};

class TouchCone : public GLApp{
    
public:
    
    TouchCone();
    
    void init();
    
    void render();

    virtual void onTouchBegan(float x, float y);
    
    virtual void onTouchMove(float x, float y);
    
    virtual void onTouchEnd(float x, float y);
    
private:
    
    void applyOrtho(float maxX, float maxY);
    
    void applyRotation(float degrees);

private:
    
    GLfloat m_rotationAngle;
    GLfloat m_scale;
    GLuint m_vertexBuffer;
    GLuint m_indexBuffer;
    ivec2 m_pivotPoint;
    int m_diskIndexCount;
    int m_bodyIndexCount;
    float m_previousX;
    float m_previousY;
};
#endif
