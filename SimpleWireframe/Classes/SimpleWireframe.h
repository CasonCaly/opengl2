#ifndef HELLO_CONE_H
#define HELLO_CONE_H
#include <vector>
using namespace std;
#include "glclass/GLApp.h"
#include "glclass/GLBuffer.h"
#include "Interfaces.hpp"
#include "ParametricEquations.hpp"
#include "glsurface/GLCone.h"
#include "glsurface/GLSphere.h"

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

class SimpleWireframe : public GLApp{
    
public:
    
    SimpleWireframe();
    
    void init();
    
    void render() override;

	void onTouchBegan(float x, float y) override;

	void onTouchMove(float x, float y) override;

	void onTouchEnd(float x, float y) override;

private:
    
    void applyOrtho(float maxX, float maxY);
    
    void applyRotation(float degrees);

private:
    
    GLCone* m_cone;
    GLSphere* m_sphere;
    GLSurface* m_glSurface;
    
    GLUniform* m_projectionUniform;
    GLUniform* m_modelviewUniform;
    GLAttribute* m_positionSlot;
    GLAttribute* m_colorSlot;
    
    GLBuffer m_vertexBuffer;
    GLBuffer m_indexBuffer;
    mat4 m_translation;
    
    float m_previousX;
    float m_previousY;
    float m_lastRotationAngle;
    float m_rotationAngle;
};
#endif
