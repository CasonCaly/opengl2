#ifndef LIGHTING_H
#define LIGHTING_H
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

struct Animation {
	bool Active;
	float Elapsed;
	float Duration;
};

class Lighting : public GLApp{
    
public:
    
	Lighting();
    
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
