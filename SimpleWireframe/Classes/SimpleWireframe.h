#ifndef HELLO_CONE_H
#define HELLO_CONE_H
#include <vector>
using namespace std;
#include "glclass/GLApp.h"
#include "glclass/GLBuffer.h"
#include "glsurface/GLCone.h"
#include "glsurface/GLSphere.h"
#include "glsurface/GLMobiusStrip.h"
#include "glsurface/GLTorus.h"
#include "glsurface/GLKleinBottle.h"
#include "glsurface/GLTrefoilKnot.h"

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

struct Visual {
	vec3 Color;
	ivec2 LowerLeft;
	ivec2 ViewportSize;
	Quaternion Orientation;
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
    
	vector<GLSurface*> m_surfaces;
    
    GLUniform* m_projectionUniform;
    GLUniform* m_modelviewUniform;
    GLAttribute* m_positionSlot;
    GLAttribute* m_colorSlot;
 
    mat4 m_translation;
    
    float m_previousX;
    float m_previousY;
    float m_lastRotationAngle;
    float m_rotationAngle;

	ivec2 m_buttonSize;
	ivec2 m_screenSize;
	int m_currentSurface;
};
#endif
