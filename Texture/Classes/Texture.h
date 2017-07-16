#ifndef Texture_H
#define Texture_H
#include <vector>
using namespace std;

#include "math/Quaternion.hpp"
#include "glclass/GLApp.h"
#include "SimpleProgram.h"
#include "PixelLightProgram.h"
#include "ToonProgram.h"
#include "TextureProgram.h"

class GLSurface;
class GLUniform;
class GLAttribute;

struct Animation {
	bool Active;
	float Elapsed;
	float Duration;
};

class Texture : public GLApp{
    
public:
    
	Texture();
    
    void init();
    
    void render() override;

	void onTouchBegan(float x, float y) override;

	void onTouchMove(float x, float y) override;

	void onTouchEnd(float x, float y) override;

	void onMouseScroll(double x, double y) override;

	void initProgram() override;

private:
    
    void applyOrtho(float maxX, float maxY);
    
    void applyRotation(float degrees);

	vec3 mapToSphere(vec2 touchPoint);

	int mapToButton(vec2 touchpoint);

	void updateSurface();

	void renderSurface();

private:
    
    GLSurface* m_showedSurface;
    
	vector<GLSurface*> m_surfaces;
    
protected:

	SimpleProgram m_simpleProgram;
	PixelLightProgram m_pixelLightProgram;
	ToonProgram m_toonProgram;
    TextureProgram m_texProgram;
    
protected:

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
