#ifndef PixelLight_program_h
#define PixelLight_program_h
#include "glclass/GLProgram.h"
#include "math/Matrix.hpp"

class GLSurface;
class PixelLightProgram : public GLProgram
{
public:

	void init();

	void useWith(GLSurface* surface, mat4& translation);

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