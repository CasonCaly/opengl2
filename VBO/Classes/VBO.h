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
#include "math/Quaternion.hpp"

struct Vertex {
    vec3 Position;
    vec4 Color;
};

struct Animation {
	bool Active;
	float Elapsed;
	float Duration;
};

struct Visual {
	vec3 Color;
	ivec2 LowerLeft;
	ivec2 ViewportSize;
	Quaternion Orientation;
};


class VBO : public GLApp{
    
public:
    
	VBO();
    
    void init();
    
    void render() override;

	void onTouchBegan(float x, float y) override;

	void onTouchMove(float x, float y) override;

	void onTouchEnd(float x, float y) override;

private:
    
    void applyOrtho(float maxX, float maxY);
    
    void applyRotation(float degrees);

	vec3 mapToSphere(vec2 touchPoint);

	int mapToButton(vec2 touchpoint);

	void updateSurface();

	void renderSurface();

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


	vec2 m_buttonSize;
	vec2 m_screenSize;
	int m_currentSurface;

	vec2 m_centerPoint;
	float m_trackballRadius;
	bool m_spinning;
	vec2 m_fingerStart;
	Quaternion m_previousOrientation;
	Quaternion m_orientation;
	int m_pressedButton;
	Animation m_animation;

};
#endif
