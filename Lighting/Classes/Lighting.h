#ifndef LIGHTING_H
#define LIGHTING_H
#include <vector>
using namespace std;

#include "math/Quaternion.hpp"
#include "glclass/GLApp.h"

class GLSurface;
class GLUniform;
class GLAttribute;

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

	void initProgram() override;

private:
    
    void applyOrtho(float maxX, float maxY);
    
    void applyRotation(float degrees);

	vec3 mapToSphere(vec2 touchPoint);

	int mapToButton(vec2 touchpoint);

	void updateSurface();

	void initVertexAttributeAndUniform();

	void initPixelAttributeAndUniform();

	void renderSurface();

	void renderUseVertex(GLSurface* surface);

	void renderUsePixel(GLSurface* surface);

private:
    
	vector<GLSurface*> m_surfaces;
    
protected:

    GLUniform* m_projectionUniform;//投影矩阵
    GLUniform* m_modelviewUniform;//模型视图矩阵
	GLUniform* m_normalMatrixUniform;//法线矩阵
	GLUniform* m_lightPositionUniform;//光源位置
	GLUniform* m_ambientMaterialUniform;//环境光颜色值
	GLUniform* m_specularMaterialUniform;//镜面光材质
	GLUniform* m_shininessUniform;//发光?

    GLAttribute* m_positionSlot;//顶点属性
    GLAttribute* m_colorSlot;
	GLAttribute* m_normalSlot;//法线属性
	GLAttribute* m_diffuseMaterialSlot;//漫反射属性

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

protected:

	GLProgram m_pixelLightProgram;

	GLAttribute* m_pixelPosition;
	GLAttribute* m_pixelNormal;
	GLAttribute* m_pixelDiffuseMaterial;

	GLUniform* m_pixelProjection;
	GLUniform* m_pixelModelview;
	GLUniform* m_pixelNormalMatrix;

	GLUniform* m_pixelLightPosition;
	GLUniform* m_pixelAmbientMaterial;
	GLUniform* m_pixelSpecularMaterial;
	GLUniform* m_pixelShininess;
};
#endif
