#ifndef Toon_program_h
#define Toon_program_h
#include "glclass/GLProgram.h"
#include "math/Matrix.hpp"

class GLSurface;
class ToonProgram : public GLProgram
{
public:

	void init();

	void useWith(GLSurface* surface, mat4& translation);

protected:

	GLProgram m_toonLightProgram;
	GLAttribute* m_toonPosition;
	GLAttribute* m_toonNormal;
	GLAttribute* m_toonDiffuseMaterial;

	GLUniform* m_toonProjection;
	GLUniform* m_toonModelview;
	GLUniform* m_toonNormalMatrix;

	GLUniform* m_toonLightPosition;
	GLUniform* m_toonAmbientMaterial;
	GLUniform* m_toonSpecularMaterial;
	GLUniform* m_toonShininess;
};

#endif